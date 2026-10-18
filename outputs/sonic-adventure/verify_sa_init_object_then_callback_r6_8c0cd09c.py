import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C0CD09C
LENGTH = 12
RAM_SIZE = 16 * 1024 * 1024
CALLBACK_LITERAL = 0x8C0CD174
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('01 e3 30 24 00 e0 34 d3 2b 43 67 81')
assert int.from_bytes(binary[CALLBACK_LITERAL - BASE:CALLBACK_LITERAL - BASE + 4], 'little') == 0x8C0D1E38

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'sa_init_object_then_callback_r6_8c0cd09c.dll'))
native = library.sa_init_object_then_callback_r6_8c0cd09c
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(FUNCTION)
cases = []
for case in range(160):
    ram = bytearray([0xA5]) * RAM_SIZE
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION

    callback = 0x8C720000 + rng.randrange(0x10000 // 2) * 2
    object1 = 0x8C700000 + rng.randrange(0x10000 // 2) * 2
    object2 = 0x8C710000 + rng.randrange(0x10000 // 2) * 2
    if case == 0:
        object1 = CALLBACK_LITERAL  # byte store changes the callback pointer before it is loaded
        callback = 0x8C720200
    elif case == 1:
        object2 = CALLBACK_LITERAL - 14  # delay-slot halfword store changes the literal after it was read
    elif case == 2:
        object2 = object1 - 14  # later halfword store covers the earlier byte store
    elif case == 3:
        object1 = CALLBACK_LITERAL + 1  # byte store changes a higher callback-pointer byte
        callback = 0x8C722202

    state.r[4] = object1
    state.r[6] = object2
    literal_at = CALLBACK_LITERAL - 0x8C000000
    ram[literal_at:literal_at + 4] = callback.to_bytes(4, 'little')
    if case == 0:
        callback = (callback & 0xFFFFFF00) | 1
    elif case == 3:
        callback = (callback & 0xFFFF00FF) | (1 << 8)

    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    reference(expected_ram, expected_state, stop_pc=callback)

    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual_state))
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch in case {case}'
    assert actual_state.r[0] == 0 and actual_state.r[3] == callback
    assert actual_state.r[4] == object1 and actual_state.r[6] == object2
    assert actual_state.pc == callback
    cases.append({'case': case, 'callback': hex(callback), 'first_object': hex(object1), 'second_object': hex(object2), 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_init_object_then_callback_r6_8c0cd09c.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against retail SH-4 interpreter at tail-call target',
    'scope': 'Write byte 1 to the object in R4, clear R0, load the callback pointer at 0x8c0cd174, store a zero halfword at offset 14 of the object in R6 in the jump delay slot, and tail-transfer.',
    'limitations': ['Callback body behavior is outside this 12-byte wrapper.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'callback_literal': hex(CALLBACK_LITERAL), 'cases': cases,
}
(PROJECT / 'analysis' / 'sa_init_object_then_callback_r6_8c0cd09c_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} R6 object initializer callback RAM/CPU comparisons passed')
