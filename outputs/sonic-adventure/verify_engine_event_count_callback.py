"""Compare the event counter handler with retail, stubbing only its child."""
import ctypes
import hashlib
import itertools
import json
import random
import struct

from verify_fpu_cleanup_transfer import Fpu
from verify_startup import ROOT, State, reference
from verify_tikal_controller import Call

P = ctypes.POINTER(ctypes.c_uint32)
Write32 = ctypes.CFUNCTYPE(ctypes.c_int, ctypes.c_void_p,
                           ctypes.c_uint32, ctypes.c_uint32)


def main():
    project = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    source = project / "source/sa_engine_event_count_callback.c"
    dll_path = ROOT / "work/sonic-adventure/build/engine_event_count_callback.dll"
    fn = ctypes.CDLL(str(dll_path)).sa_engine_event_count_callback
    ByteP = ctypes.POINTER(ctypes.c_uint8)
    fn.argtypes = [ByteP, ctypes.POINTER(Fpu), P, P, P, P, P,
                   ctypes.c_void_p, Write32, Call]
    fn.restype = ctypes.c_int

    cases, total_steps = [], 0
    for flag, counter_a, counter_b, seed, mmio_initial in itertools.product(
            (0, 1, 2, 0xffffffff), (0, 1, 0xfffffffe),
            (0, 3, 0xffffffff), range(2), (0, 0x12345678)):
        rng = random.Random(0x6521ca ^ flag ^ counter_a ^ counter_b ^ seed ^ mmio_initial)
        ram = bytearray(16 * 1024 * 1024)
        ram[0x10000:0x10000 + len(binary)] = binary
        ram[0x8a31f8:0x8a31fc] = flag.to_bytes(4, "little")
        context_base = 0x8a323c
        ram[context_base + 0x10:context_base + 0x14] = counter_b.to_bytes(4, "little")
        ram[context_base + 0x1c:context_base + 0x20] = counter_a.to_bytes(4, "little")
        # Test-only callee stub: preserve R0, then return through the retail PR.
        struct.pack_into("<HH", ram, 0x652034, 0x000b, 0x0009)
        entry = State()
        entry.r[:] = [rng.getrandbits(32) for _ in range(16)]
        entry.r[15] = 0x8cf00000
        entry.sr = 0x70000020 | seed
        entry.pr, entry.pc = 0x8c000100, 0x8c6521ca
        state = State.from_buffer_copy(bytes(entry))
        expected_state = State.from_buffer_copy(bytes(entry))
        retail_ram = bytearray(ram)
        fpu = Fpu()
        fpu.fpscr = seed << 21
        for bank in range(2):
            for index in range(16):
                fpu.banks[bank][index] = rng.getrandbits(32)
        expected_fpu = Fpu.from_buffer_copy(bytes(fpu))
        expected_mmio = {0xa05f6908: mmio_initial}
        steps = reference(retail_ram, expected_state, stop_pc=entry.pr,
                          fpu=expected_fpu, mmio=expected_mmio)
        total_steps += steps

        native_ram = bytearray(ram)
        view = (ctypes.c_uint8 * len(native_ram)).from_buffer(native_ram)
        regs = (ctypes.c_uint32 * 16)(*entry.r)
        sr, pr = ctypes.c_uint32(entry.sr), ctypes.c_uint32(entry.pr)
        native_fpu = Fpu.from_buffer_copy(bytes(fpu))
        fpul, macl = ctypes.c_uint32(0x12345678), ctypes.c_uint32(0x9abcdef0)
        native_mmio = {0xa05f6908: mmio_initial}
        writes, calls = [], []

        @Write32
        def write_register(_context, address, value):
            writes.append((int(address), int(value)))
            native_mmio[int(address)] = int(value)
            return 0

        @Call
        def event_child(_context, target, _memory, _fpu, _sr, live_regs, live_pr):
            calls.append((int(target), live_regs[4], live_pr[0], live_regs[15]))
            assert target == 0x8c652034 and live_regs[4] == 1
            assert live_pr[0] == 0x8c6521ec
            return 0

        assert fn(view, ctypes.byref(native_fpu), ctypes.byref(sr), regs,
                  ctypes.byref(pr), ctypes.byref(fpul), ctypes.byref(macl),
                  None, write_register, event_child) == 0
        assert native_ram == retail_ram, [
            (hex(i), native_ram[i:i + 4].hex(), retail_ram[i:i + 4].hex())
            for i in range(len(native_ram)) if native_ram[i] != retail_ram[i]][:10]
        assert list(regs) == list(expected_state.r), (
            flag, counter_a, counter_b,
            [(i, hex(regs[i]), hex(expected_state.r[i]))
             for i in range(16) if regs[i] != expected_state.r[i]])
        assert sr.value == expected_state.sr and pr.value == expected_state.pr
        assert bytes(native_fpu) == bytes(expected_fpu)
        assert native_mmio == expected_mmio
        assert writes == [(0xa05f6908, 8)]
        assert calls == ([(0x8c652034, 1, 0x8c6521ec, entry.r[15] - 4)]
                         if flag == 1 else [])
        assert fpul.value == 0x12345678 and macl.value == 0x9abcdef0
        cases.append({"state_flag": hex(flag), "counter_at_plus_0x10": hex(counter_b),
                      "counter_at_plus_0x1c": hex(counter_a), "seed": seed,
                      "mmio_before": hex(mmio_initial), "callback_count": len(calls),
                      "full_ram_cpu_fpu_sr_pr_mmio_match": True,
                      "retail_instructions_executed": steps})

    start, end = 0x8c6521ca, 0x8c6521f2
    begin, finish = start - 0x8c010000, end - 0x8c010000
    report = {
        "module": "1ST_READ.BIN", "routine": hex(start),
        "end_exclusive": hex(end), "cases_passed": len(cases),
        "original_instructions_executed": total_steps, "cases": cases,
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[begin:finish]).hexdigest(),
        "source": "source/sa_engine_event_count_callback.c",
        "source_sha256": hashlib.sha256(source.read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Full RAM/CPU/FPU/SR/PR/MMIO equality for four event-state values, three values for each event counter, two SR/FPU banks, two initial MMIO values, and exact callback target/argument/PR/SP when state is 1. The child function body is replaced in the retail test image by an RTS/NOP stub only.",
        "limitations": ["The event helper at 0x8c652034 is stubbed at the callee boundary; its own behavior is not tested by this verifier.",
                        "MMIO write is a host callback; event scheduling and complete game integration remain unverified."],
    }
    (project / "analysis/engine_event_count_callback_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} full-state comparisons passed; {total_steps} retail instructions executed")


if __name__ == "__main__":
    main()
