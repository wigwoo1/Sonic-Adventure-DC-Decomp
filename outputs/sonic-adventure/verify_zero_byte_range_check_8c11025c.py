import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C11025C
LENGTH = 30
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'zero_byte_range_check_8c11025c.dll'))
native = library.sa_zero_byte_range_check_8c11025c
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE, hex(address)
    return result

rng = random.Random(FUNCTION)
counts = [0, 1, 2, 8, -1, -2, 0x80000000]
cases = []
for case in range(96):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    ptr = 0x8C740000 + case * 0x100
    count = counts[case % len(counts)] if case < 32 else rng.choice([0, 1, 2, 3, 8, -1, -3])
    n = count if 0 < count <= 32 else 32
    at = offset(ptr)
    ram[at:at + n] = bytes(n)
    nonzero_index = None
    if n and (case % 3 != 0):
        nonzero_index = rng.randrange(n)
        ram[at + nonzero_index] = rng.choice([1, 0x7F, 0x80, 0xFF])
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4], state.r[5] = ptr, count & 0xFFFFFFFF
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    reference(expected_ram, expected_state)
    actual_ram = bytearray(ram)
    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(view, ctypes.byref(actual_state))
    assert actual_ram == expected_ram, f'RAM mismatch case={case}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch case={case}: rdiff={[(i,hex(actual_state.r[i]),hex(expected_state.r[i])) for i in range(16) if actual_state.r[i]!=expected_state.r[i]]} sr={actual_state.sr:#x}/{expected_state.sr:#x}'
    cases.append({'case': case, 'count': count, 'first_nonzero_index': nonzero_index, 'ram_and_cpu_match': True})

source = PROJECT / 'source' / 'sa_zero_byte_range_check_8c11025c.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH), 'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(), 'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state',
    'scope': 'Exact 30-byte zero-byte range predicate; tests signed length boundaries, all-zero ranges, early nonzero positions, signed byte values, and independent initial T-bit state.',
    'limitations': ['The caller-specific use of the zero-range result remains unidentified.'],
    'cases': cases,
}
(PROJECT / 'analysis' / 'zero_byte_range_check_8c11025c_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} zero-byte range RAM/CPU comparisons passed')
