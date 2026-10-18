import ctypes
import hashlib
import json
import random
import struct
from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C600DD0
LENGTH = 12
binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'record_initialize_8c600dd0.dll'
library = ctypes.CDLL(str(library_path))
native = library.sa_record_initialize_8c600dd0
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None
rng = random.Random(FUNCTION)
reports = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    if case == 0:
        target = 0x8C740000
    elif case == 1:
        target = FUNCTION - 8
    elif case == 2:
        target = FUNCTION - 4
    elif case == 3:
        target = 0x8CFFFFF4
    else:
        target = 0x8C740000 + rng.randrange(0, 0x1000) * 4
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = target
    state.r[5] = rng.getrandbits(32)
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    expected_ram = bytearray(ram)
    expected = State.from_buffer_copy(state)
    reference(expected_ram, expected)
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual))
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual) == bytes(expected), f'CPU mismatch in case {case}: r={[(i, hex(actual.r[i]), hex(expected.r[i])) for i in range(16) if actual.r[i] != expected.r[i]]}, sr={actual.sr:#x}/{expected.sr:#x}, pr={actual.pr:#x}/{expected.pr:#x}, pc={actual.pc:#x}/{expected.pc:#x}'
    reports.append({'case': case, 'target': hex(target), 'ram_and_cpu_match': True})

source_path = ROOT / 'source' / 'sa_record_initialize_8c600dd0.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE),
    'address': hex(FUNCTION), 'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source_path.read_bytes()).hexdigest(),
    'cases_passed': len(reports), 'comparison_level': 'full RAM and GPR/SR/PR/PC',
    'scope': 'Exact 12-byte retail routine; 128 interpreter/native comparisons including aligned RAM limits and code overlap.',
    'limitations': ['Record field meanings and callers remain unidentified.'],
    'cases': reports,
}
(ROOT / 'analysis' / 'record_initialize_8c600dd0_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f"Record initializer at {FUNCTION:#x}: {len(reports)} full RAM/CPU cases passed")
