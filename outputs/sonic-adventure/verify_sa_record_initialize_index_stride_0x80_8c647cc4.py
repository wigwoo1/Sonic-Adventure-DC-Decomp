import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C647CC4
LENGTH = 36
LITERAL = 0x8C647D10
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('08 44 12 d3 08 44 08 44 00 44 3c 34 55 14 66 14 77 14 f2 62 28 14 f1 51 19 14 01 e1 f2 52 2a 14 0b 00 11 14')

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'sa_record_initialize_index_stride_0x80_8c647cc4.dll'))
native = library.sa_record_initialize_index_stride_0x80_8c647cc4
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(FUNCTION)
indices = [0, 1, 2, 3, 0x7F, 0x100, 0x7FFF, 0xFFFF, 0x02000000, 0x02000001, 0x06000000] + [rng.randrange(0x10000) for _ in range(149)]
bases = [0x8C700000, 0x8C710000, 0x8CFFFFD0, LITERAL]
cases = []
for case, index_value in enumerate(indices):
    if case in (0, 1, 2):
        index_value = 0
    ram = bytearray([0xA5]) * RAM_SIZE
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = index_value
    for reg in (5, 6, 7):
        state.r[reg] = rng.getrandbits(32)
    state.r[15] = 0x8C800000 + rng.randrange(0x10000 // 4) * 4
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    table = bases[case % len(bases)] if case < len(bases) else 0x8C700000 + rng.randrange(0x10000 // 4) * 4
    literal_at = LITERAL - 0x8C000000
    ram[literal_at:literal_at + 4] = table.to_bytes(4, 'little')
    effective = (table + ((index_value << 7) & 0xFFFFFFFF)) & 0xFFFFFFFF
    if case == 0:
        state.r[15] = effective + 0x14  # stack arguments overlap the register fields being initialized
    elif case == 1:
        state.r[15] = effective + 0x1c
    elif case == 2:
        state.r[15] = effective + 0x20
    assert 0x8C000000 <= state.r[15] + 8 <= 0x8CFFFFFC
    stack_values = [rng.getrandbits(32) for _ in range(3)]
    for slot, value in enumerate(stack_values):
        at = state.r[15] + slot * 4 - 0x8C000000
        ram[at:at + 4] = value.to_bytes(4, 'little')

    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    reference(expected_ram, expected_state)

    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual_state))
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch in case {case}'
    assert actual_state.r[3] == table and actual_state.r[4] == effective
    assert actual_state.r[1] == 1 and actual_state.r[2] == expected_state.r[2]
    assert actual_state.r[15] == state.r[15] and actual_state.pc == state.pr
    cases.append({'case': case, 'index': hex(index_value), 'table': hex(table), 'record': hex(effective), 'stack': hex(state.r[15]), 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_record_initialize_index_stride_0x80_8c647cc4.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against retail SH-4 interpreter through RTS delay-slot status write',
    'scope': 'Compute base+index*0x80, write R5/R6/R7 and three stack arguments into record offsets 0x14..0x28, write active flag 1 at offset 4 in the RTS delay slot, and return.',
    'limitations': ['The wider record lifecycle and meaning of these fields are still being mapped.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'literal': hex(LITERAL), 'cases': cases,
}
(PROJECT / 'analysis' / 'sa_record_initialize_index_stride_0x80_8c647cc4_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} record initializer RAM/CPU comparisons passed')
