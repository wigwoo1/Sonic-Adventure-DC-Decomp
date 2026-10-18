"""Verify input bit writes and the conditional retail callback dispatch."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ENTRY, END = 0x8c0a6918, 0x8c0a6960
BASE, RAM_SIZE = 0x8c010000, 16 * 1024 * 1024
MODE_POINTER_LITERAL, MODE_STORAGE, CALLBACK_LITERAL = 0x8c0a69e0, 0x8c760970, 0x8c0a69e4
CALLBACK_TARGET, STACK_TOP = 0x8c0115d0, 0x8cfff000
STACK_OFFSET = (STACK_TOP & 0x1fffffff) - 0x0c000000
CALLBACK = ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_uint8),
                            ctypes.POINTER(State), ctypes.c_uint32,
                            ctypes.c_void_p)


class Scenario(ctypes.Structure):
    _fields_ = [('case', ctypes.c_uint32), ('calls', ctypes.c_uint32)]


def callback_effect(target, memory, regs, case):
    marker = (target ^ (case * 0x10203) ^ 0x6b17c2a5) & 0xffffffff
    struct.pack_into('<I', memory, 0xb80000 + case * 4, marker)
    for register in range(8):
        regs[register] = (marker + register * 0x01010101) & 0xffffffff
    return marker


def main():
    out = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    assert struct.unpack_from('<I', binary, MODE_POINTER_LITERAL - BASE)[0] == MODE_STORAGE
    assert struct.unpack_from('<I', binary, CALLBACK_LITERAL - BASE)[0] == CALLBACK_TARGET
    lib = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/input_bitset_set.dll'))
    function = lib.sa_input_bitset_set_8c0a6918
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                         CALLBACK, ctypes.c_void_p]
    function.restype = None
    rng = random.Random(ENTRY)
    bit_indices = [-1024, -256, -17, -9, -8, -7, -1, 0, 1, 6, 7, 8,
                   15, 16, 31, 63, 64, 127, 128, 134, 135]
    cases = []

    for case in range(48):
        mode = 5 if case % 2 else (1 if case % 4 == 0 else 0)
        bit_index = bit_indices[case % len(bit_indices)] if case < 32 else rng.randrange(136)
        original = bytearray(b'\xa5') * RAM_SIZE
        original[0x10000:0x10000 + len(binary)] = binary
        struct.pack_into('<I', original, MODE_STORAGE & 0xffffff, mode)
        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.r[4] = 0x8c100000
        expected.r[5] = bit_index & 0xffffffff
        expected.r[15] = STACK_TOP
        expected.sr = rng.getrandbits(32)
        expected.pr = 0x8c001c00 + case * 4
        expected.pc = ENTRY
        actual_state = State.from_buffer_copy(bytes(expected))
        retail_ram, actual_ram = bytearray(original), bytearray(original)
        expected_trace = []

        def hook(memory, regs, sr):
            assert mode == 5
            marker = callback_effect(CALLBACK_TARGET, memory, regs, case)
            expected_trace.append((CALLBACK_TARGET, marker))
            return sr

        steps = reference(retail_ram, expected,
                          hooks={CALLBACK_TARGET: hook} if mode == 5 else None)
        scenario = Scenario(case, 0)
        actual_trace = []

        @CALLBACK
        def dispatch(memory_pointer, state_pointer, target, context):
            settings = ctypes.cast(context, ctypes.POINTER(Scenario)).contents
            assert target == CALLBACK_TARGET
            memory = (ctypes.c_uint8 * RAM_SIZE).from_address(
                ctypes.addressof(memory_pointer.contents))
            state = state_pointer.contents
            regs = list(state.r)
            marker = (target ^ (settings.case * 0x10203) ^ 0x6b17c2a5) & 0xffffffff
            ctypes.memmove(ctypes.addressof(memory_pointer.contents) + 0xb80000 +
                           settings.case * 4, struct.pack('<I', marker), 4)
            for register in range(8):
                state.r[register] = (marker + register * 0x01010101) & 0xffffffff
            settings.calls += 1
            actual_trace.append((target, marker))

        memory_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        function(memory_view, ctypes.byref(actual_state), dispatch, ctypes.byref(scenario))
        stack_low = STACK_OFFSET - 0x100
        assert actual_ram[:stack_low] == retail_ram[:stack_low], f'RAM mismatch below stack, case {case}'
        assert actual_ram[STACK_OFFSET:] == retail_ram[STACK_OFFSET:], f'RAM mismatch above stack, case {case}'
        assert list(actual_state.r[8:]) == list(expected.r[8:]), f'callee-saved registers mismatch, case {case}'
        assert actual_state.pr == expected.pr and actual_state.pc == expected.pc, f'control mismatch, case {case}'
        assert actual_trace == expected_trace, f'callback trace mismatch, case {case}'
        assert scenario.calls == (1 if mode == 5 else 0)
        cases.append({'case': case, 'mode': mode, 'bit_index': bit_index,
                      'callback_count': scenario.calls, 'retail_instructions': steps,
                      'abi_state_and_ram_match_outside_stack': True})

    source = out / 'source/input_bitset_set.c'
    report = {
        'routine_start': hex(ENTRY), 'routine_end_exclusive': hex(END),
        'cases_passed': len(cases),
        'comparison_level': 'Preserved R8-R15, PR/PC, complete RAM outside 256-byte volatile stack region, bitset changes, callback target and dispatch trace. Caller-clobbered R0-R7 and SR are not compared before callback dispatch.',
        'retail_span_sha256': hashlib.sha256(binary[ENTRY - BASE:END - BASE]).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'scope': 'Input bit setter at 0x8c0a6918, including wrapped signed bit indices and the state==5 indirect callback branch. The callback target is dispatched through an injected native adapter.',
        'limitations': [
            'Callback target 0x8c0115d0 is modeled as a deterministic dispatch boundary here; its own body and dependent services have a separate verification record.',
            'Volatile stack writes and caller-clobbered registers are not compared byte-for-byte; ABI-preserved state, data writes, callback target and call count are checked.',
            'Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or end-to-end game execution.'
        ],
        'cases': cases,
    }
    (out / 'analysis/input_bitset_set_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"Input bit setter passed {len(cases)} ABI-level callback and memory comparisons")


if __name__ == '__main__':
    main()
