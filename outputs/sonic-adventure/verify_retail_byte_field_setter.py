"""Verify the indexed byte field store against its three retail SH-4 ops."""
import ctypes
import hashlib
import json
import random
import struct
from pathlib import Path

from verify_startup import ROOT, State, reference

ENTRY, SIZE, BASE = 0x8c6446d2, 6, 0x8c010000
RAM_SIZE = 16 * 1024 * 1024


def main():
    out = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    assert struct.unpack_from('<HHH', binary, ENTRY - BASE) == (0xe06d, 0x000b, 0x0454)
    lib = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/retail_byte_field_setter.dll'))
    function = lib.sa_retail_byte_field_setter_8c6446d2
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None
    template = bytearray(b'\xa5') * RAM_SIZE
    template[0x10000:0x10000 + len(binary)] = binary
    rng = random.Random(ENTRY)
    cases = []

    for case in range(64):
        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.r[4] = 0x8c300000 + case * 0x100
        expected.sr = rng.getrandbits(32)
        expected.pr = 0x8c001000 + case * 4
        expected.pc = ENTRY
        actual = State.from_buffer_copy(bytes(expected))
        retail_ram, actual_ram = bytearray(template), bytearray(template)
        steps = reference(retail_ram, expected)
        view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        function(view, ctypes.byref(actual))
        assert steps == 3
        assert actual_ram == retail_ram, f'RAM mismatch in case {case}'
        assert bytes(actual) == bytes(expected), f'CPU mismatch in case {case}'
        assert retail_ram[(0x300000 + case * 0x100 + 0x6d) & 0xffffff] == expected.r[5] & 255
        cases.append({'case': case, 'steps': steps, 'full_state_and_ram_match': True})

    source = out / 'source/retail_byte_field_setter.c'
    report = {'routine_start': hex(ENTRY), 'routine_end_exclusive': hex(ENTRY + SIZE),
              'retail_span_sha256': hashlib.sha256(
                  binary[ENTRY - BASE:ENTRY - BASE + SIZE]).hexdigest(),
              'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
              'cases_passed': len(cases), 'cases': cases,
              'scope': 'Three retail instructions including the RTS delay-slot indexed byte store; 64 randomized full GPR/SR/PR/PC and 16 MiB RAM comparisons.',
              'limitations': ['Only this leaf field setter is recovered; its callers and containing systems remain incomplete.',
                              'The project subset SH-4 interpreter is not hardware or end-to-end game verification.']}
    (out / 'analysis/retail_byte_field_setter_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"{len(cases)} retail byte field-setter comparisons passed")


if __name__ == '__main__':
    main()
