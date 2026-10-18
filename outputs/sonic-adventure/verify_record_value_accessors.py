"""Compare signed record getters and pointer advance with retail instructions."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference


def run_call(ram, start, regs):
    state = State.from_buffer_copy(bytes(regs))
    steps = reference(ram, state)
    return state, steps


def main():
    out = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    old = bytearray(16 * 1024 * 1024)
    old[0x10000:0x10000 + len(binary)] = binary
    native = bytearray(old)
    view = (ctypes.c_uint8 * len(native)).from_buffer(native)
    dll_path = ROOT / "work/sonic-adventure/build/record_value_accessors.dll"
    dll = ctypes.CDLL(str(dll_path))
    byte_get = dll.sa_record_signed_byte_get_8c65c1c6
    byte_get.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint32]
    byte_get.restype = ctypes.c_int32
    half_get = dll.sa_record_signed_halfword_get_8c667702
    half_get.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint32]
    half_get.restype = ctypes.c_int32
    next_record = dll.sa_record_next_record_8c65bce4
    next_record.argtypes = [ctypes.c_uint32]
    next_record.restype = ctypes.c_uint32
    counts = {"sa_record_signed_byte_get_8c65c1c6": 0,
              "sa_record_signed_halfword_get_8c667702": 0,
              "sa_record_next_record_8c65bce4": 0}
    steps = 0
    for alias in (0x8C000000, 0xAC000000):
        for region in (0x00A00000, 0x00B00000):
            object_address = alias + region
            for value in range(256):
                old[region:region + 0x40] = bytes([0xA5]) * 0x40
                native[region:region + 0x40] = old[region:region + 0x40]
                old[region + 1] = value
                native[region + 1] = value
                regs = State()
                regs.r[4], regs.r[15] = object_address, 0x8CF00000
                regs.pc, regs.pr = 0x8C65C1C6, 0x8C000100
                expected, ran = run_call(old, 0x8C65C1C6, regs)
                steps += ran
                assert byte_get(view, object_address) == ctypes.c_int32(expected.r[0]).value
                assert native[region:region + 0x40] == old[region:region + 0x40]
                counts["sa_record_signed_byte_get_8c65c1c6"] += 1

            halfwords = [0, 1, 0x7FFF, 0x8000, 0x8001, 0xFFFF]
            rng = random.Random(object_address ^ 0x8C667702)
            halfwords.extend(rng.getrandbits(16) for _ in range(512))
            for value in halfwords:
                old[region:region + 0x40] = bytes([0x5A]) * 0x40
                native[region:region + 0x40] = old[region:region + 0x40]
                struct.pack_into("<H", old, region + 0x24, value)
                struct.pack_into("<H", native, region + 0x24, value)
                regs = State()
                regs.r[4], regs.r[15] = object_address, 0x8CF00000
                regs.pc, regs.pr = 0x8C667702, 0x8C000100
                expected, ran = run_call(old, 0x8C667702, regs)
                steps += ran
                assert half_get(view, object_address) == ctypes.c_int32(expected.r[0]).value
                assert native[region:region + 0x40] == old[region:region + 0x40]
                counts["sa_record_signed_halfword_get_8c667702"] += 1

    addresses = [0, 1, 0xFFFFFFAF, 0xFFFFFFB0, 0xFFFFFFF0,
                 0x7FFFFFAF, 0x7FFFFFFF, 0x8C000000, 0xFFFFFFFF]
    rng = random.Random(0x8C65BCE4)
    addresses.extend(rng.getrandbits(32) for _ in range(512))
    for address in addresses:
        regs = State()
        regs.r[4], regs.r[15] = address, 0x8CF00000
        regs.pc, regs.pr = 0x8C65BCE4, 0x8C000100
        expected, ran = run_call(old, 0x8C65BCE4, regs)
        steps += ran
        assert next_record(address) == expected.r[0]
        counts["sa_record_next_record_8c65bce4"] += 1

    entries = (
        ("sa_record_signed_byte_get_8c65c1c6", 0x8C65C1C6, 0x8C65C1CA),
        ("sa_record_signed_halfword_get_8c667702", 0x8C667702, 0x8C667708),
        ("sa_record_next_record_8c65bce4", 0x8C65BCE4, 0x8C65BCEA),
    )
    reports = [{"name": name, "routine_start": hex(start),
                "routine_end_exclusive": hex(end), "cases_passed": counts[name],
                "retail_span_sha256": hashlib.sha256(
                    binary[start - 0x8C010000:end - 0x8C010000]).hexdigest()}
               for name, start, end in entries]
    report = {
        "routines": reports, "cases_passed": sum(counts.values()),
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_instructions_executed": steps,
        "source": "source/record_value_accessors.c",
        "source_sha256": hashlib.sha256((out / "source/record_value_accessors.c").read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Retail comparison for all signed-byte values, signed-halfword boundary cases and 512 deterministic values per RAM alias, plus pointer-advance boundary and 512 random inputs.",
        "limitations": ["Invalid object pointers and SH-4 exceptions are outside the getter test scope.",
                        "Tests sample the 16-bit getter domain except for specified signed boundaries."],
    }
    (out / "analysis/record_value_accessors_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{report['cases_passed']} record value accessor comparisons passed")


if __name__ == "__main__":
    main()
