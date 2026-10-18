"""Differentially test bounded and NUL-terminated string helpers against retail."""
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
    dll_path = ROOT / "work/sonic-adventure/build/string_helpers.dll"
    lib = ctypes.CDLL(str(dll_path))
    bounded = lib.sa_bounded_bytes_equal_8c603bfc
    bounded.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint32,
                        ctypes.c_uint32, ctypes.c_int32]
    bounded.restype = ctypes.c_uint32
    strings = lib.sa_strings_equal_8c6453cc
    strings.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint32, ctypes.c_uint32]
    strings.restype = ctypes.c_uint32
    table_get = lib.sa_string_table_pointer_get_8c645328
    table_get.argtypes = [ctypes.POINTER(ctypes.c_uint8)]
    table_get.restype = ctypes.c_uint32
    upper = lib.sa_ascii_upper_8c645348
    upper.argtypes = [ctypes.c_uint32]
    upper.restype = ctypes.c_uint32
    copy_fn = lib.sa_forward_copy_positive_8c64535c
    copy_fn.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint32,
                        ctypes.c_uint32, ctypes.c_int32]
    copy_fn.restype = None
    bounded_ci = lib.sa_case_insensitive_bounded_equal_8c645374
    bounded_ci.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint32,
                           ctypes.c_uint32, ctypes.c_int32]
    bounded_ci.restype = ctypes.c_uint32

    rng = random.Random(0x8c603bfc)
    base_offset = 0x00a00000
    bounded_fixtures = []
    bounded_fixtures.extend((None, None, length) for length in (-0x80000000, -7, -1, 0))
    for length in range(1, 65):
        data = bytes(rng.getrandbits(8) for _ in range(length))
        bounded_fixtures.append((data, data, length))
        for position in range(length):
            changed = bytearray(data)
            changed[position] ^= 1
            bounded_fixtures.append((data, bytes(changed), length))

    bounded_cases = bounded_steps = 0
    for alias in (0x8c000000, 0xac000000):
        for left_data, right_data, length in bounded_fixtures:
            if length <= 0:
                left = 0xdead0000
                right = 0xcafe0000
                old_region = native_region = b""
            else:
                left = alias + base_offset
                right = alias + base_offset + 0x100
                old_region = bytes(0x180)
                if left_data is not None:
                    temp = bytearray(old_region)
                    temp[:length] = left_data
                    temp[0x100:0x100 + length] = right_data
                    old_region = bytes(temp)
                native_region = old_region
                old[base_offset:base_offset + len(old_region)] = old_region
                native[base_offset:base_offset + len(native_region)] = native_region
            regs = State()
            regs.r[:] = [rng.getrandbits(32) for _ in range(16)]
            regs.r[4], regs.r[5], regs.r[6], regs.r[15] = left, right, length & 0xffffffff, 0x8cf00000
            regs.pc, regs.pr = 0x8c603bfc, 0x8c000100
            expected = State.from_buffer_copy(bytes(regs))
            bounded_steps += reference(old, expected)
            actual = bounded(view, left, right, length)
            assert actual == expected.r[0], (alias, length, left_data, right_data, actual, expected.r[0])
            if length > 0:
                assert native[base_offset:base_offset + len(native_region)] == old[base_offset:base_offset + len(old_region)]
            bounded_cases += 1

    string_fixtures = [b"\x00", b"a\x00", b"\x80\xff\x00", b"sonic\x00", bytes(range(1, 64)) + b"\x00"]
    for data in (b"a", b"abc", b"a\x80\x00", bytes(rng.randrange(1, 256) for _ in range(24))):
        string_fixtures.extend((data + b"\x00", data + b"\x00"))
        for position in range(len(data)):
            different = bytearray(data + b"\x00")
            different[position] ^= 0x80
            string_fixtures.append((data + b"\x00", bytes(different)))
        string_fixtures.append((data + b"\x00", data[:max(0, len(data) - 1)] + b"\x00"))
    string_fixtures = [item if isinstance(item, tuple) else (item, item) for item in string_fixtures]
    string_cases = string_steps = 0
    for alias in (0x8c000000, 0xac000000):
        for left_data, right_data in string_fixtures:
            left = alias + base_offset
            right = alias + base_offset + 0x100
            size = max(len(left_data), len(right_data))
            region = bytearray(0x100 + size)
            region[:len(left_data)] = left_data
            region[0x100:0x100 + len(right_data)] = right_data
            old[base_offset:base_offset + len(region)] = region
            native[base_offset:base_offset + len(region)] = region
            regs = State()
            regs.r[:] = [rng.getrandbits(32) for _ in range(16)]
            regs.r[4], regs.r[5], regs.r[15] = left, right, 0x8cf00000
            regs.pc, regs.pr = 0x8c6453cc, 0x8c000100
            expected = State.from_buffer_copy(bytes(regs))
            string_steps += reference(old, expected)
            actual = strings(view, left, right)
            assert actual == expected.r[0], (alias, left_data, right_data, actual, expected.r[0])
            assert native[base_offset:base_offset + len(region)] == old[base_offset:base_offset + len(region)]
            string_cases += 1

    getter_regs = State()
    getter_regs.r[:] = [rng.getrandbits(32) for _ in range(16)]
    getter_regs.r[15] = 0x8cf00000
    getter_regs.pc, getter_regs.pr = 0x8c645328, 0x8c000100
    getter_expected = State.from_buffer_copy(bytes(getter_regs))
    getter_steps = reference(old, getter_expected)
    getter_actual = table_get(view)
    assert getter_actual == getter_expected.r[0]

    upper_values = list(range(256)) + [0xffffffff, 0x101, 0x161, 0x17a, 0x17b] + [rng.getrandbits(32) for _ in range(128)]
    upper_cases = upper_steps = 0
    for value in upper_values:
        regs = State()
        regs.r[:] = [rng.getrandbits(32) for _ in range(16)]
        regs.r[4], regs.r[15] = value, 0x8cf00000
        regs.pc, regs.pr = 0x8c645348, 0x8c000100
        expected = State.from_buffer_copy(bytes(regs))
        upper_steps += reference(old, expected)
        actual = upper(value)
        assert actual == expected.r[0], (hex(value), hex(actual), hex(expected.r[0]))
        upper_cases += 1

    copy_cases = copy_steps = 0
    for alias in (0x8c000000, 0xac000000):
        for length in (-2, -1, 0, 1, 2, 7, 8, 31, 64, 127, 256):
            for destination_offset, source_offset in ((0, 0x100), (0x100, 0), (0, 1), (1, 0)):
                initial = bytes(rng.getrandbits(8) for _ in range(0x400))
                old[base_offset:base_offset + len(initial)] = initial
                native[base_offset:base_offset + len(initial)] = initial
                destination = alias + base_offset + destination_offset
                source = alias + base_offset + source_offset
                regs = State()
                regs.r[:] = [rng.getrandbits(32) for _ in range(16)]
                regs.r[4], regs.r[5], regs.r[6], regs.r[15] = source, destination, length & 0xffffffff, 0x8cf00000
                regs.pc, regs.pr = 0x8c64535c, 0x8c000100
                expected = State.from_buffer_copy(bytes(regs))
                copy_steps += reference(old, expected)
                copy_fn(view, source, destination, length)
                assert native[base_offset:base_offset + len(initial)] == old[base_offset:base_offset + len(initial)], (alias, length, destination_offset, source_offset)
                copy_cases += 1

    ci_fixtures = []
    for length in range(-2, 19):
        ci_fixtures.extend([
            (b"AbC\x00", b"aBc\x00", length),
            (b"abcdefghij\x00", b"abcdefghij\x00", length),
            (b"ab\x00", b"abX\x00", length),
            (b"\x80Z\x00", b"\x80z\x00", length),
        ])
        if 0 <= length <= 10:
            for pos in range(length + 1):
                left = bytearray(b"abcdefghij\x00")
                right = bytearray(left)
                right[pos] = ord("X") if right[pos] != ord("X") else ord("Y")
                ci_fixtures.append((bytes(left), bytes(right), length))
    ci_cases = ci_steps = 0
    for alias in (0x8c000000, 0xac000000):
        for left_data, right_data, length in ci_fixtures:
            left = alias + base_offset
            right = alias + base_offset + 0x100
            initial = bytearray(0x180)
            initial[:len(left_data)] = left_data
            initial[0x100:0x100 + len(right_data)] = right_data
            old[base_offset:base_offset + len(initial)] = initial
            native[base_offset:base_offset + len(initial)] = initial
            regs = State()
            regs.r[:] = [rng.getrandbits(32) for _ in range(16)]
            regs.r[4], regs.r[5], regs.r[6], regs.r[15] = left, right, length & 0xffffffff, 0x8cf00000
            regs.pc, regs.pr = 0x8c645374, 0x8c000100
            expected = State.from_buffer_copy(bytes(regs))
            ci_steps += reference(old, expected)
            actual = bounded_ci(view, left, right, length)
            assert actual == expected.r[0], (alias, left_data, right_data, length, actual, expected.r[0])
            ci_cases += 1

    routines = [
        {"name": "bounded_bytes_equal", "routine_start": "0x8c603bfc", "routine_end_exclusive": "0x8c603c14",
         "cases_passed": bounded_cases, "retail_instructions_executed": bounded_steps,
         "scope": "Bounded comparison across nonpositive lengths including INT_MIN, every first-mismatch offset for lengths 1-64, equal data, and cached/uncached aliases."},
        {"name": "strings_equal", "routine_start": "0x8c6453cc", "routine_end_exclusive": "0x8c6453ea",
         "cases_passed": string_cases, "retail_instructions_executed": string_steps,
         "scope": "NUL-terminated equality for empty, equal, prefix, mismatch-at-each-position, and high-bit-byte inputs under cached/uncached aliases."},
        {"name": "string_table_pointer_get", "routine_start": "0x8c645328", "routine_end_exclusive": "0x8c64532e",
         "cases_passed": 1, "retail_instructions_executed": getter_steps,
         "scope": "Return value compared with the word loaded from the retail data reference."},
        {"name": "ascii_upper", "routine_start": "0x8c645348", "routine_end_exclusive": "0x8c64535c",
         "cases_passed": upper_cases, "retail_instructions_executed": upper_steps,
         "scope": "All low-byte values, extended integers and deterministic 32-bit values compared with retail ASCII lowercase conversion."},
        {"name": "forward_copy_positive", "routine_start": "0x8c64535c", "routine_end_exclusive": "0x8c645374",
         "cases_passed": copy_cases, "retail_instructions_executed": copy_steps,
         "scope": "Positive and nonpositive lengths, four overlap directions, cached/uncached aliases, and full bounded-memory comparison."},
        {"name": "case_insensitive_bounded_equal", "routine_start": "0x8c645374", "routine_end_exclusive": "0x8c6453c0",
         "cases_passed": ci_cases, "retail_instructions_executed": ci_steps,
         "scope": "Bounded equality with ASCII case changes, length cutoffs, mismatches, prefixes, high-bit bytes, and both RAM aliases."},
    ]
    for routine in routines:
        start, end = int(routine["routine_start"], 16), int(routine["routine_end_exclusive"], 16)
        routine["retail_span_sha256"] = hashlib.sha256(binary[start - 0x8c010000:end - 0x8c010000]).hexdigest()
    report = {
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "source": "source/string_helpers.c",
        "source_sha256": hashlib.sha256((out / "source/string_helpers.c").read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "routines": routines,
        "limitations": ["Unmapped positive-length strings, SH-4 address faults, and unterminated strings are outside the bounded test fixtures."],
    }
    (out / "analysis/string_helpers_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{bounded_cases} bounded comparisons, {string_cases} string comparisons, {upper_cases} case conversions, {copy_cases} copies, and {ci_cases} case-insensitive comparisons matched retail")


if __name__ == "__main__":
    main()
