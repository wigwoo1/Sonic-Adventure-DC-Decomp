import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C63A980
LENGTH = 8
RAM_SIZE = 16 * 1024 * 1024
CALLBACK_LITERAL = 0x8C63A988
ARGUMENT_LITERAL = 0x8C63A98C
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('01 d0 02 d4 2b 40 09 00')
assert int.from_bytes(binary[CALLBACK_LITERAL - BASE:CALLBACK_LITERAL - BASE + 4], 'little') == 0x8C10EB50
assert int.from_bytes(binary[ARGUMENT_LITERAL - BASE:ARGUMENT_LITERAL - BASE + 4], 'little') == 0x8C89033C

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'literal_pair_tail_call_8c63a980.dll'))
native = library.sa_literal_pair_tail_call_8c63a980
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(FUNCTION)
cases = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    callback = 0x8C700000 + rng.randrange(0x10000 // 2) * 2
    argument = rng.getrandbits(32)
    callback_at = CALLBACK_LITERAL - 0x8C000000
    argument_at = ARGUMENT_LITERAL - 0x8C000000
    ram[callback_at:callback_at + 4] = callback.to_bytes(4, 'little')
    ram[argument_at:argument_at + 4] = argument.to_bytes(4, 'little')
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
    assert actual_state.r[0] == callback and actual_state.r[4] == argument
    assert actual_state.pr == state.pr and actual_state.pc == callback
    cases.append({'case': case, 'callback': hex(callback), 'argument': hex(argument), 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_literal_pair_tail_call_8c63a980.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against retail SH-4 interpreter at indirect tail-call target',
    'scope': 'Load callback into R0 and argument into R4 from adjacent literals, JMP @R0 with NOP delay slot, and preserve PR.',
    'limitations': ['Callback body behavior is outside this eight-byte wrapper.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'literals': {'callback': hex(CALLBACK_LITERAL), 'argument': hex(ARGUMENT_LITERAL)}, 'cases': cases,
}
(PROJECT / 'analysis' / 'literal_pair_tail_call_8c63a980_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} literal-pair tail-call RAM/CPU comparisons passed')
