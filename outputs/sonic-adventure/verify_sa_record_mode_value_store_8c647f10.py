import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C647F10
LENGTH = 60
GLOBAL_CELL = 0x8C89C54C
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('43 60 2d d3 0f 88 08 8d 32 65 21 88 08 89 41 88 08 89 7a 88 09 89 0b a0 09 00 00 e2 0b a0 21 15 04 a0 01 e0 02 e1 06 a0 11 15 03 e0 03 a0 01 15 37 90 0b 00 09 00 00 e0 0b 00 09 00')
library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'sa_record_mode_value_store_8c647f10.dll'))
native = library.sa_record_mode_value_store_8c647f10
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(FUNCTION)
values = [0x0F, 0x21, 0x41, 0x7A, 0, 1, 0x10, 0x20, 0x40, 0x79, 0x7B, 0x100, 0xFFFFFFFF]
values += [rng.getrandbits(32) for _ in range(115)]
objects = [0x8C700000, 0x8C710000, 0x8C800000]
cases = []
for case, value in enumerate(values):
    ram = bytearray([0xA5]) * RAM_SIZE
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    obj = objects[case % len(objects)] if case < len(objects) else 0x8C700000 + rng.randrange(0x10000 // 4) * 4
    ram[GLOBAL_CELL - 0x8C000000:GLOBAL_CELL - 0x8C000000 + 4] = obj.to_bytes(4, 'little')
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = value
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
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}, input {value:08x}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch in case {case}, input {value:08x}'
    cases.append({'case': case, 'input': hex(value), 'object': hex(obj), 'retail_instructions': steps, 'ram_and_cpu_match': True})

source = PROJECT / 'source' / 'sa_record_mode_value_store_8c647f10.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against the retail SH-4 interpreter',
    'scope': 'Map four recognized input values to object mode words 0..3; leave the object field untouched for other inputs and return the retail 0xffffff00 sentinel.',
    'limitations': ['Synthetic RAM and CPU states are used; complete-game behavior and hardware execution remain unverified.'],
    'global_cell': hex(GLOBAL_CELL), 'cases': cases,
}
(PROJECT / 'analysis' / 'sa_record_mode_value_store_8c647f10_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} record mode value-store RAM/CPU comparisons passed')
