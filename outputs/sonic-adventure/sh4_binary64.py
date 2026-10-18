"""Finite SH-4 double-precision register-pair arithmetic for the test emulator.

Register layout follows the Renesas SH-4 Software Manual: DRn is {FRn, FRn+1}.
Exceptional operands and enabled traps are rejected explicitly for now.
"""
import struct


def pair_bits(fpu, bank, register):
    if register & 1:
        raise NotImplementedError('Double-precision DR registers require even register numbers')
    return (int(fpu.banks[bank][register]) << 32) | int(fpu.banks[bank][register + 1])


def set_pair_bits(fpu, bank, register, bits):
    if register & 1:
        raise NotImplementedError('Double-precision DR registers require even register numbers')
    fpu.banks[bank][register] = (bits >> 32) & 0xFFFFFFFF
    fpu.banks[bank][register + 1] = bits & 0xFFFFFFFF


def execute_compare(fpu, n, m, greater=False):
    """Compare finite DR pairs, return the SH-4 T-bit condition."""
    fpscr = int(fpu.fpscr)
    assert fpscr & 0x80000, 'Double helper called outside FPSCR.PR=1'
    assert not (n & 1 or m & 1), 'Odd DR register encoding is invalid'
    bank = (fpscr >> 21) & 1
    a = as_float(pair_bits(fpu, bank, n))
    b = as_float(pair_bits(fpu, bank, m))
    if not (a == a and b == b and abs(a) != float('inf') and abs(b) != float('inf')):
        raise NotImplementedError('NaN and infinity comparisons are not modeled')
    fpu.fpscr = fpscr & ~0x3F000
    return a > b if greater else a == b


def as_float(bits):
    return struct.unpack('>d', bits.to_bytes(8, 'big'))[0]


def as_bits(value):
    return int.from_bytes(struct.pack('>d', value), 'big')


def execute_add_sub(fpu, n, m, subtract=False):
    """Execute finite DRn +/- DRm with nearest-even or toward-zero rounding."""
    fpscr = int(fpu.fpscr)
    assert fpscr & 0x80000, 'Double helper called outside FPSCR.PR=1'
    assert not fpscr & 0xF80, 'Enabled FPU traps are not modeled'
    mode = fpscr & 3
    assert mode in (0, 1), 'Reserved SH-4 rounding mode'
    assert not (n & 1 or m & 1), 'Odd DR register encoding is invalid'
    bank = (fpscr >> 21) & 1
    a_bits = pair_bits(fpu, bank, n)
    b_bits = pair_bits(fpu, bank, m)
    a = as_float(a_bits)
    b = as_float(b_bits)
    if not (a == a and b == b and abs(a) != float('inf') and abs(b) != float('inf')):
        raise NotImplementedError('NaN and infinity arithmetic is not modeled')

    exact_a = a.as_integer_ratio()
    exact_b = b.as_integer_ratio()
    from fractions import Fraction
    exact = Fraction(*exact_a) + (-(Fraction(*exact_b)) if subtract else Fraction(*exact_b))
    rounded = a - b if subtract else a + b
    result_bits = as_bits(rounded)
    if ((result_bits >> 52) & 0x7FF) == 0x7FF:
        # Finite inputs can reach infinity only through overflow.
        if mode == 1:
            result_bits = (result_bits & (1 << 63)) | 0x7FEFFFFFFFFFFFFF
        fpu.fpscr = (fpscr & ~0x3F000) | 0x5014
        set_pair_bits(fpu, bank, n, result_bits)
        return
    if exact and ((result_bits >> 52) & 0x7FF) == 0:
        raise NotImplementedError('Double-precision underflow/subnormal results are not modeled')
    represented = Fraction(*rounded.as_integer_ratio()) if rounded else Fraction(0)
    inexact = represented != exact
    if mode == 1 and inexact and abs(represented) > abs(exact):
        result_bits -= 1
    if inexact:
        fpu.fpscr = (fpscr & ~0x3F000) | 0x1004
    else:
        fpu.fpscr = fpscr & ~0x3F000
    set_pair_bits(fpu, bank, n, result_bits)


def execute_multiply(fpu, n, m):
    _execute_binary(fpu, n, m, 'multiply')


def execute_divide(fpu, n, m):
    _execute_binary(fpu, n, m, 'divide')


def execute_float(fpu, n):
    """Convert signed FPUL int32 exactly to DRn in PR=1 mode."""
    fpscr = int(fpu.fpscr)
    assert fpscr & 0x80000, 'Double helper called outside FPSCR.PR=1'
    assert not (n & 1), 'Odd DR register encoding is invalid'
    bank = (fpscr >> 21) & 1
    raw = int(fpu.fpul) & 0xFFFFFFFF
    integer = raw if raw < 0x80000000 else raw - 0x100000000
    set_pair_bits(fpu, bank, n, as_bits(float(integer)))
    fpu.fpscr = fpscr & ~0x3F000


def _execute_binary(fpu, n, m, operation):
    """Execute finite DRn * or / DRm, with SH-4 RM and FPSCR status."""
    fpscr = int(fpu.fpscr)
    assert fpscr & 0x80000, 'Double helper called outside FPSCR.PR=1'
    assert not fpscr & 0xF80, 'Enabled FPU traps are not modeled'
    mode = fpscr & 3
    assert mode in (0, 1), 'Reserved SH-4 rounding mode'
    assert not (n & 1 or m & 1), 'Odd DR register encoding is invalid'
    bank = (fpscr >> 21) & 1
    a = as_float(pair_bits(fpu, bank, n))
    b = as_float(pair_bits(fpu, bank, m))
    if not (a == a and b == b and abs(a) != float('inf') and abs(b) != float('inf')):
        raise NotImplementedError('NaN and infinity arithmetic is not modeled')
    if operation == 'divide' and b == 0.0:
        raise NotImplementedError('Division by zero is not modeled')

    from fractions import Fraction
    exact_a = Fraction(*a.as_integer_ratio())
    exact_b = Fraction(*b.as_integer_ratio())
    if operation == 'multiply':
        exact = exact_a * exact_b
        rounded = a * b
    else:
        exact = exact_a / exact_b
        rounded = a / b
    result_bits = as_bits(rounded)
    if ((result_bits >> 52) & 0x7FF) == 0x7FF:
        if mode == 1:
            result_bits = (result_bits & (1 << 63)) | 0x7FEFFFFFFFFFFFFF
        fpu.fpscr = (fpscr & ~0x3F000) | 0x5014
        set_pair_bits(fpu, bank, n, result_bits)
        return
    if exact and ((result_bits >> 52) & 0x7FF) == 0:
        raise NotImplementedError('Double-precision underflow/subnormal results are not modeled')
    represented = Fraction(*rounded.as_integer_ratio()) if rounded else Fraction(0)
    inexact = represented != exact
    if mode == 1 and inexact and abs(represented) > abs(exact):
        result_bits -= 1
    fpu.fpscr = ((fpscr & ~0x3F000) | (0x1004 if inexact else 0))
    set_pair_bits(fpu, bank, n, result_bits)
