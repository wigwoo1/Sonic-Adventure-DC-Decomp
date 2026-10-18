"""Verify native main-RAM/VRAM upload and readback as one connected path."""
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
    listing = project / "analysis/instructions/video_setup_region.tsv"
    build = ROOT / "work/sonic-adventure/build"
    upload_path = build / "engine_render_buffer_transfer.dll"
    readback_path = build / "engine_render_buffer_readback.dll"
    copy_path = build / "engine_word_copy.dll"
    upload = ctypes.CDLL(str(upload_path)).sa_engine_render_buffer_transfer
    readback = ctypes.CDLL(str(readback_path)).sa_engine_render_buffer_readback
    copy = ctypes.CDLL(str(copy_path)).sa_engine_word_copy
    ByteP = ctypes.POINTER(ctypes.c_uint8)
    IO_P = ctypes.POINTER(IO)
    upload.argtypes = [ByteP, ctypes.POINTER(Fpu), P, P, P, P, P,
                       ctypes.c_void_p, Call]
    readback.argtypes = upload.argtypes
    upload.restype = readback.restype = ctypes.c_int
    copy.argtypes = [ByteP, ctypes.POINTER(Fpu), P, P, P, P, P,
                     IO_P, ctypes.c_void_p, Call]
    copy.restype = ctypes.c_int

    cases, total_steps, copied_words = [], 0, 0
    for size, offset, bank, seed in itertools.product(
            [4, 7, 24, 128], [0x80, 0x1234], [0, 0x200000], [0, 1]):
        rng = random.Random(0x653470 ^ size ^ offset ^ bank ^ seed)
        initial_ram = bytearray(16 * 1024 * 1024)
        initial_ram[0x10000:0x10000 + len(binary)] = binary
        source_address, destination = 0x8cb00000, 0x8cb02000
        source_offset, destination_offset = source_address & 0xffffff, destination & 0xffffff
        initial_ram[source_offset:source_offset + 256] = bytes(
            rng.getrandbits(8) for _ in range(256))
        initial_ram[destination_offset - 4:destination_offset + 164] = bytes([0xa5]) * 168
        payload = bytes(initial_ram[source_offset:source_offset + 256])
        vram_address = 0xa5000000 + offset
        initial_mmio = {vram_address + i: 0xa5a5a5a5 for i in range(-4, 164, 4)}
        count = (size >> 2) * 4

        retail_state = State()
        retail_state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        retail_state.r[4], retail_state.r[5], retail_state.r[6] = (
            offset, source_address, size)
        retail_state.r[15] = 0x8cf00000
        retail_state.sr = 0x70000020 | seed
        retail_state.pr, retail_state.pc = 0x8c000100, 0x8c65347c
        retail_fpu = Fpu()
        retail_fpu.fpscr = bank
        for b in range(2):
            for i in range(16):
                retail_fpu.banks[b][i] = rng.getrandbits(32)
        entry_state = State.from_buffer_copy(bytes(retail_state))
        entry_fpu = Fpu.from_buffer_copy(bytes(retail_fpu))
        stop_pc = retail_state.pr

        native_ram = bytearray(initial_ram)
        view = (ctypes.c_uint8 * len(native_ram)).from_buffer(native_ram)
        regs = (ctypes.c_uint32 * 16)(*entry_state.r)
        sr = ctypes.c_uint32(entry_state.sr)
        pr = ctypes.c_uint32(stop_pc)
        fpul_value, macl_value = rng.getrandbits(32), rng.getrandbits(32)
        fpul, macl = ctypes.c_uint32(fpul_value), ctypes.c_uint32(macl_value)
        native_fpu = Fpu.from_buffer_copy(bytes(entry_fpu))
        native_mmio = dict(initial_mmio)
        errors, reads, writes = [], [], []

        @Read
        def read_device(_context, address, width):
            if address not in native_mmio or width != 4:
                errors.append(("bad read", address, width))
                return 0
            reads.append(address)
            return native_mmio[address]

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
        def no_nested_call(_ctx, target, _memory, _fpu, _sr, _r, _pr):
            errors.append(("unexpected nested call", target))
            return -9

        calls = []

        @Call
        def invoke_copy(_context, target, memory, live_fpu, live_sr,
                        live_regs, live_pr):
            calls.append((target, live_regs[4], live_regs[5], live_regs[6]))
            if target != 0x8c65253e:
                return -9
            return copy(memory, live_fpu, live_sr, live_regs, live_pr,
                        ctypes.byref(helper_fpul), ctypes.byref(helper_macl),
                        ctypes.byref(io), None, no_nested_call)

        # Replay the retail upload on a fresh reference state to compare both phases.
        upload_ram = bytearray(initial_ram)
        upload_mmio = dict(initial_mmio)
        upload_state = State.from_buffer_copy(bytes(entry_state))
        upload_fpu = Fpu.from_buffer_copy(bytes(entry_fpu))
        upload_steps = reference(upload_ram, upload_state, fpu=upload_fpu,
                                 mmio=upload_mmio, stop_pc=stop_pc)
        total_steps += upload_steps

        # Native upload; compare the intermediate RAM and device-visible payload.
        assert upload(view, ctypes.byref(native_fpu), ctypes.byref(sr), regs,
                      ctypes.byref(pr), ctypes.byref(fpul), ctypes.byref(macl),
                      None, invoke_copy) == 0
        expected_device_writes = [(vram_address + i,
                                   int.from_bytes(payload[i:i + 4], "little"))
                                  for i in range(0, count, 4)]
        assert writes == expected_device_writes
        assert native_ram == upload_ram
        assert native_mmio == upload_mmio
        assert list(regs) == list(upload_state.r)
        assert sr.value == upload_state.sr and pr.value == upload_state.pr
        assert bytes(native_fpu) == bytes(upload_fpu)

        # Reconfigure the same emulated call state for the reverse operation.
        regs[4], regs[5], regs[6] = destination, offset, size
        retail_state = State.from_buffer_copy(bytes(upload_state))
        retail_state.r[4], retail_state.r[5], retail_state.r[6] = (
            destination, offset, size)
        retail_state.pc = 0x8c653492
        readback_steps = reference(upload_ram, retail_state, fpu=upload_fpu,
                                   mmio=upload_mmio, stop_pc=stop_pc)
        total_steps += readback_steps
        reads.clear()
        calls.clear()
        assert readback(view, ctypes.byref(native_fpu), ctypes.byref(sr), regs,
                        ctypes.byref(pr), ctypes.byref(fpul), ctypes.byref(macl),
                        None, invoke_copy) == 0
        assert not errors
        assert native_ram == upload_ram
        assert native_mmio == upload_mmio
        assert list(regs) == list(retail_state.r)
        assert sr.value == retail_state.sr and pr.value == retail_state.pr
        assert bytes(native_fpu) == bytes(upload_fpu)
        assert fpul.value == fpul_value and macl.value == macl_value
        assert calls == [(0x8c65253e, destination, vram_address, size)]
        assert reads == [vram_address + i for i in range(0, count, 4)]
        assert native_ram[destination_offset - 4:destination_offset] == bytes([0xa5]) * 4
        assert native_ram[destination_offset:destination_offset + count] == payload[:count]
        assert native_ram[destination_offset + count:destination_offset + 164] == bytes([0xa5]) * (164 - count)
        copied_words += count // 4
        cases.append({"size": size, "vram_offset": hex(offset),
                      "fpscr_bank": hex(bank), "seed": seed,
                      "upload_mmio_writes": len(expected_device_writes),
                      "readback_mmio_reads": len(reads),
                      "roundtrip_payload_match": True,
                      "full_ram_cpu_fpu_mmio_match": True,
                      "retail_instructions_executed": upload_steps + readback_steps})

    report = {
        "module": "1ST_READ.BIN", "upload_routine": "0x8c65347c",
        "readback_routine": "0x8c653492", "cases_passed": len(cases),
        "original_instructions_executed": total_steps,
        "roundtrip_words": copied_words, "cases": cases,
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "instruction_listing": "analysis/instructions/video_setup_region.tsv",
        "instruction_listing_sha256": hashlib.sha256(listing.read_bytes()).hexdigest(),
        "upload_dll_sha256": hashlib.sha256(upload_path.read_bytes()).hexdigest(),
        "readback_dll_sha256": hashlib.sha256(readback_path.read_bytes()).hexdigest(),
        "word_copy_dll_sha256": hashlib.sha256(copy_path.read_bytes()).hexdigest(),
        "scope": "Connected native upload and readback are compared at each phase against retail execution with one persistent VRAM MMIO image. Full 16 MiB RAM, CPU/FPU, MMIO contents, write/read traces, payload bytes and destination guards are checked for 32 cases.",
        "limitations": ["VRAM is a controlled software MMIO map, not physical PVR hardware.",
                        "Rendered frame output and whole-game integration are not covered."],
    }
    (project / "analysis/engine_render_buffer_roundtrip_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    status_path = project / "project_status.json"
    status = json.loads(status_path.read_text(encoding="utf-8-sig"))
    status.setdefault("verified_progress", {})["engine_render_buffer_roundtrip"] = {
        "verification": "analysis/engine_render_buffer_roundtrip_verification.json",
        "cases": len(cases), "retail_instructions_executed": total_steps,
        "roundtrip_words": copied_words, "scope": report["scope"],
        "limitations": report["limitations"],
    }
    status_path.write_text(json.dumps(status, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} connected roundtrips passed; {total_steps} retail instructions and {copied_words} words checked")


if __name__ == "__main__":
    main()
