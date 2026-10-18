import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C600DDC
LENGTH = 10
RAM_SIZE = 16 * 1024 * 1024
POINTER_LITERAL = 0x8C600DF0
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('04 d3 2b 43 00 e5 ff 07 0e 26')
assert int.from_bytes(binary[POINTER_LITERAL - BASE:POINTER_LITERAL - BASE + 4], 'little') == 0x8C60167E

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'call_global_zero_8c600ddc.dll'))
native = library.sa_call_global_zero_8c600ddc
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(FUNCTION)
cases = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    callback = 0x8C700000 + rng.randrange(0x10000 // 2) * 2
    callback -= callback % 2
    callback_at = callback - 0x8C000000
    ram[callback_at:callback_at + 2] = (0x0009).to_bytes(2, 'little')
    pointer_at = POINTER_LITERAL - 0x8C000000
    ram[pointer_at:pointer_at + 4] = callback.to_bytes(4, 'little')
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = rng.getrandbits(32)
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    reference(expected_ram, expected_state, stop_pc=callback, hooks={callback: lambda _ram, _regs, sr: sr})
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual_state))
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch in case {case}'
    assert actual_state.r[5] == 0 and actual_state.pr == state.pr and actual_state.pc == callback
    cases.append({'case': case, 'callback': hex(callback), 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_call_global_zero_8c600ddc.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against the retail SH-4 interpreter, stopping at the external tail-callee entry',
    'scope': 'Exact tail-call setup: resolve global callback, place zero in R5 through the JMP delay slot, and transfer control via the pointer loaded from the literal pool.',
    'limitations': ['External callback behavior is stopped at entry and is outside this wrapper.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'callback_pointer_literal': hex(POINTER_LITERAL), 'cases': cases,
}
(PROJECT / 'analysis' / 'call_global_zero_8c600ddc_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} global callback setup RAM/CPU comparisons passed')
