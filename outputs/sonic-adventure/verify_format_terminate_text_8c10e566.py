"""Compare the text-termination helper against retail SH-4 instructions."""
import ctypes
import hashlib
import itertools
import json
import struct

from verify_fpu_cleanup_transfer import Fpu
from verify_startup import ROOT, State, reference
from verify_tikal_controller import Call, P

PROJECT = ROOT / 'outputs/sonic-adventure'
FUNCTION = 0x8C10E566
END = 0x8C10E576
RAM_SIZE = 16 * 1024 * 1024
library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/format_terminate_text_8c10e566.dll'))
NATIVE = library.sa_format_terminate_text_8c10e566
NATIVE.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(Fpu), P, P, P,
                   P, P, ctypes.c_void_p, Call]
NATIVE.restype = ctypes.c_int


def run_case(number, mode, stack_offset):
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram = bytearray(RAM_SIZE)
    ram[0x10000:0x10000 + len(binary)] = binary
    context = 0x8CA10000 + number * 0x20
    output = 0x8CA20000 + number * 0x20
    struct.pack_into('<I', ram, context & 0xFFFFFF, mode)
    struct.pack_into('<I', ram, (context + 4) & 0xFFFFFF, output)
    ram[output & 0xFFFFFF] = 0xA5

    initial = State()
    initial.r[:] = [((number + 3) * 0x13579BDF + i * 0x1020304) & 0xFFFFFFFF
                    for i in range(16)]
    initial.r[4] = context
    initial.r[15] = 0x8CF00000 - stack_offset
    initial.sr = 0x70000060 | (number & 1)
    initial.pr = 0x8C000300 + number * 4
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
    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
    sr = ctypes.c_uint32(initial.sr)
    pr = ctypes.c_uint32(initial.pr)
    fpul = ctypes.c_uint32(0xCAFEBABE)
    macl = ctypes.c_uint32(initial_mac)

    @Call
    def unexpected(*_args):
        raise AssertionError('This helper must not call an external function')

    result = NATIVE(view, ctypes.byref(native_fpu), ctypes.byref(sr), native.r,
                    ctypes.byref(pr), ctypes.byref(fpul), ctypes.byref(macl),
                    None, unexpected)
    assert result == 0
    assert native_ram == expected_ram, f'RAM mismatch in case {number}'
    assert list(native.r) == list(expected.r), f'GPR mismatch in case {number}'
    assert sr.value == expected.sr and pr.value == expected.pr
    assert bytes(native_fpu) == bytes(expected_fpu)
    assert fpul.value == 0xCAFEBABE and macl.value == initial_mac
    return {'case': number, 'mode': hex(mode), 'stack_offset': stack_offset,
            'output_byte': native_ram[output & 0xFFFFFF],
            'full_ram_cpu_fpu_match': True}


def main():
    cases = [run_case(i, mode, offset)
             for i, (mode, offset) in enumerate(
                 itertools.product((0, 1, 2, 0xFFFFFFFF), (0, 2)))]
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    source = PROJECT / 'source/sa_format_terminate_text_8c10e566.c'
    span = binary[FUNCTION - 0x8C010000:END - 0x8C010000]
    report = {
        'module': '1ST_READ.BIN', 'load_address': hex(0x8C010000),
        'address': hex(FUNCTION), 'end_exclusive': hex(END),
        'indexed_instruction_bytes': END - FUNCTION,
        'retail_span_sha256': hashlib.sha256(span).hexdigest(),
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cases_passed': len(cases),
        'comparison_level': 'Complete 16 MiB RAM, all GPR/SR/PR/FPU state compared against retail SH-4 interpretation.',
        'scope': 'Tests mode-1 terminator write, non-terminating modes, stack independence, and register/T-bit effects.',
        'limitations': ['The formatter context uses synthetic test storage; the caller format engine remains unrecovered.'],
        'cases': cases,
    }
    (PROJECT / 'analysis/format_terminate_text_8c10e566_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'{len(cases)} formatter text-termination retail comparisons passed')


if __name__ == '__main__':
    main()
