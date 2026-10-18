"""Verify the literal-argument engine service tail transfer at 8c644d84."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ADDRESS, LENGTH = 0x8c644d84, 10
BASE, RAM_SIZE = 0x8c010000, 16 * 1024 * 1024


def put16(ram, address, value):
    struct.pack_into('<H', ram, address & 0xffffff, value & 0xffff)


def put32(ram, address, value):
    struct.pack_into('<I', ram, address & 0xffffff, value & 0xffffffff)


def main():
    project = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    original = bytearray(b'\xa5') * RAM_SIZE
    original[BASE & 0xffffff:(BASE & 0xffffff) + len(binary)] = binary
    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/sa_engine_service_tail_8c644d84.dll'))
    function = library.sa_engine_service_tail_8c644d84
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None
    rng = random.Random(0x8c644d84)
    cases = []

    for case in range(128):
        target = 0x8c5f0000 + case * 4
        return_pc = 0x8c00f000 + case * 4
        initial_ram = bytearray(original)
        put16(initial_ram, ADDRESS + 10, rng.getrandbits(16))
        put32(initial_ram, ADDRESS + 12, rng.getrandbits(32))
        put32(initial_ram, ADDRESS + 16, target)
        retail_ram, native_ram = bytearray(initial_ram), bytearray(initial_ram)

        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.sr = rng.getrandbits(32)
        expected.pr = return_pc
        expected.pc = ADDRESS
        actual = State.from_buffer_copy(bytes(expected))
        observed = []

        def target_service(_ram, regs, sr):
            observed.append((list(regs), sr, expected.pr))
            return sr

        steps = reference(retail_ram, expected, hooks={target: target_service})
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
        function(ram_view, ctypes.byref(actual))
        assert steps == 5, (case, steps)
        assert len(observed) == 1, (case, observed)
        expected_regs, expected_sr, expected_pr = observed[0]
        assert native_ram == retail_ram, f'RAM mismatch case {case}'
        assert list(actual.r) == expected_regs, (
            f'register mismatch case {case}: '
            f'{[hex(v) for v in actual.r]} != {[hex(v) for v in expected_regs]}')
        assert (actual.sr, actual.pr, actual.pc) == (expected_sr, expected_pr, target), (
            f'control-state mismatch case {case}')
        cases.append({'case': case, 'target': hex(target),
                      'signed_second_argument': hex(expected_regs[5]),
                      'retail_steps': steps, 'full_ram_and_tail_state_match': True})

    source = project / 'source/sa_engine_service_tail_8c644d84.c'
    report = {
        'module': '1ST_READ.BIN', 'address': hex(ADDRESS),
        'end_exclusive': hex(ADDRESS + LENGTH),
        'name': 'sa_engine_service_tail_8c644d84',
        'retail_span_sha256': hashlib.sha256(binary[ADDRESS - BASE:ADDRESS - BASE + LENGTH]).hexdigest(),
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cases_passed': len(cases),
        'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state at the service tail-transfer boundary.',
        'scope': '128 randomized literal-pool and CPU fixtures; the signed 16-bit argument, 32-bit argument, direct target, and preserved PR are checked.',
        'limitations': ['The function reached by the retail service target remains a separate unrecovered routine.'],
        'cases': cases,
    }
    (project / 'analysis/sa_engine_service_tail_8c644d84_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"Engine service tail wrapper passed {len(cases)} complete RAM/CPU comparisons")


if __name__ == '__main__':
    main()
