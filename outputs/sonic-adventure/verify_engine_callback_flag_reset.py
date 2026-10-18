"""Compare the conditional engine flag-reset helper with retail SH-4 execution."""
import ctypes
import hashlib
import itertools
import json
import pathlib
import struct

from verify_fpu_cleanup_transfer import Fpu
from verify_startup import State, reference

ROOT = pathlib.Path(__file__).resolve().parent
BASE = 0x8C000000


def main():
    project = ROOT
    workspace = ROOT.parent.parent
    binary = (workspace / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    source = project / "source/engine_callback_flag_reset.c"
    listing = project / "analysis/instructions/8c651368.tsv"
    dll_path = workspace / "work/sonic-adventure/build/engine_callback_flag_reset.dll"
    native = ctypes.CDLL(str(dll_path)).sa_engine_callback_flag_reset_8c651368
    ByteP = ctypes.POINTER(ctypes.c_uint8)
    U32P = ctypes.POINTER(ctypes.c_uint32)
    native.argtypes = [ByteP, U32P, U32P, U32P]
    native.restype = None

    cases = []
    for flag, bank, mask, marker in itertools.product(
            [0, 1, 0x8C8A2D54, 0xFFFFFFFF], [0, 0x200000], [0, 15], [0, 0x12345678]):
        ram = bytearray(16 * 1024 * 1024)
        ram[0x10000:0x10000 + len(binary)] = binary
        struct.pack_into("<I", ram, 0x67C004, flag)
        state = State()
        state.r[:] = [0x22340000 + i for i in range(16)]
        state.r[15] = 0x8CF00000
        state.sr = 0x70000003 | (mask << 4)
        state.pr = 0x8C000100
        state.pc = 0x8C651368
        fpu = Fpu()
        fpu.fpscr = bank
        for b in range(2):
            for i in range(16):
                fpu.banks[b][i] = (marker + b * 0x1000 + i) & 0xFFFFFFFF

        retail_ram = bytearray(ram)
        retail_state = State.from_buffer_copy(bytes(state))
        retail_fpu = Fpu.from_buffer_copy(fpu)
        steps = reference(retail_ram, retail_state, fpu=retail_fpu,
                          stop_pc=state.pr)

        native_ram = bytearray(ram)
        view = (ctypes.c_uint8 * len(native_ram)).from_buffer(native_ram)
        regs = (ctypes.c_uint32 * 16)(*state.r)
        sr = ctypes.c_uint32(state.sr)
        pr = ctypes.c_uint32(state.pr)
        native(view, ctypes.byref(sr), regs, ctypes.byref(pr))

        assert native_ram == retail_ram
        assert list(regs) == list(retail_state.r)
        assert sr.value == retail_state.sr and pr.value == retail_state.pr
        assert bytes(fpu) == bytes(retail_fpu)
        assert struct.unpack_from("<I", native_ram, 0x67C004)[0] == 0
        cases.append({"flag": flag, "fpscr_bank": bank, "sr_mask": mask,
                      "original_instructions_executed": steps,
                      "full_ram_cpu_fpu_match": True})

    retail_offset = 0x651368 - 0x010000
    span_end = 0x651384 - 0x010000
    report = {
        "module": "1ST_READ.BIN",
        "routine": "0x8c651368",
        "end_exclusive": "0x8c651384",
        "cases_passed": len(cases),
        "original_instructions_executed": sum(c["original_instructions_executed"] for c in cases),
        "cases": cases,
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[retail_offset:span_end]).hexdigest(),
        "source": "source/engine_callback_flag_reset.c",
        "instruction_listing": "analysis/instructions/8c651368.tsv",
        "instruction_listing_sha256": hashlib.sha256(listing.read_bytes()).hexdigest(),
        "source_sha256": hashlib.sha256(source.read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Full 16 MiB RAM, all general registers, SR, PR, and both FPU banks compared against retail execution for 32 inputs covering zero/nonzero flag branches, FPSCR banks, SR masks, and distinct backing-store markers. The retail conditional callee at 0x8c665a54 executes its actual RTS/NOP instructions.",
        "limitations": ["The SH-4 interpreter is a test harness, not hardware emulation.",
                        "This helper test does not establish complete engine initialization or game playability."],
    }
    (project / "analysis/engine_callback_flag_reset_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} complete-state retail comparisons passed; "
          f"{report['original_instructions_executed']} original instructions executed")


if __name__ == "__main__":
    main()
