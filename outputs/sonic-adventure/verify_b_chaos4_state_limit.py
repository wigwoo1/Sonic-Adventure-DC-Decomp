"""Differentially verify the B_CHAOS4 single-precision state limit."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference
from verify_fpu_cleanup_transfer import Fpu

OVERLAY_BASE, ADDRESS, END = 0x0c900000, 0x0c901dd6, 0x0c901dec
MAIN_BASE = 0x8c010000
RAM_BASE, RAM_SIZE = 0x0c000000, 16 * 1024 * 1024
STACK_GUARD, CASE_COUNT = 0x2000, 128


def offset(address):
    return (address & 0x1fffffff) - RAM_BASE


def main():
    out = ROOT / 'outputs/sonic-adventure'
    overlay = (ROOT / 'work/sonic-adventure/decoded/SONICADV/B_CHAOS4.BIN').read_bytes()
    main_binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    span_offset = ADDRESS - OVERLAY_BASE
    assert len(overlay[span_offset:span_offset + END - ADDRESS]) == END - ADDRESS
    assert int.from_bytes(overlay[0x1ee8:0x1eec], 'little') == 0x41f00000

    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/b_chaos4_state_limit.dll'))
    function = library.sa_b_chaos4_state_limit_0c901dd6
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(Fpu), ctypes.POINTER(State)]
    function.restype = None

    original = bytearray(b'\xa5') * RAM_SIZE
    main_offset = offset(MAIN_BASE)
    original[main_offset:main_offset + len(main_binary)] = main_binary
    overlay_ram_offset = offset(OVERLAY_BASE)
    original[overlay_ram_offset:overlay_ram_offset + len(overlay)] = overlay
    pointer_address = int.from_bytes(overlay[0x1eec:0x1ef0], 'little')
    object_address = struct.unpack_from('<I', original, offset(pointer_address))[0]
    value_address = object_address + 0x10
    assert 0 <= offset(value_address) < RAM_SIZE

    values = [0, 0x80000000, 0x3f800000, 0xbf800000, 0x41efffff,
              0x41f00000, 0x41f00001, 0x7f800000, 0xff800000]
    rng = random.Random(0x4348414f53344444)
    cases = []
    stack_changes = 0
    for case in range(CASE_COUNT):
        baseline = bytearray(original)
        if case < len(values):
            current = values[case]
        else:
            # Finite normalized values only; denormal and NaN exception paths need separate modeling.
            sign = rng.getrandbits(1) << 31
            exponent = rng.randrange(1, 255)
            fraction = rng.getrandbits(23)
            current = sign | (exponent << 23) | fraction
        struct.pack_into('<I', baseline, offset(value_address), current)
        retail_ram, native_ram = bytearray(baseline), bytearray(baseline)

        initial = State()
        initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
        initial.r[15] = 0x0cff0000
        initial.sr = rng.getrandbits(32)
        initial.pr = 0x0c001800 + case * 4
        initial.pc = ADDRESS
        expected, actual = State.from_buffer_copy(bytes(initial)), State.from_buffer_copy(bytes(initial))

        expected_fpu = Fpu()
        expected_fpu.fpscr = 0x00040000 | ((case & 1) << 21)  # PR=0, SZ=0, both FR banks
        for bank in range(2):
            expected_fpu.banks[bank][:] = [rng.getrandbits(32) for _ in range(16)]
        actual_fpu = Fpu.from_buffer_copy(bytes(expected_fpu))

        steps = reference(retail_ram, expected, fpu=expected_fpu)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
        function(ram_view, ctypes.byref(actual_fpu), ctypes.byref(actual))

        stack_start = initial.r[15] - RAM_BASE - STACK_GUARD
        stack_end = initial.r[15] - RAM_BASE + STACK_GUARD
        assert retail_ram[:stack_start] == native_ram[:stack_start], f'RAM before stack differs in case {case}'
        assert retail_ram[stack_end:] == native_ram[stack_end:], f'RAM after stack differs in case {case}'
        stack_changes += sum(a != b for a, b in zip(retail_ram[stack_start:stack_end],
                                                    native_ram[stack_start:stack_end]))
        assert list(actual.r[8:15]) == list(expected.r[8:15]), f'callee-saved registers differ in case {case}'
        assert actual.r[15] == expected.r[15], f'SP differs in case {case}'
        assert actual.sr == expected.sr, f'SR differs in case {case}: initial={initial.sr:#010x} expected={expected.sr:#010x} actual={actual.sr:#010x} current={current:#010x}'
        assert actual.pr == expected.pr and actual.pc == expected.pc, f'call boundary differs in case {case}'
        if bytes(actual_fpu) != bytes(expected_fpu):
            changed = [(bank, reg, hex(actual_fpu.banks[bank][reg]), hex(expected_fpu.banks[bank][reg]))
                       for bank in range(2) for reg in range(16)
                       if actual_fpu.banks[bank][reg] != expected_fpu.banks[bank][reg]]
            raise AssertionError(f'FPU state differs in case {case}: actual_fpscr={actual_fpu.fpscr:#010x}, expected_fpscr={expected_fpu.fpscr:#010x}, changed={changed}')
        cases.append({'case': case, 'current_bits': hex(current),
                      'result_bits': hex(struct.unpack_from('<I', native_ram, offset(value_address))[0]),
                      'retail_instructions': steps, 'ram_cpu_fpu_match': True})

    source = out / 'source/b_chaos4_state_limit.c'
    fpu_header = out / 'source/fpu_cleanup_transfer.h'
    report = {
        'functions_verified': 1, 'total_abi_cases': len(cases),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'fpu_header_sha256': hashlib.sha256(fpu_header.read_bytes()).hexdigest(),
        'comparison_level': f'All RAM except +/-{STACK_GUARD:#x} caller-stack scratch; FPU banks/FPSCR, SR, preserved R8-R14, SP, PR and return PC compared.',
        'stack_scratch_differences_observed': stack_changes,
        'scope': 'B_CHAOS4 state-limit routine clamps the retail state float at 0x8c6b6efc to 30.0 in FPSCR.PR=0,SZ=0 mode.',
        'limitations': ['Verification covers PR=0,SZ=0 with normal finite values, signed zero and infinities; PR=1, SZ=1, NaNs, denormal exceptions, and Dreamcast hardware remain unverified.',
                        'RAM uses the extracted retail main-image pointer chain and original overlay constants.'],
        'dependencies': [
            {'module': 'B_CHAOS4.BIN', 'load_address': hex(OVERLAY_BASE),
             'sha256': hashlib.sha256(overlay).hexdigest(),
             'limit_address': hex(0x0c901ee8), 'object_pointer_address': hex(0x0c901eec)},
            {'module': '1ST_READ.BIN', 'load_address': hex(MAIN_BASE),
             'sha256': hashlib.sha256(main_binary).hexdigest(),
             'object_pointer_global': hex(pointer_address), 'object_address': hex(object_address)},
        ],
        'routines': [{
            'module': 'B_CHAOS4.BIN', 'load_address': hex(OVERLAY_BASE),
            'address': hex(ADDRESS), 'end_exclusive': hex(END),
            'name': 'state_limit', 'retail_span_sha256': hashlib.sha256(overlay[span_offset:span_offset + END - ADDRESS]).hexdigest(),
            'cases_passed': len(cases), 'cases': cases,
        }],
    }
    report_path = out / 'analysis/common-overlays/B_CHAOS4.BIN/state_limit_verification.json'
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'B_CHAOS4 state-limit routine passed {len(cases)} RAM/CPU/FPU comparisons')


if __name__ == '__main__':
    main()
