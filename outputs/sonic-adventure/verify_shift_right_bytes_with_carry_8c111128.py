import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C111128
LENGTH = 74
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'shift_right_bytes_with_carry_8c111128.dll'))
native = library.sa_shift_right_bytes_with_carry_8c111128
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 16 <= result < RAM_SIZE - 32, hex(address)
    return result

rng = random.Random(FUNCTION)
cases = []
counts = [0, 1, 2, 7, 8, 9, 16, 31, 32, -1, -2]
for case in range(72):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    ptr = 0x8C740000 + case * 0x100
    count = counts[case % len(counts)] if case < 44 else rng.choice([0, 1, 3, 7, 8, 9, 15, 16, 23, 24, 31, 32, -1])
    at = offset(ptr)
    ram[at:at + 40] = bytes(rng.getrandbits(8) for _ in range(40))
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4], state.r[5] = ptr, count & 0xFFFFFFFF
    state.r[15] = 0x8C7FF000
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    reference(expected_ram, expected_state)
    actual_ram = bytearray(ram)
    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(view, ctypes.byref(actual_state))
    assert actual_ram == expected_ram, f'RAM mismatch case={case} count={count} ptr={ptr:#x}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch case={case}: rdiff={[(i,hex(actual_state.r[i]),hex(expected_state.r[i])) for i in range(16) if actual_state.r[i]!=expected_state.r[i]]} sr={actual_state.sr:#x}/{expected_state.sr:#x}'
    cases.append({'case': case, 'count': count, 'pointer': hex(ptr), 'ram_and_cpu_match': True})

source = PROJECT / 'source' / 'sa_shift_right_bytes_with_carry_8c111128.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH), 'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(), 'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state',
    'scope': 'Exact 74-byte in-place byte shift with inter-byte carry; tests zero/negative and group boundary lengths, random bytes, full RAM/CPU state and stack saves.',
    'limitations': ['Caller-specific meaning of the shifted byte stream remains unidentified.'],
    'cases': cases,
}
(PROJECT / 'analysis' / 'shift_right_bytes_with_carry_8c111128_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} byte-shift-with-carry RAM/CPU comparisons passed')
