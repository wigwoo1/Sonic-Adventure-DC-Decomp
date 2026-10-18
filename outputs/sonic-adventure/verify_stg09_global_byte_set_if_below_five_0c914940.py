"""Differentially verify the STG09 conditional global-byte setter."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
LOAD, ADDRESS, LENGTH = 0x0C900000, 0x0C914940, 24
RAM_BASE, RAM_SIZE = 0x0C000000, 16 * 1024 * 1024
POINTER_LITERAL, GLOBAL_SLOT = 0x0C914A40, 0x0CB1C0CC
module = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
span = module[ADDRESS - LOAD:ADDRESS - LOAD + LENGTH]
assert len(span) == LENGTH

library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/stg09_global_byte_set_if_below_five_0c914940.dll'))
native = library.sa_stg09_global_byte_set_if_below_five_0c914940
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    value = (address & 0x1FFFFFFF) - RAM_BASE
    assert 0 <= value < RAM_SIZE - 4, hex(address)
    return value

def get32(memory, address):
    at = offset(address)
    return int.from_bytes(memory[at:at + 4], 'little')

def put32(memory, address, value):
    at = offset(address)
    memory[at:at + 4] = (value & 0xFFFFFFFF).to_bytes(4, 'little')

rng = random.Random(ADDRESS)
records = []
arguments = [0x80000000, 0xFFFFFFFF, 0xFFFFFFFE, 0, 1, 3, 4, 5, 6,
             0x7FFFFFFF, 0x100, 0xFFFFFF00]
cases = [(pointer_is_null, arg) for pointer_is_null in (True, False) for arg in arguments]
cases.extend((rng.choice((True, False)), rng.getrandbits(32)) for _ in range(64))

for case, (pointer_is_null, argument) in enumerate(cases):
    ram = bytearray(b'\xa5') * RAM_SIZE
    ram[LOAD - RAM_BASE:LOAD - RAM_BASE + len(module)] = module
    object_address = 0 if pointer_is_null else 0x0C800100 + case * 0x100
    put32(ram, GLOBAL_SLOT, object_address)
    if object_address:
        ram[offset(object_address):offset(object_address) + 8] = b'\x31\x42\x53\x64\x75\x86\x97\xa8'

    initial = State()
    initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
    initial.r[4] = argument
    initial.r[14] = rng.getrandbits(32)
    initial.r[15] = 0x0C810000 + case * 0x100
    initial.pr = 0x0CFF0000 + case * 4
    initial.pc = ADDRESS
    initial.sr = rng.getrandbits(32)
    expected_ram, actual_ram = bytearray(ram), bytearray(ram)
    expected = State.from_buffer_copy(initial)
    actual = State.from_buffer_copy(initial)
    steps = reference(expected_ram, expected, stop_pc=initial.pr, instruction_budget=64)
    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    native(view, ctypes.byref(actual))
    assert actual_ram == expected_ram, f'RAM mismatch case={case}, null={pointer_is_null}, arg={argument:#x}'
    if bytes(actual) != bytes(expected):
        raise AssertionError(
            f'CPU mismatch case={case}, null={pointer_is_null}, arg={argument:#x}: '
            f'r={[(i, hex(actual.r[i]), hex(expected.r[i])) for i in range(16) if actual.r[i] != expected.r[i]]}, '
            f'sr={actual.sr:#x}/{expected.sr:#x}, pc={actual.pc:#x}/{expected.pc:#x}')
    records.append({'case': case, 'pointer_is_null': pointer_is_null,
                    'argument': argument, 'post_object_byte': None if not object_address else actual_ram[offset(object_address + 1)],
                    'stack_saved_r14': get32(actual_ram, initial.r[15] - 4),
                    'retail_instructions': steps, 'full_ram_cpu_match': True})

source = PROJECT / 'source/sa_stg09_global_byte_set_if_below_five_0c914940.c'
report = {
    'module': 'STG09.BIN', 'load_address': hex(LOAD), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': len(records),
    'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state against retail SH-4 execution.',
    'scope': '24-byte global-pointer setter, including null/non-null object handling, signed argument threshold, stack save/restore, delay slot, and byte write.',
    'limitations': ['The literal pool and global pointer slot use their extracted retail addresses; test cases vary the pointer slot value and pointed-to object state.'],
    'cases': records,
}
(PROJECT / 'analysis/common-overlays/STG09.BIN/global_byte_set_if_below_five_0c914940_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{len(records)} STG09 conditional byte-setter full RAM/CPU comparisons passed")
