"""Compare the 20 RTS/NOP leaf stubs with retail execution."""
import ctypes
import hashlib
import json
import random
import struct
from pathlib import Path

from verify_startup import ROOT, State, reference

ADDRESSES = [0x8c014b86, 0x8c02e720, 0x8c02e734, 0x8c034db2,
             0x8c047126, 0x8c60215e, 0x8c611708, 0x8c6224a0,
             0x8c63eeb0, 0x8c64241a, 0x8c645f80, 0x8c645f84,
             0x8c653440, 0x8c65b56a, 0x8c65c70c, 0x8c65ca8e,
             0x8c65ce02, 0x8c6608a0, 0x8c665a54, 0x8c666e02]
BASE, RAM_SIZE = 0x8c010000, 16 * 1024 * 1024


def main():
    out = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    lib = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/retail_empty_stubs.dll'))
    original = bytearray(b'\xa5') * RAM_SIZE
    original[0x10000:0x10000 + len(binary)] = binary
    rng = random.Random(0x5254534e4f50)
    results = []

    for address in ADDRESSES:
        offset = address - BASE
        assert struct.unpack_from('<HH', binary, offset) == (0x000b, 0x0009), hex(address)
        native = getattr(lib, f'sa_empty_stub_{address:08x}')
        native.argtypes = [ctypes.POINTER(State)]
        native.restype = None
        cases = []
        for case in range(16):
            expected = State()
            expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
            expected.sr = rng.getrandbits(32)
            expected.pr = (0x8c000100 + ((address + case * 4) & 0x7ffc)) & 0xffffffff
            expected.pc = address
            actual = State.from_buffer_copy(bytes(expected))
            retail_ram = bytearray(original)
            before_ram = bytes(retail_ram)
            steps = reference(retail_ram, expected)
            native(ctypes.byref(actual))
            assert steps == 2, f'{address:#x} executed {steps} steps'
            assert bytes(actual) == bytes(expected), f'CPU mismatch at {address:#x}, case {case}'
            assert bytes(retail_ram) == before_ram, f'RAM changed at {address:#x}, case {case}'
            assert expected.pc == expected.pr
            cases.append({'case': case, 'steps': steps, 'full_state_match': True})
        results.append({'address': hex(address), 'end_exclusive': hex(address + 4),
                        'retail_span_sha256': hashlib.sha256(binary[offset:offset + 4]).hexdigest(),
                        'cases_passed': len(cases), 'cases': cases})

    source = out / 'source/retail_empty_stubs.c'
    report = {'functions_verified': len(results),
              'total_full_state_cases': sum(item['cases_passed'] for item in results),
              'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
              'scope': 'Twenty indexed four-byte main-executable RTS/NOP leaf routines; 16 randomized complete GPR/SR/PR/PC comparisons and full 16 MiB RAM immutability checks per routine.',
              'limitations': ['These leaf stubs have no game-visible behavior in the indexed body; their callers and surrounding systems remain separately incomplete.',
                              'Verification uses the project’s partial SH-4 interpreter, not Dreamcast hardware or end-to-end game execution.'],
              'routines': results}
    (out / 'analysis/retail_empty_stubs_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"{len(results)} RTS/NOP routines passed {report['total_full_state_cases']} full-state comparisons")


if __name__ == '__main__':
    main()
