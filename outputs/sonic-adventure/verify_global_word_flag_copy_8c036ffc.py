import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C036FFC
GLOBAL = 0x8C890104
LITERAL = 0x8C037008

binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + 10]
assert span == bytes.fromhex('02 d1 12 50 c0 cb 0b 00 02 11')
literal_value = struct.unpack_from('<I', binary, LITERAL - BASE)[0]
assert literal_value == GLOBAL
binary_sha256 = hashlib.sha256(binary).hexdigest()

library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'global_word_flag_copy_8c036ffc.dll'
native = ctypes.CDLL(str(library_path)).sa_global_word_flag_copy_8c036ffc
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(0x8C036FFC)
source_values = [0, 1, 0x3f, 0x40, 0x80, 0xc0, 0xffffffff,
                 0x12345600, 0x1234563f, 0x12345640, 0x12345680,
                 0x123456c0, 0x80000000, 0x7fffffff, 0xa5a5a5a5]
cases = 0
for case in range(96):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    base_at = (GLOBAL & 0x1FFFFFFF) - 0x0C000000
    source_at = base_at + 8
    struct.pack_into('<I', ram, base_at, rng.getrandbits(32))
    value = source_values[case % len(source_values)] if case < len(source_values) else rng.getrandbits(32)
    struct.pack_into('<I', ram, source_at, value)

    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.sr = rng.choice([0, 1, 0x70000000, 0xF0000001, rng.getrandbits(32)])
    state.pr = rng.choice([0x8CFED000, 0x8C010000, GLOBAL, GLOBAL + 8])
    state.pc = FUNCTION

    expected_ram = bytearray(ram)
    expected = State.from_buffer_copy(state)
    reference(expected_ram, expected)
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual))
    if actual_ram != expected_ram:
        differences = [(hex(i), actual_ram[i], expected_ram[i])
                       for i in range(RAM_SIZE) if actual_ram[i] != expected_ram[i]][:12]
        raise AssertionError(f'RAM mismatch in case {case}: {differences}; '
                             f'actual r0/r1={[hex(actual.r[i]) for i in (0, 1)]}; '
                             f'expected={[hex(expected.r[i]) for i in (0, 1)]}')
    assert bytes(actual) == bytes(expected), f'CPU state mismatch in case {case}'
    cases += 1

report = {
    'cases_passed': cases,
    'comparison_level': 'Complete 16 MiB RAM and all GPRs, SR, PR, and return PC compared against the retail SH-4 interpreter.',
    'scope': 'The ten-byte routine at 0x8c036ffc loads the global base from its PC-relative literal, reads the longword at base+8, ORs it with 0xc0, and writes the result back to base+8 in the RTS delay slot. Tests cover flag-bit boundaries, pre-set bits, varied unrelated CPU state, and complete RAM equality.',
    'limitations': ['Uses the project SH-4 interpreter rather than Dreamcast hardware or end-to-end gameplay.', 'The global field meaning and higher-level game behavior remain unidentified.'],
    'binary_sha256': binary_sha256,
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'literal_address': hex(LITERAL),
    'literal_value': hex(literal_value),
    'source_sha256': hashlib.sha256((ROOT / 'source' / 'sa_global_word_flag_copy_8c036ffc.c').read_bytes()).hexdigest(),
    'routines': [{
        'module': '1ST_READ.BIN',
        'load_address': hex(BASE),
        'address': hex(FUNCTION),
        'end_exclusive': hex(FUNCTION + 10),
        'retail_span_sha256': hashlib.sha256(span).hexdigest(),
        'cases_passed': cases,
    }],
}
(ROOT / 'analysis' / 'global_word_flag_copy_8c036ffc_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{cases} global word flag copy full RAM/CPU comparisons passed')
