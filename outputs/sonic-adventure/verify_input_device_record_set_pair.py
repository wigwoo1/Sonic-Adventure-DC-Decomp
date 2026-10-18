"""Differentially verify the adjacent-field input-device setter."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ADDRESS, END, LOAD_BASE = 0x8c65f4ca, 0x8c65f4da, 0x8c010000
POINTER_LITERAL, OFFSET_LITERAL = 0x8c65f530, 0x8c65f522
POINTER_CELL, RECORD_BASE, RAM_SIZE, CASES = 0x8c8a83b4, 0x0c700000, 16 * 1024 * 1024, 128


def offset(address):
    return (address & 0x1fffffff) - 0x0c000000


def main():
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    body = binary[ADDRESS - LOAD_BASE:END - LOAD_BASE]
    assert len(body) == 16
    pointer_address = struct.unpack_from('<I', binary, POINTER_LITERAL - LOAD_BASE)[0]
    field_offset = struct.unpack_from('<H', binary, OFFSET_LITERAL - LOAD_BASE)[0]
    assert pointer_address == POINTER_CELL and field_offset == 0x10e0

    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/input_device_record_set_pair.dll'))
    function = library.sa_input_device_record_set_pair_8c65f4ca
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None
    base = bytearray([0xA5]) * RAM_SIZE
    binary_at = offset(LOAD_BASE)
    base[binary_at:binary_at + len(binary)] = binary
    rng = random.Random(0x8c65f4ca)
    cases = []

    for case in range(CASES):
        ram = bytearray(base)
        index_value = RECORD_BASE + case * 0x10000
        struct.pack_into('<I', ram, offset(POINTER_CELL), index_value)
        retail_ram, native_ram = bytearray(ram), bytearray(ram)
        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.r[15] = 0x0cff0000
        expected.r[4] = rng.getrandbits(32)
        expected.sr = rng.getrandbits(32)
        expected.pr = 0x0c008000 + case * 4
        expected.pc = ADDRESS
        actual = State.from_buffer_copy(bytes(expected))

        steps = reference(retail_ram, expected)
        view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
        function(view, ctypes.byref(actual))
        assert steps == 8
        assert native_ram == retail_ram, f'RAM differs in case {case}'
        assert bytes(actual) == bytes(expected), f'CPU differs in case {case}'
        first = struct.unpack_from('<I', retail_ram, offset(index_value + field_offset))[0]
        second = struct.unpack_from('<I', retail_ram, offset(index_value + field_offset + 4))[0]
        assert first == second == expected.r[4]
        cases.append({'case': case, 'record_base': hex(index_value),
                      'stored_value': hex(expected.r[4]), 'retail_instructions': steps,
                      'full_ram_and_cpu_match': True})

    report = {
        'cases_passed': len(cases),
        'comparison_level': 'Complete 16 MiB RAM and all GPRs, SR, PR, and return PC compared.',
        'scope': 'The 16-byte retail setter at 0x8c65f4ca stores R4 at record offsets 0x10e0 and 0x10e4, including both RTS-delay-slot stores.',
        'limitations': ['The structure fields are identified by literal offsets; wider ownership semantics remain under analysis.',
                        'Uses the project SH-4 interpreter, not Dreamcast hardware or end-to-end gameplay.'],
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'retail_span_sha256': hashlib.sha256(body).hexdigest(),
        'source_sha256': hashlib.sha256((ROOT / 'outputs/sonic-adventure/source/sa_input_device_record_set_pair.c').read_bytes()).hexdigest(),
        'pointer_literal': hex(pointer_address), 'offset_literal': hex(field_offset),
        'routines': [{'module': '1ST_READ.BIN', 'load_address': hex(LOAD_BASE),
                      'address': hex(ADDRESS), 'end_exclusive': hex(END),
                      'retail_span_sha256': hashlib.sha256(body).hexdigest(),
                      'cases_passed': len(cases)}],
        'cases': cases,
    }
    path = ROOT / 'outputs/sonic-adventure/analysis/input_device_record_set_pair_verification.json'
    path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'Input-device adjacent-field setter passed {len(cases)} full RAM/CPU comparisons')


if __name__ == '__main__':
    main()
