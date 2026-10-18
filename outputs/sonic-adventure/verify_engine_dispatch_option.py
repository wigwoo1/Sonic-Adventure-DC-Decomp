"""Compare the recovered option-dispatch wrapper with retail SH-4 execution."""
import ctypes
import hashlib
import itertools
import json
import random

from verify_fpu_cleanup_transfer import Fpu
from verify_startup import ROOT, State, reference
from verify_tikal_controller import Call, P


def main():
    project = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    source = project / "source/sa_engine_dispatch_option.c"
    listing = project / "analysis/instructions/8c652e80.tsv"
    dll_path = ROOT / "work/sonic-adventure/build/engine_dispatch_option.dll"
    fn = ctypes.CDLL(str(dll_path)).sa_engine_dispatch_option
    ByteP = ctypes.POINTER(ctypes.c_uint8)
    fn.argtypes = [ByteP, ctypes.POINTER(Fpu), P, P, P, P, P,
                   ctypes.c_void_p, Call]
    fn.restype = ctypes.c_int

    cases, total_steps = [], 0
    values = [0, 1, 2, 3, 4, 0xfffffffc, 0xffffffff,
              0x80000000, 0x12345678, 0xfffffffb]
    for value, bank, fill, seed in itertools.product(
            values, [0, 0x200000], [0x00, 0x5a], range(2)):
        rng = random.Random(0x652e80 ^ value ^ bank ^ (fill << 8) ^ seed)
        ram = bytearray(16 * 1024 * 1024)
        ram[0x10000:0x10000 + len(binary)] = binary
        ram[0x8a2df0:0x8a2df4] = bytes([fill]) * 4
        state = State()
        state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        state.r[4] = value
        state.r[14] = rng.getrandbits(32)
        state.r[15] = 0x8cf00000
        state.sr = 0x70000020 | seed
        state.pr, state.pc = 0x8c000100, 0x8c652e80
        fpu = Fpu()
        fpu.fpscr = bank
        for b in range(2):
            for i in range(16):
                fpu.banks[b][i] = rng.getrandbits(32)
        fpul_value, macl_value = rng.getrandbits(32), rng.getrandbits(32)

        retail_ram = bytearray(ram)
        retail_state = State.from_buffer_copy(bytes(state))
        retail_fpu = Fpu.from_buffer_copy(bytes(fpu))
        expected_events = []

        def dispatch_hook(memory, regs, live_sr):
            expected_events.append((tuple(regs), live_sr, retail_state.pr))
            return live_sr

        steps = reference(retail_ram, retail_state,
                          hooks={0x8c653444: dispatch_hook},
                          fpu=retail_fpu, stop_pc=state.pr)
        total_steps += steps

        native_ram = bytearray(ram)
        view = (ctypes.c_uint8 * len(native_ram)).from_buffer(native_ram)
        regs = (ctypes.c_uint32 * 16)(*state.r)
        sr, pr = ctypes.c_uint32(state.sr), ctypes.c_uint32(state.pr)
        fpul, macl = ctypes.c_uint32(fpul_value), ctypes.c_uint32(macl_value)
        actual_events = []

        @Call
        def dispatch_service(_context, target, _memory, _fpu, live_sr,
                             live_regs, live_pr):
            actual_events.append((target, tuple(live_regs[i] for i in range(16)), live_sr[0],
                                  live_pr[0]))
            return 0

        assert fn(view, ctypes.byref(fpu), ctypes.byref(sr), regs,
                  ctypes.byref(pr), ctypes.byref(fpul), ctypes.byref(macl),
                  None, dispatch_service) == 0

        assert native_ram == retail_ram
        assert list(regs) == list(retail_state.r)
        assert sr.value == retail_state.sr and pr.value == retail_state.pr
        assert bytes(fpu) == bytes(retail_fpu)
        assert fpul.value == fpul_value and macl.value == macl_value
        assert len(actual_events) == len(expected_events)
        if expected_events:
            event = expected_events[0]
            target, native_regs, native_sr, native_pr = actual_events[0]
            assert target == 0x8c653444
            assert (native_regs, native_sr, native_pr) == event
        cases.append({"input": hex(value), "fpscr_bank": hex(bank),
                      "memory_fill": hex(fill), "seed": seed,
                      "dispatcher_called": bool(expected_events),
                      "full_ram_cpu_fpu_match": True,
                      "service_arguments_and_return_state_match": True,
                      "retail_instructions_executed": steps})

    start, end = 0x8c652e80, 0x8c652ea0
    file_start, file_end = start - 0x8c010000, end - 0x8c010000
    report = {
        "module": "1ST_READ.BIN", "routine": hex(start),
        "end_exclusive": hex(end), "cases_passed": len(cases),
        "original_instructions_executed": total_steps, "cases": cases,
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[file_start:file_end]).hexdigest(),
        "source": "source/sa_engine_dispatch_option.c",
        "instruction_listing": "analysis/instructions/8c652e80.tsv",
        "instruction_listing_sha256": hashlib.sha256(listing.read_bytes()).hexdigest(),
        "source_sha256": hashlib.sha256(source.read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Compares complete 16 MiB RAM, general registers, SR/PR, both FPU banks, FPUL/MACL preservation, dispatcher target and call-boundary state for values 0..3 and out-of-range values across two FPU banks and memory patterns.",
        "limitations": ["The external dispatcher at 8c653444 is modeled as a boundary hook.",
                        "This function-level equivalence does not establish engine boot or game integration."],
    }
    (project / "analysis/engine_dispatch_option_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} full-state comparisons passed; {total_steps} retail instructions executed")


if __name__ == "__main__":
    main()
