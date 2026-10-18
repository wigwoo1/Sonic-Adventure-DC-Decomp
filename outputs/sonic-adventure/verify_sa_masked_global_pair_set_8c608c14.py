"""Compare the paired global update with retail SH-4 execution."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ADDRESS, LENGTH = 0x8c608c14, 22
BASE, RAM_SIZE = 0x8c010000, 16 * 1024 * 1024
LITERAL_ONE, LITERAL_TWO = 0x8c608ce4, 0x8c608ce8


def main():
    project = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    original = bytearray(b'\xa5') * RAM_SIZE
    physical_base = BASE & 0x00ffffff
    original[physical_base:physical_base + len(binary)] = binary
    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/sa_masked_global_pair_set_8c608c14.dll'))
    function = library.sa_masked_global_pair_set_8c608c14
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None
    rng = random.Random(0x8c608c14)
    values = [0, 1, 0x7ff, 0x800, 0x12345678, 0xffffffff,
              0x80000000, 0x7fffffff, 0xabcdef01]
    cases = []

    for case in range(128):
        mode = case % 5
        if mode == 0:
            target_one, target_two = 0x8c700000 + case * 0x100, 0x8c710000 + case * 0x100
        elif mode == 1:
            target_one, target_two = LITERAL_ONE, LITERAL_TWO
        elif mode == 2:
            target_one, target_two = LITERAL_TWO, LITERAL_ONE
        elif mode == 3:
            target_one = target_two = 0x8c720000 + case * 0x100
        else:
            target_one, target_two = 0x8cfffffc, 0x8cfffff8
        value_one = rng.getrandbits(32)
        value_two = values[case] if case < len(values) else rng.getrandbits(32)
        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.r[4], expected.r[5] = value_one, value_two
        expected.sr = rng.getrandbits(32)
        expected.pr = 0x8c009000 + case * 4
        expected.pc = ADDRESS
        actual = State.from_buffer_copy(bytes(expected))
        retail_ram, actual_ram = bytearray(original), bytearray(original)
        for memory in (retail_ram, actual_ram):
            struct.pack_into('<I', memory, LITERAL_ONE & 0xffffff, target_one)
            struct.pack_into('<I', memory, LITERAL_TWO & 0xffffff, target_two)
        steps = reference(retail_ram, expected)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        function(ram_view, ctypes.byref(actual))
        assert steps == 11, (case, steps)
        assert actual_ram == retail_ram, f'RAM mismatch at case {case}'
        assert bytes(actual) == bytes(expected), (
            f'CPU mismatch at case {case}, R4={value_one:#x}, R5={value_two:#x}, '
            f'sr={actual.sr:#x}/{expected.sr:#x}')
        cases.append({'case': case, 'target_one': hex(target_one), 'target_two': hex(target_two),
                      'input_one': hex(value_one), 'input_two': hex(value_two),
                      'full_state_and_ram_match': True})

    source = project / 'source/sa_masked_global_pair_set_8c608c14.c'
    report = {
        'module': '1ST_READ.BIN', 'address': hex(ADDRESS),
        'end_exclusive': hex(ADDRESS + LENGTH), 'name': 'sa_masked_global_pair_set_8c608c14',
        'retail_span_sha256': hashlib.sha256(binary[ADDRESS - BASE:ADDRESS - BASE + LENGTH]).hexdigest(),
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cases_passed': len(cases), 'retail_instruction_count': 11,
        'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state.',
        'scope': 'Both pointer loads, first store, R5 bitmask, final T-bit update and RTS delay-slot store verified.',
        'limitations': ['Verification uses the project SH-4 interpreter; broader system behavior and hardware execution are not covered.'],
        'cases': cases,
    }
    (project / 'analysis/sa_masked_global_pair_set_8c608c14_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"Masked global pair setter passed {len(cases)} full RAM/CPU comparisons")


if __name__ == '__main__':
    main()
