"""Compare the connected cached video-aperture transfer with retail execution."""
import ctypes
import hashlib
import itertools
import json
import random

from verify_engine_cache_writeback import IO, Read, Write
from verify_fpu_cleanup_transfer import Fpu
from verify_startup import ROOT, State, reference
from verify_tikal_controller import P

CacheWriteback = ctypes.CFUNCTYPE(ctypes.c_int, ctypes.c_void_p,
                                  ctypes.c_uint32)


def main():
    project = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    source = project / "source/sa_engine_render_buffer_cached_transfer.c"
    listing = project / "analysis/instructions/video_setup_region.tsv"
    dll_path = ROOT / "work/sonic-adventure/build/engine_render_buffer_cached_transfer.dll"
    fn = ctypes.CDLL(str(dll_path)).sa_engine_render_buffer_cached_transfer
    ByteP = ctypes.POINTER(ctypes.c_uint8)
    IO_P = ctypes.POINTER(IO)
    fn.argtypes = [ByteP, ctypes.POINTER(Fpu), P, P, P, P, P,
                   IO_P, ctypes.c_void_p, CacheWriteback]
    fn.restype = ctypes.c_int

    cases, total_steps, total_words, total_writebacks = [], 0, 0, 0
    for ram_offset, aperture_offset, size, bank, seed in itertools.product(
            [0x1000, 0x2340], [0x80, 0x1234], [4, 7, 24, 128],
            [0, 0x200000], [0, 1]):
        rng = random.Random(0x6535c2 ^ ram_offset ^ aperture_offset ^ size ^ bank ^ seed)
        ram = bytearray(16 * 1024 * 1024)
        ram[0x10000:0x10000 + len(binary)] = binary
        destination = 0x8cb00000 + ram_offset
        aperture = 0xa4000000 + aperture_offset
        destination_offset = destination & 0xffffff
        ram[destination_offset - 4:destination_offset + 164] = bytes([0xa5]) * 168
        count = (size >> 2) * 4
        initial_mmio = {aperture + index: rng.getrandbits(32)
                        for index in range(-4, 164, 4)}
        state = State()
        state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        state.r[4], state.r[5], state.r[6] = destination, aperture_offset, size
        state.r[15] = 0x8cf00000
        state.sr = 0x70000020 | seed
        state.pr, state.pc = 0x8c000100, 0x8c6535c2
        entry = State.from_buffer_copy(bytes(state))
        fpu = Fpu()
        fpu.fpscr = bank
        for b in range(2):
            for i in range(16):
                fpu.banks[b][i] = rng.getrandbits(32)
        retail_fpu = Fpu.from_buffer_copy(bytes(fpu))
        fpul_value, macl_value = rng.getrandbits(32), rng.getrandbits(32)
        retail_ram = bytearray(ram)
        mmio = dict(initial_mmio)
        expected_writebacks = []
        steps = reference(retail_ram, state, fpu=retail_fpu, mmio=mmio,
                          cache_writeback=expected_writebacks.append,
                          stop_pc=entry.pr)
        total_steps += steps

        native_ram = bytearray(ram)
        view = (ctypes.c_uint8 * len(native_ram)).from_buffer(native_ram)
        regs = (ctypes.c_uint32 * 16)(*entry.r)
        sr, pr = ctypes.c_uint32(entry.sr), ctypes.c_uint32(entry.pr)
        fpul, macl = ctypes.c_uint32(fpul_value), ctypes.c_uint32(macl_value)
        native_mmio = dict(initial_mmio)
        errors, reads, writebacks = [], [], []

        @Read
        def read_aperture(_context, address, width):
            if address not in native_mmio or width != 4:
                errors.append(("bad aperture read", address, width))
                return 0
            reads.append(address)
            return native_mmio[address]

        @Write
        def unexpected_io_write(_context, address, value, width):
            errors.append(("unexpected I/O write", address, value, width))

        io = IO(None, read_aperture, unexpected_io_write)

        @CacheWriteback
        def cache_writeback(_context, address):
            writebacks.append(address)
            return 0

        native_fpu = Fpu.from_buffer_copy(bytes(fpu))
        assert fn(view, ctypes.byref(native_fpu), ctypes.byref(sr), regs,
                  ctypes.byref(pr), ctypes.byref(fpul), ctypes.byref(macl),
                  ctypes.byref(io), None, cache_writeback) == 0
        assert not errors
        assert native_ram == retail_ram
        assert list(regs) == list(state.r)
        assert sr.value == state.sr and pr.value == state.pr
        assert bytes(native_fpu) == bytes(retail_fpu)
        assert fpul.value == fpul_value and macl.value == macl_value
        assert native_mmio == mmio
        expected_reads = [aperture + index for index in range(0, count, 4)]
        assert reads == expected_reads
        assert writebacks == expected_writebacks
        payload = b"".join(initial_mmio[aperture + index].to_bytes(4, "little")
                           for index in range(0, count, 4))
        assert native_ram[destination_offset - 4:destination_offset] == bytes([0xa5]) * 4
        assert native_ram[destination_offset:destination_offset + count] == payload
        assert native_ram[destination_offset + count:destination_offset + 164] == bytes([0xa5]) * (164 - count)
        total_words += count // 4
        total_writebacks += len(writebacks)
        cases.append({"ram_destination": hex(destination),
                      "a4_aperture_source": hex(aperture), "size": size,
                      "fpscr_bank": hex(bank), "seed": seed,
                      "read_addresses": [hex(v) for v in reads],
                      "writeback_addresses": [hex(v) for v in writebacks],
                      "full_ram_cpu_fpu_mmio_match": True,
                      "retail_instructions_executed": steps})

    start, end = 0x8c6535c2, 0x8c6535e6
    begin, finish = start - 0x8c010000, end - 0x8c010000
    report = {
        "module": "1ST_READ.BIN", "routine": hex(start),
        "end_exclusive": hex(end), "cases_passed": len(cases),
        "original_instructions_executed": total_steps,
        "copied_words": total_words, "cache_writebacks_observed": total_writebacks,
        "cases": cases,
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[begin:finish]).hexdigest(),
        "source": "source/sa_engine_render_buffer_cached_transfer.c",
        "instruction_listing": "analysis/instructions/video_setup_region.tsv",
        "instruction_listing_sha256": hashlib.sha256(listing.read_bytes()).hexdigest(),
        "source_sha256": hashlib.sha256(source.read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Composes the recovered main-memory copy, A4 hardware aperture reads and 32-byte cache-writeback loop. Compares full RAM/CPU/FPU/MMIO state, exact aperture read and cache-writeback sequences for 64 cases.",
        "limitations": ["A4 aperture data and cache maintenance are modeled by host callbacks.",
                        "Physical Dreamcast hardware and complete renderer integration remain unverified."],
    }
    (project / "analysis/engine_render_buffer_cached_transfer_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} connected full-state cases passed; {total_steps} retail instructions, {total_words} words, {total_writebacks} cache operations")


if __name__ == "__main__":
    main()
