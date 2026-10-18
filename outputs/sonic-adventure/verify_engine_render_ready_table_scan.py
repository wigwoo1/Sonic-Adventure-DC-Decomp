"""Compare the renderer readiness table scan with the retail SH-4 interpreter."""
import ctypes
import hashlib
import itertools
import json
import struct

from verify_fpu_cleanup_transfer import Fpu
from verify_startup import ROOT, State, reference


def main():
    project = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    source = project / "source/sa_engine_render_ready_table_scan.c"
    dll_path = ROOT / "work/sonic-adventure/build/engine_render_ready_table_scan.dll"
    fn = ctypes.CDLL(str(dll_path)).sa_engine_render_ready_table_scan
    ByteP = ctypes.POINTER(ctypes.c_uint8)
    P = ctypes.POINTER(ctypes.c_uint32)
    fn.argtypes = [ByteP, ctypes.POINTER(Fpu), P, P, P, P, P]
    fn.restype = ctypes.c_int

    cases, total_steps = [], 0
    table = 0x8c900000
    key = 0x12345678
    stack_index = 0x00f00000
    for size, step, pattern in itertools.product(
            (0, 1, 3, 8, 15, 16, 31), (1, 2, 3, 4, 8),
            ("empty", "first_active", "first_inactive", "first_other_flags",
             "last_active", "active_other_key")):
        if size == 0 and pattern not in ("empty", "first_active"):
            continue
        ram = bytearray(16 * 1024 * 1024)
        ram[0x10000:0x10000 + len(binary)] = binary
        struct.pack_into("<HHI", ram, 0x650fe0, size, step, table)
        table_offset = table & 0xffffff
        ram[table_offset:table_offset + size + 8 * 32 + step + 8] = bytes(
            size + 8 * 32 + step + 8)
        target_cursor = ((size - 1) // step) * step if size else 0
        if pattern in ("first_active", "first_inactive", "first_other_flags"):
            offset = table_offset
            struct.pack_into("<I", ram, offset, key)
            flags = {"first_active": 1, "first_inactive": 0,
                     "first_other_flags": 4}[pattern]
            struct.pack_into("<I", ram, offset + 24, flags)
        elif pattern == "last_active" and size:
            offset = table_offset + target_cursor + 7 * 32
            struct.pack_into("<I", ram, offset, key)
            struct.pack_into("<I", ram, offset + 24, 2)
        elif pattern == "active_other_key" and size:
            struct.pack_into("<I", ram, table_offset, key ^ 0xffffffff)
            struct.pack_into("<I", ram, table_offset + 24, 3)

        initial = bytes(ram)
        retail_ram = bytearray(ram)
        state = State()
        state.r[:] = [0x11110000 + i for i in range(16)]
        state.r[4] = key
        state.r[15], state.pr, state.pc = 0x8cf00000, 0x8c000100, 0x8c650f78
        entry = State.from_buffer_copy(bytes(state))
        expected_state = State.from_buffer_copy(bytes(state))
        fpu = Fpu()
        fpu.fpscr = 0x200000
        fpu.banks[0][0] = 0x3f800000
        retail_fpu = Fpu.from_buffer_copy(bytes(fpu))
        steps = reference(retail_ram, expected_state, stop_pc=entry.pr,
                          fpu=retail_fpu, instruction_budget=2_000_000)
        total_steps += steps

        native_ram = bytearray(ram)
        view = (ctypes.c_uint8 * len(native_ram)).from_buffer(native_ram)
        regs = (ctypes.c_uint32 * 16)(*entry.r)
        sr, pr = ctypes.c_uint32(entry.sr), ctypes.c_uint32(entry.pr)
        native_fpu = Fpu.from_buffer_copy(bytes(fpu))
        fpul, macl = ctypes.c_uint32(0x76543210), ctypes.c_uint32(0x89abcdef)
        result = fn(view, ctypes.byref(native_fpu), ctypes.byref(sr), regs,
                    ctypes.byref(pr), ctypes.byref(fpul), ctypes.byref(macl))

        assert result == expected_state.r[0] == regs[0], (size, step, pattern)
        for index in (1, 3, 4, 8, 9, 10, 11, 12, 13, 14, 15):
            assert regs[index] == expected_state.r[index], (
                size, step, pattern, index, hex(regs[index]),
                hex(expected_state.r[index]))
        assert regs[2] == expected_state.r[2], (
            size, step, pattern, hex(regs[2]), hex(expected_state.r[2]))
        for index in (5, 6, 7):
            assert regs[index] == expected_state.r[index], (
                size, step, pattern, index, hex(regs[index]),
                hex(expected_state.r[index]))
        assert sr.value == expected_state.sr and pr.value == expected_state.pr, (
            size, step, pattern, hex(sr.value), hex(expected_state.sr),
            hex(pr.value), hex(expected_state.pr))
        assert bytes(native_fpu) == bytes(retail_fpu)
        assert fpul.value == 0x76543210 and macl.value == 0x89abcdef
        assert native_ram[:stack_index - 64] == retail_ram[:stack_index - 64]
        assert native_ram[stack_index:] == retail_ram[stack_index:]
        assert native_ram[table_offset:table_offset + size + 8 * 32 + step + 8] == \
            retail_ram[table_offset:table_offset + size + 8 * 32 + step + 8]
        changed = [i for i, (a, b) in enumerate(zip(initial, retail_ram)) if a != b]
        assert all(stack_index - 64 <= address < stack_index for address in changed)
        cases.append({"range_length": size, "window_step": step,
                      "pattern": pattern, "result": result,
                      "retail_instructions_executed": steps,
                      "volatile_pointer_registers_match": True,
                      "memory_matches_outside_retail_stack_saves": True})

    start, end = 0x8c650f78, 0x8c650fd4
    begin, finish = start - 0x8c010000, end - 0x8c010000
    report = {
        "module": "1ST_READ.BIN", "routine": hex(start),
        "end_exclusive": hex(end), "cases_passed": len(cases),
        "original_instructions_executed": total_steps, "cases": cases,
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[begin:finish]).hexdigest(),
        "source": "source/sa_engine_render_ready_table_scan.c",
        "source_sha256": hashlib.sha256(source.read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Compares result, volatile scan pointers r2/r5/r6/r7, preserved registers, PR/SP, SR, FPU, and RAM outside stack saves against retail execution. Exercises seven lengths, five steps, and six table patterns.",
        "limitations": ["The native function does not reproduce retail stack-save bytes; those bytes are excluded from RAM equality.",
                        "Input cases avoid 32-bit pointer wraparound, signed-boundary crossings, and a zero step with a nonempty range.",
                        "This helper-level equivalence does not prove readiness-query or game integration."],
    }
    (project / "analysis/engine_render_ready_table_scan_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} retail comparisons passed; {total_steps} original instructions executed")


if __name__ == "__main__":
    main()
