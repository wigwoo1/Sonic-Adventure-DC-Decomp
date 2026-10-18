"""Trace the top-level control paths of retail input service 8c647198.

Calls outside the target are explicit no-effect/result stubs. This documents
the entry's branching and call order; it does not claim full callee recovery.
"""
import json
import struct

from verify_startup import ROOT, State, reference


PROJECT = ROOT / 'outputs/sonic-adventure'
BINARY = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C647198
BASE = 0x8C010000
LOCK_CELL = 0x8C6607F8
OBJECT_POINTER_CELL = 0x8C8A83B4
OBJECT_BASE = 0x8CA00000


def write32(ram, address, value):
    struct.pack_into('<I', ram, address & 0xFFFFFF, value & 0xFFFFFFFF)


def trace_case(name, *, lock_value, service_result=0, guard=0):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(BINARY)] = BINARY
    # Semaphore pointer literal -> semaphore word -> lock byte.
    write32(ram, 0x8C64727C, 0x8C6607F4)
    write32(ram, 0x8C6607F4, LOCK_CELL)
    ram[LOCK_CELL & 0xFFFFFF] = lock_value
    write32(ram, OBJECT_POINTER_CELL, OBJECT_BASE)
    write32(ram, 0x8C647288, 0x8C6604C0)
    write32(ram, 0x8C647284, 0x8C65F348)
    write32(ram, OBJECT_BASE + 0x10E8, guard)

    state = State()
    state.r[:] = [0x22340000 + i for i in range(16)]
    state.r[4], state.r[5] = 0x8CA20000, 0x8CA30000
    state.r[15] = 0x8CF00000
    state.sr, state.pr, state.pc = 0x70000063, 0x8C000100, FUNCTION
    seen = []
    target_results = {
        0x8C6604C0: service_result,
        0x8C646F68: 0,
        0x8C646654: 0,
        0x8C65F348: 0,
    }

    def make_hook(target):
        def hook(memory, regs, sr):
            seen.append({'target': hex(target), 'arguments': [hex(regs[i]) for i in range(4, 8)]})
            if target == 0x8C6604C0:
                regs[0] = service_result & 0xFFFFFFFF
            elif target == 0x8C646F68:
                regs[0] = 0
            elif target == 0x8C646654:
                regs[0] = 0
            return sr
        return hook

    hooks = {target: make_hook(target) for target in target_results}
    machine_trace = []
    def capture(pc, word, regs, sr, pr, fpu):
        if FUNCTION <= pc < FUNCTION + 92:
            machine_trace.append({
                'address': hex(pc), 'opcode': f'{word:04x}',
                'r0_r7': [hex(value) for value in regs[:8]],
                'r14_r15': [hex(value) for value in regs[14:16]],
                'sr': hex(sr), 'pr': hex(pr),
            })
    steps = reference(ram, state, hooks=hooks,
                      trace=capture)
    return {
        'name': name,
        'input': {'lock_byte': lock_value, 'service_result': service_result, 'object_guard': guard},
        'retail_steps': steps,
        'entry_instructions': machine_trace,
        'callee_stubs_called': seen,
        'returned_r0': hex(state.r[0]),
        'final_lock_byte': ram[LOCK_CELL & 0xFFFFFF],
        'scope': 'Retail entry instructions execute in the SH-4 subset interpreter; callees use explicit stubs. This verifies branch/call order and semaphore release, not callee behavior.',
    }


def main():
    cases = [
        trace_case('busy semaphore', lock_value=1),
        trace_case('service returns nonpositive', lock_value=0, service_result=0),
        trace_case('positive service result', lock_value=0, service_result=2),
        trace_case('positive result with cleanup flag', lock_value=0, service_result=2, guard=1),
    ]
    report = {
        'module': '1ST_READ.BIN',
        'address': hex(FUNCTION),
        'end_exclusive': hex(FUNCTION + 92),
        'binary_sha256': __import__('hashlib').sha256(BINARY).hexdigest(),
        'method': 'Retail SH-4 subset interpreter with explicit callee stubs and per-case instruction traces.',
        'cases': cases,
        'limitations': [
            'This is control-path evidence only; the service, record writer, fatal reporter, and cleanup callee are not executed by this test.',
            'The cleanup routine 0x8c646654 remains the main unresolved dependency for complete behavior verification.',
        ],
    }
    path = PROJECT / 'analysis/input_service_entry_paths.json'
    path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"{len(cases)} retail control-path traces saved to {path}")
    for case in cases:
        print(case['name'], '->', case['returned_r0'], 'calls:', ', '.join(x['target'] for x in case['callee_stubs_called']))


if __name__ == '__main__':
    main()
