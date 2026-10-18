import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C647D8A
LENGTH = 26
TABLE_LITERAL = 0x8C647DA4
TARGET_LITERAL = 0x8C647DA8
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('fc 7f 08 44 05 d3 08 44 05 d2 08 44 00 44 3c 34 42 2f 6c 74 08 e6 2b 42 04 7f')

target = int.from_bytes(binary[TARGET_LITERAL - BASE:TARGET_LITERAL - BASE + 4], 'little')
assert target == 0x8C10D25C
library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'sa_record_copy_eight_bytes_tail_8c647d8a.dll'))
native = library.sa_record_copy_eight_bytes_tail_8c647d8a
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(FUNCTION)
indices = [0, 1, 2, 0x7FFF, 0x02000000, 0x02000001] + [rng.randrange(0x10000) for _ in range(122)]
tables = [0x8C700000, 0x8C710000, 0x8C647DA4, 0x8C800000]
cases = []
for case, index in enumerate(indices):
    ram = bytearray([0xA5]) * RAM_SIZE
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = index
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    table = tables[case % len(tables)] if case < len(tables) else 0x8C700000 + rng.randrange(0x10000 // 4) * 4
    ram[TABLE_LITERAL - 0x8C000000:TABLE_LITERAL - 0x8C000000 + 4] = table.to_bytes(4, 'little')
    effective = (table + ((index << 7) & 0xFFFFFFFF)) & 0xFFFFFFFF
    stack_pointer = 0x8C800000 + rng.randrange(0x10000 // 4) * 4
    if case == 0:
        stack_pointer = TABLE_LITERAL + 4  # saved record base overwrites the table literal after both literals were loaded
    elif case == 1:
        stack_pointer = TARGET_LITERAL + 4  # saved record base overwrites the dispatch literal after it was loaded
    state.r[15] = stack_pointer
    assert 0x8C000004 <= state.r[15] <= 0x8CFFFFFF

    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    steps = reference(expected_ram, expected_state, stop_pc=target)
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual_state))
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch in case {case}'
    cases.append({'case': case, 'index': hex(index), 'table': hex(table), 'record': hex(effective), 'retail_steps_to_tail_target': steps, 'ram_and_cpu_match': True})

source = PROJECT / 'source' / 'sa_record_copy_eight_bytes_tail_8c647d8a.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': len(cases), 'callback_target': hex(target),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state at the retail tail-transfer boundary',
    'scope': 'Compute the indexed record address, save that base on the stack, set R4 to record+0x6c and R6 to 8, then tail-jump to the recovered byte-move helper while preserving incoming R5 and PR.',
    'limitations': ['The called byte-move helper is separately recovered; this wrapper comparison stops at its entry. Synthetic CPU/RAM states are not full-game or hardware validation.'],
    'table_literal': hex(TABLE_LITERAL), 'target_literal': hex(TARGET_LITERAL), 'cases': cases,
}
(PROJECT / 'analysis' / 'sa_record_copy_eight_bytes_tail_8c647d8a_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} record byte-copy tail-wrapper RAM/CPU comparisons passed')
