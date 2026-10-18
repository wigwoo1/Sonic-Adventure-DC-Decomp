import ctypes
import hashlib
import json
import random
import struct
from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C09F7E6
LENGTH = 158
binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'quad_pointer_relocate_8c09f7e6.dll'
library = ctypes.CDLL(str(library_path))
native = library.sa_quad_pointer_relocate_8c09f7e6
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None
rng = random.Random(FUNCTION)
cases = []
counts = [-0x80000000, -9, -1, 0, 1, 2, 7, 8, 9, 15, 16, 17, 31, 32, 33, 63, 64, 65, 127, 128]
for case in range(120):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    start = 0x8C740000 + case * 0x1000
    base = [0, 1, 0x8C010000, 0x7fffffff, 0xffffffff, rng.getrandbits(32)][case % 6]
    count = counts[case % len(counts)]
    groups = (count + 7) // 8 if count > 0 else 0
    for index in range(groups):
        for field in range(8):
            value = [0, 1, 0xffffffff, rng.getrandbits(32)][(case + index + field) % 4]
            struct.pack_into('<I', ram, start - 0x8C000000 + index * 32 + field * 4, value)
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4], state.r[5], state.r[6] = base, start, count & 0xffffffff
    state.r[15] = 0x8cf00000
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION

    expected_ram = bytearray(ram)
    expected = State.from_buffer_copy(state)
    reference(expected_ram, expected, instruction_budget=10000)
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual))
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}, count={count}'
    assert bytes(actual) == bytes(expected), (
        f'CPU mismatch in case {case}: '
        f'r={[(i, hex(actual.r[i]), hex(expected.r[i])) for i in range(16) if actual.r[i] != expected.r[i]]}, '
        f'sr={actual.sr:#x}/{expected.sr:#x}, pr={actual.pr:#x}/{expected.pr:#x}, '
        f'pc={actual.pc:#x}/{expected.pc:#x}')
    cases.append({'case': case, 'count': count, 'relocation_base': hex(base),
                  'ram_and_cpu_match': True})

source_path = ROOT / 'source' / 'sa_quad_pointer_relocate_8c09f7e6.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE),
    'address': hex(FUNCTION), 'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source_path.read_bytes()).hexdigest(),
    'cases_passed': len(cases), 'comparison_level': 'full RAM and GPR/SR/PR/PC',
    'scope': 'Exact 158-byte retail quad pointer-relocation routine; 120 complete interpreter/native comparisons over signed counts, sparse fields, wrapping bases, and prologue stack effects.',
    'limitations': ['The 32-byte group schema and caller count convention remain under analysis.'],
    'cases': cases,
}
(ROOT / 'analysis' / 'quad_pointer_relocate_8c09f7e6_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'Quad pointer relocation at {FUNCTION:#x}: {len(cases)} full RAM/CPU cases passed')
