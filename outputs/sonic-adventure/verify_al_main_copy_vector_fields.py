"""Differentially verify both AL_MAIN vector-field copy helpers."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

OVERLAY_BASE = 0x0c900000
MAIN_BASE = 0x8c010000
RAM_BASE, RAM_SIZE = 0x0c000000, 16 * 1024 * 1024
STACK_GUARD, CASE_COUNT = 0x2000, 128
FUNCTIONS = [
    ('sa_al_main_copy_vector_fields_a_0c90bab4', 0x0c90bab4, 0x0c90bac8, 0x8c, 0),
    ('sa_al_main_copy_vector_fields_b_0c90bac8', 0x0c90bac8, 0x0c90badc, 0xa4, 1),
]
SOURCE_BASE = 0x8c740000
DESTINATION_BASE = 0x8c760000


def offset(address):
    return (address & 0x1fffffff) - RAM_BASE


def main():
    out = ROOT / 'outputs/sonic-adventure'
    overlay = (ROOT / 'work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN').read_bytes()
    main_binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/al_main_copy_vector_fields.dll'))
    original = bytearray(b'\xa5') * RAM_SIZE
    for address, binary in ((MAIN_BASE, main_binary), (OVERLAY_BASE, overlay)):
        start = offset(address)
        original[start:start + len(binary)] = binary

    rng = random.Random(0x414c4d41494eba00)
    report_routines = []
    for name, address, end, destination_offset, destination_register in FUNCTIONS:
        span_offset = address - OVERLAY_BASE
        assert len(overlay[span_offset:span_offset + end - address]) == end - address
        function = getattr(library, name)
        function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
        function.restype = None
        cases = []
        for case in range(CASE_COUNT):
            baseline = bytearray(original)
            source = SOURCE_BASE + case * 0x2000
            destination = DESTINATION_BASE + case * 0x2000
            alias = None
            if case == 0:
                # The first destination word aliases the third source word.
                destination = source + 0x28 - destination_offset
                alias = 'destination_word_0_overwrites_source_word_2'
            elif case == 1:
                # The second destination word aliases the third source word.
                destination = source + 0x28 - destination_offset - 4
                alias = 'destination_word_1_overwrites_source_word_2'
            for delta in (0x20, 0x24, 0x28):
                baseline[offset(source + delta):offset(source + delta) + 4] = rng.getrandbits(32).to_bytes(4, 'little')
            retail_ram, native_ram = bytearray(baseline), bytearray(baseline)

            initial = State()
            initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
            initial.r[4] = source
            initial.r[5] = destination
            initial.r[15] = 0x0cff0000
            initial.sr = rng.getrandbits(32)
            initial.pr = 0x0c00b000 + case * 4
            initial.pc = address
            expected = State.from_buffer_copy(bytes(initial))
            actual = State.from_buffer_copy(bytes(initial))

            steps = reference(retail_ram, expected)
            ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
            function(ram_view, ctypes.byref(actual))

            stack_start = initial.r[15] - RAM_BASE - STACK_GUARD
            stack_end = initial.r[15] - RAM_BASE + STACK_GUARD
            assert retail_ram[:stack_start] == native_ram[:stack_start], f'RAM before caller stack differs: {name}, case {case}'
            assert retail_ram[stack_end:] == native_ram[stack_end:], f'RAM after caller stack differs: {name}, case {case}'
            assert list(actual.r) == list(expected.r), f'GPR state differs: {name}, case {case}'
            assert actual.sr == expected.sr, f'SR differs: {name}, case {case}'
            assert actual.pr == expected.pr and actual.pc == expected.pc, f'call boundary differs: {name}, case {case}'
            cases.append({'case': case, 'source': hex(source), 'destination': hex(destination),
                          'alias_case': alias, 'ram_and_full_cpu_state_match': True,
                          'retail_instructions': steps})

        report_routines.append({
            'module': 'AL_MAIN.BIN', 'load_address': hex(OVERLAY_BASE),
            'address': hex(address), 'end_exclusive': hex(end), 'name': name,
            'retail_span_sha256': hashlib.sha256(overlay[span_offset:span_offset + end - address]).hexdigest(),
            'destination_offset': hex(destination_offset), 'destination_register': destination_register,
            'cases_passed': len(cases), 'cases': cases,
        })

    source_path = out / 'source/al_main_copy_vector_fields.c'
    report = {
        'functions_verified': len(FUNCTIONS), 'total_abi_cases': CASE_COUNT * len(FUNCTIONS),
        'source_sha256': hashlib.sha256(source_path.read_bytes()).hexdigest(),
        'comparison_level': 'All RAM except caller-stack guard; all 16 GPRs, SR, PR and return PC compared.',
        'scope': 'Copies three 32-bit vector fields from source+0x20..+0x28 to destination+0x8c or destination+0xa4; preserves source-load/store order and the third RTS-delay-slot store.',
        'limitations': ['Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or AL_MAIN runtime.',
                        'Source and destination buffers are initialized in cloned RAM; retail instruction bytes remain unchanged.'],
        'dependencies': [
            {'module': 'AL_MAIN.BIN', 'load_address': hex(OVERLAY_BASE),
             'sha256': hashlib.sha256(overlay).hexdigest()},
            {'module': '1ST_READ.BIN', 'load_address': hex(MAIN_BASE),
             'sha256': hashlib.sha256(main_binary).hexdigest()},
        ],
        'routines': report_routines,
    }
    report_path = out / 'analysis/common-overlays/AL_MAIN.BIN/copy_vector_fields_verification.json'
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'AL_MAIN vector-field copies passed {CASE_COUNT} RAM/CPU cases each across {len(FUNCTIONS)} functions')


if __name__ == '__main__':
    main()
