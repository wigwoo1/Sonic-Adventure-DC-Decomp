import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C66085A
LENGTH = 8
RAM_SIZE = 16 * 1024 * 1024
ARGUMENT_POINTER_LITERAL = 0x8C660880
CALLBACK_LITERAL = 0x8C660888
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('0b d3 08 d2 2b 43 22 64')
assert int.from_bytes(binary[ARGUMENT_POINTER_LITERAL - BASE:ARGUMENT_POINTER_LITERAL - BASE + 4], 'little') == 0x8C8A83B8
assert int.from_bytes(binary[CALLBACK_LITERAL - BASE:CALLBACK_LITERAL - BASE + 4], 'little') == 0x8C668E60

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'indirect_pointer_tail_call_8c66085a.dll'))
native = library.sa_indirect_pointer_tail_call_8c66085a
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
    argument_address = 0x8C710000 + rng.randrange(0x10000 // 4) * 4
    argument = rng.getrandbits(32)
    ram[CALLBACK_LITERAL - 0x8C000000:CALLBACK_LITERAL - 0x8C000000 + 4] = callback.to_bytes(4, 'little')
    ram[ARGUMENT_POINTER_LITERAL - 0x8C000000:ARGUMENT_POINTER_LITERAL - 0x8C000000 + 4] = argument_address.to_bytes(4, 'little')
    ram[offset(argument_address):offset(argument_address) + 4] = argument.to_bytes(4, 'little')
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
    assert actual_state.r[3] == callback and actual_state.r[4] == argument
    assert actual_state.r[2] == argument_address
    assert actual_state.pr == state.pr and actual_state.pc == callback
    cases.append({'case': case, 'callback': hex(callback), 'argument_address': hex(argument_address), 'argument': hex(argument), 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_indirect_pointer_tail_call_8c66085a.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against retail SH-4 interpreter at tail-call target',
    'scope': 'Load callback and argument pointer from literals, fetch argument through R2 in JMP delay slot without changing R2, then tail-transfer preserving PR.',
    'limitations': ['Callback implementation is outside this wrapper.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'literals': {'argument_pointer': hex(ARGUMENT_POINTER_LITERAL), 'callback': hex(CALLBACK_LITERAL)}, 'cases': cases,
}
(PROJECT / 'analysis' / 'indirect_pointer_tail_call_8c66085a_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} indirect pointer tail-call RAM/CPU comparisons passed')
