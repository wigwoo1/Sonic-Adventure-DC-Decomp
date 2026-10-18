"""Differentially verify the STG09 signed-byte word table lookup."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
LOAD, ADDRESS, LENGTH = 0x0C900000, 0x0C918790, 24
RAM_BASE, RAM_SIZE = 0x0C000000, 16 * 1024 * 1024
TABLE = 0x0CB2397C
module = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
span = module[ADDRESS - LOAD:ADDRESS - LOAD + LENGTH]
assert len(span) == LENGTH

library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/stg09_signed_byte_word_lookup_0c918790.dll'))
native = library.sa_stg09_signed_byte_word_lookup_0c918790
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    value = (address & 0x1FFFFFFF) - RAM_BASE
    assert 0 <= value < RAM_SIZE - 4, hex(address)
    return value

def put32(memory, address, value):
    at = offset(address)
    memory[at:at + 4] = (value & 0xFFFFFFFF).to_bytes(4, 'little')

rng = random.Random(ADDRESS)
records = []
for case, raw_index in enumerate(range(256)):
    signed_index = raw_index if raw_index < 128 else raw_index - 256
    ram = bytearray(b'\xa5') * RAM_SIZE
    ram[LOAD - RAM_BASE:LOAD - RAM_BASE + len(module)] = module
    entries = {}
    for index in range(-128, 5):
        value = rng.getrandbits(32)
        entries[index] = value
        put32(ram, TABLE + index * 4, value)

    initial = State()
    initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
    initial.r[4] = (initial.r[4] & 0xFFFFFF00) | raw_index
    initial.pr = 0x0CFF0000 + case * 4
    initial.pc = ADDRESS
    initial.sr = rng.getrandbits(32)
    expected_ram, actual_ram = bytearray(ram), bytearray(ram)
    expected = State.from_buffer_copy(initial)
    actual = State.from_buffer_copy(initial)
    steps = reference(expected_ram, expected, stop_pc=initial.pr, instruction_budget=32)
    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    native(view, ctypes.byref(actual))
    assert actual_ram == expected_ram, f'RAM mismatch case={case}, index={signed_index}'
    if bytes(actual) != bytes(expected):
        raise AssertionError(
            f'CPU mismatch case={case}, index={signed_index}: '
            f'r={[(i, hex(actual.r[i]), hex(expected.r[i])) for i in range(16) if actual.r[i] != expected.r[i]]}, '
            f'sr={actual.sr:#x}/{expected.sr:#x}, pc={actual.pc:#x}/{expected.pc:#x}')
    records.append({'case': case, 'raw_index': raw_index, 'signed_index': signed_index,
                    'expected_word': entries.get(signed_index, 0),
                    'retail_instructions': steps, 'full_ram_cpu_match': True})

source = PROJECT / 'source/sa_stg09_signed_byte_word_lookup_0c918790.c'
report = {
    'module': 'STG09.BIN', 'load_address': hex(LOAD), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': len(records),
    'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state against retail SH-4 execution.',
    'scope': '24-byte signed-byte table lookup, exhaustively tested for all 256 raw byte inputs and table entries accessed by each valid signed index.',
    'limitations': ['Negative signed-byte indices intentionally read words preceding the table base, matching retail address arithmetic.'],
    'cases': records,
}
(PROJECT / 'analysis/common-overlays/STG09.BIN/signed_byte_word_lookup_0c918790_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{len(records)} STG09 signed-byte lookup full RAM/CPU comparisons passed")
