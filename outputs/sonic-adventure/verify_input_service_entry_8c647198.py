"""Differentially check input service entry control flow against retail SH-4."""
import ctypes
import hashlib
import itertools
import json
import struct

from verify_startup import ROOT, State, reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call, P


PROJECT = ROOT / 'outputs/sonic-adventure'
FUNCTION = 0x8C647198
LENGTH = 92
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
BINARY = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
SPAN = BINARY[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(SPAN) == LENGTH
LIBRARY = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/input_service_entry_8c647198.dll'))
NATIVE = LIBRARY.sa_input_service_entry_8c647198
NATIVE.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(Fpu), P, P, P, P, P,
                   ctypes.c_void_p, Call]
NATIVE.restype = ctypes.c_int
REPORT_LIBRARY = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/input_device_report_callback_8c65f348.dll'))
REPORT_NATIVE = REPORT_LIBRARY.sa_input_device_report_callback_8c65f348
REPORT_NATIVE.argtypes = NATIVE.argtypes
REPORT_NATIVE.restype = ctypes.c_int

LOCK_ADDRESS = 0x8C6607F8
OBJECT_BASE = 0x8CA00000
CALL_TARGETS = {0x8C65F348, 0x8C6604C0, 0x8C646F68, 0x8C646654}


def put32(ram, address, value):
    struct.pack_into('<I', ram, address & 0xFFFFFF, value & 0xFFFFFFFF)


def put16(ram, address, value):
    struct.pack_into('<H', ram, address & 0xFFFFFF, value & 0xFFFF)


def callback_effect(target, regs, service_result, pr, ram):
    event = {
        'target': target,
        'arguments': tuple(regs[i] for i in range(4, 8)),
        'stack_pointer': regs[15],
        'return_address': pr,
        'stack_words': tuple(struct.unpack_from('<I', ram, (regs[15] + off) & 0xFFFFFF)[0]
                             for off in (0, 4)),
    }
    if target == 0x8C6604C0:
        regs[0] = service_result & 0xFFFFFFFF
    return event


def run_case(case_number, lock_value, service_result, guard):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(BINARY)] = BINARY
    # Recreate the retail literal targets and runtime-owned object state.
    put16(ram, 0x8C647278, 0x10E8)
    put32(ram, 0x8C64727C, 0x8C6607F4)
    put32(ram, 0x8C647280, 0x8C66D34C)
    put32(ram, 0x8C647284, 0x8C65F348)
    put32(ram, 0x8C647288, 0x8C6604C0)
    put32(ram, 0x8C64728C, 0x8C8A83B4)
    put32(ram, 0x8C6607F4, LOCK_ADDRESS)
    ram[LOCK_ADDRESS & 0xFFFFFF] = lock_value
    put32(ram, 0x8C8A83B4, OBJECT_BASE)
    put32(ram, OBJECT_BASE + 0x10E8, guard)

    initial = State()
    initial.r[:] = [((case_number + 1) * 0x13579BDF + i * 0x1020304) & 0xFFFFFFFF
                    for i in range(16)]
    initial.r[4] = (0x8CA20000 + case_number * 0x100) & 0xFFFFFFFF
    initial.r[7] ^= 0x5A5AA5A5
    initial.r[14] = 0x8CE10000 + case_number * 16
    initial.r[15] = 0x8CF00000
    initial.sr = (0x70000000 | (case_number * 0x101)) & 0xFFFFFFFF
    initial.pr = 0x8C000100
    initial.pc = FUNCTION

    fpu = Fpu()
    fpu.fpscr = 0x200000 if case_number & 1 else 0
    initial_mac = (0xA1B2C3D4 ^ case_number) & 0xFFFFFFFF
    expected_ram = bytearray(ram)
    expected = State.from_buffer_copy(initial)
    expected_events = []

    def make_hook(target):
        def hook(memory, regs, sr):
            expected_events.append(callback_effect(target, regs, service_result,
                                                    expected.pr, memory))
            return sr
        return hook

    hooks = {target: make_hook(target) for target in CALL_TARGETS
             if target != 0x8C65F348}
    expected_special = {'macl': initial_mac}
    reference(expected_ram, expected, hooks=hooks, fpu=fpu,
              special=expected_special)

    native_ram = bytearray(ram)
    native = State.from_buffer_copy(initial)
    native_fpu = Fpu.from_buffer_copy(fpu)
    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
    sr = ctypes.c_uint32(initial.sr)
    pr = ctypes.c_uint32(initial.pr)
    fpul = ctypes.c_uint32(0xCAFEBABE)
    macl = ctypes.c_uint32(initial_mac)
    actual_events = []
    errors = []

    @Call
    def dispatch(_context, target, memory, _fpu, sr_ptr, regs_ptr, pr_ptr):
        if target == 0x8C65F348:
            return REPORT_NATIVE(view, ctypes.byref(native_fpu), sr_ptr, regs_ptr,
                                 pr_ptr, ctypes.byref(fpul), ctypes.byref(macl),
                                 None, dispatch)
        if target not in CALL_TARGETS:
            errors.append(hex(target))
            return -9
        regs = [regs_ptr[i] for i in range(16)]
        event = callback_effect(target, regs, service_result, pr_ptr[0], native_ram)
        actual_events.append(event)
        for i, value in enumerate(regs):
            regs_ptr[i] = value
        return 0

    result = NATIVE(view, ctypes.byref(native_fpu), ctypes.byref(sr), native.r,
                    ctypes.byref(pr), ctypes.byref(fpul), ctypes.byref(macl), None, dispatch)
    assert result == 0, f'Native harness returned {result} in case {case_number}'
    assert not errors, f'Unexpected callback targets: {errors}'
    assert native_ram == expected_ram, f'Full RAM mismatch in case {case_number}'
    assert list(native.r) == list(expected.r), f'CPU register mismatch in case {case_number}'
    assert sr.value == expected.sr, f'SR mismatch in case {case_number}'
    assert pr.value == expected.pr, f'PR mismatch in case {case_number}'
    assert bytes(native_fpu) == bytes(fpu), f'FPU state mismatch in case {case_number}'
    assert fpul.value == 0xCAFEBABE and macl.value == initial_mac
    assert actual_events == expected_events, f'Call arguments/order mismatch in case {case_number}'
    assert native_ram[LOCK_ADDRESS & 0xFFFFFF] == (lock_value | 0x80 if lock_value else 0)
    return {
        'case': case_number,
        'lock_byte_in': lock_value,
        'service_result': hex(service_result),
        'cleanup_flag': guard,
        'returned_r0': hex(native.r[0]),
        'calls': [hex(event['target']) for event in actual_events],
        'complete_ram_cpu_sr_pr_and_fpu_match': True,
    }


def main():
    scenarios = [(busy, 0, 0) for busy in (1, 0x80, 0xFF)]
    scenarios += [(0, result, guard) for result, guard in itertools.product(
        (0, 1, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF), (0, 1))]
    cases = [run_case(i, *scenario) for i, scenario in enumerate(scenarios)]
    source = PROJECT / 'source/sa_input_service_entry_8c647198.c'
    report = {
        'module': '1ST_READ.BIN',
        'load_address': hex(BASE),
        'address': hex(FUNCTION),
        'end_exclusive': hex(FUNCTION + LENGTH),
        'retail_span_sha256': hashlib.sha256(SPAN).hexdigest(),
        'binary_sha256': hashlib.sha256(BINARY).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cases_passed': len(cases),
        'comparison_level': 'Complete 16 MiB RAM, all GPR/SR/PR/FPU state, and callback arguments/order against retail SH-4 execution.',
        'scope': 'Recovers the 92-byte entry control flow, semaphore test/release, positive-result record call, and optional cleanup dispatch. The busy-path report callback at 0x8c65f348 executes recovered native code; its report-service condition is clear in these entry fixtures.',
        'limitations': [
            'The device service, record writer, and cleanup are dispatched as explicit test callbacks.',
            'The report callback and forwarding wrapper execute as recovered native code; the formatter and optional runtime callback remain fixtures. The report gate is clear in these caller scenarios.',
            'This isolated entry test still dispatches cleanup as a callee stub; the separately verified 0x8c646654 routine is covered by analysis/input_service_cleanup_8c646654_verification.json, where its external service callbacks remain explicit stubs.',
            'This verifies one input-service entry, not full game behavior or project completion.',
        ],
        'cases': cases,
    }
    path = PROJECT / 'analysis/input_service_entry_8c647198_verification.json'
    path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'{len(cases)} full-state retail comparisons passed for 0x8c647198')


if __name__ == '__main__':
    main()
