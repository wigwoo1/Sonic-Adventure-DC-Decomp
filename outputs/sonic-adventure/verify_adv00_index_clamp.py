"""Compare ADV00's signed range-clamp helper with retail SH-4 execution."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

BASE, ADDRESS, END = 0x0c900000, 0x0c90ffbe, 0x0c90ffd2
RAM_SIZE = 16 * 1024 * 1024
EXPLICIT = [
    (0, 0, 0), (0, 0, 1), (0, 1, 1), (0, 1, 2),
    (0xffffffff, 0, 0), (0xffffffff, 0, 0xffffffff),
    (0x7fffffff, 1, 0x7fffffff), (0x7fffffff, 1, 0x80000000),
    (0x80000000, 0, 0), (0x80000000, 0, 0x80000000),
    (0xfffffffe, 3, 0), (0x7ffffffe, 1, 0x7fffffff),
    (0x7ffffffe, 1, 0x7ffffffe), (5, 7, 13), (5, 7, 12),
]


def main():
    out = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/decoded/SONICADV/ADV00.BIN').read_bytes()
    lib = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/adv00_index_clamp.dll'))
    function = lib.sa_adv00_index_clamp_0c90ffbe
    function.argtypes = [ctypes.POINTER(State)]
    function.restype = None
    original = bytearray(b'\xa5') * RAM_SIZE
    ram_base = BASE - 0x0c000000
    original[ram_base:ram_base + len(binary)] = binary
    offset = ADDRESS - BASE
    assert len(binary) >= END - BASE
    assert binary[offset:offset + END - ADDRESS] == bytes.fromhex(
        '5c 34 11 44 01 8b 63 34 01 8b 0b 00 ff e0 43 60 0b 00 09 00')

    rng = random.Random(0x4144563030494e44)
    inputs = list(EXPLICIT)
    inputs.extend((rng.getrandbits(32), rng.getrandbits(32), rng.getrandbits(32))
                  for _ in range(128))
    outcomes = set()
    cases = []
    for case, (left, right, limit) in enumerate(inputs):
        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.r[4], expected.r[5], expected.r[6] = left, right, limit
        expected.sr = rng.getrandbits(32)
        expected.pr = 0x0c001000 + case * 4
        expected.pc = ADDRESS
        actual = State.from_buffer_copy(bytes(expected))
        retail_ram, actual_ram = bytearray(original), bytearray(original)
        steps = reference(retail_ram, expected)
        function(ctypes.byref(actual))
        assert 5 <= steps <= 8, f'unexpected path length {steps} for {left:#x},{right:#x},{limit:#x}'
        assert actual_ram == retail_ram, f'RAM mismatch at case {case}'
        assert bytes(actual) == bytes(expected), (
            f'CPU-state mismatch at case {case}: inputs={left:#x},{right:#x},{limit:#x}; '
            f'actual r0={actual.r[0]:#x}, r4={actual.r[4]:#x}, sr={actual.sr:#x}; '
            f'retail r0={expected.r[0]:#x}, r4={expected.r[4]:#x}, sr={expected.sr:#x}')
        signed_sum = ((left + right) & 0xffffffff)
        signed_sum = signed_sum - 0x100000000 if signed_sum & 0x80000000 else signed_sum
        outcomes.add('negative' if signed_sum < 0 else
                     'at_or_above_limit' if signed_sum >= (limit - 0x100000000 if limit & 0x80000000 else limit)
                     else 'in_range')
        cases.append({'case': case, 'left': hex(left), 'right': hex(right),
                      'upper_exclusive': hex(limit), 'retail_instructions': steps,
                      'full_state_and_ram_match': True})
    assert outcomes == {'negative', 'at_or_above_limit', 'in_range'}, outcomes

    source = out / 'source/adv00_index_clamp.c'
    report = {
        'functions_verified': 1,
        'total_full_state_cases': len(cases),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'scope': 'ADV00.BIN helper at 0x0c90ffbe sums two 32-bit arguments, rejects signed-negative sums and sums at or above a signed upper bound, and returns -1 for rejected values. Full SH-4 CPU state and all 16 MiB RAM compared against retail execution.',
        'limitations': ['Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or end-to-end ADV00 gameplay.'],
        'branch_outcomes_exercised': sorted(outcomes),
        'routines': [{'module': 'ADV00.BIN', 'load_address': hex(BASE),
                      'address': hex(ADDRESS), 'end_exclusive': hex(END),
                      'name': 'index_clamp',
                      'module_sha256': hashlib.sha256(binary).hexdigest(),
                      'retail_span_sha256': hashlib.sha256(binary[offset:offset + END - ADDRESS]).hexdigest(),
                      'cases_passed': len(cases), 'cases': cases}],
    }
    report_path = out / 'analysis/common-overlays/ADV00.BIN/index_clamp_verification.json'
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"ADV00 index clamp passed {len(cases)} full-state comparisons across {len(outcomes)} branch outcomes")


if __name__ == '__main__':
    main()
