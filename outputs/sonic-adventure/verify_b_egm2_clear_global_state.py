"""Differentially verify B_EGM2's delay-slot global-state clear."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

OVERLAY_BASE, ADDRESS, END = 0x0c900000, 0x0c906b80, 0x0c906b8c
MAIN_BASE = 0x8c010000
RAM_BASE, RAM_SIZE = 0x0c000000, 16 * 1024 * 1024
STACK_GUARD, CASE_COUNT = 0x2000, 128


def offset(address):
    return (address & 0x1fffffff) - RAM_BASE


def main():
    out = ROOT / 'outputs/sonic-adventure'
    overlay = (ROOT / 'work/sonic-adventure/decoded/SONICADV/B_EGM2.BIN').read_bytes()
    main_binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    span_offset = ADDRESS - OVERLAY_BASE
    assert len(overlay[span_offset:span_offset + END - ADDRESS]) == END - ADDRESS
    word_address = int.from_bytes(overlay[0x6c60:0x6c64], 'little')
    byte_address = int.from_bytes(overlay[0x6c64:0x6c68], 'little')
    assert word_address == 0x0c9926f8 and byte_address == 0x0c9926f4

    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/b_egm2_clear_global_state.dll'))
    function = library.sa_b_egm2_clear_global_state_0c906b80
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None

    original = bytearray(b'\xa5') * RAM_SIZE
    for address, binary in ((MAIN_BASE, main_binary), (OVERLAY_BASE, overlay)):
        start = offset(address)
        original[start:start + len(binary)] = binary

    rng = random.Random(0x4245474d3206b80)
    cases = []
    for case in range(CASE_COUNT):
        baseline = bytearray(original)
        baseline[offset(word_address):offset(word_address) + 4] = rng.getrandbits(32).to_bytes(4, 'little')
        baseline[offset(byte_address)] = rng.randrange(256)
        retail_ram, native_ram = bytearray(baseline), bytearray(baseline)

        initial = State()
        initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
        initial.r[15] = 0x0cff0000
        initial.sr = rng.getrandbits(32)
        initial.pr = 0x0c006000 + case * 4
        initial.pc = ADDRESS
        expected = State.from_buffer_copy(bytes(initial))
        actual = State.from_buffer_copy(bytes(initial))

        steps = reference(retail_ram, expected)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
        function(ram_view, ctypes.byref(actual))

        stack_start = initial.r[15] - RAM_BASE - STACK_GUARD
        stack_end = initial.r[15] - RAM_BASE + STACK_GUARD
        assert retail_ram[:stack_start] == native_ram[:stack_start], f'RAM before caller stack differs in case {case}'
        assert retail_ram[stack_end:] == native_ram[stack_end:], f'RAM after caller stack differs in case {case}'
        assert list(actual.r) == list(expected.r), f'GPR state differs in case {case}'
        assert actual.sr == expected.sr, f'SR differs in case {case}'
        assert actual.pr == expected.pr and actual.pc == expected.pc, f'call boundary differs in case {case}'
        cases.append({'case': case, 'ram_and_full_cpu_state_match': True,
                      'retail_instructions': steps})

    source_path = out / 'source/b_egm2_clear_global_state.c'
    report = {
        'functions_verified': 1, 'total_abi_cases': len(cases),
        'source_sha256': hashlib.sha256(source_path.read_bytes()).hexdigest(),
        'comparison_level': 'All RAM except caller-stack guard; all 16 GPRs, SR, PR and return PC compared.',
        'scope': 'Loads B_EGM2 literal targets 0x0c9926f8 and 0x0c9926f4, clears one 32-bit global and one byte global, and preserves the RTS-delay-slot byte clear.',
        'limitations': ['Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or gameplay.',
                        'Destination globals are seeded in cloned RAM; retail overlay and instruction bytes remain unchanged.'],
        'dependencies': [
            {'module': 'B_EGM2.BIN', 'load_address': hex(OVERLAY_BASE),
             'sha256': hashlib.sha256(overlay).hexdigest(),
             'word_pointer_literal': hex(0x0c906c60), 'word_target': hex(word_address),
             'byte_pointer_literal': hex(0x0c906c64), 'byte_target': hex(byte_address)},
            {'module': '1ST_READ.BIN', 'load_address': hex(MAIN_BASE),
             'sha256': hashlib.sha256(main_binary).hexdigest()},
        ],
        'routines': [{
            'module': 'B_EGM2.BIN', 'load_address': hex(OVERLAY_BASE),
            'address': hex(ADDRESS), 'end_exclusive': hex(END),
            'name': 'clear_global_state',
            'retail_span_sha256': hashlib.sha256(overlay[span_offset:span_offset + END - ADDRESS]).hexdigest(),
            'cases_passed': len(cases), 'cases': cases,
        }],
    }
    report_path = out / 'analysis/common-overlays/B_EGM2.BIN/clear_global_state_verification.json'
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'B_EGM2 global-state clear passed {len(cases)} RAM and full CPU comparisons')


if __name__ == '__main__':
    main()
