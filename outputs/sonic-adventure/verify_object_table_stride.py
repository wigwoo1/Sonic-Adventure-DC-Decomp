"""Compare the object-table index helper with retail SH-4 execution."""
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
    view = (ctypes.c_uint8 * len(ram)).from_buffer(ram)
    dll_path = ROOT / "work/sonic-adventure/build/object_table_stride.dll"
    fn = ctypes.CDLL(str(dll_path)).sa_object_table_stride_8c646856
    fn.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint32]
    fn.restype = ctypes.c_uint32

    rng = random.Random(0x8c646856)
    indices = [0, 1, 2, 3, 0x7fffffff, 0x80000000, 0xffffffff]
    indices += [rng.getrandbits(32) for _ in range(193)]
    steps = 0
    for index in indices:
        regs = State()
        regs.r[:] = [rng.getrandbits(32) for _ in range(16)]
        regs.r[4] = index
        regs.r[15] = 0x8cf00000
        regs.pc, regs.pr = 0x8c646856, 0x8c000100
        expected = State.from_buffer_copy(bytes(regs))
        steps += reference(ram, expected)
        actual = fn(view, index)
        assert actual == expected.r[0], (
            f"index={index:#010x}, native={actual:#010x}, retail={expected.r[0]:#010x}")

    start, end = 0x8c646856, 0x8c646868
    report = {
        "cases_passed": len(indices), "routine_start": hex(start),
        "routine_end_exclusive": hex(end),
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[start - 0x8c010000:end - 0x8c010000]).hexdigest(),
        "retail_instructions_executed": steps,
        "source": "source/object_table_stride.c",
        "source_sha256": hashlib.sha256((out / "source/object_table_stride.c").read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Return value compared for boundary and 193 deterministic arbitrary 32-bit indices; arithmetic wraparound included.",
        "limitations": [],
    }
    (out / "analysis/object_table_stride_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(indices)} object-table stride cases matched retail")


if __name__ == "__main__":
    main()
