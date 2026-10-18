"""Differentially verify input-device table-index address helpers."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

LOAD_BASE, RAM_SIZE, CASES = 0x8c010000, 16 * 1024 * 1024, 128
POINTER_LITERAL, POINTER_CELL, TABLE_BASE = 0x8c65f3d8, 0x8c8a83b4, 0x0c700000
ROUTINES = [
    ('sa_input_device_record_index_8c65f390', 0x8c65f390, 0x8c65f3a0, 'macl', 0x2c, 0x28, None),
    ('sa_input_device_record_index_8c65f3a0', 0x8c65f3a0, 0x8c65f3b2, 'macl', 0x78, None, 0x8c65f3ca),
    ('sa_input_device_record_index_8c65f3b2', 0x8c65f3b2, 0x8c65f3c0, None, 8, 4, None),
]


def offset(address):
    return (address & 0x1fffffff) - 0x0c000000


def main():
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    assert struct.unpack_from('<I', binary, POINTER_LITERAL - LOAD_BASE)[0] == POINTER_CELL
    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/input_device_record_index_helpers.dll'))
    source_path = ROOT / 'outputs/sonic-adventure/source/sa_input_device_record_index_helpers.c'
    rng = random.Random(0x8c65f390)
    baseline = bytearray([0xA5]) * RAM_SIZE
    main_at = offset(LOAD_BASE)
    baseline[main_at:main_at + len(binary)] = binary
    records = []

    for symbol, address, end, uses_macl, stride, base_add, offset_literal in ROUTINES:
        body = binary[address - LOAD_BASE:end - LOAD_BASE]
        assert len(body) == end - address
        function = getattr(library, symbol)
        if uses_macl:
            function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                                 ctypes.POINTER(ctypes.c_uint32)]
        else:
            function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
        function.restype = None
        cases = []
        literal_offset = None
        if offset_literal is not None:
            literal_offset = struct.unpack_from('<h', binary, offset_literal - LOAD_BASE)[0]
            assert literal_offset == 0x44c

        for case in range(CASES):
            ram = bytearray(baseline)
            struct.pack_into('<I', ram, offset(POINTER_CELL), TABLE_BASE)
            retail_ram, native_ram = bytearray(ram), bytearray(ram)
            initial = State()
            initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
            initial.r[4] = [0, 1, 2, 0x7fffffff, 0x80000000, 0xffffffff][case % 6]
            if case >= 6:
                initial.r[4] = rng.getrandbits(32)
            initial.r[15] = 0x0cff0000
            initial.sr = rng.getrandbits(32)
            initial.pr = 0x0c009000 + case * 4
            initial.pc = address
            expected = State.from_buffer_copy(bytes(initial))
            actual = State.from_buffer_copy(bytes(initial))
            initial_macl = rng.getrandbits(32)
            special = {'macl': initial_macl}

            steps = reference(retail_ram, expected, special=special)
            view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
            if uses_macl:
                actual_macl = ctypes.c_uint32(initial_macl)
                function(view, ctypes.byref(actual), ctypes.byref(actual_macl))
                assert actual_macl.value == special['macl'], f'{symbol}: MACL differs in case {case}'
            else:
                function(view, ctypes.byref(actual))
            assert native_ram == retail_ram, f'{symbol}: RAM differs in case {case}'
            assert bytes(actual) == bytes(expected), (f'{symbol}: CPU differs in case {case}; '
                f'regs={[ (i, hex(actual.r[i]), hex(expected.r[i])) for i in range(16) if actual.r[i] != expected.r[i] ]}; '
                f'sr={(hex(actual.sr), hex(expected.sr))}; pc={(hex(actual.pc), hex(expected.pc))}')
            cases.append({'case': case, 'index': hex(initial.r[4]),
                          'return_address': hex(expected.r[0]),
                          'retail_instructions': steps, 'full_ram_cpu_and_macl_match': True})

        records.append({
            'module': '1ST_READ.BIN', 'load_address': hex(LOAD_BASE),
            'name': symbol, 'address': hex(address), 'end_exclusive': hex(end),
            'pointer_literal_address': hex(POINTER_LITERAL),
            'pointer_global_cell': hex(POINTER_CELL), 'stride': hex(stride),
            'base_addend': None if base_add is None else hex(base_add),
            'offset_literal_address': None if offset_literal is None else hex(offset_literal),
            'offset_literal_value': None if literal_offset is None else hex(literal_offset),
            'retail_span_sha256': hashlib.sha256(body).hexdigest(),
            'cases_passed': len(cases), 'cases': cases,
        })

    report = {
        'functions_verified': len(records), 'total_cases': sum(r['cases_passed'] for r in records),
        'comparison_level': 'Complete 16 MiB RAM and all GPRs, SR, PR, return PC, and MACL where used compared.',
        'scope': 'Three input-device table-index helpers compute pointers from the shared table base, fixed strides, and literal addends; each is compared instruction-by-instruction with retail behavior.',
        'limitations': ['Structure ownership and semantic names remain under analysis.',
                        'Uses the project SH-4 interpreter, not Dreamcast hardware or end-to-end gameplay.'],
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256(source_path.read_bytes()).hexdigest(),
        'routines': records,
    }
    path = ROOT / 'outputs/sonic-adventure/analysis/input_device_record_index_helpers_verification.json'
    path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"Three input-device index helpers passed {report['total_cases']} full-state comparisons")


if __name__ == '__main__':
    main()
