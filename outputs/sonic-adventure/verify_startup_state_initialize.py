"""Verify the startup initializer with native memory-fill callee integration."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call, P

DISPATCH = ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                            ctypes.c_uint32, ctypes.c_void_p)
ENTRY = 0x8c010b00
END = 0x8c010b38
TARGETS = (0x8c10d31c, 0x8c602b68)
SHIFT_GLOBAL = 0x8c161ca8
FILL_DESTINATION = 0x8c6b23f4
FILL_LENGTH = 0x260


def second_service(target, memory, regs, pr, calls):
    args = tuple(regs[i] for i in range(4, 8))
    calls.append((target, args, regs[15], pr))
    marker = (target ^ args[0] ^ args[1] ^ args[2] ^ args[3]) & 0xffffffff
    for index in range(4):
        memory[0xb60000 + index] = (marker >> (index * 8)) & 255
    for index in range(8):
        regs[index] = (target + args[index % 4] + (index + 1) * 2) & 0xffffffff


@Call
def no_nested_call(ctx, target, memory, fpu, sr, regs, pr):
    return 0

def main():
    out = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    library_path = ROOT / "work/sonic-adventure/build/startup_state_initialize.dll"
    library = ctypes.CDLL(str(library_path))
    function = library.sa_startup_state_initialize_8c010b00
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State), DISPATCH,
                         ctypes.c_void_p]
    fill = library.sa_memory_fill
    fill.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(Fpu), P, P, P, P, P,
                     ctypes.c_void_p, Call]
    shifts = [0, 1, 2, 7, 8, 15, 31, 32, 0x7fffffff, 0x80000000,
              0x80000001, 0xffffffe0, 0xffffffff]
    rng = random.Random(ENTRY)
    shifts += [rng.getrandbits(32) for _ in range(11)]
    cases = []
    for case, shift in enumerate(shifts):
        original = bytearray(rng.randbytes(16 * 1024 * 1024))
        original[0x10000:0x10000 + len(binary)] = binary
        import struct
        struct.pack_into("<I", original, SHIFT_GLOBAL & 0xffffff, shift)
        actual = bytearray(original)
        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.r[15] = 0x8cf00000
        expected.pr = 0x8c000100 + case * 4
        expected.pc = ENTRY
        initial_r7 = expected.r[7]
        initial_pr = expected.pr
        native_state = State.from_buffer_copy(bytes(expected))
        expected_fpu = Fpu()
        expected_fpu.banks[0][:] = [rng.getrandbits(32) for _ in range(16)]
        expected_fpu.banks[1][:] = [rng.getrandbits(32) for _ in range(16)]
        expected_fpu.fpscr = rng.getrandbits(32)
        native_fpu = Fpu.from_buffer_copy(bytes(expected_fpu))
        expected_calls, native_calls = [], []

        def hook(memory, regs, sr):
            second_service(TARGETS[1], memory, regs, expected.pr, expected_calls)
            return sr

        steps = reference(original, expected, hooks={TARGETS[1]: hook}, fpu=expected_fpu)
        fpul = ctypes.c_uint32(0x12345678)
        macl = ctypes.c_uint32(0x87654321)

        @DISPATCH
        def dispatch(memory_ptr, state_ptr, target, context):
            state = state_ptr.contents
            trace = (target, tuple(state.r[i] for i in range(4, 8)),
                     state.r[15], state.pr)
            native_calls.append(trace)
            if target == TARGETS[0]:
                assert trace[1][:3] == (FILL_DESTINATION, 0, FILL_LENGTH)
                assert trace[1][3] == initial_r7
                assert trace[2] == 0x8ceffffc and trace[3] == 0x8c010b10
                native_sr = ctypes.c_uint32(state.sr)
                native_pr = ctypes.c_uint32(state.pr)
                status = fill(memory_ptr, ctypes.byref(native_fpu), ctypes.byref(native_sr),
                              state.r, ctypes.byref(native_pr), ctypes.byref(fpul),
                              ctypes.byref(macl), None, no_nested_call)
                assert status == 0
                state.sr = native_sr.value
                state.pr = native_pr.value
            elif target == TARGETS[1]:
                second_service(target, memory_ptr, state.r, state.pr, [])
            else:
                raise AssertionError(f"unexpected startup service {target:#010x}")

        view = (ctypes.c_uint8 * len(actual)).from_buffer(actual)
        function(view, ctypes.byref(native_state), dispatch, None)
        assert bytes(actual) == bytes(original), f"RAM mismatch case={case} shift={shift:#010x}"
        assert bytes(native_state) == bytes(expected), f"CPU-state mismatch case={case} shift={shift:#010x}"
        assert bytes(native_fpu) == bytes(expected_fpu), f"FPU mismatch case={case}"
        assert len(native_calls) == 2 and native_calls[0][0] == TARGETS[0]
        assert native_calls[1] == expected_calls[0]
        assert native_calls[0][1][:3] == (FILL_DESTINATION, 0, FILL_LENGTH)
        assert native_calls[0][1][3] == initial_r7
        assert native_calls[0][2:] == (0x8ceffffc, 0x8c010b10)
        assert native_calls[1][0] == TARGETS[1]
        expected_shift = ((~shift & 31) + 1) if shift & 0x80000000 else shift & 31
        computed = (1 >> expected_shift) if shift & 0x80000000 and expected_shift < 32 else 0
        if not shift & 0x80000000:
            computed = 1 << expected_shift
        assert native_calls[1][1][1] == computed
        cases.append({"case": case, "shift_input": hex(shift),
                      "callback_shift_value": hex(computed),
                      "retail_instructions_executed": steps,
                      "native_memory_fill_integrated": True,
                      "full_state_and_calls_match": True})

    report = {
        "cases_passed": len(cases), "routine_start": hex(ENTRY),
        "routine_end_exclusive": hex(END),
        "retail_span_sha256": hashlib.sha256(
            binary[ENTRY - 0x8c010000:END - 0x8c010000]).hexdigest(),
        "source": "source/startup_state_initialize.c",
        "source_sha256": hashlib.sha256(
            (out / "source/startup_state_initialize.c").read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(library_path.read_bytes()).hexdigest(),
        "cases": cases,
        "scope": "Full 16 MiB RAM, GPR/SR/PR/PC and both FPU banks compared to retail. The first call executes recovered native sa_memory_fill against retail's actual memory-fill body; the second indirect service is modeled with deterministic volatile-register and memory effects. Positive/negative SHAD boundaries, all call arguments/order, final globals, and PR restoration are covered.",
        "limitations": ["The second callee at 0x8c602b68 is a deterministic modeled callback; its body and wider game semantics remain separate recovery work."],
    }
    (out / "analysis/startup_state_initialize_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} startup state initializer full-state comparisons passed")


if __name__ == "__main__":
    main()

