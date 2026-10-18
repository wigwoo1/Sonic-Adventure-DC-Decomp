import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C65C190
LENGTH = 14
TARGET = 0x8C65C026
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('41 84 02 88 01 8b 46 af 09 00 0b 00 09 00')

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'sa_byte_equal_two_tail_branch_8c65c190.dll'))
native = library.sa_byte_equal_two_tail_branch_8c65c190
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(FUNCTION)
values = [0, 1, 2, 3, 0x7F, 0x80, 0x81, 0xFE, 0xFF] + [rng.randrange(256) for _ in range(151)]
cases = []
for case, byte_value in enumerate(values):
    ram = bytearray([0xA5]) * RAM_SIZE
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    object_address = 0x8C700000 + rng.randrange(0x10000 // 4) * 4
    state.r[4] = object_address
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    ram[object_address + 1 - 0x8C000000] = byte_value
    matched = byte_value == 2
    stop_pc = TARGET if matched else state.pr

    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    reference(expected_ram, expected_state, stop_pc=stop_pc)

    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual_state))
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch in case {case}'
    assert actual_state.r[0] == ((byte_value if byte_value < 0x80 else byte_value - 0x100) & 0xFFFFFFFF)
    assert actual_state.pc == stop_pc and (actual_state.sr & 1) == int(matched)
    cases.append({'case': case, 'input_byte': hex(byte_value), 'branch_taken': matched, 'stop_pc': hex(stop_pc), 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_byte_equal_two_tail_branch_8c65c190.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state on both taken and return paths against retail SH-4 interpreter',
    'scope': 'Sign-extend the byte at R4+1, set the T flag according to equality with 2, branch to 0x8c65c026 when equal, otherwise return through PR.',
    'limitations': ['The handler at 0x8c65c026 is outside this routine verification.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'target': hex(TARGET), 'cases': cases,
}
(PROJECT / 'analysis' / 'sa_byte_equal_two_tail_branch_8c65c190_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} byte equality branch RAM/CPU comparisons passed')
