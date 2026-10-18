"""Verify B_CHAOS4 six-byte external tail thunks against SH-4 execution."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

OVERLAY_BASE = 0x0c900000
RAM_SIZE = 16 * 1024 * 1024
ROUTINES = [
    ('thunk_EXT_FUN_8c0606c6', 0x0c904654, 0x0c90465a, 0x0c90467c,
     0x8c0606c6, 'sa_b_chaos4_thunk_ext_fun_8c0606c6_0c904654'),
    ('thunk_EXT_FUN_8c107aaa', 0x0c907116, 0x0c90711c, 0x0c907154,
     0x8c107aaa, 'sa_b_chaos4_thunk_ext_fun_8c107aaa_0c907116'),
    ('thunk_function_8c60edf0', 0x0c90424c, 0x0c904254, 0x0c904358,
     None, 'sa_b_chaos4_thunk_0c90424c'),
]


def main():
    out = ROOT / 'outputs/sonic-adventure'
    overlay = (ROOT / 'work/sonic-adventure/decoded/SONICADV/B_CHAOS4.BIN').read_bytes()
    dll = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/b_chaos4_external_thunks.dll'))
    functions = {}
    for _, _, _, _, _, symbol in ROUTINES:
        function = getattr(dll, symbol)
        function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
        function.restype = None
        functions[symbol] = function

    rng = random.Random(0x424348345448554e)
    routines_report = []
    for name, address, end, literal, target, symbol in ROUTINES:
        literal_value = int.from_bytes(overlay[literal - OVERLAY_BASE:
                                               literal - OVERLAY_BASE + 4], 'little')
        if target is None:
            target = literal_value
        assert literal_value == target, f'{name}: literal target mismatch'
        cases = []
        for case in range(128):
            baseline = bytearray(b'\xa5') * RAM_SIZE
            baseline[OVERLAY_BASE - 0x0c000000:
                    OVERLAY_BASE - 0x0c000000 + len(overlay)] = overlay
            retail_ram, actual_ram = bytearray(baseline), bytearray(baseline)
            expected = State()
            expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
            expected.sr = rng.getrandbits(32)
            expected.pr = 0x0c001000 + case * 4
            expected.pc = address
            actual = State.from_buffer_copy(bytes(expected))

            steps = reference(retail_ram, expected, stop_pc=target)
            ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
            functions[symbol](ram_view, ctypes.byref(actual))
            assert retail_ram == actual_ram, f'{name}: RAM mismatch in case {case}'
            assert bytes(expected) == bytes(actual), f'{name}: CPU-state mismatch in case {case}'
            cases.append({'case': case, 'retail_instructions': steps,
                          'full_ram_and_cpu_state_match': True})

        offset = address - OVERLAY_BASE
        routines_report.append({
            'module': 'B_CHAOS4.BIN', 'load_address': hex(OVERLAY_BASE),
            'address': hex(address), 'end_exclusive': hex(end), 'name': name,
            'native_symbol': symbol,
            'external_target': hex(target), 'target_literal_address': hex(literal),
            'retail_span_sha256': hashlib.sha256(overlay[offset:offset + end - address]).hexdigest(),
            'cases_passed': len(cases), 'cases': cases,
        })

    source = out / 'source/b_chaos4_external_thunks.c'
    report = {
        'functions_verified': len(ROUTINES), 'total_abi_cases': 384,
        'comparison_level': 'All 16 MiB RAM and complete modeled CPU state compared byte-for-byte up to the exact external tail-jump target.',
        'scope': 'Two six-byte B_CHAOS4 tail thunks load their external function targets into R3 and jump through R3.',
        'limitations': ['The dispatch targets are outside these short thunk spans; this report verifies the register loads and handoff only, not behavior after entering any target.'],
        'dependencies': [{'module': 'B_CHAOS4.BIN', 'load_address': hex(OVERLAY_BASE),
                          'sha256': hashlib.sha256(overlay).hexdigest()}],
        'routines': routines_report,
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    }
    report_path = out / 'analysis/common-overlays/B_CHAOS4.BIN/external_thunks_verification.json'
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"B_CHAOS4 external thunks passed {report['total_abi_cases']} full-RAM and CPU-state comparisons")


if __name__ == '__main__':
    main()
