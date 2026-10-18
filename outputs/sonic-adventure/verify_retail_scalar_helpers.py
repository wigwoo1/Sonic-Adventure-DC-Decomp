"""Verify three compact integer and memory helpers against retail SH-4."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

BASE, RAM_SIZE = 0x8c010000, 16 * 1024 * 1024
SPECS = [(0x8c5ff396, 6, 'address_minus_30'),
         (0x8c602618, 6, 'subtract_arguments'),
         (0x8c09de34, 6, 'zero_field_plus4')]
EDGE_VALUES = [0, 1, 0x2f, 0x30, 0x31, 0x7fffffff,
               0x80000000, 0xffffffcf, 0xffffffff]


def main():
    out = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    lib = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/retail_scalar_helpers.dll'))
    original = bytearray(b'\xa5') * RAM_SIZE
    original[0x10000:0x10000 + len(binary)] = binary
    rng = random.Random(0x5343414C41524845)
    routines = []

    for address, span, name in SPECS:
        function = getattr(lib, f'sa_{name}_8c{address & 0xffffff:06x}')
        function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
        function.restype = None
        off = address - BASE
        words = struct.unpack_from('<HHH', binary, off)
        if address == 0x8c5ff396:
            assert words == (0x6043, 0x000b, 0x70d0)
        elif address == 0x8c602618:
            assert words == (0x6053, 0x000b, 0x3048)
        else:
            assert words == (0xe300, 0x000b, 0x1431)
        cases = []

        for case in range(64):
            expected = State()
            expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
            if name == 'address_minus_30':
                expected.r[4] = EDGE_VALUES[case] if case < len(EDGE_VALUES) else rng.getrandbits(32)
            elif name == 'subtract_arguments':
                expected.r[4] = EDGE_VALUES[case] if case < len(EDGE_VALUES) else rng.getrandbits(32)
                expected.r[5] = EDGE_VALUES[(case * 3) % len(EDGE_VALUES)] if case < 27 else rng.getrandbits(32)
            else:
                expected.r[4] = 0x8c100000 + case * 0x100
            expected.sr = rng.getrandbits(32)
            expected.pr = 0x8c002c00 + case * 4
            expected.pc = address
            actual = State.from_buffer_copy(bytes(expected))
            retail_ram, actual_ram = bytearray(original), bytearray(original)
            steps = reference(retail_ram, expected)
            ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
            function(ram_view, ctypes.byref(actual))
            assert steps == 3
            assert actual_ram == retail_ram, f'RAM mismatch at {address:#x}, case {case}'
            assert bytes(actual) == bytes(expected), f'CPU-state mismatch at {address:#x}, case {case}'
            cases.append({'case': case, 'retail_instructions': steps,
                          'full_state_and_ram_match': True})

        routines.append({'module': '1ST_READ.BIN', 'address': hex(address),
                         'end_exclusive': hex(address + span), 'name': name,
                         'retail_span_sha256': hashlib.sha256(binary[off:off + span]).hexdigest(),
                         'cases_passed': len(cases), 'cases': cases})

    source = out / 'source/retail_scalar_helpers.c'
    report = {
        'functions_verified': len(routines),
        'total_full_state_cases': sum(item['cases_passed'] for item in routines),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'scope': 'Three six-byte integer/memory helpers: R4-0x30, R5-R4, and a zero longword store at R4+4. Exact CPU state and all 16 MiB RAM were compared with retail execution over edge and randomized values.',
        'limitations': ['Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or end-to-end game execution.'],
        'routines': routines,
    }
    (out / 'analysis/retail_scalar_helpers_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"{len(routines)} scalar helpers passed {report['total_full_state_cases']} full-state comparisons")


if __name__ == '__main__':
    main()
