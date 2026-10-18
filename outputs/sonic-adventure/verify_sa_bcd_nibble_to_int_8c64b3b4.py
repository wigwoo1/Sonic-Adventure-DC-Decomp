"""Compare the packed-decimal nibble conversion with retail SH-4 execution."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

ADDRESS, LENGTH = 0x8c64b3b4, 24
BASE, RAM_SIZE = 0x8c010000, 16 * 1024 * 1024


def main():
    project = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    original = bytearray(b'\xa5') * RAM_SIZE
    physical_base = BASE & 0x00ffffff
    original[physical_base:physical_base + len(binary)] = binary
    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/sa_bcd_nibble_to_int_8c64b3b4.dll'))
    function = library.sa_bcd_nibble_to_int_8c64b3b4
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None
    rng = random.Random(0x8c64b3b4)
    values = [0, 1, 9, 10, 0x1234, 0x9999, 0xffff, 0x7fffffff,
              0x80000000, 0xffffffff, 0x89abcdef, 0x01010101]
    cases = []

    for case in range(128):
        value = values[case] if case < len(values) else rng.getrandbits(32)
        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.r[4] = value
        expected.sr = rng.getrandbits(32)
        expected.pr = 0x8c003000 + case * 4
        expected.pc = ADDRESS
        actual = State.from_buffer_copy(bytes(expected))
        retail_ram, actual_ram = bytearray(original), bytearray(original)
        steps = reference(retail_ram, expected)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        function(ram_view, ctypes.byref(actual))
        assert steps == 12, (case, steps)
        assert actual_ram == retail_ram, f'RAM mismatch at case {case}'
        assert bytes(actual) == bytes(expected), (
            f'CPU mismatch at case {case}, input={value:#x}, '
            f'r0={actual.r[0]:#x}/{expected.r[0]:#x}, '
            f'r2={actual.r[2]:#x}/{expected.r[2]:#x}, '
            f'r3={actual.r[3]:#x}/{expected.r[3]:#x}, '
            f'r4={actual.r[4]:#x}/{expected.r[4]:#x}, '
            f'r5={actual.r[5]:#x}/{expected.r[5]:#x}, '
            f'sr={actual.sr:#x}/{expected.sr:#x}')
        cases.append({'case': case, 'input': hex(value),
                      'result': hex(expected.r[0]), 'full_state_and_ram_match': True})

    source = project / 'source/sa_bcd_nibble_to_int_8c64b3b4.c'
    report = {
        'module': '1ST_READ.BIN', 'address': hex(ADDRESS),
        'end_exclusive': hex(ADDRESS + LENGTH), 'name': 'sa_bcd_nibble_to_int_8c64b3b4',
        'retail_span_sha256': hashlib.sha256(binary[ADDRESS - BASE:ADDRESS - BASE + LENGTH]).hexdigest(),
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cases_passed': len(cases), 'retail_instruction_count': 12,
        'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state.',
        'scope': 'All retail arithmetic and register effects verified, including the addition in the RTS delay slot.',
        'limitations': ['Verification uses the project SH-4 interpreter; callers and hardware execution are not covered.'],
        'cases': cases,
    }
    (project / 'analysis/sa_bcd_nibble_to_int_8c64b3b4_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"Packed decimal conversion passed {len(cases)} full RAM/CPU comparisons")


if __name__ == '__main__':
    main()
