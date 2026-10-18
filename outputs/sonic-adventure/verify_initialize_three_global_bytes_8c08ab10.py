import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C08AB10
LENGTH = 16
RAM_SIZE = 16 * 1024 * 1024
FIELD_R4 = 0x8C752BE1
FIELD_R5 = 0x8C752BE2
FIELD_ZERO = 0x8C752BE3
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('25 d2 00 e1 23 d3 25 d0 50 23 40 22 0b 00 10 20')
assert int.from_bytes(binary[0x8C08ABA4 - BASE:0x8C08ABA4 - BASE + 4], 'little') == FIELD_R5
assert int.from_bytes(binary[0x8C08ABA8 - BASE:0x8C08ABA8 - BASE + 4], 'little') == FIELD_R4
assert int.from_bytes(binary[0x8C08ABAC - BASE:0x8C08ABAC - BASE + 4], 'little') == FIELD_ZERO

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'initialize_three_global_bytes_8c08ab10.dll'))
native = library.sa_initialize_three_global_bytes_8c08ab10
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE, hex(address)
    return result

rng = random.Random(FUNCTION)
cases = []
for case in range(96):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    initial = rng.randbytes(8)
    start = offset(FIELD_R4)
    ram[start:start + len(initial)] = initial
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    reference(expected_ram, expected_state)
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual_state))
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch in case {case}'
    assert actual_ram[offset(FIELD_R4)] == (state.r[4] & 0xFF)
    assert actual_ram[offset(FIELD_R5)] == (state.r[5] & 0xFF)
    assert actual_ram[offset(FIELD_ZERO)] == 0
    cases.append({'case': case, 'r4': hex(state.r[4]), 'r5': hex(state.r[5]), 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_initialize_three_global_bytes_8c08ab10.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against the retail SH-4 interpreter',
    'scope': 'Exact three byte stores, literal pool pointer checks, and final zero store in the RTS delay slot.',
    'limitations': ['Semantic names for the three fields remain unknown.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'field_addresses': {'r4_byte': hex(FIELD_R4), 'r5_byte': hex(FIELD_R5), 'zero_byte': hex(FIELD_ZERO)},
    'cases': cases,
}
(PROJECT / 'analysis' / 'initialize_three_global_bytes_8c08ab10_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} three-byte initializer RAM/CPU comparisons passed')
