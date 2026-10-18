"""Exercise finite PR=1 FMUL/FDIV DR pairs against exact rational results."""
import hashlib
import json
import random
import struct
from fractions import Fraction

from sh4_binary64 import as_bits, as_float, pair_bits
from verify_fpu_cleanup_transfer import Fpu
from verify_startup import ROOT, State, reference


def set_pair(fpu, bank, register, value):
    bits = as_bits(value)
    fpu.banks[bank][register] = bits >> 32
    fpu.banks[bank][register + 1] = bits & 0xFFFFFFFF


def expected_bits(a, b, operation, mode):
    x, y = Fraction(*a.as_integer_ratio()), Fraction(*b.as_integer_ratio())
    exact = x * y if operation == 'multiply' else x / y
    nearest = float(exact)
    bits = as_bits(nearest)
    represented = Fraction(*nearest.as_integer_ratio()) if nearest else Fraction(0)
    inexact = represented != exact
    if mode == 1 and inexact and abs(represented) > abs(exact):
        bits -= 1
    return bits, inexact


def main():
    rng = random.Random(0x5348344D)
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram_template = bytearray(16 * 1024 * 1024)
    ram_template[0x10000:0x10000 + len(binary)] = binary
    cases = 0
    for bank in range(2):
        for n in range(0, 16, 2):
            for mode in range(2):
                for operation, opcode in (('multiply', 2), ('divide', 3)):
                    m = (n + 2) & 14
                    if m == n:
                        m = (n + 4) & 14
                    for _ in range(32):
                        a = rand_double(rng)
                        b = rand_double(rng)
                        fpu = Fpu()
                        fpu.fpscr = 0x80000 | (bank << 21) | mode | 0x7C | 0x3F000
                        for bnk in range(2):
                            for reg in range(16):
                                fpu.banks[bnk][reg] = rng.getrandbits(32)
                        set_pair(fpu, bank, n, a)
                        set_pair(fpu, bank, m, b)
                        before = bytes(fpu)
                        expected, inexact = expected_bits(a, b, operation, mode)
                        ram = bytearray(ram_template)
                        word = 0xF000 | (n << 8) | (m << 4) | opcode
                        struct.pack_into('<3H', ram, 0x100, word, 0x000B, 0x0009)
                        state = State()
                        state.r[:] = [rng.getrandbits(32) for _ in range(16)]
                        state.r[15] = 0x8CF00000
                        state.pc = 0x8C000100
                        state.pr = 0x8C000080
                        state.sr = 0x700083F3
                        reference(ram, state, fpu=fpu)
                        assert pair_bits(fpu, bank, n) == expected
                        for bnk in range(2):
                            for reg in range(16):
                                if bnk == bank and reg in (n, n + 1):
                                    continue
                                offset = bnk * 64 + reg * 4
                                assert fpu.banks[bnk][reg] == int.from_bytes(before[offset:offset + 4], 'little')
                        assert fpu.fpscr == (0x80000 | (bank << 21) | mode | 0x7C |
                                             (0x1004 if inexact else 0))
                        assert state.sr == 0x700083F3 and state.r[15] == 0x8CF00000
                        cases += 1

    out = ROOT / 'outputs/sonic-adventure'
    report = {
        'cases_passed': cases,
        'implementation_sha256': hashlib.sha256((out / 'sh4_binary64.py').read_bytes()).hexdigest(),
        'single_precision_dispatch_sha256': hashlib.sha256((out / 'sh4_binary32.py').read_bytes()).hexdigest(),
        'interpreter_sha256': hashlib.sha256((out / 'verify_startup.py').read_bytes()).hexdigest(),
        'manual': 'https://www.renesas.com/en/document/mas/sh-4-software-manual',
        'manual_revision': 'Rev. 6.00, 2006.09',
        'scope': 'Finite normal PR=1 DR-pair FMUL/FDIV, both banks, all eight even DR registers, both rounding modes, inexact cause/sticky flags, and unchanged CPU/unrelated register state. Exact rational products and quotients are the independent expected values.',
        'limitations': ['NaNs, infinities, denormals, overflow/traps, zero divisors, odd DR encodings, and Dreamcast hardware behavior are not validated.'],
    }
    (out / 'analysis/fpu_double_mul_div_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'{cases} PR=1 double FMUL/FDIV instruction checks passed')


def rand_double(rng):
    sign = -1.0 if rng.getrandbits(1) else 1.0
    exponent = rng.uniform(-100.0, 100.0)
    return sign * (0.5 + rng.random()) * (2.0 ** exponent)


if __name__ == '__main__':
    main()
