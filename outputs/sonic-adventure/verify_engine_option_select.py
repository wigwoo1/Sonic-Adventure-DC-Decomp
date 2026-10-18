"""Compare option selection plus native string helpers with retail execution."""
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
    source = project / "source/sa_engine_option_select.c"
    listing = project / "analysis/instructions/8c665a80.tsv"
    dll_path = ROOT / "work/sonic-adventure/build/engine_context_initialize_native.dll"
    lib = ctypes.CDLL(str(dll_path))
    ByteP = ctypes.POINTER(ctypes.c_uint8)
    abi = [ByteP, ctypes.POINTER(Fpu), P, P, P, P, P, ctypes.c_void_p, Call]
    option = lib.sa_engine_option_select
    option.argtypes, option.restype = abi, ctypes.c_int
    wrapper = lib.sa_string_compare_wrapper
    wrapper.argtypes, wrapper.restype = abi, ctypes.c_int
    comparator = lib.sa_string_compare
    comparator.argtypes, comparator.restype = abi, ctypes.c_int

    values = [b"HARD", b"Enable", b"Other", b"", b"EnableX"]
    alignments = [(0, 0), (1, 3), (2, 1), (3, 2)]
    cases, total_steps = [], 0
    for left, right, (left_align, right_align), bank in itertools.product(
            values, values, alignments, [0, 0x200000]):
        seed = (values.index(left) * 1000 + values.index(right) * 100
                + left_align * 10 + right_align + (bank >> 21))
        rng = random.Random(0x665A80 + seed)
        ram = bytearray(16 * 1024 * 1024)
        ram[0x10000:0x10000 + len(binary)] = binary
        left_addr, right_addr = 0x8CA00000 + left_align, 0x8CA00100 + right_align
        ram[0xA00000 + left_align:0xA00000 + left_align + len(left) + 1] = left + b"\0"
        ram[0xA00100 + right_align:0xA00100 + right_align + len(right) + 1] = right + b"\0"
        state = State()
        state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        state.r[4], state.r[5], state.r[6] = left_addr, right_addr, 0xF00D1234 + seed
        state.r[15], state.sr, state.pr, state.pc = 0x8CF00000, 0x70000020 | (seed & 1), 0x8C000100, 0x8C665A80
        fpu = Fpu()
        fpu.fpscr = bank
        for b in range(2):
            for i in range(16):
                fpu.banks[b][i] = rng.getrandbits(32)
        fpul_value, macl_value = rng.getrandbits(32), rng.getrandbits(32)

        retail_ram = bytearray(ram)
        retail_state = State.from_buffer_copy(bytes(state))
        retail_fpu = Fpu.from_buffer_copy(bytes(fpu))
        reference_calls, nested_steps = [], []
        def string_wrapper_hook(memory, regs, sr_value):
            entry_regs = list(regs)
            nested = State()
            nested.r[:] = entry_regs
            nested.sr, nested.pr, nested.pc = sr_value, retail_state.pr, 0x8C10D3C8
            count = reference(memory, nested, fpu=retail_fpu, stop_pc=nested.pr)
            regs[:] = list(nested.r)
            retail_state.pr = nested.pr
            nested_steps.append(count)
            reference_calls.append((entry_regs[4], entry_regs[5], nested.r[0]))
            return nested.sr
        steps = reference(retail_ram, retail_state,
                          hooks={0x8C10D3C8: string_wrapper_hook},
                          fpu=retail_fpu, stop_pc=state.pr)
        total_steps += steps + sum(nested_steps)

        native_ram = bytearray(ram)
        view = (ctypes.c_uint8 * len(native_ram)).from_buffer(native_ram)
        regs = (ctypes.c_uint32 * 16)(*state.r)
        sr, pr = ctypes.c_uint32(state.sr), ctypes.c_uint32(state.pr)
        fpul, macl = ctypes.c_uint32(fpul_value), ctypes.c_uint32(macl_value)
        native_calls = []
        @Call
        def compare_service(_ctx, target, m, f, live_sr, live_regs, live_pr):
            if target != 0x8C10D3C8:
                return -9
            native_calls.append((live_regs[4], live_regs[5]))
            @Call
            def comparator_service(_ctx2, target2, m2, f2, sr2, r2, pr2):
                if target2 != 0x8C10DB1C:
                    return -9
                @Call
                def no_call(_c3, _t3, _m3, _f3, _s3, _r3, _p3):
                    return -9
                return comparator(m2, f2, sr2, r2, pr2,
                                  ctypes.byref(fpul), ctypes.byref(macl), None, no_call)
            result = wrapper(m, f, live_sr, live_regs, live_pr,
                             ctypes.byref(fpul), ctypes.byref(macl), None,
                             comparator_service)
            native_calls[-1] += (live_regs[0],)
            return result
        result = option(view, ctypes.byref(fpu), ctypes.byref(sr), regs,
                        ctypes.byref(pr), ctypes.byref(fpul), ctypes.byref(macl),
                        None, compare_service)

        assert result == 0
        assert native_ram == retail_ram
        assert list(regs) == list(retail_state.r)
        assert sr.value == retail_state.sr and pr.value == retail_state.pr
        assert bytes(fpu) == bytes(retail_fpu)
        assert fpul.value == fpul_value and macl.value == macl_value
        assert native_calls == reference_calls
        cases.append({"left": left.decode("ascii"), "right": right.decode("ascii"),
                      "left_alignment": left_align, "right_alignment": right_align,
                      "fpscr_bank": bank, "full_ram_cpu_fpu_match": True,
                      "string_call_arguments_and_results_match": True,
                      "selected_result": hex(regs[4])})

    start, end = 0x8C665A80, 0x8C665AAC
    file_start, file_end = start - 0x8C010000, end - 0x8C010000
    report = {
        "module": "1ST_READ.BIN", "routine": hex(start),
        "end_exclusive": hex(end), "cases_passed": len(cases),
        "original_instructions_executed": total_steps, "cases": cases,
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[file_start:file_end]).hexdigest(),
        "source": "source/sa_engine_option_select.c",
        "instruction_listing": "analysis/instructions/8c665a80.tsv",
        "instruction_listing_sha256": hashlib.sha256(listing.read_bytes()).hexdigest(),
        "source_sha256": hashlib.sha256(source.read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "The recovered selector and native string wrapper/comparator are compared against retail execution for 200 pairs across five string values, four alignment combinations, and both FPU banks. Compares complete RAM, CPU/FPU, SR/PR, string-call arguments/results, and return selection.",
        "limitations": ["Only ASCII strings are sampled; high-byte string behavior is not independently checked.",
                        "Engine boot and downstream option-service behavior remain outside scope."],
    }
    (project / "analysis/engine_option_select_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} full-state option cases passed; {total_steps} retail instructions executed")


if __name__ == "__main__":
    main()
