"""Verify selected multi-store and conditional globals against retail code."""
import ctypes
import hashlib
import json
import random
import struct
from pathlib import Path

from verify_startup import ROOT, State, reference

SPECS = [(0x8c64401c, 6, 'sa_global_setter_8c64401c', None),
         (0x8c606f14, 12, 'sa_dual_global_setter_8c606f14', None),
         (0x8c60ede6, 10, 'sa_dual_argument_global_setter_8c60ede6', None),
         (0x8c107ba0, 16, 'sa_conditional_global_setter_8c107ba0', 0x8c5fba64)]
BASE, RAM_SIZE = 0x8c010000, 16 * 1024 * 1024


def main():
    out = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    lib = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/retail_global_state_wrappers.dll'))
    original = bytearray(b'\xa5') * RAM_SIZE
    original[0x10000:0x10000 + len(binary)] = binary
    rng = random.Random(0x474c4f42414c)
    results = []

    for address, size, symbol, flag_address in SPECS:
        function = getattr(lib, symbol)
        function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
        function.restype = None
        cases = []
        for case in range(32):
            expected = State()
            expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
            expected.sr = rng.getrandbits(32)
            expected.pr = 0x8c000c00 + case * 4
            expected.pc = address
            actual = State.from_buffer_copy(bytes(expected))
            retail_ram, actual_ram = bytearray(original), bytearray(original)
            flag = None
            if flag_address is not None:
                flag = 0 if case % 2 == 0 else (case * 0x10203) & 0xffffffff or 1
                flag_index = (flag_address & 0x1fffffff) - 0x0c000000
                struct.pack_into('<I', retail_ram, flag_index, flag)
                struct.pack_into('<I', actual_ram, flag_index, flag)
            steps = reference(retail_ram, expected)
            ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
            function(ram_view, ctypes.byref(actual))
            if flag_address is None:
                assert steps == {0x8c64401c: 3, 0x8c606f14: 6, 0x8c60ede6: 5}[address]
            else:
                assert steps == (6 if flag == 0 else 8)
            assert actual_ram == retail_ram, f'RAM mismatch at {address:#x}, case {case}'
            assert bytes(actual) == bytes(expected), f'CPU mismatch at {address:#x}, case {case}'
            cases.append({'case': case, 'retail_steps': steps, 'flag': flag,
                          'full_state_and_ram_match': True})
        offset = address - BASE
        results.append({'address': hex(address), 'end_exclusive': hex(address + size),
                        'retail_span_sha256': hashlib.sha256(
                            binary[offset:offset + size]).hexdigest(),
                        'cases_passed': len(cases), 'cases': cases})

    source = out / 'source/retail_global_state_wrappers.c'
    report = {'functions_verified': len(results),
              'total_full_state_cases': sum(item['cases_passed'] for item in results),
              'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
              'scope': 'Three unconditional one/two-global argument setters and one conditional global setter; all retail opcodes compared in 32 randomized full GPR/SR/PR/PC and 16 MiB RAM cases each, with both conditional branch outcomes.',
              'limitations': ['These leaf state wrappers are recovered in isolation; caller systems remain incomplete.',
                              'Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
              'routines': results}
    (out / 'analysis/retail_global_state_wrappers_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"{len(results)} global-state wrappers passed {report['total_full_state_cases']} full-state comparisons")


if __name__ == '__main__':
    main()
