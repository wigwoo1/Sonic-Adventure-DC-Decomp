import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C65BC9C
LENGTH = 12
RAM_SIZE = 16 * 1024 * 1024
CALLBACK_LITERAL = 0x8C65BD14
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('fc 7f 42 2f 1c d3 41 54 2b 43 04 7f')
assert int.from_bytes(binary[CALLBACK_LITERAL - BASE:CALLBACK_LITERAL - BASE + 4], 'little') == 0x8C667702

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'sa_callback_tail_load_object_word_8c65bc9c.dll'))
native = library.sa_callback_tail_load_object_word_8c65bc9c
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

    callback = 0x8C720000 + rng.randrange(0x10000 // 2) * 2
    object_address = 0x8C700000 + rng.randrange(0x10000 // 4) * 4
    stack_pointer = 0x8C710000 + rng.randrange(0x10000 // 4) * 4
    if case == 0:
        stack_pointer = CALLBACK_LITERAL + 4  # push overwrites callback literal before it is loaded
    elif case == 1:
        object_address = stack_pointer - 8  # object's +4 word aliases the pushed R4
    elif case == 2:
        object_address = CALLBACK_LITERAL - 4  # object read aliases callback-literal storage
    state.r[4] = object_address
    state.r[15] = stack_pointer
    ram[CALLBACK_LITERAL - 0x8C000000:CALLBACK_LITERAL - 0x8C000000 + 4] = callback.to_bytes(4, 'little')
    object_word_at = object_address + 4 - 0x8C000000
    object_word = callback if case == 2 else rng.getrandbits(32)
    ram[object_word_at:object_word_at + 4] = object_word.to_bytes(4, 'little')
    if case == 0:
        # The push is first, so this becomes the actual callback loaded by the wrapper.
        callback = state.r[4]
    elif case == 2:
        callback = object_word
    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    reference(expected_ram, expected_state, stop_pc=callback)

    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual_state))
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch in case {case}'
    assert actual_state.r[15] == state.r[15], f'stack pointer mismatch in case {case}'
    assert actual_state.pc == callback, f'callback target mismatch in case {case}'
    cases.append({'case': case, 'callback': hex(callback), 'object': hex(object_address), 'stack_pointer': hex(stack_pointer), 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_callback_tail_load_object_word_8c65bc9c.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against retail SH-4 interpreter at tail-call target',
    'scope': 'Temporarily save R4 at R15-4, load callback pointer from 0x8c65bd14, load the object word at R4+4 into R4, and tail-transfer; delay slot restores R15.',
    'limitations': ['Callback body behavior is outside this 12-byte wrapper.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'callback_literal': hex(CALLBACK_LITERAL), 'cases': cases,
}
(PROJECT / 'analysis' / 'sa_callback_tail_load_object_word_8c65bc9c_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} callback tail-call RAM/CPU comparisons passed')
