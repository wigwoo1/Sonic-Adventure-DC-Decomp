"""Differentially verify the 22-byte input device submit-rearm helper."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call, P


PROJECT = ROOT / 'outputs/sonic-adventure'
FUNCTION = 0x8C65F1EC
LENGTH = 22
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
BINARY = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
SPAN = BINARY[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(SPAN) == LENGTH
LIBRARY = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/input_device_submit_rearm.dll'))
NATIVE = LIBRARY.sa_input_device_submit_rearm
NATIVE.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(Fpu), P, P, P, P, P,
                   ctypes.c_void_p, Call]
NATIVE.restype = ctypes.c_int

GLOBAL_POINTER_CELL = 0x8C8A83B4
LOCK_POINTER_CELL = 0x8C6607F4


def put32(ram, address, value):
    struct.pack_into('<I', ram, address & 0xFFFFFF, value & 0xFFFFFFFF)


def run_case(case, offset_word, object_base, lock_byte):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(BINARY)] = BINARY
    struct.pack_into('<H', ram, 0x65F286, offset_word & 0xFFFF)
    put32(ram, GLOBAL_POINTER_CELL, object_base)
    put32(ram, LOCK_POINTER_CELL, 0x8C6607F8)
    ram[0x6607F8] = lock_byte

    rng = random.Random(0x8C65F1EC ^ case)
    target_address = (object_base + ctypes.c_int16(offset_word & 0xFFFF).value) & 0xFFFFFFFF
    target_offset = target_address & 0xFFFFFF
    ram[target_offset:target_offset + 16] = bytes(rng.getrandbits(8) for _ in range(16))
    before_target = bytes(ram[target_offset:target_offset + 4])

    initial = State()
    initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
    initial.r[15] = 0x8CF00000
    initial.sr = rng.getrandbits(32)
    initial.pr = 0x8C000100
    initial.pc = FUNCTION
    fpu = Fpu()
    fpu.fpscr = rng.choice((0, 0x200000))
    for bank in range(2):
        for register in range(16):
            fpu.banks[bank][register] = rng.getrandbits(32)
    initial_mac = rng.getrandbits(32)

    expected_ram = bytearray(ram)
    expected = State.from_buffer_copy(initial)
    expected_special = {'macl': initial_mac}
    reference(expected_ram, expected, fpu=fpu, special=expected_special)

    native_ram = bytearray(ram)
    actual = State.from_buffer_copy(initial)
    actual_fpu = Fpu.from_buffer_copy(fpu)
    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
    sr = ctypes.c_uint32(initial.sr)
    pr = ctypes.c_uint32(initial.pr)
    fpul = ctypes.c_uint32(0x12345678)
    macl = ctypes.c_uint32(initial_mac)

    calls = []

    @Call
    def unexpected_call(_ctx, _target, _ram, _fpu, _sr, _r, _pr):
        calls.append(_target)
        return -11

    result = NATIVE(view, ctypes.byref(actual_fpu), ctypes.byref(sr), actual.r,
                    ctypes.byref(pr), ctypes.byref(fpul), ctypes.byref(macl),
                    None, unexpected_call)
    assert result == 0
    assert not calls, f'Unexpected external calls: {calls}'
    assert native_ram == expected_ram, f'RAM mismatch in case {case}'
    assert list(actual.r) == list(expected.r), f'CPU register mismatch in case {case}'
    assert sr.value == expected.sr and pr.value == expected.pr
    assert bytes(actual_fpu) == bytes(fpu)
    assert fpul.value == 0x12345678 and macl.value == initial_mac
    assert native_ram[target_offset:target_offset + 4] == struct.pack('<I', 1)
    assert native_ram[0x6607F8] == 0
    return {
        'case': case,
        'signed_offset': ctypes.c_int16(offset_word & 0xFFFF).value,
        'object_base': hex(object_base),
        'lock_byte_in': lock_byte,
        'target_bytes_before': before_target.hex(),
        'complete_ram_cpu_sr_pr_and_fpu_match': True,
    }


def main():
    rng = random.Random(FUNCTION)
    offsets = [0x0FC0, 0, 1, 0x7FFF, 0x8000, 0xFFFF]
    cases = []
    for case in range(64):
        offset = offsets[case] if case < len(offsets) else rng.getrandbits(16)
        object_base = 0x8CA00000 + (case % 16) * 0x10000
        lock_byte = rng.getrandbits(8)
        cases.append(run_case(case, offset, object_base, lock_byte))

    source = PROJECT / 'source/sa_input_device_submit_rearm.c'
    report = {
        'module': '1ST_READ.BIN',
        'load_address': hex(BASE),
        'address': hex(FUNCTION),
        'end_exclusive': hex(FUNCTION + LENGTH),
        'retail_span_sha256': hashlib.sha256(SPAN).hexdigest(),
        'binary_sha256': hashlib.sha256(BINARY).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cases_passed': len(cases),
        'comparison_level': 'Complete 16 MiB RAM and all GPR/SR/PR/FPU state against retail SH-4 execution.',
        'scope': 'Sets a signed-offset request word to one, clears the device lock byte, and returns zero. Tests cover literal offset, signed boundaries, random offsets, bases and prior lock values.',
        'limitations': [
            'This verifies the 22-byte helper in isolation, not the 334-byte cleanup routine or full input subsystem.',
            'Synthetic interpreter comparisons are not full-game validation.',
        ],
        'cases': cases,
    }
    path = PROJECT / 'analysis/input_device_submit_rearm_verification.json'
    path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'{len(cases)} full-state retail comparisons passed for 0x8c65f1ec')


if __name__ == '__main__':
    main()
