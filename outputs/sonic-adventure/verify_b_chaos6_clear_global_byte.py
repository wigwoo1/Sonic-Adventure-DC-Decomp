"""Differentially verify B_CHAOS6's delay-slot global-word clear."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

OVERLAY_BASE, ADDRESS, END = 0x0c900000, 0x0c903bc6, 0x0c903bce
MAIN_BASE = 0x8c010000
RAM_BASE, RAM_SIZE = 0x0c000000, 16 * 1024 * 1024
STACK_GUARD, CASE_COUNT = 0x2000, 128


def offset(address):
    return (address & 0x1fffffff) - RAM_BASE


def main():
    out = ROOT / 'outputs/sonic-adventure'
    overlay = (ROOT / 'work/sonic-adventure/decoded/SONICADV/B_CHAOS6.BIN').read_bytes()
    main_binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    span_offset = ADDRESS - OVERLAY_BASE
    assert len(overlay[span_offset:span_offset + END - ADDRESS]) == END - ADDRESS
    destination = int.from_bytes(overlay[0x3c38:0x3c3c], 'little')
    assert destination == 0x0ca91a5c and 0 <= offset(destination) <= RAM_SIZE - 4

    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/b_chaos6_clear_global_byte.dll'))
    function = library.sa_b_chaos6_clear_global_byte_0c903bc6
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None

    original = bytearray(b'\xa5') * RAM_SIZE
    main_offset = offset(MAIN_BASE)
    original[main_offset:main_offset + len(main_binary)] = main_binary
    overlay_offset = offset(OVERLAY_BASE)
    original[overlay_offset:overlay_offset + len(overlay)] = overlay

    rng = random.Random(0x4348414f5336bc06)
    cases = []
    for case in range(CASE_COUNT):
        baseline = bytearray(original)
        initial_word = rng.getrandbits(32)
        struct.pack_into('<I', baseline, offset(destination), initial_word)
        retail_ram, native_ram = bytearray(baseline), bytearray(baseline)

        initial = State()
        initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
        initial.r[15] = 0x0cff0000
        initial.sr = rng.getrandbits(32)
        initial.pr = 0x0c004000 + case * 4
        initial.pc = ADDRESS
        expected, actual = State.from_buffer_copy(bytes(initial)), State.from_buffer_copy(bytes(initial))

        steps = reference(retail_ram, expected)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
        function(ram_view, ctypes.byref(actual))

        stack_start = initial.r[15] - RAM_BASE - STACK_GUARD
        stack_end = initial.r[15] - RAM_BASE + STACK_GUARD
        if retail_ram[:stack_start] != native_ram[:stack_start]:
            first = next(i for i, (a, b) in enumerate(zip(retail_ram[:stack_start], native_ram[:stack_start])) if a != b)
            rc = [(i + RAM_BASE, b, a) for i, (b, a) in enumerate(zip(baseline, retail_ram)) if a != b and i < stack_start][:16]
            nc = [(i + RAM_BASE, b, a) for i, (b, a) in enumerate(zip(baseline, native_ram)) if a != b and i < stack_start][:16]
            raise AssertionError(f'RAM differs case={case} first={first + RAM_BASE:#010x} retail={retail_ram[first]:02x} native={native_ram[first]:02x} destination={destination:#010x} retail_changes={rc} native_changes={nc} R1R2={expected.r[1]:#x},{expected.r[2]:#x}')
        assert retail_ram[stack_end:] == native_ram[stack_end:], f'RAM after stack differs in case {case}'
        assert list(actual.r) == list(expected.r), f'GPR state differs in case {case}'
        assert actual.sr == expected.sr, f'SR differs in case {case}'
        assert actual.pr == expected.pr and actual.pc == expected.pc, f'call boundary differs in case {case}'
        cases.append({'case': case, 'initial_word': hex(initial_word),
                      'retail_instructions': steps, 'ram_and_full_cpu_state_match': True})

    source = out / 'source/b_chaos6_clear_global_byte.c'
    report = {
        'functions_verified': 1, 'total_abi_cases': len(cases),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'comparison_level': 'All RAM except caller-stack guard; all 16 GPRs, SR, PR and return PC compared.',
        'scope': 'Loads the destination pointer from the B_CHAOS6 overlay literal, clears one byte, and reproduces R1/R2 effects including the RTS delay-slot store.',
        'limitations': ['Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or gameplay.',
                        'The destination byte and neighboring bytes are randomized in a cloned RAM image; retail overlay and instruction bytes are unchanged.'],
        'dependencies': [
            {'module': 'B_CHAOS6.BIN', 'load_address': hex(OVERLAY_BASE),
             'sha256': hashlib.sha256(overlay).hexdigest(),
             'destination_pointer_address': hex(0x0c903c38), 'destination_address': hex(destination)},
            {'module': '1ST_READ.BIN', 'load_address': hex(MAIN_BASE),
             'sha256': hashlib.sha256(main_binary).hexdigest()},
        ],
        'routines': [{
            'module': 'B_CHAOS6.BIN', 'load_address': hex(OVERLAY_BASE),
            'address': hex(ADDRESS), 'end_exclusive': hex(END),
            'name': 'clear_global_word',
            'retail_span_sha256': hashlib.sha256(overlay[span_offset:span_offset + END - ADDRESS]).hexdigest(),
            'cases_passed': len(cases), 'cases': cases,
        }],
    }
    report_path = out / 'analysis/common-overlays/B_CHAOS6.BIN/clear_global_byte_verification.json'
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'B_CHAOS6 delay-slot byte clear passed {len(cases)} RAM and full CPU comparisons')


if __name__ == '__main__':
    main()
