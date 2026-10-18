"""Compare the two-target callback wrapper against retail SH-4 execution."""
import ctypes
import hashlib
import itertools
import json
import struct

from verify_fpu_cleanup_transfer import Fpu
from verify_startup import ROOT, State, reference
from verify_tikal_controller import Call, P

PROJECT = ROOT / 'outputs/sonic-adventure'
FUNCTION = 0x8C038DFC
END = 0x8C038E0C
FIRST = 0x8C036FFC
SECOND = 0x8C605E0A
RAM_SIZE = 16 * 1024 * 1024
library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/callback_pair_tail_8c038dfc.dll'))
NATIVE = library.sa_callback_pair_tail_8c038dfc
NATIVE.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(Fpu), P, P, P,
                   P, P, ctypes.c_void_p, Call]
NATIVE.restype = ctypes.c_int


def write_ram32(ram, address, value):
    at = address & 0xFFFFFF
    if isinstance(ram, bytearray):
        struct.pack_into('<I', ram, at, value & 0xFFFFFFFF)
        return
    memory = ctypes.cast(ram, ctypes.POINTER(ctypes.c_uint8))
    for i in range(4):
        memory[at + i] = (value >> (8 * i)) & 0xFF


def apply_service(number, target, ram, regs, sr, pr):
    if target == FIRST:
        assert pr[0] == 0x8C038E04
        regs[1] = (regs[1] + 0x10203 + number) & 0xFFFFFFFF
        regs[5] ^= 0xA5000000 | number
        sr[0] = (sr[0] & ~1) | (number & 1)
        write_ram32(ram, 0x8CA10000 + number * 4, 0xF1000000 | number)
    elif target == SECOND:
        assert pr[0] == 0x8C000500 + number * 4
        regs[0] = (0xA0000000 | number)
        regs[7] = (regs[7] - 0x1122 - number) & 0xFFFFFFFF
        sr[0] = (sr[0] & ~1) | ((number >> 1) & 1)
        write_ram32(ram, 0x8CA20000 + number * 4, 0xF2000000 | number)
    else:
        raise AssertionError(f'unexpected callback target {target:#010x}')


def run_case(number, stack_offset):
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram = bytearray(RAM_SIZE)
    ram[0x10000:0x10000 + len(binary)] = binary
    initial = State()
    initial.r[:] = [((number + 5) * 0x10203041 + i * 0x13579B) & 0xFFFFFFFF
                    for i in range(16)]
    initial.r[15] = 0x8CF00000 - stack_offset
    initial.sr = 0x70000060 | (number & 1)
    initial.pr = 0x8C000500 + number * 4
    initial.pc = FUNCTION
    initial_fpu = Fpu()
    initial_fpu.fpscr = 0x200000 if number & 1 else 0
    initial_mac = 0xA1B2C3D4 ^ number

    expected_ram = bytearray(ram)
    expected = State.from_buffer_copy(initial)
    expected_fpu = Fpu.from_buffer_copy(initial_fpu)
    expected_events = []

    def hook_for(target):
        def hook(memory, regs, sr):
            expected_events.append((target, expected.pr, tuple(regs), regs[15]))
            sr_ptr = [sr]
            pr_ptr = [expected.pr]
            apply_service(number, target, memory, regs, sr_ptr, pr_ptr)
            expected.pr = pr_ptr[0]
            return sr_ptr[0]
        return hook

    reference(expected_ram, expected,
              hooks={FIRST: hook_for(FIRST), SECOND: hook_for(SECOND)},
              fpu=expected_fpu, special={'macl': initial_mac})

    native_ram = bytearray(ram)
    native = State.from_buffer_copy(initial)
    native_fpu = Fpu.from_buffer_copy(initial_fpu)
    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
    sr = ctypes.c_uint32(initial.sr)
    pr = ctypes.c_uint32(initial.pr)
    fpul = ctypes.c_uint32(0xCAFEBABE)
    macl = ctypes.c_uint32(initial_mac)
    native_events = []

    @Call
    def dispatch(_context, target, memory, _fpu, sr_ptr, regs, pr_ptr):
        target = int(target)
        native_events.append((target, pr_ptr[0], tuple(regs[i] for i in range(16)),
                              regs[15]))
        apply_service(number, target, memory, regs, sr_ptr, pr_ptr)
        return 0

    result = NATIVE(view, ctypes.byref(native_fpu), ctypes.byref(sr), native.r,
                    ctypes.byref(pr), ctypes.byref(fpul), ctypes.byref(macl),
                    None, dispatch)
    assert result == 0
    assert native_ram == expected_ram, f'RAM mismatch in case {number}'
    assert list(native.r) == list(expected.r), f'GPR mismatch in case {number}'
    assert sr.value == expected.sr and pr.value == expected.pr
    assert bytes(native_fpu) == bytes(expected_fpu)
    assert fpul.value == 0xCAFEBABE and macl.value == initial_mac
    expected_trace = [(event[0], event[1], tuple(event[2]), event[3])
                      for event in expected_events]
    assert native_events == expected_trace, (native_events, expected_trace)
    return {'case': number, 'stack_offset': stack_offset,
            'callback_targets': [hex(event[0]) for event in native_events],
            'full_ram_cpu_fpu_and_callback_trace_match': True}


def main():
    cases = [run_case(number, (0, 2, 4, 8)[number % 4])
             for number in range(24)]
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    source = PROJECT / 'source/sa_callback_pair_tail_8c038dfc.c'
    span = binary[FUNCTION - 0x8C010000:END - 0x8C010000]
    report = {
        'module': '1ST_READ.BIN', 'load_address': hex(0x8C010000),
        'address': hex(FUNCTION), 'end_exclusive': hex(END),
        'indexed_instruction_bytes': END - FUNCTION,
        'retail_span_sha256': hashlib.sha256(span).hexdigest(),
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cases_passed': len(cases),
        'comparison_level': 'Complete 16 MiB RAM, all GPR/SR/PR/FPU state, and both callback targets/order/return addresses compared against retail SH-4 execution.',
        'scope': 'Tests saved-PR first call, restored stack and PR, second tail-call, callback mutations, and return to caller.',
        'limitations': ['Both external service routines use deterministic callbacks; their implementations are outside this wrapper recovery.'],
        'cases': cases,
    }
    (PROJECT / 'analysis/callback_pair_tail_8c038dfc_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'{len(cases)} callback-pair tail-wrapper retail comparisons passed')


if __name__ == '__main__':
    main()
