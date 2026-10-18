import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C05697C
READ_LITERAL = 0x8C0569C0
WRITE_LITERAL = 0x8C0569BC
RETAIL_SOURCE = 0x8C788C40
RETAIL_DESTINATION = 0x8C88F62C

binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + 10]
assert span == bytes.fromhex('10 d3 0f d1 32 62 0b 00 22 21')
assert struct.unpack_from('<I', binary, READ_LITERAL - BASE)[0] == RETAIL_SOURCE
assert struct.unpack_from('<I', binary, WRITE_LITERAL - BASE)[0] == RETAIL_DESTINATION
binary_sha256 = hashlib.sha256(binary).hexdigest()

library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'global_word_copy_8c05697c.dll'
native = ctypes.CDLL(str(library_path)).sa_global_word_copy_8c05697c
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(0x8C05697C)
alias_a = 0x8C710000
alias_b = 0x8C710100
target_pairs = [
    (RETAIL_SOURCE, RETAIL_DESTINATION),
    (alias_a, alias_a),
    (READ_LITERAL, alias_b),
    (alias_a, WRITE_LITERAL),
    (READ_LITERAL, WRITE_LITERAL),
    (0x8CFFFFFF - 3, 0x8CFFFFFF - 3),
]
cases = 0
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    if case < len(target_pairs):
        source, destination = target_pairs[case]
    else:
        source = 0x8C720000 + rng.randrange(0x70000) * 4
        destination = 0x8C720000 + rng.randrange(0x70000) * 4
    for address in {source, destination}:
        if address not in {READ_LITERAL, WRITE_LITERAL}:
            at = (address & 0x1FFFFFFF) - 0x0C000000
            struct.pack_into('<I', ram, at, rng.getrandbits(32))
    struct.pack_into('<I', ram, READ_LITERAL - 0x8C000000, source)
    struct.pack_into('<I', ram, WRITE_LITERAL - 0x8C000000, destination)

    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.sr = rng.choice([0, 1, 0x70000000, 0xF0000001, rng.getrandbits(32)])
    state.pr = rng.choice([0x8CFED000, 0x8C010000, READ_LITERAL, WRITE_LITERAL])
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
    'scope': 'The ten-byte routine at 0x8c05697c reads a longword through the source pointer in the first literal and writes it through the destination pointer in the second literal. Tests vary both pointers across the retail targets, same-address and literal aliases, randomized mapped RAM, and the final aligned longword in RAM.',
    'limitations': ['Uses the project SH-4 interpreter rather than Dreamcast hardware or end-to-end gameplay.', 'The global fields and their higher-level ownership remain unidentified.'],
    'binary_sha256': binary_sha256,
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'literal_addresses': [hex(READ_LITERAL), hex(WRITE_LITERAL)],
    'literal_values': [hex(RETAIL_SOURCE), hex(RETAIL_DESTINATION)],
    'source_sha256': hashlib.sha256((ROOT / 'source' / 'sa_global_word_copy_8c05697c.c').read_bytes()).hexdigest(),
    'routines': [{
        'module': '1ST_READ.BIN',
        'load_address': hex(BASE),
        'address': hex(FUNCTION),
        'end_exclusive': hex(FUNCTION + 10),
        'retail_span_sha256': hashlib.sha256(span).hexdigest(),
        'cases_passed': cases,
    }],
}
(ROOT / 'analysis' / 'global_word_copy_8c05697c_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{cases} reverse global word copy full RAM/CPU comparisons passed')
