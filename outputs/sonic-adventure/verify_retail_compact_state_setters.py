"""Verify two compact state setters against retail machine instructions."""
import ctypes
import hashlib
import json
import random
import struct
from pathlib import Path

from verify_startup import ROOT, State, reference

SPECS = [(0x8c062634, 12, 'sa_zero_two_globals_8c062634', 6),
         (0x8c5ff2de, 10, 'sa_zero_global_return_value_8c5ff2de', 5)]
BASE, RAM_SIZE = 0x8c010000, 16 * 1024 * 1024


def main():
    out = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    lib = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/retail_compact_state_setters.dll'))
    original = bytearray(b'\xa5') * RAM_SIZE
    original[0x10000:0x10000 + len(binary)] = binary
    rng = random.Random(0x434f4d50414354)
    results = []

    for address, size, symbol, expected_steps in SPECS:
        function = getattr(lib, symbol)
        function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
        function.restype = None
        cases = []
        for case in range(32):
            expected = State()
            expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
            expected.sr = rng.getrandbits(32)
            expected.pr = 0x8c000800 + case * 4
            expected.pc = address
            actual = State.from_buffer_copy(bytes(expected))
            retail_ram, actual_ram = bytearray(original), bytearray(original)
            steps = reference(retail_ram, expected)
            ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
            function(ram_view, ctypes.byref(actual))
            assert steps == expected_steps, (hex(address), steps)
            assert actual_ram == retail_ram, f'RAM mismatch at {address:#x}, case {case}'
            assert bytes(actual) == bytes(expected), f'CPU mismatch at {address:#x}, case {case}'
            cases.append({'case': case, 'retail_steps': steps,
                          'full_state_and_ram_match': True})
        offset = address - BASE
        results.append({'address': hex(address), 'end_exclusive': hex(address + size),
                        'retail_span_sha256': hashlib.sha256(
                            binary[offset:offset + size]).hexdigest(),
                        'cases_passed': len(cases), 'cases': cases})

    source = out / 'source/retail_compact_state_setters.c'
    report = {'functions_verified': len(results),
              'total_full_state_cases': sum(item['cases_passed'] for item in results),
              'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
              'scope': 'Two short global state setters, including ordered dual-pointer stores and an RTS delay-slot return value; 32 randomized complete machine-state and 16 MiB RAM comparisons each.',
              'limitations': ['These are isolated leaf routines; their broader callers and systems remain incomplete.',
                              'Comparison uses the project partial SH-4 interpreter and is not hardware or full-game verification.'],
              'routines': results}
    (out / 'analysis/retail_compact_state_setters_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"{len(results)} compact setters passed {report['total_full_state_cases']} full-state comparisons")


if __name__ == '__main__':
    main()
