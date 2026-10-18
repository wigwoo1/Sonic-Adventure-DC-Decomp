import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C1086BA
LENGTH = 76
RAM_SIZE = 16 * 1024 * 1024
BLOCK = 0x600
OUTPUT = BLOCK // 8
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'pack_nonzero_flags_reverse_8c1086ba.dll'))
native = library.sa_pack_nonzero_flags_reverse_8c1086ba
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 12 <= result <= RAM_SIZE - BLOCK - OUTPUT - 16, hex(address)
    return result

rng = random.Random(FUNCTION)
cases = []
for case in range(12):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    source = 0x8C740000 + case * 0x2000
    destination = [0x8C750000 + case * 0x200, source, source + 1, source - 1][case % 4]
    src = offset(source)
    if case == 0:
        values = bytes(BLOCK)
    elif case == 1:
        values = bytes([1]) * BLOCK
    elif case == 2:
        values = bytes([0, 1]) * (BLOCK // 2)
    elif case == 3:
        values = bytes([0x80, 0, 0xFF, 0]) * (BLOCK // 4)
    else:
        values = bytes(rng.choice([0, 0, 1, 0x7F, 0x80, 0xFF]) for _ in range(BLOCK))
    ram[src:src + BLOCK] = values
    dst = offset(destination)
    ram[dst:dst + OUTPUT] = bytes([0xA5]) * OUTPUT
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4], state.r[5] = source, destination
    state.r[15] = 0x8C7FF000
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    reference(expected_ram, expected_state)
    actual_ram = bytearray(ram)
    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(view, ctypes.byref(actual_state))
    assert actual_ram == expected_ram, f'RAM mismatch case={case} source={source:#x} destination={destination:#x}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch case={case}: rdiff={[(i,hex(actual_state.r[i]),hex(expected_state.r[i])) for i in range(16) if actual_state.r[i]!=expected_state.r[i]]} sr={actual_state.sr:#x}/{expected_state.sr:#x}'
    cases.append({'case': case, 'source': hex(source), 'destination': hex(destination), 'input_pattern': case, 'ram_and_cpu_match': True})

source_file = PROJECT / 'source' / 'sa_pack_nonzero_flags_reverse_8c1086ba.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH), 'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source_file.read_bytes()).hexdigest(), 'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state',
    'scope': 'Exact 76-byte fixed-size reverse byte order/LSB-first flag packer; tests all-zero/all-nonzero/patterned data, overlap, stack saves, and CPU outputs.',
    'limitations': ['Caller-specific meaning of the 0x600 input flag array remains unidentified.'],
    'cases': cases,
}
(PROJECT / 'analysis' / 'pack_nonzero_flags_reverse_8c1086ba_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} reverse flag-packing RAM/CPU comparisons passed')
