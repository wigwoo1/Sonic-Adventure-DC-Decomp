"""Compare callback registration wrappers for indices 3 and 5 with retail."""
import ctypes
import hashlib
import itertools
import json
import random
import struct

from verify_startup import ROOT, State, reference

P = ctypes.POINTER(ctypes.c_uint32)


def main():
    out = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    dll_path = ROOT / "work/sonic-adventure/build/callback_install.dll"
    dll = ctypes.CDLL(str(dll_path))
    view_ram = bytearray(16 * 1024 * 1024)
    view = (ctypes.c_uint8 * len(view_ram)).from_buffer(view_ram)
    sr = ctypes.c_uint32()
    cases = []
    for name, start, end, index in (
        ("sa_callback_register_index3_8c6415f2", 0x8C6415F2, 0x8C64160E, 3),
        ("sa_callback_register_index5_8c64162a", 0x8C64162A, 0x8C641646, 5),
        ("sa_callback_register_index8_8c64167e", 0x8C64167E, 0x8C64169A, 8),
    ):
        fn = getattr(dll, name)
        fn.argtypes = [ctypes.POINTER(ctypes.c_uint8), P, ctypes.c_uint32,
                       ctypes.c_uint32, ctypes.c_void_p, ctypes.c_void_p]
        fn.restype = ctypes.c_int
        routine_cases = 0
        steps = 0
        for driver, callback, argument, initial_sr in itertools.product(
                (0x8CA00000, 0xACA00000),
                (0x8C645B1E, 0xFFFFFFFF),
                (0, 1, 0x12345678),
                (0, 1, 0x60, 0xF0, 0x700083F3)):
            old = bytearray(16 * 1024 * 1024)
            old[0x10000:0x10000 + len(binary)] = binary
            view_ram[:] = old
            driver_base = (driver & 0x1FFFFFFF) - 0x0C000000
            struct.pack_into("<I", old, 0x67C004, driver)
            struct.pack_into("<I", view_ram, 0x67C004, driver)
            regs = State()
            rng = random.Random(driver ^ callback ^ argument ^ initial_sr ^ index)
            regs.r[:] = [rng.getrandbits(32) for _ in range(16)]
            regs.r[4], regs.r[5] = callback, argument
            regs.r[15] = 0x8CF00000
            regs.pc, regs.pr, regs.sr = start, 0x8C000100, initial_sr
            expected = State.from_buffer_copy(bytes(regs))
            steps += reference(old, expected)
            sr.value = initial_sr
            passed_argument = 0 if index == 8 else argument
            assert fn(view, ctypes.byref(sr), callback, passed_argument, None, None) == 0
            slot = driver_base + 0x230 + index * 8
            assert view_ram[slot - 8:slot + 16] == old[slot - 8:slot + 16], (
                hex(start), hex(driver), hex(callback), hex(argument), hex(slot),
                view_ram[slot - 8:slot + 16].hex(), old[slot - 8:slot + 16].hex())
            assert sr.value == expected.sr, (hex(start), hex(sr.value), hex(expected.sr))
            routine_cases += 1
        span = binary[start - 0x8C010000:end - 0x8C010000]
        cases.append({
            "name": name, "routine_start": hex(start), "routine_end_exclusive": hex(end),
            "index": index, "cases_passed": routine_cases,
            "retail_span_sha256": hashlib.sha256(span).hexdigest(),
            "retail_instructions_executed": steps,
        })
    report = {
        "routines": cases,
        "cases_passed": sum(item["cases_passed"] for item in cases),
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "source": "source/callback_install.c",
        "source_sha256": hashlib.sha256((out / "source/callback_install.c").read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Retail instruction execution compared to native wrappers for nonzero callback registration, three argument values, two aliased driver bases, SR masks and adjacent table guards.",
        "limitations": ["Zero-callback removal service behavior is verified separately by the primary wrapper test.",
                       "CPU exceptions and invalid unmapped driver pointers are not modeled."],
    }
    (out / "analysis/callback_register_index_wrappers_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{report['cases_passed']} callback index wrapper comparisons passed")


if __name__ == "__main__":
    main()
