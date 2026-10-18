"""Differentially verify the startup device-info copy wrapper."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

ENTRY = 0x8c010f9a
END = 0x8c010fcc
TARGET = 0x8c10baaa
RAM_SIZE = 16 * 1024 * 1024
DISPATCH = ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_uint8),
                            ctypes.POINTER(State), ctypes.c_uint32,
                            ctypes.c_void_p)


def callback_effect(target, memory, regs, case):
    stack = regs[4]
    seed = (target ^ stack ^ (case * 0x1020304)) & 0xffffffff
    data = bytes(((seed >> ((index & 3) * 8)) + index * 37 + case) & 255
                 for index in range(8))
    at = stack & 0xffffff
    if isinstance(memory, bytearray):
        memory[at:at + len(data)] = data
    else:
        ctypes.memmove(ctypes.addressof(memory.contents) + at, data, len(data))
    regs[0] = (seed ^ 0x12345678) & 0xffffffff
    regs[1] = (regs[1] ^ seed) & 0xffffffff
    regs[2] = (regs[2] + case + 0x100) & 0xffffffff
    regs[3] = (regs[3] ^ 0x87654321) & 0xffffffff
    regs[4] = (regs[4] ^ 0x55aa55aa) & 0xffffffff
    return data


def main():
    out = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    library_path = ROOT / "work/sonic-adventure/build/startup_copy_device_info.dll"
    function = ctypes.CDLL(str(library_path)).sa_startup_copy_device_info_8c010f9a
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                         DISPATCH, ctypes.c_void_p]
    function.restype = None
    rng = random.Random(ENTRY)
    cases = []

    for case in range(32):
        original = bytearray(rng.randbytes(RAM_SIZE))
        original[0x10000:0x10000 + len(binary)] = binary
        actual = bytearray(original)
        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.r[4] = 0x8c900000 + case * 0x20
        expected.r[14] = rng.getrandbits(32)
        expected.r[15] = 0x8cf00000
        expected.sr = rng.getrandbits(32)
        expected.pr = 0x8c000100 + case * 4
        expected.pc = ENTRY
        actual_state = State.from_buffer_copy(bytes(expected))
        destination = expected.r[4] & 0xffffff
        expected_calls, native_calls = [], []

        def hook(memory, regs, sr):
            if expected_calls:
                raise AssertionError("unexpected repeated callback")
            expected_calls.append((TARGET, regs[4], expected.pr))
            callback_effect(TARGET, memory, regs, case)
            return sr

        steps = reference(original, expected, hooks={TARGET: hook})

        @DISPATCH
        def dispatch(memory_ptr, state_ptr, target, _context):
            state = state_ptr.contents
            native_calls.append((target, state.r[4], state.pr))
            callback_effect(target, memory_ptr, state.r, case)

        view = (ctypes.c_uint8 * len(actual)).from_buffer(actual)
        function(view, ctypes.byref(actual_state), dispatch, None)
        if actual != original:
            changed = [(hex(i), original[i:i + 8].hex(), actual[i:i + 8].hex())
                       for i in range(len(actual)) if actual[i] != original[i]][:12]
            raise AssertionError(f"RAM mismatch in case {case}: {changed}")
        assert bytes(actual_state) == bytes(expected), f"CPU-state mismatch in case {case}"
        assert native_calls == expected_calls, f"callback boundary mismatch in case {case}"
        assert len(native_calls) == 1 and native_calls[0][0] == TARGET
        destination = 0x900000 + case * 0x20
        stack_output = bytes(original[0xefffec:0xeffff4])
        expected_output = stack_output
        assert bytes(original[destination:destination + 8]) == expected_output, (
            f"retail output bytes differ from encoded stores in case {case}"
        )
        cases.append({"case": case, "retail_instructions_executed": steps,
                      "callback_target": hex(TARGET),
                      "full_ram_cpu_state_and_output_match": True})

    source_path = out / "source/startup_copy_device_info.c"
    report = {
        "cases_passed": len(cases), "routine_start": hex(ENTRY),
        "routine_end_exclusive": hex(END),
        "retail_span_sha256": hashlib.sha256(
            binary[ENTRY - 0x8c010000:END - 0x8c010000]).hexdigest(),
        "source_sha256": hashlib.sha256(source_path.read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(library_path.read_bytes()).hexdigest(),
        "cases": cases,
        "scope": "Full 16 MiB RAM and GPR/SR/PR/PC, callback target/argument/stack boundary, saved-register restoration, and all eight copied output bytes compared against retail execution in 32 randomized cases.",
        "limitations": [
            "The callee at 0x8c10baaa is modeled as a deterministic device-info provider; its body and the hardware service it represents are not claimed recovered.",
            "Verification uses a partial SH-4 interpreter and does not establish physical Dreamcast or complete-game behavior."
        ]
    }
    (out / "analysis/startup_copy_device_info_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} startup device-info copy full-state comparisons passed")


if __name__ == "__main__":
    main()
