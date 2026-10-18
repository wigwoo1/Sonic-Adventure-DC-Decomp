"""Verify compact RTS delay-slot stubs in the executable and game overlays."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

RAM_SIZE = 16 * 1024 * 1024
SPECS = [
    ('1ST_READ.BIN', 0x8c042dc4, 'zero'),
    ('1ST_READ.BIN', 0x8c094dc4, 'zero'),
    ('1ST_READ.BIN', 0x8c094dc8, 'zero'),
    ('1ST_READ.BIN', 0x8c094dcc, 'zero'),
    ('1ST_READ.BIN', 0x8c64b2dc, 'zero'),
    ('1ST_READ.BIN', 0x8c65b510, 'zero'),
    ('1ST_READ.BIN', 0x8c65da68, 'zero'),
    ('STG01.BIN', 0x0c90867c, 'zero'),
    ('AL_MAIN.BIN', 0x0c90c560, 'noop'),
    ('AL_MAIN.BIN', 0x0c90c564, 'noop'),
    ('AL_MAIN.BIN', 0x0c90c568, 'noop'),
    ('AL_MAIN.BIN', 0x0c90d210, 'noop'),
    ('B_E101_R.BIN', 0x0c906986, 'noop'),
    ('B_EGM1.BIN', 0x0c900b20, 'noop'),
    ('B_EGM1.BIN', 0x0c900d94, 'store32'),
    ('B_EGM1.BIN', 0x0c903ce0, 'noop'),
    ('B_EGM1.BIN', 0x0c9049c0, 'noop'),
    ('B_ROBO.BIN', 0x0c906b8e, 'noop'),
    ('STG09.BIN', 0x0c90bdd4, 'noop'),
]


def image_path(module):
    if module == '1ST_READ.BIN':
        return ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN', 0x8c010000
    return ROOT / 'work/sonic-adventure/decoded/SONICADV' / module, 0x0c900000


def main():
    out = ROOT / 'outputs/sonic-adventure'
    lib = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/retail_return_delay_stubs.dll'))
    rng = random.Random(0x52545344454C4159)
    routines = []

    for module, address, kind in SPECS:
        path, load_base = image_path(module)
        binary = path.read_bytes()
        offset = address - load_base
        words = struct.unpack_from('<HH', binary, offset)
        assert words[0] == 0x000b
        expected_delay = {'noop': 0x0009, 'zero': 0xe000, 'store32': 0x2452}[kind]
        assert words[1] == expected_delay, (module, hex(address), words)
        function = getattr(lib, f'sa_return_{kind}_{address:08x}')
        function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
        function.restype = None
        cases = []

        for case in range(16):
            original = bytearray(b'\xa5') * RAM_SIZE
            physical_base = load_base & 0x00ffffff
            assert physical_base + len(binary) <= RAM_SIZE
            original[physical_base:physical_base + len(binary)] = binary
            expected = State()
            expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
            if kind == 'store32':
                expected.r[4] = 0x8c100000 + case * 4
            expected.sr = rng.getrandbits(32)
            expected.pr = (address & 0xfffff000) + 0x300 + case * 4
            expected.pc = address
            actual = State.from_buffer_copy(bytes(expected))
            retail_ram, actual_ram = bytearray(original), bytearray(original)
            steps = reference(retail_ram, expected)
            ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
            function(ram_view, ctypes.byref(actual))
            assert steps == 2
            assert actual_ram == retail_ram, f'RAM mismatch at {module}:{address:#x}, case {case}'
            assert bytes(actual) == bytes(expected), f'CPU mismatch at {module}:{address:#x}, case {case}'
            cases.append({'case': case, 'full_state_and_ram_match': True})

        routines.append({
            'module': module, 'address': hex(address), 'end_exclusive': hex(address + 4),
            'kind': kind, 'delay_slot_opcode': hex(expected_delay),
            'retail_span_sha256': hashlib.sha256(binary[offset:offset + 4]).hexdigest(),
            'cases_passed': len(cases), 'cases': cases,
        })

    source = out / 'source/retail_return_delay_stubs.c'
    report = {
        'functions_verified': len(routines),
        'total_full_state_cases': sum(item['cases_passed'] for item in routines),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'scope': 'Nineteen four-byte return routines across 1ST_READ.BIN and six overlays: ten RTS/NOP, eight RTS/MOV #0,R0, and one RTS/MOV.L R5,@R4. Each was compared against retail instructions over randomized full CPU state and 16 MiB RAM fixtures.',
        'limitations': [
            'The store routine destination was constrained to mapped writable RAM; runtime callers and callback roles remain incomplete.',
            'Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or end-to-end game execution.'
        ],
        'routines': routines,
    }
    (out / 'analysis/retail_return_delay_stubs_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"{len(routines)} return-delay stubs passed {report['total_full_state_cases']} full-state comparisons")


if __name__ == '__main__':
    main()
