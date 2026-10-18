"""Differentially verify B_CHAOS6's signed countdown and accumulator update."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

OVERLAY_BASE, ADDRESS, END = 0x0c900000, 0x0c9062f0, 0x0c906310
MAIN_BASE = 0x8c010000
RAM_BASE, RAM_SIZE = 0x0c000000, 16 * 1024 * 1024
STACK_GUARD, CASE_COUNT = 0x2000, 128
CONTEXT, WORK = 0x8c020000, 0x8c021000
COUNTS = [0, 1, 2, 3, 0x7fff, 0x8000, 0xffff, 4, 1, 0x8001, 0x7ffe, 0xfffe]


def offset(address):
    return (address & 0x1fffffff) - RAM_BASE


def main():
    out = ROOT / 'outputs/sonic-adventure'
    overlay = (ROOT / 'work/sonic-adventure/decoded/SONICADV/B_CHAOS6.BIN').read_bytes()
    main_binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    span_offset = ADDRESS - OVERLAY_BASE
    assert len(overlay[span_offset:span_offset + END - ADDRESS]) == END - ADDRESS

    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/b_chaos6_countdown_accumulator.dll'))
    function = library.sa_b_chaos6_countdown_accumulator_0c9062f0
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None

    original = bytearray(b'\xa5') * RAM_SIZE
    for address, binary in ((MAIN_BASE, main_binary), (OVERLAY_BASE, overlay)):
        start = offset(address)
        original[start:start + len(binary)] = binary

    rng = random.Random(0x4348414f533662f0)
    cases = []
    for case in range(CASE_COUNT):
        baseline = bytearray(original)
        count = COUNTS[case] if case < len(COUNTS) else rng.getrandbits(16)
        accumulator = rng.getrandbits(32)
        struct.pack_into('<I', baseline, offset(CONTEXT) + 0x20, WORK)
        struct.pack_into('<H', baseline, offset(WORK) + 0x10, count)
        struct.pack_into('<I', baseline, offset(WORK) + 0x18, accumulator)
        retail_ram, native_ram = bytearray(baseline), bytearray(baseline)

        initial = State()
        initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
        initial.r[4] = CONTEXT
        initial.r[15] = 0x0cff0000
        initial.sr = (rng.getrandbits(32) & ~1) | (case & 1)
        initial.pr = 0x0c004000 + case * 4
        initial.pc = ADDRESS
        expected = State.from_buffer_copy(bytes(initial))
        actual = State.from_buffer_copy(bytes(initial))

        steps = reference(retail_ram, expected)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
        function(ram_view, ctypes.byref(actual))

        stack_start = initial.r[15] - RAM_BASE - STACK_GUARD
        stack_end = initial.r[15] - RAM_BASE + STACK_GUARD
        if retail_ram[:stack_start] != native_ram[:stack_start]:
            first = next(i for i, (a, b) in enumerate(zip(retail_ram[:stack_start], native_ram[:stack_start])) if a != b)
            raise AssertionError(f'RAM differs case={case} count={count:#x} first={first + RAM_BASE:#010x}; retail={retail_ram[first]:02x}; native={native_ram[first]:02x}')
        assert retail_ram[stack_end:] == native_ram[stack_end:], f'RAM differs after caller stack in case {case}'
        assert list(actual.r) == list(expected.r), f'GPR state differs in case {case}, count={count:#x}'
        assert actual.sr == expected.sr, f'SR differs in case {case}, count={count:#x}'
        assert actual.pr == expected.pr and actual.pc == expected.pc, f'call boundary differs in case {case}'
        cases.append({'case': case, 'initial_count_u16': hex(count),
                      'initial_accumulator': hex(accumulator), 'retail_instructions': steps,
                      'ram_and_full_cpu_state_match': True})

    source_path = out / 'source/b_chaos6_countdown_accumulator.c'
    report = {
        'functions_verified': 1, 'total_abi_cases': len(cases),
        'source_sha256': hashlib.sha256(source_path.read_bytes()).hexdigest(),
        'comparison_level': 'All RAM except caller-stack guard; all 16 GPRs, SR, PR and return PC compared.',
        'scope': 'Loads the work pointer through the context at R4+0x20, decrements and stores the signed 16-bit count at work+0x10 with wraparound, and when the sign-extended result is nonpositive resets it to 3 and adds 0xb6 modulo 2^32 to work+0x18.',
        'limitations': ['Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or gameplay.',
                        'Context, counter and accumulator are initialized in cloned RAM; retail instruction bytes remain unchanged.'],
        'dependencies': [
            {'module': 'B_CHAOS6.BIN', 'load_address': hex(OVERLAY_BASE),
             'sha256': hashlib.sha256(overlay).hexdigest()},
            {'module': '1ST_READ.BIN', 'load_address': hex(MAIN_BASE),
             'sha256': hashlib.sha256(main_binary).hexdigest()},
        ],
        'routines': [{
            'module': 'B_CHAOS6.BIN', 'load_address': hex(OVERLAY_BASE),
            'address': hex(ADDRESS), 'end_exclusive': hex(END),
            'name': 'countdown_accumulator',
            'retail_span_sha256': hashlib.sha256(overlay[span_offset:span_offset + END - ADDRESS]).hexdigest(),
            'cases_passed': len(cases), 'cases': cases,
        }],
    }
    report_path = out / 'analysis/common-overlays/B_CHAOS6.BIN/countdown_accumulator_verification.json'
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'B_CHAOS6 countdown accumulator passed {len(cases)} RAM and full CPU comparisons')


if __name__ == '__main__':
    main()
