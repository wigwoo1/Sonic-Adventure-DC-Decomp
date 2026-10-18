import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C08A5BE
LENGTH = 14
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('3b d1 33 d4 12 63 42 62 3c 32 0b 00 22 24')

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'add_fixed_globals_8c08a5be.dll'))
native = library.sa_add_fixed_globals_8c08a5be
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
    # The PC-relative literal slots contain the retail pointers; initialize both
    # pointees independently, including aliases, to exercise the actual indirection.
    source = struct.unpack_from('<I', binary, 0x8C08A6AC - BASE)[0]
    destination = struct.unpack_from('<I', binary, 0x8C08A690 - BASE)[0]
    value = rng.choice([0, 1, 0xFFFFFFFF, 0x80000000, rng.getrandbits(32)])
    old = rng.choice([0, 0xFFFFFFFF, rng.getrandbits(32)])
    struct.pack_into('<I', ram, offset(source), value)
    struct.pack_into('<I', ram, offset(destination), old)
    if case % 8 == 0:
        struct.pack_into('<I', ram, offset(destination), value)
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
    assert bytes(actual_state) == bytes(expected_state), f'CPU state mismatch in case {case}'
    cases.append({'case': case, 'source_value': hex(value), 'initial_destination': hex(old), 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_add_fixed_globals_8c08a5be.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against the retail SH-4 interpreter',
    'scope': 'Exact 14-byte helper; follows both PC-relative pointer literals, adds the source word to the destination word modulo 2^32, stores the sum, and checks SH-4 register clobbers and RTS behavior.',
    'limitations': ['The semantic names and ownership of the two global words remain unidentified.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'cases': cases,
}
(PROJECT / 'analysis' / 'add_fixed_globals_8c08a5be_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} fixed-global addition RAM/CPU comparisons passed')
