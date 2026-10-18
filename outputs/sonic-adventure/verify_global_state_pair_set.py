"""Compare global-state pair writes against their retail SH-4 routine."""
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
    dll_path = ROOT / "work/sonic-adventure/build/global_state_pair_set.dll"
    fn = ctypes.CDLL(str(dll_path)).sa_global_state_pair_set_8c65b468
    fn.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint32, ctypes.c_uint32]
    fn.restype = None
    first_address = 0x8C67C1E8
    second_address = 0x8C67C1EC
    first_offset, second_offset = 0x67C1E8, 0x67C1EC
    cases = 0
    steps = 0
    values = [(0, 0), (1, 0xFFFFFFFF), (0xFFFFFFFF, 1),
              (0x80000000, 0x7FFFFFFF), (0x12345678, 0x87654321)]
    rng = random.Random(0x8C65B468)
    values.extend((rng.getrandbits(32), rng.getrandbits(32)) for _ in range(59))
    for seed, pair in enumerate(values):
        first, second = pair
        sentinel_a = (0xA5A50000 ^ seed) & 0xFFFFFFFF
        sentinel_b = (0x5A5A0000 ^ (seed * 17)) & 0xFFFFFFFF
        import struct
        struct.pack_into("<II", old, first_offset, sentinel_a, sentinel_b)
        struct.pack_into("<II", native, first_offset, sentinel_a, sentinel_b)
        regs = State()
        regs.r[:] = [rng.getrandbits(32) for _ in range(16)]
        regs.r[4], regs.r[5] = first, second
        regs.r[15], regs.pc, regs.pr = 0x8CF00000, 0x8C65B468, 0x8C000100
        expected = State.from_buffer_copy(bytes(regs))
        steps += reference(old, expected)
        fn(view, first, second)
        assert native == old, (hex(first), hex(second))
        assert int.from_bytes(native[first_offset:first_offset + 4], "little") == first
        assert int.from_bytes(native[second_offset:second_offset + 4], "little") == second
        assert expected.pr == 0x8C000100
        cases += 1
    start, end = 0x8C65B468, 0x8C65B472
    report = {
        "cases_passed": cases, "routine_start": hex(start),
        "routine_end_exclusive": hex(end), "literal_targets": [hex(first_address), hex(second_address)],
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[start - 0x8C010000:end - 0x8C010000]).hexdigest(),
        "retail_instructions_executed": steps,
        "source": "source/global_state_pair_set.c",
        "source_sha256": hashlib.sha256((out / "source/global_state_pair_set.c").read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Full 16 MiB RAM comparison for two retail pointer-literal destinations, fixed edge-case and deterministic random input pairs, and sentinel preservation.",
        "limitations": ["Invalid pointer-literal targets and SH-4 exception behavior are outside the test scope."],
    }
    (out / "analysis/global_state_pair_set_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{cases} global-state pair comparisons passed")


if __name__ == "__main__":
    main()
