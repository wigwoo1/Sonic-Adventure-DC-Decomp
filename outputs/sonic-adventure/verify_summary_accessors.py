"""Differentially verify five SUMMARY.BIN field accessors against retail SH-4."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
LOAD, RAM_BASE, RAM_SIZE = 0x8C900000, 0x0C000000, 16 * 1024 * 1024
module = (ROOT / 'work/sonic-adventure/decoded/SONICADV/SUMMARY.BIN').read_bytes()
functions = [
    (0x8C901640, 8, 'sa_summary_set_byte_pointer_to_one_8c901640'),
    (0x8C901BA0, 10, 'sa_summary_get_nested_signed_halfword_8c901ba0'),
    (0x8C901BAA, 6, 'sa_summary_get_nested_word_plus8_8c901baa'),
    (0x8C901BB6, 6, 'sa_summary_set_nested_word_plus12_8c901bb6'),
    (0x8C901BBC, 6, 'sa_summary_get_signed_byte_pointer_8c901bbc'),
    (0x8C901BC2, 6, 'sa_summary_set_byte_pointer_8c901bc2'),
    (0x8C901BC8, 6, 'sa_summary_get_nested_word_plus4_8c901bc8'),
]
library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/summary_accessors.dll'))
for _, _, name in functions:
    fn = getattr(library, name)
    fn.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    fn.restype = None

def offset(address):
    value = (address & 0x1FFFFFFF) - RAM_BASE
    assert 0 <= value < RAM_SIZE - 4, hex(address)
    return value

def put32(memory, address, value):
    struct.pack_into('<I', memory, offset(address), value & 0xFFFFFFFF)

rng = random.Random(0x8C901BAA)
records = []
for address, length, name in functions:
    span = module[address - LOAD:address - LOAD + length]
    assert len(span) == length
    cases = 80
    for case in range(cases):
        ram = bytearray(b'\xa5') * RAM_SIZE
        ram[(LOAD & 0x1FFFFFFF) - RAM_BASE:(LOAD & 0x1FFFFFFF) - RAM_BASE + len(module)] = module
        object_address = 0x0C300000 + case * 0x100
        nested_address = 0x0C500000 + case * 0x100
        byte_address = 0x0C700000 + case * 0x100
        put32(ram, object_address + 0x2c, nested_address)
        put32(ram, object_address + 0x20,
              nested_address if address == 0x8C901BA0 else byte_address)
        put32(ram, nested_address + 4, rng.getrandbits(32))
        put32(ram, nested_address + 8, rng.getrandbits(32))
        put32(ram, nested_address + 0x10, byte_address)
        struct.pack_into('<H', ram, offset(byte_address + 2), rng.getrandbits(16))
        ram[offset(byte_address)] = (case * 37) & 0xFF

        initial = State()
        initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
        initial.r[4] = object_address
        initial.r[5] = rng.getrandbits(32)
        initial.pr = 0x0CFF0000 + case * 4
        initial.pc = address
        initial.sr = rng.getrandbits(32)
        expected_ram, actual_ram = bytearray(ram), bytearray(ram)
        expected, actual = State.from_buffer_copy(initial), State.from_buffer_copy(initial)
        steps = reference(expected_ram, expected, stop_pc=initial.pr, instruction_budget=16)
        view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        getattr(library, name)(view, ctypes.byref(actual))
        assert actual_ram == expected_ram, f'{name}: RAM mismatch case={case}'
        assert bytes(actual) == bytes(expected), (
            f'{name}: CPU mismatch case={case}, '
            f'r={[(i, hex(actual.r[i]), hex(expected.r[i])) for i in range(16) if actual.r[i] != expected.r[i]]}, '
            f'sr={actual.sr:#x}/{expected.sr:#x}, pr={actual.pr:#x}/{expected.pr:#x}, pc={actual.pc:#x}/{expected.pc:#x}')
        records.append({'address': hex(address), 'name': name, 'case': case,
                        'retail_instructions': steps, 'full_ram_cpu_match': True})

report = {
    'module': 'SUMMARY.BIN', 'load_address': hex(LOAD),
    'functions': [{'address': hex(a), 'end_exclusive': hex(a + n), 'name': name,
                   'retail_span_sha256': hashlib.sha256(module[a - LOAD:a - LOAD + n]).hexdigest()}
                  for a, n, name in functions],
    'cases_per_function': cases, 'total_cases_passed': len(records),
    'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state.',
    'scope': 'Five six-byte field accessors, including load/store delay-slot effects and signed-byte return extension.',
    'limitations': [], 'cases': records,
}
(PROJECT / 'analysis/additional-modules/SUMMARY.BIN/accessors_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{len(records)} SUMMARY accessor full RAM/CPU comparisons passed')
