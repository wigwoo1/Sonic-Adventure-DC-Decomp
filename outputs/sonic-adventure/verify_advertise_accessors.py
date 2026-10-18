"""Differentially verify two ADVERTISE.BIN field accessors against retail SH-4."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
LOAD, RAM_BASE, RAM_SIZE = 0x8C900000, 0x0C000000, 16 * 1024 * 1024
module = (ROOT / 'work/sonic-adventure/decoded/SONICADV/ADVERTISE.BIN').read_bytes()
functions = [
    (0x8C904020, 6, 'sa_advertise_clear_word_plus18_8c904020'),
    (0x8C90C34E, 8, 'sa_advertise_get_signed_byte_pointer_plus2c_8c90c34e'),
    (0x8C90E3C0, 14, 'sa_advertise_clear_three_fields_8c90e3c0'),
    (0x8C9031C0, 14, 'sa_advertise_global_nonzero_flag_8c9031c0'),
    (0x8C905572, 12, 'sa_advertise_set_nested_word_eight_8c905572'),
    (0x8C909532, 12, 'sa_advertise_set_nested_word_seven_8c909532'),
    (0x8C909D36, 12, 'sa_advertise_set_nested_word_six_8c909d36'),
    (0x8C90BBDA, 12, 'sa_advertise_set_nested_word_eight_8c90bbda'),
    (0x8C903F12, 10, 'sa_advertise_set_nested_input_word_8c903f12'),
    (0x8C90314C, 12, 'sa_advertise_set_nested_input_byte_8c90314c'),
    (0x8C900D60, 16, 'sa_advertise_get_flags_mask_0c_8c900d60'),
    (0x8C900D70, 18, 'sa_advertise_get_flags_mask_402_8c900d70'),
    (0x8C900F20, 12, 'sa_advertise_clear_literal_pair_8c900f20'),
    (0x8C90327E, 16, 'sa_advertise_sync_global_byte_to_object_8c90327e'),
]
library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/advertise_accessors.dll'))
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

rng = random.Random(0x8C904020)
records = []
for address, length, name in functions:
    span = module[address - LOAD:address - LOAD + length]
    assert len(span) == length
    for case in range(100):
        ram = bytearray(b'\xa5') * RAM_SIZE
        ram[(LOAD & 0x1FFFFFFF) - RAM_BASE:(LOAD & 0x1FFFFFFF) - RAM_BASE + len(module)] = module
        if address == 0x8C9031C0:
            ram[offset(0x8C78B45A)] = (case * 53) & 0xFF
        if address in (0x8C900D60, 0x8C900D70):
            put32(ram, 0x8C754E5C, rng.getrandbits(32))
        if address == 0x8C900F20:
            put32(ram, 0x8C901060, 0x0C580000 + case * 0x100)
        if address == 0x8C90327E:
            global_record = 0x0C590000 + case * 0x100
            put32(ram, 0x8C9032A4, global_record)
            ram[offset(global_record + 0x0b)] = rng.getrandbits(8)
            ram[offset(global_record + 0x0d)] = rng.getrandbits(8)
        literal_globals = {
            0x8C903F12: (0x8C903F28, 8, 0),
            0x8C90314C: (0x8C903194, 0x34, 0),
            0x8C905572: (0x8C905588, 8, 8),
            0x8C909532: (0x8C909548, 8, 7),
            0x8C909D36: (0x8C909D78, 4, 6),
            0x8C90BBDA: (0x8C90BC24, 4, 8),
        }
        if address in literal_globals:
            literal, field_offset, _value = literal_globals[address]
            global_slot = 0x0C300000 + case * 0x100
            outer_object = 0x0C400000 + case * 0x100
            inner_object = 0x0C500000 + case * 0x100
            put32(ram, literal, global_slot)
            put32(ram, global_slot, outer_object)
            put32(ram, outer_object + 0x2c, inner_object)
            put32(ram, inner_object + field_offset, rng.getrandbits(32))
        object_address = 0x0C300000 + case * 0x100
        byte_address = 0x0C500000 + case * 0x100
        put32(ram, object_address + 0x18, rng.getrandbits(32))
        put32(ram, object_address + 0x2c, byte_address)
        ram[offset(byte_address)] = (case * 53) & 0xFF

        initial = State()
        initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
        initial.r[4] = object_address
        if address == 0x8C90314C:
            initial.r[4] = rng.getrandbits(32)
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
    'module': 'ADVERTISE.BIN', 'load_address': hex(LOAD),
    'functions': [{'address': hex(a), 'end_exclusive': hex(a + n), 'name': name,
                   'retail_span_sha256': hashlib.sha256(module[a - LOAD:a - LOAD + n]).hexdigest()}
                  for a, n, name in functions],
    'cases_per_function': 100, 'total_cases_passed': len(records),
    'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state.',
    'scope': 'Six-byte structure field clear and eight-byte signed-byte pointer getter, including return-delay-slot effects.',
    'limitations': [], 'cases': records,
}
(PROJECT / 'analysis/additional-modules/ADVERTISE.BIN/accessors_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{len(records)} ADVERTISE accessor full RAM/CPU comparisons passed')
