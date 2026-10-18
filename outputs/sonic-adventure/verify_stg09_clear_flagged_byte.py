"""Differentially verify STG09's conditional byte clear."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

OVERLAY_BASE, ADDRESS, END = 0x0c900000, 0x0c903a60, 0x0c903a6e
MAIN_BASE = 0x8c010000
RAM_BASE, RAM_SIZE = 0x0c000000, 16 * 1024 * 1024
STACK_GUARD, CASE_COUNT = 0x2000, 256
OBJECT_BASE = 0x8c700000


def offset(address):
    return (address & 0x1fffffff) - RAM_BASE


def main():
    out = ROOT / 'outputs/sonic-adventure'
    overlay = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
    main_binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    span_offset = ADDRESS - OVERLAY_BASE
    assert len(overlay[span_offset:span_offset + END - ADDRESS]) == END - ADDRESS

    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/stg09_clear_flagged_byte.dll'))
    function = library.sa_stg09_clear_flagged_byte_0c903a60
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None

    original = bytearray(b'\xa5') * RAM_SIZE
    for address, binary in ((MAIN_BASE, main_binary), (OVERLAY_BASE, overlay)):
        start = offset(address)
        original[start:start + len(binary)] = binary

    rng = random.Random(0x535447093a60)
    cases = []
    for case in range(CASE_COUNT):
        baseline = bytearray(original)
        value = case
        address = OBJECT_BASE + case * 16
        baseline[offset(address)] = value
        baseline[offset(address + 8)] = rng.randrange(256)
        retail_ram, native_ram = bytearray(baseline), bytearray(baseline)

        initial = State()
        initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
        initial.r[4] = address
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
        assert retail_ram[:stack_start] == native_ram[:stack_start], f'RAM before caller stack differs in case {case}'
        assert retail_ram[stack_end:] == native_ram[stack_end:], f'RAM after caller stack differs in case {case}'
        assert list(actual.r) == list(expected.r), f'GPR state differs in case {case}, input={value:#x}'
        assert actual.sr == expected.sr, f'SR differs in case {case}, input={value:#x}'
        assert actual.pr == expected.pr and actual.pc == expected.pc, f'call boundary differs in case {case}'
        cases.append({'case': case, 'input_byte': hex(value),
                      'cleared_adjacent_byte': (value & 3) == 0,
                      'retail_instructions': steps, 'ram_and_full_cpu_state_match': True})

    source_path = out / 'source/stg09_clear_flagged_byte.c'
    report = {
        'functions_verified': 1, 'total_abi_cases': len(cases),
        'source_sha256': hashlib.sha256(source_path.read_bytes()).hexdigest(),
        'comparison_level': 'All RAM except caller-stack guard; all 16 GPRs, SR, PR and return PC compared.',
        'scope': 'Reads the signed byte at R4, sets T when its low two bits are zero, and only then clears R4+8 and returns R0=0; otherwise preserves R0 as the sign-extended input byte.',
        'limitations': ['Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or gameplay.',
                        'The object bytes are test-initialized in cloned RAM; retail overlay and instruction bytes remain unchanged.'],
        'dependencies': [
            {'module': 'STG09.BIN', 'load_address': hex(OVERLAY_BASE),
             'sha256': hashlib.sha256(overlay).hexdigest()},
            {'module': '1ST_READ.BIN', 'load_address': hex(MAIN_BASE),
             'sha256': hashlib.sha256(main_binary).hexdigest()},
        ],
        'routines': [{
            'module': 'STG09.BIN', 'load_address': hex(OVERLAY_BASE),
            'address': hex(ADDRESS), 'end_exclusive': hex(END),
            'name': 'clear_flagged_byte',
            'retail_span_sha256': hashlib.sha256(overlay[span_offset:span_offset + END - ADDRESS]).hexdigest(),
            'cases_passed': len(cases), 'cases': cases,
        }],
    }
    report_path = out / 'analysis/common-overlays/STG09.BIN/clear_flagged_byte_verification.json'
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'STG09 conditional byte clear passed {len(cases)} RAM and full CPU comparisons')


if __name__ == '__main__':
    main()
