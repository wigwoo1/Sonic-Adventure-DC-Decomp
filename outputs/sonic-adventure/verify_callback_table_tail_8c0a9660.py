"""Differentially verify the retail 0x8c0a9660 callback tail thunk."""
import ctypes
import hashlib
import json
import random
import struct
from pathlib import Path

from verify_startup import ROOT, State, reference

ENTRY, END = 0x8c0a9660, 0x8c0a966c
CALLBACK = 0x8c0ae3d6
RAM_SIZE = 16 * 1024 * 1024
DISPATCH = ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_uint8),
                            ctypes.POINTER(State), ctypes.c_uint32,
                            ctypes.c_void_p)


def effect(memory, regs, sr, case):
    args = (regs[4], regs[6])
    marker = (CALLBACK ^ regs[4] ^ (regs[6] * 0x10203) ^
              (case * 0x30405)) & 0xffffffff
    if isinstance(memory, bytearray):
        struct.pack_into('<I', memory, 0xb80000 + case * 4, marker)
    else:
        ctypes.memmove(ctypes.addressof(memory) + 0xb80000 + case * 4,
                       marker.to_bytes(4, 'little'), 4)
    regs[0] = (marker ^ 0xa5a55a5a) & 0xffffffff
    regs[2] = (regs[2] + marker) & 0xffffffff
    regs[5] ^= marker
    regs[6] = (regs[6] - case - 1) & 0xffffffff
    regs[7] ^= 0x55aa55aa
    return args, marker, (sr ^ marker) & 0xffffffff


def main():
    out = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    lib = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/callback_table_tail_8c0a9660.dll'))
    function = lib.sa_callback_table_tail_8c0a9660
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                         DISPATCH, ctypes.c_void_p]
    function.restype = None
    assert struct.unpack_from('<I', binary, 0x8c0a9740 - 0x8c010000)[0] == CALLBACK
    rng = random.Random(ENTRY)
    cases = []

    for case in range(32):
        original = bytearray(rng.randbytes(RAM_SIZE))
        original[0x10000:0x10000 + len(binary)] = binary
        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.r[6] = 0x8c300000 + case * 0x100
        expected.r[4] = 0x8c200000 + case * 0x100
        first_pointer, second_pointer = expected.r[4], expected.r[6]
        expected.sr = rng.getrandbits(32)
        expected.pr = 0x8c000100 + case * 4
        expected.pc = ENTRY
        actual_state = State.from_buffer_copy(bytes(expected))
        retail_ram, actual_ram = bytearray(original), bytearray(original)
        retail_calls, native_calls = [], []

        def hook(memory, regs, sr):
            assert tuple(regs[index] for index in (4, 6)) == (
                expected.r[4], expected.r[6])
            args, marker, new_sr = effect(memory, regs, sr, case)
            retail_calls.append((CALLBACK, args, expected.pr, marker))
            return new_sr

        steps = reference(retail_ram, expected, hooks={CALLBACK: hook})

        @DISPATCH
        def dispatch(memory_pointer, state_pointer, target, context):
            assert target == CALLBACK
            state = state_pointer.contents
            memory = (ctypes.c_uint8 * RAM_SIZE).from_address(
                ctypes.addressof(memory_pointer.contents))
            regs = list(state.r)
            args, marker, new_sr = effect(memory, regs, state.sr, case)
            for index, value in enumerate(regs):
                state.r[index] = value
            state.sr = new_sr
            native_calls.append((target, args, state.pr, marker))

        actual_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        function(actual_view, ctypes.byref(actual_state), dispatch, None)
        assert actual_ram == retail_ram, f'RAM mismatch in case {case}'
        assert bytes(actual_state) == bytes(expected), f'CPU state mismatch in case {case}'
        assert native_calls == retail_calls, f'callback ABI mismatch in case {case}'
        assert retail_ram[first_pointer & 0xffffff] == 1
        assert struct.unpack_from('<H', retail_ram,
                                  (second_pointer + 0xe) & 0xffffff)[0] == 0
        cases.append({'case': case, 'retail_steps': steps,
                      'target': hex(CALLBACK), 'pr': hex(expected.pr),
                      'r4': hex(first_pointer), 'r6': hex(second_pointer),
                      'callback_marker': hex(retail_calls[0][3]),
                      'full_state_and_ram_match': True})

    source = out / 'source/callback_table_tail_8c0a9660.c'
    report = {'cases_passed': len(cases), 'routine_start': hex(ENTRY),
              'routine_end_exclusive': hex(END), 'instruction_bytes': END - ENTRY,
              'retail_span_sha256': hashlib.sha256(
                  binary[ENTRY - 0x8c010000:END - 0x8c010000]).hexdigest(),
              'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
              'callback_target': hex(CALLBACK), 'cases': cases,
              'scope': 'All 12 retail bytes interpreted and compared with a native C tail-call wrapper across 32 seeded full 16 MiB RAM and complete GPR/SR/PR/PC states; callback effects modeled identically.',
              'limitations': ['The callback at 0x8c0ae3d6 is modeled as a service and is not claimed recovered.',
                              'The subset SH-4 interpreter comparison is not hardware or end-to-end game verification.']}
    (out / 'analysis/callback_table_tail_8c0a9660_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'{len(cases)} callback-tail full-state and RAM comparisons passed')


if __name__ == '__main__':
    main()
