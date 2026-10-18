"""Differentially verify compact retail global indirect-load routines."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

BASE, RAM_SIZE = 0x8c010000, 16 * 1024 * 1024
SPECS = [
    (0x8c01b37a, 0x8c01b3c0, 3, 0, 4),
    (0x8c04f8f4, 0x8c04f954, 2, 0, 2),
    (0x8c050a06, 0x8c050a2c, 2, 0, 1),
    (0x8c0585e2, 0x8c05861c, 2, 0, 4),
    (0x8c062704, 0x8c062740, 3, 0, 4),
    (0x8c0818de, 0x8c081968, 2, 0, 4),
    (0x8c08a2bc, 0x8c08a354, 1, 0, 2),
    (0x8c08a572, 0x8c08a68c, 1, 0, 1),
    (0x8c08a71e, 0x8c08a7a8, 2, 0, 4),
    (0x8c0a7770, 0x8c0a778c, 3, 0, 4),
    (0x8c0df0ca, 0x8c0df0ec, 3, 0, 4),
    (0x8c107bc2, 0x8c107c70, 3, 0, 4),
    (0x8c646eda, 0x8c646f58, 3, 0, 4),
    (0x8c657968, 0x8c657a00, 3, 0, 4),
]


def main():
    out = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    binary_sha256 = hashlib.sha256(binary).hexdigest()
    lib = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/retail_global_indirect_getters.dll'))
    original = bytearray(b'\xa5') * RAM_SIZE
    original[0x10000:0x10000 + len(binary)] = binary
    rng = random.Random(0x474C4F42414C4745)
    routines = []

    for address, literal, pointer_reg, destination_reg, width in SPECS:
        off = address - BASE
        words = struct.unpack_from('<HHH', binary, off)
        assert address % 2 == 0
        assert words[0] >> 12 == 0xD and ((words[0] >> 8) & 15) == pointer_reg
        calculated_literal = ((address + 4) & ~3) + (words[0] & 255) * 4
        assert calculated_literal == literal
        assert words[1] == 0x000B
        assert words[2] >> 12 == 0x6
        assert ((words[2] >> 8) & 15) == destination_reg
        assert ((words[2] >> 4) & 15) == pointer_reg
        assert (words[2] & 15) == {1: 0, 2: 1, 4: 2}[width]
        pointer = struct.unpack_from('<I', binary, literal - BASE)[0]
        assert pointer % width == 0, (hex(address), hex(pointer), width)
        function = getattr(lib, f'sa_global_indirect_getter_{address:08x}')
        function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
        function.restype = None
        cases = []

        boundaries = {
            1: [0, 1, 0x7f, 0x80, 0xff],
            2: [0, 1, 0x7fff, 0x8000, 0xffff],
            4: [0, 1, 0x7fffffff, 0x80000000, 0xffffffff],
        }[width]
        for case in range(64):
            fixture_value = boundaries[case % len(boundaries)] if case < len(boundaries) else rng.getrandbits(width * 8)
            slot = pointer & 0x00ffffff
            if width == 1:
                original[slot] = fixture_value
            elif width == 2:
                struct.pack_into('<H', original, slot, fixture_value)
            else:
                struct.pack_into('<I', original, slot, fixture_value)
            expected = State()
            expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
            expected.sr = rng.getrandbits(32)
            expected.pr = 0x8c001800 + case * 4
            expected.pc = address
            actual = State.from_buffer_copy(bytes(expected))
            retail_ram, actual_ram = bytearray(original), bytearray(original)
            steps = reference(retail_ram, expected)
            ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
            function(ram_view, ctypes.byref(actual))
            assert steps == 3
            assert actual_ram == retail_ram, f'RAM mismatch at {address:#x}, case {case}'
            assert bytes(actual) == bytes(expected), f'CPU mismatch at {address:#x}, case {case}'
            cases.append({'case': case, 'fixture_value': hex(fixture_value),
                          'full_state_and_ram_match': True})

        routines.append({'address': hex(address), 'end_exclusive': hex(address + 6),
                         'literal_address': hex(literal), 'pointer_value': hex(pointer),
                         'pointer_register': pointer_reg,
                         'destination_register': destination_reg, 'load_width_bytes': width,
                         'retail_span_sha256': hashlib.sha256(binary[off:off + 6]).hexdigest(),
                         'cases_passed': len(cases), 'cases': cases})

    source = out / 'source/retail_global_indirect_getters.c'
    report = {
        'functions_verified': len(routines),
        'total_full_state_cases': sum(item['cases_passed'] for item in routines),
        'binary_sha256': binary_sha256,
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'scope': 'Fourteen six-byte 1ST_READ.BIN accessors; validated the PC-relative pointer literal, RTS delay-slot load, full CPU state and complete 16 MiB RAM over 64 controlled global-value fixtures per routine, with explicit signed/unsigned width boundaries.',
        'limitations': [
            'Global storage was assigned controlled values for differential execution; surrounding data ownership and callers remain incomplete.',
            'Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or end-to-end game execution.'
        ],
        'routines': routines,
    }
    (out / 'analysis/retail_global_indirect_getters_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"{len(routines)} indirect getters passed {report['total_full_state_cases']} full-state comparisons")


if __name__ == '__main__':
    main()
