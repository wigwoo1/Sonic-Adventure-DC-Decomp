"""Differentially compare directory-name lookup with retail SH-4 execution."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference


def retail_call(ram, address, args):
    state = State()
    state.r[:] = [0x62900000 + i * 0x107 for i in range(16)]
    for index, value in enumerate(args):
        state.r[4 + index] = value & 0xffffffff
    state.r[15] = 0x8cf00000
    state.pc, state.pr = address, 0x8c000100
    result = State.from_buffer_copy(bytes(state))
    steps = reference(ram, result)
    return result.r[0], steps


def main():
    out = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    old = bytearray(16 * 1024 * 1024)
    old[0x10000:0x10000 + len(binary)] = binary
    native = bytearray(old)
    view = (ctypes.c_uint8 * len(native)).from_buffer(native)
    dll_path = ROOT / "work/sonic-adventure/build/directory_lookup.dll"
    fn = ctypes.CDLL(str(dll_path)).sa_directory_name_find_8c6455ce
    fn.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint32,
                   ctypes.c_uint32, ctypes.c_int32]
    fn.restype = ctypes.c_int32

    region = 0x00a00000
    alias_values = (0x8c000000, 0xac000000)
    fixtures = [
        ([], b"Sonic", 1),
        ([b"Sonic", b"Tails", b"Knuckles"], b"Tails", 1),
        ([b"Sonic", b"Tails"], b"tails", 1),
        ([b"Sonic", b"Tails"], b"tails", 0),
        ([b"ab", b"ba", b"cd"], b"ab", 1),
        ([b"ab", b"ba", b"cd"], b"AB", 0),
        ([b"Folder", b"folder", b"FOLDER"], b"fOlDeR", 2),
        ([b"Folder", b"folder"], b"missing", 0),
        ([b"one", b"two", b"three", b"four", b"five"], b"five", 1),
    ]
    rng = random.Random(0x8c6455ce)
    words = [b"alpha", b"BETA", b"Gamma", b"delta", b"epsilon", b"zeta", b"eta", b"theta"]
    for _ in range(64):
        names = [rng.choice(words) for _ in range(rng.randint(0, 8))]
        target = rng.choice(words + [b"not-present"])
        fixtures.append((names, target, rng.choice((0, 1, 2, -1))) )

    cases = steps = 0
    for alias in alias_values:
        for names, target, mode in fixtures:
            table = alias + region
            name = alias + region + 0x800
            block = bytearray(0x1200)
            block[0:4] = len(names).to_bytes(4, "little")
            for index, entry_name in enumerate(names):
                entry = 16 + index * 44
                entry_name = entry_name + b"\x00"
                name_addr = alias + region + entry + 10
                block[entry + 10:entry + 10 + len(entry_name)] = entry_name
                # Obtain the entry hash from retail execution, independent of lookup.
                old[region:region + len(block)] = block
                hash_value, hash_steps = retail_call(old, 0x8c645442, (name_addr,))
                steps += hash_steps
                block[entry + 9] = hash_value & 0xff
            target_bytes = target + b"\x00"
            block[0x800:0x800 + len(target_bytes)] = target_bytes
            old[region:region + len(block)] = block
            native[region:region + len(block)] = block
            expected, run_steps = retail_call(old, 0x8c6455ce, (name, table, mode))
            steps += run_steps
            actual = fn(view, name, table, mode)
            assert actual == (expected if expected < 0x80000000 else expected - 0x100000000), (
                alias, names, target, mode, actual, expected)
            assert native[region:region + len(block)] == old[region:region + len(block)]
            cases += 1

    start, end = 0x8c6455ce, 0x8c64563e
    report = {
        "cases_passed": cases, "routine_start": hex(start), "routine_end_exclusive": hex(end),
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[start - 0x8c010000:end - 0x8c010000]).hexdigest(),
        "retail_instructions_executed": steps,
        "source": "source/directory_lookup.c",
        "source_sha256": hashlib.sha256((out / "source/directory_lookup.c").read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "1-based directory lookup results and read-only table memory compared for exact/case-insensitive modes, hash collisions, empty tables, misses, repeated names and cached/uncached RAM aliases.",
        "limitations": ["Malformed entry counts or unterminated names are outside the bounded fixtures."],
    }
    (out / "analysis/directory_lookup_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{cases} directory lookup cases matched retail")


if __name__ == "__main__":
    main()
