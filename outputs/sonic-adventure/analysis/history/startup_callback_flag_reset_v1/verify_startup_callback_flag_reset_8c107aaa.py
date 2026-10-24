"""Compare the callback-gated startup flag cleanup with the original SH-4 body."""
import ctypes
import hashlib
import json
import random
import struct
from pathlib import Path

from verify_startup import ROOT, State, reference

ENTRY = 0x8C107AAA
END = 0x8C107AC6
FLAG_POINTER_LITERAL = 0x8C107B80
CALLBACK_LITERAL = 0x8C107B84
CALLBACK = 0x8C09865C
FLAG = 0x8C5FBABC
RAM_SIZE = 16 * 1024 * 1024
DISPATCH = ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_uint8),
                            ctypes.POINTER(State), ctypes.c_uint32,
                            ctypes.c_void_p)


class Scenario(ctypes.Structure):
    _fields_ = [("case", ctypes.c_uint32), ("calls", ctypes.c_uint32)]


def callback_effect(memory, regs, sr, target, case, serial):
    """Deterministic test boundary for the registered task-disposal callee."""
    args = tuple(regs[index] for index in range(4, 8))
    stack = regs[15]
    call_pr = 0x8C107AB8
    marker = (target ^ stack ^ (case * 0x10203) ^ (serial * 0x30405) ^
              regs[4] ^ regs[5]) & 0xFFFFFFFF
    struct.pack_into("<I", memory, 0xB80000 + case * 4, marker)
    regs[0] = (marker ^ 0x13579BDF) & 0xFFFFFFFF
    regs[1] = (regs[1] + marker) & 0xFFFFFFFF
    regs[2] ^= (marker + 0x2468ACE0) & 0xFFFFFFFF
    regs[3] = (regs[3] - 0x1020304) & 0xFFFFFFFF
    regs[4] ^= 0x55667788
    regs[5] = (regs[5] + 0x12345678) & 0xFFFFFFFF
    regs[6] ^= 0xA5A55A5A
    regs[7] = (regs[7] - 0x31415926) & 0xFFFFFFFF
    new_sr = (sr & 1) | ((sr ^ marker) & 0xFFFFFFFE)
    return (target, args, stack, call_pr, marker), new_sr


def main():
    project = ROOT / "outputs/sonic-adventure"
    binary_path = ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN"
    binary = binary_path.read_bytes()
    assert len(binary) >= END - 0x8C010000
    expected_words = (0xD335, 0x4F22, 0x6432, 0x2448, 0x8905, 0xD133,
                      0x410B, 0x0009, 0xD231, 0xE300, 0x2232, 0x4F26,
                      0x000B, 0x0009)
    actual_words = tuple(struct.unpack_from("<H", binary,
                                             ENTRY - 0x8C010000 + i * 2)[0]
                         for i in range(len(expected_words)))
    assert actual_words == expected_words, (actual_words, expected_words)

    library_path = ROOT / "work/sonic-adventure/build/startup_callback_flag_reset_8c107aaa.dll"
    library = ctypes.CDLL(str(library_path))
    function = library.sa_startup_callback_flag_reset_8c107aaa
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                         DISPATCH, ctypes.c_void_p]
    function.restype = ctypes.c_int32
    rng = random.Random(ENTRY)
    gate_values = (0, 1, 0xFFFFFFFF, 0x12345678)
    cases = []
    coverage = set()
    total = 48

    for case in range(total):
        original = bytearray(b"\xa5") * RAM_SIZE
        original[0x10000:0x10000 + len(binary)] = binary
        flag_value = gate_values[case % len(gate_values)]
        struct.pack_into("<I", original, FLAG - 0x8C000000, flag_value)
        initial = bytearray(original)
        actual = bytearray(original)
        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.r[15] = 0x8CF00000 - (case % 8) * 0x100
        expected.sr = rng.getrandbits(32)
        expected.pr = 0x8C000100 + case * 4
        expected.pc = ENTRY
        actual_state = State.from_buffer_copy(bytes(expected))
        expected_calls = []
        visited = []

        def trace(pc, word, regs, sr, pr, fpu):
            visited.append(pc)

        def dispose_hook(memory, regs, sr):
            assert regs[4] == flag_value, (case, regs[4], flag_value)
            call, new_sr = callback_effect(memory, regs, sr, CALLBACK, case, 0)
            expected_calls.append(call)
            return new_sr

        steps = reference(initial, expected, hooks={CALLBACK: dispose_hook},
                          trace=trace)

        scenario = Scenario(case, 0)
        native_calls = []

        @DISPATCH
        def dispatch(memory_pointer, state_pointer, target, scenario_pointer):
            state = state_pointer.contents
            settings = ctypes.cast(scenario_pointer,
                                   ctypes.POINTER(Scenario)).contents
            assert target == CALLBACK
            assert settings.calls == 0
            memory = (ctypes.c_uint8 * RAM_SIZE).from_address(
                ctypes.addressof(memory_pointer.contents))
            regs = list(state.r)
            call, new_sr = callback_effect(memory, regs, state.sr, target,
                                           settings.case, settings.calls)
            for index, value in enumerate(regs):
                state.r[index] = value
            state.sr = new_sr
            settings.calls += 1
            native_calls.append(call)

        actual_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual)
        native_result = function(actual_view, ctypes.byref(actual_state),
                                  dispatch, ctypes.byref(scenario))
        assert bytes(actual) == bytes(initial), (case, "complete RAM mismatch")
        assert bytes(actual_state) == bytes(expected), (
            case, "complete GPR/SR/PR/PC mismatch",
            [hex(value) for value in actual_state.r],
            [hex(value) for value in expected.r],
            hex(actual_state.sr), hex(expected.sr),
            hex(actual_state.pr), hex(expected.pr))
        assert native_calls == expected_calls, (case, native_calls, expected_calls)
        assert native_result == ctypes.c_int32(expected.r[0]).value
        assert scenario.calls == int(flag_value != 0)
        assert struct.unpack_from("<I", actual, FLAG - 0x8C000000)[0] == 0
        coverage.update(visited)
        cases.append({"case": case, "gate_value": hex(flag_value),
                      "callback_calls": scenario.calls,
                      "instruction_steps": steps,
                      "full_ram_and_machine_state_match": True})

    assert coverage == set(range(ENTRY, END, 2)), sorted(coverage)
    source = project / "source/startup_callback_flag_reset_8c107aaa.c"
    report = {
        "cases_passed": len(cases), "routine_start": hex(ENTRY),
        "routine_end_exclusive": hex(END),
        "instruction_bytes": END - ENTRY,
        "instruction_rows": len(expected_words),
        "covered_rows": len(coverage),
        "retail_span_sha256": hashlib.sha256(
            binary[ENTRY - 0x8C010000:END - 0x8C010000]).hexdigest(),
        "executable_sha256": hashlib.sha256(binary).hexdigest(),
        "source_sha256": hashlib.sha256(source.read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(library_path.read_bytes()).hexdigest(),
        "callback_target": hex(CALLBACK), "callback_calls": sum(
            row["callback_calls"] for row in cases),
        "cases": cases,
        "scope": "Complete 14-instruction conditional flag-reset helper compared against the retail interpreter across zero/nonzero gates, randomized registers/SR, all RAM and callback side effects. The original callback target and call arguments are checked.",
        "limitations": [
            "The indirect callback at 0x8c09865c is a deterministic service boundary here; its independent task-disposal report covers its own routine with destructor/children/work/free services hooked.",
            "The SH-4 interpreter is a software model. This does not prove the complete boot chain, natural scheduling, Dreamcast hardware behavior, visible rendering or gameplay."
        ]
    }
    output = project / "analysis/startup_callback_flag_reset_8c107aaa_verification.json"
    output.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} startup callback flag reset full-state comparisons passed; "
          f"all {len(coverage)} instruction rows reached; "
          f"{report['callback_calls']} modeled callback calls")


if __name__ == "__main__":
    main()
