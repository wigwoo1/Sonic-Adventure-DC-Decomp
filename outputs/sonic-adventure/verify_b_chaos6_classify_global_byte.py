"""Differentially verify B_CHAOS6's global signed-byte classifier."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

OVERLAY_BASE, ADDRESS, END = 0x0c900000, 0x0c902f18, 0x0c902f40
MAIN_BASE = 0x8c010000
RAM_BASE, RAM_SIZE = 0x0c000000, 16 * 1024 * 1024
STACK_GUARD, CASE_COUNT = 0x2000, 128
POINTER_SLOT, BYTE_TABLE = 0x8c6b6884, 0x8c020000


def offset(address):
    return (address & 0x1fffffff) - RAM_BASE


def main():
    out = ROOT / 'outputs/sonic-adventure'
    overlay = (ROOT / 'work/sonic-adventure/decoded/SONICADV/B_CHAOS6.BIN').read_bytes()
    main_binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    span_offset = ADDRESS - OVERLAY_BASE
    assert len(overlay[span_offset:span_offset + END - ADDRESS]) == END - ADDRESS
    pointer_literal = int.from_bytes(overlay[0x3014:0x3018], 'little')
    assert pointer_literal == POINTER_SLOT

    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/b_chaos6_classify_global_byte.dll'))
    function = library.sa_b_chaos6_classify_global_byte_0c902f18
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None

    original = bytearray(b'\xa5') * RAM_SIZE
    for address, binary in ((MAIN_BASE, main_binary), (OVERLAY_BASE, overlay)):
        start = offset(address)
        original[start:start + len(binary)] = binary

    rng = random.Random(0x4348414f53362f18)
    values = [0, 1, 2, 3, 0x13, 4, 0x7f, 0x80, 0xff]
    cases = []
    for case in range(CASE_COUNT):
        baseline = bytearray(original)
        initial_value = values[case] if case < len(values) else rng.randrange(256)
        struct.pack_into('<I', baseline, offset(POINTER_SLOT), BYTE_TABLE)
        baseline[offset(BYTE_TABLE)] = initial_value
        retail_ram, native_ram = bytearray(baseline), bytearray(baseline)

        initial = State()
        initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
        initial.r[15] = 0x0cff0000
        initial.sr = (rng.getrandbits(32) & ~1) | (case & 1)
        initial.pr = 0x0c004000 + case * 4
        initial.pc = ADDRESS
        expected = State.from_buffer_copy(bytes(initial))
        actual = State.from_buffer_copy(bytes(initial))

        steps = reference(retail_ram, expected)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
        function(ram_view, ctypes.byref(actual))

        assert retail_ram == native_ram, f'RAM differs in case {case} (unexpected write by classifier)'
        assert list(actual.r) == list(expected.r), f'GPR state differs in case {case}, value={initial_value:#x}'
        assert actual.sr == expected.sr, f'SR differs in case {case}, value={initial_value:#x}'
        assert actual.pr == expected.pr and actual.pc == expected.pc, f'call boundary differs in case {case}'
        cases.append({'case': case, 'initial_byte': hex(initial_value),
                      'retail_instructions': steps, 'ram_and_full_cpu_state_match': True})

    source = out / 'source/b_chaos6_classify_global_byte.c'
    report = {
        'functions_verified': 1, 'total_abi_cases': len(cases),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'comparison_level': 'All 16 MiB RAM; all 16 GPRs, SR, PR and return PC compared.',
        'scope': 'Loads the global pointer from the B_CHAOS6 literal pool, sign-extends its pointed byte, classifies values 1, 2, 0x13 and 3 as zero and all other signed-byte values as one, preserving retail register and T-bit effects.',
        'limitations': ['Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or gameplay.',
                        'The global pointer slot and pointed byte are initialized in cloned RAM; retail instruction bytes remain unchanged.'],
        'dependencies': [
            {'module': 'B_CHAOS6.BIN', 'load_address': hex(OVERLAY_BASE),
             'sha256': hashlib.sha256(overlay).hexdigest(),
             'pointer_literal_address': hex(0x0c903014), 'pointer_slot_address': hex(POINTER_SLOT)},
            {'module': '1ST_READ.BIN', 'load_address': hex(MAIN_BASE),
             'sha256': hashlib.sha256(main_binary).hexdigest()},
        ],
        'routines': [{
            'module': 'B_CHAOS6.BIN', 'load_address': hex(OVERLAY_BASE),
            'address': hex(ADDRESS), 'end_exclusive': hex(END),
            'name': 'classify_global_signed_byte',
            'retail_span_sha256': hashlib.sha256(overlay[span_offset:span_offset + END - ADDRESS]).hexdigest(),
            'cases_passed': len(cases), 'cases': cases,
        }],
    }
    report_path = out / 'analysis/common-overlays/B_CHAOS6.BIN/classify_global_byte_verification.json'
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'B_CHAOS6 global-byte classifier passed {len(cases)} RAM and full CPU comparisons')


if __name__ == '__main__':
    main()
