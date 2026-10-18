import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C08A04A
LENGTH = 18
RAM_SIZE = 16 * 1024 * 1024
FIELD_BYTE = 0x8C752BE0
FIELD_HALF = 0x8C752BE4
FIELD_WORD = 0x8C752BF8
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('2a d2 04 e1 28 d3 00 e4 29 d0 41 23 10 22 0b 00 42 20')
assert int.from_bytes(binary[0x8C08A0F0 - BASE:0x8C08A0F0 - BASE + 4], 'little') == FIELD_HALF
assert int.from_bytes(binary[0x8C08A0F4 - BASE:0x8C08A0F4 - BASE + 4], 'little') == FIELD_BYTE
assert int.from_bytes(binary[0x8C08A0F8 - BASE:0x8C08A0F8 - BASE + 4], 'little') == FIELD_WORD

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'initialize_three_global_fields_8c08a04a.dll'))
native = library.sa_initialize_three_global_fields_8c08a04a
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
    initial = rng.randbytes(16)
    start = offset(FIELD_BYTE)
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
    assert actual_ram[offset(FIELD_BYTE)] == 4
    assert actual_ram[offset(FIELD_HALF):offset(FIELD_HALF) + 2] == b'\0\0'
    assert actual_ram[offset(FIELD_WORD):offset(FIELD_WORD) + 4] == bytes(4)
    cases.append({'case': case, 'initial_fields_sha256': hashlib.sha256(initial).hexdigest(), 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_initialize_three_global_fields_8c08a04a.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against the retail SH-4 interpreter',
    'scope': 'Exact 18-byte function, including the 32-bit zero store in the RTS delay slot and verified literal-pool pointers.',
    'limitations': ['Semantic names for the three initialized fields are not yet known.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'field_addresses': {'byte': hex(FIELD_BYTE), 'halfword': hex(FIELD_HALF), 'longword': hex(FIELD_WORD)},
    'cases': cases,
}
(PROJECT / 'analysis' / 'initialize_three_global_fields_8c08a04a_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} three-field initializer RAM/CPU comparisons passed')
