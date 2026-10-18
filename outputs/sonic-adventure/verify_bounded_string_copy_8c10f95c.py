import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C10F95C
LENGTH = 46
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'bounded_string_copy_8c10f95c.dll'))
native = library.sa_bounded_string_copy_8c10f95c
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE - 64, hex(address)
    return result

rng = random.Random(FUNCTION)
cases = []
for case in range(80):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    source = 0x8C740000 + case * 0x100
    length = [0, 1, 2, 4, 8, 16][case % 6] if case < 36 else rng.randrange(0, 25)
    destination = [0x8C750000 + case * 0x40, source, source + 1, source - 1][case % 4]
    input_at = offset(source)
    payload = bytearray(rng.randrange(1, 256) for _ in range(32))
    if length and case % 3 != 0:
        payload[rng.randrange(min(length, 24))] = 0
    ram[input_at:input_at + len(payload)] = payload
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4], state.r[5], state.r[6] = destination, source, length
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
    assert actual_ram == expected_ram, f'RAM mismatch case={case} length={length} src={source:#x} dst={destination:#x}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch case={case}: rdiff={[(i,hex(actual_state.r[i]),hex(expected_state.r[i])) for i in range(16) if actual_state.r[i]!=expected_state.r[i]]} sr={actual_state.sr:#x}/{expected_state.sr:#x}'
    cases.append({'case': case, 'length': length, 'source': hex(source), 'destination': hex(destination), 'ram_and_cpu_match': True})

source_file = PROJECT / 'source' / 'sa_bounded_string_copy_8c10f95c.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH), 'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source_file.read_bytes()).hexdigest(), 'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state',
    'scope': 'Exact 46-byte bounded string copy/pad helper; tests zero and short lengths, null positions, no terminator within limit, overlapping source/destination and all CPU/RAM effects.',
    'limitations': ['Caller-specific string field semantics remain unidentified.'],
    'cases': cases,
}
(PROJECT / 'analysis' / 'bounded_string_copy_8c10f95c_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} bounded string copy RAM/CPU comparisons passed')
