"""Compare fixed-offset record setters against retail SH-4 routines."""
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
    dll_path = ROOT / "work/sonic-adventure/build/record_accessors.dll"
    dll = ctypes.CDLL(str(dll_path))
    routines = [
        ("sa_record_field1c_set_8c65086c", 0x8C65086C, 0x8C650870, 1, 0x1C, False),
        ("sa_record_field8_set_8c65a5ac", 0x8C65A5AC, 0x8C65A5B2, 1, 8, True),
        ("sa_record_field4_set_8c65a5cc", 0x8C65A5CC, 0x8C65A5D2, 1, 4, True),
        ("sa_record_fieldc_set_8c65a5d4", 0x8C65A5D4, 0x8C65A5DA, 1, 0xC, True),
        ("sa_record_fields38_3c_set_8c65bfbc", 0x8C65BFBC, 0x8C65BFC2, 2, 0x38, False),
    ]
    cases = []
    steps = 0
    for name, start, end, value_count, offset, returns_zero in routines:
        fn = getattr(dll, name)
        argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint32] + [ctypes.c_uint32] * value_count
        fn.argtypes = argtypes
        fn.restype = ctypes.c_uint32 if returns_zero else None
        count = 0
        for alias, object_offset in itertools.product((0x8C000000, 0xAC000000),
                                                       (0x00A00000, 0x00B00000)):
            object_address = alias + object_offset
            for seed in range(12):
                region = bytes((seed + i * 17) & 0xFF for i in range(0x80))
                old[object_offset:object_offset + len(region)] = region
                native[object_offset:object_offset + len(region)] = region
                rng = random.Random(start ^ object_address ^ seed)
                values = [rng.getrandbits(32) for _ in range(value_count)]
                regs = State()
                regs.r[:] = [rng.getrandbits(32) for _ in range(16)]
                regs.r[4], regs.r[5] = object_address, values[0]
                if value_count == 2:
                    regs.r[6] = values[1]
                regs.r[15], regs.pc, regs.pr = 0x8CF00000, start, 0x8C000100
                expected = State.from_buffer_copy(bytes(regs))
                steps += reference(old, expected)
                actual = fn(view, object_address, *values)
                if returns_zero:
                    assert actual == expected.r[0] == 0
                assert native == old, (name, hex(object_address), [hex(x) for x in values])
                count += 1
        cases.append({"name": name, "routine_start": hex(start),
                      "routine_end_exclusive": hex(end), "field_offset": hex(offset),
                      "cases_passed": count,
                      "retail_span_sha256": hashlib.sha256(
                          binary[start - 0x8C010000:end - 0x8C010000]).hexdigest()})
    report = {
        "routines": cases, "cases_passed": sum(item["cases_passed"] for item in cases),
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_instructions_executed": steps,
        "source": "source/record_accessors.c",
        "source_sha256": hashlib.sha256((out / "source/record_accessors.c").read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Full 16 MiB RAM checks for five record setters, two RAM aliases, two object regions, twelve deterministic field-value sets per routine and unchanged-memory guards; zero-return routines also compare R0.",
        "limitations": ["Invalid object pointers and SH-4 exception behavior are outside the test scope."],
    }
    (out / "analysis/record_accessors_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{report['cases_passed']} record accessor comparisons passed")


if __name__ == "__main__":
    main()
