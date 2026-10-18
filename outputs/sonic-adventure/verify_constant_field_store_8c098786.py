import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C098786
LITERAL = 0x8C0987E8
VALUE = 0x8C09865C

binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
assert binary[FUNCTION - BASE:FUNCTION - BASE + 6] == bytes.fromhex('18 d3 0b 00 34 14')
literal_value = struct.unpack_from('<I', binary, LITERAL - BASE)[0]
assert literal_value == VALUE
binary_sha256 = hashlib.sha256(binary).hexdigest()

library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'constant_field_store_8c098786.dll'
native = ctypes.CDLL(str(library_path)).sa_constant_field_store_8c098786
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(FUNCTION)
write_addresses = [0x8C098786, 0x8C09878A, LITERAL, VALUE,
                   0x8C700000, 0x8C75A120, 0x8CFFFFFC]
cases = 0
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    destination = (write_addresses[case] if case < len(write_addresses)
                   else 0x8C700000 + rng.randrange(0x8FFFFC))
    destination &= 0xFFFFFFFC
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = (destination - 0x10) & 0xFFFFFFFF
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C, 0x8C0987E8])
    state.pc = FUNCTION
    expected_ram = bytearray(ram)
    expected = State.from_buffer_copy(state)
    reference(expected_ram, expected)
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual))
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual) == bytes(expected), f'CPU state mismatch in case {case}'
    cases += 1

span = binary[FUNCTION - BASE:FUNCTION - BASE + 6]
report = {
    'cases_passed': cases,
    'comparison_level': 'Complete 16 MiB RAM and all GPRs, SR, PR, and return PC compared against the retail SH-4 interpreter.',
    'scope': 'The six-byte routine at 0x8c098786 loads the longword at PC-relative literal 0x8c0987e8 (0x8c09865c), stores it to R4+0x10 in the RTS delay slot, and returns through PR. Tests include code/literal aliases and the final aligned RAM word.',
    'limitations': ['Uses the project SH-4 interpreter rather than Dreamcast hardware or end-to-end gameplay.', 'The stored field meaning and higher-level ownership semantics remain unidentified.'],
    'binary_sha256': binary_sha256,
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'literal_address': hex(LITERAL),
    'literal_value': hex(literal_value),
    'source_sha256': hashlib.sha256((ROOT / 'source' / 'sa_constant_field_store_8c098786.c').read_bytes()).hexdigest(),
    'routines': [{
        'module': '1ST_READ.BIN',
        'load_address': hex(BASE),
        'address': hex(FUNCTION),
        'end_exclusive': hex(FUNCTION + len(span)),
        'retail_span_sha256': hashlib.sha256(span).hexdigest(),
        'cases_passed': cases,
    }],
}
(ROOT / 'analysis' / 'constant_field_store_8c098786_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{cases} constant field store full RAM/CPU comparisons passed')
