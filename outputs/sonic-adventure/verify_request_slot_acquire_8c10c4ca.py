import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C10C4CA
LENGTH = 50
RAM_SIZE = 16 * 1024 * 1024
TABLE = 0x8C7AC10C
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
assert int.from_bytes(binary[0x8C10C500 - BASE:0x8C10C504 - BASE], 'little') == 0x280
assert int.from_bytes(binary[0x8C10C50C - BASE:0x8C10C510 - BASE], 'little') == TABLE

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'request_slot_acquire_8c10c4ca.dll'))
native = library.sa_request_slot_acquire_8c10c4ca
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE, hex(address)
    return result

def write32(ram, address, value):
    at = offset(address)
    ram[at:at + 4] = (value & 0xFFFFFFFF).to_bytes(4, 'little')

rng = random.Random(FUNCTION)
cases = []
for case in range(96):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    free_index = case % 17 - 1
    for index in range(16):
        record = bytearray(rng.getrandbits(8) for _ in range(0x28))
        record[0] = 0 if index == free_index else rng.choice([1, 0x33, 0x80, 0xFF])
        at = offset(TABLE + index * 0x28)
        ram[at:at + 0x28] = record

    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[15] = 0x8C780F00 + case * 8
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    stack_at = offset(state.r[15] - 4)
    ram[stack_at:stack_at + 4] = rng.getrandbits(32).to_bytes(4, 'little')

    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    reference(expected_ram, expected_state)
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual_state))

    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual_state) == bytes(expected_state), (
        f'CPU state mismatch in case {case}, free_index={free_index}'
    )
    cases.append({'case': case, 'free_index': free_index, 'ram_and_cpu_match': True})

source_path = PROJECT / 'source' / 'sa_request_slot_acquire_8c10c4ca.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source_path.read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against the retail SH-4 interpreter',
    'scope': 'Call the recovered free-slot search, initialize the returned 40-byte record, return its address, and reproduce stack/register/flag effects.',
    'limitations': [
        'The record field meanings are not established by this allocator alone; untouched bytes are preserved.',
        'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'
    ],
    'cases': cases,
}
(PROJECT / 'analysis' / 'request_slot_acquire_8c10c4ca_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} request-slot acquisition complete RAM/CPU comparisons passed')
