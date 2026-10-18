"""Differentially verify the startup transfer coordinator against retail."""
import ctypes
import hashlib
import json
import random
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from verify_startup import ROOT, State, reference

ENTRY = 0x8c011606
END = 0x8c01170e
FLAG = 0x8c161cac
STAGING = 0x8c6b2654
COPY_SOURCE = STAGING + 0x480
COPY_DESTINATION = 0x8c7989a0
COPY_SIZE = 0xde0
RAM_SIZE = 16 * 1024 * 1024
TARGETS = [0x8c6033e4, 0x8c602f5c, 0x8c603474, 0x8c010fcc,
           0x8c603508, 0x8c0887c8, 0x8c088680]
TARGET_INDEX = {target: index for index, target in enumerate(TARGETS)}
DISPATCH = ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_uint8),
                            ctypes.POINTER(State), ctypes.c_uint32,
                            ctypes.c_void_p)


class Scenario(ctypes.Structure):
    _fields_ = [("case", ctypes.c_uint32),
                ("counts", ctypes.c_uint32 * len(TARGETS)),
                ("used", ctypes.c_uint32 * len(TARGETS)),
                ("values", (ctypes.c_int32 * 8) * len(TARGETS)),
                ("serial", ctypes.c_uint32)]


def effect(target, memory, regs, sr, case, serial, result):
    args = tuple(regs[index] for index in range(4, 8))
    stack = regs[15]
    marker = (target ^ stack ^ (case * 0x10203) ^ (serial * 0x30405) ^
              (result & 0xffffffff)) & 0xffffffff
    struct.pack_into("<I", memory, 0xb80000 + case * 0x100 + serial * 4,
                     marker)
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
    library_path = ROOT / "work/sonic-adventure/build/startup_transfer_entry.dll"
    function = ctypes.CDLL(str(library_path)).sa_startup_transfer_entry_8c011606
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                         DISPATCH, ctypes.c_void_p]
    function.restype = ctypes.c_int32
    rng = random.Random(ENTRY)
    cases_config = [
        (1, {target: [] for target in TARGETS}),
        (0, {0x8c6033e4: [0], 0x8c602f5c: [0], 0x8c603474: [7],
             0x8c010fcc: [-1]}),
        (0, {0x8c6033e4: [-1, 0], 0x8c602f5c: [0], 0x8c603474: [7],
             0x8c010fcc: [1]}),
        (0, {0x8c6033e4: [-1, 0, -1, 3], 0x8c602f5c: [0],
             0x8c603474: [7], 0x8c010fcc: [0], 0x8c603508: [5],
             0x8c0887c8: [0]}),
        (0, {0x8c6033e4: [-1, 0, 0], 0x8c602f5c: [0],
             0x8c603474: [0x12345678], 0x8c010fcc: [0],
             0x8c603508: [5], 0x8c0887c8: [1], 0x8c088680: [0]}),
        (0, {0x8c6033e4: [-1, -1, 1, -1, -1, 0],
             0x8c602f5c: [-4], 0x8c603474: [-7], 0x8c010fcc: [0],
             0x8c603508: [0x654321], 0x8c0887c8: [0x7fffffff],
             0x8c088680: [0]}),
    ]
    cases = []

    for case, (flag, results) in enumerate(cases_config):
        original = bytearray(b"\xa5") * RAM_SIZE
        original[0x10000:0x10000 + len(binary)] = binary
        struct.pack_into("<I", original, FLAG & 0xffffff, flag)
        for index in range(COPY_SIZE):
            original[(COPY_SOURCE + index) & 0xffffff] = (index * 37 + 11) & 0xff
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
                result_values = results.get(target, [])
                assert cursor < len(result_values), (
                    f"retail made unexpected call {target:#010x} case={case}")
                result = result_values[cursor]
                used[target] += 1
                args, stack, marker, new_sr = effect(
                    target, memory, regs, sr, case, len(expected_calls), result)
                expected_calls.append((target, args, stack, marker, expected.pr,
                                       tuple(regs)))
                return new_sr
            return hook

        steps = reference(original, expected,
                          hooks={target: hook_for(target) for target in TARGETS})
        scenario = Scenario(case, (ctypes.c_uint32 * len(TARGETS))(
            *(len(results.get(target, [])) for target in TARGETS)),
            (ctypes.c_uint32 * len(TARGETS))(*([0] * len(TARGETS))),
            ((ctypes.c_int32 * 8) * len(TARGETS))(), 0)
        for target, row in TARGET_INDEX.items():
            for column, value in enumerate(results.get(target, [])):
                scenario.values[row][column] = value

        @DISPATCH
        def dispatch(memory_pointer, state_pointer, target, scenario_pointer):
            state = state_pointer.contents
            settings = ctypes.cast(scenario_pointer,
                                   ctypes.POINTER(Scenario)).contents
            row = TARGET_INDEX[target]
            cursor = settings.used[row]
            assert cursor < settings.counts[row], (
                f"native made unexpected call {target:#010x} case={case}")
            settings.used[row] += 1
            serial = settings.serial
            settings.serial += 1
            result = settings.values[row][cursor]
            memory = (ctypes.c_uint8 * RAM_SIZE).from_address(
                ctypes.addressof(memory_pointer.contents))
            args, stack, marker, new_sr = effect(
                target, memory, list(state.r), state.sr, case, serial, result)
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
            expected_changes = [(i, initial[i:i + 4].hex(), original[i:i + 4].hex())
                                for i in range(0, RAM_SIZE, 4)
                                if original[i:i + 4] != initial[i:i + 4]]
            native_changes = [(i, initial[i:i + 4].hex(), actual[i:i + 4].hex())
                              for i in range(0, RAM_SIZE, 4)
                              if actual[i:i + 4] != initial[i:i + 4]]
            assert False, (f"RAM mismatch case={case}: "
                           f"retail={expected_changes[:20]} "
                           f"native={native_changes[:20]}")
        assert bytes(actual_state) == bytes(expected), (
            f"CPU mismatch case={case}: native="
            f"{[hex(x) for x in actual_state.r]} retail="
            f"{[hex(x) for x in expected.r]} native_sr={actual_state.sr:#x} "
            f"retail_sr={expected.sr:#x} expected_calls={expected_calls} "
            f"native_calls={native_calls}"
        )
        assert native_calls == expected_calls, (
            f"callback trace mismatch case={case}: "
            f"retail={expected_calls} native={native_calls}")
        assert native_result == ctypes.c_int32(expected.r[0]).value
        cases.append({"case": case, "gate": flag,
                      "callback_count": len(expected_calls),
                      "return": hex(expected.r[0]),
                      "retail_instructions_executed": steps,
                      "full_state_and_memory_match": True})

    source_path = out / "source/startup_transfer_entry.c"
    report = {
        "cases_passed": len(cases), "routine_start": hex(ENTRY),
        "routine_end_exclusive": hex(END), "instruction_bytes": END - ENTRY,
        "retail_span_sha256": hashlib.sha256(
            binary[ENTRY - 0x8c010000:END - 0x8c010000]).hexdigest(),
        "source_sha256": hashlib.sha256(source_path.read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(library_path.read_bytes()).hexdigest(),
        "callback_targets": [hex(target) for target in TARGETS],
        "cases": cases,
        "scope": "Complete 264-byte indexed body including embedded literals, with runtime paths compared over full 16 MiB RAM, all GPR/SR/PR/PC, stack frame, local identifier, both poll loops, helper/config/read/finalize boundaries, and the 3552-byte triple-copy success path.",
        "limitations": [
            "Services at 0x8c6033e4, 0x8c602f5c, 0x8c603474, 0x8c603508, 0x8c0887c8 and 0x8c088680 are deterministic modeled callbacks. The helper at 0x8c010fcc is independently recovered but is also intercepted as a boundary in this coordinator verifier.",
            "The partial SH-4 interpreter validates retail instruction behavior on these scenarios; it is not a hardware or end-to-end game validation."
        ]
    }
    (out / "analysis/startup_transfer_entry_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} startup transfer coordinator full-state comparisons passed")


if __name__ == "__main__":
    main()
