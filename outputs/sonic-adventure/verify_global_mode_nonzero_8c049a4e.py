import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
POINTER_SLOT = 0x8C049AE8
FUNCTION = 0x8C049A4E
LENGTH = 12
binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'global_mode_nonzero_8c049a4e.dll'
library = ctypes.CDLL(str(library_path))
native = library.sa_global_mode_nonzero_8c049a4e
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(FUNCTION)
reports = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    target = [0x8C7492AC, 0x8C730000 + case * 4,
              FUNCTION, POINTER_SLOT, 0x8CFFFFFC][case % 5]
    struct.pack_into('<I', ram, POINTER_SLOT - 0x8C000000, target)
    value = [0, 1, 0xffffffff, 0x80000000,
             rng.getrandbits(32)][case % 5]
    overlaps_code = FUNCTION <= target <= FUNCTION + LENGTH - 4
    if target != POINTER_SLOT and not overlaps_code:
        struct.pack_into('<I', ram, target - 0x8C000000, value)

    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
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
    'source_sha256': hashlib.sha256((ROOT / 'source' / 'sa_global_mode_nonzero_8c049a4e.c').read_bytes()).hexdigest(),
    'cases_passed': len(reports),
    'comparison_level': 'Complete 16 MiB RAM and all GPRs, SR, PR, and return PC compared against the retail SH-4 interpreter.',
    'scope': 'Tests the global nonzero predicate with zero/nonzero words, randomized pointer targets, code reads, the pointer literal, and the last aligned RAM word; includes the RTS delay-slot XOR immediate.',
    'limitations': ['Uses the project SH-4 interpreter rather than Dreamcast hardware or end-to-end gameplay.'],
    'cases': reports,
}
(ROOT / 'analysis' / 'global_mode_nonzero_8c049a4e_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{len(reports)} global mode nonzero full RAM/CPU comparisons passed")
