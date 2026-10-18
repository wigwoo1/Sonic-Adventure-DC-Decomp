import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C0272CC
LENGTH = 10
RAM_SIZE = 16 * 1024 * 1024
ARGUMENT_LITERAL = 0x8C027340
CALLBACK_LITERAL = 0x8C027344
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('1d d3 02 e5 1b d6 2b 43 53 64')
assert int.from_bytes(binary[ARGUMENT_LITERAL - BASE:ARGUMENT_LITERAL - BASE + 4], 'little') == 0x8C026DA8
assert int.from_bytes(binary[CALLBACK_LITERAL - BASE:CALLBACK_LITERAL - BASE + 4], 'little') == 0x8C09852E

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'callback_tail_call_two_pointer_8c0272cc.dll'))
native = library.sa_callback_tail_call_two_pointer_8c0272cc
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(FUNCTION)
cases = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    callback = 0x8C700000 + rng.randrange(0x10000 // 2) * 2
    argument_pointer = rng.getrandbits(32)
    ram[CALLBACK_LITERAL - 0x8C000000:CALLBACK_LITERAL - 0x8C000000 + 4] = callback.to_bytes(4, 'little')
    ram[ARGUMENT_LITERAL - 0x8C000000:ARGUMENT_LITERAL - 0x8C000000 + 4] = argument_pointer.to_bytes(4, 'little')
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    reference(expected_ram, expected_state, stop_pc=callback)
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual_state))
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch in case {case}'
    assert actual_state.r[3] == callback and actual_state.r[4] == 2
    assert actual_state.r[5] == 2 and actual_state.r[6] == argument_pointer
    assert actual_state.pr == state.pr and actual_state.pc == callback
    cases.append({'case': case, 'callback': hex(callback), 'argument_pointer': hex(argument_pointer), 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_callback_tail_call_two_pointer_8c0272cc.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against retail SH-4 interpreter at callback entry',
    'scope': 'Load callback into R3, set R5=2, load pointer argument into R6, copy R5 to R4 in JMP delay slot, then tail-transfer preserving PR.',
    'limitations': ['The callback implementation is outside this ten-byte wrapper; register effects are recorded directly rather than assigning uncertain C argument types.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'literals': {'pointer_argument': hex(ARGUMENT_LITERAL), 'callback': hex(CALLBACK_LITERAL)}, 'cases': cases,
}
(PROJECT / 'analysis' / 'callback_tail_call_two_pointer_8c0272cc_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} callback tail-call RAM/CPU comparisons passed')
