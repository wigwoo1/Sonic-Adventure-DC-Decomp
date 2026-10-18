import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C04F786
DESTINATION4 = 0x8C7493C2
DESTINATION5 = 0x8C7493C4
LITERAL4 = 0x8C04F804
LITERAL5 = 0x8C04F808

binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + 14]
assert span == bytes.fromhex('1f d3 4c 64 1f d2 5c 65 41 23 0b 00 51 22')
assert struct.unpack_from('<I', binary, LITERAL4 - BASE)[0] == DESTINATION4
assert struct.unpack_from('<I', binary, LITERAL5 - BASE)[0] == DESTINATION5
binary_sha256 = hashlib.sha256(binary).hexdigest()

library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'global_u8_pair_store_8c04f786.dll'
native = ctypes.CDLL(str(library_path)).sa_global_u8_pair_store_8c04f786
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(0x8C04F786)
byte_values = [0, 1, 0x7f, 0x80, 0xff, 0x100, 0x17f, 0x180,
               0x1ff, 0x1234567f, 0x12345680, 0xffffffff]
cases = 0
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    destination_at = (DESTINATION4 & 0x1FFFFFFF) - 0x0C000000
    ram[destination_at - 2:destination_at + 6] = bytes(rng.getrandbits(8) for _ in range(8))
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = (rng.getrandbits(24) << 8) | byte_values[case % len(byte_values)]
    state.r[5] = (rng.getrandbits(24) << 8) | byte_values[(case * 7 + 5) % len(byte_values)]
    state.sr = rng.choice([0, 1, 0x70000000, 0xF0000001, rng.getrandbits(32)])
    state.pr = rng.choice([0x8CFED000, 0x8C010000, DESTINATION4, DESTINATION5])
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
    'scope': 'The 14-byte routine at 0x8c04f786 zero-extends the low bytes in R4 and R5 and stores them as adjacent 16-bit words at 0x8c7493c2 and 0x8c7493c4; the second word store is in the RTS delay slot. Tests cover byte boundaries, discarded upper GPR bits, surrounding RAM guards, and varied CPU state.',
    'limitations': ['Uses the project SH-4 interpreter rather than Dreamcast hardware or end-to-end gameplay.', 'The global fields’ higher-level meaning remains unidentified.'],
    'binary_sha256': binary_sha256,
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'literal_addresses': [hex(LITERAL4), hex(LITERAL5)],
    'literal_values': [hex(DESTINATION4), hex(DESTINATION5)],
    'source_sha256': hashlib.sha256((ROOT / 'source' / 'sa_global_u8_pair_store_8c04f786.c').read_bytes()).hexdigest(),
    'routines': [{
        'module': '1ST_READ.BIN',
        'load_address': hex(BASE),
        'address': hex(FUNCTION),
        'end_exclusive': hex(FUNCTION + 14),
        'retail_span_sha256': hashlib.sha256(span).hexdigest(),
        'cases_passed': cases,
    }],
}
(ROOT / 'analysis' / 'global_u8_pair_store_8c04f786_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{cases} global unsigned-byte pair store full RAM/CPU comparisons passed')
