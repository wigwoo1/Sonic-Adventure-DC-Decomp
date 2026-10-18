import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C043998
DESTINATION = 0x8C74913C
LITERAL = 0x8C043A04

binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
assert len(binary) == 6735488
assert binary[FUNCTION - BASE:FUNCTION - BASE + 8] == bytes.fromhex('1a d2 42 63 0b 00 32 22')
literal_value = struct.unpack_from('<I', binary, LITERAL - BASE)[0]
assert literal_value == DESTINATION
binary_sha256 = hashlib.sha256(binary).hexdigest()

library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'global_word_store_8c043998.dll'
native = ctypes.CDLL(str(library_path)).sa_global_word_store_8c043998
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(0x8C043998)
source_addresses = [
    DESTINATION,
    DESTINATION + 1,
    DESTINATION - 2,
    0x8C75A120,
    0x8C78FFFF,
    0x8C700004,
]
cases = 0
for case in range(96):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    source = source_addresses[case % len(source_addresses)] if case < 28 else 0x8C700000 + rng.randrange(0x80000)
    # Keep all tested source words mapped and initialize overlapping words before the call.
    for address in {source, DESTINATION}:
        at = (address & 0x1FFFFFFF) - 0x0C000000
        struct.pack_into('<I', ram, at, rng.getrandbits(32))
    if case % 3 == 0:
        sr = rng.getrandbits(32)
    else:
        sr = rng.choice([0, 1, 0x70000000, 0xF0000001])
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = source
    state.sr = sr
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    expected_ram = bytearray(ram)
    expected = State.from_buffer_copy(state)
    reference(expected_ram, expected)
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual))
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual) == bytes(expected), f'CPU state mismatch in case {case}'
    cases += 1

report = {
    'cases_passed': cases,
    'comparison_level': 'Complete 16 MiB RAM and all GPRs, SR, PR, and return PC compared against the retail SH-4 interpreter.',
    'scope': 'The eight-byte routine at 0x8c043998 loads its fixed destination from the PC-relative literal at 0x8c043a04, reads a longword through R4, and stores that value to 0x8c74913c in the RTS delay slot. Tests include exact alias, partial overlap, unaligned addresses, varied mapped sources, and varied CPU state.',
    'limitations': ['Uses the project SH-4 interpreter rather than Dreamcast hardware or end-to-end gameplay.', 'The destination field name and higher-level ownership semantics remain unidentified.'],
    'binary_sha256': binary_sha256,
    'retail_span_sha256': hashlib.sha256(binary[FUNCTION - BASE:FUNCTION - BASE + 8]).hexdigest(),
    'literal_address': hex(LITERAL),
    'literal_value': hex(literal_value),
    'source_sha256': hashlib.sha256((ROOT / 'source' / 'sa_global_word_store_8c043998.c').read_bytes()).hexdigest(),
    'routines': [{
        'module': '1ST_READ.BIN',
        'load_address': hex(BASE),
        'address': hex(FUNCTION),
        'end_exclusive': hex(FUNCTION + 8),
        'retail_span_sha256': hashlib.sha256(binary[FUNCTION - BASE:FUNCTION - BASE + 8]).hexdigest(),
        'cases_passed': cases,
    }],
}
(ROOT / 'analysis' / 'global_word_store_8c043998_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{cases} global word store full RAM/CPU comparisons passed')
