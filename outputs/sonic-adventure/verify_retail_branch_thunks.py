"""Verify short unconditional retail tail-branch wrappers."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

BASE, RAM_SIZE = 0x8c010000, 16 * 1024 * 1024
ADDRESSES = [0x8c09da40, 0x8c6021d2, 0x8c603bec, 0x8c603c24, 0x8c6453c0]


def main():
    out = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    lib = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/retail_branch_thunks.dll'))
    original = bytearray(b'\xa5') * RAM_SIZE
    original[0x10000:0x10000 + len(binary)] = binary
    rng = random.Random(0x4252414e43485448)
    routines = []

    for address in ADDRESSES:
        off = address - BASE
        branch, delay = struct.unpack_from('<HH', binary, off)
        assert branch >> 12 == 0xA and delay == 0x0009
        displacement = branch & 0x0fff
        if displacement & 0x0800:
            displacement -= 0x1000
        target = address + 4 + displacement * 2
        function = getattr(lib, f'sa_branch_thunk_{address:08x}')
        function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
        function.restype = None
        cases = []

        for case in range(16):
            expected = State()
            expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
            expected.sr = rng.getrandbits(32)
            expected.pr = rng.getrandbits(32)
            expected.pc = address
            actual = State.from_buffer_copy(bytes(expected))
            retail_ram, actual_ram = bytearray(original), bytearray(original)
            steps = reference(retail_ram, expected, stop_pc=target)
            ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
            function(ram_view, ctypes.byref(actual))
            assert steps == 2
            assert actual.pc == target, f'wrong tail target at {address:#x}'
            assert actual_ram == retail_ram, f'RAM mismatch at {address:#x}, case {case}'
            assert bytes(actual) == bytes(expected), f'CPU mismatch at {address:#x}, case {case}'
            cases.append({'case': case, 'tail_target': hex(target),
                          'full_state_and_ram_match': True})

        routines.append({'address': hex(address), 'end_exclusive': hex(address + 4),
                         'tail_target': hex(target),
                         'retail_span_sha256': hashlib.sha256(binary[off:off + 4]).hexdigest(),
                         'cases_passed': len(cases), 'cases': cases})

    source = out / 'source/retail_branch_thunks.c'
    report = {
        'functions_verified': len(routines),
        'total_full_state_cases': sum(item['cases_passed'] for item in routines),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'scope': 'Five four-byte 1ST_READ.BIN BRA/NOP tail thunks; branch targets were decoded from signed 12-bit displacements, and full CPU state plus 16 MiB RAM were compared over 16 cases per thunk.',
        'limitations': [
            'Target functions were not reverified by this thunk check; each target body requires its own routine verification.',
            'Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or end-to-end game execution.'
        ],
        'routines': routines,
    }
    (out / 'analysis/retail_branch_thunks_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"{len(routines)} branch thunks passed {report['total_full_state_cases']} full-state comparisons")


if __name__ == '__main__':
    main()
