import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C107FA4
LENGTH = 22
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
native = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'byte_copy_8c107fa4.dll')).sa_byte_copy_8c107fa4
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE - 256, hex(address)
    return result

rng = random.Random(FUNCTION)
cases = []
for count in [0, 1, 2, 3, 4, 7, 16, 64]:
    for delta in [-12, -1, 0, 1, 12, 128]:
        ram = bytearray(RAM_SIZE)
        ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
        source = 0x8C740000 + (count * 0x200)
        destination = source + delta
        seed = bytes(rng.getrandbits(8) for _ in range(192))
        ram[offset(source - 16):offset(source - 16) + len(seed)] = seed
        state = State()
        state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        state.r[4], state.r[5], state.r[6] = source, destination, count
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
        assert actual_ram == expected_ram, f'RAM mismatch count={count} delta={delta}'
        assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch count={count} delta={delta}'
        cases.append({'count': count, 'destination_delta': delta, 'ram_and_cpu_match': True})

source_file = PROJECT / 'source' / 'sa_byte_copy_8c107fa4.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH), 'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source_file.read_bytes()).hexdigest(), 'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state',
    'scope': 'Forward byte copy; counts 0/1/2/3/4/7/16/64; six destination relationships including overlap; arbitrary initial registers and SR; full RAM and CPU comparison against the retail instruction interpreter.',
    'limitations': ['Negative counts and unsigned pointer wraparound are not covered; intended caller count is positive (observed call sites use 0x600).', 'Full module behavior and gameplay remain unverified.'],
    'cases': cases,
}
(PROJECT / 'analysis' / 'byte_copy_8c107fa4_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} byte-copy RAM/CPU comparisons passed')
