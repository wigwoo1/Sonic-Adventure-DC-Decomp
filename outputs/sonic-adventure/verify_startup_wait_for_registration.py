"""Differentially verify startup registration polling and helper dispatch."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ENTRY = 0x8c0114f0
END = 0x8c011524
INIT = 0x8c088560
POLL = 0x8c6033e4
HELPER = 0x8c0112e6
FLAG = 0x8c161cac
ARGUMENT = 0x8c161ca8
RAM_SIZE = 16 * 1024 * 1024
DISPATCH = ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_uint8),
                            ctypes.POINTER(State), ctypes.c_uint32,
                            ctypes.c_void_p)


class Scenario(ctypes.Structure):
    _fields_ = [("case", ctypes.c_uint32), ("poll_count", ctypes.c_uint32),
                ("poll_limit", ctypes.c_uint32),
                ("poll_values", ctypes.c_int32 * 8),
                ("helper_result", ctypes.c_int32), ("call_serial", ctypes.c_uint32)]


def effect(target, memory, regs, sr, case, serial, result):
    stack = regs[15]
    args = tuple(regs[index] for index in range(4, 8))
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
    regs[7] ^= (marker >> 3)
    sr = (sr & 1) | ((sr ^ marker) & 0xfffffe00)
    return args, stack, marker, sr


def main():
    out = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    library_path = ROOT / "work/sonic-adventure/build/startup_wait_for_registration.dll"
    function = ctypes.CDLL(str(library_path)).sa_startup_wait_for_registration_8c0114f0
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                         DISPATCH, ctypes.c_void_p]
    function.restype = ctypes.c_int32
    rng = random.Random(ENTRY)

    configurations = [
        (1, [], 0), (2, [], 0), (0xffffffff, [], 0),
        (0, [0], 0), (0, [-1, 0], 7), (0, [-1, -1, 1], -3),
        (0, [-1, 0x12345678], 0x7fffffff),
        (0, [-1, -1, -1, -2], -0x80000000),
    ]
    cases = []

    for case, (flag, poll_values, helper_result) in enumerate(configurations):
        original = bytearray(b"\xa5") * RAM_SIZE
        original[0x10000:0x10000 + len(binary)] = binary
        struct.pack_into("<I", original, FLAG & 0xffffff, flag)
        struct.pack_into("<I", original, ARGUMENT & 0xffffff, rng.getrandbits(32))
        actual = bytearray(original)
        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.r[15] = 0x8cf00000
        expected.sr = rng.getrandbits(32)
        expected.pr = 0x8c000100 + case * 4
        expected.pc = ENTRY
        actual_state = State.from_buffer_copy(bytes(expected))
        expected_calls, native_calls = [], []
        serial = [0]

        def hook_for(target):
            def hook(memory, regs, sr):
                call_number = serial[0]
                serial[0] += 1
                if target == INIT:
                    result = 0x13572468
                elif target == POLL:
                    poll_number = sum(1 for call in expected_calls if call[0] == POLL)
                    assert poll_number < len(poll_values)
                    result = poll_values[poll_number]
                else:
                    result = helper_result
                args, stack, marker, new_sr = effect(
                    target, memory, regs, sr, case, call_number, result)
                expected_calls.append((target, args, stack, marker, expected.pr))
                return new_sr
            return hook

        steps = reference(original, expected,
                          hooks={INIT: hook_for(INIT), POLL: hook_for(POLL),
                                 HELPER: hook_for(HELPER)})
        values = (ctypes.c_int32 * 8)()
        for index, value in enumerate(poll_values):
            values[index] = value
        scenario = Scenario(case, 0, len(poll_values), values, helper_result, 0)

        @DISPATCH
        def dispatch(memory_pointer, state_pointer, target, scenario_pointer):
            state = state_pointer.contents
            settings = ctypes.cast(scenario_pointer, ctypes.POINTER(Scenario)).contents
            call_number = settings.call_serial
            settings.call_serial += 1
            if target == INIT:
                result = 0x13572468
            elif target == POLL:
                poll_number = settings.poll_count
                assert poll_number < settings.poll_limit
                result = settings.poll_values[poll_number]
                settings.poll_count += 1
            elif target == HELPER:
                result = settings.helper_result
            else:
                raise AssertionError(f"unexpected startup callback {target:#010x}")

            memory = (ctypes.c_uint8 * RAM_SIZE).from_address(
                ctypes.addressof(memory_pointer.contents))
            stack = state.r[15]
            args = tuple(state.r[index] for index in range(4, 8))
            marker = (target ^ stack ^ (case * 0x10203) ^
                      (call_number * 0x30405) ^ (result & 0xffffffff)) & 0xffffffff
            ctypes.memmove(ctypes.addressof(memory_pointer.contents) +
                           0xb80000 + case * 0x20 + call_number * 4,
                           struct.pack("<I", marker), 4)
            state.r[0] = result & 0xffffffff
            state.r[1] ^= (marker + 0x11111111) & 0xffffffff
            state.r[2] = (state.r[2] + marker) & 0xffffffff
            state.r[3] ^= (marker ^ 0x22222222) & 0xffffffff
            state.r[4] = (state.r[4] + 0x33333333) & 0xffffffff
            state.r[5] ^= 0x44444444
            state.r[6] = (state.r[6] - 0x5555) & 0xffffffff
            state.r[7] ^= marker >> 3
            state.sr = (state.sr & 1) | ((state.sr ^ marker) & 0xfffffe00)
            native_calls.append((target, args, stack, marker, state.pr))

        memory_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual)
        native_result = function(memory_view, ctypes.byref(actual_state), dispatch,
                                 ctypes.byref(scenario))
        assert actual == original, f"RAM mismatch case={case} flag={flag:#x}"
        assert bytes(actual_state) == bytes(expected), (
            f"CPU mismatch case={case} flag={flag:#x}: "
            f"native={[hex(x) for x in actual_state.r]}, "
            f"retail={[hex(x) for x in expected.r]}"
        )
        assert native_calls == expected_calls, f"callback trace mismatch case={case}"
        assert native_result == ctypes.c_int32(expected.r[0]).value
        cases.append({"case": case, "flag": hex(flag),
                      "poll_returns": [int(value) for value in poll_values],
                      "helper_return": helper_result,
                      "calls": len(expected_calls),
                      "retail_instructions_executed": steps,
                      "full_state_and_call_trace_match": True})

    source_path = out / "source/startup_wait_for_registration.c"
    report = {
        "cases_passed": len(cases), "routine_start": hex(ENTRY),
        "routine_end_exclusive": hex(END),
        "retail_span_sha256": hashlib.sha256(
            binary[ENTRY - 0x8c010000:END - 0x8c010000]).hexdigest(),
        "source_sha256": hashlib.sha256(source_path.read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(library_path.read_bytes()).hexdigest(),
        "callback_targets": [hex(INIT), hex(POLL), hex(HELPER)],
        "cases": cases,
        "scope": "Full 16 MiB RAM and GPR/SR/PR/PC, initializer/poller/helper call order, poll argument, retry count, gate return, callback scratch effects and stack/register restoration compared with retail execution.",
        "limitations": [
            "The initializer at 0x8c088560, poll service at 0x8c6033e4, and same-module helper at 0x8c0112e6 are deterministic modeled boundaries, not recovered by this wrapper source.",
            "The verifier uses a partial SH-4 interpreter and does not establish hardware or complete startup behavior."
        ]
    }
    (out / "analysis/startup_wait_for_registration_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} startup registration-wait full-state comparisons passed")


if __name__ == "__main__":
    main()
