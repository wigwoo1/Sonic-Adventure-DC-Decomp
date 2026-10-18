import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C647F4C
LENGTH = 304
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'sa_record_buffer_descriptors_build_8c647f4c.dll'))
native = library.sa_record_buffer_descriptors_build_8c647f4c
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(FUNCTION)
valid_modes = [0x0F, 0x21, 0x41, 0x7A]
cases_to_run = [(mode, mask) for mode in valid_modes for mask in (0, 1, 5, 0xA5, 0xFF)]
cases_to_run += [(0x99, 1), (0xFFFFFFFF, 0xFF), (0x21, 0x80)]
cases = []
for case, (mode, mask) in enumerate(cases_to_run):
    ram = bytearray([0xA5]) * RAM_SIZE
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    object_base = [0x8C700000, 0x8C710000, 0x8C800000][case % 3]
    ram[0x89C54C:0x89C550] = object_base.to_bytes(4, 'little')
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = mode
    state.r[5] = mask
    state.r[6] = 0x8C720000 if case % 2 == 0 else 0x8C740000
    state.r[15] = 0x8C800000 + rng.randrange(0x10000 // 4) * 4
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION

    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    steps = reference(expected_ram, expected_state)
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual_state))
    if actual_ram != expected_ram:
        diffs = [i for i, (a, e) in enumerate(zip(actual_ram, expected_ram)) if a != e][:16]
        raise AssertionError(f'RAM mismatch case {case}, mode={mode:08x}, mask={mask:02x}: {[(hex(i), actual_ram[i:i+8].hex(), expected_ram[i:i+8].hex()) for i in diffs]}; actual={actual_ram[0x89c550:0x89c590].hex(" ")}; expected={expected_ram[0x89c550:0x89c590].hex(" ")}')
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch case {case}, mode={mode:08x}, mask={mask:02x}: actual={[hex(v) for v in actual_state.r]} SR={actual_state.sr:08x} PR={actual_state.pr:08x} expected={[hex(v) for v in expected_state.r]} SR={expected_state.sr:08x} PR={expected_state.pr:08x}'
    cases.append({'case': case, 'mode': hex(mode), 'mask': hex(mask), 'retail_instructions': steps, 'ram_and_cpu_match': True})

source = PROJECT / 'source' / 'sa_record_buffer_descriptors_build_8c647f4c.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against retail SH-4 execution, including recovered byte-fill calls',
    'scope': 'Select a supported mode, build and clear one descriptor for each selected mask bit, fill the associated data blocks with their retail byte values, and match stack and CPU effects.',
    'limitations': ['Synthetic RAM and CPU states are used; full engine initialization, complete-game execution, and hardware behavior remain unverified.'],
    'cases': cases,
}
(PROJECT / 'analysis' / 'sa_record_buffer_descriptors_build_8c647f4c_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} record buffer descriptor RAM/CPU comparisons passed')
