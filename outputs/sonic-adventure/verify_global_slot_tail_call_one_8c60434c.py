import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C60434C
LENGTH = 8
RAM_SIZE = 16 * 1024 * 1024
SLOT_LITERAL = 0x8C604368
SLOT_ADDRESS = 0x8C0000E0
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('06 d2 22 63 2b 43 01 e4')
assert int.from_bytes(binary[SLOT_LITERAL - BASE:SLOT_LITERAL - BASE + 4], 'little') == SLOT_ADDRESS

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'global_slot_tail_call_one_8c60434c.dll'))
native = library.sa_global_slot_tail_call_one_8c60434c
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
    callback_at = offset(callback)
    ram[callback_at:callback_at + 2] = (0x0009).to_bytes(2, 'little')
    slot_at = offset(SLOT_ADDRESS)
    ram[slot_at:slot_at + 4] = callback.to_bytes(4, 'little')
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
    assert actual_state.r[2] == SLOT_ADDRESS and actual_state.r[3] == callback
    assert actual_state.r[4] == 1 and actual_state.pr == state.pr and actual_state.pc == callback
    cases.append({'case': case, 'callback': hex(callback), 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_global_slot_tail_call_one_8c60434c.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against retail SH-4 interpreter at callback entry',
    'scope': 'Load global slot address from a PC-relative literal, dereference callback target, set R4 to one in the JMP delay slot, and transfer control preserving PR.',
    'limitations': ['Callback body behavior is outside this wrapper.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'slot_literal': hex(SLOT_LITERAL), 'global_slot': hex(SLOT_ADDRESS), 'cases': cases,
}
(PROJECT / 'analysis' / 'global_slot_tail_call_one_8c60434c_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} global-slot callback RAM/CPU comparisons passed')
