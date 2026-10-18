"""Compare the forward word-copy loop with retail for nonzero counts."""
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
    dll_path = ROOT / "work/sonic-adventure/build/word_copy_forward.dll"
    fn = ctypes.CDLL(str(dll_path)).sa_word_copy_forward_8c6044e0
    fn.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint32,
                   ctypes.c_uint32, ctypes.c_uint32]
    fn.restype = None
    counts = list(range(1, 65)) + [96, 127, 128, 255]
    patterns = ((0x180, 0x180), (0x280, 0x100),
                (0x104, 0x100), (0x100, 0x104))
    rng = random.Random(0x8C6044E0)
    cases = steps = 0
    for alias in (0x8C000000, 0xAC000000):
        for count in counts:
            for dst_offset, src_offset in patterns:
                region = 0x00A00000
                fixture = bytes(rng.getrandbits(8) for _ in range(0x500))
                old[region:region + len(fixture)] = fixture
                native[region:region + len(fixture)] = fixture
                destination = alias + region + dst_offset
                source = alias + region + src_offset
                regs = State()
                regs.r[:] = [rng.getrandbits(32) for _ in range(16)]
                regs.r[4:7] = [destination, source, count]
                regs.r[15], regs.pc, regs.pr = 0x8CF00000, 0x8C6044E0, 0x8C000100
                expected = State.from_buffer_copy(bytes(regs))
                steps += reference(old, expected)
                fn(view, destination, source, count)
                assert native[region:region + len(fixture)] == old[region:region + len(fixture)], (
                    hex(destination), hex(source), count)
                assert expected.pr == 0x8C000100
                cases += 1
    start, end = 0x8C6044E0, 0x8C6044EE
    report = {
        "cases_passed": cases, "routine_start": hex(start),
        "routine_end_exclusive": hex(end), "count_unit": "32-bit words",
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[start - 0x8C010000:end - 0x8C010000]).hexdigest(),
        "retail_instructions_executed": steps,
        "source": "source/word_copy_forward.c",
        "source_sha256": hashlib.sha256((out / "source/word_copy_forward.c").read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Copied-region comparison for two RAM aliases, 68 nonzero word counts, aligned equal/disjoint/forward-overlap/backward-overlap layouts and untouched-memory guards.",
        "limitations": ["Count zero is omitted because the retail do-while loop wraps its 32-bit count and requires 2^32 iterations.",
                        "Unaligned addresses, faults and SH-4 exceptions are outside the test scope."],
    }
    (out / "analysis/word_copy_forward_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{cases} forward word-copy comparisons passed")


if __name__ == "__main__":
    main()
