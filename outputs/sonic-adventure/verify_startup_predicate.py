"""Full-state retail comparison for the early startup status predicate."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ENTRY = 0x8c010b58
END = 0x8c010b7a
GLOBAL_ENABLE = 0x8c161cb8
GLOBAL_MODE = 0x8c161cb4


def main():
    out = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    library_path = ROOT / "work/sonic-adventure/build/startup_predicate.dll"
    function = ctypes.CDLL(str(library_path)).sa_startup_predicate_8c010b58
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None
    rng = random.Random(0x8c010b58)
    mode_values = [0, 1, 2, 3, 4, 5, 0x7fffffff, 0x80000000,
                   0xfffffffc, 0xfffffffd, 0xfffffffe, 0xffffffff]
    cases = []
    for enabled in [0, 1]:
        for mode in mode_values:
            for seed in range(4):
                original = bytearray(16 * 1024 * 1024)
                original[0x10000:0x10000 + len(binary)] = binary
                struct.pack_into("<I", original, GLOBAL_ENABLE & 0xffffff, enabled)
                struct.pack_into("<I", original, GLOBAL_MODE & 0xffffff, mode)
                actual = bytearray(original)
                expected = State()
                expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
                expected.sr = rng.getrandbits(32)
                expected.pr = 0x8c000100 + (len(cases) * 4)
                expected.pc = ENTRY
                actual_state = State.from_buffer_copy(bytes(expected))
                steps = reference(original, expected)
                view = (ctypes.c_uint8 * len(actual)).from_buffer(actual)
                function(view, ctypes.byref(actual_state))
                assert actual == original, f"retail unexpectedly wrote memory in case {len(cases)}"
                assert bytes(actual_state) == bytes(expected), (
                    f"full-state mismatch case={len(cases)} enabled={enabled} mode={mode:#010x}"
                )
                cases.append({"enabled": enabled, "mode": hex(mode), "seed": seed,
                              "retail_instructions_executed": steps,
                              "full_cpu_state_matches": True})

    start, end = ENTRY, END
    report = {
        "cases_passed": len(cases),
        "routine_start": hex(start),
        "routine_end_exclusive": hex(end),
        "retail_span_sha256": hashlib.sha256(
            binary[start - 0x8c010000:end - 0x8c010000]).hexdigest(),
        "source": "source/startup_predicate.c",
        "source_sha256": hashlib.sha256(
            (out / "source/startup_predicate.c").read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(library_path.read_bytes()).hexdigest(),
        "global_values": {"enable": hex(GLOBAL_ENABLE), "mode": hex(GLOBAL_MODE)},
        "cases": cases,
        "scope": "Full GPR/SR/PR/PC and 16 MiB RAM compared against the retail SH-4 interpreter; both FPU-independent branches, signed comparison boundaries, and all predicate return paths are covered.",
        "limitations": ["The indirect callers and the meaning/lifecycle of the two globals remain unverified.",
                        "No whole-game startup or hardware behavior is claimed."],
    }
    (out / "analysis/startup_predicate_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} startup predicate full-state comparisons passed")


if __name__ == "__main__":
    main()
