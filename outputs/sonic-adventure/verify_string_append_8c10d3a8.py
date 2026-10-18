import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C10D3A8
LENGTH = 30
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'string_append_8c10d3a8.dll'))
native = library.sa_string_append_8c10d3a8
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE - 128, hex(address)
    return result

rng = random.Random(FUNCTION)
cases = []
for case in range(72):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    base_addr = 0x8C740000 + case * 0x200
    if case % 4 == 0:
        destination, source = base_addr, base_addr + 0x80
        ram[offset(destination):offset(destination) + 8] = b'overlap\0'
        ram[offset(source):offset(source) + 4] = b'XYZ\0'
    elif case % 4 == 1:
        destination, source = base_addr, base_addr + 3
        arena = b'ab\0XY\0'
        ram[offset(base_addr):offset(base_addr) + len(arena)] = arena
    elif case % 4 == 2:
        destination, source = base_addr, base_addr + 0x40
        dest_text, src_text = b'abc', b'XY'
        ram[offset(destination):offset(destination) + len(dest_text) + 1] = dest_text + b'\0'
        ram[offset(source):offset(source) + len(src_text) + 1] = src_text + b'\0'
    else:
        destination, source = base_addr, base_addr + 0x80
        dest_len, src_len = rng.randrange(0, 16), rng.randrange(0, 16)
        dest_text = bytes(rng.choice(b'ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789') for _ in range(dest_len))
        src_text = bytes(rng.choice(b'abcdefghijklmnopqrstuvwxyz0123456789') for _ in range(src_len))
        ram[offset(destination):offset(destination) + dest_len + 1] = dest_text + b'\0'
        ram[offset(source):offset(source) + src_len + 1] = src_text + b'\0'
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4], state.r[5] = destination, source
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
    assert actual_ram == expected_ram, f'RAM mismatch case={case} dst={destination:#x} src={source:#x}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch case={case}: rdiff={[(i,hex(actual_state.r[i]),hex(expected_state.r[i])) for i in range(16) if actual_state.r[i]!=expected_state.r[i]]} sr={actual_state.sr:#x}/{expected_state.sr:#x}'
    cases.append({'case': case, 'destination': hex(destination), 'source': hex(source), 'ram_and_cpu_match': True})

source_file = PROJECT / 'source' / 'sa_string_append_8c10d3a8.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH), 'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source_file.read_bytes()).hexdigest(), 'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state',
    'scope': 'Exact 30-byte forward string append; tests empty/nonempty strings, self and partial overlap, distinct buffers, pointer outputs and complete RAM/CPU state.',
    'limitations': ['Caller-specific string storage semantics remain unidentified.'],
    'cases': cases,
}
(PROJECT / 'analysis' / 'string_append_8c10d3a8_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} string append RAM/CPU comparisons passed')
