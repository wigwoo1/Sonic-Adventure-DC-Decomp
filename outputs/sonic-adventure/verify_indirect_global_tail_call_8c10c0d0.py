import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C10C0D0
LENGTH = 10
RAM_SIZE = 16 * 1024 * 1024
ARGUMENT_LITERAL = 0x8C10C0DC
POINTER_SLOT_LITERAL = 0x8C10C0E0
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('02 d7 03 d0 02 60 2b 40 09 00')
assert int.from_bytes(binary[ARGUMENT_LITERAL - BASE:ARGUMENT_LITERAL - BASE + 4], 'little') == 0
assert int.from_bytes(binary[POINTER_SLOT_LITERAL - BASE:POINTER_SLOT_LITERAL - BASE + 4], 'little') == 0x8C0000B0

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'indirect_global_tail_call_8c10c0d0.dll'))
native = library.sa_indirect_global_tail_call_8c10c0d0
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
    argument = rng.getrandbits(32)
    slot = 0x8C700000 + rng.randrange(0x10000 // 4) * 4
    callback = 0x8C710000 + rng.randrange(0x10000 // 2) * 2
    ram[ARGUMENT_LITERAL - 0x8C000000:ARGUMENT_LITERAL - 0x8C000000 + 4] = argument.to_bytes(4, 'little')
    ram[POINTER_SLOT_LITERAL - 0x8C000000:POINTER_SLOT_LITERAL - 0x8C000000 + 4] = slot.to_bytes(4, 'little')
    ram[offset(slot):offset(slot) + 4] = callback.to_bytes(4, 'little')
    ram[offset(callback):offset(callback) + 2] = (0x0009).to_bytes(2, 'little')
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
    assert actual_state.r[7] == argument and actual_state.r[0] == callback
    assert actual_state.pr == state.pr and actual_state.pc == callback
    cases.append({'case': case, 'argument': hex(argument), 'slot': hex(slot), 'callback': hex(callback), 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_indirect_global_tail_call_8c10c0d0.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against retail SH-4 interpreter at indirect tail-call target',
    'scope': 'Load R7 argument and pointer-slot address from literals, dereference callback into R0, tail-jump through R0, preserve PR.',
    'limitations': ['The callback body is outside this ten-byte wrapper.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'literals': {'argument': hex(ARGUMENT_LITERAL), 'callback_slot': hex(POINTER_SLOT_LITERAL)}, 'cases': cases,
}
(PROJECT / 'analysis' / 'indirect_global_tail_call_8c10c0d0_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} indirect global tail-call RAM/CPU comparisons passed')
