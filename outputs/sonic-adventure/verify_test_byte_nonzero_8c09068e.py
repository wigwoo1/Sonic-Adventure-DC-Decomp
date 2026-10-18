import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C09068E
LENGTH = 14
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('0c d0 4c 03 3c 63 38 23 29 00 0b 00 01 ca')
assert int.from_bytes(binary[0x8C0906C0 - BASE:0x8C0906C0 - BASE + 4], 'little') == 0x8C799920

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'test_byte_nonzero_8c09068e.dll'))
native = library.sa_test_byte_nonzero_8c09068e
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE, hex(address)
    return result

rng = random.Random(FUNCTION)
cases = []
values = [0, 1, 0x7F, 0x80, 0xFF] + [rng.randrange(256) for _ in range(59)]
for case, value in enumerate(values):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    displacement = rng.randrange(256)
    address = 0x8C799920 + displacement
    ram[offset(address)] = value
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = displacement
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
    assert bytes(actual_state) == bytes(expected_state), (
        f'CPU mismatch in case {case}: value={value:02x}, '
        f'actual_r={list(actual_state.r)!r}, expected_r={list(expected_state.r)!r}, '
        f'actual_sr={actual_state.sr:08x}, expected_sr={expected_state.sr:08x}')
    assert actual_state.r[0] == int(value != 0), f'wrong boolean result in case {case}'
    assert actual_state.r[3] == value, f'wrong loaded byte in case {case}'
    cases.append({'case': case, 'table_offset': displacement, 'byte': value, 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_test_byte_nonzero_8c09068e.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against the retail SH-4 interpreter',
    'scope': 'Exact literal table-base load, indexed byte load at base+R4, zero extension, T-bit zero test, MOVT result and XOR #1 RTS delay slot.',
    'limitations': ['The table’s higher-level meaning remains unresolved.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'cases': cases,
}
(PROJECT / 'analysis' / 'test_byte_nonzero_8c09068e_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} byte nonzero RAM/CPU comparisons passed')
