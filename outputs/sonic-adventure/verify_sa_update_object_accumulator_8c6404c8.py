"""Compare the nullable object accumulator helper with retail execution."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ADDRESS, LENGTH = 0x8c6404c8, 20
BASE, RAM_SIZE = 0x8c010000, 16 * 1024 * 1024
FALLBACK_GLOBAL = 0x8c8e9f00


def main():
    project = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    original = bytearray(b'\xa5') * RAM_SIZE
    physical_base = BASE & 0x00ffffff
    original[physical_base:physical_base + len(binary)] = binary
    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/sa_update_object_accumulator_8c6404c8.dll'))
    function = library.sa_update_object_accumulator_8c6404c8
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None
    rng = random.Random(0x8c6404c8)
    cases = []

    for case in range(128):
        direct_object = 0x8c700000 + case * 0x100
        fallback_object = 0x8c800000 + case * 0x100
        object_arg = 0 if case % 2 == 0 else direct_object
        initial_field = rng.getrandbits(32)
        addend = rng.getrandbits(32)
        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.r[4] = object_arg
        expected.r[5] = addend
        expected.sr = rng.getrandbits(32)
        expected.pr = 0x8c00b000 + case * 4
        expected.pc = ADDRESS
        actual = State.from_buffer_copy(bytes(expected))
        retail_ram, actual_ram = bytearray(original), bytearray(original)
        for memory in (retail_ram, actual_ram):
            struct.pack_into('<I', memory, FALLBACK_GLOBAL & 0xffffff, fallback_object)
            struct.pack_into('<I', memory, (direct_object + 0x74) & 0xffffff, initial_field)
            struct.pack_into('<I', memory, (fallback_object + 0x74) & 0xffffff, initial_field)
            struct.pack_into('<I', memory, (direct_object + 0x58) & 0xffffff, rng.getrandbits(32))
            struct.pack_into('<I', memory, (fallback_object + 0x58) & 0xffffff, rng.getrandbits(32))
        # Give retail and native fixtures identical initial destination words.
        for address in (direct_object + 0x58, fallback_object + 0x58):
            initial = struct.unpack_from('<I', retail_ram, address & 0xffffff)[0]
            struct.pack_into('<I', actual_ram, address & 0xffffff, initial)
        steps = reference(retail_ram, expected)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        function(ram_view, ctypes.byref(actual))
        assert steps == (10 if object_arg == 0 else 8), (case, steps)
        assert actual_ram == retail_ram, f'RAM mismatch at case {case}'
        assert bytes(actual) == bytes(expected), f'CPU mismatch at case {case}'
        cases.append({'case': case, 'object_argument': hex(object_arg),
                      'effective_object': hex(fallback_object if object_arg == 0 else direct_object),
                      'full_state_and_ram_match': True})

    source = project / 'source/sa_update_object_accumulator_8c6404c8.c'
    report = {
        'module': '1ST_READ.BIN', 'address': hex(ADDRESS),
        'end_exclusive': hex(ADDRESS + LENGTH), 'name': 'sa_update_object_accumulator_8c6404c8',
        'retail_span_sha256': hashlib.sha256(binary[ADDRESS - BASE:ADDRESS - BASE + LENGTH]).hexdigest(),
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cases_passed': len(cases), 'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state.',
        'scope': 'Null fallback and direct-object branches verified; updates the word at object +0x58 from the word at +0x74 plus R5.',
        'limitations': ['Verification uses the project SH-4 interpreter; wider system integration and hardware execution are not covered.'],
        'cases': cases,
    }
    (project / 'analysis/sa_update_object_accumulator_8c6404c8_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"Nullable object accumulator passed {len(cases)} full RAM/CPU comparisons")


if __name__ == '__main__':
    main()
