"""Differentially verify the STG09 global-word boolean getter against retail SH-4."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
LOAD, ADDRESS, LENGTH = 0x0C900000, 0x0C910FD6, 18
RAM_BASE, RAM_SIZE = 0x0C000000, 16 * 1024 * 1024
POINTER_LITERAL, GLOBAL = 0x0C911018, 0x0CB1B460
module = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
span = module[ADDRESS - LOAD:ADDRESS - LOAD + LENGTH]
assert len(span) == LENGTH

library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/stg09_global_word_nonzero_0c910fd6.dll'))
native = library.sa_stg09_global_word_nonzero_0c910fd6
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
values = [0, 1, 0xFFFFFFFF, 0x80000000, 0x7FFFFFFF, 0x100, 0xFFFF0000]
values.extend(rng.getrandbits(32) for _ in range(128))
records = []
for case, value in enumerate(values):
    ram = bytearray(b'\xa5') * RAM_SIZE
    ram[LOAD - RAM_BASE:LOAD - RAM_BASE + len(module)] = module
    put32(ram, GLOBAL, value)

    initial = State()
    initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
    initial.pr = 0x0CFF0000 + case * 4
    initial.pc = ADDRESS
    initial.sr = rng.getrandbits(32)
    expected_ram, actual_ram = bytearray(ram), bytearray(ram)
    expected = State.from_buffer_copy(initial)
    actual = State.from_buffer_copy(initial)
    steps = reference(expected_ram, expected, stop_pc=initial.pr, instruction_budget=32)
    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    native(view, ctypes.byref(actual))
    assert actual_ram == expected_ram, f'RAM mismatch case={case}, value={value:#x}'
    if bytes(actual) != bytes(expected):
        raise AssertionError(
            f'CPU mismatch case={case}, value={value:#x}: '
            f'r={[(i, hex(actual.r[i]), hex(expected.r[i])) for i in range(16) if actual.r[i] != expected.r[i]]}, '
            f'sr={actual.sr:#x}/{expected.sr:#x}, pc={actual.pc:#x}/{expected.pc:#x}')
    records.append({'case': case, 'global_word': value,
                    'retail_instructions': steps, 'full_ram_cpu_match': True})

source = PROJECT / 'source/sa_stg09_global_word_nonzero_0c910fd6.c'
report = {
    'module': 'STG09.BIN', 'load_address': hex(LOAD), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': len(records),
    'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state against retail SH-4 execution.',
    'scope': '18-byte global word-nonzero predicate, including zero/nonzero branches, output register, and T bit.',
    'limitations': ['The global word is varied at its retail pointer target; the pointer literal and slot retain their extracted retail values.'],
    'cases': records,
}
(PROJECT / 'analysis/common-overlays/STG09.BIN/global_word_nonzero_0c910fd6_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{len(records)} STG09 global-word predicate full RAM/CPU comparisons passed")
