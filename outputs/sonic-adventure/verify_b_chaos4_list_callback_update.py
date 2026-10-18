"""Differentially verify B_CHAOS4 list updates including its recovered callback."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

OVERLAY_BASE, ADDRESS, END = 0x0c900000, 0x0c900aa4, 0x0c900afa
MAIN_BASE = 0x8c010000
RAM_SIZE = 16 * 1024 * 1024
STACK_GUARD = 0x2000
CALLBACK_ADDRESS = 0x8c10ca5c
CASE_COUNT = 128


def main():
    out = ROOT / 'outputs/sonic-adventure'
    overlay = (ROOT / 'work/sonic-adventure/decoded/SONICADV/B_CHAOS4.BIN').read_bytes()
    main_binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    offset = ADDRESS - OVERLAY_BASE
    assert len(overlay[offset:offset + END - ADDRESS]) == END - ADDRESS
    assert int.from_bytes(overlay[0x0ae8:0x0aec], 'little') == CALLBACK_ADDRESS

    dll_path = ROOT / 'work/sonic-adventure/build/b_chaos4_list_callback_update.dll'
    dll = ctypes.CDLL(str(dll_path))
    function = dll.sa_b_chaos4_list_callback_update_0c900aa4
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None

    original = bytearray(b'\xa5') * RAM_SIZE
    main_offset = (MAIN_BASE & 0x1fffffff) - 0x0c000000
    original[main_offset:main_offset + len(main_binary)] = main_binary
    original[OVERLAY_BASE - 0x0c000000:
             OVERLAY_BASE - 0x0c000000 + len(overlay)] = overlay

    rng = random.Random(0x424348344c495354)
    cases = []
    stack_changes = 0
    for case in range(CASE_COUNT):
        list_address = 0x0c300000 + case * 0x200
        node_base = 0x0c400000 + case * 0x1000
        child_base = 0x0c700000 + case * 0x1000
        object_base = 0x0c800000 + case * 0x2000
        node_count = 0 if case % 8 == 0 else rng.randrange(1, 9)
        baseline = bytearray(original)

        nested_objects = []
        for item in range(node_count):
            node = node_base + item * 0x20
            data = child_base + item * 0x40
            object_address = object_base + item * 0x100
            struct.pack_into('<I', baseline, list_address - 0x0c000000 + item * 4, node)
            struct.pack_into('<I', baseline, node - 0x0c000000 + 4,
                             data if (case + item) % 4 else 0)
            if (case + item) % 4:
                struct.pack_into('<I', baseline, data - 0x0c000000 + 0x10, object_address)
                struct.pack_into('<I', baseline, object_address - 0x0c000000 + 0x10,
                                 rng.getrandbits(32))
                nested_objects.append(object_address)
        struct.pack_into('<I', baseline, list_address - 0x0c000000 + node_count * 4, 0)
        retail_ram, actual_ram = bytearray(baseline), bytearray(baseline)

        initial = State()
        initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
        initial.r[4] = list_address
        initial.r[15] = 0x0cff0000
        initial.sr = rng.getrandbits(32)
        initial.pr = 0x0c001000 + case * 4
        initial.pc = ADDRESS
        expected = State.from_buffer_copy(bytes(initial))
        actual = State.from_buffer_copy(bytes(initial))

        steps = reference(retail_ram, expected)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        function(ram_view, ctypes.byref(actual))

        stack_start = initial.r[15] - 0x0c000000 - STACK_GUARD
        stack_end = initial.r[15] - 0x0c000000 + STACK_GUARD
        if retail_ram[:stack_start] != actual_ram[:stack_start]:
            first = next(i for i, (a, b) in enumerate(zip(retail_ram[:stack_start], actual_ram[:stack_start])) if a != b)
            raise AssertionError(f'RAM before stack differs in case {case} at {first + 0x0c000000:#010x}: retail={retail_ram[first]:02x}, native={actual_ram[first]:02x}')
        assert retail_ram[stack_end:] == actual_ram[stack_end:], f'RAM after stack differs in case {case}'
        stack_changes += sum(a != b for a, b in zip(retail_ram[stack_start:stack_end],
                                                    actual_ram[stack_start:stack_end]))
        assert list(actual.r[8:15]) == list(expected.r[8:15]), f'callee-saved GPR mismatch in case {case}'
        assert actual.r[15] == expected.r[15], f'SP mismatch in case {case}'
        assert actual.pr == expected.pr and actual.pc == expected.pc, f'call boundary mismatch in case {case}'
        cases.append({'case': case, 'list_entries': node_count,
                      'nested_objects': len(nested_objects), 'retail_instructions': steps,
                      'non_stack_ram_and_preserved_abi_match': True})

    source = out / 'source/b_chaos4_list_callback_update.c'
    helper_source = out / 'source/sa_signed_division.c'
    report = {
        'functions_verified': 1, 'total_abi_cases': len(cases),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'helper_source_sha256': hashlib.sha256(helper_source.read_bytes()).hexdigest(),
        'comparison_level': f'RAM compared across 16 MiB except +/-{STACK_GUARD:#x} caller-stack scratch; preserved R8-R14, SP, PR and return PC compared. Volatile registers, SR and stack scratch are excluded.',
        'stack_scratch_differences_observed': stack_changes,
        'scope': 'B_CHAOS4 list updater applies three mask constants and a signed-division callback result to nested objects in a zero-terminated pointer list.',
        'limitations': ['The callback at 0x8c10ca5c is linked to its separately recovered signed-division implementation; verification covers the integrated helper path.', 'Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or end-to-end boss gameplay.'],
        'dependencies': [{'module': 'B_CHAOS4.BIN', 'load_address': hex(OVERLAY_BASE),
                          'sha256': hashlib.sha256(overlay).hexdigest(),
                          'callback_literal_address': hex(0x0c900ae8),
                          'callback_target': hex(CALLBACK_ADDRESS),
                          'mask_add_address': hex(0x0c900aec),
                          'mask_clear_address': hex(0x0c900af4),
                          'mask_final_address': hex(0x0c900af0)},
                         {'module': '1ST_READ.BIN', 'load_address': hex(MAIN_BASE),
                          'sha256': hashlib.sha256(main_binary).hexdigest(),
                          'callback_source': 'source/sa_signed_division.c'}],
        'routines': [{'module': 'B_CHAOS4.BIN', 'load_address': hex(OVERLAY_BASE),
                      'address': hex(ADDRESS), 'end_exclusive': hex(END),
                      'name': 'list_callback_update',
                      'retail_span_sha256': hashlib.sha256(overlay[offset:offset + END - ADDRESS]).hexdigest(),
                      'cases_passed': len(cases), 'cases': cases}],
    }
    report_path = out / 'analysis/common-overlays/B_CHAOS4.BIN/list_callback_update_verification.json'
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"B_CHAOS4 list updater passed {len(cases)} RAM and ABI comparisons")


if __name__ == '__main__':
    main()
