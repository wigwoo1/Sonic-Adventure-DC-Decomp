"""Trace retail call boundaries for the startup retry helper while recovering it."""
import json
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from verify_startup import ROOT, State, reference

BASE = 0x8c010000
ENTRY = 0x8c010fcc
FLAG = 0x8c161cac
SERVICE_A = 0x8c602fb8
SERVICE_B = 0x8c10d3c8
SERVICE_C = 0x8c602fe6
RAM_SIZE = 16 * 1024 * 1024


def trace(results, flag=0):
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    memory = bytearray(b"\xa5") * RAM_SIZE
    memory[0x10000:0x10000 + len(binary)] = binary
    struct.pack_into("<I", memory, FLAG & 0xffffff, flag)
    state = State()
    state.r[:] = [0x10101010 + i * 0x01010101 for i in range(16)]
    state.r[4] = 0x8c161ca8
    state.r[5] = 0x8c161cb0
    state.r[15] = 0x8cf00000
    state.sr = 0x12345678
    state.pr = 0x8c000100
    state.pc = ENTRY
    calls = []
    used = {SERVICE_A: 0, SERVICE_B: 0, SERVICE_C: 0}

    def make_hook(target):
        def hook(ram, registers, sr):
            index = used[target]
            used[target] += 1
            result = results[target][index]
            calls.append({"target": hex(target), "args": [hex(registers[i])
                           for i in range(4, 8)], "sp": hex(registers[15]),
                          "pr": hex(state.pr), "result": result,
                          "registers": [hex(x) for x in registers],
                          "sr": hex(sr)})
            registers[0] = result & 0xffffffff
            return sr
        return hook

    steps = reference(memory, state,
                      hooks={target: make_hook(target) for target in used})
    return {"result": hex(state.r[0]), "steps": steps,
            "final_registers": [hex(x) for x in state.r],
            "final_sr": hex(state.sr), "final_pr": hex(state.pr),
            "final_pc": hex(state.pc), "calls": calls}


def main():
    scenarios = {
        "gate_off": trace({}, flag=1),
        "initial_a_negative": trace({SERVICE_A: [-1], SERVICE_B: [0],
                                      SERVICE_C: []}),
        "initial_b_zero": trace({SERVICE_A: [0], SERVICE_B: [0],
                                 SERVICE_C: []}),
        "retry_then_success": trace({SERVICE_A: [0], SERVICE_B: [1, 0],
                                     SERVICE_C: [0]}),
        "retry_then_b_nonzero_negative": trace({SERVICE_A: [2],
                                                SERVICE_B: [1, -7, 5],
                                                SERVICE_C: [0, -1]}),
        "retry_c_negative": trace({SERVICE_A: [2], SERVICE_B: [1, 1],
                                   SERVICE_C: [-1]}),
    }
    print(json.dumps({name: {"result": case["result"],
                             "calls": case["calls"],
                             "final_registers": case["final_registers"],
                             "final_sr": case["final_sr"]}
                      for name, case in scenarios.items()}, indent=2))


if __name__ == "__main__":
    main()
