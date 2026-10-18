"""Verify the B_CHAOS4 initializer with its 1ST_READ.BIN template loaded."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

OVERLAY_BASE, ADDRESS, END = 0x0c900000, 0x0c900348, 0x0c9003e4
MAIN_BASE, TEMPLATE = 0x8c010000, 0x8c6b68a4
RAM_SIZE = 16 * 1024 * 1024


def main():
    out = ROOT / 'outputs/sonic-adventure'
    overlay = (ROOT / 'work/sonic-adventure/decoded/SONICADV/B_CHAOS4.BIN').read_bytes()
    main_binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    lib = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/b_chaos4_template_initialize.dll'))
    function = lib.sa_b_chaos4_template_initialize_0c900348
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None
    original = bytearray(b'\xa5') * RAM_SIZE
    main_ram_offset = (MAIN_BASE & 0x1fffffff) - 0x0c000000
    original[main_ram_offset:main_ram_offset + len(main_binary)] = main_binary
    original[OVERLAY_BASE - 0x0c000000:OVERLAY_BASE - 0x0c000000 + len(overlay)] = overlay
    offset = ADDRESS - OVERLAY_BASE
    assert len(overlay[offset:offset + END - ADDRESS]) == END - ADDRESS

    pointer_offset = 0x0c900404 - OVERLAY_BASE
    template_pointer = int.from_bytes(overlay[pointer_offset:pointer_offset + 4], 'little')
    assert template_pointer == TEMPLATE
    template_offset = (TEMPLATE & 0x1fffffff) - 0x0c000000
    template_file_offset = TEMPLATE - MAIN_BASE
    assert template_file_offset >= len(main_binary), 'The pointed-to table unexpectedly overlaps the extracted main binary'
    template_rng = random.Random(0x54454d504c415445)
    template = bytes(template_rng.getrandbits(8) for _ in range(57 * 4))
    original[template_offset:template_offset + len(template)] = template
    template_sha = hashlib.sha256(template).hexdigest()

    rng = random.Random(0x42434834494e4954)
    cases = []
    for case in range(128):
        context = 0x0c300000 + case * 0x200
        obj = 0x0c500000 + case * 0x400
        baseline = bytearray(original)
        for at in range(context - 0x0c000000, context - 0x0c000000 + 0x40, 4):
            baseline[at:at + 4] = rng.getrandbits(32).to_bytes(4, 'little')
        baseline[context - 0x0c000000 + 0x0c:context - 0x0c000000 + 0x14] = obj.to_bytes(4, 'little') + b'\xa5\xa5\xa5\xa5'
        for at in range(obj - 0x0c000000, obj - 0x0c000000 + 0x180, 4):
            baseline[at:at + 4] = rng.getrandbits(32).to_bytes(4, 'little')
        retail_ram, actual_ram = bytearray(baseline), bytearray(baseline)

        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.r[4] = context
        expected.r[15] = 0x0cff0000
        expected.sr = rng.getrandbits(32)
        expected.pr = 0x0c001000 + case * 4
        expected.pc = ADDRESS
        actual = State.from_buffer_copy(bytes(expected))
        steps = reference(retail_ram, expected)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        function(ram_view, ctypes.byref(actual))
        if retail_ram != actual_ram:
            mismatches = [i for i, (left, right) in enumerate(zip(retail_ram, actual_ram))
                          if left != right]
            raise AssertionError(f'RAM mismatch at case {case}; first addresses '
                                 f'{[hex(i + 0x0c000000) for i in mismatches[:24]]}; '
                                 f'object words retail={retail_ram[obj - 0x0c000000 + 8:obj - 0x0c000000 + 0x18].hex()} '
                                 f'actual={actual_ram[obj - 0x0c000000 + 8:obj - 0x0c000000 + 0x18].hex()}; '
                                 f'table={retail_ram[template_offset:template_offset + 24].hex()} '
                                 f'actual_table={actual_ram[template_offset:template_offset + 24].hex()} '
                                 f'pointer={retail_ram[0x900404:0x900408].hex()}')
        assert list(actual.r[8:16]) == list(expected.r[8:16]), (
            f'callee-saved CPU-state mismatch at case {case}')
        assert actual.pr == expected.pr and actual.pc == expected.pc, (
            f'call-boundary mismatch at case {case}')
        cases.append({'case': case, 'context': hex(context), 'object': hex(obj),
                      'retail_instructions': steps, 'full_ram_and_preserved_abi_match': True})

    source = out / 'source/b_chaos4_template_initialize.c'
    report = {
        'functions_verified': 1, 'total_abi_cases': len(cases),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'comparison_level': 'All 16 MiB RAM, preserved GPRs R8-R14, SP, PR and return PC compared.',
        'scope': 'B_CHAOS4 routine at 0x0c900348 copies seven three-word groups into an object using the pointer stored in the boss overlay. Full RAM and call-preservation state compared against retail with a deterministic table fixture.',
        'limitations': ['Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or end-to-end boss gameplay.', 'The table pointer targets 0x8c6b68a4, beyond the extracted 1ST_READ.BIN bytes; deterministic test data is injected there, and the retail runtime initialization of that memory remains unverified.'],
        'dependencies': [{'module': 'B_CHAOS4.BIN', 'load_address': hex(OVERLAY_BASE),
                          'sha256': hashlib.sha256(overlay).hexdigest()},
                         {'module': '1ST_READ.BIN', 'load_address': hex(MAIN_BASE),
                          'sha256': hashlib.sha256(main_binary).hexdigest(),
                          'template_pointer_address': hex(0x0c900404),
                          'template_address': hex(TEMPLATE), 'template_bytes': len(template),
                          'template_sha256': template_sha,
                          'template_fixture_hex': template.hex(),
                          'template_fixture_provenance': 'Deterministic verifier input; not asserted to be original retail runtime data.'}],
        'routines': [{'module': 'B_CHAOS4.BIN', 'load_address': hex(OVERLAY_BASE),
                      'address': hex(ADDRESS), 'end_exclusive': hex(END),
                      'name': 'template_initialize',
                      'retail_span_sha256': hashlib.sha256(overlay[offset:offset + END - ADDRESS]).hexdigest(),
                      'cases_passed': len(cases), 'cases': cases}],
    }
    report_path = out / 'analysis/common-overlays/B_CHAOS4.BIN/template_initialize_verification.json'
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"B_CHAOS4 template initializer passed {len(cases)} full-RAM and ABI comparisons")


if __name__ == '__main__':
    main()
