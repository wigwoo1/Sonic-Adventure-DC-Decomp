import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
specs = [
    (0x8C10DA3C, 0x8C10DA48, 0x8C10DA4C, 0),
    (0x8C10DA50, 0x8C10DA5C, 0x8C10DA60, 1),
    (0x8C10DA64, 0x8C10DA70, 0x8C10DA74, 2),
    (0x8C10DA78, 0x8C10DA84, 0x8C10DA88, 3),
]
for function, argument_literal, slot_literal, expected_argument in specs:
    assert binary[function - BASE:function - BASE + 10] == bytes.fromhex('02 d7 03 d0 02 60 2b 40 09 00')
    assert int.from_bytes(binary[argument_literal - BASE:argument_literal - BASE + 4], 'little') == expected_argument
    assert int.from_bytes(binary[slot_literal - BASE:slot_literal - BASE + 4], 'little') == 0x8C0000B8

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'indirect_literal_tail_call_cluster.dll'))
native = library.sa_indirect_literal_tail_call_cluster
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State), ctypes.c_uint32, ctypes.c_uint32]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE, hex(address)
    return result

rng = random.Random(0x8C10DA3C)
reports = []
for function, argument_literal, slot_literal, _ in specs:
    span = binary[function - BASE:function - BASE + 10]
    cases = []
    for case in range(64):
        ram = bytearray(RAM_SIZE)
        ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
        argument = rng.getrandbits(32)
        slot = 0x8C700000 + rng.randrange(0x10000 // 4) * 4
        callback = 0x8C710000 + rng.randrange(0x10000 // 2) * 2
        ram[argument_literal - 0x8C000000:argument_literal - 0x8C000000 + 4] = argument.to_bytes(4, 'little')
        ram[slot_literal - 0x8C000000:slot_literal - 0x8C000000 + 4] = slot.to_bytes(4, 'little')
        ram[offset(slot):offset(slot) + 4] = callback.to_bytes(4, 'little')
        ram[offset(callback):offset(callback) + 2] = (0x0009).to_bytes(2, 'little')
        state = State()
        state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        state.sr = rng.getrandbits(32)
        state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
        state.pc = function
        expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
        reference(expected_ram, expected_state, stop_pc=callback)
        actual_ram = bytearray(ram)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        actual_state = State.from_buffer_copy(state)
        native(ram_view, ctypes.byref(actual_state), argument_literal, slot_literal)
        assert actual_ram == expected_ram, f'{function:08x} RAM mismatch in case {case}'
        assert bytes(actual_state) == bytes(expected_state), f'{function:08x} CPU mismatch in case {case}'
        assert actual_state.r[7] == argument and actual_state.r[0] == callback
        assert actual_state.pr == state.pr and actual_state.pc == callback
        cases.append({'case': case, 'argument': hex(argument), 'slot': hex(slot), 'callback': hex(callback), 'matched': True})
    reports.append({
        'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(function),
        'end_exclusive': hex(function + 10),
        'retail_span_sha256': hashlib.sha256(span).hexdigest(),
        'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_indirect_literal_tail_call_cluster.c').read_bytes()).hexdigest(),
        'cases_passed': len(cases),
        'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against retail SH-4 interpreter at callback entry',
        'scope': 'Load argument and callback-slot address from this wrapper’s literals, dereference callback through R0, and tail-jump while preserving PR.',
        'limitations': ['Callback body behavior is outside this ten-byte wrapper.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
        'argument_literal': hex(argument_literal), 'callback_slot_literal': hex(slot_literal), 'cases': cases,
    })

report_path = PROJECT / 'analysis' / 'indirect_literal_tail_call_cluster_verification.json'
report_path.write_text(json.dumps({'routines': reports}, indent=2) + '\n')
print(f'{sum(item["cases_passed"] for item in reports)} indirect literal tail-call comparisons passed across {len(reports)} routines')
