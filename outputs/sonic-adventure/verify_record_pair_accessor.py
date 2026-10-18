"""Compare the null-safe two-field setter against its retail SH-4 body."""
import ctypes
import hashlib
import itertools
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
    dll_path = ROOT / "work/sonic-adventure/build/record_pair_accessor.dll"
    fn = ctypes.CDLL(str(dll_path)).sa_record_fields10_14_set_8c65a5b4
    fn.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint32,
                   ctypes.c_uint32, ctypes.c_uint32]
    fn.restype = ctypes.c_uint32
    cases = 0
    steps = 0
    object_regions = (0, 0x00A00000, 0x00B00000)
    for alias, region_offset in itertools.product((0x8C000000, 0xAC000000), object_regions):
        object_address = 0 if region_offset == 0 else alias + region_offset
        for seed in range(16):
            rng = random.Random(0x8C65A5B4 ^ object_address ^ seed)
            values = (rng.getrandbits(32), rng.getrandbits(32))
            if region_offset:
                region = bytes((seed * 3 + i * 29) & 0xFF for i in range(0x60))
                old[region_offset:region_offset + len(region)] = region
                native[region_offset:region_offset + len(region)] = region
            regs = State()
            regs.r[:] = [rng.getrandbits(32) for _ in range(16)]
            regs.r[4:7] = [object_address, values[0], values[1]]
            regs.r[15], regs.pc, regs.pr = 0x8CF00000, 0x8C65A5B4, 0x8C000100
            expected = State.from_buffer_copy(bytes(regs))
            steps += reference(old, expected)
            result = fn(view, object_address, *values)
            assert result == expected.r[0], (hex(object_address), values,
                                              hex(result), hex(expected.r[0]))
            assert native == old, (hex(object_address), values)
            cases += 1
    start, end = 0x8C65A5B4, 0x8C65A5C8
    report = {
        "cases_passed": cases, "routine_start": hex(start),
        "routine_end_exclusive": hex(end),
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[start - 0x8C010000:end - 0x8C010000]).hexdigest(),
        "retail_instructions_executed": steps,
        "source": "source/record_pair_accessor.c",
        "source_sha256": hashlib.sha256((out / "source/record_pair_accessor.c").read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Full 16 MiB RAM and return-value comparison for null/non-null objects, two RAM aliases, two object regions, and sixteen deterministic input pairs per combination.",
        "limitations": ["Invalid non-null object pointers and SH-4 exception behavior are outside the test scope."],
    }
    (out / "analysis/record_pair_accessor_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{cases} null-safe record accessor comparisons passed")


if __name__ == "__main__":
    main()
