"""Verify an indexed field-setter family against its retail SH-4 routines."""
import ctypes
import hashlib
import json
import random
import struct
from pathlib import Path

from verify_startup import ROOT, State, reference

SPECS = [(0x8c65f462, 0x8c65f530, 0x8c65f51a),
         (0x8c65f476, 0x8c65f530, 0x8c65f51c),
         (0x8c65f4a2, 0x8c65f530, 0x8c65f51e),
         (0x8c65f4b6, 0x8c65f530, 0x8c65f520),
         (0x8c65f4ee, 0x8c65f530, 0x8c65f528),
         (0x8c65f502, 0x8c65f530, 0x8c65f52a),
         (0x8c65f53c, 0x8c65f554, 0x8c65f550)]
BASE, RAM_SIZE, INDEX_POINTER = 0x8c010000, 16 * 1024 * 1024, 0x8c8a83b4


def main():
    out = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    lib = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/indexed_field_setters.dll'))
    original = bytearray(b'\xa5') * RAM_SIZE
    original[0x10000:0x10000 + len(binary)] = binary
    index_ram = (INDEX_POINTER & 0x1fffffff) - 0x0c000000
    rng = random.Random(0x494e4445584544)
    results = []

    for address, pointer_literal, offset_literal in SPECS:
        off = address - BASE
        words = struct.unpack_from('<HHHHH', binary, off)
        assert words[0] & 0xf000 == 0xd000 and (words[0] >> 8) & 15 == 2
        assert words[1] & 0xf000 == 0x9000 and (words[1] >> 8) & 15 == 0
        calculated_pointer_literal = ((address + 4) & ~3) + (words[0] & 255) * 4
        # SH-4 MOV.W @(disp,PC) uses PC+4 without longword alignment.
        calculated_offset_literal = address + 2 + 4 + (words[1] & 255) * 2
        assert calculated_pointer_literal == pointer_literal
        assert calculated_offset_literal == offset_literal
        assert words[2] == 0x6322 and words[3] == 0x000b and words[4] == 0x0346
        pointer_value = struct.unpack_from('<I', binary, pointer_literal - BASE)[0]
        assert pointer_value == INDEX_POINTER
        field_offset = struct.unpack_from('<h', binary, offset_literal - BASE)[0]

        function = getattr(lib, f'sa_indexed_field_setter_{address:08x}')
        function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
        function.restype = None
        cases = []
        for case in range(16):
            expected = State()
            expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
            index_value = 0x8c100000 + case * 0x100
            struct.pack_into('<I', original, index_ram, index_value)
            expected.sr = rng.getrandbits(32)
            expected.pr = 0x8c001800 + case * 4
            expected.pc = address
            actual = State.from_buffer_copy(bytes(expected))
            retail_ram, actual_ram = bytearray(original), bytearray(original)
            steps = reference(retail_ram, expected)
            ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
            function(ram_view, ctypes.byref(actual))
            assert steps == 5
            assert actual_ram == retail_ram, f'RAM mismatch at {address:#x}, case {case}'
            assert bytes(actual) == bytes(expected), f'CPU mismatch at {address:#x}, case {case}'
            address_written = index_value + field_offset
            write_offset = (address_written & 0x1fffffff) - 0x0c000000
            assert struct.unpack_from('<I', retail_ram, write_offset)[0] == expected.r[4]
            cases.append({'case': case, 'index_value': hex(index_value),
                          'field_offset': field_offset, 'retail_steps': steps,
                          'full_state_and_ram_match': True})
        results.append({'address': hex(address), 'end_exclusive': hex(address + 10),
                        'pointer_literal': hex(pointer_literal),
                        'offset_literal': hex(offset_literal), 'field_offset': field_offset,
                        'retail_span_sha256': hashlib.sha256(
                            binary[off:off + 10]).hexdigest(),
                        'cases_passed': len(cases), 'cases': cases})

    source = out / 'source/indexed_field_setters.c'
    report = {'functions_verified': len(results),
              'total_full_state_cases': sum(item['cases_passed'] for item in results),
              'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
              'scope': 'Seven ten-byte indexed field setters; validated each pointer and signed literal offset, and matched complete CPU state and 16 MiB RAM over 16 fixture index values per routine.',
              'limitations': ['The index global was set to controlled in-RAM bases for differential execution; surrounding runtime ownership and callers remain incomplete.',
                              'Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or end-to-end game execution.'],
              'routines': results}
    (out / 'analysis/indexed_field_setters_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"{len(results)} indexed field setters passed {report['total_full_state_cases']} full-state comparisons")


if __name__ == '__main__':
    main()
