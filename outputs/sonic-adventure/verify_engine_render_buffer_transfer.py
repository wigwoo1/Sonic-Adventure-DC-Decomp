"""Verify the render-buffer wrapper composed with the native word-copy routine."""
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
    source = project / "source/sa_engine_render_buffer_transfer.c"
    listing = project / "analysis/instructions/engine_render_buffer_transfer_binary.tsv"
    wrapper_path = ROOT / "work/sonic-adventure/build/engine_render_buffer_transfer.dll"
    copy_path = ROOT / "work/sonic-adventure/build/engine_word_copy.dll"
    wrapper = ctypes.CDLL(str(wrapper_path)).sa_engine_render_buffer_transfer
    copy = ctypes.CDLL(str(copy_path)).sa_engine_word_copy
    ByteP = ctypes.POINTER(ctypes.c_uint8)
    IO_P = ctypes.POINTER(IO)
    wrapper.argtypes = [ByteP, ctypes.POINTER(Fpu), P, P, P, P, P,
                        ctypes.c_void_p, Call]
    wrapper.restype = ctypes.c_int
    copy.argtypes = [ByteP, ctypes.POINTER(Fpu), P, P, P, P, P,
                     IO_P, ctypes.c_void_p, Call]
    copy.restype = ctypes.c_int

    cases = []
    total_steps = 0
    total_words = 0
    for offset, size, bank, seed in itertools.product(
            [0x80, 0x1234], [4, 7, 24, 128], [0, 0x200000], [0, 1]):
        rng = random.Random(0x65347c ^ offset ^ size ^ bank ^ seed)
        ram = bytearray(16 * 1024 * 1024)
        ram[0x10000:0x10000 + len(binary)] = binary
        payload = bytes(rng.getrandbits(8) for _ in range(160))
        ram[0xb00000:0xb00000 + len(payload)] = payload
        destination = 0xa5000000 + offset
        count = (size >> 2) * 4
        registers = {destination + i: 0xa5a5a5a5
                     for i in range(-4, 164, 4)}
        retail_ram = bytearray(ram)
        retail_state = State()
        retail_state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        retail_state.r[4], retail_state.r[5], retail_state.r[6] = (
            offset, 0x8cb00000, size)
        retail_state.r[15] = 0x8cf00000
        retail_state.sr = 0x70000020 | seed
        retail_state.pr, retail_state.pc = 0x8c000100, 0x8c65347c
        entry_state = State.from_buffer_copy(bytes(retail_state))
        fpu = Fpu()
        fpu.fpscr = bank
        for b in range(2):
            for i in range(16):
                fpu.banks[b][i] = rng.getrandbits(32)
        retail_fpu = Fpu.from_buffer_copy(bytes(fpu))
        fpul_value, macl_value = rng.getrandbits(32), rng.getrandbits(32)
        mmio = dict(registers)
        steps = reference(retail_ram, retail_state, fpu=retail_fpu,
                          mmio=mmio, stop_pc=retail_state.pr)
        total_steps += steps

        native_ram = bytearray(ram)
        view = (ctypes.c_uint8 * len(native_ram)).from_buffer(native_ram)
        entry = entry_state
        regs = (ctypes.c_uint32 * 16)(*entry.r)
        sr, pr = ctypes.c_uint32(entry.sr), ctypes.c_uint32(entry.pr)
        fpul, macl = ctypes.c_uint32(fpul_value), ctypes.c_uint32(macl_value)
        native_mmio = dict(registers)
        errors, calls, writes = [], [], []

        @Read
        def read_device(_context, address, width):
            errors.append(("unexpected read", address, width))
            return 0

        @Write
        def write_device(_context, address, value, width):
            if address not in native_mmio or width != 4:
                errors.append(("bad write", address, width))
                return
            native_mmio[address] = value
            writes.append((address, value))

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
        assert list(regs) == list(retail_state.r)
        assert sr.value == retail_state.sr and pr.value == retail_state.pr
        assert bytes(native_fpu) == bytes(retail_fpu)
        assert fpul.value == fpul_value and macl.value == macl_value
        assert native_mmio == mmio
        assert len(calls) == 1 and calls[0][0] == 0x8c65253e
        assert calls[0][1:] == (destination, 0x8cb00000, size, 0x8c65348a)
        assert writes == [(destination + i,
                           int.from_bytes(payload[i:i + 4], "little"))
                          for i in range(0, count, 4)]
        assert all(value == 0xa5a5a5a5
                   for address, value in native_mmio.items()
                   if not destination <= address < destination + count)
        total_words += count // 4
        cases.append({"destination": hex(destination), "size": size,
                      "fpscr_bank": hex(bank), "seed": seed,
                      "copied_words": count // 4,
                      "full_ram_cpu_fpu_mmio_match": True,
                      "helper_call_arguments_match": True,
                      "retail_instructions_executed": steps})

    start, end = 0x8c65347c, 0x8c653492
    begin, finish = start - 0x8c010000, end - 0x8c010000
    report = {
        "module": "1ST_READ.BIN", "routine": hex(start),
        "end_exclusive": hex(end), "cases_passed": len(cases),
        "original_instructions_executed": total_steps,
        "copied_words": total_words, "cases": cases,
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[begin:finish]).hexdigest(),
        "source": "source/sa_engine_render_buffer_transfer.c",
        "instruction_listing": "analysis/instructions/engine_render_buffer_transfer_binary.tsv",
        "instruction_listing_sha256": hashlib.sha256(listing.read_bytes()).hexdigest(),
        "source_sha256": hashlib.sha256(source.read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(wrapper_path.read_bytes()).hexdigest(),
        "word_copy_dll_sha256": hashlib.sha256(copy_path.read_bytes()).hexdigest(),
        "scope": "Connects the retail wrapper to the separately recovered native word-copy body. Compares full 16 MiB RAM, CPU/FPU, MMIO register contents and write sequence for RAM-source to VRAM transfers with four sizes, two offsets, two FPU banks and two seeds.",
        "limitations": ["Only the upload-to-VRAM direction is verified here.",
                        "Physical graphics hardware and rendered output are not covered."],
    }
    (project / "analysis/engine_render_buffer_transfer_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} connected full-state cases passed; {total_steps} retail instructions and {total_words} copied words")


if __name__ == "__main__":
    main()
