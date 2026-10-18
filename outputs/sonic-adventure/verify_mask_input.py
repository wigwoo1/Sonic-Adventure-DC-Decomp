"""Compare a retail-constant bit-mask helper against its SH-4 implementation."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference


def main():
    out = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    old = bytearray(16 * 1024 * 1024)
    old[0x10000:0x10000 + len(binary)] = binary
    dll_path = ROOT / "work/sonic-adventure/build/mask_input.dll"
    fn = ctypes.CDLL(str(dll_path)).sa_mask_input_8c6607be
    fn.argtypes = [ctypes.c_uint32]
    fn.restype = ctypes.c_uint32
    mask = struct.unpack_from("<I", binary, 0x8C6607EC - 0x8C010000)[0]
    assert mask == 0x0FFFFFFF
    values = [0, 1, mask - 1, mask, mask + 1, 0x7FFFFFFF,
              0x80000000, 0xFFFFFFFF, 0xF000000F, 0x00F00000]
    rng = random.Random(0x8C6607BE)
    values.extend(rng.getrandbits(32) for _ in range(4096))
    cases = 0
    steps = 0
    for value in values:
        regs = State()
        regs.r[:] = [rng.getrandbits(32) for _ in range(16)]
        regs.r[4], regs.r[15] = value, 0x8CF00000
        regs.pc, regs.pr = 0x8C6607BE, 0x8C000100
        expected = State.from_buffer_copy(bytes(regs))
        steps += reference(old, expected)
        assert fn(value) == expected.r[0] == (value & mask), hex(value)
        cases += 1
    start, end = 0x8C6607BE, 0x8C6607C4
    report = {
        "cases_passed": cases, "routine_start": hex(start),
        "routine_end_exclusive": hex(end), "retail_constant_mask": hex(mask),
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[start - 0x8C010000:end - 0x8C010000]).hexdigest(),
        "retail_instructions_executed": steps,
        "source": "source/mask_input.c",
        "source_sha256": hashlib.sha256((out / "source/mask_input.c").read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Retail return-value comparison for mask boundary patterns and 4096 deterministic 32-bit inputs; constant independently read from the Rev A binary.",
        "limitations": ["CPU exceptions are outside this arithmetic leaf test."],
    }
    (out / "analysis/mask_input_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{cases} retail bit-mask comparisons passed")


if __name__ == "__main__":
    main()
