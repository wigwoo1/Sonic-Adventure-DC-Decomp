"""Compare the input-device record getters against the original SH-4 bodies."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

LOAD_BASE, RAM_SIZE, CASES_PER_FUNCTION = 0x8c010000, 16 * 1024 * 1024, 128
GLOBAL_POINTER_CELL = 0x8c8a83b4
ROUTINES = [
    ('sa_input_device_record_get_8c65f458', 0x8c65f458, 0x8c65f462, 0x8c65f530, 0x8c65f518, 0x10c4),
    ('sa_input_device_record_get_8c65f46c', 0x8c65f46c, 0x8c65f476, 0x8c65f530, 0x8c65f51a, 0x10cc),
    ('sa_input_device_record_get_8c65f480', 0x8c65f480, 0x8c65f48a, 0x8c65f530, 0x8c65f51c, 0x10d0),
    ('sa_input_device_record_get_8c65f4ac', 0x8c65f4ac, 0x8c65f4b6, 0x8c65f530, 0x8c65f51e, 0x10c8),
    ('sa_input_device_record_get_8c65f4c0', 0x8c65f4c0, 0x8c65f4ca, 0x8c65f530, 0x8c65f520, 0x10d8),
    ('sa_input_device_record_get_8c65f4da', 0x8c65f4da, 0x8c65f4e4, 0x8c65f530, 0x8c65f524, 0x10e4),
    ('sa_input_device_record_get_8c65f4f8', 0x8c65f4f8, 0x8c65f502, 0x8c65f530, 0x8c65f528, 0x10dc),
    ('sa_input_device_record_get_8c65f50c', 0x8c65f50c, 0x8c65f516, 0x8c65f530, 0x8c65f52c, 0x0fb0),
    ('sa_input_device_record_get_8c65f546', 0x8c65f546, 0x8c65f550, 0x8c65f554, 0x8c65f550, 0x0fbc),
]


def offset(address):
    return (address & 0x1fffffff) - 0x0c000000


def main():
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/input_device_record_getters.dll'))
    source_path = ROOT / 'outputs/sonic-adventure/source/sa_input_device_record_getters.c'
    rng = random.Random(0x8c65f458)
    records, all_cases = [], []
    binary_base = bytearray([0xA5]) * RAM_SIZE
    binary_offset = offset(LOAD_BASE)
    binary_base[binary_offset:binary_offset + len(binary)] = binary

    for symbol, address, end, pointer_literal_address, offset_address, expected_offset in ROUTINES:
        body = binary[address - LOAD_BASE:end - LOAD_BASE]
        assert len(body) == 10
        pointer_value = struct.unpack_from('<I', binary, pointer_literal_address - LOAD_BASE)[0]
        assert pointer_value == GLOBAL_POINTER_CELL
        assert struct.unpack_from('<h', binary, offset_address - LOAD_BASE)[0] == expected_offset
        function = getattr(library, symbol)
        function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
        function.restype = None
        cases = []

        for case in range(CASES_PER_FUNCTION):
            base_address = 0x0c700000 + case * 0x10000
            initial_value = rng.getrandbits(32)
            retail_ram = bytearray(binary_base)
            struct.pack_into('<I', retail_ram, offset(GLOBAL_POINTER_CELL), base_address)
            struct.pack_into('<I', retail_ram, offset(base_address + expected_offset), initial_value)
            native_ram = bytearray(retail_ram)

            initial = State()
            initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
            initial.r[15] = 0x0cff0000
            initial.sr = rng.getrandbits(32)
            initial.pr = 0x0c006000 + case * 4
            initial.pc = address
            expected = State.from_buffer_copy(bytes(initial))
            actual = State.from_buffer_copy(bytes(initial))

            steps = reference(retail_ram, expected)
            view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
            function(view, ctypes.byref(actual))
            assert retail_ram == native_ram, f'{symbol}: RAM differs in case {case}'
            assert bytes(actual) == bytes(expected), f'{symbol}: CPU differs in case {case}'
            assert actual.r[0] == initial_value
            cases.append({'case': case, 'returned_value': hex(initial_value),
                          'retail_instructions': steps, 'full_ram_and_cpu_match': True})

        records.append({
            'module': '1ST_READ.BIN', 'load_address': hex(LOAD_BASE),
            'name': symbol, 'address': hex(address), 'end_exclusive': hex(end),
            'pointer_literal_address': hex(pointer_literal_address),
            'pointer_global_cell': hex(pointer_value),
            'offset_literal_address': hex(offset_address),
            'record_field_offset': hex(expected_offset),
            'retail_span_sha256': hashlib.sha256(body).hexdigest(),
            'cases_passed': len(cases), 'cases': cases,
        })
        all_cases.extend(cases)

    report = {
        'functions_verified': len(records), 'total_abi_cases': len(all_cases),
        'comparison_level': 'Complete 16 MiB RAM and all GPRs, SR, PR, and return PC compared.',
        'scope': 'Nine input-device record getters load the shared record pointer through 0x8c8a83b4 and return 32-bit fields selected by the retail literal pool. The verified pointer and field offsets are recorded per routine. Retail instruction bodies and literal words are checked against the extracted executable.',
        'limitations': ['The structure-field meanings remain tentative.',
                        'Uses the project SH-4 interpreter, not Dreamcast hardware or end-to-end gameplay.'],
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256(source_path.read_bytes()).hexdigest(),
        'routines': records,
    }
    path = ROOT / 'outputs/sonic-adventure/analysis/input_device_record_getters_verification.json'
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'{len(records)} input-device record getters passed {len(all_cases)} full RAM/CPU comparisons')


if __name__ == '__main__':
    main()
