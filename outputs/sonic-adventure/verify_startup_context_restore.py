"""Compare the recovered startup context restore with the retail SH-4 body."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference
from verify_fpu_cleanup_transfer import Fpu

ENTRY = 0x8c010810
END = 0x8c010878
MODE = 0x8c19b468
CONTEXT_POINTER = 0x8c749130
CONTEXT = 0x8c8e0000
RAM_SIZE = 16 * 1024 * 1024


def main():
    out = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    library_path = ROOT / "work/sonic-adventure/build/startup_context_restore.dll"
    library = ctypes.CDLL(str(library_path))
    function = library.sa_startup_context_restore_8c010810
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                         ctypes.POINTER(Fpu)]
    function.restype = None
    rng = random.Random(ENTRY)
    cases = []
    scenarios = [(mode, 0, 0) for mode in (0, 2, 0xffffffff)]
    scenarios += [(1, width, fr) for width in (0, 1) for fr in (0, 1)
                  for _ in range(6)]

    for case_number, (mode, width, fr) in enumerate(scenarios):
        original = bytearray(b"\xa5") * RAM_SIZE
        original[0x10000:0x10000 + len(binary)] = binary
        context_offset = CONTEXT & 0xffffff
        original[context_offset:context_offset + 0x80] = bytes(
            rng.getrandbits(8) for _ in range(0x80))
        struct.pack_into("<I", original, CONTEXT_POINTER & 0xffffff, CONTEXT)
        struct.pack_into("<I", original, MODE & 0xffffff, mode)
        original[0x6ac58c:0x6ac5b0] = bytes(rng.getrandbits(8) for _ in range(0x24))
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
        expected_fpu.fpscr = ((rng.getrandbits(32) & ~0x00380000) |
                              (width << 20) | (fr << 21))
        actual_fpu = Fpu.from_buffer_copy(bytes(expected_fpu))
        steps = reference(original, expected, fpu=expected_fpu)

        view = (ctypes.c_uint8 * len(actual)).from_buffer(actual)
        function(view, ctypes.byref(actual_state), ctypes.byref(actual_fpu))
        if actual != original:
            changed = [(hex(i), original[i:i + 8].hex(), actual[i:i + 8].hex())
                       for i in range(len(actual)) if actual[i] != original[i]][:20]
            raise AssertionError(f"RAM mismatch case={case_number} mode={mode:#x}: {changed}")
        assert bytes(actual_state) == bytes(expected), (
            f"CPU mismatch case={case_number} mode={mode:#x}: "
            f"native={[hex(x) for x in actual_state.r]}, "
            f"retail={[hex(x) for x in expected.r]}"
        )
        assert bytes(actual_fpu) == bytes(expected_fpu), (
            f"FPU mismatch case={case_number} mode={mode:#x} "
            f"SZ={width} FR={fr}"
        )
        cases.append({"case": case_number, "mode": hex(mode), "fpscr_sz": width,
                      "fpscr_fr": fr, "retail_instructions_executed": steps,
                      "full_ram_cpu_fpu_match": True})

    source_path = out / "source/startup_context_restore.c"
    report = {
        "cases_passed": len(cases),
        "routine_start": hex(ENTRY),
        "routine_end_exclusive": hex(END),
        "retail_span_sha256": hashlib.sha256(
            binary[ENTRY - 0x8c010000:END - 0x8c010000]).hexdigest(),
        "source_sha256": hashlib.sha256(source_path.read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(library_path.read_bytes()).hexdigest(),
        "cases": cases,
        "scope": "Full 16 MiB RAM and complete GPR/SR/PR/PC plus both FPU banks, FPSCR and FPUL compared against retail. Covers mode-gated return, scalar and double-width context transfers, both FR banks, sign-extended 16-bit fields and final register state.",
        "limitations": [
            "The verifier uses a partial SH-4 instruction interpreter, not physical Dreamcast hardware.",
            "The recovered leaf's callers and complete game behavior remain separately unverified.",
            "Randomized FPSCR cases exercise SZ and FR; exceptional FPU configuration behavior has not been hardware-validated."
        ]
    }
    (out / "analysis/startup_context_restore_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} startup context-restore full-state comparisons passed")


if __name__ == "__main__":
    main()
