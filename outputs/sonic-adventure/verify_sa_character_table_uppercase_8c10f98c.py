"""Compare the table-driven character helper against retail SH-4 execution."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

ADDRESS, LENGTH = 0x8c10f98c, 24
BASE, RAM_SIZE = 0x8c010000, 16 * 1024 * 1024


def main():
    project = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    original = bytearray(b'\xa5') * RAM_SIZE
    physical_base = BASE & 0x00ffffff
    original[physical_base:physical_base + len(binary)] = binary
    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/sa_character_table_uppercase_8c10f98c.dll'))
    function = library.sa_character_table_uppercase_8c10f98c
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None
    rng = random.Random(0x8c10f98c)
    cases, changed, unchanged = [], 0, 0

    for low_byte in range(256):
        value = (rng.getrandbits(24) << 8) | low_byte
        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.r[4] = value
        expected.sr = rng.getrandbits(32)
        expected.pr = 0x8c005000 + low_byte * 4
        expected.pc = ADDRESS
        actual = State.from_buffer_copy(bytes(expected))
        retail_ram, actual_ram = bytearray(original), bytearray(original)
        steps = reference(retail_ram, expected)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        function(ram_view, ctypes.byref(actual))
        assert steps == 9, (low_byte, steps)
        assert actual_ram == retail_ram, f'RAM mismatch for low byte {low_byte:#x}'
        assert bytes(actual) == bytes(expected), (
            f'CPU mismatch for input={value:#x}, r0={actual.r[0]:#x}/{expected.r[0]:#x}, '
            f'r1={actual.r[1]:#x}/{expected.r[1]:#x}, sr={actual.sr:#x}/{expected.sr:#x}')
        if expected.r[0] == (value - 0x20) & 0xffffffff:
            changed += 1
        else:
            unchanged += 1
        cases.append({'low_byte': low_byte, 'input': hex(value), 'result': hex(expected.r[0]),
                      'full_state_and_ram_match': True})

    source = project / 'source/sa_character_table_uppercase_8c10f98c.c'
    report = {
        'module': '1ST_READ.BIN', 'address': hex(ADDRESS),
        'end_exclusive': hex(ADDRESS + LENGTH), 'name': 'sa_character_table_uppercase_8c10f98c',
        'retail_span_sha256': hashlib.sha256(binary[ADDRESS - BASE:ADDRESS - BASE + LENGTH]).hexdigest(),
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cases_passed': len(cases), 'changed_cases': changed, 'unchanged_cases': unchanged,
        'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state.',
        'scope': 'All 256 low-byte table indexes verified, including signed-byte loads, T-bit results and the RTS delay slot.',
        'limitations': ['Verification uses the project SH-4 interpreter; system callers and hardware execution are not covered.'],
        'cases': cases,
    }
    (project / 'analysis/sa_character_table_uppercase_8c10f98c_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"Character table helper passed {len(cases)} full RAM/CPU comparisons ({changed} adjusted, {unchanged} unchanged)")


if __name__ == '__main__':
    main()
