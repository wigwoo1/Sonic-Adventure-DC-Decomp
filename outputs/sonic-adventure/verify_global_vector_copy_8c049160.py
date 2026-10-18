import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C049160
LENGTH = 18
POINTER_SLOT = 0x8C0491A0
binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'global_vector_copy_8c049160.dll'
library = ctypes.CDLL(str(library_path))
native = library.sa_global_vector_copy_8c049160
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(FUNCTION)
reports = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    pointer_address = [0x8C18B7EC, 0x8C730000 + case * 0x100,
                       0x8C740000 + case * 0x100, POINTER_SLOT,
                       0x8CFFFFD4][case % 5]
    object_address = [0x8C720000 + case * 0x100, FUNCTION,
                      0x8C740000 + case * 0x100,
                      POINTER_SLOT, 0x8CFFFFD4][case % 5]
    if pointer_address == POINTER_SLOT:
        object_address = POINTER_SLOT
    struct.pack_into('<I', ram, POINTER_SLOT - 0x8C000000, pointer_address)
    if pointer_address != POINTER_SLOT and not (FUNCTION <= pointer_address <= FUNCTION + LENGTH - 4):
        struct.pack_into('<I', ram, pointer_address - 0x8C000000, object_address)

    if object_address >= 0x8CFFFFD4 and case % 8 in (1, 2, 3, 4):
        destination = 0x8C760000 + case * 0x100
    elif case % 8 == 1:
        destination = object_address + 0x1C
    elif case % 8 == 2:
        destination = object_address + 0x20
    elif case % 8 == 3:
        destination = object_address + 0x24
    elif case % 8 == 4:
        destination = object_address + 0x28
    elif case % 8 == 5:
        destination = POINTER_SLOT
    elif case % 8 == 6:
        destination = 0x8CFFFFF4
    else:
        destination = 0x8C760000 + case * 0x100

    if object_address != FUNCTION:
        for offset in (0x20, 0x24, 0x28):
            struct.pack_into('<I', ram, object_address - 0x8C000000 + offset,
                             rng.getrandbits(32))
    destination_overlaps_function = FUNCTION <= destination + 8 and destination <= FUNCTION + LENGTH - 1
    if destination_overlaps_function:
        destination = 0x8C760000 + case * 0x100
    if destination != POINTER_SLOT and not (object_address + 0x20 <= destination <= object_address + 0x2B):
        for offset in (0, 4, 8):
            struct.pack_into('<I', ram, destination - 0x8C000000 + offset,
                             rng.getrandbits(32))

    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = destination
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
    assert bytes(actual) == bytes(expected), f'CPU state mismatch in case {case}'
    reports.append({'case': case, 'ram_and_cpu_match': True})

report = {
    'module': '1ST_READ.BIN',
    'load_address': hex(BASE),
    'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'binary_sha256': hashlib.sha256(binary).hexdigest(),
    'source_sha256': hashlib.sha256((ROOT / 'source' / 'sa_global_vector_copy_8c049160.c').read_bytes()).hexdigest(),
    'cases_passed': len(reports),
    'comparison_level': 'Complete 16 MiB RAM and all GPRs, SR, PR, and return PC compared against the retail SH-4 interpreter.',
    'scope': 'Tests the PC-relative global pointer chain and three-word vector copy with independent and overlapping source/destination fields, code-backed source data, pointer-slot aliases, and final aligned RAM boundaries.',
    'limitations': ['Uses the project SH-4 interpreter rather than Dreamcast hardware or end-to-end gameplay.'],
    'cases': reports,
}
(ROOT / 'analysis' / 'global_vector_copy_8c049160_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{len(reports)} global vector copy full RAM/CPU comparisons passed")
