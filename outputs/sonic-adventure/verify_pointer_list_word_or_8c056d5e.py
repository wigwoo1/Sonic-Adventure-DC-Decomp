import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C056D5E
LENGTH = 24
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('42 63 38 23 06 89 46 63 32 62 5b 22 22 23 42 63 38 23 f8 8b 0b 00 09 00')

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'pointer_list_word_or_8c056d5e.dll'))
native = library.sa_pointer_list_word_or_8c056d5e
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE, hex(address)
    return result

def put32(ram, address, value):
    at = offset(address)
    ram[at:at + 4] = (value & 0xFFFFFFFF).to_bytes(4, 'little')

rng = random.Random(FUNCTION)
cases = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    count = [0, 1, 2, 7, 16][case % 5] if case < 25 else rng.randrange(0, 17)
    list_address = 0x8C750000 + case * 0x100
    targets = [0x8C780000 + case * 0x100 + i * 4 for i in range(count)]
    for i, target in enumerate(targets):
        put32(ram, list_address + i * 4, target)
        put32(ram, target, rng.getrandbits(32))
    put32(ram, list_address + count * 4, 0)
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = list_address
    state.r[5] = rng.choice([0, 1, 0xFFFFFFFF, 0x80000000, rng.getrandbits(32)])
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    reference(expected_ram, expected_state)
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual_state))
    assert actual_ram == expected_ram, f'RAM mismatch in case {case} count={count}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU state mismatch in case {case} count={count}'
    cases.append({'case': case, 'entry_count': count, 'mask': hex(state.r[5]), 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_pointer_list_word_or_8c056d5e.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against the retail SH-4 interpreter',
    'scope': 'Exact 24-byte sentinel-terminated list helper; tests empty lists and up to 16 pointer entries, checking list-pointer advancement, T-bit exit state, and all pointed-word updates.',
    'limitations': ['The function assumes a valid zero-terminated list of mapped word pointers.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'cases': cases,
}
(PROJECT / 'analysis' / 'pointer_list_word_or_8c056d5e_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} pointer-list word-OR RAM/CPU comparisons passed')
