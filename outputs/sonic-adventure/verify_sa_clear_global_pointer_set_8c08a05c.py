"""Compare the five-pointer global clear with retail SH-4 execution."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ADDRESS, LENGTH = 0x8c08a05c, 24
BASE, RAM_SIZE = 0x8c010000, 16 * 1024 * 1024
LITERALS = (0x8c08a0fc, 0x8c08a100, 0x8c08a104, 0x8c08a108, 0x8c08a10c)


def main():
    project = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    original = bytearray(b'\xa5') * RAM_SIZE
    physical_base = BASE & 0x00ffffff
    original[physical_base:physical_base + len(binary)] = binary
    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/sa_clear_global_pointer_set_8c08a05c.dll'))
    function = library.sa_clear_global_pointer_set_8c08a05c
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None
    rng = random.Random(0x8c08a05c)
    retail_values = [struct.unpack_from('<I', binary, literal - BASE)[0] for literal in LITERALS]
    cases = []

    for case in range(128):
        choices = [retail_values, list(LITERALS),
                   [0x8c700000 + case * 0x100 + i * 4 for i in range(5)],
                   [0x8cfffffc] * 5]
        targets = [choices[(case + i) % len(choices)][i] for i in range(5)]
        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.sr = rng.getrandbits(32)
        expected.pr = 0x8c002000 + case * 4
        expected.pc = ADDRESS
        actual = State.from_buffer_copy(bytes(expected))
        retail_ram, actual_ram = bytearray(original), bytearray(original)
        for literal, target in zip(LITERALS, targets):
            struct.pack_into('<I', retail_ram, literal & 0xffffff, target)
            struct.pack_into('<I', actual_ram, literal & 0xffffff, target)
        steps = reference(retail_ram, expected)
        view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        function(view, ctypes.byref(actual))
        assert steps == 12, (case, steps)
        assert actual_ram == retail_ram, f'RAM mismatch at case {case}'
        assert bytes(actual) == bytes(expected), f'CPU mismatch at case {case}'
        cases.append({'case': case, 'initial_pointer_targets': [hex(x) for x in targets],
                      'full_state_and_ram_match': True})

    source = project / 'source/sa_clear_global_pointer_set_8c08a05c.c'
    report = {
        'module': '1ST_READ.BIN', 'address': hex(ADDRESS),
        'end_exclusive': hex(ADDRESS + LENGTH), 'name': 'sa_clear_global_pointer_set_8c08a05c',
        'retail_span_sha256': hashlib.sha256(binary[ADDRESS - BASE:ADDRESS - BASE + LENGTH]).hexdigest(),
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cases_passed': len(cases), 'retail_instruction_count': 12,
        'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state.',
        'scope': 'All five pointer loads, global word clears, register effects and RTS delay-slot store verified.',
        'limitations': ['Verification uses the project SH-4 interpreter; system-level callers and Dreamcast hardware are not covered.'],
        'cases': cases,
    }
    (project / 'analysis/sa_clear_global_pointer_set_8c08a05c_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"Five-pointer global clear passed {len(cases)} full RAM/CPU comparisons")


if __name__ == '__main__':
    main()
