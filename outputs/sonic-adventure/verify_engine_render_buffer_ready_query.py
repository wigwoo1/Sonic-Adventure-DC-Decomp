"""Check the graphics readiness wrapper's result mapping and call boundary."""
import ctypes
import hashlib
import itertools
import json

from verify_fpu_cleanup_transfer import Fpu
from verify_startup import ROOT
from verify_tikal_controller import Call, P


def main():
    project = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    listing_path = project / "analysis/instructions/engine_render_buffer_ready_query.tsv"
    listing = listing_path.read_text(encoding="utf-8-sig").splitlines()
    rows = [line.split("\t") for line in listing[1:] if line]
    assert len(rows) == 18
    assert [int(row[0], 16) for row in rows] == list(range(0x8c6533e0, 0x8c653404, 2))
    for symbol in ("8c8a317c", "8c8a3178", "8c650f78"):
        assert symbol in "\n".join(listing)

    fn = ctypes.CDLL(str(ROOT / "work/sonic-adventure/build/engine_render_buffer_ready_query.dll"))
    fn = fn.sa_engine_render_buffer_ready_query
    ByteP = ctypes.POINTER(ctypes.c_uint8)
    fn.argtypes = [ByteP, ctypes.POINTER(Fpu), P, P, P, P, P,
                   ctypes.c_void_p, Call]
    fn.restype = ctypes.c_int

    cases = []
    for ready, callback_result in itertools.product((0, 1, 2, 0xffffffff),
                                                     (0, 1, 2, 0x11, 0xffffffff)):
        ram = bytearray(16 * 1024 * 1024)
        ram[0x10000:0x10000 + len(binary)] = binary
        ram[0x8a317c:0x8a3180] = ready.to_bytes(4, "little")
        argument = (0x87654321 ^ ready ^ callback_result) & 0xffffffff
        ram[0x8a3178:0x8a317c] = argument.to_bytes(4, "little")
        original_sp, original_pr = 0x8cf00000, 0x8c001234
        registers = [0x61000000 + n * 0x10101 for n in range(16)]
        registers[4], registers[15] = 0x8cabcd00, original_sp
        original_r2 = registers[2]
        regs = (ctypes.c_uint32 * 16)(*registers)
        sr = ctypes.c_uint32(0x70000021)
        pr = ctypes.c_uint32(original_pr)
        fpu = Fpu()
        fpu.fpscr = 0x200000
        fpu.banks[0][0] = 0x3f800000
        before_fpu = bytes(fpu)
        fpul, macl = ctypes.c_uint32(0xabcddcba), ctypes.c_uint32(0x31415926)
        calls = []

        @Call
        def service(_context, target, _memory, _fpu, _sr, live_regs, live_pr):
            calls.append((int(target), live_regs[4], live_pr[0]))
            assert target == 0x8c650f78
            assert live_regs[2] == 0x8c650f78 and live_regs[3] == 0x8c8a3178
            assert live_regs[4] == argument and live_pr[0] == 0x8c6533f0
            live_regs[0] = callback_result
            return 0

        result = fn((ctypes.c_uint8 * len(ram)).from_buffer(ram),
                    ctypes.byref(fpu), ctypes.byref(sr), regs, ctypes.byref(pr),
                    ctypes.byref(fpul), ctypes.byref(macl), None, service)
        expected_calls = [] if ready == 1 else [(0x8c650f78, argument, 0x8c6533f0)]
        expected = 0 if ready == 1 or callback_result == 1 else 0x11
        assert calls == expected_calls
        assert result == expected and regs[0] == expected
        assert regs[3] == (0x8c8a317c if ready == 1 else 0x8c8a3178)
        assert regs[2] == (original_r2 if ready == 1 else 0x8c650f78)
        assert regs[15] == original_sp and pr.value == original_pr
        expected_sr = 0x70000020 | int(ready == 1 or callback_result == 1)
        assert sr.value == expected_sr and bytes(fpu) == before_fpu
        assert fpul.value == 0xabcddcba and macl.value == 0x31415926
        saved = (original_sp - 4) & 0xffffff
        assert int.from_bytes(ram[saved:saved + 4], "little") == original_pr
        cases.append({"ready_global": hex(ready), "callback_result": hex(callback_result),
                      "result": hex(expected), "callback_count": len(calls),
                      "stack_and_preserved_state_checked": True})

    start, end = 0x8c6533e0, 0x8c653404
    fs, fe = start - 0x8c010000, end - 0x8c010000
    source = project / "source/sa_engine_render_buffer_ready_query.c"
    report = {
        "module": "1ST_READ.BIN", "routine": hex(start), "end_exclusive": hex(end),
        "cases_passed": len(cases), "cases": cases,
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[fs:fe]).hexdigest(),
        "source": "source/sa_engine_render_buffer_ready_query.c",
        "instruction_listing": "analysis/instructions/engine_render_buffer_ready_query.tsv",
        "instruction_listing_sha256": hashlib.sha256(listing_path.read_bytes()).hexdigest(),
        "source_sha256": hashlib.sha256(source.read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256((ROOT / "work/sonic-adventure/build/engine_render_buffer_ready_query.dll").read_bytes()).hexdigest(),
        "scope": "20 cases cover ready-global values 0, 1, 2, and 0xffffffff, five callback return classes, exact helper address/argument/PR boundary, R2/R3 effects, return mapping, SR T-bit behavior, saved PR, and unchanged FPU/FPUL/MACL.",
        "limitations": ["The helper at 0x8c650f78 is represented by a host callback; its linked-list scan and side effects are not verified here.",
                        "Wrapper contract tests do not establish graphics readiness end-to-end or game integration."]}
    (project / "analysis/engine_render_buffer_ready_query_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} graphics-ready wrapper cases passed")


if __name__ == "__main__":
    main()
