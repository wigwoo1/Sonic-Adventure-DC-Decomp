"""Differentially verify the startup retry helper against retail instructions."""
import ctypes
import hashlib
import json
import random
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from verify_startup import ROOT, State, reference

ENTRY = 0x8c010fcc
END = 0x8c011042
FLAG = 0x8c161cac
SERVICE_A = 0x8c602fb8
SERVICE_B = 0x8c10d3c8
SERVICE_C = 0x8c602fe6
RAM_SIZE = 16 * 1024 * 1024
DISPATCH = ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_uint8),
                            ctypes.POINTER(State), ctypes.c_uint32,
                            ctypes.c_void_p)


class Scenario(ctypes.Structure):
    _fields_ = [("case", ctypes.c_uint32),
                ("counts", ctypes.c_uint32 * 3),
                ("used", ctypes.c_uint32 * 3),
                ("values", (ctypes.c_int32 * 8) * 3),
                ("serial", ctypes.c_uint32)]


def effect(target, memory, regs, sr, case, serial, result):
    args = tuple(regs[index] for index in range(4, 8))
    stack = regs[15]
    marker = (target ^ stack ^ (case * 0x10203) ^ (serial * 0x30405) ^
              (result & 0xffffffff)) & 0xffffffff
    struct.pack_into("<I", memory, 0xb80000 + case * 0x20 + serial * 4, marker)
    regs[0] = result & 0xffffffff
    regs[1] ^= (marker + 0x11111111) & 0xffffffff
    regs[2] = (regs[2] + marker) & 0xffffffff
    regs[3] ^= (marker ^ 0x22222222) & 0xffffffff
    regs[4] = (regs[4] + 0x33333333) & 0xffffffff
    regs[5] ^= 0x44444444
    regs[6] = (regs[6] - 0x5555) & 0xffffffff
    regs[7] ^= marker >> 3
    sr = (sr & 1) | ((sr ^ marker) & 0xfffffe00)
    return args, stack, marker, sr


def main():
    out = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    library_path = ROOT / "work/sonic-adventure/build/startup_retry_helper.dll"
    function = ctypes.CDLL(str(library_path)).sa_startup_retry_helper_8c010fcc
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                         DISPATCH, ctypes.c_void_p]
    function.restype = ctypes.c_int32
    rng = random.Random(ENTRY)
    configs = [
        (1, [], [], []),
        (0, [-1], [0], []),
        (0, [0], [0], []),
        (0, [2], [1, 0], [0]),
        (0, [2], [1, 1], [-1]),
        (0, [2], [1, -7, 4], [2, -1]),
    ]
    targets = [SERVICE_A, SERVICE_B, SERVICE_C]
    target_index = {target: index for index, target in enumerate(targets)}
    cases = []

    for case, (flag, results_a, results_b, results_c) in enumerate(configs):
        values_by_target = {SERVICE_A: results_a, SERVICE_B: results_b,
                            SERVICE_C: results_c}
        original = bytearray(b"\xa5") * RAM_SIZE
        original[0x10000:0x10000 + len(binary)] = binary
        struct.pack_into("<I", original, FLAG & 0xffffff, flag)
        initial = bytearray(original)
        actual = bytearray(original)
        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.r[4] = 0x8c161ca8
        expected.r[5] = 0x8c161cb0
        expected.r[15] = 0x8cf00000
        expected.sr = rng.getrandbits(32)
        expected.pr = 0x8c000100 + case * 4
        expected.pc = ENTRY
        actual_state = State.from_buffer_copy(bytes(expected))
        expected_calls, native_calls = [], []
        cursors = {target: 0 for target in targets}

        def hook_for(target):
            def hook(memory, regs, sr):
                cursor = cursors[target]
                result = values_by_target[target][cursor]
                cursors[target] += 1
                args, stack, marker, new_sr = effect(
                    target, memory, regs, sr, case, len(expected_calls), result)
                expected_calls.append((target, args, stack, marker, expected.pr,
                                       tuple(regs)))
                return new_sr
            return hook

        steps = reference(original, expected,
                          hooks={target: hook_for(target) for target in targets})
        scenario = Scenario(case, (ctypes.c_uint32 * 3)(
            len(results_a), len(results_b), len(results_c)),
            (ctypes.c_uint32 * 3)(0, 0, 0),
            ((ctypes.c_int32 * 8) * 3)(), 0)
        for row, values in enumerate((results_a, results_b, results_c)):
            for column, value in enumerate(values):
                scenario.values[row][column] = value

        @DISPATCH
        def dispatch(memory_pointer, state_pointer, target, scenario_pointer):
            state = state_pointer.contents
            settings = ctypes.cast(scenario_pointer,
                                   ctypes.POINTER(Scenario)).contents
            row = target_index[target]
            cursor = settings.used[row]
            assert cursor < settings.counts[row]
            settings.used[row] += 1
            serial = settings.serial
            settings.serial += 1
            result = settings.values[row][cursor]
            memory = (ctypes.c_uint8 * RAM_SIZE).from_address(
                ctypes.addressof(memory_pointer.contents))
            args, stack, marker, new_sr = effect(
                target, memory, list(state.r), state.sr, case, serial, result)
            # Apply the same deterministic provider effects to the live state.
            state.r[0] = result & 0xffffffff
            state.r[1] ^= (marker + 0x11111111) & 0xffffffff
            state.r[2] = (state.r[2] + marker) & 0xffffffff
            state.r[3] ^= (marker ^ 0x22222222) & 0xffffffff
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
        if actual != original:
            retail_changes = [(i, initial[i:i + 4].hex(), original[i:i + 4].hex())
                              for i in range(0, RAM_SIZE, 4)
                              if original[i:i + 4] != initial[i:i + 4]]
            native_changes = [(i, initial[i:i + 4].hex(), actual[i:i + 4].hex())
                              for i in range(0, RAM_SIZE, 4)
                              if actual[i:i + 4] != initial[i:i + 4]]
            assert False, (f"RAM mismatch in case {case}: "
                           f"retail={retail_changes[:12]} "
                           f"native={native_changes[:12]}")
        assert bytes(actual_state) == bytes(expected), (
            f"CPU mismatch case={case}: native="
            f"{[hex(x) for x in actual_state.r]} retail="
            f"{[hex(x) for x in expected.r]} native_sr={actual_state.sr:#x} "
            f"retail_sr={expected.sr:#x} native_pr={actual_state.pr:#x} "
            f"retail_pr={expected.pr:#x} expected_calls={expected_calls} "
            f"native_calls={native_calls}"
        )
        assert native_calls == expected_calls, (
            f"call trace mismatch case={case}: "
            f"retail={expected_calls} native={native_calls}")
        assert native_result == ctypes.c_int32(expected.r[0]).value
        cases.append({"case": case, "gate": flag,
                      "service_results": {hex(target): values_by_target[target]
                                          for target in targets},
                      "calls": len(expected_calls),
                      "retail_instructions_executed": steps,
                      "full_state_and_call_trace_match": True})

    source_path = out / "source/startup_retry_helper.c"
    report = {
        "cases_passed": len(cases), "routine_start": hex(ENTRY),
        "routine_end_exclusive": hex(END), "instruction_bytes": END - ENTRY,
        "retail_span_sha256": hashlib.sha256(
            binary[ENTRY - 0x8c010000:END - 0x8c010000]).hexdigest(),
        "source_sha256": hashlib.sha256(source_path.read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(library_path.read_bytes()).hexdigest(),
        "callback_targets": [hex(target) for target in targets],
        "cases": cases,
        "scope": "Full 118-byte helper span, including stack frame and 20-byte local buffer. Compares complete 16 MiB RAM, GPR/SR/PR/PC, callback order and arguments, initial gate, first-service sign, result-zero and repeated helper/service paths.",
        "limitations": [
            "Services at 0x8c602fb8 and 0x8c602fe6, plus the 14-byte wrapper at 0x8c10d3c8, are deterministic modeled callback boundaries.",
            "Verification uses a partial SH-4 interpreter rather than physical Dreamcast hardware or end-to-end game behavior."
        ]
    }
    (out / "analysis/startup_retry_helper_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} startup retry helper full-state comparisons passed")


if __name__ == "__main__":
    main()
