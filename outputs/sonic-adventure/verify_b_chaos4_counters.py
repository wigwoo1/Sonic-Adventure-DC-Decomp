"""Differentially verify the B_CHAOS4 byte-counter helper family."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

OVERLAY_BASE = 0x0c900000
RAM_SIZE = 16 * 1024 * 1024
POINTER_A_LITERAL, POINTER_B_LITERAL = 0x0c905ae0, 0x0c905ae4
POINTER_A, POINTER_B = 0x0c95d9c5, 0x0c95d9c4
ROUTINES = [
    ('counter_pair_initialize', 0x0c905a80, 0x0c905a8c,
     'sa_b_chaos4_counter_pair_initialize_0c905a80'),
    ('counter_secondary_increment', 0x0c905a8c, 0x0c905a96,
     'sa_b_chaos4_counter_secondary_increment_0c905a8c'),
    ('counter_secondary_decrement', 0x0c905a96, 0x0c905aa0,
     'sa_b_chaos4_counter_secondary_decrement_0c905a96'),
    ('counter_secondary_read', 0x0c905aa0, 0x0c905aa8,
     'sa_b_chaos4_counter_secondary_read_0c905aa0'),
    ('counter_primary_increment', 0x0c905aa8, 0x0c905ab2,
     'sa_b_chaos4_counter_primary_increment_0c905aa8'),
    ('counter_primary_decrement', 0x0c905ab2, 0x0c905abc,
     'sa_b_chaos4_counter_primary_decrement_0c905ab2'),
    ('counter_primary_read', 0x0c905abc, 0x0c905ac4,
     'sa_b_chaos4_counter_primary_read_0c905abc'),
]


def main():
    out = ROOT / 'outputs/sonic-adventure'
    overlay = (ROOT / 'work/sonic-adventure/decoded/SONICADV/B_CHAOS4.BIN').read_bytes()
    assert int.from_bytes(overlay[POINTER_A_LITERAL - OVERLAY_BASE:
                                  POINTER_A_LITERAL - OVERLAY_BASE + 4], 'little') == POINTER_A
    assert int.from_bytes(overlay[POINTER_B_LITERAL - OVERLAY_BASE:
                                  POINTER_B_LITERAL - OVERLAY_BASE + 4], 'little') == POINTER_B

    lib = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/b_chaos4_counters.dll'))
    functions = {}
    for _, _, _, symbol in ROUTINES:
        function = getattr(lib, symbol)
        function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
        function.restype = None
        functions[symbol] = function

    rng = random.Random(0x42434834434e5452)
    edge_values = [0x00, 0x01, 0x7e, 0x7f, 0x80, 0x81, 0xfe, 0xff]
    routines_report = []
    for name, address, end, symbol in ROUTINES:
        values = edge_values + [rng.randrange(256) for _ in range(56)]
        cases = []
        for case, value in enumerate(values):
            adjacent = rng.randrange(256)
            baseline = bytearray(b'\xa5') * RAM_SIZE
            baseline[OVERLAY_BASE - 0x0c000000:
                    OVERLAY_BASE - 0x0c000000 + len(overlay)] = overlay
            baseline[POINTER_A - 0x0c000000] = value
            baseline[POINTER_B - 0x0c000000] = adjacent
            retail_ram, actual_ram = bytearray(baseline), bytearray(baseline)

            expected = State()
            expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
            expected.sr = rng.getrandbits(32)
            expected.pr = 0x0c001000 + case * 4
            expected.pc = address
            actual = State.from_buffer_copy(bytes(expected))

            steps = reference(retail_ram, expected)
            ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
            functions[symbol](ram_view, ctypes.byref(actual))
            assert retail_ram == actual_ram, f'{name}: RAM mismatch in case {case}'
            assert bytes(expected) == bytes(actual), f'{name}: CPU-state mismatch in case {case}'
            cases.append({'case': case, 'target_initial': value,
                          'adjacent_initial': adjacent, 'retail_instructions': steps,
                          'full_ram_and_cpu_state_match': True})

        offset = address - OVERLAY_BASE
        routines_report.append({
            'module': 'B_CHAOS4.BIN', 'load_address': hex(OVERLAY_BASE),
            'address': hex(address), 'end_exclusive': hex(end), 'name': name,
            'retail_span_sha256': hashlib.sha256(overlay[offset:offset + end - address]).hexdigest(),
            'cases_passed': len(cases), 'cases': cases,
        })

    source = out / 'source/b_chaos4_counters.c'
    report = {
        'functions_verified': len(ROUTINES),
        'total_abi_cases': sum(item['cases_passed'] for item in routines_report),
        'comparison_level': 'All 16 MiB RAM and complete modeled CPU state compared byte-for-byte.',
        'scope': 'Seven B_CHAOS4 routines initialize, increment, decrement, and read two adjacent byte counters; 64 cases per routine include signed-boundary and wraparound inputs.',
        'limitations': ['Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or end-to-end boss gameplay.'],
        'dependencies': [{'module': 'B_CHAOS4.BIN', 'load_address': hex(OVERLAY_BASE),
                          'sha256': hashlib.sha256(overlay).hexdigest(),
                          'pointer_a_literal': hex(POINTER_A_LITERAL), 'pointer_a': hex(POINTER_A),
                          'pointer_b_literal': hex(POINTER_B_LITERAL), 'pointer_b': hex(POINTER_B)}],
        'routines': routines_report,
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    }
    report_path = out / 'analysis/common-overlays/B_CHAOS4.BIN/counters_verification.json'
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"B_CHAOS4 counter family passed {report['total_abi_cases']} full-RAM and CPU-state comparisons")


if __name__ == '__main__':
    main()
