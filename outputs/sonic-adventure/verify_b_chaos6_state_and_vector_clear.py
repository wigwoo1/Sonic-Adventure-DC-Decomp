"""Differentially verify B_CHAOS6's state update and FPU vector clear."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference
from verify_fpu_cleanup_transfer import Fpu

OVERLAY_BASE, ADDRESS, END = 0x0c900000, 0x0c9062ac, 0x0c9062c0
MAIN_BASE = 0x8c010000
RAM_BASE, RAM_SIZE = 0x0c000000, 16 * 1024 * 1024
STACK_GUARD, CASE_COUNT = 0x2000, 128
CONTEXT, WORK = 0x8c020000, 0x8c021000


def offset(address):
    return (address & 0x1fffffff) - RAM_BASE


def main():
    out = ROOT / 'outputs/sonic-adventure'
    overlay = (ROOT / 'work/sonic-adventure/decoded/SONICADV/B_CHAOS6.BIN').read_bytes()
    main_binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    span_offset = ADDRESS - OVERLAY_BASE
    assert len(overlay[span_offset:span_offset + END - ADDRESS]) == END - ADDRESS

    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/b_chaos6_state_and_vector_clear.dll'))
    function = library.sa_b_chaos6_state_and_vector_clear_0c9062ac
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(Fpu), ctypes.POINTER(State)]
    function.restype = None

    original = bytearray(b'\xa5') * RAM_SIZE
    for address, binary in ((MAIN_BASE, main_binary), (OVERLAY_BASE, overlay)):
        start = offset(address)
        original[start:start + len(binary)] = binary

    rng = random.Random(0x4348414f533662ac)
    cases = []
    for case in range(CASE_COUNT):
        baseline = bytearray(original)
        current = rng.getrandbits(32)
        parameter = current if case % 4 == 0 else rng.getrandbits(32)
        if case % 4 != 0 and parameter == current:
            parameter ^= 1
        struct.pack_into('<I', baseline, offset(CONTEXT) + 0x20, WORK)
        struct.pack_into('<I', baseline, offset(WORK) + 8, current)
        retail_ram, native_ram = bytearray(baseline), bytearray(baseline)

        initial = State()
        initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
        initial.r[4] = CONTEXT
        initial.r[5] = parameter
        initial.r[15] = 0x0cff0000
        initial.sr = rng.getrandbits(32)
        initial.pr = 0x0c004000 + case * 4
        initial.pc = ADDRESS
        expected = State.from_buffer_copy(bytes(initial))
        actual = State.from_buffer_copy(bytes(initial))

        expected_fpu = Fpu()
        active = (case >> 1) & 1
        width = (case >> 2) & 1
        expected_fpu.fpscr = 0x00040000 | (active << 21) | (width << 20)  # PR=0; cover both FR banks and SZ modes.
        for bank in range(2):
            expected_fpu.banks[bank][:] = [rng.getrandbits(32) for _ in range(16)]
        actual_fpu = Fpu.from_buffer_copy(bytes(expected_fpu))

        steps = reference(retail_ram, expected, fpu=expected_fpu)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
        function(ram_view, ctypes.byref(actual_fpu), ctypes.byref(actual))

        stack_start = initial.r[15] - RAM_BASE - STACK_GUARD
        stack_end = initial.r[15] - RAM_BASE + STACK_GUARD
        assert retail_ram[:stack_start] == native_ram[:stack_start], f'RAM before caller stack differs in case {case}'
        assert retail_ram[stack_end:] == native_ram[stack_end:], f'RAM after caller stack differs in case {case}'
        assert list(actual.r) == list(expected.r), f'GPR state differs in case {case}'
        assert actual.sr == expected.sr, f'SR differs in case {case}'
        assert actual.pr == expected.pr and actual.pc == expected.pc, f'call boundary differs in case {case}'
        assert bytes(actual_fpu) == bytes(expected_fpu), f'FPU state differs in case {case}'
        cases.append({'case': case, 'current_state': hex(current), 'parameter': hex(parameter),
                      'fpscr_active_bank': active, 'fpscr_sz': width,
                      'retail_instructions': steps, 'ram_cpu_fpu_match': True})

    source_path = out / 'source/b_chaos6_state_and_vector_clear.c'
    header_path = out / 'source/fpu_cleanup_transfer.h'
    report = {
        'functions_verified': 1, 'total_abi_cases': len(cases),
        'source_sha256': hashlib.sha256(source_path.read_bytes()).hexdigest(),
        'fpu_header_sha256': hashlib.sha256(header_path.read_bytes()).hexdigest(),
        'comparison_level': 'All RAM except caller-stack guard; all 16 GPRs, SR, PR, return PC and complete modeled FPU state compared.',
        'scope': 'When the state at work+8 differs from R5, stores R5 there, sets active-bank FR3 to zero, and performs the retail SZ-dependent FMOV at work+0x0c; if equal, returns without changing the FPU or state vector.',
        'limitations': ['Verified with the project partial SH-4 interpreter for FPSCR.PR=0, both SZ modes and both FR banks; PR=1 is an illegal FLDI0 mode and is not covered.',
                        'FPU behavior is model-to-model differential verification, not Dreamcast hardware validation.',
                        'Context and work object are test-initialized in cloned RAM.'],
        'dependencies': [
            {'module': 'B_CHAOS6.BIN', 'load_address': hex(OVERLAY_BASE),
             'sha256': hashlib.sha256(overlay).hexdigest()},
            {'module': '1ST_READ.BIN', 'load_address': hex(MAIN_BASE),
             'sha256': hashlib.sha256(main_binary).hexdigest()},
            {'source': 'source/fpu_cleanup_transfer.h', 'sha256': hashlib.sha256(header_path.read_bytes()).hexdigest()},
        ],
        'routines': [{
            'module': 'B_CHAOS6.BIN', 'load_address': hex(OVERLAY_BASE),
            'address': hex(ADDRESS), 'end_exclusive': hex(END),
            'name': 'state_and_vector_clear',
            'retail_span_sha256': hashlib.sha256(overlay[span_offset:span_offset + END - ADDRESS]).hexdigest(),
            'cases_passed': len(cases), 'cases': cases,
        }],
    }
    report_path = out / 'analysis/common-overlays/B_CHAOS6.BIN/state_and_vector_clear_verification.json'
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'B_CHAOS6 state/vector-clear routine passed {len(cases)} RAM/CPU/FPU comparisons')


if __name__ == '__main__':
    main()
