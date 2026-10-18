import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
GLOBAL_POINTER = 0x8C11204C
LITERAL = 0x8C01B4E0
LITERAL_VALUE = 0x8C11204C
ROUTINES = [
    (0x8C01B406, 9, bytes.fromhex('36 d3 32 60 0b 00 09 84'), 'sa_indirect_byte_get_9_8c01b406'),
    (0x8C01B40E, 6, bytes.fromhex('34 d3 32 60 0b 00 06 84'), 'sa_indirect_byte_get_6_8c01b40e'),
]

binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
assert struct.unpack_from('<I', binary, LITERAL - BASE)[0] == LITERAL_VALUE
for address, _, expected_span, _ in ROUTINES:
    assert binary[address - BASE:address - BASE + 8] == expected_span
binary_sha256 = hashlib.sha256(binary).hexdigest()

library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'indirect_byte_getters_8c01b406.dll'
library = ctypes.CDLL(str(library_path))
rng = random.Random(0x8C01B406)
target_choices = [0x8C6B6F4C, GLOBAL_POINTER - 9, GLOBAL_POINTER - 6,
                  0x8C700000, 0x8C77FFF0]
reports = []

for function, field_offset, expected_span, symbol in ROUTINES:
    native = getattr(library, symbol)
    native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    native.restype = None
    cases = 0
    for case in range(96):
        ram = bytearray(RAM_SIZE)
        ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
        target = target_choices[case % len(target_choices)] if case < 20 else 0x8C700000 + rng.randrange(0x80000)
        field_address = target + field_offset
        field_at = (field_address & 0x1FFFFFFF) - 0x0C000000
        ram[field_at] = rng.randrange(256)
        pointer_at = (GLOBAL_POINTER & 0x1FFFFFFF) - 0x0C000000
        struct.pack_into('<I', ram, pointer_at, target)

        state = State()
        state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        state.sr = rng.choice([0, 1, 0x70000000, 0xF0000001, rng.getrandbits(32)])
        state.pr = rng.choice([0x8CFED000, 0x8C010000, GLOBAL_POINTER, GLOBAL_POINTER + 4])
        state.pc = function

        expected_ram = bytearray(ram)
        expected = State.from_buffer_copy(state)
        reference(expected_ram, expected)
        actual_ram = bytearray(ram)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        actual = State.from_buffer_copy(state)
        native(ram_view, ctypes.byref(actual))
        assert actual_ram == expected_ram, f'RAM mismatch at {function:08x}, case {case}'
        assert bytes(actual) == bytes(expected), f'CPU mismatch at {function:08x}, case {case}'
        cases += 1

    span = binary[function - BASE:function - BASE + len(expected_span)]
    reports.append({
        'module': '1ST_READ.BIN',
        'load_address': hex(BASE),
        'address': hex(function),
        'end_exclusive': hex(function + len(expected_span)),
        'retail_span_sha256': hashlib.sha256(span).hexdigest(),
        'name': symbol,
        'field_offset': field_offset,
        'cases_passed': cases,
    })

report = {
    'cases_passed': sum(entry['cases_passed'] for entry in reports),
    'comparison_level': 'For each routine, complete 16 MiB RAM and all GPRs, SR, PR, and return PC compared against the retail SH-4 interpreter.',
    'scope': 'Two eight-byte indirect signed-byte getters load the pointer from 0x8c11204c and return sign-extended fields at offsets 9 and 6. Each was tested in 96 states, including fixed retail pointer data, pointer-to-pointer-slot alias cases, mapped variable targets, all signed-byte values by random sampling, and full CPU/RAM equality.',
    'limitations': ['Uses the project SH-4 interpreter rather than Dreamcast hardware or end-to-end gameplay.', 'Higher-level field names and game behavior remain unidentified.'],
    'binary_sha256': binary_sha256,
    'literal_address': hex(LITERAL),
    'literal_value': hex(LITERAL_VALUE),
    'source_sha256': hashlib.sha256((ROOT / 'source' / 'sa_indirect_byte_getters_8c01b406.c').read_bytes()).hexdigest(),
    'routines': reports,
}
(ROOT / 'analysis' / 'indirect_byte_getters_8c01b406_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{report['cases_passed']} indirect byte getter full RAM/CPU comparisons passed")
