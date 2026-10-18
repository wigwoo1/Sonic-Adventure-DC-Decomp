"""Compare all 24 compacted directory rows with retail SH-4 output."""
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
    native = bytearray(old)
    view = (ctypes.c_uint8 * len(native)).from_buffer(native)
    dll_path = ROOT / "work/sonic-adventure/build/object_table_compact.dll"
    fn = ctypes.CDLL(str(dll_path)).sa_object_table_compact_8c6461ae
    fn.argtypes = [ctypes.POINTER(ctypes.c_uint8)]
    fn.restype = None

    source_slot = 0x8c8a83b4 - 0x8c000000
    destination = 0x8c893bd0
    destination_offset = destination - 0x8c000000
    region = 0x00a00000
    rng = random.Random(0x8c6461ae)
    cases = steps = 0
    for alias in (0x8c000000, 0xac000000):
        for _ in range(48):
            source = alias + region
            source_data = bytes(rng.getrandbits(8) for _ in range(24 * 0x2c + 32))
            old[region:region + len(source_data)] = source_data
            native[region:region + len(source_data)] = source_data
            old[source_slot:source_slot + 4] = struct.pack("<I", source)
            native[source_slot:source_slot + 4] = struct.pack("<I", source)
            initial_destination = bytes(rng.getrandbits(8) for _ in range(24 * 0x18 + 16))
            old[destination_offset:destination_offset + len(initial_destination)] = initial_destination
            native[destination_offset:destination_offset + len(initial_destination)] = initial_destination

            regs = State()
            regs.r[:] = [rng.getrandbits(32) for _ in range(16)]
            regs.r[15] = 0x8cf00000
            regs.pc, regs.pr = 0x8c6461ae, 0x8c000100
            expected = State.from_buffer_copy(bytes(regs))
            steps += reference(old, expected)
            fn(view)

            size = 24 * 0x18
            if native[destination_offset:destination_offset + size] != old[destination_offset:destination_offset + size]:
                for offset, (actual, expected_byte) in enumerate(zip(
                        native[destination_offset:destination_offset + size],
                        old[destination_offset:destination_offset + size])):
                    if actual != expected_byte:
                        raise AssertionError(
                            f"destination mismatch alias={alias:#x} source={source:#x} "
                            f"offset={offset:#x} native={actual:#x} retail={expected_byte:#x}; "
                            f"retail PC={expected.pc:#x}, steps={steps}")
            assert native[region:region + len(source_data)] == old[region:region + len(source_data)]
            assert native[source_slot:source_slot + 4] == old[source_slot:source_slot + 4]
            cases += 1

    start, end = 0x8c6461ae, 0x8c6461f0
    report = {
        "cases_passed": cases, "routine_start": hex(start), "routine_end_exclusive": hex(end),
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[start - 0x8c010000:end - 0x8c010000]).hexdigest(),
        "retail_instructions_executed": steps,
        "source": "source/object_table_compact.c",
        "source_sha256": hashlib.sha256((out / "source/object_table_compact.c").read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Full destination-table, source-data and source-global comparison after compaction of all 24 rows, with 48 deterministic inputs under cached/uncached source aliases.",
        "limitations": ["Invalid source pointers and overlap between source and fixed destination tables are not tested."],
    }
    (out / "analysis/object_table_compact_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{cases} object-table compactions matched retail")


if __name__ == "__main__":
    main()
