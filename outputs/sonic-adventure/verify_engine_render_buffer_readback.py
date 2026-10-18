"""Verify the VRAM-to-RAM wrapper composed with the native word-copy routine."""
import ctypes
import hashlib
import itertools
import json
import random

from verify_engine_cache_writeback import IO, Read, Write
from verify_fpu_cleanup_transfer import Fpu
from verify_startup import ROOT, State, reference
from verify_tikal_controller import Call, P


def main():
    project = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    source = project / "source/sa_engine_render_buffer_readback.c"
    listing = project / "analysis/instructions/video_setup_region.tsv"
    wrapper_path = ROOT / "work/sonic-adventure/build/engine_render_buffer_readback.dll"
    copy_path = ROOT / "work/sonic-adventure/build/engine_word_copy.dll"
    wrapper = ctypes.CDLL(str(wrapper_path)).sa_engine_render_buffer_readback
    copy = ctypes.CDLL(str(copy_path)).sa_engine_word_copy
    ByteP = ctypes.POINTER(ctypes.c_uint8)
    IO_P = ctypes.POINTER(IO)
    wrapper.argtypes = [ByteP, ctypes.POINTER(Fpu), P, P, P, P, P,
                        ctypes.c_void_p, Call]
    wrapper.restype = ctypes.c_int
    copy.argtypes = [ByteP, ctypes.POINTER(Fpu), P, P, P, P, P,
                     IO_P, ctypes.c_void_p, Call]
    copy.restype = ctypes.c_int

    cases, total_steps, total_words = [], 0, 0
    for ram_offset, vram_offset, size, bank, seed in itertools.product(
            [0x1000, 0x2340], [0x80, 0x1234], [4, 7, 24, 128],
            [0, 0x200000], [0, 1]):
        rng = random.Random(0x653492 ^ ram_offset ^ vram_offset ^ size ^ bank ^ seed)
        ram = bytearray(16 * 1024 * 1024)
        ram[0x10000:0x10000 + len(binary)] = binary
        destination = 0x8cb00000 + ram_offset
        source_address = 0xa5000000 + vram_offset
        ram_destination_offset = destination & 0xffffff
        ram[ram_destination_offset - 4:ram_destination_offset + 164] = bytes([0xa5]) * 168
        count = (size >> 2) * 4
        initial_words = {source_address + index: rng.getrandbits(32)
                         for index in range(-4, 164, 4)}
        retail_ram = bytearray(ram)
        state = State()
        state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        state.r[4], state.r[5], state.r[6] = destination, vram_offset, size
        state.r[15] = 0x8cf00000
        state.sr = 0x70000020 | seed
        state.pr, state.pc = 0x8c000100, 0x8c653492
        entry_state = State.from_buffer_copy(bytes(state))
        fpu = Fpu()
        fpu.fpscr = bank
        for b in range(2):
            for i in range(16):
                fpu.banks[b][i] = rng.getrandbits(32)
        retail_fpu = Fpu.from_buffer_copy(bytes(fpu))
        fpul_value, macl_value = rng.getrandbits(32), rng.getrandbits(32)
        mmio = dict(initial_words)
        steps = reference(retail_ram, state, fpu=retail_fpu, mmio=mmio,
                          stop_pc=entry_state.pr)
        total_steps += steps

        native_ram = bytearray(ram)
        view = (ctypes.c_uint8 * len(native_ram)).from_buffer(native_ram)
        regs = (ctypes.c_uint32 * 16)(*entry_state.r)
        sr, pr = ctypes.c_uint32(entry_state.sr), ctypes.c_uint32(entry_state.pr)
        fpul, macl = ctypes.c_uint32(fpul_value), ctypes.c_uint32(macl_value)
        native_mmio = dict(initial_words)
        errors, calls, reads = [], [], []

        @Read
        def read_device(_context, address, width):
            if address not in native_mmio or width != 4:
                errors.append(("bad read", address, width))
                return 0
            reads.append(address)
            return native_mmio[address]

        @Write
        def write_device(_context, address, value, width):
            errors.append(("unexpected write", address, value, width))

        io = IO(None, read_device, write_device)
        helper_fpul, helper_macl = ctypes.c_uint32(), ctypes.c_uint32()

        @Call
        def no_nested_call(_ctx, target, _memory, _f, _sr, _r, _pr):
            errors.append(("unexpected nested service", target))
            return -9

        @Call
        def invoke_copy(_context, target, memory, live_fpu, live_sr,
                        live_regs, live_pr):
            calls.append((target, live_regs[4], live_regs[5], live_regs[6],
                          live_pr[0]))
            if target != 0x8c65253e:
                return -9
            return copy(memory, live_fpu, live_sr, live_regs, live_pr,
                        ctypes.byref(helper_fpul), ctypes.byref(helper_macl),
                        ctypes.byref(io), None, no_nested_call)

        native_fpu = Fpu.from_buffer_copy(bytes(fpu))
        assert wrapper(view, ctypes.byref(native_fpu), ctypes.byref(sr), regs,
                       ctypes.byref(pr), ctypes.byref(fpul), ctypes.byref(macl),
                       None, invoke_copy) == 0

        assert not errors
        assert native_ram == retail_ram
        assert list(regs) == list(state.r)
        assert sr.value == state.sr and pr.value == state.pr
        assert bytes(native_fpu) == bytes(retail_fpu)
        assert fpul.value == fpul_value and macl.value == macl_value
        assert native_mmio == mmio
        assert calls == [(0x8c65253e, destination, source_address, size,
                          0x8c65349e)]
        expected_read_addresses = [source_address + index
                                   for index in range(0, count, 4)]
        assert reads == expected_read_addresses
        expected_payload = b"".join(initial_words[source_address + index].to_bytes(4, "little")
                                     for index in range(0, count, 4))
        assert native_ram[ram_destination_offset - 4:ram_destination_offset] == bytes([0xa5]) * 4
        assert native_ram[ram_destination_offset:ram_destination_offset + count] == expected_payload
        assert native_ram[ram_destination_offset + count:ram_destination_offset + 164] == bytes([0xa5]) * (164 - count)
        total_words += count // 4
        cases.append({"ram_destination": hex(destination),
                      "vram_source": hex(source_address), "size": size,
                      "fpscr_bank": hex(bank), "seed": seed,
                      "read_addresses": [hex(v) for v in reads],
                      "full_ram_cpu_fpu_mmio_match": True,
                      "helper_call_arguments_match": True,
                      "retail_instructions_executed": steps})

    start, end = 0x8c653492, 0x8c6534a8
    begin, finish = start - 0x8c010000, end - 0x8c010000
    report = {
        "module": "1ST_READ.BIN", "routine": hex(start),
        "end_exclusive": hex(end), "cases_passed": len(cases),
        "original_instructions_executed": total_steps,
        "copied_words": total_words, "cases": cases,
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[begin:finish]).hexdigest(),
        "source": "source/sa_engine_render_buffer_readback.c",
        "instruction_listing": "analysis/instructions/video_setup_region.tsv",
        "instruction_listing_sha256": hashlib.sha256(listing.read_bytes()).hexdigest(),
        "source_sha256": hashlib.sha256(source.read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(wrapper_path.read_bytes()).hexdigest(),
        "word_copy_dll_sha256": hashlib.sha256(copy_path.read_bytes()).hexdigest(),
        "scope": "Connects the retail VRAM-to-RAM wrapper to the native word-copy body. Compares full RAM/CPU/FPU/MMIO state for four byte lengths, two VRAM offsets, two RAM offsets, two FPU banks and two seeds.",
        "limitations": ["VRAM is provided as controlled MMIO read data.",
                        "Physical graphics hardware and rendered output are not covered."],
    }
    (project / "analysis/engine_render_buffer_readback_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} connected full-state cases passed; {total_steps} retail instructions and {total_words} words read back")


if __name__ == "__main__":
    main()
