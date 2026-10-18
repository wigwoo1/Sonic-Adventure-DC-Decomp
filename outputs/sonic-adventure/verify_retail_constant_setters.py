"""Verify simple constant setters against the retail instruction interpreter."""
import ctypes
import hashlib
import json
import random
import struct
from pathlib import Path

from verify_startup import ROOT, State, reference

SPECS = [
    (0x8c04c688, 0x8c04c780, 0, 1, 0, 2),
    (0x8c053a00, 0x8c053a78, 2, 3, 1, 4),
    (0x8c055c44, 0x8c055d08, 1, 2, 0, 4),
    (0x8c0585ba, 0x8c058614, 2, 3, 1, 4),
    (0x8c05b1da, 0x8c05b23c, 2, 3, 0, 2),
    (0x8c0818ce, 0x8c081968, 1, 2, 1, 4),
    (0x8c0818d6, 0x8c081968, 0, 3, 2, 4),
    (0x8c0869e0, 0x8c086b00, 2, 3, 0, 4),
    (0x8c08a074, 0x8c08a0f0, 1, 2, 0, 2),
    (0x8c08a5ae, 0x8c08a6a8, 1, 2, 1, 4),
    (0x8c08a5b6, 0x8c08a6a8, 0, 3, 0, 4),
    (0x8c08fe08, 0x8c08feac, 1, 3, 1, 4),
    (0x8c08fe10, 0x8c08feac, 0, 2, 0, 4),
    (0x8c0a2ba0, 0x8c0a2c20, 2, 3, 2, 4),
    (0x8c0a3fa0, 0x8c0a4060, 2, 3, 2, 4),
    (0x8c0cb9e8, 0x8c0cba24, 1, 2, 0, 4),
    (0x8c0df0d0, 0x8c0df0ec, 1, 2, 0, 4),
    (0x8c63adb0, 0x8c63adbc, 2, 3, 0, 4),
    (0x8c65ad60, 0x8c65adb8, 2, 3, 0, 4),
    (0x8c660898, 0x8c660954, 2, 3, 0, 4),
]
BASE, RAM_SIZE = 0x8c010000, 16 * 1024 * 1024


def main():
    out = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    lib = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/retail_constant_setters.dll'))
    original = bytearray(b'\xa5') * RAM_SIZE
    original[0x10000:0x10000 + len(binary)] = binary
    rng = random.Random(0x52455441494c)
    results = []

    for address, literal, pointer_register, value_register, value, width in SPECS:
        offset = address - BASE
        first, immediate, rts, delayed_store = struct.unpack_from('<HHHH', binary, offset)
        calculated_literal = ((address + 4) & ~3) + (first & 255) * 4
        assert first & 0xf000 == 0xd000 and calculated_literal == literal
        assert immediate & 0xf000 == 0xe000 and ((immediate >> 8) & 15) == value_register
        assert (immediate & 255 if immediate & 255 < 128 else (immediate & 255) - 256) == value
        assert rts == 0x000b
        assert ((first >> 8) & 15) == pointer_register
        assert ((delayed_store >> 8) & 15) == pointer_register
        assert ((delayed_store >> 4) & 15) == value_register
        assert (delayed_store & 15) == {1: 0, 2: 1, 4: 2}[width]

        function = getattr(lib, f'sa_retail_constant_setter_{address:08x}')
        function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
        function.restype = None
        cases = []
        for case in range(16):
            expected = State()
            expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
            expected.sr = rng.getrandbits(32)
            expected.pr = 0x8c000400 + case * 4
            expected.pc = address
            actual = State.from_buffer_copy(bytes(expected))
            retail_ram, actual_ram = bytearray(original), bytearray(original)
            steps = reference(retail_ram, expected)
            ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
            function(ram_view, ctypes.byref(actual))
            assert steps == 4
            assert actual_ram == retail_ram, f'RAM mismatch at {address:#x}, case {case}'
            assert bytes(actual) == bytes(expected), f'CPU mismatch at {address:#x}, case {case}'
            cases.append({'case': case, 'retail_steps': steps,
                          'full_state_and_ram_match': True})
        results.append({'address': hex(address), 'end_exclusive': hex(address + 8),
                        'literal_address': hex(literal), 'pointer_register': pointer_register,
                        'value_register': value_register, 'value': value, 'store_width': width,
                        'retail_span_sha256': hashlib.sha256(
                            binary[offset:offset + 8]).hexdigest(),
                        'cases_passed': len(cases), 'cases': cases})

    source = out / 'source/retail_constant_setters.c'
    report = {'functions_verified': len(results),
              'total_full_state_cases': sum(item['cases_passed'] for item in results),
              'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
              'scope': 'Twenty eight-byte indexed constant setters verified across all instructions, register effects and byte/halfword/word stores; 16 seeded full GPR/SR/PR/PC and complete 16 MiB RAM comparisons per routine.',
              'limitations': ['These are isolated leaf state setters; their broader caller and game systems remain incomplete.',
                              'Verification uses the project partial SH-4 interpreter, not hardware or whole-game execution.'],
              'routines': results}
    (out / 'analysis/retail_constant_setters_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"{len(results)} constant setters passed {report['total_full_state_cases']} full-state comparisons")


if __name__ == '__main__':
    main()
