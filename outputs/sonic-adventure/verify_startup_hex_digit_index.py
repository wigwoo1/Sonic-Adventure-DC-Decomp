"""Exhaustively compare the startup hex-digit lookup against retail code."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

ENTRY = 0x8c011042
END = 0x8c011060
STRING = 0x8c19acb8
POINTER_SLOT = 0x8c0110bc
ALTERNATE_TABLE = 0x8c700000
RAM_SIZE = 16 * 1024 * 1024


def main():
    out = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    library_path = ROOT / "work/sonic-adventure/build/startup_hex_digit_index.dll"
    function = ctypes.CDLL(str(library_path)).sa_startup_hex_digit_index_8c011042
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = ctypes.c_int32

    original = bytearray(b"\xa5") * RAM_SIZE
    original[0x10000:0x10000 + len(binary)] = binary
    assert original[STRING & 0xffffff:(STRING & 0xffffff) + 17] == b"0123456789ABCDEF\0"
    alternate = bytearray(original)
    alternate_bytes = bytes(range(0x80, 0x90))
    alternate[ALTERNATE_TABLE & 0xffffff:(ALTERNATE_TABLE & 0xffffff) + 16] = alternate_bytes
    import struct
    struct.pack_into("<I", alternate, POINTER_SLOT & 0xffffff, ALTERNATE_TABLE)
    rng = random.Random(ENTRY)
    cases = []

    for table_name, fixture, table_bytes in (("retail", original, b"0123456789ABCDEF"),
                                              ("alternate", alternate, alternate_bytes)):
      for char in range(256):
        for representation, argument in (("zero_extended", char),
                                         ("sign_extended", char if char < 128 else char - 256)):
            expected = State()
            expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
            expected.r[4] = argument & 0xffffffff
            expected.sr = rng.getrandbits(32)
            expected.pr = 0x8c000100 + len(cases) * 4
            expected.pc = ENTRY
            native_state = State.from_buffer_copy(bytes(expected))
            retail_ram, actual_ram = bytearray(fixture), bytearray(fixture)
            steps = reference(retail_ram, expected)
            view = (ctypes.c_uint8 * len(actual_ram)).from_buffer(actual_ram)
            result = function(view, ctypes.byref(native_state))

            assert bytes(native_state) == bytes(expected), (
                f"state mismatch table={table_name} char={char:#x} representation={representation}: "
                f"native={[hex(x) for x in native_state.r]}, "
                f"retail={[hex(x) for x in expected.r]}"
            )
            assert actual_ram == retail_ram, (
                f"RAM mismatch table={table_name} char={char:#x} representation={representation}"
            )
            assert result == ctypes.c_int32(expected.r[0]).value
            signed_char = char if char < 128 else char - 256
            expected_index = next((i for i, item in enumerate(table_bytes)
                                   if (item if item < 128 else item - 256) == signed_char), -1)
            assert result == expected_index, (
                f"lookup result mismatch table={table_name} char={char:#x}: "
                f"got={result} expected={expected_index}"
            )
            expected_steps = 12 + 7 * expected_index if expected_index >= 0 else 118
            assert steps == expected_steps, (table_name, char, steps, expected_steps)
            cases.append({"table": table_name, "input_byte": hex(char), "representation": representation,
                          "result": result, "retail_instructions_executed": steps,
                          "full_cpu_state_and_ram_match": True})
    source_path = out / "source/startup_hex_digit_index.c"
    report = {
        "cases_passed": len(cases), "routine_start": hex(ENTRY),
        "routine_end_exclusive": hex(END),
        "retail_span_sha256": hashlib.sha256(
            binary[ENTRY - 0x8c010000:END - 0x8c010000]).hexdigest(),
        "source_sha256": hashlib.sha256(source_path.read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(library_path.read_bytes()).hexdigest(),
        "cases": cases,
        "scope": "All 256 input bytes tested as both zero-extended and sign-extended values for both the retail table and an alternate table redirected through the routine's literal pointer (1,024 total). Full 16 MiB RAM and GPR/SR/PR/PC comparisons against retail confirm pointer loading, lookup results, not-found behavior, and no RAM writes.",
        "limitations": [
            "The routine is verified as a standalone lookup; caller-level parsing and full game behavior remain separate.",
            "The reference is a partial SH-4 interpreter, not physical Dreamcast hardware."
        ]
    }
    (out / "analysis/startup_hex_digit_index_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} exhaustive startup hex-digit lookup comparisons passed")


if __name__ == "__main__":
    main()
