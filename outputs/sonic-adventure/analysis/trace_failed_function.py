"""Trace a failed-export 1ST_READ.BIN routine using the retail interpreter."""
import ctypes
import inspect
import json
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from verify_startup import ROOT, State, reference
from verify_fpu_cleanup_transfer import Fpu

ENTRY = 0x8c0a966c
END = 0x8c0a976a
RAM_SIZE = 16 * 1024 * 1024


def main():
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    binary_base = 0x8c010000
    ram = bytearray(b"\xa5") * RAM_SIZE
    ram[0x10000:0x10000 + len(binary)] = binary
    state = State()
    state.r[:] = [0x8c100000 + i * 0x100 for i in range(16)]
    state.r[4], state.r[5], state.r[6], state.r[7] = (
        0x8c100000, 0x8c110000, 0x8c120000, 0x8c130000)
    state.r[15] = 0x8cf00000
    state.sr = 0
    state.pr = 0x8c000100
    state.pc = ENTRY
    fpu = Fpu()
    fpu.fpscr = 0
    fpu.fpul = 0
    hooks = {}
    boundaries = []
    trace = []
    def indirect_boundary(memory, regs, sr):
        boundaries.append({"target": hex(0x8c0aff1a), "pr": hex(state.pr),
                           "sp": hex(regs[15]),
                           "args": [hex(regs[i]) for i in range(4, 8)]})
        regs[0] = 0
        return sr
    hooks[0x8c0aff1a] = indirect_boundary
    lines, first_line = inspect.getsourcelines(reference)
    instruction_line = first_line + next(
        i for i, line in enumerate(lines) if "n = (word >> 8)" in line)

    def trace_line(frame, event, arg):
        if frame.f_code is not reference.__code__ or event != "line":
            return trace_line
        local = frame.f_locals
        pc = local.get("pc")
        # Treat the indirect call reached from the candidate body as a service
        # boundary.  The instruction interpreter has only an integer subset;
        # executing this unrelated callee with synthetic pointers is invalid.
        if frame.f_lineno == instruction_line and pc is not None:
            trace.append({"pc": hex(pc), "word": hex(local["word"]),
                          "regs": [hex(x) for x in local["regs"]],
                          "sr": hex(local["sr"]),
                          "pr": hex(state.pr)})
        return trace_line

    sys.settrace(trace_line)
    error = None
    try:
        steps = reference(ram, state, hooks=hooks, fpu=fpu,
                          instruction_budget=500000)
    except Exception as exc:  # Preserve the first unsupported/faulting instruction.
        error = f"{type(exc).__name__}: {exc}"
        steps = len(trace)
    finally:
        sys.settrace(None)

    xrefs = [binary_base + offset for offset in range(0, len(binary) - 3, 4)
             if struct.unpack_from("<I", binary, offset)[0] == ENTRY]
    report = {"entry": hex(ENTRY), "ghidra_body_bytes": 254,
              "end_exclusive": hex(END),
              "aligned_literal_xrefs": [hex(xref) for xref in xrefs],
              "xref_context_words": {
                  hex(xref): [hex(struct.unpack_from("<I", binary, offset)[0])
                              for offset in range(xref - binary_base - 16,
                                                  xref - binary_base + 20, 4)]
                  for xref in xrefs},
              "observed_block_starts": [
                  hex(int(item["pc"], 16)) for index, item in enumerate(trace)
                  if index == 0 or int(item["pc"], 16) !=
                  int(trace[index - 1]["pc"], 16) + 2],
              "fixture": "synthetic registers and RAM; control-flow diagnostic only",
              "service_boundaries": "only indirect target 0x8c0aff1a was modeled",
              "recovery_status": "not recovered; trace does not prove semantic equivalence",
              "trace_entries": len(trace), "interpreter_steps": steps,
              "error": error, "trace": trace, "boundaries": boundaries,
              "final_registers": [hex(x) for x in state.r],
              "final_sr": hex(state.sr), "final_pr": hex(state.pr),
              "final_pc": hex(state.pc)}
    output = ROOT / "outputs/sonic-adventure/analysis/failed_0a966c_trace.json"
    output.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"Traced {len(trace)} retail instructions; error={error}; report={output}")


if __name__ == "__main__":
    main()
