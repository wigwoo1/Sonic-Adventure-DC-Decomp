"""Compare an object-field initializer against retail SH-4 execution."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

ADDRESS, LENGTH = 0x8c0e65be, 18
RAM_SIZE, BASE = 16 * 1024 * 1024, 0x8c010000


def main():
    project = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram_base = BASE & 0x00ffffff
    original = bytearray(b'\xa5') * RAM_SIZE
    original[ram_base:ram_base + len(binary)] = binary
    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/sa_initialize_object_flags_8c0e65be.dll'))
    function = library.sa_initialize_object_flags_8c0e65be
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None
    rng = random.Random(0x8c0e65be)
    cases = []

    for case in range(128):
        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.r[4] = 0x8c700000 + case * 0x80
        expected.r[6] = 0x8c800000 + case * 0x400 if case != 127 else 0x8cfffeda
        expected.sr = rng.getrandbits(32)
        expected.pr = 0x8c000c00 + case * 4
        expected.pc = ADDRESS
        actual = State.from_buffer_copy(bytes(expected))
        retail_ram, actual_ram = bytearray(original), bytearray(original)
        steps = reference(retail_ram, expected)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        function(ram_view, ctypes.byref(actual))
        assert steps == 9, (hex(ADDRESS), steps)
        assert actual_ram == retail_ram, f'RAM mismatch at case {case}'
        assert bytes(actual) == bytes(expected), f'CPU mismatch at case {case}'
        cases.append({'case': case, 'full_state_and_ram_match': True,
                      'final_word_address': hex(expected.r[6] + 0x124)})

    source = project / 'source/sa_initialize_object_flags_8c0e65be.c'
    report = {
        'module': '1ST_READ.BIN', 'address': hex(ADDRESS),
        'end_exclusive': hex(ADDRESS + LENGTH), 'name': 'sa_initialize_object_flags_8c0e65be',
        'retail_span_sha256': hashlib.sha256(binary[ADDRESS - BASE:ADDRESS - BASE + LENGTH]).hexdigest(),
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cases_passed': len(cases), 'retail_instruction_count': 9,
        'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state.',
        'scope': 'Recovered all nine retail instructions, including the RTS delay-slot halfword clear.',
        'limitations': ['Verification uses the project SH-4 interpreter and is not Dreamcast hardware or end-to-end game verification.'],
        'cases': cases,
    }
    (project / 'analysis/sa_initialize_object_flags_8c0e65be_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"Object initializer passed {len(cases)} full RAM/CPU comparisons")


if __name__ == '__main__':
    main()
