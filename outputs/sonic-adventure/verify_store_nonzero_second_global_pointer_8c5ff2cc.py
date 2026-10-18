import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C5FF2CC
LENGTH = 18
RAM_SIZE = 16 * 1024 * 1024
TARGET = 0x8C67340C
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('48 24 01 8b 0b 00 ff e0 77 d2 00 e0 42 22 0b 00 09 00')
assert int.from_bytes(binary[0x8C5FF4B4 - BASE:0x8C5FF4B4 - BASE + 4], 'little') == TARGET

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'store_nonzero_second_global_pointer_8c5ff2cc.dll'))
native = library.sa_store_nonzero_second_global_pointer_8c5ff2cc
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE, hex(address)
    return result

rng = random.Random(FUNCTION)
values = [0, 1, 0xffffffff, 0x80000000, 0x7fffffff] + [rng.getrandbits(32) for _ in range(91)]
cases = []
for case, value in enumerate(values):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    at = offset(TARGET)
    initial = rng.randbytes(4)
    ram[at:at + 4] = initial
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = value
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
    if value == 0:
        assert actual_ram[at:at + 4] == initial
        assert actual_state.r[0] == 0xffffffff
    else:
        assert actual_ram[at:at + 4] == value.to_bytes(4, 'little')
        assert actual_state.r[0] == 0
    cases.append({'case': case, 'input': hex(value), 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_store_nonzero_second_global_pointer_8c5ff2cc.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against the retail SH-4 interpreter',
    'scope': 'Exact zero-input return path, nonzero pointer store, condition T bit, and second literal-derived destination.',
    'limitations': ['The semantic identity of the stored global remains unknown.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'target_address': hex(TARGET), 'cases': cases,
}
(PROJECT / 'analysis' / 'store_nonzero_second_global_pointer_8c5ff2cc_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} second global pointer RAM/CPU comparisons passed')
