"""Compare event pending-clear behavior and MMIO write against retail SH-4."""
import ctypes
import hashlib
import itertools
import json
import random

from verify_fpu_cleanup_transfer import Fpu
from verify_startup import ROOT, State, reference

P = ctypes.POINTER(ctypes.c_uint32)
Write32 = ctypes.CFUNCTYPE(ctypes.c_int, ctypes.c_void_p,
                           ctypes.c_uint32, ctypes.c_uint32)


def main():
    project = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    source = project / "source/sa_engine_event_clear_pending.c"
    dll_path = ROOT / "work/sonic-adventure/build/engine_event_clear_pending.dll"
    fn = ctypes.CDLL(str(dll_path)).sa_engine_event_clear_pending
    ByteP = ctypes.POINTER(ctypes.c_uint8)
    fn.argtypes = [ByteP, ctypes.POINTER(Fpu), P, P, P, P, P,
                   ctypes.c_void_p, Write32]
    fn.restype = ctypes.c_int

    cases, total_steps = [], 0
    for flag, pending, seed, mmio_value in itertools.product(
            (0, 1, 2, 0x10, 0xffffffff),
            (0, 1, 0x10, 0x80000000, 0xffffffff), range(2),
            (0, 0x12345678)):
        rng = random.Random(0x6521f2 ^ flag ^ pending ^ seed ^ mmio_value)
        ram = bytearray(16 * 1024 * 1024)
        ram[0x10000:0x10000 + len(binary)] = binary
        ram[0x8a3290:0x8a3294] = flag.to_bytes(4, "little")
        ram[0x8a2ffc:0x8a3000] = pending.to_bytes(4, "little")
        state = State()
        state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        state.r[15] = 0x8cf00000
        state.sr = 0x70000020 | seed
        state.pr, state.pc = 0x8c000100, 0x8c6521f2
        entry = State.from_buffer_copy(bytes(state))
        expected_state = State.from_buffer_copy(bytes(state))
        retail_ram = bytearray(ram)
        fpu = Fpu()
        fpu.fpscr = seed << 21
        for bank in range(2):
            for index in range(16):
                fpu.banks[bank][index] = rng.getrandbits(32)
        expected_fpu = Fpu.from_buffer_copy(bytes(fpu))
        expected_mmio = {0xa05f6908: mmio_value}
        steps = reference(retail_ram, expected_state, stop_pc=entry.pr,
                          fpu=expected_fpu, mmio=expected_mmio)
        total_steps += steps

        native_ram = bytearray(ram)
        view = (ctypes.c_uint8 * len(native_ram)).from_buffer(native_ram)
        regs = (ctypes.c_uint32 * 16)(*entry.r)
        sr, pr = ctypes.c_uint32(entry.sr), ctypes.c_uint32(entry.pr)
        native_fpu = Fpu.from_buffer_copy(bytes(fpu))
        fpul, macl = ctypes.c_uint32(0x12345678), ctypes.c_uint32(0x9abcdef0)
        native_mmio = {0xa05f6908: mmio_value}
        writes = []

        @Write32
        def write_register(_context, address, value):
            writes.append((int(address), int(value)))
            native_mmio[int(address)] = int(value)
            return 0

        assert fn(view, ctypes.byref(native_fpu), ctypes.byref(sr), regs,
                  ctypes.byref(pr), ctypes.byref(fpul), ctypes.byref(macl),
                  None, write_register) == 0
        assert native_ram == retail_ram, [
            (hex(i), native_ram[i:i + 4].hex(), retail_ram[i:i + 4].hex())
            for i in range(len(native_ram)) if native_ram[i] != retail_ram[i]][:10]
        assert list(regs) == list(expected_state.r), (
            flag, pending, seed,
            [(i, hex(regs[i]), hex(expected_state.r[i]))
             for i in range(16) if regs[i] != expected_state.r[i]])
        assert sr.value == expected_state.sr and pr.value == expected_state.pr
        assert bytes(native_fpu) == bytes(expected_fpu)
        assert native_mmio == expected_mmio
        assert writes == [(0xa05f6908, 2)]
        assert fpul.value == 0x12345678 and macl.value == 0x9abcdef0
        cases.append({"global_flag": hex(flag), "initial_pending": hex(pending),
                      "seed": seed, "mmio_before": hex(mmio_value),
                      "mmio_after": hex(native_mmio[0xa05f6908]),
                      "full_ram_cpu_fpu_sr_pr_mmio_match": True,
                      "retail_instructions_executed": steps})

    start, end = 0x8c6521f2, 0x8c652210
    begin, finish = start - 0x8c010000, end - 0x8c010000
    report = {
        "module": "1ST_READ.BIN", "routine": hex(start),
        "end_exclusive": hex(end), "cases_passed": len(cases),
        "original_instructions_executed": total_steps, "cases": cases,
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[begin:finish]).hexdigest(),
        "source": "source/sa_engine_event_clear_pending.c",
        "source_sha256": hashlib.sha256(source.read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Compares full RAM, CPU registers, SR/PR, FPU, FPUL/MACL preservation and the MMIO event write against retail execution for five global flag values, five pending-bit patterns, two SR/FPU banks, and two initial device values.",
        "limitations": ["The Dreamcast register write is represented by a host MMIO callback.",
                        "This leaf handler comparison does not establish event-thread scheduling or game integration."],
    }
    (project / "analysis/engine_event_clear_pending_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} full-state retail comparisons passed; {total_steps} instructions executed")


if __name__ == "__main__":
    main()
