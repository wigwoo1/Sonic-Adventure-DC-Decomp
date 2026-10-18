"""Differentially verify the B_CHAOS4 seven-record flag-table callback."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

OVERLAY_BASE, ADDRESS, END = 0x0c900000, 0x0c900280, 0x0c900348
TABLE_POINTER_LITERAL, TABLE_ADDRESS = 0x0c900400, 0x0c936e18
RAM_SIZE = 16 * 1024 * 1024
STACK_GUARD = 0x2000
CASE_COUNT = 128
TABLE_ROWS = 32


def main():
    out = ROOT / 'outputs/sonic-adventure'
    overlay = (ROOT / 'work/sonic-adventure/decoded/SONICADV/B_CHAOS4.BIN').read_bytes()
    offset = ADDRESS - OVERLAY_BASE
    assert len(overlay[offset:offset + END - ADDRESS]) == END - ADDRESS
    pointer_offset = TABLE_POINTER_LITERAL - OVERLAY_BASE
    assert int.from_bytes(overlay[pointer_offset:pointer_offset + 4], 'little') == TABLE_ADDRESS

    fixture_rng = random.Random(0x42434834464c4147)
    fixture = bytes(0 if (row + col) % 3 == 0 else fixture_rng.randrange(1, 256)
                    for row in range(TABLE_ROWS) for col in range(7))

    dll = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/b_chaos4_flag_table_callback.dll'))
    function = dll.sa_b_chaos4_flag_table_callback_0c900280
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None

    original = bytearray(b'\xa5') * RAM_SIZE
    original[OVERLAY_BASE - 0x0c000000:
             OVERLAY_BASE - 0x0c000000 + len(overlay)] = overlay
    table_offset = TABLE_ADDRESS - 0x0c000000
    original[table_offset:table_offset + len(fixture)] = fixture

    rng = random.Random(0x4243483443415345)
    cases = []
    stack_changes = 0
    for case in range(CASE_COUNT):
        context = 0x0c300000 + case * 0x200
        context_data = 0x0c340000 + case * 0x200
        object_address = 0x0c500000 + case * 0x400
        table_row = case % TABLE_ROWS
        baseline = bytearray(original)
        baseline[context - 0x0c000000 + 0x38:context - 0x0c000000 + 0x3c] = context_data.to_bytes(4, 'little')
        baseline[context_data - 0x0c000000 + 0x0c:context_data - 0x0c000000 + 0x10] = object_address.to_bytes(4, 'little')
        for record in range(7):
            flags = rng.getrandbits(32)
            at = object_address - 0x0c000000 + record * 0x34
            baseline[at:at + 4] = flags.to_bytes(4, 'little')
        retail_ram, actual_ram = bytearray(baseline), bytearray(baseline)

        initial = State()
        initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
        initial.r[4] = context
        initial.r[5] = table_row
        initial.r[15] = 0x0cff0000
        initial.sr = rng.getrandbits(32)
        initial.pr = 0x0c001000 + case * 4
        initial.pc = ADDRESS
        expected = State.from_buffer_copy(bytes(initial))
        actual = State.from_buffer_copy(bytes(initial))

        steps = reference(retail_ram, expected)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        function(ram_view, ctypes.byref(actual))

        stack_start = initial.r[15] - 0x0c000000 - STACK_GUARD
        stack_end = initial.r[15] - 0x0c000000 + STACK_GUARD
        assert retail_ram[:stack_start] == actual_ram[:stack_start], f'RAM before stack differs in case {case}'
        assert retail_ram[stack_end:] == actual_ram[stack_end:], f'RAM after stack differs in case {case}'
        stack_changes += sum(a != b for a, b in zip(retail_ram[stack_start:stack_end],
                                                    actual_ram[stack_start:stack_end]))
        assert list(actual.r[8:15]) == list(expected.r[8:15]), f'callee-saved GPR mismatch in case {case}'
        assert actual.r[15] == expected.r[15], f'SP mismatch in case {case}'
        assert actual.pr == expected.pr and actual.pc == expected.pc, f'call boundary mismatch in case {case}'
        cases.append({'case': case, 'table_row': table_row, 'retail_instructions': steps,
                      'non_stack_ram_and_preserved_abi_match': True})

    source = out / 'source/b_chaos4_flag_table_callback.c'
    report = {
        'functions_verified': 1, 'total_abi_cases': len(cases),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'comparison_level': f'RAM compared across 16 MiB except +/-{STACK_GUARD:#x} caller-stack scratch; preserved R8-R14, SP, PR and return PC compared. Volatile registers, SR and stack scratch are excluded.',
        'stack_scratch_differences_observed': stack_changes,
        'scope': 'B_CHAOS4 callback at 0x0c900280 uses seven table bytes to set or clear bit 0x10 in seven 0x34-byte-spaced object records. Thirty-two deterministic rows exercise every table column and both zero/nonzero paths.',
        'limitations': ['The pointed-to runtime flag table at 0x0c936e18 is outside the B_CHAOS4 overlay and is replaced with deterministic verifier fixture data; its retail initialization is not asserted.', 'Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or end-to-end boss gameplay.'],
        'dependencies': [{'module': 'B_CHAOS4.BIN', 'load_address': hex(OVERLAY_BASE),
                          'sha256': hashlib.sha256(overlay).hexdigest(),
                          'table_pointer_literal': hex(TABLE_POINTER_LITERAL),
                          'table_pointer': hex(TABLE_ADDRESS),
                          'fixture_bytes': len(fixture),
                          'fixture_sha256': hashlib.sha256(fixture).hexdigest(),
                          'fixture_hex': fixture.hex(),
                          'fixture_provenance': 'Deterministic test data; not asserted to be original retail runtime data.'}],
        'routines': [{'module': 'B_CHAOS4.BIN', 'load_address': hex(OVERLAY_BASE),
                      'address': hex(ADDRESS), 'end_exclusive': hex(END),
                      'name': 'flag_table_callback',
                      'retail_span_sha256': hashlib.sha256(overlay[offset:offset + END - ADDRESS]).hexdigest(),
                      'cases_passed': len(cases), 'cases': cases}],
    }
    report_path = out / 'analysis/common-overlays/B_CHAOS4.BIN/flag_table_callback_verification.json'
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'B_CHAOS4 flag-table callback passed {len(cases)} RAM and ABI comparisons')


if __name__ == '__main__':
    main()
