"""Differentially verify AL_MAIN's secondary callback-field stores."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

OVERLAY_BASE, ADDRESS, END = 0x0c900000, 0x0c908530, 0x0c90853e
MAIN_BASE = 0x8c010000
RAM_BASE, RAM_SIZE = 0x0c000000, 16 * 1024 * 1024
STACK_GUARD, CASE_COUNT = 0x2000, 128
OBJECT_BASE = 0x8c720000


def offset(address):
    return (address & 0x1fffffff) - RAM_BASE


def main():
    out = ROOT / 'outputs/sonic-adventure'
    overlay = (ROOT / 'work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN').read_bytes()
    main_binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    span_offset = ADDRESS - OVERLAY_BASE
    assert len(overlay[span_offset:span_offset + END - ADDRESS]) == END - ADDRESS

    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/al_main_set_secondary_callback_fields.dll'))
    function = library.sa_al_main_set_secondary_callback_fields_0c908530
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None

    original = bytearray(b'\xa5') * RAM_SIZE
    for address, binary in ((MAIN_BASE, main_binary), (OVERLAY_BASE, overlay)):
        start = offset(address)
        original[start:start + len(binary)] = binary

    rng = random.Random(0x414c4d41494e8530)
    cases = []
    for case in range(CASE_COUNT):
        baseline = bytearray(original)
        object_address = OBJECT_BASE + case * 0x1000
        retail_ram, native_ram = bytearray(baseline), bytearray(baseline)

        initial = State()
        initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
        initial.r[4] = object_address
        initial.r[5] = rng.getrandbits(32)
        initial.r[6] = rng.getrandbits(32)
        initial.r[15] = 0x0cff0000
        initial.sr = rng.getrandbits(32)
        initial.pr = 0x0c008000 + case * 4
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
        assert list(actual.r) == list(expected.r), f'GPR state differs in case {case}'
        assert actual.sr == expected.sr, f'SR differs in case {case}'
        assert actual.pr == expected.pr and actual.pc == expected.pc, f'call boundary differs in case {case}'
        cases.append({'case': case, 'object_address': hex(object_address),
                      'ram_and_full_cpu_state_match': True, 'retail_instructions': steps})

    source_path = out / 'source/al_main_set_secondary_callback_fields.c'
    report = {
        'functions_verified': 1, 'total_abi_cases': len(cases),
        'source_sha256': hashlib.sha256(source_path.read_bytes()).hexdigest(),
        'comparison_level': 'All RAM except caller-stack guard; all 16 GPRs, SR, PR and return PC compared.',
        'scope': 'Sets object fields at param+0x7f0 and param+0x800 from R5 and R6; reproduces R3/R4 address-calculation effects and the second RTS-delay-slot store.',
        'limitations': ['Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or AL_MAIN runtime.',
                        'Object storage is test-initialized in cloned RAM; retail instruction bytes remain unchanged.'],
        'dependencies': [
            {'module': 'AL_MAIN.BIN', 'load_address': hex(OVERLAY_BASE),
             'sha256': hashlib.sha256(overlay).hexdigest()},
            {'module': '1ST_READ.BIN', 'load_address': hex(MAIN_BASE),
             'sha256': hashlib.sha256(main_binary).hexdigest()},
        ],
        'routines': [{
            'module': 'AL_MAIN.BIN', 'load_address': hex(OVERLAY_BASE),
            'address': hex(ADDRESS), 'end_exclusive': hex(END),
            'name': 'set_secondary_callback_fields',
            'retail_span_sha256': hashlib.sha256(overlay[span_offset:span_offset + END - ADDRESS]).hexdigest(),
            'cases_passed': len(cases), 'cases': cases,
        }],
    }
    report_path = out / 'analysis/common-overlays/AL_MAIN.BIN/set_secondary_callback_fields_verification.json'
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'AL_MAIN secondary callback-field setter passed {len(cases)} RAM and full CPU comparisons')


if __name__ == '__main__':
    main()
