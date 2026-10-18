"""Verify the indirect tail transfer at the input channel callback thunk."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
BASE, ADDRESS, LENGTH = 0x8C010000, 0x8C647908, 6
LITERAL, RAM_SIZE = 0x8C64797C, 16 * 1024 * 1024
BINARY = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
SPAN = BINARY[ADDRESS - BASE:ADDRESS - BASE + LENGTH]
assert SPAN == bytes.fromhex('1c d3 2b 43 09 00')
LIBRARY = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/sa_input_channel_callback_thunk_8c647908.dll'))
FUNCTION = LIBRARY.sa_input_channel_callback_thunk_8c647908
FUNCTION.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
FUNCTION.restype = None
rng = random.Random(ADDRESS)
cases = []
for case in range(256):
    initial = bytearray(b'\xA5') * RAM_SIZE
    initial[BASE - 0x8C000000:BASE - 0x8C000000 + len(BINARY)] = BINARY
    target = 0x8C00E000 + case * 4
    struct.pack_into('<I', initial, LITERAL & 0xFFFFFF, target)
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.sr = rng.getrandbits(32)
    state.pr = 0x8CFED000 + case * 4
    state.pc = ADDRESS
    retail_ram, native_ram = bytearray(initial), bytearray(initial)
    expected, actual = State.from_buffer_copy(bytes(state)), State.from_buffer_copy(bytes(state))
    observed = []
    def callback(_ram, regs, sr):
        observed.append((list(regs), sr))
        return sr
    steps = reference(retail_ram, expected, hooks={target: callback})
    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
    FUNCTION(view, ctypes.byref(actual))
    assert steps == 3
    assert len(observed) == 1
    assert retail_ram == native_ram, f'RAM mismatch case {case}'
    assert list(actual.r) == observed[0][0], f'register mismatch case {case}'
    assert (actual.sr, actual.pr, actual.pc) == (observed[0][1], state.pr, target), f'control state mismatch case {case}'
    cases.append({'case': case, 'target': hex(target), 'retail_instructions': steps,
                  'full_ram_and_tail_state_match': True})

source = PROJECT / 'source/sa_input_channel_callback_thunk_8c647908.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(SPAN).hexdigest(),
    'binary_sha256': hashlib.sha256(BINARY).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'Complete 16 MiB RAM and all GPR/SR/PR/PC state at the indirect tail-call boundary against retail SH-4 execution.',
    'scope': 'Loads the callback target from 0x8c64797c, places it in R3, and tail-jumps to it while preserving caller PR and stack.',
    'limitations': ['The target callback body is a separate routine.', 'Verification uses the project SH-4 interpreter, not Dreamcast hardware or complete-game execution.'],
    'cases': cases,
}
(PROJECT / 'analysis/sa_input_channel_callback_thunk_8c647908_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{len(cases)} input channel thunk RAM/CPU comparisons passed')