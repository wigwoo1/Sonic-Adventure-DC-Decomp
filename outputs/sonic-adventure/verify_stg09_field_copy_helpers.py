"""Compare five STG09 field-copy loops and their branch thunks with retail."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

BASE, RAM_SIZE = 0x0c900000, 16 * 1024 * 1024
SPECS = [
    ('0c90bcba', 0x0c90bcba, 18, 20, 12, 20, 2, 0x0c90bcae),
    ('0c90bcd0', 0x0c90bcd0, 18, 20, 16, 20, 1, 0x0c90bcc4),
    ('0c90bdca', 0x0c90bdca, 18, 16, 12, 20, 1, 0x0c90bdbe),
    ('0c90bf88', 0x0c90bf88, 18, 12, 4, 12, 2, 0x0c90bf4c),
    ('0c90bf9e', 0x0c90bf9e, 18, 12, 8, 12, 1, 0x0c90bf92),
]


def write32(ram, address, value):
    offset = (address & 0x1fffffff) - 0x0c000000
    ram[offset:offset + 4] = int(value & 0xffffffff).to_bytes(4, 'little')


def main():
    out = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
    lib = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/stg09_field_copy_helpers.dll'))
    original = bytearray(b'\xa5') * RAM_SIZE
    original[BASE - 0x0c000000:BASE - 0x0c000000 + len(binary)] = binary
    rng = random.Random(0x5354473039464945)
    reports = []
    thunk_words = {0x0c90bcae: bytes.fromhex('04 a0 09 00'),
                   0x0c90bcc4: bytes.fromhex('04 a0 09 00'),
                   0x0c90bdbe: bytes.fromhex('04 a0 09 00'),
                   0x0c90bf4c: bytes.fromhex('1c a0 09 00'),
                   0x0c90bf92: bytes.fromhex('04 a0 09 00')}

    for name, address, span, stride, source_offset, destination_offset, object_reg, thunk in SPECS:
        if destination_offset == 20:
            symbols = {
                0x0c90bcba: 'field14_stride20_source12_object_r2',
                0x0c90bcd0: 'field14_stride20_source16_object_r1',
                0x0c90bdca: 'field14_stride16_source12_object_r1',
            }
        else:
            symbols = {
                0x0c90bf88: 'field0c_stride12_source04_object_r2',
                0x0c90bf9e: 'field0c_stride12_source08_object_r1',
            }
        symbol = symbols[address]
        actual_function = getattr(lib, f'sa_stg09_copy_{symbol}_{address:08x}')
        thunk_function = getattr(lib, f'sa_stg09_copy_{symbol}_{thunk:08x}')
        for function in (actual_function, thunk_function):
            function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
            function.restype = None

        offset = address - BASE
        assert len(binary[offset:offset + span]) == span
        item_cases = {address: [], thunk: []}
        for case in range(64):
            count = [0, 1, 2, 3, 4, 5, 8, rng.randrange(0, 9)][case % 8]
            records = 0x0c300000
            objects = [0x0c500000 + i * 0x100 for i in range(count)]
            retail_ram, actual_ram = bytearray(original), bytearray(original)
            for i, obj in enumerate(objects):
                record = records + i * stride
                write32(retail_ram, record, obj)
                write32(actual_ram, record, obj)
                value = rng.getrandbits(32)
                write32(retail_ram, record + source_offset, value)
                write32(actual_ram, record + source_offset, value)

            initial = State()
            initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
            initial.r[4], initial.r[5] = records, count
            initial.sr = rng.getrandbits(32)
            initial.pr = 0x0c001000 + case * 4

            for entry, function in ((address, actual_function), (thunk, thunk_function)):
                expected = State.from_buffer_copy(bytes(initial))
                expected.pc = entry
                changed_ram = bytearray(retail_ram)
                observed_ram = bytearray(actual_ram)
                steps = reference(changed_ram, expected)
                ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(observed_ram)
                actual = State.from_buffer_copy(bytes(initial))
                actual.pc = entry
                function(ram_view, ctypes.byref(actual))
                assert changed_ram == observed_ram, (
                    f'RAM mismatch at {address:#x} entry {entry:#x}, case {case}, count {count}')
                assert bytes(actual) == bytes(expected), (
                    f'CPU-state mismatch at {address:#x} entry {entry:#x}, case {case}, '
                    f'count {count}: actual r1-r5={[hex(v) for v in actual.r[1:6]]}, '
                    f'retail={[hex(v) for v in expected.r[1:6]]}, '
                    f'sr {actual.sr:#x}/{expected.sr:#x}')
                item_cases[entry].append({'case': case, 'entry_address': hex(entry),
                                          'count': count, 'retail_instructions': steps,
                                          'full_state_and_ram_match': True})

        for entry, item_name, item_span in (
            (address, f'sa_stg09_copy_{symbol}_{address:08x}', span),
            (thunk, f'sa_stg09_copy_{symbol}_{thunk:08x}', 4),
        ):
            off = entry - BASE
            if entry == thunk:
                assert binary[off:off + 4] == thunk_words[thunk]
            reports.append({'module': 'STG09.BIN', 'load_address': hex(BASE),
                            'address': hex(entry), 'end_exclusive': hex(entry + item_span),
                            'name': item_name, 'retail_span_sha256':
                            hashlib.sha256(binary[off:off + item_span]).hexdigest(),
                            'cases_passed': len(item_cases[entry]),
                            'cases': item_cases[entry]})

    source = out / 'source/stg09_field_copy_helpers.c'
    result = {
        'functions_verified': len(reports),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'total_full_state_cases': sum(item['cases_passed'] for item in reports),
        'scope': 'Five STG09 list field-copy loops and their five direct branch-thunk entries. Valid counts from zero through eight, randomized records and field values; complete SH-4 CPU state and all 16 MiB RAM compared with retail execution.',
        'limitations': ['Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or end-to-end STG09 gameplay.'],
        'routines': reports,
    }
    path = out / 'analysis/common-overlays/STG09.BIN/field_copy_helpers_verification.json'
    path.write_text(json.dumps(result, indent=2) + '\n', encoding='utf-8')
    print(f"{len(reports)} STG09 routines passed {result['total_full_state_cases']} full-state and RAM comparisons")


if __name__ == '__main__':
    main()
