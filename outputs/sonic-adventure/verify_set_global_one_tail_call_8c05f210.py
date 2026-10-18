import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C05F210
LENGTH = 10
RAM_SIZE = 16 * 1024 * 1024
CALLBACK_LITERAL = 0x8C05F234
TARGET_LITERAL = 0x8C05F23C
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('08 d1 01 e3 09 d2 2b 41 32 22')
assert int.from_bytes(binary[CALLBACK_LITERAL - BASE:CALLBACK_LITERAL - BASE + 4], 'little') == 0x8C107BB0
assert int.from_bytes(binary[TARGET_LITERAL - BASE:TARGET_LITERAL - BASE + 4], 'little') == 0x8C1BA1E4

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'set_global_one_tail_call_8c05f210.dll'))
native = library.sa_set_global_one_tail_call_8c05f210
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE, hex(address)
    return result

rng = random.Random(FUNCTION)
cases = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    callback = 0x8C700000 + rng.randrange(0x10000 // 2) * 2
    target = 0x8C710000 + rng.randrange(0x10000 // 4) * 4
    callback_at = CALLBACK_LITERAL - 0x8C000000
    target_literal_at = TARGET_LITERAL - 0x8C000000
    ram[callback_at:callback_at + 4] = callback.to_bytes(4, 'little')
    ram[target_literal_at:target_literal_at + 4] = target.to_bytes(4, 'little')
    target_at = offset(target)
    initial = rng.randbytes(4)
    ram[target_at:target_at + 4] = initial
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
    assert actual_ram[target_at:target_at + 4] == (1).to_bytes(4, 'little')
    assert actual_state.r[1] == callback and actual_state.r[2] == target and actual_state.r[3] == 1
    assert actual_state.pr == state.pr and actual_state.pc == callback
    cases.append({'case': case, 'callback': hex(callback), 'target': hex(target), 'initial': initial.hex(), 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_set_global_one_tail_call_8c05f210.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against retail SH-4 interpreter at tail-call target',
    'scope': 'Load callback and destination pointer through literals, set R3=1, store it at *R2 in the JMP delay slot, and transfer control preserving PR.',
    'limitations': ['Behavior of the callback at the transfer target is outside this ten-byte wrapper.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'literals': {'callback': hex(CALLBACK_LITERAL), 'global_pointer': hex(TARGET_LITERAL)}, 'cases': cases,
}
(PROJECT / 'analysis' / 'set_global_one_tail_call_8c05f210_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} global store/tail-call RAM/CPU comparisons passed')
