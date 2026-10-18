"""Compare the forward byte-copy loop with retail for nonzero lengths."""
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
    dll_path = ROOT / "work/sonic-adventure/build/byte_copy_forward.dll"
    fn = ctypes.CDLL(str(dll_path)).sa_byte_copy_forward_8c604500
    fn.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint32,
                   ctypes.c_uint32, ctypes.c_uint32]
    fn.restype = None
    lengths = list(range(1, 65)) + [96, 127, 128, 255]
    patterns = ((0x180, 0x180), (0x280, 0x100),
                (0x110, 0x100), (0x100, 0x110))
    rng = random.Random(0x8C604500)
    cases = 0
    steps = 0
    for alias in (0x8C000000, 0xAC000000):
        for length in lengths:
            for pattern_index, (dst_offset, src_offset) in enumerate(patterns):
                region = 0x00A00000
                fixture = bytes(rng.getrandbits(8) for _ in range(0x400))
                old[region:region + len(fixture)] = fixture
                native[region:region + len(fixture)] = fixture
                destination, source = alias + region + dst_offset, alias + region + src_offset
                regs = State()
                regs.r[:] = [rng.getrandbits(32) for _ in range(16)]
                regs.r[4:7] = [destination, source, length]
                regs.r[15], regs.pc, regs.pr = 0x8CF00000, 0x8C604500, 0x8C000100
                expected = State.from_buffer_copy(bytes(regs))
                steps += reference(old, expected)
                fn(view, destination, source, length)
                assert native[region:region + len(fixture)] == old[region:region + len(fixture)], (
                    hex(destination), hex(source), length, pattern_index)
                assert expected.pr == 0x8C000100
                cases += 1
    start, end = 0x8C604500, 0x8C60450E
    report = {
        "cases_passed": cases, "routine_start": hex(start),
        "routine_end_exclusive": hex(end),
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[start - 0x8C010000:end - 0x8C010000]).hexdigest(),
        "retail_instructions_executed": steps,
        "source": "source/byte_copy_forward.c",
        "source_sha256": hashlib.sha256((out / "source/byte_copy_forward.c").read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Full copied-region comparison for two RAM aliases, 68 nonzero lengths and equal, disjoint, forward-overlap and backward-overlap source/destination layouts.",
        "limitations": ["Length zero is omitted: retail executes its do-while loop and wraps the 32-bit counter, requiring 2^32 iterations before termination.",
                        "Address faults and SH-4 exceptions are outside the test scope."],
    }
    (out / "analysis/byte_copy_forward_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{cases} forward-copy comparisons passed")


if __name__ == "__main__":
    main()
