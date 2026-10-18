"""Differentially verify the STG09 null-terminated word-array search."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
LOAD, ADDRESS, LENGTH = 0x0C900000, 0x0C9196D4, 20
RAM_BASE, RAM_SIZE = 0x0C000000, 16 * 1024 * 1024
module = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
span = module[ADDRESS - LOAD:ADDRESS - LOAD + LENGTH]
assert len(span) == LENGTH

library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/stg09_word_array_find_0c9196d4.dll'))
native = library.sa_stg09_word_array_find_0c9196d4
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
cases = [([0], 0), ([0], 1), ([1, 2, 3, 0], 1), ([1, 2, 3, 0], 3),
         ([1, 2, 3, 0], 4), ([0xFFFFFFFF, 0x80000000, 0], 0x80000000),
         ([0xFFFFFFFF, 0], 0xFFFFFFFF), ([5, 5, 0], 5)]
for _ in range(128):
    length = rng.randrange(0, 32)
    target = rng.getrandbits(32)
    words = [rng.getrandbits(32) or 1 for _ in range(length)]
    if words and rng.randrange(2):
        words[rng.randrange(len(words))] = target
    words.append(0)
    cases.append((words, target))

for case, (words, target) in enumerate(cases):
    ram = bytearray(b'\xa5') * RAM_SIZE
    ram[LOAD - RAM_BASE:LOAD - RAM_BASE + len(module)] = module
    array = 0x0C800000 + case * 0x100
    for index, word in enumerate(words):
        put32(ram, array + index * 4, word)

    initial = State()
    initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
    initial.r[4] = target
    initial.r[5] = array
    initial.pr = 0x0CFF0000 + case * 4
    initial.pc = ADDRESS
    initial.sr = rng.getrandbits(32)
    expected_ram, actual_ram = bytearray(ram), bytearray(ram)
    expected = State.from_buffer_copy(initial)
    actual = State.from_buffer_copy(initial)
    steps = reference(expected_ram, expected, stop_pc=initial.pr, instruction_budget=512)
    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    native(view, ctypes.byref(actual))
    assert actual_ram == expected_ram, f'RAM mismatch case={case}'
    if bytes(actual) != bytes(expected):
        raise AssertionError(
            f'CPU mismatch case={case}, target={target:#x}, words={words}: '
            f'r={[(i, hex(actual.r[i]), hex(expected.r[i])) for i in range(16) if actual.r[i] != expected.r[i]]}, '
            f'sr={actual.sr:#x}/{expected.sr:#x}, pc={actual.pc:#x}/{expected.pc:#x}')
    records.append({'case': case, 'target': target, 'words': words,
                    'retail_instructions': steps, 'full_ram_cpu_match': True})

source = PROJECT / 'source/sa_stg09_word_array_find_0c9196d4.c'
report = {
    'module': 'STG09.BIN', 'load_address': hex(LOAD), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': len(records),
    'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state against retail SH-4 execution.',
    'scope': '20-byte null-terminated 32-bit search, including immediate/later matches, terminator return, pointer advancement, output word, and T bit.',
    'limitations': ['The input array is a valid in-RAM sequence terminated by a zero word.'],
    'cases': records,
}
(PROJECT / 'analysis/common-overlays/STG09.BIN/word_array_find_0c9196d4_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{len(records)} STG09 word-array search full RAM/CPU comparisons passed")
