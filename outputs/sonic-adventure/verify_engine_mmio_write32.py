"""Compare the recovered Dreamcast MMIO word writer against retail SH-4 code."""
import ctypes
import hashlib
import itertools
import json
import random

from verify_fpu_cleanup_transfer import Fpu
from verify_startup import ROOT, State, reference

Write32 = ctypes.CFUNCTYPE(ctypes.c_int, ctypes.c_void_p,
                           ctypes.c_uint32, ctypes.c_uint32)
P = ctypes.POINTER(ctypes.c_uint32)


def main():
    project = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    source = project / "source/sa_engine_mmio_write32.c"
    listing = project / "analysis/instructions/video_setup_region.tsv"
    dll_path = ROOT / "work/sonic-adventure/build/engine_mmio_write32.dll"
    fn = ctypes.CDLL(str(dll_path)).sa_engine_mmio_write32
    ByteP = ctypes.POINTER(ctypes.c_uint8)
    fn.argtypes = [ByteP, ctypes.POINTER(Fpu), P, P, P, P, P,
                   ctypes.c_void_p, Write32]
    fn.restype = ctypes.c_int

    offsets = [0, 4, 8, 0x40, 0x78, 0xb0, 0xffc, 0x1234,
               0xfffffffc, 0x80000000,
               0x12345678, 0xffffff00]
    cases, total_steps = [], 0
    for offset, bank, seed in itertools.product(offsets, [0, 0x200000], range(2)):
        rng = random.Random(0x653444 ^ offset ^ bank ^ seed)
        ram = bytearray(16 * 1024 * 1024)
        ram[0x10000:0x10000 + len(binary)] = binary
        state = State()
        state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        state.r[4], state.r[5] = offset, rng.getrandbits(32)
        state.r[15] = 0x8cf00000
        state.sr = 0x70000020 | seed
        state.pr, state.pc = 0x8c000100, 0x8c653444
        fpu = Fpu()
        fpu.fpscr = bank
        for b in range(2):
            for i in range(16):
                fpu.banks[b][i] = rng.getrandbits(32)
        fpul_value, macl_value = rng.getrandbits(32), rng.getrandbits(32)

        retail_ram = bytearray(ram)
        retail_state = State.from_buffer_copy(bytes(state))
        retail_fpu = Fpu.from_buffer_copy(bytes(fpu))
        initial_address = (0xa05f8000 + offset) & 0xffffffff
        retail_mmio = {initial_address: rng.getrandbits(32)}
        steps = reference(retail_ram, retail_state, fpu=retail_fpu,
                          mmio=retail_mmio, stop_pc=state.pr)
        total_steps += steps

        native_ram = bytearray(ram)
        view = (ctypes.c_uint8 * len(native_ram)).from_buffer(native_ram)
        regs = (ctypes.c_uint32 * 16)(*state.r)
        sr, pr = ctypes.c_uint32(state.sr), ctypes.c_uint32(state.pr)
        fpul, macl = ctypes.c_uint32(fpul_value), ctypes.c_uint32(macl_value)
        native_mmio = {initial_address: retail_mmio[initial_address]}
        events = []

        @Write32
        def write_register(_context, address, value):
            native_mmio[address] = value
            events.append((address, value))
            return 0

        assert fn(view, ctypes.byref(fpu), ctypes.byref(sr), regs,
                  ctypes.byref(pr), ctypes.byref(fpul), ctypes.byref(macl),
                  None, write_register) == 0

        assert native_ram == retail_ram
        assert list(regs) == list(retail_state.r)
        assert sr.value == retail_state.sr and pr.value == retail_state.pr
        assert bytes(fpu) == bytes(retail_fpu)
        assert fpul.value == fpul_value and macl.value == macl_value
        assert native_mmio == retail_mmio
        assert events == [(initial_address, state.r[5])]
        cases.append({"register_offset": hex(offset),
                      "absolute_address": hex(initial_address),
                      "value": hex(state.r[5]), "fpscr_bank": hex(bank),
                      "seed": seed, "full_ram_cpu_fpu_match": True,
                      "mmio_write_matches": True,
                      "retail_instructions_executed": steps})

    start, end = 0x8c653444, 0x8c65344e
    file_start, file_end = start - 0x8c010000, end - 0x8c010000
    report = {
        "module": "1ST_READ.BIN", "routine": hex(start),
        "end_exclusive": hex(end), "cases_passed": len(cases),
        "original_instructions_executed": total_steps, "cases": cases,
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[file_start:file_end]).hexdigest(),
        "source": "source/sa_engine_mmio_write32.c",
        "instruction_listing": "analysis/instructions/video_setup_region.tsv",
        "instruction_listing_sha256": hashlib.sha256(listing.read_bytes()).hexdigest(),
        "source_sha256": hashlib.sha256(source.read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Compares all 16 MiB RAM, CPU registers, SR/PR, both FPU banks, FPUL/MACL preservation, and host MMIO write address/value across12 offsets, two FPU banks, and two register seeds.",
        "limitations": ["Dreamcast hardware behavior is represented by a host MMIO callback.",
                        "This routine-level test does not validate physical graphics hardware or full-game integration."],
    }
    (project / "analysis/engine_mmio_write32_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} full-state comparisons passed; {total_steps} retail instructions executed")


if __name__ == "__main__":
    main()
