"""Compare backward byte fill with retail for nonzero lengths."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference


def main():
    out = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    old = bytearray(16 * 1024 * 1024)
    old[0x10000:0x10000 + len(binary)] = binary
    native = bytearray(old)
    view = (ctypes.c_uint8 * len(native)).from_buffer(native)
    dll_path = ROOT / "work/sonic-adventure/build/byte_fill_backward.dll"
    fn = ctypes.CDLL(str(dll_path)).sa_byte_fill_backward_8c0869a4
    fn.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint32,
                   ctypes.c_uint32, ctypes.c_uint32]
    fn.restype = None
    lengths = list(range(1, 65)) + [96, 127, 128, 255]
    fill_values = (0, 1, 0x7F, 0x80, 0xFF, 0x1FF)
    rng = random.Random(0x8C0869A4)
    cases = steps = 0
    for alias in (0x8C000000, 0xAC000000):
        for length in lengths:
            for value in fill_values:
                region, start = 0x00A00000, 0x180
                fixture = bytes(rng.getrandbits(8) for _ in range(0x400))
                old[region:region + len(fixture)] = fixture
                native[region:region + len(fixture)] = fixture
                destination = alias + region + start
                regs = State()
                regs.r[:] = [rng.getrandbits(32) for _ in range(16)]
                regs.r[4:7] = [destination, value, length]
                regs.r[15], regs.pc, regs.pr = 0x8CF00000, 0x8C0869A4, 0x8C000100
                expected = State.from_buffer_copy(bytes(regs))
                steps += reference(old, expected)
                fn(view, destination, value, length)
                assert native[region:region + len(fixture)] == old[region:region + len(fixture)], (
                    hex(destination), value, length)
                assert expected.pr == 0x8C000100
                cases += 1
    start_address, end = 0x8C0869A4, 0x8C0869B0
    report = {
        "cases_passed": cases, "routine_start": hex(start_address),
        "routine_end_exclusive": hex(end),
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[start_address - 0x8C010000:end - 0x8C010000]).hexdigest(),
        "retail_instructions_executed": steps,
        "source": "source/byte_fill_backward.c",
        "source_sha256": hashlib.sha256((out / "source/byte_fill_backward.c").read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Full fixture-region comparison for two RAM aliases, 68 nonzero lengths, six fill inputs and untouched-memory guards.",
        "limitations": ["Length zero is omitted because retail uses a do-while counter that wraps for 2^32 iterations.",
                        "Address faults and SH-4 exceptions are outside the test scope."],
    }
    (out / "analysis/byte_fill_backward_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{cases} backward byte-fill comparisons passed")


if __name__ == "__main__":
    main()
