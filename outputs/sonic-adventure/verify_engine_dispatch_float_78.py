"""Compare the FR4 sign-mask dispatcher wrapper with retail SH-4/FPU execution."""
import ctypes
import hashlib
import json
import pathlib

from verify_fpu_cleanup_transfer import Fpu
from verify_startup import ROOT, State, reference
from verify_tikal_controller import Call, P


def main():
    project = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    source = project / "source/engine_dispatch_float_78.c"
    listing = project / "analysis/instructions/8c652d3a.tsv"
    dll_path = ROOT / "work/sonic-adventure/build/engine_dispatch_float_78.dll"
    fn = ctypes.CDLL(str(dll_path)).sa_engine_dispatch_float_78_8c652d3a
    ByteP = ctypes.POINTER(ctypes.c_uint8)
    fn.argtypes = [ByteP, ctypes.POINTER(Fpu), P, P, P, P, P,
                   ctypes.c_void_p, Call]
    fn.restype = ctypes.c_int

    values = [0x00000000, 0x80000000, 0x3F800000, 0xBF800000,
              0x7F800000, 0xFF800000, 0x7FC12345, 0xFFC54321,
              0x00000001, 0x80800000, 0x41200000, 0xC1200000]
    cases = []
    total_steps = 0
    for bank in [0, 0x200000]:
        for i, value in enumerate(values):
            ram = bytearray(16 * 1024 * 1024)
            ram[0x10000:0x10000 + len(binary)] = binary
            state = State()
            state.r[:] = [((0x12345678 * (i + 1)) + n * 0x1001) & 0xFFFFFFFF
                          for n in range(16)]
            state.r[15] = 0x8CF00000
            state.sr = 0x70000020 | (i & 1)
            state.pr = 0x8C000100
            state.pc = 0x8C652D3A
            fpu = Fpu()
            fpu.fpscr = bank
            for b in range(2):
                for n in range(16):
                    fpu.banks[b][n] = ((0x10203040 * (b + 1)) + n * 0x01010101) & 0xFFFFFFFF
            fpu.banks[bank >> 21][4] = value
            fpul_value, macl_value = 0xA1B2C3D4, 0xE5F60718

            retail_ram = bytearray(ram)
            retail_state = State.from_buffer_copy(bytes(state))
            retail_fpu = Fpu.from_buffer_copy(bytes(fpu))
            expected_events = []
            def dispatcher_hook(_memory, regs, sr):
                expected_events.append((0x8C653444, regs[4], regs[5], regs[6], regs[7]))
                return sr
            steps = reference(retail_ram, retail_state,
                              hooks={0x8C653444: dispatcher_hook}, fpu=retail_fpu,
                              stop_pc=state.pr)
            total_steps += steps

            native_ram = bytearray(ram)
            view = (ctypes.c_uint8 * len(native_ram)).from_buffer(native_ram)
            regs = (ctypes.c_uint32 * 16)(*state.r)
            sr = ctypes.c_uint32(state.sr)
            pr = ctypes.c_uint32(state.pr)
            fpul = ctypes.c_uint32(fpul_value)
            macl = ctypes.c_uint32(macl_value)
            actual_events = []
            @Call
            def dispatch(_ctx, target, _m, _f, _sr, live_regs, _pr):
                actual_events.append((target, live_regs[4], live_regs[5],
                                      live_regs[6], live_regs[7]))
                return 0
            assert fn(view, ctypes.byref(fpu), ctypes.byref(sr), regs,
                      ctypes.byref(pr), ctypes.byref(fpul), ctypes.byref(macl),
                      None, dispatch) == 0

            assert native_ram == retail_ram
            assert list(regs) == list(retail_state.r)
            assert sr.value == retail_state.sr and pr.value == retail_state.pr
            assert bytes(fpu) == bytes(retail_fpu)
            assert fpul.value == fpul_value and macl.value == macl_value
            assert actual_events == expected_events == [
                (0x8C653444, 0x78, value & 0x7FFFFFFF, state.r[6], state.r[7])]
            cases.append({"fpscr_bank": bank, "fr4_bits": hex(value),
                          "dispatcher_arguments_match": True,
                          "full_ram_cpu_fpu_match": True,
                          "original_instructions_executed": steps})

    start, end = 0x8C652D3A, 0x8C652D56
    file_start, file_end = start - 0x8C010000, end - 0x8C010000
    report = {
        "module": "1ST_READ.BIN", "routine": hex(start),
        "end_exclusive": hex(end), "cases_passed": len(cases),
        "original_instructions_executed": total_steps, "cases": cases,
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[file_start:file_end]).hexdigest(),
        "source": "source/engine_dispatch_float_78.c",
        "instruction_listing": "analysis/instructions/8c652d3a.tsv",
        "instruction_listing_sha256": hashlib.sha256(listing.read_bytes()).hexdigest(),
        "source_sha256": hashlib.sha256(source.read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Compares full 16 MiB RAM, general registers, SR/PR, both FPU banks, FPUL/MACL preservation, and dispatch arguments in 24 cases. FR4 samples include signed zero, normal values, infinities, NaNs, and subnormal/edge bit patterns across both FR banks.",
        "limitations": ["Dispatcher implementation remains outside this wrapper test."],
    }
    (project / "analysis/engine_dispatch_float_78_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} float wrapper comparisons passed; {total_steps} retail instructions executed")


if __name__ == "__main__":
    main()
