"""Retail differential verifier for the startup event-poll loop."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

DISPATCH = ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                            ctypes.c_uint32, ctypes.c_void_p)
ENTRY = 0x8c010a0e
END = 0x8c010a3e
TARGET = 0x8c6050b0
OBJECT = 0x8c6ac39c
ARG1 = 0x8c6ac3a0
ARG2 = 0x8c6ac3a4
PENDING = 0x8c6ac3c0


def event(memory, regs, pr, calls, stop_after):
    number = len(calls) + 1
    trace = (TARGET, tuple(regs[i] for i in range(4, 8)), regs[15], pr)
    calls.append(trace)
    marker = (TARGET ^ number ^ regs[4] ^ regs[5] ^ regs[6] ^ regs[7]) & 0xffffffff
    for index in range(4):
        memory[0xb70000 + index] = (marker >> (index * 8)) & 255
    for index in range(8):
        regs[index] = (marker + number * (index + 1)) & 0xffffffff
    if number >= stop_after:
        offset = PENDING & 0xffffff
        for index in range(4):
            memory[offset + index] = 0


def main():
    out = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    library_path = ROOT / 'work/sonic-adventure/build/startup_event_poll.dll'
    function = ctypes.CDLL(str(library_path)).sa_startup_event_poll_8c010a0e
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State), DISPATCH,
                         ctypes.c_void_p]
    rng = random.Random(ENTRY)
    cases = []
    for case, stop_after in enumerate([1, 2, 3, 5, 8, 13, 21, 34]):
        original = bytearray(rng.randbytes(16 * 1024 * 1024))
        original[0x10000:0x10000 + len(binary)] = binary
        actual = bytearray(original)
        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.r[15] = 0x8cf00000
        expected.pr = 0x8c000100 + case * 4
        expected.pc = ENTRY
        inputs = tuple(expected.r[i] for i in range(4, 7))
        original_r14 = expected.r[14]
        native_state = State.from_buffer_copy(bytes(expected))
        expected_calls, native_calls = [], []

        def hook(memory, regs, sr):
            event(memory, regs, expected.pr, expected_calls, stop_after)
            return sr

        steps = reference(original, expected, hooks={TARGET: hook})

        @DISPATCH
        def dispatch(memory_ptr, state_ptr, target, context):
            state = state_ptr.contents
            if target != TARGET:
                raise AssertionError(f'unexpected event target {target:#010x}')
            if len(native_calls) >= stop_after:
                raise AssertionError('callback repeated after completion threshold')
            event(memory_ptr, state.r, state.pr, native_calls, stop_after)

        view = (ctypes.c_uint8 * len(actual)).from_buffer(actual)
        function(view, ctypes.byref(native_state), dispatch, None)
        assert bytes(actual) == bytes(original), f'RAM mismatch case {case}'
        assert bytes(native_state) == bytes(expected), f'CPU-state mismatch case {case}'
        if native_calls != expected_calls: raise AssertionError((case, native_calls, expected_calls))
        assert len(native_calls) == stop_after
        assert struct.unpack_from('<I', actual, OBJECT & 0xffffff)[0] == 2
        assert struct.unpack_from('<I', actual, ARG1 & 0xffffff)[0] == inputs[0]
        assert struct.unpack_from('<I', actual, ARG2 & 0xffffff)[0] == inputs[1]
        assert struct.unpack_from('<I', actual, (OBJECT + 12) & 0xffffff)[0] == inputs[2]
        assert struct.unpack_from('<I', actual, PENDING & 0xffffff)[0] == 0
        assert native_state.r[14] == original_r14 and native_state.r[15] == 0x8cf00000
        cases.append({'case': case, 'handler_calls': stop_after,
                      'retail_instructions_executed': steps,
                      'full_state_and_events_match': True})

    report = {
        'cases_passed': len(cases), 'routine_start': hex(ENTRY),
        'routine_end_exclusive': hex(END),
        'retail_span_sha256': hashlib.sha256(
            binary[ENTRY - 0x8c010000:END - 0x8c010000]).hexdigest(),
        'source': 'source/startup_event_poll.c',
        'source_sha256': hashlib.sha256((out / 'source/startup_event_poll.c').read_bytes()).hexdigest(),
        'dll_sha256': hashlib.sha256(library_path.read_bytes()).hexdigest(),
        'cases': cases,
        'scope': 'Full 16 MiB RAM and GPR/SR/PR/PC compared to retail across eight loop lengths (1 to 34 calls); callback target, per-iteration PR/stack state, event arguments and mutations, four initialized global fields, pending-flag exit, and r14/stack restoration are checked.',
        'limitations': ['The event handler at 0x8c6050b0 is a deterministic modeled callback; its implementation and hardware/game semantics are not recovered by this wrapper.'],
    }
    (out / 'analysis/startup_event_poll_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"{len(cases)} startup event-poll full-state comparisons passed")


if __name__ == '__main__':
    main()
