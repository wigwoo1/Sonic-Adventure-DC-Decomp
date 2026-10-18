"""Compare the bounded zero-prefix bit scanner with retail SH-4 execution."""
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
    dll_path = ROOT / "work/sonic-adventure/build/zero_prefix_bits.dll"
    fn = ctypes.CDLL(str(dll_path)).sa_count_zero_prefix_bits_8c603c4c
    fn.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint32, ctypes.c_int32]
    fn.restype = ctypes.c_uint32
    region = 0x00A00000
    cases = steps = 0
    fixtures = []
    for byte_limit in (64, 128, 192):
        for byte_index in range(byte_limit):
            fixtures.append((byte_limit // 64, byte_index, 0x80, byte_index * 8))
    for byte_value in range(256):
        expected_value = next((bit for bit in range(8) if byte_value & (0x80 >> bit)), 512)
        fixtures.append((1, 0, byte_value, expected_value))
    for bit in range(8):
        for byte_index in (1, 31, 63):
            fixtures.append((1, byte_index, 0x80 >> bit, byte_index * 8 + bit))
    fixtures.extend((n, None, None, n * 512) for n in (1, 2, 3))
    fixtures.extend((n, None, None, 0) for n in (0, -1, -0x80000000,
                                                   0x02000000, 0x20000000))
    fixtures.append((0x01000000, 0, 0x80, 0))
    rng = random.Random(0x8C603C4C)
    for block_count in (1, 2, 3):
        for _ in range(100):
            byte_index = rng.randrange(block_count * 64)
            value = rng.randrange(1, 256)
            first_bit = next(bit for bit in range(8) if value & (0x80 >> bit))
            fixtures.append((block_count, byte_index, value,
                             byte_index * 8 + first_bit))

    for alias in (0x8C000000, 0xAC000000):
        for block_count, byte_index, value, _known in fixtures:
            region_data = bytes(0x400)
            old[region:region + len(region_data)] = region_data
            native[region:region + len(region_data)] = region_data
            if byte_index is not None:
                old[region + byte_index] = value
                native[region + byte_index] = value
            data = alias + region
            regs = State()
            regs.r[:] = [rng.getrandbits(32) for _ in range(16)]
            regs.r[4], regs.r[5], regs.r[15] = data, block_count & 0xFFFFFFFF, 0x8CF00000
            regs.pc, regs.pr = 0x8C603C4C, 0x8C000100
            expected = State.from_buffer_copy(bytes(regs))
            steps += reference(old, expected)
            actual = fn(view, data, block_count)
            assert actual == expected.r[0] == _known, (block_count, byte_index,
                                                       value, hex(actual), hex(expected.r[0]), _known)
            assert native[region:region + len(region_data)] == old[region:region + len(region_data)]
            cases += 1
    start, end = 0x8C603C4C, 0x8C603C84
    report = {
        "cases_passed": cases, "routine_start": hex(start),
        "routine_end_exclusive": hex(end), "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[start - 0x8C010000:end - 0x8C010000]).hexdigest(),
        "retail_instructions_executed": steps,
        "source": "source/zero_prefix_bits.c",
        "source_sha256": hashlib.sha256((out / "source/zero_prefix_bits.c").read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Retail return and read-only memory comparison across every byte position in 64/128/192-byte limits, all first-byte values, every leading-bit boundary, zero-filled buffers, signed-length edge cases and deterministic samples.",
        "limitations": ["Unmapped input pointers and SH-4 exception behavior are outside this scanner test."],
    }
    (out / "analysis/zero_prefix_bits_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{cases} zero-prefix bit scans matched retail")


if __name__ == "__main__":
    main()
