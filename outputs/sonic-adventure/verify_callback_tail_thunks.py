"""Verify direct indirect-tail-call thunks with retail SH-4 execution."""
import ctypes
import hashlib
import json
import random
import struct
from pathlib import Path

from verify_startup import ROOT, State, reference

ADDRESSES = [0x8c014422, 0x8c0544dc, 0x8c054998, 0x8c0585ae,
             0x8c0585b4, 0x8c0585ca, 0x8c0585d0, 0x8c0585d6,
             0x8c0585e8, 0x8c058640, 0x8c09da08, 0x8c0d2156,
             0x8c600032, 0x8c601874, 0x8c602dda, 0x8c602df2,
             0x8c602e9c, 0x8c604370, 0x8c6055ba, 0x8c63fe3c,
             0x8c63fe48, 0x8c64489c, 0x8c646884, 0x8c646b36,
             0x8c646b3c, 0x8c647908, 0x8c65b4c8, 0x8c65b4d4,
             0x8c65b4f8, 0x8c65b4fe, 0x8c65b50a, 0x8c65b51a,
             0x8c65b526, 0x8c65b52c, 0x8c65b538, 0x8c65b53e,
             0x8c65b55c, 0x8c66086a, 0x8c669260, 0x8c66926c,
             0x8c669444, 0x8c6694dc]
BASE, RAM_SIZE = 0x8c010000, 16 * 1024 * 1024
DISPATCH = ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_uint8),
                            ctypes.POINTER(State), ctypes.c_uint32,
                            ctypes.c_void_p)


def ram_index(address):
    return (address & 0x1fffffff) - 0x0c000000


def callback_effect(memory, regs, sr, case, target):
    args = tuple(regs[4:8])
    marker = (target ^ regs[3] ^ regs[2] ^ (case * 0x1020304)) & 0xffffffff
    data = marker.to_bytes(4, 'little')
    at = 0xb80000 + case * 4
    if isinstance(memory, bytearray):
        memory[at:at + 4] = data
    else:
        ctypes.memmove(ctypes.addressof(memory) + at, data, 4)
    regs[0] = (marker ^ 0x13579bdf) & 0xffffffff
    regs[1] = (regs[1] + marker) & 0xffffffff
    regs[2] ^= 0x2468ace0
    regs[4] = (regs[4] - case - 1) & 0xffffffff
    regs[5] ^= marker
    regs[6] = (regs[6] + 0x11111111) & 0xffffffff
    regs[7] ^= 0x87654321
    return args, marker, (sr ^ marker) & 0xffffffff


def main():
    out = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    lib = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/callback_tail_thunks.dll'))
    original = bytearray(b'\xa5') * RAM_SIZE
    original[0x10000:0x10000 + len(binary)] = binary
    rng = random.Random(0x5441494c)
    results = []

    for address in ADDRESSES:
        offset = address - BASE
        first, jump, delay = struct.unpack_from('<HHH', binary, offset)
        register = (first >> 8) & 15
        assert first & 0xf000 == 0xd000 and jump == (0x402b | register << 8) and delay == 9, hex(address)
        literal = ((address + 4) & ~3) + (first & 255) * 4
        target = struct.unpack_from('<I', binary, literal - BASE)[0]
        native = getattr(lib, f'sa_callback_tail_{address:08x}')
        native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                           DISPATCH, ctypes.c_void_p]
        native.restype = None
        cases = []

        for case in range(8):
            expected = State()
            expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
            expected.sr = rng.getrandbits(32)
            expected.pr = 0x8c000100 + case * 4
            expected.pc = address
            actual = State.from_buffer_copy(bytes(expected))
            retail_ram, actual_ram = bytearray(original), bytearray(original)
            retail_calls, native_calls = [], []

            def hook(memory, regs, sr):
                assert regs[register] == target
                args, marker, new_sr = callback_effect(memory, regs, sr, case, target)
                retail_calls.append((target, args, marker, expected.pr))
                return new_sr

            steps = reference(retail_ram, expected, hooks={target: hook})

            @DISPATCH
            def dispatch(memory_pointer, state_pointer, call_target, context):
                assert call_target == target
                state = state_pointer.contents
                regs = list(state.r)
                memory = (ctypes.c_uint8 * RAM_SIZE).from_address(
                    ctypes.addressof(memory_pointer.contents))
                args, marker, new_sr = callback_effect(
                    memory, regs, state.sr, case, call_target)
                for index, value in enumerate(regs):
                    state.r[index] = value
                state.sr = new_sr
                native_calls.append((call_target, args, marker, state.pr))

            ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
            native(ram_view, ctypes.byref(actual), dispatch, None)
            assert steps == 3, (hex(address), steps)
            assert actual_ram == retail_ram, f'RAM mismatch at {address:#x}, case {case}'
            assert bytes(actual) == bytes(expected), f'CPU mismatch at {address:#x}, case {case}'
            assert native_calls == retail_calls, f'callback mismatch at {address:#x}, case {case}'
            cases.append({'case': case, 'retail_steps': steps,
                          'target': hex(target), 'full_state_and_ram_match': True})

        results.append({'address': hex(address), 'end_exclusive': hex(address + 6),
                        'literal_address': hex(literal), 'target': hex(target),
                        'target_register': register,
                        'retail_span_sha256': hashlib.sha256(binary[offset:offset + 6]).hexdigest(),
                        'cases_passed': len(cases), 'cases': cases})

    source = out / 'source/callback_tail_thunks.c'
    report = {'functions_verified': len(results),
              'total_full_state_cases': sum(item['cases_passed'] for item in results),
              'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
              'scope': 'Forty-two six-byte indirect tail-call thunks whose exact retail shape is PC-relative target load, JMP @Rn, NOP delay slot; eight randomized full CPU and 16 MiB RAM comparisons per thunk with callees modeled as services.',
              'limitations': ['The 42 leaf thunks are verified; their target callee bodies are not included in this recovery batch.',
                              'The callback service effects are modeled, and verification uses the project’s partial SH-4 interpreter rather than hardware or end-to-end game execution.'],
              'routines': results}
    (out / 'analysis/callback_tail_thunks_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"{len(results)} tail thunks passed {report['total_full_state_cases']} full-state comparisons")


if __name__ == '__main__':
    main()
