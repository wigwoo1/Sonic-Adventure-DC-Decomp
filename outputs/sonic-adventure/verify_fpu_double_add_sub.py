"""Exercise PR=1 FADD/FSUB register pairs in the SH-4 test interpreter."""
import hashlib
import json
import random
import struct
from fractions import Fraction

from verify_fpu_cleanup_transfer import Fpu
from verify_startup import ROOT, State, reference
from sh4_binary64 import as_bits, as_float, pair_bits


def fpu_float(fpu, bank, register, value):
    bits = as_bits(value)
    fpu.banks[bank][register] = bits >> 32
    fpu.banks[bank][register + 1] = bits & 0xFFFFFFFF


def expected_result(a, b, subtract, mode):
    exact = Fraction(*a.as_integer_ratio()) + (
        -Fraction(*b.as_integer_ratio()) if subtract else Fraction(*b.as_integer_ratio()))
    nearest = float(exact)
    result = nearest
    if mode == 1 and Fraction(*nearest.as_integer_ratio()) != exact and abs(nearest) > abs(exact):
        bits = as_bits(nearest) - 1
        result = as_float(bits)
    return as_bits(result), Fraction(*result.as_integer_ratio()) != exact


def main():
    rng = random.Random(0x53483464)
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram_template = bytearray(16 * 1024 * 1024)
    ram_template[0x10000:0x10000 + len(binary)] = binary
    cases = 0
    for bank in range(2):
        for n in range(0, 16, 2):
            m = (n + 2) & 14
            if m == n:
                m = (n + 4) & 14
            for mode in range(2):
                for subtract in range(2):
                    for _ in range(32):
                        # Generate ordinary finite doubles away from overflow and underflow.
                        a = math_value(rng)
                        b = math_value(rng)
                        fpu = Fpu()
                        fpu.fpscr = 0x80000 | (bank << 21) | mode | 0x7C | 0x3F000
                        for index in range(16):
                            fpu.banks[bank][index] = rng.getrandbits(32)
                        fpu_float(fpu, bank, n, a)
                        fpu_float(fpu, bank, m, b)
                        before = bytes(fpu)
                        expected, inexact = expected_result(a, b, subtract, mode)
                        ram = bytearray(ram_template)
                        word = 0xF000 | (n << 8) | (m << 4) | int(subtract)
                        struct.pack_into('<3H', ram, 0x100, word, 0x000B, 0x0009)
                        state = State()
                        state.r[:] = [rng.getrandbits(32) for _ in range(16)]
                        state.r[15] = 0x8CF00000
                        state.pc = 0x8C000100
                        state.pr = 0x8C000080
                        state.sr = 0x700083F3
                        reference(ram, state, fpu=fpu)
                        assert pair_bits(fpu, bank, n) == expected
                        for index in range(16):
                            if index not in (n, n + 1):
                                assert fpu.banks[bank][index] == struct.unpack_from(
                                    '<I', before, bank * 64 + index * 4)[0]
                        assert fpu.fpscr == (0x80000 | (bank << 21) | mode | 0x7C |
                                             (0x1004 if inexact else 0))
                        assert state.sr == 0x700083F3 and state.r[15] == 0x8CF00000
                        cases += 1

    out = ROOT / 'outputs/sonic-adventure'
    report = {
        'cases_passed': cases,
        'implementation_sha256': hashlib.sha256((out / 'sh4_binary64.py').read_bytes()).hexdigest(),
        'instruction_dispatch_sha256': hashlib.sha256((out / 'sh4_binary32.py').read_bytes()).hexdigest(),
        'interpreter_sha256': hashlib.sha256((out / 'verify_startup.py').read_bytes()).hexdigest(),
        'manual': 'https://www.renesas.com/en/document/mas/sh-4-software-manual',
        'manual_revision': 'Rev. 6.00, 2006.09',
        'scope': 'Finite normal PR=1 DR-pair FADD/FSUB, both banks, all eight even DR registers, both rounding modes, FPSCR cause clearing/sticky flags, and unchanged CPU state. Exact rational sums provide the independent expected values.',
        'limitations': ['NaNs, infinities, denormal operands/results, overflow, enabled traps, odd DR encodings, and Dreamcast hardware behavior are not yet validated.'],
    }
    (out / 'analysis/fpu_double_add_sub_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'{cases} PR=1 double FADD/FSUB instruction checks passed')


def math_value(rng):
    sign = -1.0 if rng.getrandbits(1) else 1.0
    exponent = rng.uniform(-100.0, 100.0)
    mantissa = 0.5 + rng.random()
    return sign * mantissa * (2.0 ** exponent)


if __name__ == '__main__':
    main()
