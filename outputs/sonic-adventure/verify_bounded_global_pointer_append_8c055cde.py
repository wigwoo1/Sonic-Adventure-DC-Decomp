import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C055CDE
LENGTH = 26
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('0a d6 08 e3 62 65 33 35 05 89 08 d0 08 45 46 05 62 63 01 73 32 26 0b 00 09 00')

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'bounded_global_pointer_append_8c055cde.dll'))
native = library.sa_bounded_global_pointer_append_8c055cde
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE, hex(address)
    return result

def put32(ram, address, value):
    at = offset(address)
    ram[at:at + 4] = (value & 0xFFFFFFFF).to_bytes(4, 'little')

count_address = int.from_bytes(binary[0x8C055D08 - BASE:0x8C055D08 - BASE + 4], 'little')
array_base = int.from_bytes(binary[0x8C055D0C - BASE:0x8C055D0C - BASE + 4], 'little')
offset(count_address)
offset(array_base + 7 * 4)
rng = random.Random(FUNCTION)
counts = [0, 1, 6, 7, 8, 9, 0xFFFFFFFF, 0x80000000]
cases = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    count = counts[case] if case < len(counts) else rng.randrange(0, 13)
    put32(ram, count_address, count)
    initial_slots = [rng.getrandbits(32) for _ in range(8)]
    for index, value in enumerate(initial_slots):
        put32(ram, array_base + index * 4, value)
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = rng.choice([0, 1, 0xFFFFFFFF, rng.getrandbits(32)])
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    reference(expected_ram, expected_state)
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual_state))
    assert actual_ram == expected_ram, f'RAM mismatch case={case} count={count}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch case={case} count={count}'
    cases.append({'case': case, 'initial_count': count, 'argument': hex(state.r[4]), 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_bounded_global_pointer_append_8c055cde.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against the retail SH-4 interpreter',
    'scope': 'Exact 26-byte bounded append helper; covers counts below, at, and above eight, count overflow values, array-slot writes, count increments, and branch T-bit effects.',
    'limitations': ['The global count and array are identified by retail pointer literals; higher-level owner semantics remain unknown.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'count_address': hex(count_address), 'array_base': hex(array_base),
    'cases': cases,
}
(PROJECT / 'analysis' / 'bounded_global_pointer_append_8c055cde_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} bounded global pointer-append RAM/CPU comparisons passed')
