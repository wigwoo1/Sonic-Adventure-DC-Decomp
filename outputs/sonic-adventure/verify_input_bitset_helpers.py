"""Differentially verify input bitset and key-index helper routines."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

BASE, RAM_SIZE = 0x8c010000, 16 * 1024 * 1024
SPECS = [(0x8c0a68d8, 64, 'bit_test'),
         (0x8c0a6960, 50, 'bit_count'),
         (0x8c0a6992, 22, 'index_to_code')]
SYMBOLS = {'bit_test': 'bitset_test', 'bit_count': 'bitset_count',
           'index_to_code': 'index_to_code'}
BIT_INDICES = [-1024, -256, -17, -9, -8, -7, -1, 0, 1, 6, 7, 8, 15, 16,
               63, 64, 127, 128, 134, 135]
INDEX_VALUES = [0, 1, 2, 3, 4, 5, 6, 14, 15, 16, 0x22, 0x23, 0x26,
                0x27, 0x2a, 0x2b, 0x7fffffff, 0x80000000, 0xfffffffe, 0xffffffff]


def main():
    out = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    lib = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/input_bitset_helpers.dll'))
    original = bytearray(b'\xa5') * RAM_SIZE
    original[0x10000:0x10000 + len(binary)] = binary
    rng = random.Random(0x494E505554424954)
    routines = []

    for address, span, kind in SPECS:
        function = getattr(lib, f"sa_input_{SYMBOLS[kind]}_8c0a{address & 0xffff:04x}")
        function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
        function.restype = None
        cases = []
        for case in range(64):
            expected = State()
            expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
            expected.sr = rng.getrandbits(32)
            expected.pr = 0x8c001800 + case * 4
            expected.pc = address
            fixture = bytearray(original)
            if kind in ('bit_test', 'bit_count'):
                expected.r[4] = 0x8c100000
                fixture[0x100240:0x100251] = bytes(rng.getrandbits(8) for _ in range(17))
                if kind == 'bit_test':
                    expected.r[5] = (BIT_INDICES[case] & 0xffffffff) if case < len(BIT_INDICES) else rng.randrange(136)
            else:
                expected.r[4] = (INDEX_VALUES[case] if case < len(INDEX_VALUES)
                                 else rng.getrandbits(32))

            actual = State.from_buffer_copy(bytes(expected))
            retail_ram, actual_ram = bytearray(fixture), bytearray(fixture)
            steps = reference(retail_ram, expected)
            ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
            function(ram_view, ctypes.byref(actual))
            assert actual_ram == retail_ram, f'RAM mismatch at {address:#x}, case {case}'
            assert actual.r[0] == expected.r[0], f'return value mismatch at {address:#x}, case {case}'
            assert list(actual.r[8:]) == list(expected.r[8:]), (
                f'callee-saved register mismatch at {address:#x}, case {case}')
            assert actual.pr == expected.pr and actual.pc == expected.pc, (
                f'control-state mismatch at {address:#x}, case {case}')
            cases.append({'case': case, 'retail_instructions': steps,
                          'full_state_and_ram_match': True})

        off = address - BASE
        routines.append({'address': hex(address), 'end_exclusive': hex(address + span),
                         'kind': kind,
                         'retail_span_sha256': hashlib.sha256(binary[off:off + span]).hexdigest(),
                         'cases_passed': len(cases), 'cases': cases})

    source = out / 'source/input_bitset_helpers.c'
    report = {
        'functions_verified': len(routines),
        'total_abi_cases': sum(item['cases_passed'] for item in routines),
        'comparison_level': 'Return register R0, preserved registers R8-R15, PR/PC, and complete RAM; caller-clobbered R1-R7 and SR are not compared.',
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'scope': 'Three input bitset helpers: signed bit test, population count over 17 bytes, and key-index mapping. Compared return value, preserved R8-R15, PR/PC, and full 16 MiB RAM against retail SH-4 execution across edge and randomized inputs. Caller-clobbered R1-R7 and SR are excluded because the C translation need not reproduce scratch-register allocation.',
        'limitations': [
            'Negative bit indices were tested against retail execution, including wrapped byte offsets; public callers still need complete ABI and lifecycle analysis.',
            'Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or end-to-end game execution.'
        ],
        'routines': routines,
    }
    (out / 'analysis/input_bitset_helpers_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"{len(routines)} input bitset helpers passed {report['total_abi_cases']} ABI-level comparisons")


if __name__ == '__main__':
    main()
