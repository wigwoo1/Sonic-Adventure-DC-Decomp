"""Inspect the retail service flow for startup transfer routine 0x8c011606."""
import json
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from verify_startup import ROOT, State, reference

ENTRY = 0x8c011606
FLAG = 0x8c161cac
ARG = 0x8c161ca8
SERVICE_POLL = 0x8c6033e4
SERVICE_BUFFER = 0x8c602f5c
SERVICE_METADATA = 0x8c603474
HELPER = 0x8c010fcc
SERVICE_CONFIGURE = 0x8c603508
SERVICE_READ = 0x8c0887c8
SERVICE_FINISH = 0x8c088680
RAM_SIZE = 16 * 1024 * 1024


def run(flag, result_map):
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    memory = bytearray(b"\xa5") * RAM_SIZE
    memory[0x10000:0x10000 + len(binary)] = binary
    struct.pack_into("<I", memory, FLAG & 0xffffff, flag)
    struct.pack_into("<I", memory, ARG & 0xffffff, 0x8c161ca8)
    # Seed the reported staging region with a recognizable byte pattern.
    copy_from = 0x8c6b2654 + 0x480
    for index in range(0xde0):
        memory[(copy_from + index) & 0xffffff] = (index * 37 + 11) & 0xff
    state = State()
    state.r[:] = [0x10101010 + i * 0x01010101 for i in range(16)]
    state.r[15] = 0x8cf00000
    state.sr = 0x12345678
    state.pr = 0x8c000100
    state.pc = ENTRY
    calls = []
    used = {target: 0 for target in result_map}

    def make_hook(target):
        def hook(ram, regs, sr):
            index = used[target]
            used[target] += 1
            result = result_map[target][index]
            calls.append({"target": hex(target),
                          "args": [hex(regs[i]) for i in range(4, 8)],
                          "sp": hex(regs[15]), "pr": hex(state.pr),
                          "result": result,
                          "registers": [hex(x) for x in regs],
                          "sr": hex(sr)})
            regs[0] = result & 0xffffffff
            return sr
        return hook

    steps = reference(memory, state,
                      hooks={target: make_hook(target) for target in result_map})
    return {"result": hex(state.r[0]), "steps": steps,
            "registers": [hex(x) for x in state.r], "sr": hex(state.sr),
            "pr": hex(state.pr), "pc": hex(state.pc), "calls": calls,
            "copy_sample": memory[(0x8c7989a0 & 0xffffff):
                                  (0x8c7989a0 & 0xffffff) + 16].hex(),
            "caller_stack": memory[0xefff34:0xf00000].hex(),
            "stack_changes": [(hex(address), hex(memory[address]))
                              for address in range(0xefff34, 0xf00000)
                              if memory[address] != 0xa5]}


def main():
    poll = [SERVICE_POLL]
    buffer = [SERVICE_BUFFER]
    metadata = [SERVICE_METADATA]
    helper = [HELPER]
    configure = [SERVICE_CONFIGURE]
    read = [SERVICE_READ]
    finish = [SERVICE_FINISH]
    cases = {
        "gate_off": run(1, {}),
        "helper_failure": run(0, {
            SERVICE_POLL: [0], SERVICE_BUFFER: [0], SERVICE_METADATA: [7],
            HELPER: [-1]}),
        "read_failure": run(0, {
            SERVICE_POLL: [-1, 0, -1, 3], SERVICE_BUFFER: [0],
            SERVICE_METADATA: [7], HELPER: [0], SERVICE_CONFIGURE: [0],
            SERVICE_READ: [0]}),
        "copy_success": run(0, {
            SERVICE_POLL: [-1, 0, 0], SERVICE_BUFFER: [0],
            SERVICE_METADATA: [7], HELPER: [0], SERVICE_CONFIGURE: [0],
            SERVICE_READ: [1], SERVICE_FINISH: [0]}),
    }
    print(json.dumps(cases, indent=2))


if __name__ == "__main__":
    main()
