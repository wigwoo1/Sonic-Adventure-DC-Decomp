"""Differentially test packet CRC and validation against retail SH-4 routines."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference


def run_retail(ram, address, args):
    regs = State()
    regs.r[:] = [0xdead0000 + n * 0x101 for n in range(16)]
    # Keep the ABI setup explicit so accidental argument reordering is visible.
    regs.r[4] = args[0]
    if len(args) > 1:
        regs.r[5] = args[1]
    if len(args) > 2:
        regs.r[6] = args[2]
    regs.r[15] = 0x8CF00000
    regs.pc, regs.pr = address, 0x8C000100
    result = State.from_buffer_copy(bytes(regs))
    steps = reference(ram, result)
    return result.r[0], steps


def main():
    out = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    old = bytearray(16 * 1024 * 1024)
    old[0x10000:0x10000 + len(binary)] = binary
    native = bytearray(old)
    view = (ctypes.c_uint8 * len(native)).from_buffer(native)
    dll_path = ROOT / "work/sonic-adventure/build/packet_checksum.dll"
    lib = ctypes.CDLL(str(dll_path))
    crc_fn = lib.sa_crc16_packet_8c64bda0
    crc_fn.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_int32, ctypes.c_uint32]
    crc_fn.restype = ctypes.c_uint32
    packet_fn = lib.sa_packet_validate_8c603cd2
    packet_fn.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint32,
                          ctypes.c_uint32]
    packet_fn.restype = ctypes.c_uint32
    buffer_fn = lib.sa_packet_buffer_check_8c603f8c
    buffer_fn.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint32]
    buffer_fn.restype = ctypes.c_int32

    region = 0x00A00000
    rng = random.Random(0x8C64BDA0)
    crc_cases = 0
    crc_steps = 0
    lengths = list(range(-3, 66)) + [127, 255]
    for alias in (0x8C000000, 0xAC000000):
        for length in lengths:
            data = bytes(rng.getrandbits(8) for _ in range(272))
            old[region:region + len(data)] = data
            native[region:region + len(data)] = data
            actual, steps = run_retail(old, 0x8C64BDA0, (length & 0xffffffff, alias + region))
            crc_steps += steps
            actual_native = crc_fn(view, length, alias + region)
            assert actual_native == actual, (alias, length, hex(actual_native), hex(actual))
            assert native[region:region + len(data)] == old[region:region + len(data)]
            crc_cases += 1

    packet_cases = 0
    packet_steps = 0
    def compare_packet(alias, packet):
        nonlocal packet_cases, packet_steps
        old[region:region + 64] = packet
        native[region:region + 64] = packet
        context = alias + region + 128
        actual, steps = run_retail(old, 0x8C603CD2, (context, alias + region))
        packet_steps += steps
        native_result = packet_fn(view, context, alias + region)
        assert native_result == actual, (alias, packet.hex(), native_result, actual)
        assert native[region:region + 64] == old[region:region + 64], (alias, packet.hex())
        packet_cases += 1

    # Exercise every early-exit byte, ignored header bytes, valid CRC success,
    # invalid checksum, and deterministic mixed packet contents.
    for alias in (0x8C000000, 0xAC000000):
        packet = bytearray([0xff] * 64)
        packet[0:2] = b"\x12\x34"
        # Populate a fixture first so the CRC reads the intended 62 bytes.
        old[region:region + 64] = packet
        native[region:region + 64] = packet
        checksum, steps = run_retail(old, 0x8C64BDA0, (62, alias + region))
        crc_steps += steps
        packet[62:64] = checksum.to_bytes(2, "little")
        compare_packet(alias, packet)

        invalid = bytearray(packet)
        invalid[62] ^= 1
        compare_packet(alias, invalid)
        for index in range(2, 62):
            invalid = bytearray(packet)
            invalid[index] ^= 1
            compare_packet(alias, invalid)

        for _ in range(100):
            mixed = bytearray(rng.getrandbits(8) for _ in range(64))
            if rng.random() < 0.5:
                mixed[2:62] = b"\xff" * 60
                old[region:region + 64] = mixed
                checksum, steps = run_retail(old, 0x8C64BDA0, (62, alias + region))
                crc_steps += steps
                mixed[62:64] = checksum.to_bytes(2, "little")
            compare_packet(alias, mixed)

    # The next caller copies a 60-byte payload into a packet slot and then
    # checks the 0xffff sentinel or CRC, unless its state field is nonzero.
    buffer_cases = 0
    buffer_steps = 0
    for alias in (0x8C000000, 0xAC000000):
        context = alias + region
        packet = context + 0x180
        for mode in ("state", "sentinel", "valid_crc", "bad_crc"):
            for _ in range(32):
                initial = bytearray(rng.getrandbits(8) for _ in range(0x300))
                ctxoff = region
                initial[0x0c:0x10] = (context + 0x100).to_bytes(4, "little")
                initial[0x28:0x2c] = (1).to_bytes(4, "little")
                source = context + 0x80
                initial[0x30:0x34] = source.to_bytes(4, "little")
                initial[0x80:0xbc] = bytes(rng.getrandbits(8) for _ in range(60))
                packetoff = packet - context
                initial[packetoff:packetoff + 2] = (0xffff if mode == "sentinel" else 0x1234).to_bytes(2, "little")
                initial[packetoff + 2:packetoff + 62] = initial[0x80:0xbc]
                initial[packetoff + 62:packetoff + 64] = b"\x00\x00"
                if mode == "valid_crc":
                    old[region:region + len(initial)] = initial
                    checksum, steps = run_retail(old, 0x8C64BDA0, (62, packet))
                    crc_steps += steps
                    initial[packetoff + 62:packetoff + 64] = checksum.to_bytes(2, "little")
                elif mode == "bad_crc":
                    initial[packetoff + 62:packetoff + 64] = b"\x01\x00"
                if mode == "state":
                    initial[2:4] = (1).to_bytes(2, "little")
                else:
                    initial[2:4] = b"\x00\x00"
                old[region:region + len(initial)] = initial
                native[region:region + len(initial)] = initial
                actual, steps = run_retail(old, 0x8C603F8C, (context,))
                buffer_steps += steps
                native_result = buffer_fn(view, context)
                assert (native_result & 0xffffffff) == actual, (alias, mode, native_result, actual)
                assert native[region:region + len(initial)] == old[region:region + len(initial)], (alias, mode)
                buffer_cases += 1

    source_hash = hashlib.sha256((out / "source/packet_checksum.c").read_bytes()).hexdigest()
    dll_hash = hashlib.sha256(dll_path.read_bytes()).hexdigest()
    routines = [
        ("0x8c64bda0", "0x8c64bdd4", "crc16", crc_cases, crc_steps,
         "CRC-16 return and read-only memory comparison for signed lengths -3..65, 127 and 255 with deterministic input data and both cached/uncached RAM aliases."),
        ("0x8c603cd2", "0x8c603d22", "packet_validator", packet_cases, packet_steps,
         "Return and full 64-byte memory comparison for checksum success/failure, every padding-byte early exit, ignored header bytes, mixed packets, and both RAM aliases."),
        ("0x8c603f8c", "0x8c603fe4", "packet_buffer_check", buffer_cases, buffer_steps,
         "Return and full context-memory comparison for nonzero state, sentinel bypass, matching CRC and mismatched CRC after payload copy, under both RAM aliases."),
    ]
    report = {
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "source": "source/packet_checksum.c", "source_sha256": source_hash,
        "dll_sha256": dll_hash,
        "routines": [{"routine_start": start, "routine_end_exclusive": end,
                      "name": name,
                      "retail_span_sha256": hashlib.sha256(binary[int(start, 16) - 0x8C010000:int(end, 16) - 0x8C010000]).hexdigest(),
                      "cases_passed": cases, "retail_instructions_executed": steps,
                      "scope": scope} for start, end, name, cases, steps, scope in routines],
        "limitations": ["Unmapped pointers, SH-4 exception/cache effects, and the CRC INT_MIN post-decrement case that would execute over two billion iterations are outside this test."],
    }
    (out / "analysis/packet_checksum_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{crc_cases} CRC and {packet_cases} packet validation cases matched retail")


if __name__ == "__main__":
    main()
