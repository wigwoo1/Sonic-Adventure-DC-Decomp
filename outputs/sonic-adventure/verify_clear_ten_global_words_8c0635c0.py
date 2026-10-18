import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C0635C0
LENGTH = 24
RAM_SIZE = 16 * 1024 * 1024
TARGET = 0x8C78C564
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('4f d6 00 e4 0a e5 42 26 fe 75 04 76 58 25 42 26 f9 8f 04 76 0b 00 09 00')
assert int.from_bytes(binary[0x8C063700 - BASE:0x8C063700 - BASE + 4], 'little') == TARGET

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'clear_ten_global_words_8c0635c0.dll'))
native = library.sa_clear_ten_global_words_8c0635c0
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE, hex(address)
    return result

rng = random.Random(FUNCTION)
cases = []
for case in range(96):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    at = offset(TARGET)
    initial = rng.randbytes(40)
    ram[at:at + 40] = initial
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    reference(expected_ram, expected_state)
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual_state))
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch in case {case}'
    assert actual_ram[at:at + 40] == bytes(40), f'not all ten words cleared in case {case}'
    cases.append({'case': case, 'initial_region_sha256': hashlib.sha256(initial).hexdigest(), 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_clear_ten_global_words_8c0635c0.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against the retail SH-4 interpreter',
    'scope': 'Exact 24-byte loop; clears ten consecutive words at the retail literal target and checks pointer/count registers, branch T bit, and delay-slot pointer increment.',
    'limitations': ['The cleared array field semantics remain unidentified.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'target_address': hex(TARGET), 'cases': cases,
}
(PROJECT / 'analysis' / 'clear_ten_global_words_8c0635c0_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} ten-word clear RAM/CPU comparisons passed')
