import ctypes
import hashlib
import json
import random
import struct
from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C09F73C
LENGTH = 30
binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'sparse_pointer_relocate_8c09f73c.dll'
library = ctypes.CDLL(str(library_path))
native = library.sa_sparse_pointer_relocate_8c09f73c
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None
rng = random.Random(FUNCTION)
cases = []
counts = [-0x80000000, -3, -1, 0, 1, 2, 3, 4, 5, 63, 64, 65, 127, 128, 129, 255]
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    start = 0x8C740000 + case * 0x1000
    base = [0, 1, 0x8C010000, 0x7fffffff, 0xffffffff, rng.getrandbits(32)][case % 6]
    count = counts[case % len(counts)]
    steps = max(0, (count + 1) // 2) if count > 0 else 0
    for index in range(steps):
        value = [0, 1, 0xffffffff, rng.getrandbits(32)][(case + index) % 4]
        struct.pack_into('<I', ram, start - 0x8C000000 + index * 8, value)
        struct.pack_into('<I', ram, start - 0x8C000000 + index * 8 + 4, rng.getrandbits(32))
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4], state.r[5], state.r[6] = base, start, count & 0xffffffff
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

source_path = ROOT / 'source' / 'sa_sparse_pointer_relocate_8c09f73c.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE),
    'address': hex(FUNCTION), 'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source_path.read_bytes()).hexdigest(),
    'cases_passed': len(cases), 'comparison_level': 'full RAM and GPR/SR/PR/PC',
    'scope': 'Exact 30-byte retail sparse pointer-relocation helper; 128 interpreter/native comparisons with signed counts, zero/nonzero words, odd/even lengths, and 32-bit relocation wraparound.',
    'limitations': ['The pair-record schema and caller interpretation of the returned cursor remain unknown.'],
    'cases': cases,
}
(ROOT / 'analysis' / 'sparse_pointer_relocate_8c09f73c_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'Sparse pointer relocation at {FUNCTION:#x}: {len(cases)} full RAM/CPU cases passed')
