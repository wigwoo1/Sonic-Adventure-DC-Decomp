"""Verify the table-driven input mapping value helper against retail SH-4."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

BASE, ADDRESS, SPAN = 0x8c010000, 0x8c0a69a8, 162
RAM_SIZE = 16 * 1024 * 1024
STACK_TOP = 0x8cfff000
STACK_OFFSET = (STACK_TOP & 0x1fffffff) - 0x0c000000


def main():
    out = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    lib = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/input_mapping_value.dll'))
    function = lib.sa_input_mapping_value_8c0a69a8
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None
    original = bytearray(b'\xa5') * RAM_SIZE
    original[0x10000:0x10000 + len(binary)] = binary
    cases_to_run = []

    for row in range(16):
        for code in range(1, 15):
            cases_to_run.append((row, code, ((row + code) & 3) - 1))
    for code in (-8, -1, 0, 15, 34, 35, 36, 37, 38, 39, 41, 42, 43, 100):
        cases_to_run.extend(((0, code, 0), (15, code, -1)))
    rng = random.Random(0x494E5055544D4150)
    for _ in range(24):
        cases_to_run.append((rng.randrange(16), rng.choice((-8, -1, 0, 1, 7, 14, 15, 34, 35, 38, 39, 42, 100)),
                             rng.randrange(-3, 4)))
    cases = []

    for index, (row, code, bank) in enumerate(cases_to_run):
        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.r[4], expected.r[5], expected.r[6] = row, code & 0xffffffff, bank & 0xffffffff
        expected.r[15] = STACK_TOP
        expected.sr = rng.getrandbits(32)
        expected.pr = 0x8c001a00 + index * 4
        expected.pc = ADDRESS
        actual = State.from_buffer_copy(bytes(expected))
        retail_ram, actual_ram = bytearray(original), bytearray(original)
        steps = reference(retail_ram, expected)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        function(ram_view, ctypes.byref(actual))
        stack_low = STACK_OFFSET - 64
        assert actual_ram[:stack_low] == retail_ram[:stack_low], f'RAM mismatch below stack frame, case {index}'
        assert actual_ram[STACK_OFFSET:] == retail_ram[STACK_OFFSET:], f'RAM mismatch above stack frame, case {index}'
        assert actual.r[0] == expected.r[0], f'return mismatch for {row=} {code=} {bank=}'
        assert list(actual.r[8:]) == list(expected.r[8:]), f'callee-saved register mismatch, case {index}'
        assert actual.pr == expected.pr and actual.pc == expected.pc, f'control-state mismatch, case {index}'
        cases.append({'case': index, 'row': row, 'code': code, 'bank': bank,
                      'retail_instructions': steps, 'return_value': hex(expected.r[0]),
                      'abi_state_and_ram_match_outside_volatile_stack_frame': True})

    source = out / 'source/input_mapping_value.c'
    report = {
        'functions_verified': 1,
        'total_abi_cases': len(cases),
        'comparison_level': 'Return register R0, preserved registers R8-R15, PR/PC, and complete RAM outside a reserved 64-byte stack scratch region; caller-clobbered R1-R7, SR, and ephemeral stack bytes are excluded.',
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'scope': 'Translated the table-driven mapping-value routine at 0x8c0a69a8, including its ROM mask and 16-bit lookup-table reads. Differential cases cover all 16 rows for codes 1-14, branch boundaries, negative bank values, and randomized inputs.',
        'limitations': [
            'The ROM lookup table and mask are read from their retail addresses; higher-level device mapping and callers remain incomplete.',
            'Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or end-to-end game execution.'
        ],
        'routine': {'module': '1ST_READ.BIN', 'address': hex(ADDRESS),
                    'end_exclusive': hex(ADDRESS + SPAN),
                    'retail_span_sha256': hashlib.sha256(binary[ADDRESS - BASE:ADDRESS - BASE + SPAN]).hexdigest(),
                    'cases_passed': len(cases), 'cases': cases},
    }
    (out / 'analysis/input_mapping_value_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"Input mapping-value routine passed {len(cases)} ABI-level comparisons")


if __name__ == '__main__':
    main()
