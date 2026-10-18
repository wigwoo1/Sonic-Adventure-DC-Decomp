"""Verify selected one-store global setter wrappers against retail execution."""
import ctypes
import hashlib
import json
import random
from pathlib import Path

from verify_startup import ROOT, State, reference

ENTRIES = [(0x8c041ea4, 6), (0x8c0585dc, 6), (0x8c05f01c, 6),
           (0x8c09b4aa, 6), (0x8c109ce0, 6), (0x8c10d39c, 6),
           (0x8c60d1f2, 6), (0x8c60d1f8, 6), (0x8c60ede0, 6),
           (0x8c640766, 6), (0x8c647534, 8), (0x8c657960, 8)]
BASE, RAM_SIZE = 0x8c010000, 16 * 1024 * 1024


def main():
    out = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    lib = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/global_setter_wrappers.dll'))
    original = bytearray(b'\xa5') * RAM_SIZE
    original[0x10000:0x10000 + len(binary)] = binary
    rng = random.Random(0x534554544552)
    results = []

    for address, size in ENTRIES:
        native = getattr(lib, f'sa_global_setter_{address:08x}')
        native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
        native.restype = None
        cases = []
        for case in range(32):
            expected = State()
            expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
            expected.sr = rng.getrandbits(32)
            expected.pr = 0x8c000100 + case * 4
            expected.pc = address
            actual = State.from_buffer_copy(bytes(expected))
            retail_ram, actual_ram = bytearray(original), bytearray(original)
            steps = reference(retail_ram, expected)
            ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
            native(ram_view, ctypes.byref(actual))
            assert steps == (3 if size == 6 else 4), (hex(address), steps)
            assert actual_ram == retail_ram, f'RAM mismatch at {address:#x}, case {case}'
            assert bytes(actual) == bytes(expected), f'CPU mismatch at {address:#x}, case {case}'
            cases.append({'case': case, 'retail_steps': steps,
                          'full_state_and_ram_match': True})
        span = binary[address - BASE:address - BASE + size]
        results.append({'address': hex(address), 'end_exclusive': hex(address + size),
                        'retail_span_sha256': hashlib.sha256(span).hexdigest(),
                        'cases_passed': len(cases), 'cases': cases})

    source = out / 'source/global_setter_wrappers.c'
    report = {'functions_verified': len(results),
              'total_full_state_cases': sum(item['cases_passed'] for item in results),
              'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
              'scope': 'Ten six-byte and two eight-byte indexed global-setter functions; all retail instructions interpreted and compared against the native wrapper in 32 randomized complete machine-state and 16 MiB RAM cases per function.',
              'limitations': ['Only the listed leaf setters are recovered; their owning systems and uses remain incomplete.',
                              'The project SH-4 interpreter is a subset and this does not establish hardware or whole-game correctness.'],
              'routines': results}
    (out / 'analysis/global_setter_wrappers_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"{len(results)} global setters passed {report['total_full_state_cases']} full-state comparisons")


if __name__ == '__main__':
    main()
