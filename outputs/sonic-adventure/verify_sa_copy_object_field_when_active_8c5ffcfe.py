"""Compare the conditional object-field helper against retail SH-4 execution."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ADDRESS, LENGTH = 0x8c5ffcfe, 22
RAM_SIZE, BASE = 16 * 1024 * 1024, 0x8c010000


def main():
    project = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    original = bytearray(b'\xa5') * RAM_SIZE
    physical_base = BASE & 0x00ffffff
    original[physical_base:physical_base + len(binary)] = binary
    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/sa_copy_object_field_when_active_8c5ffcfe.dll'))
    function = library.sa_copy_object_field_when_active_8c5ffcfe
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None
    rng = random.Random(0x8c5ffcfe)
    values = [0, 1, 2, 3, 0x7fffffff, 0x80000000, 0xffffffff]
    cases = []

    for case in range(128):
        object_address = 0 if case % 8 == 0 else 0x8c700000 + case * 0x100
        if case == 127:
            object_address = 0x8cffffc0
        count = values[case % len(values)] if case < 56 else rng.getrandbits(32)
        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.r[4] = object_address
        expected.sr = rng.getrandbits(32)
        expected.pr = 0x8c001000 + case * 4
        expected.pc = ADDRESS
        actual = State.from_buffer_copy(bytes(expected))
        retail_ram, actual_ram = bytearray(original), bytearray(original)
        if object_address:
            struct.pack_into('<I', retail_ram, object_address & 0xffffff, count)
            struct.pack_into('<I', actual_ram, object_address & 0xffffff, count)
            source_value = rng.getrandbits(32)
            struct.pack_into('<I', retail_ram, (object_address + 0x10) & 0xffffff, source_value)
            struct.pack_into('<I', actual_ram, (object_address + 0x10) & 0xffffff, source_value)
            destination = (object_address + 0x3c) & 0xffffff
            struct.pack_into('<I', retail_ram, destination, rng.getrandbits(32))
            struct.pack_into('<I', actual_ram, destination, struct.unpack_from('<I', retail_ram, destination)[0])
        steps = reference(retail_ram, expected)
        view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        function(view, ctypes.byref(actual))
        expected_steps = 4 if object_address == 0 else (11 if count < 0x80000000 and count >= 2 else 8)
        assert steps == expected_steps, (case, steps, expected_steps, hex(object_address), hex(count))
        assert actual_ram == retail_ram, f'RAM mismatch at case {case}'
        assert bytes(actual) == bytes(expected), f'CPU mismatch at case {case}'
        cases.append({'case': case, 'object': hex(object_address), 'count': hex(count),
                      'retail_steps': steps, 'full_state_and_ram_match': True})

    source = project / 'source/sa_copy_object_field_when_active_8c5ffcfe.c'
    report = {
        'module': '1ST_READ.BIN', 'address': hex(ADDRESS),
        'end_exclusive': hex(ADDRESS + LENGTH), 'name': 'sa_copy_object_field_when_active_8c5ffcfe',
        'retail_span_sha256': hashlib.sha256(binary[ADDRESS - BASE:ADDRESS - BASE + LENGTH]).hexdigest(),
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cases_passed': len(cases), 'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state.',
        'scope': 'All retail branches verified, including null, signed count threshold, field read and destination write.',
        'limitations': ['Verification uses the project SH-4 interpreter; runtime system callers and Dreamcast hardware are not covered.'],
        'cases': cases,
    }
    (project / 'analysis/sa_copy_object_field_when_active_8c5ffcfe_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"Conditional object helper passed {len(cases)} full RAM/CPU comparisons")


if __name__ == '__main__':
    main()
