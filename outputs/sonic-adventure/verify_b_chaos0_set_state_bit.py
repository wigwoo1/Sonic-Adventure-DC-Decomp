"""Differentially verify B_CHAOS0's state-byte bit setter."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

OVERLAY_BASE, ADDRESS, END = 0x0c900000, 0x0c904f40, 0x0c904f52
MAIN_BASE = 0x8c010000
RAM_BASE, RAM_SIZE = 0x0c000000, 16 * 1024 * 1024
STACK_GUARD, CASE_COUNT = 0x2000, 256


def offset(address):
    return (address & 0x1fffffff) - RAM_BASE


def main():
    out = ROOT / 'outputs/sonic-adventure'
    overlay = (ROOT / 'work/sonic-adventure/decoded/SONICADV/B_CHAOS0.BIN').read_bytes()
    main_binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    span_offset = ADDRESS - OVERLAY_BASE
    assert len(overlay[span_offset:span_offset + END - ADDRESS]) == END - ADDRESS

    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/b_chaos0_set_state_bit.dll'))
    function = library.sa_b_chaos0_set_state_bit_0c904f40
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None

    original = bytearray(b'\xa5') * RAM_SIZE
    main_offset = offset(MAIN_BASE)
    original[main_offset:main_offset + len(main_binary)] = main_binary
    overlay_offset = offset(OVERLAY_BASE)
    original[overlay_offset:overlay_offset + len(overlay)] = overlay

    rng = random.Random(0x4348414f53300040)
    cases = []
    stack_changes = 0
    for value in range(CASE_COUNT):
        baseline = bytearray(original)
        parameter = 0x0c300000 + value * 0x10
        area = bytes(rng.randrange(256) for _ in range(8))
        area = area[:3] + bytes([value]) + area[4:]
        baseline[offset(parameter):offset(parameter) + len(area)] = area
        retail_ram, native_ram = bytearray(baseline), bytearray(baseline)

        initial = State()
        initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
        initial.r[4] = parameter
        initial.r[15] = 0x0cff0000
        initial.sr = rng.getrandbits(32)
        initial.pr = 0x0c002800 + value * 4
        initial.pc = ADDRESS
        expected = State.from_buffer_copy(bytes(initial))
        actual = State.from_buffer_copy(bytes(initial))

        steps = reference(retail_ram, expected)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
        function(ram_view, ctypes.byref(actual))

        stack_start = initial.r[15] - RAM_BASE - STACK_GUARD
        stack_end = initial.r[15] - RAM_BASE + STACK_GUARD
        assert retail_ram[:stack_start] == native_ram[:stack_start], f'RAM before stack differs for byte {value:#04x}'
        assert retail_ram[stack_end:] == native_ram[stack_end:], f'RAM after stack differs for byte {value:#04x}'
        stack_changes += sum(a != b for a, b in zip(retail_ram[stack_start:stack_end],
                                                    native_ram[stack_start:stack_end]))
        assert list(actual.r[8:15]) == list(expected.r[8:15]), f'callee-saved registers differ for byte {value:#04x}'
        assert actual.r[15] == expected.r[15], f'SP differs for byte {value:#04x}'
        assert actual.sr == expected.sr, f'SR differs for byte {value:#04x}'
        assert actual.pr == expected.pr and actual.pc == expected.pc, f'call boundary differs for byte {value:#04x}'
        cases.append({'input': value, 'output': value | 1,
                      'retail_instructions': steps, 'ram_and_abi_match': True})

    source = out / 'source/b_chaos0_set_state_bit.c'
    report = {
        'functions_verified': 1, 'total_abi_cases': len(cases),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'comparison_level': f'All RAM except +/-{STACK_GUARD:#x} caller-stack scratch; SR, preserved R8-R14, SP, PR and return PC compared.',
        'stack_scratch_differences_observed': stack_changes,
        'scope': 'Sets bit 0 in the byte at parameter+3 and preserves the other seven bits and all surrounding RAM.',
        'limitations': ['Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or gameplay.',
                        'The routine is parameterized by a synthetic byte fixture; the overlay code and address are retail-derived.'],
        'dependencies': [
            {'module': 'B_CHAOS0.BIN', 'load_address': hex(OVERLAY_BASE),
             'sha256': hashlib.sha256(overlay).hexdigest()},
            {'module': '1ST_READ.BIN', 'load_address': hex(MAIN_BASE),
             'sha256': hashlib.sha256(main_binary).hexdigest()},
        ],
        'routines': [{
            'module': 'B_CHAOS0.BIN', 'load_address': hex(OVERLAY_BASE),
            'address': hex(ADDRESS), 'end_exclusive': hex(END),
            'name': 'set_state_bit',
            'retail_span_sha256': hashlib.sha256(overlay[span_offset:span_offset + END - ADDRESS]).hexdigest(),
            'cases_passed': len(cases), 'cases': cases,
        }],
    }
    report_path = out / 'analysis/common-overlays/B_CHAOS0.BIN/set_state_bit_verification.json'
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'B_CHAOS0 state-bit setter passed {len(cases)} RAM and ABI comparisons')


if __name__ == '__main__':
    main()
