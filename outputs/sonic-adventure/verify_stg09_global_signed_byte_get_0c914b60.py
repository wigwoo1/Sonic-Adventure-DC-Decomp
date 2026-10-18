"""Differentially verify the STG09 global signed-byte getter against retail SH-4."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
LOAD, ADDRESS, LENGTH = 0x0C900000, 0x0C914B60, 18
RAM_BASE, RAM_SIZE = 0x0C000000, 16 * 1024 * 1024
SLOT, GLOBAL = 0x0C914C40, 0x0CB1C570
module = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
span = module[ADDRESS - LOAD:ADDRESS - LOAD + LENGTH]
assert len(span) == LENGTH

library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/stg09_global_signed_byte_get_0c914b60.dll'))
native = library.sa_stg09_global_signed_byte_get_0c914b60
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    value = (address & 0x1FFFFFFF) - RAM_BASE
    assert 0 <= value < RAM_SIZE, hex(address)
    return value

def put32(memory, address, value):
    at = offset(address)
    memory[at:at + 4] = (value & 0xFFFFFFFF).to_bytes(4, 'little')

rng = random.Random(ADDRESS)
records = []
values = [None, *range(256)]
for case, byte_value in enumerate(values):
    ram = bytearray(b'\xa5') * RAM_SIZE
    ram[LOAD - RAM_BASE:LOAD - RAM_BASE + len(module)] = module
    data_pointer = 0 if byte_value is None else 0x0C800100 + case * 8
    put32(ram, GLOBAL, data_pointer)
    if data_pointer:
        ram[offset(data_pointer)] = byte_value

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
    assert actual_ram == expected_ram, f'RAM mismatch case={case}'
    if bytes(actual) != bytes(expected):
        raise AssertionError(
            f'CPU mismatch case={case}, byte={byte_value}: '
            f'r={[(i, hex(actual.r[i]), hex(expected.r[i])) for i in range(16) if actual.r[i] != expected.r[i]]}, '
            f'sr={actual.sr:#x}/{expected.sr:#x}, pc={actual.pc:#x}/{expected.pc:#x}')
    records.append({'case': case, 'input_byte': byte_value,
                    'retail_instructions': steps, 'full_ram_cpu_match': True})

source = PROJECT / 'source/sa_stg09_global_signed_byte_get_0c914b60.c'
report = {
    'module': 'STG09.BIN', 'load_address': hex(LOAD), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': len(records),
    'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state against retail SH-4 execution.',
    'scope': '18-byte global signed-byte getter, including the null-pointer return branch and signed interpretation of every byte value.',
    'limitations': ['The global pointer slot is initialized to a valid in-RAM address, matching the retail pointer-chain contract.'],
    'cases': records,
}
(PROJECT / 'analysis/common-overlays/STG09.BIN/global_signed_byte_get_0c914b60_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{len(records)} STG09 signed-byte getter full RAM/CPU comparisons passed")
