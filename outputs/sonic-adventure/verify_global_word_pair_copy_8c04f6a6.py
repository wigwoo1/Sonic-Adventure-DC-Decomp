import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C04F6A6
SOURCE1 = 0x8C7493BA
SOURCE2 = 0x8C7493BC
DESTINATION1 = 0x8C7493B6
DESTINATION2 = 0x8C7493B8
LITERALS = {
    0x8C04F6C4: SOURCE1,
    0x8C04F704: DESTINATION1,
    0x8C04F708: SOURCE2,
    0x8C04F70C: DESTINATION2,
}

binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + 18]
assert span == bytes.fromhex('07 d1 16 d3 11 62 21 23 16 d2 16 d3 21 60 0b 00 01 23')
for address, value in LITERALS.items():
    assert struct.unpack_from('<I', binary, address - BASE)[0] == value
binary_sha256 = hashlib.sha256(binary).hexdigest()

library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'global_word_pair_copy_8c04f6a6.dll'
native = ctypes.CDLL(str(library_path)).sa_global_word_pair_copy_8c04f6a6
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(0x8C04F6A6)
cases = 0
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    start = (DESTINATION1 & 0x1FFFFFFF) - 0x0C000000
    ram[start - 4:start + 12] = bytes(rng.getrandbits(8) for _ in range(16))
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.sr = rng.choice([0, 1, 0x70000000, 0xF0000001, rng.getrandbits(32)])
    state.pr = rng.choice([0x8CFED000, 0x8C010000, SOURCE1, SOURCE2])
    state.pc = FUNCTION

    expected_ram = bytearray(ram)
    expected = State.from_buffer_copy(state)
    reference(expected_ram, expected)
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual))
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual) == bytes(expected), f'CPU mismatch in case {case}'
    cases += 1

report = {
    'cases_passed': cases,
    'comparison_level': 'Complete 16 MiB RAM and all GPRs, SR, PR, and return PC compared against the retail SH-4 interpreter.',
    'scope': 'The 18-byte routine at 0x8c04f6a6 copies signed-loaded words from 0x8c7493ba and 0x8c7493bc to 0x8c7493b6 and 0x8c7493b8. The second read and write occur before return, with the final write in the RTS delay slot. Tests compare every RAM byte and CPU register over varied neighboring data and CPU state.',
    'limitations': ['Uses the project SH-4 interpreter rather than Dreamcast hardware or end-to-end gameplay.', 'The global fields’ higher-level meaning remains unidentified.'],
    'binary_sha256': binary_sha256,
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'literal_addresses': [hex(a) for a in LITERALS],
    'literal_values': [hex(v) for v in LITERALS.values()],
    'source_sha256': hashlib.sha256((ROOT / 'source' / 'sa_global_word_pair_copy_8c04f6a6.c').read_bytes()).hexdigest(),
    'routines': [{
        'module': '1ST_READ.BIN',
        'load_address': hex(BASE),
        'address': hex(FUNCTION),
        'end_exclusive': hex(FUNCTION + 18),
        'retail_span_sha256': hashlib.sha256(span).hexdigest(),
        'cases_passed': cases,
    }],
}
(ROOT / 'analysis' / 'global_word_pair_copy_8c04f6a6_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{cases} global word pair copy full RAM/CPU comparisons passed')
