import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C056926
LENGTH = 18
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('21 d1 21 d0 12 63 22 d1 32 20 20 d3 32 62 0b 00 22 21')

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'copy_global_pair_8c056926.dll'))
native = library.sa_copy_global_pair_8c056926
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE, hex(address)
    return result

def put32(ram, address, value):
    at = offset(address)
    ram[at:at + 4] = (value & 0xFFFFFFFF).to_bytes(4, 'little')

literal_addresses = [0x8C0569AC, 0x8C0569B0, 0x8C0569B4, 0x8C0569B8]
pointers = [int.from_bytes(binary[a - BASE:a - BASE + 4], 'little') for a in literal_addresses]
for pointer in pointers:
    offset(pointer)

rng = random.Random(FUNCTION)
cases = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    # Initialize the words addressed by the four retail pointer literals. Where
    # pointers alias, the same RAM word is intentionally initialized once.
    for address in set(pointers):
        put32(ram, address, rng.getrandbits(32))
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
    cases.append({'case': case, 'pointer_values': [hex(x) for x in pointers], 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_copy_global_pair_8c056926.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against the retail SH-4 interpreter',
    'scope': 'Exact 18-byte helper; copies two words through four fixed retail pointer literals and checks the second write in the RTS delay slot.',
    'limitations': ['The copied fields and their owning structures remain unidentified.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'literal_addresses': [hex(x) for x in literal_addresses],
    'pointer_values': [hex(x) for x in pointers],
    'cases': cases,
}
(PROJECT / 'analysis' / 'copy_global_pair_8c056926_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} global pair copy RAM/CPU comparisons passed')
