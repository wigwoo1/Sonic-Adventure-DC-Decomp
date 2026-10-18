"""Compare the engine state mirror against retail SH-4 instruction execution."""
import ctypes
import hashlib
import itertools
import json
import pathlib
import struct

from verify_startup import State, reference

ROOT = pathlib.Path(__file__).resolve().parent


def main():
    project = ROOT
    workspace = ROOT.parent.parent
    binary = (workspace / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    source = project / "source/engine_state_mirror.c"
    listing = project / "analysis/instructions/8c107ac6.tsv"
    dll_path = workspace / "work/sonic-adventure/build/engine_state_mirror.dll"
    native = ctypes.CDLL(str(dll_path)).sa_engine_state_mirror_8c107ac6
    ByteP = ctypes.POINTER(ctypes.c_uint8)
    U32P = ctypes.POINTER(ctypes.c_uint32)
    native.argtypes = [ByteP, U32P, U32P]
    native.restype = None

    state_values = [0, 1, 2, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF, 0x12345678, 0xFFFF0001]
    cases = []
    for enabled, value, t in itertools.product([0, 1, 0x80000000, 0xFFFFFFFF], state_values, [0, 1]):
        ram = bytearray(16 * 1024 * 1024)
        ram[0x10000:0x10000 + len(binary)] = binary
        struct.pack_into("<I", ram, 0x5FBA60, enabled)
        struct.pack_into("<I", ram, 0x5FBAA8, 0xA5A5A5A5)
        struct.pack_into("<I", ram, 0x5FBAB0, 0x5A5A5A5A)
        state = State()
        state.r[:] = [0x22340000 + i for i in range(16)]
        state.r[4] = value
        state.r[15] = 0x8CF00000
        state.sr = 0x70000002 | t
        state.pr = 0x8C000100
        state.pc = 0x8C107AC6

        retail_ram = bytearray(ram)
        retail_state = State.from_buffer_copy(bytes(state))
        steps = reference(retail_ram, retail_state, stop_pc=state.pr)
        native_ram = bytearray(ram)
        view = (ctypes.c_uint8 * len(native_ram)).from_buffer(native_ram)
        regs = (ctypes.c_uint32 * 16)(*state.r)
        sr = ctypes.c_uint32(state.sr)
        native(view, ctypes.byref(sr), regs)

        assert native_ram == retail_ram
        assert list(regs) == list(retail_state.r)
        assert sr.value == retail_state.sr
        if enabled:
            assert struct.unpack_from("<I", native_ram, 0x5FBAA8)[0] == value
            assert struct.unpack_from("<I", native_ram, 0x5FBAB0)[0] == value
        else:
            assert struct.unpack_from("<I", native_ram, 0x5FBAA8)[0] == 0xA5A5A5A5
            assert struct.unpack_from("<I", native_ram, 0x5FBAB0)[0] == 0x5A5A5A5A
        cases.append({"enable_value": enabled, "state_value": value, "initial_t": t,
                      "original_instructions_executed": steps, "full_ram_register_match": True})

    start, end = 0x8C107AC6, 0x8C107ADA
    file_start, file_end = start - 0x8C010000, end - 0x8C010000
    report = {
        "module": "1ST_READ.BIN",
        "routine": hex(start),
        "end_exclusive": hex(end),
        "cases_passed": len(cases),
        "original_instructions_executed": sum(c["original_instructions_executed"] for c in cases),
        "cases": cases,
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[file_start:file_end]).hexdigest(),
        "source": "source/engine_state_mirror.c",
        "instruction_listing": "analysis/instructions/8c107ac6.tsv",
        "instruction_listing_sha256": hashlib.sha256(listing.read_bytes()).hexdigest(),
        "source_sha256": hashlib.sha256(source.read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Compares every byte of 16 MiB RAM and all general registers/SR against retail SH-4 execution in 64 cases, including zero/nonzero enable states, both T-bit starting values, and signed/unsigned state-value edges.",
        "limitations": ["The SH-4 instruction interpreter is a test harness, not hardware emulation.",
                        "Call-site integration and engine runtime behavior are not established by this leaf test."],
    }
    (project / "analysis/engine_state_mirror_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} full RAM/register retail comparisons passed; "
          f"{report['original_instructions_executed']} instructions executed")


if __name__ == "__main__":
    main()
