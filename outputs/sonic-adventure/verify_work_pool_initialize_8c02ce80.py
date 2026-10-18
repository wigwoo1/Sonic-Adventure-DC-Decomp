import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C02CE80
LENGTH = 28
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
native = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'work_pool_initialize_8c02ce80.dll')).sa_work_pool_initialize_8c02ce80
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE - 8192, hex(address)
    return result

rng = random.Random(FUNCTION)
cases = []
for case in range(64):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    count = 2 + (case % 63)
    base = 0x8C740000 + case * 0x4000
    region = bytes(rng.getrandbits(8) for _ in range((count + 1) * 0x40))
    start = offset(base)
    ram[start:start + len(region)] = region
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4], state.r[5] = base, count
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
    assert actual_ram == expected_ram, f'RAM mismatch case={case}, count={count}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch case={case}, count={count}: rdiff={[(i,hex(actual_state.r[i]),hex(expected_state.r[i])) for i in range(16) if actual_state.r[i]!=expected_state.r[i]]} sr={actual_state.sr:#x}/{expected_state.sr:#x}'
    cases.append({'case': case, 'pool_count': count, 'ram_and_cpu_match': True})

source_file = PROJECT / 'source' / 'sa_work_pool_initialize_8c02ce80.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH), 'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source_file.read_bytes()).hexdigest(), 'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state',
    'scope': 'Free-list pool initialization for valid counts 2 through 64, 0x40-byte stride, initialized head/tail links, preserved surrounding memory, full RAM/CPU comparisons against retail interpreter.',
    'limitations': ['Counts below two are not covered; the retail do-while counter wraps for count zero or one. Invalid addresses and full gameplay remain unverified.'],
    'cases': cases,
}
(PROJECT / 'analysis' / 'work_pool_initialize_8c02ce80_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} work-pool initialization RAM/CPU comparisons passed')
