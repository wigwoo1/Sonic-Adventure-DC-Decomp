"""Compare the packet global setter's memory effect with retail SH-4 execution."""
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
    dll_path = ROOT / "work/sonic-adventure/build/packet_global_setter.dll"
    fn = ctypes.CDLL(str(dll_path)).sa_packet_global_value_set_8c604510
    fn.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint32]
    fn.restype = None
    callback_fn = ctypes.CDLL(str(dll_path)).sa_packet_callback_reset_8c604546
    callback_type = ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint32)
    callback_fn.argtypes = [ctypes.POINTER(ctypes.c_uint8), callback_type]
    callback_fn.restype = None

    destination = 0x8c673454
    offset = destination - 0x8c000000
    rng = random.Random(0x8c604510)
    cases = steps = 0
    for value in [0, 1, 0xffffffff, 0x12345678, 0x80000000] + [rng.getrandbits(32) for _ in range(128)]:
        initial = rng.getrandbits(32)
        old[offset:offset + 4] = struct.pack("<I", initial)
        native[offset:offset + 4] = struct.pack("<I", initial)
        regs = State()
        regs.r[:] = [rng.getrandbits(32) for _ in range(16)]
        regs.r[4], regs.r[15] = value, 0x8cf00000
        regs.pc, regs.pr = 0x8c604510, 0x8c000100
        expected = State.from_buffer_copy(bytes(regs))
        steps += reference(old, expected)
        fn(view, value)
        assert native[offset:offset + 4] == old[offset:offset + 4] == struct.pack("<I", value)
        cases += 1

    callback_cases = callback_steps = 0
    callback_slot = 0x8c673478 - 0x8c000000
    flag_a = 0x8c88f7d0 - 0x8c000000
    flag_b = 0x8c88f7d8 - 0x8c000000
    callback_byte = 0x00a00080
    targets = (0, 0x8c000200, 0x8c000202, 0xffffffff)
    for target in targets:
        for initial_byte in range(8):
            for array, slot_value in ((old, target), (native, target)):
                array[callback_slot:callback_slot + 4] = struct.pack("<I", slot_value)
                array[flag_a:flag_a + 4] = struct.pack("<I", 0x11111111)
                array[flag_b:flag_b + 4] = struct.pack("<I", 0x22222222)
                array[callback_byte] = initial_byte
            regs = State()
            regs.r[:] = [rng.getrandbits(32) for _ in range(16)]
            regs.r[15] = 0x8cf00000
            regs.pc, regs.pr = 0x8c604546, 0x8c000100
            expected = State.from_buffer_copy(bytes(regs))
            def retail_callback(ram, _regs, sr):
                ram[callback_byte] = (ram[callback_byte] + 1) & 0xff
                return sr
            hooks = {target: retail_callback} if target else None
            callback_steps += reference(old, expected, hooks=hooks)

            def host_dispatch(_ram_pointer, sh4_target):
                if sh4_target:
                    native[callback_byte] = (native[callback_byte] + 1) & 0xff
            callback = callback_type(host_dispatch)
            callback_fn(view, callback)
            assert native[flag_a:flag_a + 4] == old[flag_a:flag_a + 4] == b"\x00\x00\x00\x00"
            assert native[flag_b:flag_b + 4] == old[flag_b:flag_b + 4] == b"\x01\x00\x00\x00"
            assert native[callback_byte] == old[callback_byte]
            callback_cases += 1

    routines = [
        {"name": "packet_global_value_set", "routine_start": "0x8c604510", "routine_end_exclusive": "0x8c604516",
         "cases_passed": cases, "retail_instructions_executed": steps,
         "scope": "Destination global write comparison for fixed edge values and 128 deterministic 32-bit values, including zero and all-ones."},
        {"name": "packet_callback_reset", "routine_start": "0x8c604546", "routine_end_exclusive": "0x8c604568",
         "cases_passed": callback_cases, "retail_instructions_executed": callback_steps,
         "scope": "Two global resets and optional callback dispatch comparison for null and three non-null targets, with eight prior callback-byte values."},
    ]
    for routine in routines:
        start, end = int(routine["routine_start"], 16), int(routine["routine_end_exclusive"], 16)
        routine["retail_span_sha256"] = hashlib.sha256(binary[start - 0x8c010000:end - 0x8c010000]).hexdigest()
    report = {
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "source": "source/packet_global_setter.c",
        "source_sha256": hashlib.sha256((out / "source/packet_global_setter.c").read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "routines": routines,
        "limitations": ["Callback dispatch is represented by a native resolver for SH-4 target addresses; unmapped-memory exceptions are outside these tests."],
    }
    (out / "analysis/packet_global_setter_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{cases} setter and {callback_cases} callback-reset cases matched retail")


if __name__ == "__main__":
    main()
