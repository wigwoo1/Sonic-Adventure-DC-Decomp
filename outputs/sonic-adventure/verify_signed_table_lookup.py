"""Compare a signed-byte table lookup with the corresponding retail SH-4 leaf."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

BASE, RAM_SIZE = 0x8c010000, 16 * 1024 * 1024
ADDRESS, END, LITERAL = 0x8c049a5a, 0x8c049a60, 0x8c049ad8
TABLE_BASE = 0x8c799b88
INDICES = [0, 1, 2, 3, 7, 15, 31, 63, 127, 128, 129, 255,
           0xfffffffe, 0xffffffff] + [None] * 50


def main():
    out = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    lib = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/signed_table_lookup.dll'))
    function = lib.sa_signed_table_byte_8c049a5a
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None
    original = bytearray(b'\xa5') * RAM_SIZE
    original[0x10000:0x10000 + len(binary)] = binary
    off = ADDRESS - BASE
    assert binary[off:off + END - ADDRESS] == bytes.fromhex('1f d0 0b 00 4c 00')
    assert struct.unpack_from('<I', original, LITERAL & 0xffffff)[0] == TABLE_BASE
    rng = random.Random(0x5349474e45444259)
    cases = []

    for case, chosen_index in enumerate(INDICES):
        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        # The table's RAM allocation is finite; exercise a broad valid range,
        # plus wrapped -1/-2 indices that remain inside the mapped RAM window.
        index = chosen_index if chosen_index is not None else rng.getrandbits(20)
        expected.r[4] = index
        expected.sr = rng.getrandbits(32)
        expected.pr = 0x8c002c00 + case * 4
        expected.pc = ADDRESS
        actual = State.from_buffer_copy(bytes(expected))
        retail_ram, actual_ram = bytearray(original), bytearray(original)
        target = (TABLE_BASE + index) & 0x00ffffff
        sample = rng.randrange(256)
        retail_ram[target] = sample
        actual_ram[target] = sample
        steps = reference(retail_ram, expected)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        function(ram_view, ctypes.byref(actual))
        assert steps == 3
        assert actual_ram == retail_ram, f'RAM mismatch at case {case}'
        assert bytes(actual) == bytes(expected), f'CPU-state mismatch at case {case}'
        cases.append({'case': case, 'index': hex(index), 'sample_byte': sample,
                      'retail_instructions': steps, 'full_state_and_ram_match': True})

    source = out / 'source/signed_table_lookup.c'
    report = {
        'functions_verified': 1,
        'total_full_state_cases': len(cases),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'scope': 'Signed-byte table lookup at 0x8c049a5a; compared complete SH-4 CPU state and all 16 MiB RAM against retail execution using boundary and randomized indices and byte values.',
        'limitations': ['Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or end-to-end game execution.'],
        'routines': [{'module': '1ST_READ.BIN', 'address': hex(ADDRESS),
                      'end_exclusive': hex(END), 'name': 'signed_table_byte',
                      'retail_span_sha256': hashlib.sha256(binary[off:off + END - ADDRESS]).hexdigest(),
                      'cases_passed': len(cases), 'cases': cases}],
    }
    (out / 'analysis/signed_table_lookup_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"Signed table lookup passed {len(cases)} full-state comparisons")


if __name__ == '__main__':
    main()
