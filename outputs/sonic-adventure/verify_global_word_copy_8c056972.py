import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C056972
SOURCE_LITERAL = 0x8C0569BC
DESTINATION_LITERAL = 0x8C0569C0
RETAIL_SOURCE = 0x8C88F62C
RETAIL_DESTINATION = 0x8C788C40

binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + 10]
assert span == bytes.fromhex('12 d1 12 d0 12 63 0b 00 32 20')
assert struct.unpack_from('<I', binary, SOURCE_LITERAL - BASE)[0] == RETAIL_SOURCE
assert struct.unpack_from('<I', binary, DESTINATION_LITERAL - BASE)[0] == RETAIL_DESTINATION
binary_sha256 = hashlib.sha256(binary).hexdigest()

library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'global_word_copy_8c056972.dll'
native = ctypes.CDLL(str(library_path)).sa_global_word_copy_8c056972
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(0x8C056972)
alias_a = 0x8C710000
alias_b = 0x8C710100
target_pairs = [
    (RETAIL_SOURCE, RETAIL_DESTINATION),
    (alias_a, alias_a),
    (SOURCE_LITERAL, alias_b),
    (alias_a, DESTINATION_LITERAL),
    (SOURCE_LITERAL, DESTINATION_LITERAL),
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
    # Seed ordinary data locations before writing the two pointer literals.
    literal_addresses = {SOURCE_LITERAL, DESTINATION_LITERAL}
    for address in {source, destination}:
        if address not in literal_addresses:
            at = (address & 0x1FFFFFFF) - 0x0C000000
            struct.pack_into('<I', ram, at, rng.getrandbits(32))
    struct.pack_into('<I', ram, SOURCE_LITERAL - 0x8C000000, source)
    struct.pack_into('<I', ram, DESTINATION_LITERAL - 0x8C000000, destination)

    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.sr = rng.choice([0, 1, 0x70000000, 0xF0000001, rng.getrandbits(32)])
    state.pr = rng.choice([0x8CFED000, 0x8C010000, SOURCE_LITERAL, DESTINATION_LITERAL])
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
    'scope': 'The ten-byte routine at 0x8c056972 reads source and destination pointers from separate PC-relative literals, copies the source longword, and writes in the RTS delay slot. Tests vary both pointers across retail targets, same-address aliases, literal aliases, randomized mapped RAM and the final aligned longword in physical RAM.',
    'limitations': ['Uses the project SH-4 interpreter rather than Dreamcast hardware or end-to-end gameplay.', 'The global fields and their higher-level ownership remain unidentified.'],
    'binary_sha256': binary_sha256,
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'literal_addresses': [hex(SOURCE_LITERAL), hex(DESTINATION_LITERAL)],
    'literal_values': [hex(RETAIL_SOURCE), hex(RETAIL_DESTINATION)],
    'source_sha256': hashlib.sha256((ROOT / 'source' / 'sa_global_word_copy_8c056972.c').read_bytes()).hexdigest(),
    'routines': [{
        'module': '1ST_READ.BIN',
        'load_address': hex(BASE),
        'address': hex(FUNCTION),
        'end_exclusive': hex(FUNCTION + 10),
        'retail_span_sha256': hashlib.sha256(span).hexdigest(),
        'cases_passed': cases,
    }],
}
(ROOT / 'analysis' / 'global_word_copy_8c056972_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{cases} global word copy full RAM/CPU comparisons passed')
