"""Compare directory descriptor-slot initialization with retail SH-4 execution."""
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
    dll_path = ROOT / "work/sonic-adventure/build/directory_slot_init.dll"
    fn = ctypes.CDLL(str(dll_path)).sa_directory_slot_initialize_8c645898
    fn.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_int32,
                   ctypes.c_uint32, ctypes.c_uint32, ctypes.c_int32]
    fn.restype = None

    table = int.from_bytes(binary[0x6458f4 - 0x010000:0x6458f8 - 0x010000], "little")
    region = 0x00a00000
    rng = random.Random(0x8c645898)
    cases = steps = 0
    slots = list(range(-16, 129)) + [rng.randint(-512, 512) for _ in range(80)]
    for alias in (0x8c000000, 0xac000000):
        for slot_index in slots:
            output = alias + region + 0x700
            object_pointer = alias + region + 0x400
            source = alias + region + 0x600
            initial = bytes(rng.getrandbits(8) for _ in range(0x900))
            old[region:region + len(initial)] = initial
            native[region:region + len(initial)] = initial
            source_pointer_offset = region + 0x408
            old[source_pointer_offset:source_pointer_offset + 4] = struct.pack("<I", source)
            native[source_pointer_offset:source_pointer_offset + 4] = struct.pack("<I", source)
            values = bytes(rng.getrandbits(8) for _ in range(16))
            old[region + 0x608:region + 0x618] = values
            native[region + 0x608:region + 0x618] = values
            value = rng.choice((-0x100, -0x15, -1, 0, 1, 7, 0x12345, 0x7fffffff))

            regs = State()
            regs.r[:] = [rng.getrandbits(32) for _ in range(16)]
            regs.r[4], regs.r[5], regs.r[6], regs.r[7], regs.r[15] = (
                slot_index & 0xffffffff, output, object_pointer, value & 0xffffffff, 0x8cf00000)
            regs.pc, regs.pr = 0x8c645898, 0x8c000100
            expected = State.from_buffer_copy(bytes(regs))
            steps += reference(old, expected)
            fn(view, slot_index, output, object_pointer, value)

            slot_address = (table + slot_index * 8) & 0xffffffff
            slot_offset = (slot_address & 0x1fffffff) - 0x0c000000
            assert native[region:region + len(initial)] == old[region:region + len(initial)]
            assert native[slot_offset:slot_offset + 8] == old[slot_offset:slot_offset + 8]
            cases += 1

    start, end = 0x8c645898, 0x8c6458d2
    report = {
        "cases_passed": cases, "routine_start": hex(start), "routine_end_exclusive": hex(end),
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[start - 0x8c010000:end - 0x8c010000]).hexdigest(),
        "retail_instructions_executed": steps,
        "source": "source/directory_slot_init.c",
        "source_sha256": hashlib.sha256((out / "source/directory_slot_init.c").read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Full output descriptor and selected eight-byte global-slot comparison across negative/positive indices, signed value boundaries, and cached/uncached source and output aliases.",
        "limitations": ["Indices that resolve outside mapped RAM and invalid nested object pointers are not tested."],
    }
    (out / "analysis/directory_slot_init_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{cases} directory slot initialization cases matched retail")


if __name__ == "__main__":
    main()
