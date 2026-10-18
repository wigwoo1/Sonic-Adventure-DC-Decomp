import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C107EC0
LENGTH = 26
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'clear_0x600_bytes_8c107ec0.dll'))
native = library.sa_clear_0x600_bytes_8c107ec0
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result <= RAM_SIZE - 0x600, hex(address)
    return result

rng = random.Random(FUNCTION)
cases = []
starts = [0x8C740000, 0x8C740100, 0x8C7FF900, 0x8C010100, 0x8C107EC0 - 0x1000]
for case in range(64):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    start = starts[case % len(starts)] if case < len(starts) else 0x8C740000 + rng.randrange(0x100000)
    at = offset(start)
    ram[at:at + 0x600] = bytes([case & 0xFF]) * 0x600
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = start
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    reference(expected_ram, expected_state)
    actual_ram = bytearray(ram)
    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(view, ctypes.byref(actual_state))
    assert actual_ram == expected_ram, f'RAM mismatch case={case} start={start:#x}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch case={case}: rdiff={[(i,hex(actual_state.r[i]),hex(expected_state.r[i])) for i in range(16) if actual_state.r[i]!=expected_state.r[i]]} sr={actual_state.sr:#x}/{expected_state.sr:#x} pr={actual_state.pr:#x}/{expected_state.pr:#x} pc={actual_state.pc:#x}/{expected_state.pc:#x}'
    cases.append({'case': case, 'start': hex(start), 'bytes_cleared': 0x600, 'ram_and_cpu_match': True})

source = PROJECT / 'source' / 'sa_clear_0x600_bytes_8c107ec0.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH), 'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(), 'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state',
    'scope': 'Exact 26-byte fixed-size clear routine; compares all RAM and CPU state for 64 mapped blocks including executable-memory overlap and boundary-adjacent ranges.',
    'limitations': ['The caller-defined structure represented by the 0x600-byte block remains unidentified.'],
    'cases': cases,
}
(PROJECT / 'analysis' / 'clear_0x600_bytes_8c107ec0_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} fixed-size clear RAM/CPU comparisons passed')
