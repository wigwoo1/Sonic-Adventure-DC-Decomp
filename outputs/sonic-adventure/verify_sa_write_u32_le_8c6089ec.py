"""Compare the byte-wise little-endian writer with retail SH-4 execution."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

ADDRESS, LENGTH = 0x8c6089ec, 24
BASE, RAM_SIZE = 0x8c010000, 16 * 1024 * 1024


def main():
    project = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    original = bytearray(b'\xa5') * RAM_SIZE
    physical_base = BASE & 0x00ffffff
    original[physical_base:physical_base + len(binary)] = binary
    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/sa_write_u32_le_8c6089ec.dll'))
    function = library.sa_write_u32_le_8c6089ec
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None
    rng = random.Random(0x8c6089ec)
    values = [0, 0x01020304, 0x89abcdef, 0xffffffff,
              0x80000000, 0x7fffffff, 0x0000ffff, 0xffff0000]
    cases = []

    for case in range(128):
        value = values[case] if case < len(values) else rng.getrandbits(32)
        destination = 0x8c700000 + case * 0x100 if case != 127 else 0x8cfffffc
        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.r[4] = value
        expected.r[5] = destination
        expected.sr = rng.getrandbits(32)
        expected.pr = 0x8c007000 + case * 4
        expected.pc = ADDRESS
        actual = State.from_buffer_copy(bytes(expected))
        retail_ram, actual_ram = bytearray(original), bytearray(original)
        steps = reference(retail_ram, expected)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        function(ram_view, ctypes.byref(actual))
        assert steps == 12, (case, steps)
        assert actual_ram == retail_ram, f'RAM mismatch at case {case}, input={value:#x}'
        assert bytes(actual) == bytes(expected), f'CPU mismatch at case {case}, input={value:#x}'
        cases.append({'case': case, 'input': hex(value), 'destination': hex(destination),
                      'full_state_and_ram_match': True})

    source = project / 'source/sa_write_u32_le_8c6089ec.c'
    report = {
        'module': '1ST_READ.BIN', 'address': hex(ADDRESS),
        'end_exclusive': hex(ADDRESS + LENGTH), 'name': 'sa_write_u32_le_8c6089ec',
        'retail_span_sha256': hashlib.sha256(binary[ADDRESS - BASE:ADDRESS - BASE + LENGTH]).hexdigest(),
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cases_passed': len(cases), 'retail_instruction_count': 12,
        'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state.',
        'scope': 'All four byte writes, intermediate register effects and the final RTS delay-slot byte write verified.',
        'limitations': ['Verification uses the project SH-4 interpreter; callers and Dreamcast hardware are not covered.'],
        'cases': cases,
    }
    (project / 'analysis/sa_write_u32_le_8c6089ec_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"Little-endian word writer passed {len(cases)} full RAM/CPU comparisons")


if __name__ == '__main__':
    main()
