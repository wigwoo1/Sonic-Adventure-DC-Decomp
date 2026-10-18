"""Verify the two-service startup tail wrapper against retail SH-4 instructions."""
import ctypes
import hashlib
import json
import random
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from verify_startup import ROOT, State, reference

ENTRY = 0x8c010a6e
END = 0x8c010a84
SERVICE = 0x8c608a0e
TAIL = 0x8c6418e8
RAM_SIZE = 16 * 1024 * 1024
TARGETS = [SERVICE, TAIL]
TARGET_INDEX = {target: index for index, target in enumerate(TARGETS)}
DISPATCH = ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_uint8),
                            ctypes.POINTER(State), ctypes.c_uint32,
                            ctypes.c_void_p)


class Scenario(ctypes.Structure):
    _fields_ = [("case", ctypes.c_uint32),
                ("results", ctypes.c_int32 * 2),
                ("serial", ctypes.c_uint32)]


def effect(target, memory, regs, sr, case, serial, result):
    args = tuple(regs[index] for index in range(4, 8))
    stack = regs[15]
    marker = (target ^ stack ^ (case * 0x10203) ^ (serial * 0x30405) ^
              (result & 0xffffffff)) & 0xffffffff
    struct.pack_into("<I", memory, 0xb80000 + case * 8 + serial * 4, marker)
    regs[0] = result & 0xffffffff
    regs[1] ^= (marker + 0x11111111) & 0xffffffff
    regs[2] = (regs[2] + marker) & 0xffffffff
    regs[3] ^= marker ^ 0x22222222
    regs[4] = (regs[4] + 0x33333333) & 0xffffffff
    regs[5] ^= 0x44444444
    regs[6] = (regs[6] - 0x5555) & 0xffffffff
    regs[7] ^= marker >> 3
    sr = (sr & 1) | ((sr ^ marker) & 0xfffffe00)
    return args, stack, marker, sr


def main():
    out = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    library_path = ROOT / "work/sonic-adventure/build/startup_service_wrapper.dll"
    function = ctypes.CDLL(str(library_path)).sa_startup_service_wrapper_8c010a6e
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                         DISPATCH, ctypes.c_void_p]
    function.restype = ctypes.c_int32
    rng = random.Random(ENTRY)
    cases = []

    for case in range(48):
        results = [rng.randrange(-0x7fffffff, 0x7fffffff),
                   rng.randrange(-0x7fffffff, 0x7fffffff)]
        original = bytearray(b"\xa5") * RAM_SIZE
        original[0x10000:0x10000 + len(binary)] = binary
        initial = bytearray(original)
        actual = bytearray(original)
        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.r[15] = 0x8cf00000
        expected.sr = rng.getrandbits(32)
        expected.pr = 0x8c000100 + case * 4
        expected.pc = ENTRY
        actual_state = State.from_buffer_copy(bytes(expected))
        expected_calls, native_calls = [], []
        used = {target: 0 for target in TARGETS}

        def hook_for(target):
            def hook(memory, regs, sr):
                cursor = used[target]
                result = results[TARGET_INDEX[target]]
                assert cursor == 0, f"duplicate retail call {target:#010x}"
                used[target] += 1
                args, stack, marker, new_sr = effect(
                    target, memory, regs, sr, case, len(expected_calls), result)
                expected_calls.append((target, args, stack, marker, expected.pr,
                                       tuple(regs)))
                return new_sr
            return hook

        steps = reference(original, expected,
                          hooks={target: hook_for(target) for target in TARGETS})
        scenario = Scenario(case, (ctypes.c_int32 * 2)(*results), 0)

        @DISPATCH
        def dispatch(memory_pointer, state_pointer, target, scenario_pointer):
            state = state_pointer.contents
            settings = ctypes.cast(scenario_pointer,
                                   ctypes.POINTER(Scenario)).contents
            serial = settings.serial
            settings.serial += 1
            result = settings.results[TARGET_INDEX[target]]
            memory = (ctypes.c_uint8 * RAM_SIZE).from_address(
                ctypes.addressof(memory_pointer.contents))
            args, stack, marker, new_sr = effect(
                target, memory, list(state.r), state.sr, case, serial, result)
            state.r[0] = result & 0xffffffff
            state.r[1] ^= (marker + 0x11111111) & 0xffffffff
            state.r[2] = (state.r[2] + marker) & 0xffffffff
            state.r[3] ^= marker ^ 0x22222222
            state.r[4] = (state.r[4] + 0x33333333) & 0xffffffff
            state.r[5] ^= 0x44444444
            state.r[6] = (state.r[6] - 0x5555) & 0xffffffff
            state.r[7] ^= marker >> 3
            state.sr = new_sr
            native_calls.append((target, args, stack, marker, state.pr,
                                 tuple(state.r)))

        memory_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual)
        native_result = function(memory_view, ctypes.byref(actual_state),
                                 dispatch, ctypes.byref(scenario))
        assert actual == original, f"RAM mismatch case={case}"
        assert bytes(actual_state) == bytes(expected), (
            f"CPU mismatch case={case}: native="
            f"{[hex(x) for x in actual_state.r]} retail="
            f"{[hex(x) for x in expected.r]} native_sr={actual_state.sr:#x} "
            f"retail_sr={expected.sr:#x} native_pr={actual_state.pr:#x} "
            f"retail_pr={expected.pr:#x} expected_calls="
            f"{[(x[0],x[1],x[2],hex(x[3]),x[5][2:8]) for x in expected_calls]} "
            f"native_calls={[(x[0],x[1],x[2],hex(x[3]),x[5][2:8]) for x in native_calls]}")
        assert native_calls == expected_calls, (
            f"call trace mismatch case={case}: "
            f"retail={expected_calls} native={native_calls}")
        assert native_result == ctypes.c_int32(expected.r[0]).value
        cases.append({"case": case, "service_results": results,
                      "callback_count": len(expected_calls),
                      "retail_instructions_executed": steps,
                      "full_state_and_memory_match": True})

    source_path = out / "source/startup_service_wrapper.c"
    report = {
        "cases_passed": len(cases), "routine_start": hex(ENTRY),
        "routine_end_exclusive": hex(END), "instruction_bytes": END - ENTRY,
        "retail_span_sha256": hashlib.sha256(
            binary[ENTRY - 0x8c010000:END - 0x8c010000]).hexdigest(),
        "source_sha256": hashlib.sha256(source_path.read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(library_path.read_bytes()).hexdigest(),
        "callback_targets": [hex(target) for target in TARGETS],
        "cases": cases,
        "scope": "The 22-byte indexed startup tail wrapper compared in 48 randomized cases across full 16 MiB RAM, GPR/SR/PR/PC, callback arguments/order, stack saves and tail return behavior.",
        "limitations": [
            "The external callees at 0x8c608a0e and 0x8c6418e8 are deterministic modeled services; their bodies and game-level meaning are not claimed recovered.",
            "The partial SH-4 interpreter validates selected states, not hardware or end-to-end game behavior."
        ]
    }
    (out / "analysis/startup_service_wrapper_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} startup tail-wrapper full-state comparisons passed")


if __name__ == "__main__":
    main()
