"""Differentially compare the bitstream padding helper with retail SH-4 code."""
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
    dll_path = ROOT / "work/sonic-adventure/build/bitstream_pad.dll"
    fn = ctypes.CDLL(str(dll_path)).sa_bitstream_pad_8c603c84
    fn.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_int32,
                   ctypes.c_uint32, ctypes.c_uint32]
    fn.restype = ctypes.c_uint32

    region = 0x00A00000
    destination = 0x8C000000 + region
    fixtures = []
    for blocks in (0, 1, 2, 3, 4, 0x02000000, 0x02000001, 0x04000001):
        byte_count = (blocks << 6) & 0xffffffff
        if byte_count & 0x80000000:
            byte_count -= 0x100000000
        # Exhaust every transition where the helper switches from full zero
        # bytes to partial-byte/one-suffix padding for bounded loop lengths.
        if 0 < byte_count <= 256:
            fixtures.extend((bits, blocks) for bits in range(-3, byte_count * 8 + 10))
        else:
            fixtures.extend((bits, blocks) for bits in (-0x80000000, -1, 0, 1, 7, 8, 9, 63, 64, 255))

    rng = random.Random(0x8C603C84)
    fixtures.extend((rng.randint(-100, 2200), rng.choice((1, 2, 3, 4)))
                    for _ in range(300))
    cases = steps = 0
    max_bytes = 256
    for alias in (0x8C000000, 0xAC000000):
        for valid_bits, blocks in fixtures:
            initial = bytes(rng.getrandbits(8) for _ in range(max_bytes + 16))
            old[region:region + len(initial)] = initial
            native[region:region + len(initial)] = initial
            regs = State()
            regs.r[:] = [rng.getrandbits(32) for _ in range(16)]
            regs.r[4], regs.r[5], regs.r[6], regs.r[15] = (
                valid_bits & 0xffffffff, alias + region, blocks, 0x8CF00000)
            regs.pc, regs.pr = 0x8C603C84, 0x8C000100
            expected = State.from_buffer_copy(bytes(regs))
            steps += reference(old, expected)
            actual = fn(view, valid_bits, alias + region, blocks)
            assert actual == expected.r[0] == 0, (valid_bits, blocks, hex(actual), hex(expected.r[0]))
            byte_count = (blocks << 6) & 0xffffffff
            byte_count = byte_count - 0x100000000 if byte_count & 0x80000000 else byte_count
            written = max(0, min(byte_count, max_bytes + 16))
            assert native[region:region + max_bytes + 16] == old[region:region + max_bytes + 16], (
                valid_bits, blocks, native[region:region + max_bytes + 16].hex(),
                old[region:region + max_bytes + 16].hex())
            # Ensure untouched suffix matches as well; initial bytes are retained.
            assert native[region + written:region + max_bytes + 16] == initial[written:]
            cases += 1

    start, end = 0x8C603C84, 0x8C603CD2
    report = {
        "cases_passed": cases, "routine_start": hex(start),
        "routine_end_exclusive": hex(end),
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[start - 0x8C010000:end - 0x8C010000]).hexdigest(),
        "retail_instructions_executed": steps,
        "source": "source/bitstream_pad.c",
        "source_sha256": hashlib.sha256((out / "source/bitstream_pad.c").read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Return and full bounded-memory comparison across both cached/uncached RAM aliases, exhaustive valid-bit transitions for 1-256-byte blocks, signed and wrapped block-count cases, and deterministic random inputs.",
        "limitations": ["Unmapped pointers, SH-4 exceptions/cache side effects, and impractically large positive wrapped block counts are not tested."],
    }
    (out / "analysis/bitstream_pad_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{cases} bitstream padding cases matched retail")


if __name__ == "__main__":
    main()
