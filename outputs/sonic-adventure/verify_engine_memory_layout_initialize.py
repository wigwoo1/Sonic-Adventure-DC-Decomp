"""Compare engine memory-layout initialization with retail SH-4 execution."""
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
    source = project / "source/sa_engine_memory_layout_initialize.c"
    listing = project / "analysis/instructions/8c6666c0.tsv"
    dll_path = ROOT / "work/sonic-adventure/build/engine_memory_layout_initialize.dll"
    fn = ctypes.CDLL(str(dll_path)).sa_engine_memory_layout_initialize
    ByteP = ctypes.POINTER(ctypes.c_uint8)
    fn.argtypes = [ByteP, ctypes.POINTER(Fpu), P, P, P, P, P,
                   ctypes.c_void_p, Call]
    fn.restype = ctypes.c_int

    cases = []
    total_steps = 0
    for fill, bank, mask, seed in itertools.product(
            [0x00, 0x5A, 0xC3], [0, 0x200000], [0, 15], [0, 1]):
        rng = random.Random(0x6666C0 + seed)
        ram = bytearray(16 * 1024 * 1024)
        ram[0x10000:0x10000 + len(binary)] = binary
        ram[0x8A2F00:0x8A3300] = bytes([fill]) * 0x400
        state = State()
        state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        state.r[15] = 0x8CF00000
        state.sr = 0x70000020 | (mask << 4)
        state.pr = 0x8C000100
        state.pc = 0x8C6666C0
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
        def service_hook(target):
            def hook(_memory, regs, sr):
                expected_events.append((target, tuple(regs[i] for i in range(4, 8))))
                return sr
            return hook
        hooks = {0x8C653444: service_hook(0x8C653444),
                 0x8C650AA0: service_hook(0x8C650AA0)}
        steps = reference(retail_ram, retail_state, hooks=hooks,
                          fpu=retail_fpu, stop_pc=state.pr)
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
        def service(_ctx, target, _m, _f, _sr, live_regs, _pr):
            actual_events.append((target, tuple(live_regs[i] for i in range(4, 8))))
            return 0
        assert fn(view, ctypes.byref(fpu), ctypes.byref(sr), regs,
                  ctypes.byref(pr), ctypes.byref(fpul), ctypes.byref(macl),
                  None, service) == 0

        assert native_ram == retail_ram
        assert list(regs) == list(retail_state.r)
        assert sr.value == retail_state.sr and pr.value == retail_state.pr
        assert bytes(fpu) == bytes(retail_fpu)
        assert fpul.value == fpul_value and macl.value == macl_value
        assert actual_events == expected_events
        assert [event[0] for event in actual_events] == [0x8C653444, 0x8C653444, 0x8C650AA0]
        for address, expected in [(0x8A3194, 0x800000), (0x8A319C, 0x600000),
                                  (0x8A31A0, 0x600000), (0x8A31B8, 0x400000),
                                  (0x8A31D8, 0), (0x8A31F8, 1)]:
            assert int.from_bytes(native_ram[address:address + 4], "little") == expected
        cases.append({"fill": fill, "fpscr_bank": bank, "sr_mask": mask,
                      "register_seed": seed, "full_ram_cpu_fpu_match": True,
                      "three_service_boundaries_match": True,
                      "original_instructions_executed": steps})

    start, end = 0x8C6666C0, 0x8C666706
    file_start, file_end = start - 0x8C010000, end - 0x8C010000
    report = {
        "module": "1ST_READ.BIN", "routine": hex(start),
        "end_exclusive": hex(end), "cases_passed": len(cases),
        "original_instructions_executed": total_steps, "cases": cases,
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[file_start:file_end]).hexdigest(),
        "source": "source/sa_engine_memory_layout_initialize.c",
        "instruction_listing": "analysis/instructions/8c6666c0.tsv",
        "instruction_listing_sha256": hashlib.sha256(listing.read_bytes()).hexdigest(),
        "source_sha256": hashlib.sha256(source.read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Compares all 16 MiB RAM, general registers, SR/PR, both FPU banks, FPUL/MACL preservation, and three service-call boundaries in 24 cases across initial memory patterns and register/FPU/SR states. External services are boundary hooks with no mutations.",
        "limitations": ["Dispatcher and service bodies are not emulated in this function-level test.",
                        "No hardware rendering or whole-game boot claim is made."],
    }
    (project / "analysis/engine_memory_layout_initialize_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} full-state comparisons passed; {total_steps} retail instructions executed")


if __name__ == "__main__":
    main()
