"""Compare the two-field record writer and constant result against retail."""
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
    dll_path = ROOT / "work/sonic-adventure/build/record_pair_result.dll"
    fn = ctypes.CDLL(str(dll_path)).sa_record_fields18_1c_set_8c65c200
    fn.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint32,
                   ctypes.c_uint32, ctypes.c_uint32]
    fn.restype = ctypes.c_uint32
    cases = 0
    steps = 0
    for alias, object_offset in itertools.product((0x8C000000, 0xAC000000),
                                                  (0x00A00000, 0x00B00000)):
        object_address = alias + object_offset
        for seed in range(32):
            region = bytes((seed * 11 + i * 23) & 0xFF for i in range(0x60))
            old[object_offset:object_offset + len(region)] = region
            native[object_offset:object_offset + len(region)] = region
            rng = random.Random(object_address ^ seed ^ 0x8C65C200)
            field1c, field18 = rng.getrandbits(32), rng.getrandbits(32)
            regs = State()
            regs.r[:] = [rng.getrandbits(32) for _ in range(16)]
            regs.r[4:7] = [object_address, field1c, field18]
            regs.r[15], regs.pc, regs.pr = 0x8CF00000, 0x8C65C200, 0x8C000100
            expected = State.from_buffer_copy(bytes(regs))
            steps += reference(old, expected)
            result = fn(view, object_address, field1c, field18)
            assert result == expected.r[0] == 1
            assert native == old, (hex(object_address), hex(field1c), hex(field18))
            cases += 1
    start, end = 0x8C65C200, 0x8C65C208
    report = {
        "cases_passed": cases, "routine_start": hex(start),
        "routine_end_exclusive": hex(end),
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[start - 0x8C010000:end - 0x8C010000]).hexdigest(),
        "retail_instructions_executed": steps,
        "source": "source/record_pair_result.c",
        "source_sha256": hashlib.sha256((out / "source/record_pair_result.c").read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Full 16 MiB RAM and R0 comparison for two RAM aliases, two object regions, thirty-two deterministic value pairs and unchanged-memory guards.",
        "limitations": ["Invalid object pointers and SH-4 exceptions are outside the test scope."],
    }
    (out / "analysis/record_pair_result_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{cases} record pair result comparisons passed")


if __name__ == "__main__":
    main()
