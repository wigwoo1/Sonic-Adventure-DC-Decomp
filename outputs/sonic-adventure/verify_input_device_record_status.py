"""Verify input-device record comparison and status-byte routines."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

LOAD_BASE, RAM_SIZE, CASES = 0x8c010000, 16 * 1024 * 1024, 128
POINTER_CELL, TABLE_BASE = 0x8c8a83b4, 0x0c700000
SPECS = [
    ('sa_input_device_record_matches_8c65f3f4', 0x8c65f3f4, 0x8c65f40e, True),
    ('sa_input_device_record_status_get_8c65f40e', 0x8c65f40e, 0x8c65f41e, True),
    ('sa_input_device_record_status_set_8c65f41e', 0x8c65f41e, 0x8c65f42e, True),
]


def offset(address):
    return (address & 0x1fffffff) - 0x0c000000


def main():
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    pointer_cell = struct.unpack_from('<I', binary, 0x8c65f3d8 - LOAD_BASE)[0]
    assert pointer_cell == POINTER_CELL
    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/input_device_record_status.dll'))
    source = ROOT / 'outputs/sonic-adventure/source/sa_input_device_record_status.c'
    rng = random.Random(0x8c65f3f4)
    baseline = bytearray([0xA5]) * RAM_SIZE
    binary_at = offset(LOAD_BASE)
    baseline[binary_at:binary_at + len(binary)] = binary
    results = []

    for symbol, address, end, uses_macl in SPECS:
        body = binary[address - LOAD_BASE:end - LOAD_BASE]
        function = getattr(library, symbol)
        if uses_macl:
            function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                                 ctypes.POINTER(ctypes.c_uint32)]
        else:
            function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
        function.restype = None
        cases = []

        for case in range(CASES):
            ram = bytearray(baseline)
            record_address = TABLE_BASE + 0x28 + (case % 16) * 0x2c
            struct.pack_into('<I', ram, offset(POINTER_CELL), TABLE_BASE)
            initial_field = rng.getrandbits(32)
            initial_status = rng.randrange(256)
            ram[offset(record_address + 0x20):offset(record_address + 0x24)] = initial_field.to_bytes(4, 'little')
            ram[offset(record_address + 0x0c)] = initial_status
            ram[offset(record_address + 0x0d)] = rng.randrange(256)
            retail_ram, native_ram = bytearray(ram), bytearray(ram)

            initial = State()
            initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
            initial.r[4] = case % 16
            initial.r[15] = 0x0cff0000
            initial.sr = rng.getrandbits(32)
            initial.pr = 0x0c00a000 + case * 4
            initial.pc = address
            if address == 0x8c65f3f4:
                initial.r[5] = initial_field if case % 2 == 0 else (initial_field ^ 1)
            expected = State.from_buffer_copy(bytes(initial))
            actual = State.from_buffer_copy(bytes(initial))
            initial_macl = rng.getrandbits(32)
            special = {'macl': initial_macl}

            steps = reference(retail_ram, expected, special=special)
            view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
            native_macl = ctypes.c_uint32(initial_macl)
            function(view, ctypes.byref(actual), ctypes.byref(native_macl))
            assert native_ram == retail_ram, f'{symbol}: RAM differs in case {case}'
            assert bytes(actual) == bytes(expected), f'{symbol}: CPU differs in case {case}'
            assert native_macl.value == special['macl'], f'{symbol}: MACL differs in case {case}'
            if address == 0x8c65f3f4:
                assert actual.r[0] == int(case % 2 == 0)
            elif address == 0x8c65f40e:
                assert actual.r[0] == initial_status
            else:
                assert native_ram[offset(record_address + 0x0d)] == 1
            cases.append({'case': case, 'index': case % 16,
                          'returned_r0': hex(actual.r[0]), 'retail_instructions': steps,
                          'full_ram_cpu_and_macl_match': True})

        results.append({
            'module': '1ST_READ.BIN', 'load_address': hex(LOAD_BASE),
            'name': symbol, 'address': hex(address), 'end_exclusive': hex(end),
            'retail_span_sha256': hashlib.sha256(body).hexdigest(),
            'cases_passed': len(cases), 'cases': cases,
        })

    report = {
        'functions_verified': len(results), 'total_cases': sum(r['cases_passed'] for r in results),
        'comparison_level': 'Complete 16 MiB RAM and all GPRs, SR, PR, return PC, and MACL compared.',
        'scope': 'Compares the input-device record equality test, status-byte getter, and status-byte setter against retail, including nested 0x8c65f390 index calculation and stack/PR restoration.',
        'limitations': ['The input record layout is controlled RAM fixture state; ownership and broader device lifecycle remain incomplete.',
                        'Uses the project SH-4 interpreter, not Dreamcast hardware or end-to-end gameplay.'],
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'routines': results,
    }
    path = ROOT / 'outputs/sonic-adventure/analysis/input_device_record_status_verification.json'
    path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"Three input-device record routines passed {report['total_cases']} full-state comparisons")


if __name__ == '__main__':
    main()
