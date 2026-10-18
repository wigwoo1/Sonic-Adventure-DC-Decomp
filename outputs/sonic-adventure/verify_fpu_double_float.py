"""Exercise PR=1 FLOAT FPUL,DRn across the signed int32 domain."""
import hashlib
import json
import random
import struct

from sh4_binary64 import as_bits, pair_bits
from verify_fpu_cleanup_transfer import Fpu
from verify_startup import ROOT, State, reference


def main():
    rng = random.Random(0x53483446)
    values = [0, 1, -1, 0x7FFFFFFF, -0x80000000, 0x01000001,
              -0x01000001, 0x40000001, -0x40000001]
    values += [rng.randrange(-0x80000000, 0x80000000) for _ in range(256)]
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram_template = bytearray(16 * 1024 * 1024)
    ram_template[0x10000:0x10000 + len(binary)] = binary
    cases = 0
    for bank in range(2):
        for n in range(0, 16, 2):
            for mode in range(2):
                for integer in values:
                    fpu = Fpu()
                    fpu.fpscr = 0x80000 | (bank << 21) | mode | 0x7C | 0x3F000
                    fpu.fpul = integer & 0xFFFFFFFF
                    for bnk in range(2):
                        for reg in range(16):
                            fpu.banks[bnk][reg] = rng.getrandbits(32)
                    before = bytes(fpu)
                    ram = bytearray(ram_template)
                    word = 0xF02D | (n << 8)
                    struct.pack_into('<3H', ram, 0x100, word, 0x000B, 0x0009)
                    state = State()
                    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
                    state.r[15] = 0x8CF00000
                    state.pc = 0x8C000100
                    state.pr = 0x8C000080
                    state.sr = 0x700083F3
                    reference(ram, state, fpu=fpu)
                    expected = as_bits(float(integer))
                    assert pair_bits(fpu, bank, n) == expected
                    for bnk in range(2):
                        for reg in range(16):
                            if bnk == bank and reg in (n, n + 1):
                                continue
                            offset = bnk * 64 + reg * 4
                            assert fpu.banks[bnk][reg] == int.from_bytes(before[offset:offset + 4], 'little')
                    assert fpu.fpul == (integer & 0xFFFFFFFF)
                    assert fpu.fpscr == (0x80000 | (bank << 21) | mode | 0x7C)
                    assert state.sr == 0x700083F3 and state.r[15] == 0x8CF00000
                    cases += 1

    out = ROOT / 'outputs/sonic-adventure'
    report = {
        'cases_passed': cases,
        'implementation_sha256': hashlib.sha256((out / 'sh4_binary64.py').read_bytes()).hexdigest(),
        'dispatch_sha256': hashlib.sha256((out / 'sh4_binary32.py').read_bytes()).hexdigest(),
        'interpreter_sha256': hashlib.sha256((out / 'verify_startup.py').read_bytes()).hexdigest(),
        'manual': 'https://www.renesas.com/en/document/mas/sh-4-software-manual',
        'manual_revision': 'Rev. 6.00, 2006.09',
        'scope': 'PR=1 signed int32 FLOAT to DRn; both banks, all even DR registers, both rounding modes, all critical signed boundaries plus randomized values, exact result bits, cleared cause bits, and preserved other state.',
        'limitations': ['FPU exception behavior, disabled-FPU state, Dreamcast hardware, and the formatter helper that consumes the conversion remain unverified.'],
    }
    (out / 'analysis/fpu_double_float_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'{cases} PR=1 double FLOAT instruction checks passed')


if __name__ == '__main__':
    main()
