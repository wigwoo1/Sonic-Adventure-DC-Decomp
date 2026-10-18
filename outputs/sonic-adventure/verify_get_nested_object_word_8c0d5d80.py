import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C0D5D80
LENGTH = 16
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('48 24 01 8b 0b 00 00 e0 48 50 04 50 0b 00 09 00')

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'get_nested_object_word_8c0d5d80.dll'))
native = library.sa_get_nested_object_word_8c0d5d80
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
    if case == 0:
        object_address = 0
        nested_address = None
        expected_value = 0
    else:
        object_address = 0x8C100000 + rng.randrange(0x300000 // 4) * 4
        nested_address = 0x8C700000 + rng.randrange(0x100000 // 4) * 4
        ram[offset(object_address + 0x20):offset(object_address + 0x24)] = nested_address.to_bytes(4, 'little')
        expected_value = rng.getrandbits(32)
        at = offset(nested_address + 0x10)
        ram[at:at + 4] = expected_value.to_bytes(4, 'little')
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = object_address
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
    assert actual_state.r[0] == expected_value, f'return value mismatch in case {case}'
    cases.append({'case': case, 'object': hex(object_address), 'nested': None if nested_address is None else hex(nested_address), 'result': hex(expected_value), 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_get_nested_object_word_8c0d5d80.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against the retail SH-4 interpreter',
    'scope': 'Null return and nested pointer reads at object+0x20 and nested+0x10, including condition T bit.',
    'limitations': ['The semantic identity of the nested fields remains unknown.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'cases': cases,
}
(PROJECT / 'analysis' / 'get_nested_object_word_8c0d5d80_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} nested object getter RAM/CPU comparisons passed')
