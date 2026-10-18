import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C65BDCA
LENGTH = 8
RAM_SIZE = 16 * 1024 * 1024
CALLBACK_LITERAL = 0x8C65BE48
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('1f d3 64 e6 2b 43 00 e5')
assert int.from_bytes(binary[CALLBACK_LITERAL - BASE:CALLBACK_LITERAL - BASE + 4], 'little') == 0x8C10D31C

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'callback_tail_call_zero_hundred_8c65bdca.dll'))
native = library.sa_callback_tail_call_zero_hundred_8c65bdca
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(FUNCTION)
cases = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    callback = 0x8C700000 + rng.randrange(0x10000 // 2) * 2
    literal_at = CALLBACK_LITERAL - 0x8C000000
    ram[literal_at:literal_at + 4] = callback.to_bytes(4, 'little')
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
    assert actual_state.r[3] == callback and actual_state.r[5] == 0 and actual_state.r[6] == 100
    assert actual_state.pr == state.pr and actual_state.pc == callback
    cases.append({'case': case, 'callback': hex(callback), 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_callback_tail_call_zero_hundred_8c65bdca.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against retail SH-4 interpreter at tail-call target',
    'scope': 'Load callback into R3, set R6=100, set R5=0 in JMP delay slot, and tail-transfer while preserving PR.',
    'limitations': ['Callback body behavior is outside this eight-byte wrapper.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'callback_literal': hex(CALLBACK_LITERAL), 'cases': cases,
}
(PROJECT / 'analysis' / 'callback_tail_call_zero_hundred_8c65bdca_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} callback tail-call RAM/CPU comparisons passed')
