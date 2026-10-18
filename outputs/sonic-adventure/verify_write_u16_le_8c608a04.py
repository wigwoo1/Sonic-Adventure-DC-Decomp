import ctypes
import hashlib
import json
import random
from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C608A04
LENGTH = 10
binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'write_u16_le_8c608a04.dll'
library = ctypes.CDLL(str(library_path))
native = library.sa_write_u16_le_8c608a04
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None
rng = random.Random(FUNCTION)
values = [0, 1, 0xff, 0x100, 0x8000, 0xffff, 0x1234, 0xabcd] + [rng.getrandbits(32) for _ in range(120)]
destinations = [0x8C710000 + i * 0x100 for i in range(120)] + [0x8C000000, 0x8CFFFFFE,
                0x8C608A00, 0x8C608A04, 0x8C600000, 0x8C010000]
reports = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    destination = destinations[case] if case < len(destinations) else 0x8C720000 + case * 0x100
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = values[case]
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
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}, destination={destination:#x}'
    assert bytes(actual) == bytes(expected), (
        f'CPU mismatch in case {case}, initial_r4={values[case]:#x}, final_r4={actual.r[4]:#x}/{expected.r[4]:#x}: '
        f'r={[(i, hex(actual.r[i]), hex(expected.r[i])) for i in range(16) if actual.r[i] != expected.r[i]]}, '
        f'sr={actual.sr:#x}/{expected.sr:#x}, pc={actual.pc:#x}/{expected.pc:#x}'
    )
    reports.append({'case': case, 'value': hex(state.r[4]), 'destination': hex(destination), 'ram_and_cpu_match': True})

report = {
    'module': '1ST_READ.BIN',
    'load_address': hex(BASE),
    'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'binary_sha256': hashlib.sha256(binary).hexdigest(),
    'source_sha256': hashlib.sha256((ROOT / 'source' / 'sa_write_u16_le_8c608a04.c').read_bytes()).hexdigest(),
    'cases_passed': len(reports),
    'comparison_level': 'Complete 16 MiB RAM and all GPRs, SR, PR, and return PC compared against the retail SH-4 interpreter.',
    'scope': 'Tests little-endian low/high byte stores, high-byte scratch register effects, full 32-bit input truncation, code-backed writes, and first/last valid two-byte RAM destinations.',
    'limitations': ['Hardware instruction-cache behavior for writes targeting executable memory is not modeled; comparisons cover architectural RAM bytes and the interpreter execution path for tested safe code aliases.'],
    'cases': reports,
}
(ROOT / 'analysis' / 'write_u16_le_8c608a04_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{len(reports)} 16-bit writer full RAM/CPU comparisons passed")
