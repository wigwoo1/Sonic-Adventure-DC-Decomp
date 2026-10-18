"""Differentially verify the STG09 nested signed-range predicate."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
LOAD, ADDRESS, LENGTH = 0x0C900000, 0x0C91A75C, 26
RAM_BASE, RAM_SIZE = 0x0C000000, 16 * 1024 * 1024
module = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
span = module[ADDRESS - LOAD:ADDRESS - LOAD + LENGTH]
assert len(span) == LENGTH

library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/stg09_nested_signed_range_predicate_0c91a75c.dll'))
native = library.sa_stg09_nested_signed_range_predicate_0c91a75c
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
cases = []
fields = [0, 1, 2, 3, 4, 5, 0x7FFFFFFF, 0x80000000, 0x80000001,
          0xFFFFFFFE, 0xFFFFFFFF, 0x12345678]
for field in fields:
    threshold = (field - 2) & 0xFFFFFFFF
    values = [0, 1, 2, 0xFFFFFFFF, 0x80000000, 0x7FFFFFFF,
              threshold, (threshold - 1) & 0xFFFFFFFF, (threshold + 1) & 0xFFFFFFFF]
    cases.extend((field, value) for value in values)
cases.extend((rng.getrandbits(32), rng.getrandbits(32)) for _ in range(96))

records = []
for case, (field, parameter) in enumerate(cases):
    ram = bytearray(b'\xa5') * RAM_SIZE
    ram[LOAD - RAM_BASE:LOAD - RAM_BASE + len(module)] = module
    root = 0x0C800000 + case * 0x100
    nested = root + 0x40
    put32(ram, root + 0x2c, nested)
    put32(ram, nested + 0x1c, field)

    initial = State()
    initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
    initial.r[4] = parameter
    initial.r[5] = root
    initial.pr = 0x0CFF0000 + case * 4
    initial.pc = ADDRESS
    initial.sr = rng.getrandbits(32)
    expected_ram, actual_ram = bytearray(ram), bytearray(ram)
    expected = State.from_buffer_copy(initial)
    actual = State.from_buffer_copy(initial)
    steps = reference(expected_ram, expected, stop_pc=initial.pr, instruction_budget=64)
    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    native(view, ctypes.byref(actual))
    assert actual_ram == expected_ram, f'RAM mismatch case={case}, field={field:#x}, parameter={parameter:#x}'
    if bytes(actual) != bytes(expected):
        raise AssertionError(
            f'CPU mismatch case={case}, field={field:#x}, parameter={parameter:#x}: '
            f'r={[(i, hex(actual.r[i]), hex(expected.r[i])) for i in range(16) if actual.r[i] != expected.r[i]]}, '
            f'sr={actual.sr:#x}/{expected.sr:#x}, pc={actual.pc:#x}/{expected.pc:#x}')
    records.append({'case': case, 'field': field, 'parameter': parameter,
                    'result': actual.r[0], 'retail_instructions': steps,
                    'full_ram_cpu_match': True})

source = PROJECT / 'source/sa_stg09_nested_signed_range_predicate_0c91a75c.c'
report = {
    'module': 'STG09.BIN', 'load_address': hex(LOAD), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': len(records),
    'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state against retail SH-4 execution.',
    'scope': '26-byte nested signed comparison predicate with strict positive-input condition and 32-bit threshold subtraction.',
    'limitations': ['Nested object pointers are valid in-RAM addresses; input fields cover signed and subtraction boundaries.'],
    'cases': records,
}
(PROJECT / 'analysis/common-overlays/STG09.BIN/nested_signed_range_predicate_0c91a75c_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{len(records)} STG09 signed-range predicate full RAM/CPU comparisons passed")
