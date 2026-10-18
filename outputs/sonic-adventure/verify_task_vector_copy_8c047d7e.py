import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C047D7E
LENGTH = 18

binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'task_vector_copy_8c047d7e.dll'
library = ctypes.CDLL(str(library_path))
native = library.sa_task_vector_copy_8c047d7e
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(FUNCTION)
reports = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    if case % 8 == 0:
        source = FUNCTION
    elif case % 8 == 5:
        source = 0x8CFFFFD4
    else:
        source = 0x8C710000 + case * 0x100

    if case % 8 == 1:
        destination = source
    elif case % 8 == 2:
        destination = source - 0x30
    elif case % 8 == 3:
        destination = source - 0x2C
    elif case % 8 == 6:
        destination = 0x8CFFFFA4
    else:
        destination = 0x8C740000 + case * 0x100

    if source != FUNCTION:
        for offset in (0x20, 0x24, 0x28):
            struct.pack_into('<I', ram, source - 0x8C000000 + offset, rng.getrandbits(32))
    if destination not in (0x8CFFFFA4,) and not (source - 0x30 <= destination <= source + 0x28):
        for offset in (0x50, 0x54, 0x58):
            struct.pack_into('<I', ram, destination - 0x8C000000 + offset, rng.getrandbits(32))

    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = source
    state.r[5] = destination
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
    'load_address': hex(BASE),
    'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'binary_sha256': hashlib.sha256(binary).hexdigest(),
    'source_sha256': hashlib.sha256((ROOT / 'source' / 'sa_task_vector_copy_8c047d7e.c').read_bytes()).hexdigest(),
    'cases_passed': len(reports),
    'comparison_level': 'Complete 16 MiB RAM and all GPRs, SR, PR, and return PC compared against the retail SH-4 interpreter.',
    'scope': 'Tests the three-word vector copy at 0x8c047d7e with independent and overlapping buffers, retail-code source data, and the final aligned source and destination ranges in RAM.',
    'limitations': ['Uses the project SH-4 interpreter rather than Dreamcast hardware or end-to-end gameplay.'],
    'cases': reports,
}
(ROOT / 'analysis' / 'task_vector_copy_8c047d7e_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{len(reports)} task vector copy full RAM/CPU comparisons passed")
