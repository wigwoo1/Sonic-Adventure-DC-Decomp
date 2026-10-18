"""Exercise the recovered renderer dispatch wrapper's callback contracts."""
import ctypes
import hashlib
import itertools
import json
from pathlib import Path

from verify_fpu_cleanup_transfer import Fpu
from verify_startup import ROOT
from verify_tikal_controller import Call, P


def main():
    project = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    listing_path = project / "analysis/instructions/engine_render_buffer_dispatch.tsv"
    listing = listing_path.read_text(encoding="utf-8-sig").splitlines()
    instructions = [line.split("\t") for line in listing[1:] if line]
    assert len(instructions) == 34
    addresses = [int(row[0], 16) for row in instructions]
    assert addresses == list(range(0x8c6536ec, 0x8c653730, 2))
    text = "\n".join(line for line in listing if line)
    for target in ("8c653f08", "8c663712", "8c663540", "8c6524d2"):
        assert target in text

    fn = ctypes.CDLL(str(ROOT / "work/sonic-adventure/build/engine_render_buffer_dispatch.dll"))
    fn = fn.sa_engine_render_buffer_dispatch
    ByteP = ctypes.POINTER(ctypes.c_uint8)
    fn.argtypes = [ByteP, ctypes.POINTER(Fpu), P, P, P, P, P,
                   ctypes.c_void_p, Call]
    fn.restype = ctypes.c_int

    cases = []
    for query_result, kind, dispatched_result in itertools.product(
            (-1, 0, 1, 3), (2, 3), (-1, 0, 2, 3)):
        ram = bytearray(16 * 1024 * 1024)
        ram[0x10000:0x10000 + len(binary)] = binary
        descriptor = 0x8cb10000
        offset = descriptor & 0xffffff
        ram[offset:offset + 24] = bytes(24)
        ram[offset:offset + 2] = kind.to_bytes(2, "little")
        ram[offset + 20:offset + 24] = (0x8cabc000).to_bytes(4, "little")
        original_sp = 0x8cf00000
        original_r13, original_r14, original_pr = 0x13579bdf, 0x2468ace0, 0x8c000123
        registers = [0x40000000 + i * 0x101 for i in range(16)]
        registers[4], registers[13], registers[14], registers[15] = (
            descriptor, original_r13, original_r14, original_sp)
        regs = (ctypes.c_uint32 * 16)(*registers)
        sr = ctypes.c_uint32(0x70000020)
        pr = ctypes.c_uint32(original_pr)
        fpu = Fpu()
        fpu.fpscr = 0x200000
        before_fpu = bytes(fpu)
        fpul, macl = ctypes.c_uint32(0x12345678), ctypes.c_uint32(0x9abcdef0)
        calls = []
        expected_dispatch = (0x8c663712 if kind == 3 else 0x8c663540)

        @Call
        def service(_context, target, _memory, _fpu, _sr, live_regs, live_pr):
            target = int(target)
            calls.append((target, live_regs[4], live_regs[5], live_pr[0]))
            if target == 0x8c653f08:
                assert live_regs[4] == descriptor
                assert live_pr[0] == 0x8c6536f6
                live_regs[0] = query_result & 0xffffffff
                live_regs[4] = descriptor
            elif target == 0x8c663712:
                assert target == expected_dispatch and live_regs[4] == descriptor
                assert live_pr[0] == 0x8c65370e
                live_regs[0] = dispatched_result & 0xffffffff
            elif target == 0x8c663540:
                assert target == expected_dispatch
                assert live_regs[4] == 0x8cabc000 and live_regs[5] == descriptor
                assert live_pr[0] == 0x8c65371a
                live_regs[0] = dispatched_result & 0xffffffff
            elif target == 0x8c6524d2:
                assert live_regs[4] == 3 and live_pr[0] == 0x8c653726
                live_regs[0] = 0xdeadbeef
            else:
                raise AssertionError(f"unexpected callback target {target:08x}")
            return 0

        result = fn((ctypes.c_uint8 * len(ram)).from_buffer(ram),
                    ctypes.byref(fpu), ctypes.byref(sr), regs, ctypes.byref(pr),
                    ctypes.byref(fpul), ctypes.byref(macl), None, service)
        expected_calls = [(0x8c653f08, descriptor, registers[5], 0x8c6536f6)]
        if query_result == 0:
            expected_calls.append((expected_dispatch,
                                   descriptor if kind == 3 else 0x8cabc000,
                                   descriptor if kind != 3 else registers[5],
                                   0x8c65370e if kind == 3 else 0x8c65371a))
            if dispatched_result == 3:
                expected_calls.append((0x8c6524d2, 3,
                                       registers[5] if kind == 3 else descriptor,
                                       0x8c653726))
        assert calls == expected_calls
        expected_result = dispatched_result if query_result == 0 else query_result
        assert result == expected_result
        assert regs[0] == (expected_result & 0xffffffff)
        assert regs[13] == original_r13 and regs[14] == original_r14
        assert regs[15] == original_sp and pr.value == original_pr
        assert sr.value == 0x70000020 and bytes(fpu) == before_fpu
        assert fpul.value == 0x12345678 and macl.value == 0x9abcdef0
        saved = {original_sp - 12: original_pr,
                 original_sp - 8: original_r13,
                 original_sp - 4: original_r14}
        for address, value in saved.items():
            assert int.from_bytes(ram[address & 0xffffff:(address & 0xffffff) + 4],
                                  "little") == value
        cases.append({"query_result": query_result, "descriptor_kind": kind,
                      "dispatch_result": dispatched_result,
                      "calls": len(calls), "stack_and_callee_saved_state_restored": True})

    start, end = 0x8c6536ec, 0x8c653730
    file_start, file_end = start - 0x8c010000, end - 0x8c010000
    source = project / "source/sa_engine_render_buffer_dispatch.c"
    report = {
        "module": "1ST_READ.BIN", "routine": hex(start),
        "end_exclusive": hex(end), "cases_passed": len(cases), "cases": cases,
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[file_start:file_end]).hexdigest(),
        "source": "source/sa_engine_render_buffer_dispatch.c",
        "instruction_listing": "analysis/instructions/engine_render_buffer_dispatch.tsv",
        "instruction_listing_sha256": hashlib.sha256(listing_path.read_bytes()).hexdigest(),
        "source_sha256": hashlib.sha256(source.read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256((ROOT / "work/sonic-adventure/build/engine_render_buffer_dispatch.dll").read_bytes()).hexdigest(),
        "scope": "36 cases cover nonzero and zero status-query returns, both descriptor branches, dispatch result 3 and non-3 values, exact retail call targets/arguments/PR boundaries, and saved stack/callee-preserved state.",
        "limitations": ["Calls at 0x8c653f08, 0x8c663712, 0x8c663540, and 0x8c6524d2 are host-controlled callbacks.",
                        "Function-level callback-contract validation does not prove retail equivalence for callee side effects or game integration."]}
    (project / "analysis/engine_render_buffer_dispatch_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} renderer-dispatch callback contract cases passed")


if __name__ == "__main__":
    main()
