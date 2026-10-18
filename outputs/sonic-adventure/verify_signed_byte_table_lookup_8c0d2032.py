import ctypes
import hashlib
import json
import random
import struct
from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C0D2032
LENGTH = 8
LITERAL = 0x8C0D2060
binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'signed_byte_table_lookup_8c0d2032.dll'
library = ctypes.CDLL(str(library_path))
native = library.sa_signed_byte_table_lookup_8c0d2032
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None
rng = random.Random(FUNCTION)
reports = []
for case in range(512):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    input_address = 0x8C700000 + case * 0x100
    input_byte = case & 0xff if case < 256 else rng.getrandbits(8)
    ram[input_address - 0x8C000000] = input_byte
    signed_index = input_byte if input_byte < 0x80 else input_byte - 0x100
    if case < 256:
        table = [0x8C000080, 0x8CFFFF80][case // 128]
    elif case % 4 == 0:
        table = 0x8C010000
    elif case % 4 == 1:
        table = FUNCTION + 0x20
    elif case % 4 == 2:
        table = 0x8C710000 + (case - 256) * 0x100
    else:
        table = 0x8C422B44
    struct.pack_into('<I', ram, LITERAL - 0x8C000000, table)
    result_address = (table + signed_index) & 0xffffffff
    result_offset = (result_address & 0x1fffffff) - 0x0c000000
    assert 0 <= result_offset < RAM_SIZE, (case, hex(table), signed_index, hex(result_address))
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = input_address
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    expected_ram = bytearray(ram)
    expected = State.from_buffer_copy(state)
    reference(expected_ram, expected)
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual))
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual) == bytes(expected), (
        f'CPU mismatch in case {case}: '
        f'r={[(i, hex(actual.r[i]), hex(expected.r[i])) for i in range(16) if actual.r[i] != expected.r[i]]}, '
        f'sr={actual.sr:#x}/{expected.sr:#x}, pc={actual.pc:#x}/{expected.pc:#x}'
    )
    reports.append({'case': case, 'input_byte': hex(input_byte), 'table': hex(table),
                    'result_address': hex(result_address), 'ram_and_cpu_match': True})

report = {
    'module': '1ST_READ.BIN',
    'load_address': hex(BASE),
    'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'binary_sha256': hashlib.sha256(binary).hexdigest(),
    'source_sha256': hashlib.sha256((ROOT / 'source' / 'sa_signed_byte_table_lookup_8c0d2032.c').read_bytes()).hexdigest(),
    'cases_passed': len(reports),
    'comparison_level': 'Complete 16 MiB RAM and all GPRs, SR, PR, and return PC compared against the retail SH-4 interpreter.',
    'scope': 'Tests all 256 input byte values and signed indexing, return-byte sign extension, randomized table contents, literal-loaded pointers, code-backed reads, and accesses at both ends of mapped RAM.',
    'limitations': ['The lookup table semantics and valid input domain are not yet identified.'],
    'cases': reports,
}
(ROOT / 'analysis' / 'signed_byte_table_lookup_8c0d2032_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{len(reports)} signed byte table lookup full RAM/CPU comparisons passed")
