"""Differentially verify the six-argument internal-name registration wrapper."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ENTRY = 0x8c0113da
END = 0x8c01140a
TARGET = 0x8c603560
RAM_SIZE = 16 * 1024 * 1024
DISPATCH = ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_uint8),
                            ctypes.POINTER(State), ctypes.c_uint32,
                            ctypes.c_void_p)


def apply_callback(target, memory, regs, sr, case, result):
    stack = regs[15]
    args = tuple(regs[i] for i in range(4, 8))
    stacked = (struct.unpack_from("<I", memory, stack & 0xffffff)[0],
               struct.unpack_from("<I", memory, (stack + 4) & 0xffffff)[0])
    assert target == TARGET
    assert args == (struct.unpack_from("<I", memory, 0x161ca8)[0],
                    0x8c161cbc, 0x8c6b2654,
                    struct.unpack_from("<I", memory, 0x6b6858)[0])
    assert stacked == (0x8c6b685c, 0x80000000)

    marker = (target ^ stack ^ case ^ (result & 0xffffffff)) & 0xffffffff
    scratch = 0xb00000 + case * 4
    struct.pack_into("<I", memory, scratch, marker)
    regs[0] = result & 0xffffffff
    regs[1] ^= (marker + 0x10203) & 0xffffffff
    regs[2] = (regs[2] + marker) & 0xffffffff
    regs[3] ^= (marker ^ 0x55667788) & 0xffffffff
    regs[4] ^= 0x11112222
    regs[5] = (regs[5] + 0x33334444) & 0xffffffff
    regs[6] ^= 0x55556666
    regs[7] = (regs[7] - 0x7777) & 0xffffffff
    sr = (sr & 1) | ((sr ^ marker) & 0xfffffe00)
    return args, stack, stacked, sr


def mutate_native(target, memory_pointer, state, case, result, calls):
    memory = (ctypes.c_uint8 * RAM_SIZE).from_address(
        ctypes.addressof(memory_pointer.contents))
    before_sr = state.sr
    # Mirror validation and effects through a temporary memory-compatible byte buffer
    # for the deterministic callback model, then commit the modeled scratch write.
    stack = state.r[15]
    args = tuple(state.r[i] for i in range(4, 8))
    stacked = (struct.unpack_from("<I", memory, stack & 0xffffff)[0],
               struct.unpack_from("<I", memory, (stack + 4) & 0xffffff)[0])
    assert target == TARGET
    assert args == (struct.unpack_from("<I", memory, 0x161ca8)[0],
                    0x8c161cbc, 0x8c6b2654,
                    struct.unpack_from("<I", memory, 0x6b6858)[0])
    assert stacked == (0x8c6b685c, 0x80000000)
    marker = (target ^ stack ^ case ^ (result & 0xffffffff)) & 0xffffffff
    scratch = 0xb00000 + case * 4
    ctypes.memmove(ctypes.addressof(memory_pointer.contents) + scratch,
                   struct.pack("<I", marker), 4)
    state.r[0] = result & 0xffffffff
    state.r[1] ^= (marker + 0x10203) & 0xffffffff
    state.r[2] = (state.r[2] + marker) & 0xffffffff
    state.r[3] ^= (marker ^ 0x55667788) & 0xffffffff
    state.r[4] ^= 0x11112222
    state.r[5] = (state.r[5] + 0x33334444) & 0xffffffff
    state.r[6] ^= 0x55556666
    state.r[7] = (state.r[7] - 0x7777) & 0xffffffff
    state.sr = (before_sr & 1) | ((before_sr ^ marker) & 0xfffffe00)
    calls.append((target, args, stack, stacked, state.pr))


def main():
    out = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    library_path = ROOT / "work/sonic-adventure/build/startup_register_internal_name.dll"
    function = ctypes.CDLL(str(library_path)).sa_startup_register_internal_name_8c0113da
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                         DISPATCH, ctypes.c_void_p]
    function.restype = ctypes.c_int32
    rng = random.Random(ENTRY)
    results = [0, 1, -1, 2, -2, 0x7fffffff, -0x80000000]
    results += [ctypes.c_int32(rng.getrandbits(32)).value for _ in range(25)]
    cases = []

    for case, result in enumerate(results):
        original = bytearray(b"\xa5") * RAM_SIZE
        original[0x10000:0x10000 + len(binary)] = binary
        struct.pack_into("<I", original, 0x161ca8, rng.getrandbits(32))
        struct.pack_into("<I", original, 0x6b6858, rng.getrandbits(32))
        actual = bytearray(original)
        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.r[15] = 0x8cf00000
        expected.sr = rng.getrandbits(32)
        expected.pr = 0x8c000100 + case * 4
        expected.pc = ENTRY
        actual_state = State.from_buffer_copy(bytes(expected))
        expected_calls, native_calls = [], []

        def hook(memory, regs, sr, case=case, result=result):
            args, stack, stacked, new_sr = apply_callback(
                TARGET, memory, regs, sr, case, result)
            expected_calls.append((TARGET, args, stack, stacked, expected.pr))
            return new_sr

        steps = reference(original, expected, hooks={TARGET: hook})

        @DISPATCH
        def dispatch(memory, state_ptr, target, _context):
            state = state_ptr.contents
            mutate_native(target, memory, state, case, result, native_calls)

        memory_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual)
        native_result = function(memory_view, ctypes.byref(actual_state), dispatch, None)
        assert actual == original, f"RAM mismatch in case {case} result={result}"
        assert bytes(actual_state) == bytes(expected), (
            f"CPU mismatch case={case} result={result}: "
            f"native={[hex(x) for x in actual_state.r]}, "
            f"retail={[hex(x) for x in expected.r]}"
        )
        assert native_calls == expected_calls, f"callback boundary mismatch in case {case}"
        expected_result = 0 if result == 0 else -1
        assert native_result == expected_result
        cases.append({"case": case, "callback_result": result,
                      "wrapper_result": native_result,
                      "retail_instructions_executed": steps,
                      "full_state_and_callback_match": True})

    source_path = out / "source/startup_register_internal_name.c"
    report = {
        "cases_passed": len(cases), "routine_start": hex(ENTRY),
        "routine_end_exclusive": hex(END),
        "retail_span_sha256": hashlib.sha256(
            binary[ENTRY - 0x8c010000:END - 0x8c010000]).hexdigest(),
        "source_sha256": hashlib.sha256(source_path.read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(library_path.read_bytes()).hexdigest(),
        "callback_target": hex(TARGET), "cases": cases,
        "scope": "Full 16 MiB RAM and GPR/SR/PR/PC, six callback arguments (four registers and two stack slots), indirect target, return mapping, callback scratch writes, and stack/PR restoration compared with retail in 32 cases.",
        "limitations": [
            "The service at 0x8c603560 is a deterministic modeled callback; its body and hardware/game meaning are not claimed recovered.",
            "The verifier uses a partial SH-4 interpreter and does not establish physical Dreamcast or end-to-end startup behavior."
        ]
    }
    (out / "analysis/startup_register_internal_name_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} internal-name registration full-state comparisons passed")


if __name__ == "__main__":
    main()
