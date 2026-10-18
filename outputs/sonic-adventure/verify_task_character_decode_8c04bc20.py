import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C04BC20
LENGTH = 80
binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'task_character_decode_8c04bc20.dll'
library = ctypes.CDLL(str(library_path))
native = library.sa_task_character_decode_8c04bc20
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(FUNCTION)
reports = []
characters = [0, 0x30, 0x31, 0x40, 0x41, 0x5a, 0x5b, 0x60,
              0x61, 0x7a, 0x7b, 0x7f, 0x80, 0xff]
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    object_address = [0x8C710000 + case * 0x100,
                      0x8C740000 + case * 0x100][case % 2]
    work = 0x8C720000 + case * 0x100
    pointer_field = work + 0x64
    buffer = [0x8C730000 + case * 0x100, FUNCTION,
              pointer_field][case % 3]
    struct.pack_into('<I', ram, object_address - 0x8C000000 + 0x3C, work)
    struct.pack_into('<I', ram, pointer_field - 0x8C000000,
                     0 if case % 7 == 0 else buffer)
    if case % 7 != 0:
        character = characters[case % len(characters)] if case % 2 == 0 else rng.getrandbits(8)
        if buffer != pointer_field and buffer != FUNCTION:
            ram[buffer - 0x8C000000] = character

    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = object_address
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION

    expected_ram = bytearray(ram)
    expected = State.from_buffer_copy(state)
    try:
        reference(expected_ram, expected)
    except Exception as error:
        raise AssertionError(
            f'interpreter failed in case {case}: object={object_address:#x}, '
            f'work={work:#x}, buffer={buffer:#x}, cursor={struct.unpack_from("<I", ram, pointer_field - 0x8C000000)[0]:#x}'
        ) from error
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
    'source_sha256': hashlib.sha256((ROOT / 'source' / 'sa_task_character_decode_8c04bc20.c').read_bytes()).hexdigest(),
    'cases_passed': len(reports),
    'comparison_level': 'Complete 16 MiB RAM and all GPRs, SR, PR, and return PC compared against the retail SH-4 interpreter.',
    'scope': 'Tests task cursor advancement and signed ASCII character decoding across uppercase/lowercase boundaries, digit zero, NUL termination, other ASCII, high-bit bytes, null cursors, code reads, cursor-field aliasing, and final RAM addresses.',
    'limitations': ['Uses the project SH-4 interpreter rather than Dreamcast hardware or end-to-end gameplay.'],
    'cases': reports,
}
(ROOT / 'analysis' / 'task_character_decode_8c04bc20_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{len(reports)} task character decode full RAM/CPU comparisons passed")
