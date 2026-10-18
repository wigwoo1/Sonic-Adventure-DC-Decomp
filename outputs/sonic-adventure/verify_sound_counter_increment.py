"""Differentially verify the sound-driver counter increment against retail SH-4."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ADDRESS, END, LITERAL = 0x8c65c01c, 0x8c65c026, 0x8c65c0f4
LOAD_BASE, COUNTER, RAM_SIZE = 0x8c010000, 0x8c67c1f4, 16 * 1024 * 1024
CASE_COUNT = 128


def offset(address):
    return (address & 0x1fffffff) - 0x0c000000


def main():
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    body = binary[ADDRESS - LOAD_BASE:END - LOAD_BASE]
    literal_value = struct.unpack_from('<I', binary, LITERAL - LOAD_BASE)[0]
    assert len(body) == 10 and literal_value == COUNTER
    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/sound_counter_increment.dll'))
    function = library.sa_sound_counter_increment_8c65c01c
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None

    baseline = bytearray([0xA5]) * RAM_SIZE
    binary_offset = offset(LOAD_BASE)
    baseline[binary_offset:binary_offset + len(binary)] = binary
    rng = random.Random(0x8c65c01c)
    cases = []
    for case in range(CASE_COUNT):
        initial_value = [0, 1, 0x7fffffff, 0xfffffffe, 0xffffffff][case % 5]
        if case >= 5:
            initial_value = rng.getrandbits(32)
        baseline_case = bytearray(baseline)
        struct.pack_into('<I', baseline_case, offset(COUNTER), initial_value)
        retail_ram, native_ram = bytearray(baseline_case), bytearray(baseline_case)

        initial = State()
        initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
        initial.r[15] = 0x0cff0000
        initial.sr = rng.getrandbits(32)
        initial.pr = 0x0c006000 + case * 4
        initial.pc = ADDRESS
        expected = State.from_buffer_copy(bytes(initial))
        actual = State.from_buffer_copy(bytes(initial))

        steps = reference(retail_ram, expected)
        view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
        function(view, ctypes.byref(actual))

        assert retail_ram == native_ram, f'RAM differs in case {case}'
        assert bytes(actual) == bytes(expected), f'CPU state differs in case {case}'
        assert struct.unpack_from('<I', native_ram, offset(COUNTER))[0] == (initial_value + 1) & 0xffffffff
        cases.append({'case': case, 'initial_counter': hex(initial_value),
                      'retail_instructions': steps, 'full_ram_and_cpu_match': True})

    report = {
        'cases_passed': len(cases),
        'comparison_level': 'Complete 16 MiB RAM and all GPRs, SR, PR, and return PC compared.',
        'scope': 'Exact five-instruction sound-driver counter increment at 0x8c65c01c; literal 0x8c65c0f4 resolves to 0x8c67c1f4. Counter wraparound and the RTS delay-slot store are included.',
        'limitations': ['Uses the project SH-4 interpreter, not Dreamcast hardware or end-to-end gameplay.'],
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'retail_span_sha256': hashlib.sha256(body).hexdigest(),
        'literal_sha256': hashlib.sha256(binary[LITERAL - LOAD_BASE:LITERAL - LOAD_BASE + 4]).hexdigest(),
        'source_sha256': hashlib.sha256((ROOT / 'outputs/sonic-adventure/source/sa_sound_counter_increment.c').read_bytes()).hexdigest(),
        'routines': [{'module': '1ST_READ.BIN', 'load_address': hex(LOAD_BASE),
                      'address': hex(ADDRESS), 'end_exclusive': hex(END),
                      'literal_address': hex(LITERAL), 'literal_value': hex(literal_value),
                      'retail_span_sha256': hashlib.sha256(body).hexdigest(),
                      'cases_passed': len(cases)}],
        'cases': cases,
    }
    path = ROOT / 'outputs/sonic-adventure/analysis/sound_counter_increment_verification.json'
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'Sound counter increment passed {len(cases)} full RAM/CPU comparisons')


if __name__ == '__main__':
    main()
