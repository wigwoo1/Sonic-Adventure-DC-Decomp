"""Compare native callback-configuration behavior with the retail SH-4 routine."""
import ctypes
import hashlib
import itertools
import json
import struct

from verify_startup import ROOT, State, reference


def main():
    out = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    old = bytearray(16 * 1024 * 1024)
    old[0x10000:0x10000 + len(binary)] = binary
    native = bytearray(old)
    view = (ctypes.c_uint8 * len(native)).from_buffer(native)
    dll_path = ROOT / "work/sonic-adventure/build/callback_configuration.dll"
    dll = ctypes.CDLL(str(dll_path))
    fn = dll.sa_callback_configuration_8c6418e8
    fn.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint32,
                   ctypes.c_uint32, ctypes.c_uint32]
    fn.restype = ctypes.c_uint32
    cases = 0
    steps = 0
    mmio_address = 0xA05F689C
    modes = (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xFFFFFFFF)
    for system_mode, enabled, initial_flag in itertools.product(
            modes, (0, 1, 0xFFFFFFFF), (0, 0x10, 0xA5A50000, 0xA5A50010)):
        old[:] = bytes(16 * 1024 * 1024)
        old[0x10000:0x10000 + len(binary)] = binary
        struct.pack_into("<I", old, 0x8A2D5C, initial_flag)
        native[:] = old
        regs = State()
        regs.r[:] = [0x12340000 + i for i in range(16)]
        callback = 0x8C645B1E ^ (system_mode & 0xFFFF)
        regs.r[4], regs.r[5] = callback, enabled
        regs.r[15], regs.pc, regs.pr = 0x8CF00000, 0x8C6418E8, 0x8C000100
        regs.sr = (system_mode ^ enabled ^ initial_flag) & 0xFFFFFFFF
        mmio = {mmio_address: system_mode}
        expected = State.from_buffer_copy(bytes(regs))
        steps += reference(old, expected, mmio=mmio)
        assert fn(view, system_mode, callback, enabled) == expected.r[0] == 0
        assert native == old, (hex(system_mode), hex(enabled), hex(initial_flag))
        assert expected.pr == 0x8C000100
        cases += 1
    start, end = 0x8C6418E8, 0x8C641946
    report = {
        "cases_passed": cases,
        "routine_start": hex(start), "routine_end_exclusive": hex(end),
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[start - 0x8C010000:end - 0x8C010000]).hexdigest(),
        "source": "source/callback_configuration.c",
        "source_sha256": hashlib.sha256((out / "source/callback_configuration.c").read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Whole 16 MiB RAM, return value and PR control flow compared with retail for eleven system-mode register values, three enabled values, and four initial control-flag patterns. MMIO register 0xa05f689c is provided explicitly to the retail emulator and native wrapper.",
        "retail_instructions_executed": steps,
        "limitations": ["MMIO value is supplied by the test harness rather than read from physical Dreamcast hardware.",
                        "SH-4 SR/T-bit equivalence is not checked by this wrapper test.",
                        "Exception behavior for invalid global pointers is not modeled."],
    }
    (out / "analysis/callback_configuration_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{cases} callback configuration full-RAM comparisons passed")


if __name__ == "__main__":
    main()
