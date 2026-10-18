import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C660760
LENGTH = 8
TARGET = 0x8C65FC92
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('00 a0 09 00 95 aa 09 00')
first_target = FUNCTION + 4
displacement = 0xA95 - 0x1000
assert first_target + 4 + displacement * 2 == TARGET

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'branch_thunk_8c660760.dll'))
native = library.sa_branch_thunk_8c660760
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(FUNCTION)
cases = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.sr = rng.getrandbits(32)
    state.pr = rng.getrandbits(32)
    state.pc = FUNCTION
    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    reference(expected_ram, expected_state, stop_pc=TARGET)
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual_state))
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch in case {case}'
    assert actual_state.pc == TARGET and actual_state.pr == state.pr
    cases.append({'case': case, 'target': hex(TARGET), 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_branch_thunk_8c660760.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against retail SH-4 interpreter at thunk target',
    'scope': 'BRA with zero displacement and NOP delay slot, followed by BRA to 0x8c65fc92 with its NOP delay slot.',
    'limitations': ['The destination routine behavior is outside this eight-byte thunk.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'target': hex(TARGET), 'cases': cases,
}
(PROJECT / 'analysis' / 'branch_thunk_8c660760_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} branch thunk RAM/CPU comparisons passed')
