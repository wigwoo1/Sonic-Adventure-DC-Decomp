"""Compare two packet MMIO helpers with retail using deterministic read streams."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference


class SequenceMMIO(dict):
    def __init__(self, address, values):
        super().__init__({address: 0})
        self.address = address
        self.values = list(values)
        self.reads = 0

    def __getitem__(self, address):
        if address == self.address:
            self.reads += 1
            return self.values.pop(0)
        return super().__getitem__(address)


def main():
    out = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    old = bytearray(16 * 1024 * 1024)
    old[0x10000:0x10000 + len(binary)] = binary
    native = bytearray(old)
    view = (ctypes.c_uint8 * len(native)).from_buffer(native)
    dll_path = ROOT / "work/sonic-adventure/build/packet_mmio.dll"
    lib = ctypes.CDLL(str(dll_path))
    read_type = ctypes.CFUNCTYPE(ctypes.c_uint32, ctypes.c_void_p, ctypes.c_uint32)
    transition = lib.sa_mmio_transition_wait_8c604594
    transition.argtypes = [read_type, ctypes.c_void_p]
    transition.restype = None
    idle_store = lib.sa_mmio_idle_flag_store_8c6045a8
    idle_store.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint32,
                           read_type, ctypes.c_void_p]
    idle_store.restype = None

    rng = random.Random(0x8c604594)
    transition_cases = transition_steps = 0
    address, mask = 0xa05f810c, 0xff0f01ff
    for leading_busy in range(5):
        for trailing_clear in range(5):
            # The high nibble in this value is outside the retail mask.
            sequence = [mask] * leading_busy + [0x00f00000] + [0x00f00000] * trailing_clear + [1]
            mmio = SequenceMMIO(address, sequence.copy())
            regs = State()
            regs.r[:] = [rng.getrandbits(32) for _ in range(16)]
            regs.r[15] = 0x8cf00000
            regs.pc, regs.pr = 0x8c604594, 0x8c000100
            expected = State.from_buffer_copy(bytes(regs))
            transition_steps += reference(old, expected, mmio=mmio)

            values = list(sequence)
            native_reads = []
            def read_sequence(_context, requested_address):
                assert requested_address == address
                native_reads.append(requested_address)
                return values.pop(0)
            native_reader = read_type(read_sequence)
            transition(native_reader, None)
            assert not values
            assert len(native_reads) == mmio.reads
            transition_cases += 1

    region = 0x00a00000
    destination = 0x8c000000 + region
    idle_cases = idle_steps = 0
    mmio_values = list(range(256)) + [0xffffffff, 0x12345678] + [rng.getrandbits(32) for _ in range(64)]
    for alias in (0x8c000000, 0xac000000):
        for value in mmio_values:
            old[region:region + 4] = b"\x5a\xa5\x5a\xa5"
            native[region:region + 4] = b"\x5a\xa5\x5a\xa5"
            mmio = {0xa05f688c: value}
            regs = State()
            regs.r[:] = [rng.getrandbits(32) for _ in range(16)]
            regs.r[4], regs.r[15] = alias + region, 0x8cf00000
            regs.pc, regs.pr = 0x8c6045a8, 0x8c000100
            expected = State.from_buffer_copy(bytes(regs))
            idle_steps += reference(old, expected, mmio=mmio)

            def read_single(_context, requested_address):
                assert requested_address == 0xa05f688c
                return value
            native_reader = read_type(read_single)
            idle_store(view, alias + region, native_reader, None)
            assert native[region:region + 4] == old[region:region + 4]
            idle_cases += 1

    defs = [
        {"name": "mmio_transition_wait", "routine_start": "0x8c604594", "routine_end_exclusive": "0x8c6045a8",
         "cases_passed": transition_cases, "retail_instructions_executed": transition_steps,
         "scope": "MMIO polling transition compared over 25 deterministic sequences, including values with only bits outside the retail mask set."},
        {"name": "mmio_idle_flag_store", "routine_start": "0x8c6045a8", "routine_end_exclusive": "0x8c6045b4",
         "cases_passed": idle_cases, "retail_instructions_executed": idle_steps,
         "scope": "Output memory comparison for all low-byte MMIO values, fixed edge values and 64 deterministic words, with cached and uncached RAM aliases."},
    ]
    for routine in defs:
        start, end = int(routine["routine_start"], 16), int(routine["routine_end_exclusive"], 16)
        routine["retail_span_sha256"] = hashlib.sha256(binary[start - 0x8c010000:end - 0x8c010000]).hexdigest()
    report = {
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "source": "source/packet_mmio.c",
        "source_sha256": hashlib.sha256((out / "source/packet_mmio.c").read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "routines": defs,
        "limitations": ["MMIO is injected through a host read adapter; electrical timing, asynchronous register changes outside supplied sequences, and SH-4 bus faults are not modeled."],
    }
    (out / "analysis/packet_mmio_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{transition_cases} transition waits and {idle_cases} idle-flag stores matched retail")


if __name__ == "__main__":
    main()
