"""Compare object accessor C implementations with their retail SH-4 bodies."""
import ctypes
import hashlib
import itertools
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
    dll_path = ROOT / "work/sonic-adventure/build/object_accessors.dll"
    dll = ctypes.CDLL(str(dll_path))
    getter = dll.sa_object_byte_at_1_get_8c644500
    getter.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint32]
    getter.restype = ctypes.c_int32
    setter = dll.sa_object_field38_set_8c644602
    setter.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint32, ctypes.c_uint32]
    setter.restype = None
    cases = {"sa_object_byte_at_1_get_8c644500": 0,
             "sa_object_field38_set_8c644602": 0}
    steps = 0
    for alias, object_offset in itertools.product((0x8C000000, 0xAC000000),
                                                   (0x00A00000, 0x00B00000)):
        object_address = alias + object_offset
        for byte_value in range(256):
            old[object_offset:object_offset + 0x80] = bytes([0xA5]) * 0x80
            old[object_offset + 1] = byte_value
            native[object_offset:object_offset + 0x80] = old[object_offset:object_offset + 0x80]
            regs = State()
            regs.r[:] = [0xABCD0000 + i for i in range(16)]
            regs.r[4], regs.r[15] = object_address, 0x8CF00000
            regs.pc, regs.pr = 0x8C644500, 0x8C000100
            expected = State.from_buffer_copy(bytes(regs))
            steps += reference(old, expected)
            actual = getter(view, object_address)
            assert actual == ctypes.c_int32(expected.r[0]).value
            assert native == old
            cases["sa_object_byte_at_1_get_8c644500"] += 1

        for value in (0, 1, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF,
                      0x12345678, 0xA5A5A5A5):
            old[object_offset:object_offset + 0x80] = bytes([0xA5]) * 0x80
            native[object_offset:object_offset + 0x80] = old[object_offset:object_offset + 0x80]
            regs = State()
            regs.r[:] = [0x56780000 + i for i in range(16)]
            regs.r[4], regs.r[5], regs.r[15] = object_address, value, 0x8CF00000
            regs.pc, regs.pr = 0x8C644602, 0x8C000100
            expected = State.from_buffer_copy(bytes(regs))
            steps += reference(old, expected)
            setter(view, object_address, value)
            assert native == old, (hex(object_address), hex(value))
            cases["sa_object_field38_set_8c644602"] += 1

    entries = (("sa_object_byte_at_1_get_8c644500", 0x8C644500, 0x8C644504),
               ("sa_object_field38_set_8c644602", 0x8C644602, 0x8C644606))
    routines = []
    for name, start, end in entries:
        routines.append({"name": name, "routine_start": hex(start),
                         "routine_end_exclusive": hex(end), "cases_passed": cases[name],
                         "retail_span_sha256": hashlib.sha256(
                             binary[start - 0x8C010000:end - 0x8C010000]).hexdigest()})
    report = {
        "routines": routines, "cases_passed": sum(cases.values()),
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_instructions_executed": steps,
        "source": "source/object_accessors.c",
        "source_sha256": hashlib.sha256((out / "source/object_accessors.c").read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Full 16 MiB RAM comparison for getter and setter, including all signed-byte values, two address aliases, two RAM object regions, seven word values and untouched-memory guards.",
        "limitations": ["Invalid object pointers and SH-4 exceptions are outside the test scope."],
    }
    (out / "analysis/object_accessors_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{report['cases_passed']} object accessor comparisons passed")


if __name__ == "__main__":
    main()
