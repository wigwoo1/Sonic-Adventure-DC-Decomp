"""Differentially verify B_CHAOS0's masked state-word update."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

OVERLAY_BASE, ADDRESS, END = 0x0c900000, 0x0c903400, 0x0c903426
MAIN_BASE = 0x8c010000
RAM_BASE, RAM_SIZE = 0x0c000000, 16 * 1024 * 1024
STACK_GUARD, CASE_COUNT = 0x2000, 256


def offset(address):
    return (address & 0x1fffffff) - RAM_BASE


def read32(ram, address):
    return struct.unpack_from('<I', ram, offset(address))[0]


def main():
    out = ROOT / 'outputs/sonic-adventure'
    overlay = (ROOT / 'work/sonic-adventure/decoded/SONICADV/B_CHAOS0.BIN').read_bytes()
    main_binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    span_offset = ADDRESS - OVERLAY_BASE
    assert len(overlay[span_offset:span_offset + END - ADDRESS]) == END - ADDRESS

    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/b_chaos0_state_word_mode.dll'))
    function = library.sa_b_chaos0_state_word_mode_0c903400
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None

    original = bytearray(b'\xa5') * RAM_SIZE
    main_offset = offset(MAIN_BASE)
    original[main_offset:main_offset + len(main_binary)] = main_binary
    overlay_ram_offset = offset(OVERLAY_BASE)
    original[overlay_ram_offset:overlay_ram_offset + len(overlay)] = overlay
    global_pointer = read32(original, 0x0c903500)
    nested_pointer = read32(original, global_pointer + 4)
    object_address = read32(original, nested_pointer + 0x10)
    field_address = object_address + 0x10
    assert global_pointer == 0x0c924db4 and nested_pointer == 0x0c924d8c
    assert object_address == 0x0c922ae4 and 0 <= offset(field_address) < RAM_SIZE

    rng = random.Random(0x4348414f53303400)
    cases = []
    stack_changes = 0
    for case in range(CASE_COUNT):
        baseline = bytearray(original)
        initial_word = rng.getrandbits(32)
        struct.pack_into('<I', baseline, offset(field_address), initial_word)
        parameter = rng.getrandbits(32) & 0xfffffffe | (case & 1)
        retail_ram, native_ram = bytearray(baseline), bytearray(baseline)

        initial = State()
        initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
        initial.r[4] = parameter
        initial.r[15] = 0x0cff0000
        initial.sr = rng.getrandbits(32)
        initial.pr = 0x0c003000 + case * 4
        initial.pc = ADDRESS
        expected, actual = State.from_buffer_copy(bytes(initial)), State.from_buffer_copy(bytes(initial))

        steps = reference(retail_ram, expected)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
        function(ram_view, ctypes.byref(actual))

        stack_start = initial.r[15] - RAM_BASE - STACK_GUARD
        stack_end = initial.r[15] - RAM_BASE + STACK_GUARD
        assert retail_ram[:stack_start] == native_ram[:stack_start], f'RAM before stack differs in case {case}'
        assert retail_ram[stack_end:] == native_ram[stack_end:], f'RAM after stack differs in case {case}'
        stack_changes += sum(a != b for a, b in zip(retail_ram[stack_start:stack_end],
                                                    native_ram[stack_start:stack_end]))
        assert list(actual.r[8:15]) == list(expected.r[8:15]), f'callee-saved registers differ in case {case}'
        assert actual.r[15] == expected.r[15], f'SP differs in case {case}'
        assert actual.sr == expected.sr, f'SR differs in case {case}'
        assert actual.pr == expected.pr and actual.pc == expected.pc, f'call boundary differs in case {case}'
        cases.append({'case': case, 'parameter': hex(parameter),
                      'initial_word': hex(initial_word),
                      'result_word': hex(read32(native_ram, field_address)),
                      'retail_instructions': steps, 'ram_and_abi_match': True})

    source = out / 'source/b_chaos0_state_word_mode.c'
    report = {
        'functions_verified': 1, 'total_abi_cases': len(cases),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'comparison_level': f'All RAM except +/-{STACK_GUARD:#x} caller-stack scratch; SR, preserved R8-R14, SP, PR and return PC compared.',
        'stack_scratch_differences_observed': stack_changes,
        'scope': 'When argument bit 0 is clear, stores the overlay constant at the selected state word; otherwise applies the retail mask and set-bit constants.',
        'limitations': ['Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or full gameplay.',
                        'The pointer chain and constants come from the extracted retail overlay; target field values and arguments are randomized.'],
        'dependencies': [
            {'module': 'B_CHAOS0.BIN', 'load_address': hex(OVERLAY_BASE),
             'sha256': hashlib.sha256(overlay).hexdigest(),
             'global_pointer_address': hex(0x0c903500),
             'object_address': hex(object_address), 'field_address': hex(field_address)},
            {'module': '1ST_READ.BIN', 'load_address': hex(MAIN_BASE),
             'sha256': hashlib.sha256(main_binary).hexdigest()},
        ],
        'routines': [{
            'module': 'B_CHAOS0.BIN', 'load_address': hex(OVERLAY_BASE),
            'address': hex(ADDRESS), 'end_exclusive': hex(END),
            'name': 'state_word_mode',
            'retail_span_sha256': hashlib.sha256(overlay[span_offset:span_offset + END - ADDRESS]).hexdigest(),
            'cases_passed': len(cases), 'cases': cases,
        }],
    }
    report_path = out / 'analysis/common-overlays/B_CHAOS0.BIN/state_word_mode_verification.json'
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'B_CHAOS0 state-word mode routine passed {len(cases)} RAM and ABI comparisons')


if __name__ == '__main__':
    main()
