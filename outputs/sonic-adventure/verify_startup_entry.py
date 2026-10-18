"""Compare the reset-entry MMIO update and tail transfer against retail SH-4."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference


def main():
    out = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    ram = bytearray(16 * 1024 * 1024)
    ram[0x10000:0x10000 + len(binary)] = binary
    dll_path = ROOT / "work/sonic-adventure/build/startup_entry.dll"
    dll = ctypes.CDLL(str(dll_path))
    fn = dll.sa_startup_entry_8c010000
    fn.argtypes = [ctypes.POINTER(State), ctypes.POINTER(ctypes.c_uint32)]
    fn.restype = None

    rng = random.Random(0x8c010000)
    cases = []
    values = [0, 0xffffffff, 0x000089af, 0xfffff7ff]
    values += [rng.getrandbits(32) for _ in range(196)]
    for index, initial in enumerate(values):
        state = State()
        state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        state.sr = rng.getrandbits(32)
        state.pr = rng.getrandbits(32)
        state.pc = 0x8c010000
        expected = State.from_buffer_copy(bytes(state))
        expected_mmio = {0xff00001c: initial}
        steps = reference(ram, expected, mmio=expected_mmio, stop_pc=0x8c0100a0)

        actual_mmio = ctypes.c_uint32(initial)
        fn(ctypes.byref(state), ctypes.byref(actual_mmio))
        assert bytes(state) == bytes(expected), f"CPU-state mismatch in case {index}"
        assert actual_mmio.value == expected_mmio[0xff00001c], f"MMIO mismatch in case {index}"
        cases.append({"case": index, "initial_mmio": hex(initial),
                      "retail_instructions_executed": steps, "cpu_and_mmio_match": True})

    start, end = 0x8c010000, 0x8c010020
    report = {
        "cases_passed": len(cases), "routine_start": hex(start),
        "routine_end_exclusive": hex(end),
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[start - 0x8c010000:end - 0x8c010000]).hexdigest(),
        "source": "source/startup_entry.c",
        "source_sha256": hashlib.sha256((out / "source/startup_entry.c").read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "cases": cases,
        "scope": "Full GPR/SR/PR/PC state and startup-control MMIO result compared for fixed edge cases and 196 deterministic random register values.",
        "limitations": ["MMIO behavior is modeled as a single host-provided 32-bit register; this is not hardware validation."],
    }
    (out / "analysis/startup_entry_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} reset-entry CPU/MMIO cases matched retail")


if __name__ == "__main__":
    main()
