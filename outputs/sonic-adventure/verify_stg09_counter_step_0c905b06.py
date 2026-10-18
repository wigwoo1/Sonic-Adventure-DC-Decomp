"""Differentially verify the STG09 32-bit counter step against retail SH-4."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
LOAD, ADDRESS, LENGTH = 0x0C900000, 0x0C905B06, 46
RAM_BASE, RAM_SIZE = 0x0C000000, 16 * 1024 * 1024
module = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
span = module[ADDRESS - LOAD:ADDRESS - LOAD + LENGTH]
assert len(span) == LENGTH

library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/stg09_counter_step_0c905b06.dll'))
native = library.sa_stg09_counter_step_0c905b06
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    value = (address & 0x1FFFFFFF) - RAM_BASE
    assert 0 <= value < RAM_SIZE - 4, hex(address)
    return value

def put32(memory, address, value):
    struct.pack_into('<I', memory, offset(address), value & 0xFFFFFFFF)

def put16(memory, address, value):
    struct.pack_into('<H', memory, offset(address), value & 0xFFFF)

seed = 0x0C905B06
rng = random.Random(seed)
records = []
values = [0, 0x3FFF, 0x4000, 0xBFFF, 0xC000, 0xC001,
          0x7FFFFFFF, 0x80000000, 0xFFFFFFFF]
cases = [(flag, value) for flag in (0, 0x100, 0x101, 0xFFFF) for value in values]
cases.extend((rng.choice((0, 0x100, 0x101, 0xFFFF)), rng.getrandbits(32)) for _ in range(128))

for case, (flag, counter) in enumerate(cases):
    ram = bytearray(b'\xa5') * RAM_SIZE
    ram[LOAD - RAM_BASE:LOAD - RAM_BASE + len(module)] = module
    root = 0x0C800000 + case * 0x100
    parent, flags_object, counter_object = root + 0x40, root + 0x60, root + 0x80
    put32(ram, root + 8, parent)
    put32(ram, parent + 0x20, flags_object)
    put16(ram, flags_object + 4, flag)
    put32(ram, root + 0x20, counter_object)
    put32(ram, counter_object + 0x10, counter)

    initial = State()
    initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
    initial.r[4] = root
    initial.pr = 0x0CFF0000 + case * 4
    initial.pc = ADDRESS
    initial.sr = rng.getrandbits(32)
    expected_ram, actual_ram = bytearray(ram), bytearray(ram)
    expected = State.from_buffer_copy(initial)
    actual = State.from_buffer_copy(initial)
    steps = reference(expected_ram, expected, stop_pc=initial.pr, instruction_budget=128)
    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    native(view, ctypes.byref(actual))
    assert actual_ram == expected_ram, f'RAM mismatch case={case}, flag={flag:#x}, counter={counter:#x}'
    if bytes(actual) != bytes(expected):
        raise AssertionError(
            f'CPU mismatch case={case}, flag={flag:#x}, counter={counter:#x}: '
            f'r={[(i, hex(actual.r[i]), hex(expected.r[i])) for i in range(16) if actual.r[i] != expected.r[i]]}, '
            f'sr={actual.sr:#x}/{expected.sr:#x}, pc={actual.pc:#x}/{expected.pc:#x}')
    records.append({'case': case, 'flag_word': flag, 'initial_counter': counter,
                    'retail_instructions': steps, 'full_ram_cpu_match': True})

source = PROJECT / 'source/sa_stg09_counter_step_0c905b06.c'
report = {
    'module': 'STG09.BIN', 'load_address': hex(LOAD), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': len(records),
    'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state against retail SH-4 execution.',
    'scope': '46-byte STG09 counter step: nested flag lookup, signed threshold comparison, reset, increment, delay slots, and return state.',
    'limitations': ['This verifies the routine in isolation with valid in-RAM object pointers; stage-level caller behavior is outside this comparison.'],
    'cases': records,
}
(PROJECT / 'analysis/common-overlays/STG09.BIN/counter_step_0c905b06_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{len(records)} STG09 counter-step full RAM/CPU comparisons passed")
