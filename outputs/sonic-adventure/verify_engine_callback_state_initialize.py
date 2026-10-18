"""Compare callback-state initialization with retail SH-4 execution."""
import ctypes
import hashlib
import itertools
import json
import pathlib
import random

from verify_fpu_cleanup_transfer import Fpu
from verify_startup import State, reference
from verify_tikal_controller import Call, P

ROOT = pathlib.Path(__file__).resolve().parent


def main():
    project = ROOT
    workspace = ROOT.parent.parent
    binary = (workspace / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    source = project / "source/sa_engine_callback_state_initialize.c"
    listing = project / "analysis/instructions/8c651520.tsv"
    dll_path = workspace / "work/sonic-adventure/build/engine_callback_state_initialize.dll"
    native = ctypes.CDLL(str(dll_path)).sa_engine_callback_state_initialize
    ByteP = ctypes.POINTER(ctypes.c_uint8)
    native.argtypes = [ByteP, ctypes.POINTER(Fpu), P, P, P, P, P,
                       ctypes.c_void_p, Call]
    native.restype = ctypes.c_int

    cases = []
    total_steps = 0
    for fill, bank, mask, seed in itertools.product(
            [0x00, 0x55, 0xA5], [0, 0x200000], [0, 15], [0, 1]):
        rng = random.Random(0x651520 + seed)
        ram = bytearray(16 * 1024 * 1024)
        ram[0x10000:0x10000 + len(binary)] = binary
        ram[0x8A2F00:0x8A3300] = bytes([fill]) * 0x400
        state = State()
        state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        state.r[15] = 0x8CF00000
        state.sr = 0x70000020 | (mask << 4)
        state.pr = 0x8C000100
        state.pc = 0x8C651520
        fpu = Fpu()
        fpu.fpscr = bank
        for b in range(2):
            for i in range(16):
                fpu.banks[b][i] = rng.getrandbits(32)
        initial_fpul, initial_macl = rng.getrandbits(32), rng.getrandbits(32)

        retail_ram = bytearray(ram)
        retail_state = State.from_buffer_copy(bytes(state))
        retail_fpu = Fpu.from_buffer_copy(bytes(fpu))
        expected_events = []
        def register_hook(_memory, regs, sr):
            expected_events.append((0x8C653640, tuple(regs[i] for i in range(4, 8))))
            return sr
        steps = reference(retail_ram, retail_state,
                          hooks={0x8C653640: register_hook}, fpu=retail_fpu,
                          stop_pc=state.pr)
        total_steps += steps

        native_ram = bytearray(ram)
        view = (ctypes.c_uint8 * len(native_ram)).from_buffer(native_ram)
        regs = (ctypes.c_uint32 * 16)(*state.r)
        sr = ctypes.c_uint32(state.sr)
        pr = ctypes.c_uint32(state.pr)
        fpul = ctypes.c_uint32(initial_fpul)
        macl = ctypes.c_uint32(initial_macl)
        events = []
        @Call
        def register(_ctx, target, _m, _f, _sr, live_regs, _pr):
            events.append((target, tuple(live_regs[i] for i in range(4, 8))))
            return 0
        result = native(view, ctypes.byref(fpu), ctypes.byref(sr), regs,
                        ctypes.byref(pr), ctypes.byref(fpul), ctypes.byref(macl),
                        None, register)

        assert result == 0
        assert native_ram == retail_ram
        assert list(regs) == list(retail_state.r)
        assert sr.value == retail_state.sr and pr.value == retail_state.pr
        assert bytes(fpu) == bytes(retail_fpu)
        assert fpul.value == initial_fpul and macl.value == initial_macl
        assert events == expected_events == [(0x8C653640,
                                               (0x8C652210, *tuple(state.r[i] for i in (5, 6, 7))))]
        for address in [0x8A2F40, 0x8A2FFC, 0x8A30C4, 0x8A30C8,
                        *range(0x8A3238, 0x8A3260, 4),
                        *range(0x8A3268, 0x8A327C, 4), 0x8A328C]:
            assert int.from_bytes(native_ram[address:address + 4], "little") == 0
        cases.append({"fill": fill, "fpscr_bank": bank, "sr_mask": mask,
                      "register_seed": seed, "full_ram_cpu_fpu_match": True,
                      "callback_arguments_match": True, "original_instructions_executed": steps})

    start, end = 0x8C651520, 0x8C651570
    file_start, file_end = start - 0x8C010000, end - 0x8C010000
    report = {
        "module": "1ST_READ.BIN",
        "routine": hex(start),
        "end_exclusive": hex(end),
        "cases_passed": len(cases),
        "original_instructions_executed": total_steps,
        "cases": cases,
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[file_start:file_end]).hexdigest(),
        "source": "source/sa_engine_callback_state_initialize.c",
        "instruction_listing": "analysis/instructions/8c651520.tsv",
        "instruction_listing_sha256": hashlib.sha256(listing.read_bytes()).hexdigest(),
        "source_sha256": hashlib.sha256(source.read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Full 16 MiB RAM, general registers, SR/PR, FPU banks, callback target/arguments, and FPUL/MACL preservation compared against retail execution in 24 cases across three memory fills, both FPU banks, SR masks, and two register seeds.",
        "limitations": ["Callback registration body remains an external service.",
                        "Engine boot order and physical video behavior are not established."],
    }
    (project / "analysis/engine_callback_state_initialize_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} complete-state comparisons passed; {total_steps} retail instructions executed")


if __name__ == "__main__":
    main()
