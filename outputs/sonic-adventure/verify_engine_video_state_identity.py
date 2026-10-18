"""Verify the retail video dispatcher thunk's register and call-boundary effects."""
import ctypes
import hashlib
import itertools
import json
import pathlib

from verify_fpu_cleanup_transfer import Fpu
from verify_startup import State, reference
from verify_tikal_controller import Call, P

ROOT = pathlib.Path(__file__).resolve().parent


def main():
    project = ROOT
    workspace = ROOT.parent.parent
    binary = (workspace / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    source = project / "source/sa_engine_video_state_identity.c"
    listing = project / "analysis/instructions/8c6513ee.tsv"
    dll_path = workspace / "work/sonic-adventure/build/engine_video_state_initialize.dll"
    native = ctypes.CDLL(str(dll_path)).sa_engine_video_state_identity
    ByteP = ctypes.POINTER(ctypes.c_uint8)
    native.argtypes = [ByteP, ctypes.POINTER(Fpu), P, P, P, P, P,
                       ctypes.c_void_p, Call]
    native.restype = ctypes.c_int

    cases = []
    for case, bank, t in itertools.product(range(8), [0, 0x200000], [0, 1]):
        ram = bytearray(16 * 1024 * 1024)
        ram[0x10000:0x10000 + len(binary)] = binary
        state = State()
        state.r[:] = [((0x10203040 * (case + 1)) + i * 0x101) & 0xFFFFFFFF
                      for i in range(16)]
        state.r[15] = 0x8CF00000
        state.sr = 0x70000020 | t
        state.pr = 0x8C000100
        state.pc = 0x8C6513EE
        fpu = Fpu()
        fpu.fpscr = bank | (case << 4)
        for b in range(2):
            for i in range(16):
                fpu.banks[b][i] = (case * 0x10000 + b * 0x1000 + i) & 0xFFFFFFFF

        events = []
        @Call
        def dispatch(_ctx, target, _m, _f, _sr, regs, _pr):
            events.append((target, tuple(regs[i] for i in range(4, 8))))
            return 0

        retail_ram = bytearray(ram)
        retail_state = State.from_buffer_copy(bytes(state))
        retail_fpu = Fpu.from_buffer_copy(bytes(fpu))
        expected_events = []
        def dispatcher_hook(_memory, regs, sr):
            expected_events.append((0x8C653444, tuple(regs[i] for i in range(4, 8))))
            return sr
        reference(retail_ram, retail_state, hooks={0x8C653444: dispatcher_hook},
                  fpu=retail_fpu, stop_pc=state.pr)

        native_ram = bytearray(ram)
        view = (ctypes.c_uint8 * len(native_ram)).from_buffer(native_ram)
        regs = (ctypes.c_uint32 * 16)(*state.r)
        sr = ctypes.c_uint32(state.sr)
        pr = ctypes.c_uint32(state.pr)
        fpul = ctypes.c_uint32(0x11223344)
        macl = ctypes.c_uint32(0x55667788)
        native(view, ctypes.byref(fpu), ctypes.byref(sr), regs,
               ctypes.byref(pr), ctypes.byref(fpul), ctypes.byref(macl),
               None, dispatch)

        assert native_ram == retail_ram
        assert list(regs) == list(retail_state.r)
        assert sr.value == retail_state.sr and pr.value == retail_state.pr
        assert bytes(fpu) == bytes(retail_fpu)
        assert events == expected_events == [(0x8C653444,
                                               tuple(state.r[i] for i in range(4, 8)))]
        cases.append({"case": case, "fpscr_bank": bank, "initial_t": t,
                      "dispatcher_target_and_arguments_match": True,
                      "full_ram_cpu_fpu_match": True})

    start, end = 0x8C6513EE, 0x8C6513F4
    file_start, file_end = start - 0x8C010000, end - 0x8C010000
    report = {
        "module": "1ST_READ.BIN",
        "routine": hex(start),
        "end_exclusive": hex(end),
        "cases_passed": len(cases),
        "cases": cases,
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[file_start:file_end]).hexdigest(),
        "source": "source/sa_engine_video_state_identity.c",
        "instruction_listing": "analysis/instructions/8c6513ee.tsv",
        "instruction_listing_sha256": hashlib.sha256(listing.read_bytes()).hexdigest(),
        "source_sha256": hashlib.sha256(source.read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Compares complete 16 MiB RAM, all general registers, SR, PR, FPU banks, and dispatcher target/arguments across 32 states. The dispatcher body remains a call-boundary service; this verifies forwarding only.",
        "limitations": ["Dispatcher implementation, physical video I/O, rendering, and game boot are outside this thunk test."],
    }
    (project / "analysis/engine_video_state_identity_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} dispatcher forwarding comparisons passed")


if __name__ == "__main__":
    main()
