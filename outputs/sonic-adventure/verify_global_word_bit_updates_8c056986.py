import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
LITERAL = 0x8C0569BC
RETAIL_TARGET = 0x8C88F62C
ROUTINES = [
    (0x8C056986, 10, bytes.fromhex('0d d5 52 63 4b 23 0b 00 32 25'),
     'sa_global_word_set_mask_8c056986'),
    (0x8C056990, 12, bytes.fromhex('0a d5 47 64 52 63 49 23 0b 00 32 25'),
     'sa_global_word_clear_mask_8c056990'),
]

binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
assert struct.unpack_from('<I', binary, LITERAL - BASE)[0] == RETAIL_TARGET
for address, size, expected_span, _ in ROUTINES:
    assert binary[address - BASE:address - BASE + size] == expected_span
binary_sha256 = hashlib.sha256(binary).hexdigest()

library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'global_word_bit_updates_8c056986.dll'
library = ctypes.CDLL(str(library_path))
rng = random.Random(0x8C056986)
literal_alias = LITERAL
ordinary_target = 0x8C710000
cases_total = 0
reports = []

for function, size, expected_span, symbol in ROUTINES:
    native = getattr(library, symbol)
    native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    native.restype = None
    cases = 0
    for case in range(128):
        ram = bytearray(RAM_SIZE)
        ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
        mode = case % 4
        if mode == 0:
            target = RETAIL_TARGET
        elif mode == 1:
            target = literal_alias
        elif mode == 2:
            target = ordinary_target + (case * 4)
        else:
            target = 0x8CFFFFFF - 3
        if target != LITERAL:
            target_at = (target & 0x1FFFFFFF) - 0x0C000000
            struct.pack_into('<I', ram, target_at, rng.getrandbits(32))
        literal_at = (LITERAL & 0x1FFFFFFF) - 0x0C000000
        struct.pack_into('<I', ram, literal_at, target)

        state = State()
        state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        state.r[4] = rng.choice([
            0, 1, 0x7fffffff, 0x80000000, 0xffffffff,
            0x0000ffff, 0xffff0000, 0x00ff00ff, rng.getrandbits(32),
        ])
        state.sr = rng.choice([0, 1, 0x70000000, 0xF0000001, rng.getrandbits(32)])
        state.pr = rng.choice([0x8CFED000, 0x8C010000, LITERAL, target])
        state.pc = function

        expected_ram = bytearray(ram)
        expected = State.from_buffer_copy(state)
        reference(expected_ram, expected)
        actual_ram = bytearray(ram)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        actual = State.from_buffer_copy(state)
        native(ram_view, ctypes.byref(actual))
        assert actual_ram == expected_ram, f'RAM mismatch at {function:08x}, case {case}'
        assert bytes(actual) == bytes(expected), f'CPU mismatch at {function:08x}, case {case}'
        cases += 1

    span = binary[function - BASE:function - BASE + size]
    reports.append({
        'module': '1ST_READ.BIN',
        'load_address': hex(BASE),
        'address': hex(function),
        'end_exclusive': hex(function + size),
        'retail_span_sha256': hashlib.sha256(span).hexdigest(),
        'name': symbol,
        'cases_passed': cases,
    })
    cases_total += cases

report = {
    'functions_verified': len(reports),
    'total_full_state_cases': cases_total,
    'comparison_level': 'For each routine, complete 16 MiB RAM and all GPRs, SR, PR, and return PC compared against the retail SH-4 interpreter.',
    'scope': 'The ten-byte OR-mask routine and twelve-byte NOT/AND clear-mask routine update the longword reached through literal 0x8c0569bc, storing in the RTS delay slot. Each is tested in 128 cases with edge masks, the retail pointer, literal self-alias, randomized mapped RAM, and the final aligned longword in RAM.',
    'limitations': ['Uses the project SH-4 interpreter rather than Dreamcast hardware or end-to-end gameplay.', 'The global word’s higher-level ownership and game meaning remain unidentified.'],
    'binary_sha256': binary_sha256,
    'literal_address': hex(LITERAL),
    'retail_pointer_value': hex(RETAIL_TARGET),
    'source_sha256': hashlib.sha256((ROOT / 'source' / 'sa_global_word_bit_updates_8c056986.c').read_bytes()).hexdigest(),
    'routines': reports,
}
(ROOT / 'analysis' / 'global_word_bit_updates_8c056986_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{cases_total} global word bit-update full RAM/CPU comparisons passed")
