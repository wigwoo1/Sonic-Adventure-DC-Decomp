"""Compare directory string helpers and record parsing with retail SH-4 execution."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference


def execute(ram, address, args):
    regs = State()
    regs.r[:] = [0x71420000 + i * 0x103 for i in range(16)]
    for index, value in enumerate(args):
        regs.r[4 + index] = value & 0xffffffff
    regs.r[15] = 0x8cf00000
    regs.pc, regs.pr = address, 0x8c000100
    expected = State.from_buffer_copy(bytes(regs))
    steps = reference(ram, expected)
    return expected.r[0], steps


def main():
    out = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    old = bytearray(16 * 1024 * 1024)
    old[0x10000:0x10000 + len(binary)] = binary
    native = bytearray(old)
    view = (ctypes.c_uint8 * len(native)).from_buffer(native)
    dll_path = ROOT / "work/sonic-adventure/build/directory_entries.dll"
    lib = ctypes.CDLL(str(dll_path))
    find = lib.sa_string_set_find_8c64541c
    find.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint32, ctypes.c_uint32]
    find.restype = ctypes.c_int32
    name_hash = lib.sa_ascii_name_hash_8c645442
    name_hash.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint32]
    name_hash.restype = ctypes.c_uint32
    parse = lib.sa_directory_entry_parse_8c645468
    parse.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint32, ctypes.c_uint32]
    parse.restype = ctypes.c_uint8

    rng = random.Random(0x8c64541c)
    region = 0x00a00000
    find_fixtures = [
        (b"\x00", b"\x00"), (b"abc\x00", b"\x00"),
        (b"abc\x00", b"zby\x00"), (b"abc\x00", b"xay\x00"),
        (b"abcabc\x00", b"b\x00"), (b"\x80A\xff\x00", b"\xff\x00"),
    ]
    for length in range(1, 48):
        left = bytes(rng.randrange(1, 256) for _ in range(length)) + b"\x00"
        right = bytes(rng.randrange(1, 256) for _ in range(rng.randrange(0, 32))) + b"\x00"
        find_fixtures.append((left, right))
    find_cases = find_steps = 0
    for alias in (0x8c000000, 0xac000000):
        for left_data, right_data in find_fixtures:
            left, right = alias + region, alias + region + 0x100
            block = bytearray(0x180)
            block[:len(left_data)] = left_data
            block[0x100:0x100 + len(right_data)] = right_data
            old[region:region + len(block)] = block
            native[region:region + len(block)] = block
            expected, steps = execute(old, 0x8c64541c, (left, right))
            find_steps += steps
            actual = find(view, left, right)
            assert (actual & 0xffffffff) == expected, (alias, left_data, right_data, actual, expected)
            find_cases += 1

    hash_fixtures = [b"\x00", b"a\x00", b"A\x00", b"Sonic Adventure\x00",
                     b"\x80\xffA\x00", bytes(range(1, 128)) + b"\x00"]
    for length in range(1, 64):
        hash_fixtures.append(bytes(rng.randrange(1, 256) for _ in range(length)) + b"\x00")
    hash_cases = hash_steps = 0
    for alias in (0x8c000000, 0xac000000):
        for data in hash_fixtures:
            name = alias + region
            old[region:region + len(data)] = data
            native[region:region + len(data)] = data
            expected, steps = execute(old, 0x8c645442, (name,))
            hash_steps += steps
            actual = name_hash(view, name)
            assert actual == expected, (alias, data, actual, expected)
            hash_cases += 1

    parse_cases = parse_steps = 0
    def compare_record(alias, record_bytes, expected_return=None):
        nonlocal parse_cases, parse_steps
        record = alias + region
        output = alias + region + 0x200
        block = bytearray(0x400)
        block[:len(record_bytes)] = record_bytes
        old[region:region + len(block)] = block
        native[region:region + len(block)] = block
        expected, steps = execute(old, 0x8c645468, (record, output))
        parse_steps += steps
        actual = parse(view, record, output)
        assert actual == (expected & 0xff), (alias, record_bytes[:0x28].hex(), actual, hex(expected))
        if expected_return is not None:
            assert actual == expected_return
        assert native[region:region + len(block)] == old[region:region + len(block)], (alias, record_bytes[:0x28].hex())
        parse_cases += 1

    for alias in (0x8c000000, 0xac000000):
        for record_length in range(0, 0x22):
            record = bytearray(rng.getrandbits(8) for _ in range(0x80))
            record[0] = record_length
            compare_record(alias, bytes(record), 0)

        # Normal file records: the stored length includes two bytes before the name.
        for name_length in range(1, 50):
            name = bytes(rng.randrange(1, 256) for _ in range(name_length))
            record = bytearray(rng.getrandbits(8) for _ in range(0x80))
            record[0] = 0x22 + (name_length & 0x3f)
            record[0x19] = rng.choice((0, 1, 4, 0x80))
            record[0x20] = name_length + 2
            record[0x21:0x21 + name_length] = name
            record[0x21 + name_length] = 0
            compare_record(alias, bytes(record))

        # Directory records use bit 1 and the literal names 0/1 for . and ..
        for name in (b"folder", b"MixedCase", b".hidden", b"\x00", b"\x01"):
            record = bytearray(rng.getrandbits(8) for _ in range(0x80))
            record[0] = 0x22
            record[0x19] = 2
            record[0x20] = len(name)
            record[0x21:0x21 + len(name)] = name
            compare_record(alias, bytes(record))

    routines = [
        {"name": "string_set_find", "routine_start": "0x8c64541c", "routine_end_exclusive": "0x8c645442",
         "cases_passed": find_cases, "retail_instructions_executed": find_steps,
         "scope": "NUL-terminated search-set membership, first-match indexing, no-match length, repeated/high-bit bytes, and both RAM aliases."},
        {"name": "ascii_name_hash", "routine_start": "0x8c645442", "routine_end_exclusive": "0x8c645468",
         "cases_passed": hash_cases, "retail_instructions_executed": hash_steps,
         "scope": "Seeded ASCII-case-insensitive byte-sum hash across empty, upper/lower, high-bit and deterministic strings under both aliases."},
        {"name": "directory_entry_parse", "routine_start": "0x8c645468", "routine_end_exclusive": "0x8c6454fe",
         "cases_passed": parse_cases, "retail_instructions_executed": parse_steps,
         "scope": "Rejected record lengths, ordinary file names, directory flags, . and .. special-name normalization, return values and full bounded output memory comparison."},
    ]
    for routine in routines:
        start, end = int(routine["routine_start"], 16), int(routine["routine_end_exclusive"], 16)
        routine["retail_span_sha256"] = hashlib.sha256(binary[start - 0x8c010000:end - 0x8c010000]).hexdigest()
    report = {
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "source": "source/directory_entries.c",
        "source_sha256": hashlib.sha256((out / "source/directory_entries.c").read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "routines": routines,
        "limitations": ["Malformed records with name lengths below two and the flag-clear path can wrap the native copy count and write address; malformed-pointer exception behavior is excluded."],
    }
    (out / "analysis/directory_entries_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{find_cases} set searches, {hash_cases} hashes, and {parse_cases} directory records matched retail")


if __name__ == "__main__":
    main()
