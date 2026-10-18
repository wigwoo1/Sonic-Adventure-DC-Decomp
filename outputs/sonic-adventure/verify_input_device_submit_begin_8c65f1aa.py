"""Differentially verify the 66-byte submit preflight helper."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call, P


PROJECT = ROOT / 'outputs/sonic-adventure'
FUNCTION = 0x8C65F1AA
LENGTH = 66
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
BINARY = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
SPAN = BINARY[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(SPAN) == LENGTH
LIBRARY = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/input_device_submit_begin_8c65f1aa.dll'))
NATIVE = LIBRARY.sa_input_device_submit_begin_8c65f1aa
NATIVE.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(Fpu), P, P, P, P, P,
                   ctypes.c_void_p, Call]
NATIVE.restype = ctypes.c_int

OBJECT_POINTER_CELL = 0x8C8A83B4
LOCK_POINTER_CELL = 0x8C6607F4
LOCK_ADDRESS = 0x8C6607F8
ERROR_TARGET = 0x8C65F348


def put32(ram, address, value):
    struct.pack_into('<I', ram, address & 0xFFFFFF, value & 0xFFFFFFFF)


def callback_event(target, regs, pr, ram):
    return {
        'target': target,
        'arguments': tuple(regs[i] for i in range(4, 8)),
        'stack_pointer': regs[15],
        'return_address': pr,
        'stack_word': struct.unpack_from('<I', ram, regs[15] & 0xFFFFFF)[0],
    }


def run_case(case, status_value, flag_word, lock_value):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(BINARY)] = BINARY
    rng = random.Random(FUNCTION ^ case)
    object_base = 0x8CA00000 + (case % 8) * 0x20000
    info = 0x8CB00000 + (case % 8) * 0x1000
    put32(ram, OBJECT_POINTER_CELL, object_base)
    put32(ram, object_base + 0xFC0, status_value)
    put32(ram, object_base + 0x10F4, info)
    put32(ram, info + 0x18, flag_word)
    put32(ram, LOCK_POINTER_CELL, LOCK_ADDRESS)
    ram[LOCK_ADDRESS & 0xFFFFFF] = lock_value

    initial = State()
    initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
    initial.r[15] = 0x8CF00000
    initial.sr = rng.getrandbits(32)
    initial.pr = 0x8C000100
    initial.pc = FUNCTION
    fpu = Fpu()
    fpu.fpscr = rng.choice((0, 0x200000))
    for bank in range(2):
        for index in range(16):
            fpu.banks[bank][index] = rng.getrandbits(32)
    initial_fpul = rng.getrandbits(32)
    initial_mac = rng.getrandbits(32)

    expected_ram = bytearray(ram)
    expected = State.from_buffer_copy(initial)
    expected_events = []

    def error_hook(memory, regs, sr):
        expected_events.append(callback_event(ERROR_TARGET, regs, expected.pr, memory))
        return sr

    reference(expected_ram, expected, hooks={ERROR_TARGET: error_hook}, fpu=fpu,
              special={'macl': initial_mac})

    native_ram = bytearray(ram)
    actual = State.from_buffer_copy(initial)
    actual_fpu = Fpu.from_buffer_copy(fpu)
    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
    sr = ctypes.c_uint32(initial.sr)
    pr = ctypes.c_uint32(initial.pr)
    fpul = ctypes.c_uint32(initial_fpul)
    macl = ctypes.c_uint32(initial_mac)
    actual_events = []
    unexpected = []

    @Call
    def dispatch(_context, target, _memory, _fpu, _sr, regs_ptr, pr_ptr):
        regs = [regs_ptr[index] for index in range(16)]
        if target != ERROR_TARGET:
            unexpected.append(hex(target))
            return -9
        actual_events.append(callback_event(target, regs, pr_ptr[0], native_ram))
        return 0

    result = NATIVE(view, ctypes.byref(actual_fpu), ctypes.byref(sr), actual.r,
                    ctypes.byref(pr), ctypes.byref(fpul), ctypes.byref(macl),
                    None, dispatch)
    assert result == 0
    assert not unexpected, f'Unexpected callback targets: {unexpected}'
    assert native_ram == expected_ram, f'RAM mismatch case {case}'
    assert list(actual.r) == list(expected.r), f'CPU register mismatch case {case}'
    assert sr.value == expected.sr and pr.value == expected.pr
    assert bytes(actual_fpu) == bytes(fpu)
    assert fpul.value == initial_fpul and macl.value == initial_mac
    assert actual_events == expected_events, f'Callback arguments/order mismatch case {case}'
    expected_lock = lock_value | 0x80 if status_value == 0 and not (flag_word & 1) else lock_value
    assert native_ram[LOCK_ADDRESS & 0xFFFFFF] == expected_lock
    expected_result = (-3 if status_value else (-1 if flag_word & 1 else (-2 if lock_value else 0)))
    assert actual.r[0] == (expected_result & 0xFFFFFFFF)
    return {
        'case': case,
        'request_status': status_value,
        'device_flags': hex(flag_word),
        'lock_byte_in': lock_value,
        'returned_r0': hex(actual.r[0]),
        'callback_targets': [hex(event['target']) for event in actual_events],
        'complete_ram_cpu_sr_pr_and_fpu_match': True,
    }


def main():
    rng = random.Random(FUNCTION)
    scenarios = [
        (1, 0, 0),       # request already active -> -3
        (0, 1, 0),       # device flag set -> -1
        (0, 0, 0),       # lock acquired -> 0
        (0, 0, 1),       # busy lock -> error callback and -2
        (0, 0, 0x80),
        (0, 0, 0xFF),
    ]
    for _ in range(58):
        scenarios.append((0, rng.getrandbits(32), rng.getrandbits(8)))
    cases = [run_case(index, *scenario) for index, scenario in enumerate(scenarios)]
    source = PROJECT / 'source/sa_input_device_submit_begin_8c65f1aa.c'
    report = {
        'module': '1ST_READ.BIN',
        'load_address': hex(BASE),
        'address': hex(FUNCTION),
        'end_exclusive': hex(FUNCTION + LENGTH),
        'retail_span_sha256': hashlib.sha256(SPAN).hexdigest(),
        'binary_sha256': hashlib.sha256(BINARY).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cases_passed': len(cases),
        'comparison_level': 'Complete 16 MiB RAM and all GPR/SR/PR/FPU state plus callback arguments/order against retail SH-4 execution.',
        'scope': 'Checks request status and device flags, claims the semaphore when available, and dispatches the fatal-error callback when the semaphore is already busy.',
        'limitations': [
            'The fatal-error routine is represented by an explicit no-effect callback stub in this isolated test.',
            'This verifies one 66-byte preflight routine, not the full device service or cleanup routine.',
        ],
        'cases': cases,
    }
    path = PROJECT / 'analysis/input_device_submit_begin_8c65f1aa_verification.json'
    path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'{len(cases)} full-state retail comparisons passed for 0x8c65f1aa')


if __name__ == '__main__':
    main()
