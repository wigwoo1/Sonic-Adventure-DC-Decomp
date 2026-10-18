import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C6514E0
LENGTH = 10
RAM_SIZE = 16 * 1024 * 1024
CALLBACK_LITERAL = 0x8C651500
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('07 d3 46 2f 32 62 2b 42 04 7f')
assert int.from_bytes(binary[CALLBACK_LITERAL - BASE:CALLBACK_LITERAL - BASE + 4], 'little') == 0x8C8A2FC4

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'sa_global_callback_tail_dispatch_8c6514e0.dll'))
native = library.sa_global_callback_tail_dispatch_8c6514e0
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(FUNCTION)
cases = []
for case in range(144):
    ram = bytearray([0xA5]) * RAM_SIZE
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION

    global_slot = 0x8C730000 + rng.randrange(0x10000 // 4) * 4
    callback = 0x8C720000 + rng.randrange(0x10000 // 2) * 2
    stack_pointer = 0x8C710000 + rng.randrange(0x10000 // 4) * 4
    state.r[4] = rng.getrandbits(32)
    if case == 0:
        stack_pointer = CALLBACK_LITERAL + 4  # store replaces the literal after R3 has loaded it
    elif case == 1:
        global_slot = 0x8C740000
        stack_pointer = global_slot + 4  # store replaces the callback pointer before R2 reads it
        state.r[4] = 0x8C721000
        callback = state.r[4]
    elif case == 2:
        stack_pointer = global_slot + 4  # the indirect callback slot aliases the stack write
        state.r[4] = 0x8C722000
        callback = state.r[4]

    state.r[15] = stack_pointer
    literal_at = CALLBACK_LITERAL - 0x8C000000
    ram[literal_at:literal_at + 4] = global_slot.to_bytes(4, 'little')
    slot_at = global_slot - 0x8C000000
    ram[slot_at:slot_at + 4] = callback.to_bytes(4, 'little')

    expected_target = state.r[4] if stack_pointer - 4 == global_slot else callback
    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    reference(expected_ram, expected_state, stop_pc=expected_target)

    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual_state))
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch in case {case}'
    assert actual_state.r[3] == global_slot, f'global slot register mismatch in case {case}'
    assert actual_state.r[2] == expected_target, f'callback pointer mismatch in case {case}'
    assert actual_state.r[15] == state.r[15], f'stack mismatch in case {case}'
    cases.append({'case': case, 'global_slot': hex(global_slot), 'callback': hex(expected_target), 'stack_pointer': hex(stack_pointer), 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_global_callback_tail_dispatch_8c6514e0.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against retail SH-4 interpreter at tail-call target',
    'scope': 'Load the global callback slot address, save R4 at [R15], load the callback target through the global slot, advance R15 by four in the jump delay slot, and tail-transfer.',
    'limitations': ['The callback body and runtime setup of the global callback slot are outside this 10-byte wrapper.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'callback_literal': hex(CALLBACK_LITERAL), 'cases': cases,
}
(PROJECT / 'analysis' / 'sa_global_callback_tail_dispatch_8c6514e0_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} global callback tail-call RAM/CPU comparisons passed')
