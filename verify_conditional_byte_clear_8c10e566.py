import ctypes
import hashlib
import json
import random
from pathlib import Path

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C10E566
LENGTH = 16
RAM_SIZE = 16 * 1024 * 1024
MASK = 0xFFFFFFFF

binary = (ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library = ctypes.CDLL(str(ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'conditional_byte_clear_8c10e566.dll'))
native = library.sa_conditional_byte_clear_8c10e566
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE, hex(address)
    return result

def put32(ram, address, value):
    at = offset(address)
    ram[at:at + 4] = (value & MASK).to_bytes(4, 'little')

rng = random.Random(FUNCTION)
cases = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    arg = 0x8C740000 + case * 0x100
    condition = [0, 1, 2, MASK, 0x80000000][case % 5] if case < 20 else rng.getrandbits(32)
    destination = [0x8C750000 + case, arg, arg + 4, FUNCTION, 0x8C740000 + case * 0x100 + 12][case % 5]
    if case == 1:
        condition = 1
        destination = 0x8C750001
    put32(ram, arg, condition)
    put32(ram, arg + 4, destination)
    target = offset(destination)
    if not BASE <= destination < BASE + len(binary):
        ram[target] = rng.randrange(256)
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = arg
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    expected_ram = bytearray(ram)
    expected_state = State.from_buffer_copy(state)
    reference(expected_ram, expected_state)
    actual_ram = bytearray(ram)
    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(view, ctypes.byref(actual_state))
    assert actual_ram == expected_ram, f'RAM mismatch case={case} condition={condition:#x} destination={destination:#x}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch case={case}'
    cases.append({'case': case, 'condition': hex(condition), 'destination': hex(destination), 'ram_and_cpu_match': True})

source = PROJECT / 'source' / 'sa_conditional_byte_clear_8c10e566.c'
report = {
    'module': '1ST_READ.BIN',
    'load_address': hex(BASE),
    'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state',
    'scope': 'Exact 16-byte conditional byte-clear helper; tests equality boundary values, mapped byte destinations, argument aliasing, and code-backed destination bytes.',
    'limitations': ['The caller-specific meaning of the condition and destination byte remains unidentified.'],
    'cases': cases,
}
(PROJECT / 'analysis' / 'conditional_byte_clear_8c10e566_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} conditional byte-clear RAM/CPU comparisons passed')
