"""Compare the renderer DMA/fallback setup path against retail execution."""
import ctypes
import hashlib
import json
import random

from verify_engine_cache_writeback import IO, Read, Write
from verify_fpu_cleanup_transfer import Fpu
from verify_startup import ROOT, State, reference
from verify_tikal_controller import Call, P

CacheWriteback = ctypes.CFUNCTYPE(ctypes.c_int, ctypes.c_void_p,
                                  ctypes.c_uint32)


def read_word(memory, address):
    offset = address & 0xffffff
    return int.from_bytes(memory[offset:offset + 4], "little")


def write_word(memory, address, value):
    offset = address & 0xffffff
    memory[offset:offset + 4] = (value & 0xffffffff).to_bytes(4, "little")


def read_native_word(memory, address):
    offset = address & 0xffffff
    return sum(int(memory[offset + i]) << (8 * i) for i in range(4))


def write_native_word(memory, address, value):
    offset = address & 0xffffff
    for i in range(4):
        memory[offset + i] = (value >> (8 * i)) & 0xff


def main():
    project = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    source = project / "source/sa_engine_render_buffer_dma_setup.c"
    listing = project / "analysis/instructions/video_setup_region.tsv"
    dll_path = ROOT / "work/sonic-adventure/build/engine_render_buffer_dma_setup.dll"
    fn = ctypes.CDLL(str(dll_path)).sa_engine_render_buffer_dma_setup
    ByteP = ctypes.POINTER(ctypes.c_uint8)
    IO_P = ctypes.POINTER(IO)
    fn.argtypes = [ByteP, ctypes.POINTER(Fpu), P, P, P, P, P,
                   IO_P, ctypes.c_void_p, Call, CacheWriteback]
    fn.restype = ctypes.c_int

    cases_spec = [
        # offset, main-memory source, length, SR, completion flag, busy retries
        (0x80, 0x8cb01000, 64, 0x70000000, 0, 0),
        (0x100, 0x8cb02000, 128, 0x70000000, 1, 1),
        (0x180, 0x8cb03000, 68, 0x70000000, 1, 0),
        (0x81, 0x8cb04000, 19, 0x70000000, 0, 0),
        (0x200, 0x8cb05004, 16, 0x70000000, 0, 0),
        (0x280, 0x8cb06000, 32, 0x70000020, 1, 0),
        (0x300, 0x8cb07000, 0, 0x70000000, 1, 0),
        (0x380, 0x8cb08000, 4, 0x70000000, 0, 0),
    ]
    cases, total_steps, total_dma_calls = [], 0, 0
    total_writebacks, total_copied_words = 0, 0

    for case_index, (offset, source_address, size, initial_sr,
                     completion_flag, busy_retries) in enumerate(cases_spec):
        for bank in [0, 0x200000]:
            seed = 0x6534e0 ^ case_index ^ bank
            rng = random.Random(seed)
            initial_ram = bytearray(16 * 1024 * 1024)
            initial_ram[0x10000:0x10000 + len(binary)] = binary
            source_offset = source_address & 0xffffff
            initial_ram[source_offset - 4:source_offset + 256] = bytes(
                rng.getrandbits(8) for _ in range(260))
            aperture_address = 0xa4000000 + offset
            aperture_values = {aperture_address + i: 0xa5a5a5a5
                               for i in range(-4, 164, 4)}
            initial_ram[0x8a3178:0x8a3180] = bytes(8)
            initial_ram[0x8a3000:0x8a3004] = (0xdecafbad).to_bytes(4, "little")

            entry = State()
            entry.r[:] = [rng.getrandbits(32) for _ in range(16)]
            entry.r[4], entry.r[5], entry.r[6] = offset, source_address, size
            entry.r[15] = 0x8cf00000
            entry.sr, entry.pr, entry.pc = initial_sr, 0x8c000100, 0x8c6534e0
            fpu = Fpu()
            fpu.fpscr = bank
            for b in range(2):
                for i in range(16):
                    fpu.banks[b][i] = rng.getrandbits(32)
            fpul_value, macl_value = rng.getrandbits(32), rng.getrandbits(32)

            retail_ram = bytearray(initial_ram)
            retail_mmio = dict(aperture_values)
            retail_state = State.from_buffer_copy(bytes(entry))
            retail_fpu = Fpu.from_buffer_copy(bytes(fpu))
            expected_writebacks, expected_events = [], []
            dma_calls = [0]
            result_object = 0x8c8a3000
            token = 0x13579bdf ^ case_index

            def service_hook(target):
                def hook(memory, regs, live_sr):
                    expected_events.append((target, tuple(regs), retail_state.pr))
                    if target == 0x8c650bc8:
                        dma_calls[0] += 1
                        if dma_calls[0] <= busy_retries:
                            regs[0] = 0x100
                        else:
                            src_offset = regs[5] & 0xffffff
                            for i in range(regs[6]):
                                value = int.from_bytes(memory[src_offset + i:src_offset + i + 4], "little")
                                retail_mmio[0xa4000000 + offset + i] = value
                            local_pointer = read_word(memory, regs[15])
                            write_word(memory, local_pointer, result_object)
                            write_word(memory, result_object, token)
                            regs[0] = 0
                    return live_sr
                return hook

            def cache_hook(address):
                expected_writebacks.append(address)

            hooks = {0x8c650bc8: service_hook(0x8c650bc8),
                     0x8c651000: service_hook(0x8c651000)}
            steps = reference(retail_ram, retail_state, hooks=hooks,
                              fpu=retail_fpu, mmio=retail_mmio,
                              cache_writeback=cache_hook, stop_pc=entry.pr)
            total_steps += steps

            native_ram = bytearray(initial_ram)
            native_mmio = dict(aperture_values)
            view = (ctypes.c_uint8 * len(native_ram)).from_buffer(native_ram)
            regs = (ctypes.c_uint32 * 16)(*entry.r)
            sr, pr = ctypes.c_uint32(entry.sr), ctypes.c_uint32(entry.pr)
            fpul, macl = ctypes.c_uint32(fpul_value), ctypes.c_uint32(macl_value)
            native_fpu = Fpu.from_buffer_copy(bytes(fpu))
            errors, actual_writebacks, actual_events = [], [], []
            native_dma_calls = [0]

            @Read
            def read_device(_context, address, width):
                if address not in native_mmio or width != 4:
                    errors.append(("unexpected device read", address, width))
                    return 0
                return native_mmio[address]

            @Write
            def write_device(_context, address, value, width):
                if address not in native_mmio or width != 4:
                    errors.append(("bad device write", address, width))
                    return
                native_mmio[address] = value

            io = IO(None, read_device, write_device)

            @CacheWriteback
            def cache_writeback(_context, address):
                actual_writebacks.append(address)
                return 0

            @Call
            def service(_context, target, memory, live_fpu, live_sr,
                        live_regs, live_pr):
                actual_events.append((target, tuple(live_regs[i] for i in range(16)),
                                      live_pr[0]))
                if target == 0x8c650bc8:
                    native_dma_calls[0] += 1
                    if native_dma_calls[0] <= busy_retries:
                        live_regs[0] = 0x100
                    else:
                        for i in range(live_regs[6]):
                            source = (live_regs[5] & 0xffffff) + i
                            value = int.from_bytes(bytes(memory[source:source + 4]), "little")
                            native_mmio[0xa4000000 + offset + i] = value
                        local_pointer = read_native_word(memory, live_regs[15])
                        write_native_word(memory, local_pointer, result_object)
                        write_native_word(memory, result_object, token)
                        live_regs[0] = 0
                elif target != 0x8c651000:
                    errors.append(("unexpected target", hex(target)))
                    return -9
                return 0

            assert fn(view, ctypes.byref(native_fpu), ctypes.byref(sr), regs,
                      ctypes.byref(pr), ctypes.byref(fpul), ctypes.byref(macl),
                      ctypes.byref(io), None, service, cache_writeback) == 0
            assert not errors
            assert native_ram == retail_ram
            assert native_mmio == retail_mmio
            assert list(regs) == list(retail_state.r)
            assert sr.value == retail_state.sr and pr.value == retail_state.pr
            assert bytes(native_fpu) == bytes(retail_fpu)
            assert fpul.value == fpul_value and macl.value == macl_value
            assert actual_writebacks == expected_writebacks
            assert [event[0] for event in actual_events] == [event[0] for event in expected_events]
            assert len(actual_events) == len(expected_events)
            for actual, expected in zip(actual_events, expected_events):
                if actual[0] != expected[0] or actual[2] != expected[2]:
                    print("service boundary mismatch", case_index, bank,
                          hex(actual[0]), hex(expected[0]), hex(actual[2]), hex(expected[2]))
                assert actual[0] == expected[0] and actual[2] == expected[2]
                # Services are specified to leave registers intact except for r0,
                # which carries queue status on DMA submissions.
                if actual[0] == 0x8c650bc8:
                    assert actual[1][4:8] == expected[1][4:8]
                else:
                    assert actual[1][4] == expected[1][4]

            count = (size >> 2) * 4
            expected_payload = initial_ram[source_offset:source_offset + count]
            assert b"".join(retail_mmio[aperture_address + i].to_bytes(4, "little")
                            for i in range(0, count, 4)) == expected_payload
            total_dma_calls += dma_calls[0]
            total_writebacks += len(expected_writebacks)
            total_copied_words += count // 4
            cases.append({"aperture_offset": hex(offset),
                          "ram_source": hex(source_address), "size": size,
                          "sr": hex(initial_sr), "completion_flag": completion_flag,
                          "fpscr_bank": hex(bank), "dma_submissions": dma_calls[0],
                          "cache_writebacks": len(expected_writebacks),
                          "full_ram_cpu_fpu_mmio_match": True,
                          "retail_instructions_executed": steps})

    start, end = 0x8c6534e0, 0x8c6535c2
    begin, finish = start - 0x8c010000, end - 0x8c010000
    report = {
        "module": "1ST_READ.BIN", "routine": hex(start),
        "end_exclusive": hex(end), "cases_passed": len(cases),
        "original_instructions_executed": total_steps,
        "dma_submissions": total_dma_calls,
        "cache_writebacks": total_writebacks,
        "device_words_processed": total_copied_words,
        "cases": cases,
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[begin:finish]).hexdigest(),
        "source": "source/sa_engine_render_buffer_dma_setup.c",
        "instruction_listing": "analysis/instructions/video_setup_region.tsv",
        "instruction_listing_sha256": hashlib.sha256(listing.read_bytes()).hexdigest(),
        "source_sha256": hashlib.sha256(source.read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Compares complete RAM/CPU/FPU/device state through aligned DMA, busy retry, completion notification, nonzero remainder copy, unaligned fallback and masked-SR fallback. DMA, completion, aperture and cache events are controlled host callbacks.",
        "limitations": ["The controlled DMA service models a PVR register aperture and completion token; physical PVR behavior is not verified.",
                        "Sub-word copy lengths below four and pathological cache-range wraparound inputs are excluded.",
                        "Rendered output and whole-game integration remain unverified."],
    }
    (project / "analysis/engine_render_buffer_dma_setup_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} DMA/fallback comparisons passed; {total_steps} retail instructions, {total_dma_calls} DMA calls, {total_writebacks} cache writebacks")


if __name__ == "__main__":
    main()
