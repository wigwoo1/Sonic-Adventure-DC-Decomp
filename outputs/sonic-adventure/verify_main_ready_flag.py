"""Compare the recovered ready-flag routine against retail SH-4 execution."""
import ctypes
import hashlib
import json
import pathlib
import random
import struct

from verify_startup import State, reference

ROOT = pathlib.Path(__file__).resolve().parent
BASE = 0x8C000000


def main():
    project = ROOT
    workspace = ROOT.parent.parent
    retail = workspace / "work/sonic-adventure/extracted/1ST_READ.BIN"
    binary = retail.read_bytes()
    source = project / "source/main_ready_flag.c"
    listing = project / "analysis/instructions/8c04c680.tsv"
    dll = workspace / "work/sonic-adventure/build/main_ready_flag.dll"
    native = ctypes.CDLL(str(dll)).sa_main_ready_flag_8c04c680
    native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(ctypes.c_uint32)]
    native.restype = None

    rng = random.Random(0x8C04C680)
    cases = []
    for case in range(16):
        ram = bytearray(rng.randbytes(16 * 1024 * 1024))
        ram[0x10000:0x10000 + len(binary)] = binary
        registers = [rng.getrandbits(32) for _ in range(16)]
        state = State()
        state.r[:] = registers
        state.sr = rng.getrandbits(32)
        state.pr = 0x8C000100
        state.pc = 0x8C04C680
        state_before = list(state.r)
        original = bytearray(ram)
        steps = reference(ram, state, stop_pc=state.pr)

        storage = (ctypes.c_uint8 * len(original)).from_buffer(original)
        regs = (ctypes.c_uint32 * 16)(*registers)
        native(storage, regs)

        assert original == ram, f"RAM mismatch in case {case}"
        assert list(regs) == list(state.r), f"register mismatch in case {case}"
        assert state.r[2] == 0x8C19DE26 and state.r[3] == 1
        assert struct.unpack_from("<H", ram, 0x19DE26)[0] == 1
        changed = [i for i, (a, b) in enumerate(zip(state_before, state.r)) if a != b]
        assert changed == [2, 3], changed
        cases.append({"case": case, "original_instructions_executed": steps,
                      "full_ram_matches": True, "all_registers_match": True})

    report = {
        "module": "1ST_READ.BIN",
        "routine": "0x8c04c680",
        "end_exclusive": "0x8c04c688",
        "cases_passed": len(cases),
        "original_instructions_executed": sum(c["original_instructions_executed"] for c in cases),
        "cases": cases,
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[0x3C680:0x3C688]).hexdigest(),
        "source": "source/main_ready_flag.c",
        "instruction_listing": "analysis/instructions/8c04c680.tsv",
        "instruction_listing_sha256": hashlib.sha256(listing.read_bytes()).hexdigest(),
        "source_sha256": hashlib.sha256(source.read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll.read_bytes()).hexdigest(),
        "scope": "Compares all 16 MiB RAM bytes and all general registers for 16 randomized input states against execution of the retail instructions. PC/SR are harness control state; the return address is the execution stop condition.",
        "limitations": ["The SH-4 instruction interpreter is a test harness, not a hardware emulator.",
                        "This leaf routine alone does not establish initialization order or whole-game behavior."],
    }
    (project / "analysis/main_ready_flag_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} cases passed; {report['original_instructions_executed']} retail instructions executed")


if __name__ == "__main__":
    main()
