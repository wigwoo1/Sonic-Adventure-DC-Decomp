import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C01C28E
LENGTH = 8
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('2b d3 29 d7 2b 43 09 00')
assert int.from_bytes(binary[0x8C01C33C - BASE:0x8C01C340 - BASE], 'little') == 0x8C0569F0
assert int.from_bytes(binary[0x8C01C338 - BASE:0x8C01C33C - BASE], 'little') == 0x00400002

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'startup_tail_transfer_8c01c28e.dll'))
native = library.sa_startup_tail_transfer_8c01c28e
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
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    reference(expected_ram, expected_state, stop_pc=0x8C0569F0)
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual_state))
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch in case {case}'
    assert actual_state.r[3] == 0x8C0569F0 and actual_state.r[7] == 0x00400002
    assert actual_state.pr == state.pr and actual_state.pc == 0x8C0569F0
    cases.append({'case': case, 'target': hex(actual_state.pc), 'r7': hex(actual_state.r[7]), 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_startup_tail_transfer_8c01c28e.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against the retail SH-4 interpreter at tail-transfer target',
    'scope': 'Two PC-relative literal loads into R3/R7, JMP @R3, and NOP delay slot.',
    'limitations': ['The target routine at 0x8c0569f0 is outside this eight-byte transfer wrapper.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'literals': {'callback_target_pointer': '0x8c01c33c', 'second_literal': '0x8c01c338'}, 'cases': cases,
}
(PROJECT / 'analysis' / 'startup_tail_transfer_8c01c28e_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} startup tail-transfer RAM/CPU comparisons passed')
