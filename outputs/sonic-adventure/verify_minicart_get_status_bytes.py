"""Differentially verify MINICART's null-safe signed status-byte getters."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

OVERLAY_BASE = 0x0c900000
MAIN_BASE = 0x8c010000
RAM_BASE, RAM_SIZE = 0x0c000000, 16 * 1024 * 1024
STACK_GUARD, CASE_COUNT = 0x2000, 128
GLOBAL_SLOT = 0x0c918114
OBJECT_BASE, STRING_BASE = 0x8c740000, 0x8c760000
FUNCTIONS = [
    ('sa_minicart_get_status_byte_0c90b0d0', 0x0c90b0d0, 0x0c90b0e2, 0),
    ('sa_minicart_get_status_byte_plus3_0c90b0e2', 0x0c90b0e2, 0x0c90b0f6, 3),
]


def offset(address):
    return (address & 0x1fffffff) - RAM_BASE


def main():
    out = ROOT / 'outputs/sonic-adventure'
    overlay = (ROOT / 'work/sonic-adventure/decoded/SONICADV/MINICART.BIN').read_bytes()
    main_binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    assert int.from_bytes(overlay[0xb108:0xb10c], 'little') == GLOBAL_SLOT

    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/minicart_get_status_bytes.dll'))
    original = bytearray(b'\xa5') * RAM_SIZE
    for address, binary in ((MAIN_BASE, main_binary), (OVERLAY_BASE, overlay)):
        start = offset(address)
        original[start:start + len(binary)] = binary

    rng = random.Random(0x4d494e4943415254)
    reports = []
    for name, address, end, byte_offset in FUNCTIONS:
        span_offset = address - OVERLAY_BASE
        assert len(overlay[span_offset:span_offset + end - address]) == end - address
        function = getattr(library, name)
        function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
        function.restype = None
        cases = []
        for case in range(CASE_COUNT):
            baseline = bytearray(original)
            has_object = case % 4 != 0
            object_address = OBJECT_BASE + case * 0x100
            string_address = STRING_BASE + case * 0x100
            global_value = object_address if has_object else 0
            baseline[offset(GLOBAL_SLOT):offset(GLOBAL_SLOT) + 4] = global_value.to_bytes(4, 'little')
            input_value = [0, 0x7f, 0x80, 0xff, 0x03, 0xfd][case] if case < 6 else rng.randrange(256)
            if has_object:
                baseline[offset(object_address) + 0x24:offset(object_address) + 0x28] = string_address.to_bytes(4, 'little')
                baseline[offset(string_address) + byte_offset] = input_value
            retail_ram, native_ram = bytearray(baseline), bytearray(baseline)

            initial = State()
            initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
            initial.r[15] = 0x0cff0000
            initial.sr = rng.getrandbits(32)
            initial.pr = 0x0c00c000 + case * 4
            initial.pc = address
            expected = State.from_buffer_copy(bytes(initial))
            actual = State.from_buffer_copy(bytes(initial))

            steps = reference(retail_ram, expected)
            ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
            function(ram_view, ctypes.byref(actual))

            stack_start = initial.r[15] - RAM_BASE - STACK_GUARD
            stack_end = initial.r[15] - RAM_BASE + STACK_GUARD
            assert retail_ram[:stack_start] == native_ram[:stack_start], f'RAM before stack differs: {name}, case {case}'
            assert retail_ram[stack_end:] == native_ram[stack_end:], f'RAM after stack differs: {name}, case {case}'
            assert list(actual.r) == list(expected.r), f'GPR state differs: {name}, case {case}'
            assert actual.sr == expected.sr, f'SR differs: {name}, case {case}'
            assert actual.pr == expected.pr and actual.pc == expected.pc, f'call boundary differs: {name}, case {case}'
            cases.append({'case': case, 'global_pointer_null': not has_object,
                          'status_byte': hex(input_value) if has_object else None,
                          'ram_and_full_cpu_state_match': True, 'retail_instructions': steps})

        reports.append({
            'module': 'MINICART.BIN', 'load_address': hex(OVERLAY_BASE),
            'address': hex(address), 'end_exclusive': hex(end), 'name': name,
            'retail_span_sha256': hashlib.sha256(overlay[span_offset:span_offset + end - address]).hexdigest(),
            'byte_offset': byte_offset, 'cases_passed': len(cases), 'cases': cases,
        })

    source_path = out / 'source/minicart_get_status_bytes.c'
    report = {
        'functions_verified': len(FUNCTIONS), 'total_abi_cases': CASE_COUNT * len(FUNCTIONS),
        'source_sha256': hashlib.sha256(source_path.read_bytes()).hexdigest(),
        'comparison_level': 'All RAM except caller-stack guard; all 16 GPRs, SR, PR and return PC compared.',
        'scope': 'Both getters load the pointer from the MINICART literal slot, return zero for a null global pointer, otherwise follow object+0x24 and return a sign-extended byte at string offsets 0 and 3; pointer/register and T-bit effects are preserved.',
        'limitations': ['Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or MINICART runtime.',
                        'For nonnull global pointers, the object+0x24 string pointer is initialized to valid RAM; null inner pointers are not a valid game object and are not tested.',
                        'Pointer slot and returned bytes are test-initialized in cloned RAM; retail instruction bytes remain unchanged.'],
        'dependencies': [
            {'module': 'MINICART.BIN', 'load_address': hex(OVERLAY_BASE),
             'sha256': hashlib.sha256(overlay).hexdigest(),
             'pointer_literal_address': hex(0x0c90b108), 'global_pointer_slot': hex(GLOBAL_SLOT)},
            {'module': '1ST_READ.BIN', 'load_address': hex(MAIN_BASE),
             'sha256': hashlib.sha256(main_binary).hexdigest()},
        ],
        'routines': reports,
    }
    report_path = out / 'analysis/common-overlays/MINICART.BIN/get_status_bytes_verification.json'
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'MINICART status-byte getters passed {CASE_COUNT} RAM/CPU cases each across {len(FUNCTIONS)} functions')


if __name__ == '__main__':
    main()
