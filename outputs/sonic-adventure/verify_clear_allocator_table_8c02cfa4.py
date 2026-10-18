import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C02CFA4
LENGTH = 20
RAM_SIZE = 16 * 1024 * 1024
TABLE = 0x8C02D560
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
native = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'clear_allocator_table_8c02cfa4.dll')).sa_clear_allocator_table_8c02cfa4
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE - 2048, hex(address)
    return result

rng = random.Random(FUNCTION)
cases = []
for case in range(64):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    initial = bytes(rng.getrandbits(8) for _ in range(0x500))
    start = offset(TABLE)
    ram[start:start + len(initial)] = initial
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
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
    assert actual_ram == expected_ram, f'RAM mismatch case={case}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch case={case}: rdiff={[(i,hex(actual_state.r[i]),hex(expected_state.r[i])) for i in range(16) if actual_state.r[i]!=expected_state.r[i]]} sr={actual_state.sr:#x}/{expected_state.sr:#x}'
    cases.append({'case': case, 'zeroed_bytes': 0x400, 'ram_and_cpu_match': True})

source_file = PROJECT / 'source' / 'sa_clear_allocator_table_8c02cfa4.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH), 'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source_file.read_bytes()).hexdigest(), 'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state',
    'scope': 'Clear all 128 two-word allocator entries (0x400 bytes), verify surrounding bytes and full CPU state against retail interpreter for 64 arbitrary initial RAM/register patterns.',
    'limitations': ['Only the fixed retail table extent is covered; allocator semantics and full gameplay remain unverified.'],
    'cases': cases,
}
(PROJECT / 'analysis' / 'clear_allocator_table_8c02cfa4_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} allocator-table RAM/CPU comparisons passed')
