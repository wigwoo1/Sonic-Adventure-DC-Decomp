import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C647E16
LENGTH = 194
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'sa_record_setup_initialize_8c647e16.dll'))
native = library.sa_record_setup_initialize_8c647e16
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(FUNCTION)
scenarios = [
    (0x21, 0x00, 0), (0x21, 0x05, 0), (0x0F, 0xA5, 0),
    (0x41, 0xFF, 0), (0x7A, 0x00, 0),
    (0x21, 0x00, 0x8C720000), (0x21, 0x05, 0x8C720000),
    (0x0F, 0xA5, 0x8C720000), (0x41, 0xFF, 0x8C720000),
    (0x7A, 0x00, 0x8C720000), (0x99, 0x05, 0x8C720000),
    (0xFFFFFFFF, 0xFF, 0x8C720000),
]
cases = []
for case, (mode, mask, buffer) in enumerate(scenarios):
    ram = bytearray([0xA5]) * RAM_SIZE
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = mode
    state.r[5] = mask
    state.r[6] = buffer
    state.r[15] = 0x8C800000 + rng.randrange(0x10000 // 4) * 4
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    steps = reference(expected_ram, expected_state)
    actual_ram = bytearray(ram)
    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(view, ctypes.byref(actual_state))
    if actual_ram != expected_ram:
        diffs = [i for i, (a, e) in enumerate(zip(actual_ram, expected_ram)) if a != e][:12]
        raise AssertionError(f'RAM mismatch case {case}, mode={mode:08x}, mask={mask:02x}, buffer={buffer:08x}: {[(hex(i), actual_ram[i:i+8].hex(), expected_ram[i:i+8].hex()) for i in diffs]}')
    if bytes(actual_state) != bytes(expected_state):
        register_diffs = {f'r{i}': (hex(actual_state.r[i]), hex(expected_state.r[i])) for i in range(16) if actual_state.r[i] != expected_state.r[i]}
        register_diffs.update({name: (hex(getattr(actual_state, name)), hex(getattr(expected_state, name))) for name in ('sr', 'pr', 'pc') if getattr(actual_state, name) != getattr(expected_state, name)})
        raise AssertionError(f'CPU mismatch case {case}: {register_diffs}')
    cases.append({'case': case, 'mode': hex(mode), 'mask': hex(mask), 'buffer': hex(buffer), 'retail_instructions': steps, 'ram_and_cpu_match': True})

source = PROJECT / 'source' / 'sa_record_setup_initialize_8c647e16.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against retail SH-4 execution, composing recovered callback, mode, descriptor, and global setters',
    'scope': 'Initialize the record subsystem, count selected bits, choose the no-buffer mode setter or buffer descriptor builder, reset the associated globals, and match complete caller return state.',
    'limitations': ['Synthetic RAM and CPU states are used; full game startup, complete-game execution, and Dreamcast hardware remain unverified.'],
    'cases': cases,
}
(PROJECT / 'analysis' / 'sa_record_setup_initialize_8c647e16_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} record setup initializer RAM/CPU comparisons passed')
