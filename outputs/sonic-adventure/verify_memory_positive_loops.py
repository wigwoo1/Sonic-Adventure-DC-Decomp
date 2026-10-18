"""Compare positive-length byte copy and 0xff fill loops with retail."""
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
    dll_path = ROOT / "work/sonic-adventure/build/memory_positive_loops.dll"
    dll = ctypes.CDLL(str(dll_path))
    copy = dll.sa_byte_copy_positive_8c603c2e
    copy.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint32,
                     ctypes.c_uint32, ctypes.c_int32]
    copy.restype = None
    fill = dll.sa_byte_fill_ff_positive_8c603c38
    fill.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint32, ctypes.c_int32]
    fill.restype = None
    lengths = [-0x80000000, -127, -2, -1, 0, 1, 2, 3, 4, 7, 8,
               15, 16, 31, 32, 63, 64, 96, 127, 128, 255]
    copy_patterns = ((0x180, 0x180), (0x280, 0x100),
                     (0x101, 0x100), (0x100, 0x101))
    rng = random.Random(0x8C603C2E)
    copy_cases = fill_cases = steps = 0
    region = 0x00A00000
    for alias in (0x8C000000, 0xAC000000):
        for length in lengths:
            for destination_offset, source_offset in copy_patterns:
                fixture = bytes(rng.getrandbits(8) for _ in range(0x500))
                old[region:region + len(fixture)] = fixture
                native[region:region + len(fixture)] = fixture
                destination = alias + region + destination_offset
                source = alias + region + source_offset
                regs = State()
                regs.r[:] = [rng.getrandbits(32) for _ in range(16)]
                regs.r[4:7] = [source, destination, length & 0xFFFFFFFF]
                regs.r[15], regs.pc, regs.pr = 0x8CF00000, 0x8C603C2E, 0x8C000100
                expected = State.from_buffer_copy(bytes(regs))
                steps += reference(old, expected)
                copy(view, destination, source, length)
                if native[region:region + len(fixture)] != old[region:region + len(fixture)]:
                    mismatches = [i for i, (left, right) in enumerate(zip(
                        native[region:region + len(fixture)], old[region:region + len(fixture)]))
                        if left != right]
                    raise AssertionError((length, hex(destination), hex(source),
                                          [(hex(i), hex(native[region + i]), hex(old[region + i]))
                                           for i in mismatches[:12]]))
                assert expected.pr == 0x8C000100
                copy_cases += 1

            value = rng.getrandbits(32)
            fixture = bytes(rng.getrandbits(8) for _ in range(0x300))
            old[region:region + len(fixture)] = fixture
            native[region:region + len(fixture)] = fixture
            destination = alias + region + 0x100
            regs = State()
            regs.r[:] = [rng.getrandbits(32) for _ in range(16)]
            regs.r[4], regs.r[5] = destination, length & 0xFFFFFFFF
            regs.r[15], regs.pc, regs.pr = 0x8CF00000, 0x8C603C38, 0x8C000100
            expected = State.from_buffer_copy(bytes(regs))
            steps += reference(old, expected)
            fill(view, destination, length)
            assert native[region:region + len(fixture)] == old[region:region + len(fixture)], (
                length, hex(destination))
            assert expected.pr == 0x8C000100
            fill_cases += 1

    routines = [
        {"name": "sa_byte_copy_positive_8c603c2e", "routine_start": "0x8c603c2e",
         "routine_end_exclusive": "0x8c603c3e", "cases_passed": copy_cases},
        {"name": "sa_byte_fill_ff_positive_8c603c38", "routine_start": "0x8c603c38",
         "routine_end_exclusive": "0x8c603c4c", "cases_passed": fill_cases},
    ]
    for routine in routines:
        start, end = int(routine["routine_start"], 16), int(routine["routine_end_exclusive"], 16)
        routine["retail_span_sha256"] = hashlib.sha256(
            binary[start - 0x8C010000:end - 0x8C010000]).hexdigest()
    report = {
        "routines": routines, "cases_passed": copy_cases + fill_cases,
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_instructions_executed": steps,
        "source": "source/memory_positive_loops.c",
        "source_sha256": hashlib.sha256((out / "source/memory_positive_loops.c").read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Copied/fill region comparisons for two RAM aliases, positive and nonpositive signed lengths, overlapping/disjoint copy layouts and unchanged-memory guards.",
        "limitations": ["Unaligned word behavior is irrelevant; these routines operate on bytes.",
                        "Address faults and SH-4 exceptions are outside the test scope."],
    }
    (out / "analysis/memory_positive_loops_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{copy_cases} positive byte-copy and {fill_cases} ff-fill comparisons passed")


if __name__ == "__main__":
    main()
