"""Compare input-device record mode/flag operations with retail SH-4."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

LOAD_BASE, RAM_SIZE, CASES = 0x8c010000, 16 * 1024 * 1024, 128
POINTER_LITERAL, POINTER_CELL, RECORD_BASE = 0x8c65f530, 0x8c8a83b4, 0x0c700000
SPECS = [
    ('sa_input_device_record_mode_set_8c65f42e', 0x8c65f42e, 0x8c65f43a, 'set_mode', 0x8c65f516, 0x10c0),
    ('sa_input_device_record_mode_get_8c65f43a', 0x8c65f43a, 0x8c65f44a, 'get_mode', 0x8c65f516, 0x10c0),
    ('sa_input_device_record_flag_set_8c65f44a', 0x8c65f44a, 0x8c65f458, 'set_flag', 0x8c65f518, 0x10c4),
]


def offset(address):
    return (address & 0x1fffffff) - 0x0c000000


def main():
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    assert struct.unpack_from('<I', binary, POINTER_LITERAL - LOAD_BASE)[0] == POINTER_CELL
    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/input_device_record_mode.dll'))
    source = ROOT / 'outputs/sonic-adventure/source/sa_input_device_record_mode.c'
    rng = random.Random(0x8c65f42e)
    baseline = bytearray([0xA5]) * RAM_SIZE
    binary_at = offset(LOAD_BASE)
    baseline[binary_at:binary_at + len(binary)] = binary
    records = []

    for symbol, address, end, operation, literal_address, expected_offset in SPECS:
        body = binary[address - LOAD_BASE:end - LOAD_BASE]
        assert struct.unpack_from('<h', binary, literal_address - LOAD_BASE)[0] == expected_offset
        function = getattr(library, symbol)
        function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
        function.restype = None
        cases = []

        for case in range(CASES):
            ram = bytearray(baseline)
            record_address = RECORD_BASE + case * 0x1000
            struct.pack_into('<I', ram, offset(POINTER_CELL), record_address)
            initial_field = rng.getrandbits(32)
            struct.pack_into('<I', ram, offset(record_address + expected_offset), initial_field)
            retail_ram, native_ram = bytearray(ram), bytearray(ram)
            initial = State()
            initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
            initial.r[15] = 0x0cff0000
            initial.sr = rng.getrandbits(32)
            initial.pr = 0x0c00b000 + case * 4
            initial.pc = address
            expected_r4 = initial.r[4]
            expected = State.from_buffer_copy(bytes(initial))
            actual = State.from_buffer_copy(bytes(initial))

            steps = reference(retail_ram, expected)
            view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
            function(view, ctypes.byref(actual))
            assert native_ram == retail_ram, f'{symbol}: RAM differs in case {case}'
            assert bytes(actual) == bytes(expected), f'{symbol}: CPU differs in case {case}'

            if operation == 'set_mode':
                assert struct.unpack_from('<I', native_ram, offset(record_address + expected_offset))[0] == (expected_r4 << 16) & 0xffffffff
            elif operation == 'get_mode':
                assert actual.r[0] == (initial_field >> 16) & 0xffff
                assert actual.sr == initial.sr, 'SHLR16 must preserve SR.T'
            else:
                assert struct.unpack_from('<I', native_ram, offset(record_address + expected_offset))[0] == (expected_r4 & 1)
            cases.append({'case': case, 'initial_field': hex(initial_field),
                          'initial_r4': hex(expected_r4), 'returned_r0': hex(actual.r[0]),
                          'retail_instructions': steps, 'full_ram_and_cpu_match': True})

        records.append({
            'module': '1ST_READ.BIN', 'load_address': hex(LOAD_BASE),
            'name': symbol, 'address': hex(address), 'end_exclusive': hex(end),
            'pointer_literal_address': hex(POINTER_LITERAL),
            'pointer_global_cell': hex(POINTER_CELL),
            'offset_literal_address': hex(literal_address),
            'record_field_offset': hex(expected_offset),
            'retail_span_sha256': hashlib.sha256(body).hexdigest(),
            'cases_passed': len(cases), 'cases': cases,
        })

    report = {
        'functions_verified': len(records), 'total_cases': sum(r['cases_passed'] for r in records),
        'comparison_level': 'Complete 16 MiB RAM and all GPRs, SR, PR, and return PC compared.',
        'scope': 'Verifies high-half record-mode storage, high-half readback, and one-bit flag storage at fields 0x10c0 and 0x10c4, including retail literal operands and the SHLR16-preserved T bit.',
        'limitations': ['Input record fields use controlled RAM fixtures; wider device behavior and hardware remain unverified.'],
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'routines': records,
    }
    path = ROOT / 'outputs/sonic-adventure/analysis/input_device_record_mode_verification.json'
    path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"Three input-device mode routines passed {report['total_cases']} full RAM/CPU comparisons")


if __name__ == '__main__':
    main()
