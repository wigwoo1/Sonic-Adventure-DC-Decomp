"""Compare the recovered startup context copy with the retail SH-4 body."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference
from verify_fpu_cleanup_transfer import Fpu

ENTRY = 0x8c0107a6
END = 0x8c010810
MODE = 0x8c19b468
CONTEXT_POINTER = 0x8c749130
CONTEXT = 0x8c8e0000
OUTPUTS = (0x8c6ac58c, 0x8c6ac58e, 0x8c6ac590, 0x8c6ac592,
           0x8c6ac594, 0x8c6ac598, 0x8c6ac59c, 0x8c6ac5a0,
           0x8c6ac5a4, 0x8c6ac5a8)
RAM_SIZE = 16 * 1024 * 1024


def main():
    out = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    library_path = ROOT / "work/sonic-adventure/build/startup_context_copy.dll"
    library = ctypes.CDLL(str(library_path))
    function = library.sa_startup_context_copy_8c0107a6
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                         ctypes.POINTER(Fpu)]
    function.restype = None
    rng = random.Random(ENTRY)
    cases = []

    # The branch-not-taken path must preserve the remaining machine state and memory.
    modes = (0, 2, 0xffffffff)
    scenarios = [(mode, 0, 0) for mode in modes]
    scenarios += [(1, width, fr) for width in (0, 1) for fr in (0, 1)
                  for _ in range(6)]
    for case_number, (mode, width, fr) in enumerate(scenarios):
        original = bytearray(b"\xa5") * RAM_SIZE
        original[0x10000:0x10000 + len(binary)] = binary
        data = bytes(rng.getrandbits(8) for _ in range(0x40))
        context_offset = CONTEXT & 0x00ffffff
        original[context_offset:context_offset + len(data)] = data
        struct.pack_into("<I", original, CONTEXT_POINTER & 0xffffff, CONTEXT)
        struct.pack_into("<I", original, MODE & 0xffffff, mode)
        struct.pack_into("<H", original, 0x19b488, rng.getrandbits(16))
        actual = bytearray(original)

        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.sr = rng.getrandbits(32)
        expected.pr = 0x8c000100 + case_number * 4
        expected.pc = ENTRY
        actual_state = State.from_buffer_copy(bytes(expected))

        expected_fpu = Fpu()
        for bank in range(2):
            expected_fpu.banks[bank][:] = [rng.getrandbits(32) for _ in range(16)]
        # Keep PR clear for the routine's FMOV instructions; exercise FR and SZ.
        expected_fpu.fpscr = ((rng.getrandbits(32) & ~0x00380000) |
                              (width << 20) | (fr << 21))
        actual_fpu = Fpu.from_buffer_copy(bytes(expected_fpu))
        initial_fpu = Fpu.from_buffer_copy(bytes(expected_fpu))
        steps = reference(original, expected, fpu=expected_fpu)

        view = (ctypes.c_uint8 * len(actual)).from_buffer(actual)
        function(view, ctypes.byref(actual_state), ctypes.byref(actual_fpu))

        assert actual == original, f"RAM differs in case {case_number} mode={mode:#x}"
        assert bytes(actual_state) == bytes(expected), (
            f"CPU-state mismatch case={case_number} mode={mode:#x}: "
            f"native={[hex(x) for x in actual_state.r]}, "
            f"retail={[hex(x) for x in expected.r]}"
        )
        assert bytes(actual_fpu) == bytes(expected_fpu), (
            f"FPU-state mismatch case={case_number} mode={mode:#x} "
            f"SZ={width} FR={fr} native_fpscr={actual_fpu.fpscr:#x} "
            f"retail_fpscr={expected_fpu.fpscr:#x} "
            f"initial_bank0={[hex(v) for v in initial_fpu.banks[0]]} "
            f"context={[hex(v) for v in struct.unpack_from('<8I', original, context_offset + 0x20)]} "
            f"native_bank0={[hex(v) for v in actual_fpu.banks[0]]} "
            f"retail_bank0={[hex(v) for v in expected_fpu.banks[0]]} "
            f"native_bank1={[hex(v) for v in actual_fpu.banks[1]]} "
            f"retail_bank1={[hex(v) for v in expected_fpu.banks[1]]}"
        )
        cases.append({"case": case_number, "mode": hex(mode), "fpscr_sz": width,
                      "fpscr_fr": fr, "retail_instructions_executed": steps,
                      "full_ram_cpu_fpu_match": True})

    source_path = out / "source/startup_context_copy.c"
    report = {
        "cases_passed": len(cases),
        "routine_start": hex(ENTRY),
        "routine_end_exclusive": hex(END),
        "retail_span_sha256": hashlib.sha256(
            binary[ENTRY - 0x8c010000:END - 0x8c010000]).hexdigest(),
        "source_sha256": hashlib.sha256(source_path.read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(library_path.read_bytes()).hexdigest(),
        "cases": cases,
        "scope": "Full 16 MiB RAM and complete GPR/SR/PR/PC plus both FPU banks, FPSCR and FPUL compared against the retail instruction interpreter. Covers mode-gated no-op paths, six context fields, six scalar/double FPU transfers, both FR banks and both SZ widths.",
        "limitations": [
            "The retail reference is a partial SH-4 interpreter and does not constitute physical-hardware or complete-game verification.",
            "Randomized testing covers this routine's observed FMOV behavior; exceptional FPSCR combinations are not hardware-validated."
        ]
    }
    (out / "analysis/startup_context_copy_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} startup context-copy full-state comparisons passed")


if __name__ == "__main__":
    main()
