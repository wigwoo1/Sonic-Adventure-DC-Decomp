"""Differentially verify the STG09 global-gated word setter."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
LOAD, ADDRESS, LENGTH = 0x0C900000, 0x0C91344E, 22
RAM_BASE, RAM_SIZE = 0x0C000000, 16 * 1024 * 1024
FLAG_ADDRESS, DESTINATION = 0x0CB1BF38, 0x0CAF3490
module = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
span = module[ADDRESS - LOAD:ADDRESS - LOAD + LENGTH]
assert len(span) == LENGTH

library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/stg09_gated_global_word_set_0c91344e.dll'))
native = library.sa_stg09_gated_global_word_set_0c91344e
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
flags = [0, 1, 2, 0xFFFFFFFF, 0x80000000, 0x7FFFFFFF]
flags.extend(rng.getrandbits(32) for _ in range(64))
parameters = [0, 1, 0xFFFFFFFF, 0x80000000, 0x7FFFFFFF]
records = []
for case, flag in enumerate(flags):
    parameter = parameters[case % len(parameters)] if case < len(parameters) else rng.getrandbits(32)
    ram = bytearray(b'\xa5') * RAM_SIZE
    ram[LOAD - RAM_BASE:LOAD - RAM_BASE + len(module)] = module
    put32(ram, FLAG_ADDRESS, flag)
    put32(ram, DESTINATION, 0xA5A5A5A5)

    initial = State()
    initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
    initial.r[4] = parameter
    initial.pr = 0x0CFF0000 + case * 4
    initial.pc = ADDRESS
    initial.sr = rng.getrandbits(32)
    expected_ram, actual_ram = bytearray(ram), bytearray(ram)
    expected = State.from_buffer_copy(initial)
    actual = State.from_buffer_copy(initial)
    steps = reference(expected_ram, expected, stop_pc=initial.pr, instruction_budget=32)
    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    native(view, ctypes.byref(actual))
    assert actual_ram == expected_ram, f'RAM mismatch case={case}, flag={flag:#x}, parameter={parameter:#x}'
    if bytes(actual) != bytes(expected):
        raise AssertionError(
            f'CPU mismatch case={case}, flag={flag:#x}, parameter={parameter:#x}: '
            f'r={[(i, hex(actual.r[i]), hex(expected.r[i])) for i in range(16) if actual.r[i] != expected.r[i]]}, '
            f'sr={actual.sr:#x}/{expected.sr:#x}, pc={actual.pc:#x}/{expected.pc:#x}')
    records.append({'case': case, 'flag': flag, 'parameter': parameter,
                    'destination_word': int.from_bytes(actual_ram[offset(DESTINATION):offset(DESTINATION) + 4], 'little'),
                    'retail_instructions': steps, 'full_ram_cpu_match': True})

source = PROJECT / 'source/sa_stg09_gated_global_word_set_0c91344e.c'
report = {
    'module': 'STG09.BIN', 'load_address': hex(LOAD), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': len(records),
    'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state against retail SH-4 execution.',
    'scope': '22-byte global-gated word setter, including both return branches, fixed global destination write, output registers, and T bit.',
    'limitations': ['The retail literal-pool addresses are preserved; tests vary the pointed-to flag and input value.'],
    'cases': records,
}
(PROJECT / 'analysis/common-overlays/STG09.BIN/gated_global_word_set_0c91344e_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{len(records)} STG09 gated setter full RAM/CPU comparisons passed")
