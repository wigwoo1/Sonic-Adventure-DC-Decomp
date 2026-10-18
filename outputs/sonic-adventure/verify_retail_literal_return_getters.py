"""Verify literal-return functions in the executable and B_EGM2 overlay."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

RAM_SIZE = 16 * 1024 * 1024
SPECS = [
    ('1ST_READ.BIN', 0x8c010000, 0x8c0dff4a, 0x8c0dff78),
    ('B_EGM2.BIN', 0x0c900000, 0x0c901564, 0x0c901604),
]


def main():
    out = ROOT / 'outputs/sonic-adventure'
    lib = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/retail_literal_return_getters.dll'))
    rng = random.Random(0x4C49544552455455)
    routines = []

    for module, load_base, address, literal in SPECS:
        if module == '1ST_READ.BIN':
            binary_path = ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN'
        else:
            binary_path = ROOT / f'work/sonic-adventure/decoded/SONICADV/{module}'
        binary = binary_path.read_bytes()
        off = address - load_base
        words = struct.unpack_from('<HHH', binary, off)
        assert words[0] >> 12 == 0xD and ((words[0] >> 8) & 15) == 0
        calculated_literal = ((address + 4) & ~3) + (words[0] & 255) * 4
        assert calculated_literal == literal
        assert words[1:] == (0x000B, 0x0009)
        function = getattr(lib, f'sa_literal_return_getter_{address:08x}')
        function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
        function.restype = None
        original = bytearray(b'\xa5') * RAM_SIZE
        physical_base = load_base & 0x00ffffff
        assert physical_base + len(binary) <= RAM_SIZE
        original[physical_base:physical_base + len(binary)] = binary
        cases = []

        for case in range(16):
            expected = State()
            expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
            expected.sr = rng.getrandbits(32)
            expected.pr = (address & 0xfffff000) + 0x100 + case * 4
            expected.pc = address
            actual = State.from_buffer_copy(bytes(expected))
            retail_ram, actual_ram = bytearray(original), bytearray(original)
            steps = reference(retail_ram, expected)
            ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
            function(ram_view, ctypes.byref(actual))
            assert steps == 3
            assert actual_ram == retail_ram, f'RAM mismatch at {module}:{address:#x}, case {case}'
            assert bytes(actual) == bytes(expected), f'CPU mismatch at {module}:{address:#x}, case {case}'
            cases.append({'case': case, 'literal_value': hex(expected.r[0]),
                          'full_state_and_ram_match': True})

        routines.append({'module': module, 'address': hex(address),
                         'end_exclusive': hex(address + 6),
                         'literal_address': hex(literal),
                         'literal_value': hex(struct.unpack_from('<I', binary, literal - load_base)[0]),
                         'retail_span_sha256': hashlib.sha256(binary[off:off + 6]).hexdigest(),
                         'cases_passed': len(cases), 'cases': cases})

    source = out / 'source/retail_literal_return_getters.c'
    report = {
        'functions_verified': len(routines),
        'total_full_state_cases': sum(item['cases_passed'] for item in routines),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'scope': 'Two six-byte PC-relative MOV.L-to-R0, RTS, NOP routines; validated the literal address and value, then compared full CPU state and 16 MiB RAM over 16 cases each.',
        'limitations': [
            'This verifies the getter bodies only; the meaning and downstream consumers of returned values remain under analysis.',
            'Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or end-to-end game execution.'
        ],
        'routines': routines,
    }
    (out / 'analysis/retail_literal_return_getters_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"{len(routines)} literal-return getters passed {report['total_full_state_cases']} full-state comparisons")


if __name__ == '__main__':
    main()
