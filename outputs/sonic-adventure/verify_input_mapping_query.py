"""Differentially verify the input mapping query wrapper."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

BASE, ADDRESS, SPAN = 0x8c010000, 0x8c0a6ab4, 30
RAM_SIZE, STACK_TOP = 16 * 1024 * 1024, 0x8cfff000
STACK_OFFSET = (STACK_TOP & 0x1fffffff) - 0x0c000000


def main():
    out = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    lib = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/input_mapping_query.dll'))
    function = lib.sa_input_mapping_query_8c0a6ab4
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None
    original = bytearray(b'\xa5') * RAM_SIZE
    original[0x10000:0x10000 + len(binary)] = binary
    rng = random.Random(ADDRESS)
    combinations = []
    for row in range(16):
        for code in (1, 3, 7, 14):
            combinations.append((row, code, ((row + code) & 3) - 1))
    for code in (0, 15, 34, 35, 36, 38, 39, 41, 42, 43, 100):
        combinations.extend(((0, code, 0), (15, code, 2)))
    for _ in range(32):
        combinations.append((rng.randrange(16), rng.choice((0, 1, 5, 14, 15, 34, 35, 38, 39, 42, 100)),
                             rng.randrange(-2, 4)))

    cases = []
    returned_true = returned_false = 0
    for case, (row, code, bank) in enumerate(combinations):
        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.r[4] = 0x8c100000
        expected.r[5], expected.r[6], expected.r[7] = row, code & 0xffffffff, bank & 0xffffffff
        expected.r[15] = STACK_TOP
        expected.sr = rng.getrandbits(32)
        expected.pr = 0x8c001e00 + case * 4
        expected.pc = ADDRESS
        actual = State.from_buffer_copy(bytes(expected))
        fixture = bytearray(original)
        fixture[0x100200:0x100300] = bytes(rng.getrandbits(8) for _ in range(0x100))
        retail_ram, actual_ram = bytearray(fixture), bytearray(fixture)
        steps = reference(retail_ram, expected)
        view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        function(view, ctypes.byref(actual))
        stack_low = STACK_OFFSET - 0x100
        assert actual_ram[:stack_low] == retail_ram[:stack_low], f'RAM mismatch below stack, case {case}'
        assert actual_ram[STACK_OFFSET:] == retail_ram[STACK_OFFSET:], f'RAM mismatch above stack, case {case}'
        assert actual.r[0] == expected.r[0], f'result mismatch for {row=} {code=} {bank=}'
        assert list(actual.r[8:]) == list(expected.r[8:]), f'callee-saved register mismatch, case {case}'
        assert actual.pr == expected.pr and actual.pc == expected.pc, f'control mismatch, case {case}'
        returned_true += bool(expected.r[0])
        returned_false += not bool(expected.r[0])
        cases.append({'case': case, 'row': row, 'code': code, 'bank': bank,
                      'retail_instructions': steps, 'result': expected.r[0],
                      'abi_state_and_ram_match_outside_stack': True})

    source = out / 'source/input_mapping_query.c'
    span = binary[ADDRESS - BASE:ADDRESS - BASE + SPAN]
    report = {
        'routine_start': hex(ADDRESS), 'routine_end_exclusive': hex(ADDRESS + SPAN),
        'cases_passed': len(cases), 'true_results': returned_true,
        'false_results': returned_false,
        'comparison_level': 'Return register R0, preserved registers R8-R15, PR/PC, and complete RAM outside a 256-byte volatile stack region; caller-clobbered registers and SR are excluded.',
        'retail_span_sha256': hashlib.sha256(span).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'scope': 'Composed the retail mapping-value, bitset-test, and wrapper behavior at 0x8c0a6ab4. Tested all 16 table rows across representative ordinary codes, branch boundaries, and randomized cases.',
        'limitations': [
            'ROM mapping tables are read directly from the retail image; higher-level controller lifecycle remains incomplete.',
            'Volatile stack bytes and caller-clobbered registers are not compared byte-for-byte.',
            'Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or end-to-end game execution.'
        ],
        'cases': cases,
    }
    (out / 'analysis/input_mapping_query_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"Input mapping query passed {len(cases)} ABI-level comparisons ({returned_true} true, {returned_false} false)")


if __name__ == '__main__':
    main()
