import ctypes
import hashlib
import json
import random
import struct
from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C608922
LENGTH = 12
MASK_LITERAL = 0x8C608930
binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'triplet_initialize_8c608922.dll'
library = ctypes.CDLL(str(library_path))
native = library.sa_triplet_initialize_8c608922
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None
rng = random.Random(FUNCTION)
destinations = [0x8C720000 + i * 0x100 for i in range(120)] + [
    0x8C000000, 0x8CFFFFF4, FUNCTION - 0x20, MASK_LITERAL, 0x8C608900,
    0x8C010000, 0x8C700000, 0x8C710004]
r7_values = [0, 0x40000000, 0x80000000, 0xffffffff, 0x3fffffff, 0xc0000000]
reports = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    destination = destinations[case]
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = destination
    state.r[5] = [0, 0xffffffff, 0x12345678][case % 3] if case < 18 else rng.getrandbits(32)
    state.r[6] = [0, 0x80000000, 0xffffffff][case % 3] if case < 18 else rng.getrandbits(32)
    state.r[7] = r7_values[case % len(r7_values)] if case < 24 else rng.getrandbits(32)
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
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}, destination={destination:#x}'
    assert bytes(actual) == bytes(expected), (
        f'CPU mismatch in case {case}: '
        f'r={[(i, hex(actual.r[i]), hex(expected.r[i])) for i in range(16) if actual.r[i] != expected.r[i]]}, '
        f'sr={actual.sr:#x}/{expected.sr:#x}, pc={actual.pc:#x}/{expected.pc:#x}'
    )
    reports.append({'case': case, 'destination': hex(destination), 'r7': hex(state.r[7]),
                    't': actual.sr & 1, 'ram_and_cpu_match': True})

report = {
    'module': '1ST_READ.BIN',
    'load_address': hex(BASE),
    'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'binary_sha256': hashlib.sha256(binary).hexdigest(),
    'source_sha256': hashlib.sha256((ROOT / 'source' / 'sa_triplet_initialize_8c608922.c').read_bytes()).hexdigest(),
    'cases_passed': len(reports),
    'comparison_level': 'Complete 16 MiB RAM and all GPRs, SR, PR, and return PC compared against the retail SH-4 interpreter.',
    'scope': 'Tests three ordered word stores (including the RTS delay-slot store), the PC-relative 0x40000000 mask, SR.T results, overlapping/code/literal destinations, and first/last aligned 12-byte RAM ranges.',
    'limitations': ['Higher-level structure meaning remains unidentified; hardware instruction-cache behavior for executable-memory writes is not modeled.'],
    'cases': reports,
}
(ROOT / 'analysis' / 'triplet_initialize_8c608922_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{len(reports)} triplet initializer full RAM/CPU comparisons passed")
