"""Compare two dispatcher wrappers against retail SH-4 instruction execution."""
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
    dll_path = ROOT / "work/sonic-adventure/build/engine_dispatch_wrappers.dll"
    source = project / "source/engine_dispatch_wrappers.c"
    listings = [project / "analysis/instructions/8c652f56.tsv",
                project / "analysis/instructions/8c652d6e.tsv"]
    lib = ctypes.CDLL(str(dll_path))
    ByteP = ctypes.POINTER(ctypes.c_uint8)
    signature = [ByteP, ctypes.POINTER(Fpu), P, P, P, P, P,
                 ctypes.c_void_p, Call]
    functions = [
        (0x8C652F56, 0x8C652F66, 0x40, "sa_engine_dispatch_40_8c652f56", 0x8C653004, 0x8C652F60),
        (0x8C652D6E, 0x8C652D7E, 0xB0, "sa_engine_dispatch_b0_8c652d6e", 0x8C652D9C, 0x8C652D78),
        (0x8C652E38, 0x8C652E50, 0xB8, "sa_engine_dispatch_b8_8c652e38", 0x8C652E6C, 0x8C652E48),
        (0x8C666968, 0x8C66697C, 0x08, "sa_engine_dispatch_pair_8c666968", 0x8C6669B8, 0x8C666972),
    ]
    cases = []
    total_steps = 0
    for start, end, selector, symbol, _literal, _return_address in functions:
        fn = getattr(lib, symbol)
        fn.argtypes = signature
        fn.restype = ctypes.c_int
        for case, bank, t in itertools.product(range(8), [0, 0x200000], [0, 1]):
            rng = random.Random(start + case * 17 + bank + t)
            ram = bytearray(16 * 1024 * 1024)
            ram[0x10000:0x10000 + len(binary)] = binary
            stack_arg = rng.getrandbits(32)
            ram[0xF00000:0xF00004] = stack_arg.to_bytes(4, "little")
            state = State()
            state.r[:] = [rng.getrandbits(32) for _ in range(16)]
            initial_r4 = state.r[4]
            state.r[15] = 0x8CF00000
            state.sr = 0x70000020 | t
            state.pr = 0x8C000100
            state.pc = start
            fpu = Fpu()
            fpu.fpscr = bank | (case << 4)
            for b in range(2):
                for i in range(16):
                    fpu.banks[b][i] = rng.getrandbits(32)
            fpul_value, macl_value = rng.getrandbits(32), rng.getrandbits(32)

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
            if start == 0x8C666968:
                assert actual_events == expected_events == [
                    (0x8C653444, 8, 1, state.r[6], state.r[7]),
                    (0x8C653444, 8, 0, state.r[6], state.r[7])]
            else:
                forwarded_r5 = initial_r4 if start == 0x8C652E38 else stack_arg
                assert actual_events == expected_events == [
                    (0x8C653444, selector, forwarded_r5, state.r[6], state.r[7])]
            if start == 0x8C652E38:
                assert int.from_bytes(native_ram[0xEFFFF8:0xEFFFFC], "little") == initial_r4
            if start != 0x8C666968:
                assert regs[0] == 0
            assert regs[15] == state.r[15]
            cases.append({"entry": hex(start), "selector": selector,
                          "case": case, "fpscr_bank": bank, "initial_t": t,
                          "full_state_and_dispatch_arguments_match": True,
                          "original_instructions_executed": steps})

    for start, end, selector, _symbol, _literal, _return_address in functions:
        file_start, file_end = start - 0x8C010000, end - 0x8C010000
        listing = project / f"analysis/instructions/{start:08x}.tsv"
        report = {
            "module": "1ST_READ.BIN", "routine": hex(start),
            "end_exclusive": hex(end), "selector": selector,
            "cases_passed": 24,
            "original_instructions_executed": sum(c["original_instructions_executed"] for c in cases if c["entry"] == hex(start)),
            "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
            "retail_span_sha256": hashlib.sha256(binary[file_start:file_end]).hexdigest(),
            "source": "source/engine_dispatch_wrappers.c",
            "instruction_listing": f"analysis/instructions/{start:08x}.tsv",
            "instruction_listing_sha256": hashlib.sha256(listing.read_bytes()).hexdigest(),
            "source_sha256": hashlib.sha256(source.read_bytes()).hexdigest(),
            "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
            "scope": "Full 16 MiB RAM, all registers, SR/PR, FPU banks, FPUL/MACL preservation, and dispatcher arguments compared against retail execution in 24 randomized states. Dispatcher body remains an external service.",
            "limitations": ["Dispatcher behavior beyond the call boundary is not covered by this wrapper test."],
        }
        (project / f"analysis/engine_dispatch_{start:08x}_verification.json").write_text(
            json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} wrapper cases passed; {total_steps} retail instructions executed")


if __name__ == "__main__":
    main()
