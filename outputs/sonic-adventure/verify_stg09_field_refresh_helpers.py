"""Verify two STG09 object-to-record field refresh loops against retail."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

BASE, RAM_SIZE = 0x0c900000, 16 * 1024 * 1024
SPECS = [
    (0x0c90bd42, 0x0c90bd5a, 16, 12, 20, 'sa_stg09_refresh_field14_from_object_0c90bd42'),
    (0x0c90bf34, 0x0c90bf4c, 12, 8, 12, 'sa_stg09_refresh_field08_from_object_0c90bf34'),
]
COUNTS = [0, 1, 2, 3, 4, 5, 8, 16, 0xffffffff]


def main():
    out = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
    lib = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/stg09_field_refresh_helpers.dll'))
    original = bytearray(b'\xa5') * RAM_SIZE
    original[BASE - 0x0c000000:BASE - 0x0c000000 + len(binary)] = binary
    rng = random.Random(0x5354473039524546)
    routines = []

    for address, end, stride, record_field, object_field, export in SPECS:
        function = getattr(lib, export)
        function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
        function.restype = None
        offset = address - BASE
        cases = []
        for case in range(96):
            count = COUNTS[case % len(COUNTS)] if case < 36 else rng.randrange(0, 17)
            if count == 0xffffffff:
                signed_count = -1
                fixture_count = 0
            else:
                signed_count = count
                fixture_count = count
            record_base, object_base = 0x0c300000, 0x0c500000
            retail_ram, actual_ram = bytearray(original), bytearray(original)
            for i in range(fixture_count):
                record = record_base + i * stride
                obj = object_base + i * 0x100
                value = rng.getrandbits(32)
                for ram in (retail_ram, actual_ram):
                    struct.pack_into('<I', ram, record - 0x0c000000, obj)
                    struct.pack_into('<I', ram, obj + object_field - 0x0c000000, value)
                    struct.pack_into('<I', ram, record + record_field - 0x0c000000,
                                     rng.getrandbits(32))

            expected = State()
            expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
            expected.r[4] = record_base
            expected.r[5] = signed_count & 0xffffffff
            expected.r[15] = 0x0cff0000
            expected.sr = rng.getrandbits(32)
            expected.pr = 0x0c001000 + case * 4
            expected.pc = address
            actual = State.from_buffer_copy(bytes(expected))
            steps = reference(retail_ram, expected)
            ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
            function(ram_view, ctypes.byref(actual))
            assert retail_ram == actual_ram, (
                f'RAM mismatch at {address:#x}, case {case}, count {signed_count}')
            assert list(actual.r[8:16]) == list(expected.r[8:16]), (
                f'callee-saved CPU-state mismatch at {address:#x}, case {case}')
            assert actual.pr == expected.pr and actual.pc == expected.pc, (
                f'call-boundary mismatch at {address:#x}, case {case}')
            full_cpu_match = address == 0x0c90bd42
            if full_cpu_match:
                assert bytes(actual) == bytes(expected), (
                    f'full CPU-state mismatch at {address:#x}, case {case}, count {signed_count}')
            cases.append({'case': case, 'count': signed_count,
                          'retail_instructions': steps,
                          'full_ram_cpu_match': full_cpu_match,
                          'abi_state_and_ram_match': True})

        routines.append({'module': 'STG09.BIN', 'load_address': hex(BASE),
                         'address': hex(address), 'end_exclusive': hex(end),
                         'name': export, 'retail_span_sha256':
                         hashlib.sha256(binary[offset:offset + end - address]).hexdigest(),
                         'cases_passed': len(cases),
                         'comparison_level': ('All 16 MiB RAM and complete GPR/SR/PR/PC state.'
                                              if address == 0x0c90bd42 else
                                              'All 16 MiB RAM, preserved GPRs R8-R14, SP, PR and return PC; volatile GPRs and condition codes excluded.'),
                         'cases': cases})

    source = out / 'source/stg09_field_refresh_helpers.c'
    report = {
        'functions_verified': len(routines), 'total_abi_cases': sum(x['cases_passed'] for x in routines),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'comparison_level': 'Per-routine levels are recorded below: full CPU state for 0x0c90bd42; preserved ABI state for 0x0c90bf34.',
        'scope': 'Two STG09 loops copy object fields at +0x14 or +0x0c back into records, with record strides of 16 or 12 bytes. The 0x0c90bd42 loop compares complete GPR/SR/PR/PC state; the related 0x0c90bf34 helper retains its ABI-state comparison.',
        'limitations': ['Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or end-to-end STG09 gameplay.'],
        'routines': routines,
    }
    path = out / 'analysis/common-overlays/STG09.BIN/field_refresh_helpers_verification.json'
    path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"{len(routines)} STG09 field-refresh loops passed {report['total_abi_cases']} RAM and ABI cases")


if __name__ == '__main__':
    main()
