import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C05F000
LENGTH = 12
TARGET = 0x8C05FB0A
LITERAL = 0x8C05F100
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('43 65 46 2f 3e d4 00 e6 7f a5 04 7f')
assert int.from_bytes(binary[LITERAL - BASE:LITERAL - BASE + 4], 'little') == 0x8C05F996

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'sa_helper_tail_dispatch_global_arg_8c05f000.dll'))
native = library.sa_helper_tail_dispatch_global_arg_8c05f000
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(FUNCTION)
cases = []
for case in range(160):
    ram = bytearray([0xA5]) * RAM_SIZE
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = 0x8C700000 + rng.randrange(0x10000 // 4) * 4
    state.r[15] = 0x8C800000 + rng.randrange(0x10000 // 4) * 4
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION

    global_argument = 0x8C710000 + rng.randrange(0x10000 // 4) * 4
    ram[LITERAL - 0x8C000000:LITERAL - 0x8C000000 + 4] = global_argument.to_bytes(4, 'little')
    if case == 0:
        state.r[15] = LITERAL + 4  # the predecrement save overwrites the literal before it is loaded

    expected_argument = state.r[4] if state.r[15] - 4 == LITERAL else global_argument
    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    reference(expected_ram, expected_state, stop_pc=TARGET)

    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual_state))
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch in case {case}'
    assert actual_state.r[4] == expected_argument and actual_state.r[5] == state.r[4]
    assert actual_state.r[6] == 0 and actual_state.r[15] == state.r[15]
    assert actual_state.pc == TARGET and actual_state.pr == state.pr
    cases.append({'case': case, 'original_argument': hex(state.r[4]), 'helper_global_argument': hex(expected_argument), 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_helper_tail_dispatch_global_arg_8c05f000.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against retail SH-4 interpreter at direct helper tail-call target',
    'scope': 'Copy original R4 to R5, save it at R15-4, load R4 from literal 0x8c05f100, set R6=0, and branch to 0x8c05fb0a while restoring R15 in the delay slot.',
    'limitations': ['The destination helper at 0x8c05fb0a is a separate routine.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'literal': hex(LITERAL), 'target': hex(TARGET), 'cases': cases,
}
(PROJECT / 'analysis' / 'sa_helper_tail_dispatch_global_arg_8c05f000_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} global argument tail-dispatch RAM/CPU comparisons passed')
