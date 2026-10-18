import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C05694A
LENGTH = 18
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('18 d6 62 63 4b 23 18 d4 32 26 42 62 5b 22 0b 00 22 24')

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'global_pair_flags_or_8c05694a.dll'))
native = library.sa_global_pair_flags_or_8c05694a
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE, hex(address)
    return result

def put32(ram, address, value):
    at = offset(address)
    ram[at:at + 4] = (value & 0xFFFFFFFF).to_bytes(4, 'little')

global_a = int.from_bytes(binary[0x8C0569AC - BASE:0x8C0569AC - BASE + 4], 'little')
global_b = int.from_bytes(binary[0x8C0569B4 - BASE:0x8C0569B4 - BASE + 4], 'little')
offset(global_a)
offset(global_b)
rng = random.Random(FUNCTION)
cases = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    for address in {global_a, global_b}:
        put32(ram, address, rng.choice([0, 0xFFFFFFFF, rng.getrandbits(32)]))
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = rng.choice([0, 1, 0xFFFFFFFF, 0x80000000, rng.getrandbits(32)])
    state.r[5] = rng.choice([0, 1, 0xFFFFFFFF, 0x55555555, rng.getrandbits(32)])
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
    assert bytes(actual_state) == bytes(expected_state), f'CPU state mismatch in case {case}'
    cases.append({'case': case, 'arg1': hex(state.r[4]), 'arg2': hex(state.r[5]), 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_global_pair_flags_or_8c05694a.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against the retail SH-4 interpreter',
    'scope': 'Exact 18-byte routine; ORs R4 and R5 into two independent words selected by the retail pointer literals, including the second store in the RTS delay slot.',
    'limitations': ['The meanings of the two global words remain unidentified.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'global_addresses': [hex(global_a), hex(global_b)],
    'cases': cases,
}
(PROJECT / 'analysis' / 'global_pair_flags_or_8c05694a_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} global pair flags-OR RAM/CPU comparisons passed')
