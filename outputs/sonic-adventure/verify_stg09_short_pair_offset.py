"""Compare the STG09 short-coordinate offset loop with retail SH-4."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

BASE, ADDRESS, END = 0x0c900000, 0x0c90bc18, 0x0c90bc4a
RAM_SIZE = 16 * 1024 * 1024
COUNTS = [0, 1, 2, 3, 4, 5, 8, 16]
SHORTS = [0, 1, 0x7fff, 0x8000, 0xffff, 0x0fff, 0xf000]


def main():
    out = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
    lib = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/stg09_short_pair_offset.dll'))
    function = lib.sa_stg09_offset_short_pairs_0c90bc18
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None
    original = bytearray(b'\xa5') * RAM_SIZE
    original[BASE - 0x0c000000:BASE - 0x0c000000 + len(binary)] = binary
    offset = ADDRESS - BASE
    assert binary[offset:offset + END - ADDRESS] == bytes.fromhex(
        'e6 2f 43 6e 10 e4 53 61 18 44 ff 74 49 26 08 a0 '
        '49 27 15 64 6c 34 15 65 41 2e 02 7e 7c 35 51 2e '
        '02 7e f1 53 ff 73 31 1f 01 73 38 23 f1 8b 0b 00 f6 6e')
    rng = random.Random(0x5354473039534854)
    cases = []

    for case in range(160):
        count = COUNTS[case % len(COUNTS)] if case < 40 else rng.randrange(0, 17)
        dx = rng.choice([0, 1, 0x0fff, 0x1000, 0x1234, 0xffff,
                         rng.getrandbits(16), rng.getrandbits(32)])
        dy = rng.choice([0, 1, 0x0fff, 0x1000, 0x5678, 0xffff,
                         rng.getrandbits(16), rng.getrandbits(32)])
        destination, source, stack = 0x0c300000, 0x0c400000, 0x0cff0000
        retail_ram, actual_ram = bytearray(original), bytearray(original)
        struct.pack_into('<I', retail_ram, stack - 0x0c000000, count)
        struct.pack_into('<I', actual_ram, stack - 0x0c000000, count)
        for i in range(max(count, 1)):
            x, y = rng.choice(SHORTS), rng.choice(SHORTS)
            struct.pack_into('<HH', retail_ram, source - 0x0c000000 + i * 4, x, y)
            struct.pack_into('<HH', actual_ram, source - 0x0c000000 + i * 4, x, y)

        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.r[4:8] = [destination, source, dx, dy]
        expected.r[15] = stack
        expected.sr = rng.getrandbits(32)
        expected.pr = 0x0c001000 + case * 4
        expected.pc = ADDRESS
        actual = State.from_buffer_copy(bytes(expected))
        steps = reference(retail_ram, expected)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        function(ram_view, ctypes.byref(actual))
        assert retail_ram == actual_ram, f'RAM mismatch at case {case}, count {count}'
        assert list(actual.r[8:16]) == list(expected.r[8:16]), (
            f'callee-saved CPU-state mismatch at case {case}, count {count}')
        assert actual.pr == expected.pr and actual.pc == expected.pc, (
            f'call-boundary mismatch at case {case}, count {count}')
        cases.append({'case': case, 'count': count, 'dx': hex(dx), 'dy': hex(dy),
                      'retail_instructions': steps, 'abi_state_and_ram_match': True})

    source_path = out / 'source/stg09_short_pair_offset.c'
    report = {
        'functions_verified': 1, 'total_abi_cases': len(cases),
        'source_sha256': hashlib.sha256(source_path.read_bytes()).hexdigest(),
        'comparison_level': 'All 16 MiB RAM, preserved GPRs R8-R14, SP, PR and return PC; volatile GPRs and status condition codes are excluded as caller-clobbered.',
        'scope': 'STG09 routine at 0x0c90bc18 applies two masked 12-bit offsets to packed signed-short coordinate pairs. Count is read from the fifth argument at the retail stack location. The full RAM image and SH-4 call-preservation contract are compared against retail execution.',
        'limitations': ['Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or end-to-end STG09 gameplay.'],
        'routines': [{'module': 'STG09.BIN', 'load_address': hex(BASE),
                      'address': hex(ADDRESS), 'end_exclusive': hex(END),
                      'name': 'short_pair_offset',
                      'module_sha256': hashlib.sha256(binary).hexdigest(),
                      'retail_span_sha256': hashlib.sha256(binary[offset:offset + END - ADDRESS]).hexdigest(),
                      'cases_passed': len(cases), 'cases': cases}],
    }
    report_path = out / 'analysis/common-overlays/STG09.BIN/short_pair_offset_verification.json'
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"STG09 short-pair offset passed {len(cases)} RAM and ABI-preservation comparisons")


if __name__ == '__main__':
    main()
