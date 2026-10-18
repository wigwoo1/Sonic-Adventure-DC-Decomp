"""Differentially verify the B_CHAOS4 constant-table copier."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

OVERLAY_BASE, ADDRESS, END = 0x0c900000, 0x0c900348, 0x0c9003e4
MAIN_BASE = 0x8c010000
RAM_BASE, RAM_SIZE = 0x0c000000, 16 * 1024 * 1024
STACK_GUARD = 0x2000
CASE_COUNT = 128
SOURCE_INDICES = (0, 1, 2, 9, 10, 11, 0x1b, 0x1c, 0x1d, 0x12, 0x13,
                  0x14, 0x2d, 0x2e, 0x2f, 0x24, 0x25, 0x26, 0x36, 0x37, 0x38)
DESTINATION_OFFSETS = (0x08, 0x0c, 0x10, 0x3c, 0x40, 0x44, 0xa4, 0xa8, 0xac,
                       0x70, 0x74, 0x78, 0x10c, 0x110, 0x114, 0xd8, 0xdc,
                       0xe0, 0x140, 0x144, 0x148)


def ram_offset(address):
    return (address & 0x1fffffff) - RAM_BASE


def main():
    out = ROOT / 'outputs/sonic-adventure'
    overlay = (ROOT / 'work/sonic-adventure/decoded/SONICADV/B_CHAOS4.BIN').read_bytes()
    main_binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    overlay_offset = ADDRESS - OVERLAY_BASE
    assert len(overlay[overlay_offset:overlay_offset + END - ADDRESS]) == END - ADDRESS
    source_address = int.from_bytes(overlay[0x404:0x408], 'little')
    source_table_offset = ram_offset(source_address)
    assert 0 <= source_table_offset <= RAM_SIZE - 0x39 * 4
    assert source_table_offset >= 0x10000

    dll_path = ROOT / 'work/sonic-adventure/build/b_chaos4_template_copy.dll'
    dll = ctypes.CDLL(str(dll_path))
    function = dll.sa_b_chaos4_template_copy_0c900348
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None

    original = bytearray(b'\xa5') * RAM_SIZE
    main_offset = ram_offset(MAIN_BASE)
    original[main_offset:main_offset + len(main_binary)] = main_binary
    overlay_ram_offset = ram_offset(OVERLAY_BASE)
    original[overlay_ram_offset:overlay_ram_offset + len(overlay)] = overlay

    rng = random.Random(0x4243483454454d50)
    cases = []
    stack_changes = 0
    for case in range(CASE_COUNT):
        baseline = bytearray(original)
        if case != 0:
            source_words = [rng.getrandbits(32) for _ in range(0x39)]
            for index, value in enumerate(source_words):
                struct.pack_into('<I', baseline, source_table_offset + index * 4, value)

        parameter = 0x0c300000 + case * 0x200
        destination = 0x0c400000 + case * 0x1000
        struct.pack_into('<I', baseline, ram_offset(parameter + 0x0c), destination)
        retail_ram, actual_ram = bytearray(baseline), bytearray(baseline)

        initial = State()
        initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
        initial.r[4] = parameter
        initial.r[15] = 0x0cff0000
        initial.sr = rng.getrandbits(32)
        initial.pr = 0x0c001000 + case * 4
        initial.pc = ADDRESS
        expected = State.from_buffer_copy(bytes(initial))
        actual = State.from_buffer_copy(bytes(initial))

        steps = reference(retail_ram, expected)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        function(ram_view, ctypes.byref(actual))

        stack_start = initial.r[15] - RAM_BASE - STACK_GUARD
        stack_end = initial.r[15] - RAM_BASE + STACK_GUARD
        assert retail_ram[:stack_start] == actual_ram[:stack_start], f'RAM before stack differs in case {case}'
        assert retail_ram[stack_end:] == actual_ram[stack_end:], f'RAM after stack differs in case {case}'
        stack_changes += sum(a != b for a, b in zip(retail_ram[stack_start:stack_end],
                                                    actual_ram[stack_start:stack_end]))
        assert list(actual.r[8:15]) == list(expected.r[8:15]), f'callee-saved registers differ in case {case}'
        assert actual.r[15] == expected.r[15], f'SP differs in case {case}'
        assert actual.sr == expected.sr, f'SR differs in case {case}'
        assert actual.pr == expected.pr and actual.pc == expected.pc, f'call boundary differs in case {case}'
        cases.append({'case': case, 'retail_instructions': steps,
                      'copied_words': len(SOURCE_INDICES), 'ram_and_abi_match': True})

    source = out / 'source/b_chaos4_template_copy.c'
    report = {
        'functions_verified': 1,
        'total_abi_cases': len(cases),
        'actual_retail_table_cases': 1,
        'randomized_table_cases': CASE_COUNT - 1,
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'comparison_level': f'All RAM compared except +/-{STACK_GUARD:#x} caller-stack scratch; SR, preserved R8-R14, SP, PR, and return PC compared.',
        'stack_scratch_differences_observed': stack_changes,
        'scope': 'Copies 21 selected words from the B_CHAOS4 template table referenced at 0x0c900404 into the object referenced at parameter+0x0c.',
        'limitations': ['Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or end-to-end boss gameplay.',
                        '127 cases randomize the table to stress every copy offset; one case uses the extracted retail table unchanged.'],
        'dependencies': [
            {'module': 'B_CHAOS4.BIN', 'load_address': hex(OVERLAY_BASE),
             'sha256': hashlib.sha256(overlay).hexdigest(),
             'source_pointer_address': hex(0x0c900404), 'source_table_address': hex(source_address)},
            {'module': '1ST_READ.BIN', 'load_address': hex(MAIN_BASE),
             'sha256': hashlib.sha256(main_binary).hexdigest()},
        ],
        'routines': [{
            'module': 'B_CHAOS4.BIN', 'load_address': hex(OVERLAY_BASE),
            'address': hex(ADDRESS), 'end_exclusive': hex(END),
            'name': 'template_copy',
            'retail_span_sha256': hashlib.sha256(overlay[overlay_offset:overlay_offset + END - ADDRESS]).hexdigest(),
            'cases_passed': len(cases), 'cases': cases,
        }],
    }
    report_path = out / 'analysis/common-overlays/B_CHAOS4.BIN/template_copy_verification.json'
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'B_CHAOS4 template copier passed {len(cases)} RAM and ABI comparisons')


if __name__ == '__main__':
    main()
