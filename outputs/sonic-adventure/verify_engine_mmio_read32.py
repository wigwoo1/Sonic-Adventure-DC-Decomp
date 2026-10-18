"""Compare the recovered Dreamcast MMIO word reader against retail SH-4 code."""
import ctypes
import hashlib
import itertools
import json
import random

from verify_fpu_cleanup_transfer import Fpu
from verify_startup import ROOT, State, reference

Read32 = ctypes.CFUNCTYPE(ctypes.c_uint32, ctypes.c_void_p, ctypes.c_uint32)
P = ctypes.POINTER(ctypes.c_uint32)


def main():
    project = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    source = project / "source/sa_engine_mmio_read32.c"
    listing = project / "analysis/instructions/video_setup_region.tsv"
    dll_path = ROOT / "work/sonic-adventure/build/engine_mmio_read32.dll"
    fn = ctypes.CDLL(str(dll_path)).sa_engine_mmio_read32
    ByteP = ctypes.POINTER(ctypes.c_uint8)
    fn.argtypes = [ByteP, ctypes.POINTER(Fpu), P, P, P, P, P,
                   ctypes.c_void_p, Read32]
    fn.restype = ctypes.c_int

    offsets = [0, 1, 4, 8, 0x40, 0x78, 0xb0, 0xffc,
               0x1234, 0x10000000, 0x50000000, 0x5f000000]
    cases = []
    for offset, bank, seed in itertools.product(offsets, [0, 0x200000], range(2)):
        rng = random.Random(0x653476 ^ offset ^ bank ^ seed)
        ram = bytearray(16 * 1024 * 1024)
        ram[0x10000:0x10000 + len(binary)] = binary
        state = State()
        state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        state.r[4] = offset
        state.r[15] = 0x8cf00000
        state.sr = 0x70000020 | seed
        state.pr, state.pc = 0x8c000100, 0x8c653476
        fpu = Fpu()
        fpu.fpscr = bank
        for b in range(2):
            for i in range(16):
                fpu.banks[b][i] = rng.getrandbits(32)
        fpul_value, macl_value = rng.getrandbits(32), rng.getrandbits(32)

        address = (0xa05f8000 + offset) & 0xffffffff
        device_value = rng.getrandbits(32)
        retail_ram = bytearray(ram)
        retail_state = State.from_buffer_copy(bytes(state))
        retail_fpu = Fpu.from_buffer_copy(bytes(fpu))
        retail_mmio = {address: device_value}
        steps = reference(retail_ram, retail_state, fpu=retail_fpu,
                          mmio=retail_mmio, stop_pc=state.pr)

        native_ram = bytearray(ram)
        view = (ctypes.c_uint8 * len(native_ram)).from_buffer(native_ram)
        regs = (ctypes.c_uint32 * 16)(*state.r)
        sr, pr = ctypes.c_uint32(state.sr), ctypes.c_uint32(state.pr)
        fpul, macl = ctypes.c_uint32(fpul_value), ctypes.c_uint32(macl_value)
        events = []

        @Read32
        def read_register(_context, actual_address):
            events.append(actual_address)
            return device_value if actual_address == address else 0

        assert fn(view, ctypes.byref(fpu), ctypes.byref(sr), regs,
                  ctypes.byref(pr), ctypes.byref(fpul), ctypes.byref(macl),
                  None, read_register) == 0

        assert native_ram == retail_ram
        assert list(regs) == list(retail_state.r)
        assert sr.value == retail_state.sr and pr.value == retail_state.pr
        assert bytes(fpu) == bytes(retail_fpu)
        assert fpul.value == fpul_value and macl.value == macl_value
        assert regs[0] == retail_state.r[0] == device_value
        assert events == [address]
        cases.append({"register_offset": hex(offset), "absolute_address": hex(address),
                      "device_value": hex(device_value), "fpscr_bank": hex(bank),
                      "seed": seed, "full_ram_cpu_fpu_match": True,
                      "retail_instructions_executed": steps})

    start, end = 0x8c653476, 0x8c65347c
    file_start, file_end = start - 0x8c010000, end - 0x8c010000
    report = {
        "module": "1ST_READ.BIN", "routine": hex(start),
        "end_exclusive": hex(end), "cases_passed": len(cases), "cases": cases,
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[file_start:file_end]).hexdigest(),
        "source": "source/sa_engine_mmio_read32.c",
        "instruction_listing": "analysis/instructions/video_setup_region.tsv",
        "instruction_listing_sha256": hashlib.sha256(listing.read_bytes()).hexdigest(),
        "source_sha256": hashlib.sha256(source.read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Compares complete RAM/CPU/FPU state, MMIO address and returned register value for twelve aligned, unaligned, and high offsets across two FPU banks and two register seeds. Effective addresses remain within 32 bits.",
        "limitations": ["Device behavior is represented by a host MMIO callback.",
                        "Address-addition wraparound is not verified because the shared test interpreter does not wrap indexed effective addresses before device lookup.",
                        "Physical Dreamcast hardware and full-game integration remain unverified."],
    }
    (project / "analysis/engine_mmio_read32_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} full-state comparisons passed; {sum(c['retail_instructions_executed'] for c in cases)} retail instructions executed")


if __name__ == "__main__":
    main()
