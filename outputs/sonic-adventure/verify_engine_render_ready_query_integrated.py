"""Compare the readiness wrapper composed with its native table scanner."""
import ctypes
import hashlib
import itertools
import json
import struct

from verify_fpu_cleanup_transfer import Fpu
from verify_startup import ROOT, State, reference
from verify_tikal_controller import Call, P


def main():
    project = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    query = ctypes.CDLL(str(ROOT / "work/sonic-adventure/build/engine_render_buffer_ready_query.dll"))
    query = query.sa_engine_render_buffer_ready_query
    scan = ctypes.CDLL(str(ROOT / "work/sonic-adventure/build/engine_render_ready_table_scan.dll"))
    scan = scan.sa_engine_render_ready_table_scan
    ByteP = ctypes.POINTER(ctypes.c_uint8)
    query.argtypes = [ByteP, ctypes.POINTER(Fpu), P, P, P, P, P,
                      ctypes.c_void_p, Call]
    query.restype = ctypes.c_int
    scan.argtypes = [ByteP, ctypes.POINTER(Fpu), P, P, P, P, P]
    scan.restype = ctypes.c_int

    cases, total_steps = [], 0
    base, key = 0x8c900000, 0x352468ac
    patterns = ("empty", "active_match", "inactive_match", "active_other_key")
    for ready, pattern in itertools.product((0, 1, 2), patterns):
        ram = bytearray(16 * 1024 * 1024)
        ram[0x10000:0x10000 + len(binary)] = binary
        struct.pack_into("<I", ram, 0x8a317c, ready)
        struct.pack_into("<I", ram, 0x8a3178, key)
        size, step = (0, 2) if pattern == "empty" and ready == 2 else (8, 2)
        struct.pack_into("<HHI", ram, 0x650fe0, size, step, base)
        table_offset = base & 0xffffff
        ram[table_offset:table_offset + size + 8 * 32 + step + 8] = bytes(
            size + 8 * 32 + step + 8)
        if size and pattern in ("active_match", "inactive_match"):
            struct.pack_into("<I", ram, table_offset, key)
            struct.pack_into("<I", ram, table_offset + 24,
                             2 if pattern == "active_match" else 0)
        elif size and pattern == "active_other_key":
            struct.pack_into("<I", ram, table_offset, key ^ 1)
            struct.pack_into("<I", ram, table_offset + 24, 3)

        state = State()
        state.r[:] = [0x22660000 + i * 0x101 for i in range(16)]
        state.r[4], state.r[15] = 0xdead0004, 0x8cf00000
        state.sr, state.pr, state.pc = 0x70000020, 0x8c000100, 0x8c6533e0
        entry = State.from_buffer_copy(bytes(state))
        expected_state = State.from_buffer_copy(bytes(state))
        fpu = Fpu()
        fpu.fpscr = 0x200000
        fpu.banks[0][0], fpu.banks[1][3] = 0x3f800000, 0xc1200000
        expected_fpu = Fpu.from_buffer_copy(bytes(fpu))
        expected_ram = bytearray(ram)
        steps = reference(expected_ram, expected_state, stop_pc=entry.pr,
                          fpu=expected_fpu, instruction_budget=250000)
        total_steps += steps

        native_ram = bytearray(ram)
        view = (ctypes.c_uint8 * len(native_ram)).from_buffer(native_ram)
        regs = (ctypes.c_uint32 * 16)(*entry.r)
        sr, pr = ctypes.c_uint32(entry.sr), ctypes.c_uint32(entry.pr)
        native_fpu = Fpu.from_buffer_copy(bytes(fpu))
        fpul, macl = ctypes.c_uint32(0x12345678), ctypes.c_uint32(0x9abcdef0)
        calls = []
        helper_fpul, helper_macl = ctypes.c_uint32(), ctypes.c_uint32()

        @Call
        def call_helper(_context, target, memory, live_fpu, live_sr,
                        live_regs, live_pr):
            calls.append((int(target), live_regs[4], live_pr[0]))
            assert target == 0x8c650f78
            assert live_pr[0] == 0x8c6533f0
            scan(memory, live_fpu, live_sr, live_regs, live_pr,
                 ctypes.byref(helper_fpul), ctypes.byref(helper_macl))
            return 0

        result = query(view, ctypes.byref(native_fpu), ctypes.byref(sr), regs,
                       ctypes.byref(pr), ctypes.byref(fpul), ctypes.byref(macl),
                       None, call_helper)
        expected_calls = [] if ready == 1 else [(0x8c650f78, key, 0x8c6533f0)]
        assert calls == expected_calls
        assert result == expected_state.r[0] == regs[0], (
            ready, pattern, result, expected_state.r[0], regs[0], calls)
        assert list(regs) == list(expected_state.r), (
            ready, pattern, [hex(x) for x in regs],
            [hex(x) for x in expected_state.r])
        assert sr.value == expected_state.sr and pr.value == expected_state.pr, (
            ready, pattern, hex(sr.value), hex(expected_state.sr),
            hex(pr.value), hex(expected_state.pr))
        assert bytes(native_fpu) == bytes(expected_fpu)
        assert fpul.value == 0x12345678 and macl.value == 0x9abcdef0
        stack_index = 0x00f00000
        assert native_ram[:stack_index - 128] == expected_ram[:stack_index - 128]
        assert native_ram[stack_index:] == expected_ram[stack_index:]
        cases.append({"ready_global": ready, "table_pattern": pattern,
                      "result": result, "helper_calls": len(calls),
                      "full_cpu_sr_pr_fpu_match": True,
                      "ram_match_outside_stack_saves": True,
                      "retail_instructions_executed": steps})

    query_report = json.loads((project / "analysis/engine_render_buffer_ready_query_verification.json").read_text())
    scan_report = json.loads((project / "analysis/engine_render_ready_table_scan_verification.json").read_text())
    report = {
        "module": "1ST_READ.BIN", "wrapper": "0x8c6533e0",
        "helper": "0x8c650f78", "cases_passed": len(cases),
        "original_instructions_executed": total_steps, "cases": cases,
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "wrapper_source_sha256": query_report["source_sha256"],
        "helper_source_sha256": scan_report["source_sha256"],
        "scope": "Runs the native wrapper with the native scanner and compares full CPU registers, SR/PR, FPU state, return values, and RAM outside retail stack-save bytes against retail execution for 12 global/table combinations.",
        "limitations": ["Retail wrapper/helper stack-save bytes are excluded from RAM equality.",
                        "This graphics-readiness path test does not verify other renderer paths or full game integration."]}
    (project / "analysis/engine_render_ready_query_integrated_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} integrated retail comparisons passed; {total_steps} instructions executed")


if __name__ == "__main__":
    main()
