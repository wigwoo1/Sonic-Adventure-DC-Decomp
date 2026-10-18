"""Compare the recovered cache-writeback range wrapper with retail execution."""
import ctypes
import hashlib
import itertools
import json
import random

from verify_fpu_cleanup_transfer import Fpu
from verify_startup import ROOT, State, reference

Writeback = ctypes.CFUNCTYPE(ctypes.c_int, ctypes.c_void_p, ctypes.c_uint32)
P = ctypes.POINTER(ctypes.c_uint32)


def main():
    project = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    source = project / "source/sa_engine_cache_writeback_range.c"
    listing = project / "analysis/instructions/video_setup_region.tsv"
    dll_path = ROOT / "work/sonic-adventure/build/engine_cache_writeback_range.dll"
    fn = ctypes.CDLL(str(dll_path)).sa_engine_cache_writeback_range
    ByteP = ctypes.POINTER(ctypes.c_uint8)
    fn.argtypes = [ByteP, ctypes.POINTER(Fpu), P, P, P, P, P,
                   ctypes.c_void_p, Writeback]
    fn.restype = ctypes.c_int

    ranges = [(0, 0), (0, 1), (0x1000, 0), (0x1000, 0x1f),
              (0x1000, 0x20), (0x1001, 0x3f),
              (0xfffffe00, 0), (0xfffffe00, 0x20),
              (0x10, 0xfffffff0), (0x7fffffe0, 0x40),
              (0xdeadbeef, 0x20), (0x100, 0x100)]
    cases, total_steps, total_writebacks = [], 0, 0
    for (start, length), bank, seed in itertools.product(
            ranges, [0, 0x200000], range(2)):
        rng = random.Random(0x65344e ^ start ^ length ^ bank ^ seed)
        ram = bytearray(16 * 1024 * 1024)
        ram[0x10000:0x10000 + len(binary)] = binary
        state = State()
        state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        state.r[4], state.r[5] = start, length
        state.r[15] = 0x8cf00000
        state.sr = 0x70000020 | seed
        state.pr, state.pc = 0x8c000100, 0x8c65344e
        fpu = Fpu()
        fpu.fpscr = bank
        for b in range(2):
            for i in range(16):
                fpu.banks[b][i] = rng.getrandbits(32)
        fpul_value, macl_value = rng.getrandbits(32), rng.getrandbits(32)

        retail_ram = bytearray(ram)
        retail_state = State.from_buffer_copy(bytes(state))
        retail_fpu = Fpu.from_buffer_copy(bytes(fpu))
        expected_writebacks = []
        steps = reference(retail_ram, retail_state, fpu=retail_fpu,
                          cache_writeback=expected_writebacks.append,
                          stop_pc=state.pr)
        total_steps += steps

        native_ram = bytearray(ram)
        view = (ctypes.c_uint8 * len(native_ram)).from_buffer(native_ram)
        regs = (ctypes.c_uint32 * 16)(*state.r)
        sr, pr = ctypes.c_uint32(state.sr), ctypes.c_uint32(state.pr)
        fpul, macl = ctypes.c_uint32(fpul_value), ctypes.c_uint32(macl_value)
        actual_writebacks = []

        @Writeback
        def writeback(_context, address):
            actual_writebacks.append(address)
            return 0

        assert fn(view, ctypes.byref(fpu), ctypes.byref(sr), regs,
                  ctypes.byref(pr), ctypes.byref(fpul), ctypes.byref(macl),
                  None, writeback) == 0

        assert native_ram == retail_ram
        assert list(regs) == list(retail_state.r)
        assert sr.value == retail_state.sr and pr.value == retail_state.pr
        assert bytes(fpu) == bytes(retail_fpu)
        assert fpul.value == fpul_value and macl.value == macl_value
        assert actual_writebacks == expected_writebacks
        total_writebacks += len(actual_writebacks)
        cases.append({"start": hex(start), "length": hex(length),
                      "fpscr_bank": hex(bank), "seed": seed,
                      "writeback_addresses": [hex(v) for v in actual_writebacks],
                      "full_ram_cpu_fpu_match": True,
                      "retail_instructions_executed": steps})

    start, end = 0x8c65344e, 0x8c653476
    file_start, file_end = start - 0x8c010000, end - 0x8c010000
    report = {
        "module": "1ST_READ.BIN", "routine": hex(start),
        "end_exclusive": hex(end), "cases_passed": len(cases),
        "original_instructions_executed": total_steps,
        "cache_writebacks_observed": total_writebacks, "cases": cases,
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[file_start:file_end]).hexdigest(),
        "source": "source/sa_engine_cache_writeback_range.c",
        "instruction_listing": "analysis/instructions/video_setup_region.tsv",
        "instruction_listing_sha256": hashlib.sha256(listing.read_bytes()).hexdigest(),
        "source_sha256": hashlib.sha256(source.read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Compares complete 16 MiB RAM, CPU, SR/PR, both FPU banks, FPUL/MACL and exact cache-writeback address sequence for twelve boundary and wraparound ranges across two FPU banks and two register seeds.",
        "limitations": ["Cache writeback is represented by an explicit host service callback.",
                        "Inputs that make the 32-byte stepping loop wrap while the end address stays high can fail to terminate and are not included.",
                        "Actual cache coherence, hardware and full-game integration remain unverified."],
    }
    (project / "analysis/engine_cache_writeback_range_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} full-state cases passed; {total_steps} retail instructions and {total_writebacks} writebacks checked")


if __name__ == "__main__":
    main()
