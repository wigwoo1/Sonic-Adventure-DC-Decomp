"""Compare integer conversion against retail code and recovered division helpers."""
import ctypes
import hashlib
import itertools
import json
import struct

from verify_fpu_cleanup_transfer import Fpu
from verify_startup import ROOT, State, reference
from verify_tikal_controller import Call, P

PROJECT = ROOT / 'outputs/sonic-adventure'
FUNCTION = 0x8C10E304
END = 0x8C10E50E
RAM_SIZE = 16 * 1024 * 1024
library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/format_integer_convert_8c10e304.dll'))
NATIVE = library.sa_format_integer_convert
NATIVE.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(Fpu), P, P, P,
                   P, P, ctypes.c_void_p, Call]
NATIVE.restype = ctypes.c_int
DIVIDE = library.sa_unsigned_quotient
REMAINDER = library.sa_unsigned_remainder_8c10cc80
for helper in (DIVIDE, REMAINDER):
    helper.argtypes = NATIVE.argtypes
    helper.restype = ctypes.c_int


def scenario_list():
    values = (0, 1, 9, 10, 255, 0x80000000, 0xFFFFFFFF, 0x12345678)
    specs = (ord('d'), ord('i'), ord('u'), ord('o'), ord('x'), ord('X'))
    settings = (
        (0x00, 0, 0, ord(' ')),
        (0x04, 0, 0, ord(' ')),
        (0x0C, 4, 0, ord(' ')),
        (0x14, 8, 2, ord(' ')),
        (0x04, 10, 5, ord('0')),
        (0x1C, 12, 3, ord('0')),
    )
    cases = []
    for value, spec, setting in itertools.product(values, specs, settings):
        flags, width, precision, zero = setting
        cases.append((value, spec, flags, width, precision, zero))
    return cases


def run_case(number, case):
    value, spec, flags, width, precision, zero = case
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram = bytearray(RAM_SIZE)
    ram[0x10000:0x10000 + len(binary)] = binary
    output = 0x8CA10000 + number * 0x100
    context = 0x8CA20000 + number * 0x80
    ram[output & 0xFFFFFF:(output & 0xFFFFFF) + 96] = bytes([0xA5]) * 96
    ram[context & 0xFFFFFF:(context & 0xFFFFFF) + 0x40] = bytes(0x40)
    struct.pack_into('<I', ram, (context + 4) & 0xFFFFFF, width & 0xFFFFFFFF)
    struct.pack_into('<I', ram, (context + 8) & 0xFFFFFF, precision & 0xFFFFFFFF)
    ram[(context & 0xFFFFFF)] = flags
    ram[(context + 1) & 0xFFFFFF] = zero
    initial = State()
    initial.r[:] = [((number + 13) * 0x13579BDF + i * 0x01020304) & 0xFFFFFFFF
                    for i in range(16)]
    initial.r[4] = value
    initial.r[5] = output
    initial.r[6] = spec
    initial.r[7] = context
    initial.r[15] = 0x8CF00000 - (number % 4) * 2
    initial.sr = 0x70000060 | (number & 1)
    initial.pr = 0x8C000700 + number * 4
    initial.pc = FUNCTION
    initial_fpu = Fpu()
    initial_fpu.fpscr = 0x200000 if number & 1 else 0
    initial_mac = 0xA1B2C3D4 ^ number

    expected_ram = bytearray(ram)
    expected = State.from_buffer_copy(initial)
    expected_fpu = Fpu.from_buffer_copy(initial_fpu)
    reference(expected_ram, expected, fpu=expected_fpu,
              special={'macl': initial_mac})

    native_ram = bytearray(ram)
    native = State.from_buffer_copy(initial)
    native_fpu = Fpu.from_buffer_copy(initial_fpu)
    memory = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
    sr = ctypes.c_uint32(initial.sr)
    pr = ctypes.c_uint32(initial.pr)
    fpul = ctypes.c_uint32(0xCAFEBABE)
    macl = ctypes.c_uint32(initial_mac)
    events = []
    errors = []

    @Call
    def dispatch(_context, target, ram_arg, fpu_arg, sr_arg, regs, pr_arg):
        target = int(target)
        events.append((target, pr_arg[0], tuple(regs[i] for i in range(8)), regs[15]))
        if target == 0x8C10CC80:
            return REMAINDER(ram_arg, fpu_arg, sr_arg, regs, pr_arg,
                             ctypes.byref(fpul), ctypes.byref(macl), None, dispatch)
        if target == 0x8C10CB10:
            return DIVIDE(ram_arg, fpu_arg, sr_arg, regs, pr_arg,
                          ctypes.byref(fpul), ctypes.byref(macl), None, dispatch)
        errors.append(hex(target))
        return -1

    result = NATIVE(memory, ctypes.byref(native_fpu), ctypes.byref(sr), native.r,
                    ctypes.byref(pr), ctypes.byref(fpul), ctypes.byref(macl),
                    None, dispatch)
    assert result == 0 and not errors, (number, result, errors)
    assert native_ram == expected_ram, f'RAM mismatch in case {number}: {case}'
    assert list(native.r) == list(expected.r), (
        number, case, [(i, hex(native.r[i]), hex(expected.r[i]))
                       for i in range(16) if native.r[i] != expected.r[i]])
    assert sr.value == expected.sr and pr.value == expected.pr, (number, case)
    assert bytes(native_fpu) == bytes(expected_fpu), (number, case)
    assert fpul.value == 0xCAFEBABE and macl.value == initial_mac
    return {'case': number, 'value': hex(value), 'specifier': chr(spec),
            'flags': hex(flags), 'width': width, 'precision': precision,
            'native_division_calls': len(events), 'complete_state_matches': True}


def main():
    cases = [run_case(i, case) for i, case in enumerate(scenario_list())]
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    source = PROJECT / 'source/sa_format_integer_convert.c'
    span = binary[FUNCTION - 0x8C010000:END - 0x8C010000]
    report = {
        'module': '1ST_READ.BIN', 'load_address': hex(0x8C010000),
        'address': hex(FUNCTION), 'end_exclusive': hex(END),
        'indexed_instruction_bytes': 514,
        'address_span_bytes_including_embedded_data': END - FUNCTION,
        'retail_span_sha256': hashlib.sha256(span).hexdigest(),
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cases_passed': len(cases),
        'comparison_level': 'Complete 16 MiB RAM, all GPR/SR/PR/FPU/MACL state compared against retail SH-4 execution; both indirect division helpers linked natively.',
        'scope': 'Covers signed and unsigned decimal, octal, lower/uppercase hex, zero and high-bit values, alternate/sign/space flags, width, precision, and both FPU register banks.',
        'limitations': ['The larger formatter caller and its argument parser remain unrecovered as a complete native path.'],
        'cases': cases,
    }
    (PROJECT / 'analysis/format_integer_convert_8c10e304_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'{len(cases)} integer-conversion retail comparisons passed')


if __name__ == '__main__':
    main()
