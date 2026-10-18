import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C021868
LENGTH = 28

binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'task_count_predicate_8c021868.dll'
library = ctypes.CDLL(str(library_path))
native = library.sa_task_count_predicate_8c021868
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(FUNCTION)
reports = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    object_address = 0x8C700000 + case * 0x100
    count_address = [0x8C710000 + case * 8, 0x8C800000 + case * 8,
                     0x8CFFFFF8, FUNCTION - 2][case % 4]
    struct.pack_into('<I', ram, object_address - 0x8C000000 + 4, count_address)
    if count_address != FUNCTION - 2:
        struct.pack_into('<H', ram, count_address - 0x8C000000 + 2,
                         [0, 1, 2, 3, 4, 0x7fff, 0x8000, 0xffff,
                          rng.getrandbits(16)][case % 9])
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = rng.choice([0, 1, 2, 3, 0x7fffffff, 0x80000000,
                             0xffffffff, rng.getrandbits(32)])
    state.r[6] = object_address
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
    assert bytes(actual) == bytes(expected), (
        f'CPU state mismatch in case {case}: '
        f'r={[(i, hex(actual.r[i]), hex(expected.r[i])) for i in range(16) if actual.r[i] != expected.r[i]]}, '
        f'sr={actual.sr:#x}/{expected.sr:#x}, pc={actual.pc:#x}/{expected.pc:#x}'
    )
    reports.append({'case': case, 'ram_and_cpu_match': True})

report = {
    'module': '1ST_READ.BIN',
    'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'binary_sha256': hashlib.sha256(binary).hexdigest(),
    'source_sha256': hashlib.sha256((ROOT / 'source' / 'sa_task_count_predicate_8c021868.c').read_bytes()).hexdigest(),
    'cases_passed': len(reports),
    'comparison_level': 'Complete 16 MiB RAM and all GPRs, SR, PR, and return PC compared against the retail SH-4 interpreter.',
    'scope': 'Tests the index/count predicate at 0x8c021868 across count boundaries and signed/unsigned index boundaries, ordinary RAM, the final aligned halfword, and a code-alias read.',
    'limitations': ['Uses the project SH-4 interpreter rather than Dreamcast hardware or end-to-end gameplay.'],
    'cases': reports,
}
(ROOT / 'analysis' / 'task_count_predicate_8c021868_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{len(reports)} task count predicate full RAM/CPU comparisons passed")
