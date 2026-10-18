"""Exercise finite PR=1 FCMP/DR instructions in the SH-4 test interpreter."""
import hashlib
import json
import random

from sh4_binary64 import as_bits, pair_bits
from verify_fpu_cleanup_transfer import Fpu
from verify_startup import ROOT, State, reference


def set_pair(fpu, bank, register, value):
    bits = as_bits(value)
    fpu.banks[bank][register] = bits >> 32
    fpu.banks[bank][register + 1] = bits & 0xFFFFFFFF


def main():
    rng = random.Random(0x53483443)
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram_template = bytearray(16 * 1024 * 1024)
    ram_template[0x10000:0x10000 + len(binary)] = binary
    values = [0.0, -0.0, 1.0, -1.0, 17.25, -8.5, 2.0 ** 80, -(2.0 ** -80)]
    cases = 0
    for bank in range(2):
        for n in range(0, 16, 2):
            for m in range(0, 16, 2):
                for opcode in (4, 5):
                    for seed in range(4):
                        a = values[(n // 2 + seed) % len(values)]
                        b = values[(m // 2 + seed * 3) % len(values)]
                        if n == m:
                            b = a
                        fpu = Fpu()
                        fpu.fpscr = 0x80000 | (bank << 21) | 1 | 0x7C | 0x3F000
                        for bnk in range(2):
                            for reg in range(16):
                                fpu.banks[bnk][reg] = rng.getrandbits(32)
                        set_pair(fpu, bank, n, a)
                        set_pair(fpu, bank, m, b)
                        before = bytes(fpu)
                        expected_t = (a == b) if opcode == 4 else (a > b)
                        ram = bytearray(ram_template)
                        word = 0xF000 | (n << 8) | (m << 4) | opcode
                        import struct
                        struct.pack_into('<3H', ram, 0x100, word, 0x000B, 0x0009)
                        state = State()
                        state.r[:] = [rng.getrandbits(32) for _ in range(16)]
                        state.r[15] = 0x8CF00000
                        state.pc = 0x8C000100
                        state.pr = 0x8C000080
                        state.sr = 0x700083F2 | rng.randrange(2)
                        initial_sr = state.sr
                        reference(ram, state, fpu=fpu)
                        assert state.sr == ((initial_sr & ~1) | int(expected_t)), (
                            bank, n, m, opcode, a, b, expected_t, hex(state.sr), hex(initial_sr))
                        assert fpu.fpscr == ((0x80000 | (bank << 21) | 1 | 0x7C))
                        for bnk in range(2):
                            for reg in range(16):
                                if bnk == bank and reg in (n, n + 1, m, m + 1):
                                    continue
                                offset = bnk * 64 + reg * 4
                                assert fpu.banks[bnk][reg] == int.from_bytes(before[offset:offset + 4], 'little')
                        pair_offset = bank * 64 + n * 4
                        original_pair = (int.from_bytes(before[pair_offset:pair_offset + 4], 'little') << 32) | int.from_bytes(
                            before[pair_offset + 4:pair_offset + 8], 'little')
                        assert pair_bits(fpu, bank, n) == original_pair
                        cases += 1

    out = ROOT / 'outputs/sonic-adventure'
    report = {
        'cases_passed': cases,
        'implementation_sha256': hashlib.sha256((out / 'sh4_binary64.py').read_bytes()).hexdigest(),
        'interpreter_sha256': hashlib.sha256((out / 'verify_startup.py').read_bytes()).hexdigest(),
        'manual': 'https://www.renesas.com/en/document/mas/sh-4-software-manual',
        'manual_revision': 'Rev. 6.00, 2006.09',
        'scope': 'Finite DR-pair FCMP/EQ and FCMP/GT in PR=1, both banks, all even DR pairs, signs, zeros, exact equalities, and unchanged register/status state except T and cleared FPSCR cause bits.',
        'limitations': ['NaNs, infinities, denormal comparisons, enabled exception behavior, odd DR encodings, and Dreamcast hardware behavior are not validated.'],
    }
    (out / 'analysis/fpu_double_compare_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'{cases} PR=1 double FCMP/EQ and FCMP/GT instruction checks passed')


if __name__ == '__main__':
    main()
