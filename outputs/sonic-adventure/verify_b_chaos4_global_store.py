"""Differentially verify the B_CHAOS4 global setter against retail SH-4 code."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

OVERLAY_BASE = 0x0c900000
ADDRESS, END = 0x0c901e86, 0x0c901e8c
LITERAL_ADDRESS, GLOBAL_ADDRESS = 0x0c901ef0, 0x0c95d974
RAM_SIZE = 16 * 1024 * 1024


def main():
    out = ROOT / 'outputs/sonic-adventure'
    overlay = (ROOT / 'work/sonic-adventure/decoded/SONICADV/B_CHAOS4.BIN').read_bytes()
    offset = ADDRESS - OVERLAY_BASE
    literal_offset = LITERAL_ADDRESS - OVERLAY_BASE
    assert len(overlay[offset:offset + END - ADDRESS]) == END - ADDRESS
    assert int.from_bytes(overlay[literal_offset:literal_offset + 4], 'little') == GLOBAL_ADDRESS
    global_offset = GLOBAL_ADDRESS - 0x0c000000
    assert 0 <= global_offset <= RAM_SIZE - 4

    dll_path = ROOT / 'work/sonic-adventure/build/b_chaos4_global_store.dll'
    lib = ctypes.CDLL(str(dll_path))
    function = lib.sa_b_chaos4_global_store_0c901e86
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None

    rng = random.Random(0x42434834474c4f42)
    cases = []
    for case in range(128):
        baseline = bytearray(b'\xa5') * RAM_SIZE
        baseline[OVERLAY_BASE - 0x0c000000:
                OVERLAY_BASE - 0x0c000000 + len(overlay)] = overlay
        retail_ram, actual_ram = bytearray(baseline), bytearray(baseline)

        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.r[4] = rng.getrandbits(32)
        expected.sr = rng.getrandbits(32)
        expected.pr = 0x0c001000 + case * 4
        expected.pc = ADDRESS
        actual = State.from_buffer_copy(bytes(expected))

        steps = reference(retail_ram, expected)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        function(ram_view, ctypes.byref(actual))
        assert retail_ram == actual_ram, f'RAM mismatch in case {case}'
        assert bytes(expected) == bytes(actual), f'CPU-state mismatch in case {case}'
        cases.append({'case': case, 'stored_value': hex(expected.r[4]),
                      'retail_instructions': steps, 'full_ram_and_cpu_state_match': True})

    source = out / 'source/b_chaos4_global_store.c'
    report = {
        'functions_verified': 1,
        'total_abi_cases': len(cases),
        'comparison_level': 'All 16 MiB RAM and complete modeled CPU state compared byte-for-byte.',
        'scope': 'The six-byte SH-4 routine loads the global pointer literal at 0x0c901ef0 into R3, stores incoming R4 at that address in the RTS delay slot, and returns.',
        'limitations': ['Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or end-to-end boss gameplay.'],
        'dependencies': [{'module': 'B_CHAOS4.BIN', 'load_address': hex(OVERLAY_BASE),
                          'sha256': hashlib.sha256(overlay).hexdigest(),
                          'global_pointer_literal_address': hex(LITERAL_ADDRESS),
                          'global_pointer_value': hex(GLOBAL_ADDRESS)}],
        'routines': [{'module': 'B_CHAOS4.BIN', 'load_address': hex(OVERLAY_BASE),
                      'address': hex(ADDRESS), 'end_exclusive': hex(END),
                      'name': 'global_store',
                      'retail_span_sha256': hashlib.sha256(overlay[offset:offset + END - ADDRESS]).hexdigest(),
                      'cases_passed': len(cases), 'cases': cases}],
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    }
    report_path = out / 'analysis/common-overlays/B_CHAOS4.BIN/global_store_verification.json'
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'B_CHAOS4 global setter passed {len(cases)} full-RAM and CPU-state comparisons')


if __name__ == '__main__':
    main()
