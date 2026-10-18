import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C10C4A4
LENGTH = 38
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
assert int.from_bytes(binary[0x8C10C500 - BASE:0x8C10C504 - BASE], 'little') == 0x280
assert int.from_bytes(binary[0x8C10C50C - BASE:0x8C10C510 - BASE], 'little') == 0x8C7AC10C

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'request_slot_find_8c10c4a4.dll'))
native = library.sa_request_slot_find_8c10c4a4
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
table = 0x8C7AC10C
cases = []
for case in range(64):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    free_index = case % 17 - 1
    # Exercise every possible free slot and the fully occupied table, with
    # randomized non-marker bytes to ensure only byte zero identifies a slot.
    for index in range(16):
        record = bytearray(rng.getrandbits(8) for _ in range(0x28))
        record[0] = 0 if index == free_index else rng.choice([1, 0x7F, 0xFF])
        at = offset(table + index * 0x28)
        ram[at:at + 0x28] = record

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
    assert bytes(actual_state) == bytes(expected_state), (
        f'CPU state mismatch in case {case}, free_index={free_index}, '
        f'registers={[ (i, hex(actual_state.r[i]), hex(expected_state.r[i])) for i in range(16) if actual_state.r[i] != expected_state.r[i] ]}, '
        f'sr={actual_state.sr:#x}/{expected_state.sr:#x}, '
        f'pr={actual_state.pr:#x}/{expected_state.pr:#x}, pc={actual_state.pc:#x}/{expected_state.pc:#x}'
    )
    cases.append({'case': case, 'free_index': free_index, 'ram_and_cpu_match': True})

source_path = PROJECT / 'source' / 'sa_request_slot_find_8c10c4a4.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source_path.read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against the retail SH-4 interpreter',
    'scope': 'Search sixteen 40-byte records for a zero first byte, return the matching record or zero, and reproduce final register/flag effects.',
    'limitations': [
        'The 16-entry count and 40-byte record stride are fixed by adjacent retail literal data.',
        'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'
    ],
    'cases': cases,
}
(PROJECT / 'analysis' / 'request_slot_find_8c10c4a4_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} request-slot search complete RAM/CPU comparisons passed')
