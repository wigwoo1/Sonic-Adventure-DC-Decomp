import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C10D444
LENGTH = 20
RAM_SIZE = 16 * 1024 * 1024
GLOBAL_POINTER = 0x8C5FBB14
TABLE_OFFSET = 0xDC
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('05 d3 08 44 06 92 32 60 0c 32 2c 34 42 61 12 25 0b 00 00 e0')
assert int.from_bytes(binary[0x8C10D458 - BASE:0x8C10D458 - BASE + 2], 'little') == TABLE_OFFSET
assert int.from_bytes(binary[0x8C10D45C - BASE:0x8C10D45C - BASE + 4], 'little') == GLOBAL_POINTER

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'read_relative_table_word_8c10d444.dll'))
native = library.sa_read_relative_table_word_8c10d444
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE, hex(address)
    return result

rng = random.Random(FUNCTION)
indices = [0, 1, 2, 0x3F, 0xFF] + [rng.randrange(0x100) for _ in range(91)]
cases = []
for case, table_index in enumerate(indices):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    table_base = 0x8C700000 + rng.randrange(0x80000 // 4) * 4
    pointer_at = offset(GLOBAL_POINTER)
    ram[pointer_at:pointer_at + 4] = table_base.to_bytes(4, 'little')
    source_address = table_base + TABLE_OFFSET + table_index * 4
    source_at = offset(source_address)
    expected_value = rng.getrandbits(32)
    ram[source_at:source_at + 4] = expected_value.to_bytes(4, 'little')
    destination = 0x8C200000 + rng.randrange(0x80000 // 4) * 4
    dest_at = offset(destination)
    ram[dest_at:dest_at + 4] = rng.randbytes(4)
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = table_index
    state.r[5] = destination
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    reference(expected_ram, expected_state)
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual_state))
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch in case {case}'
    assert int.from_bytes(actual_ram[dest_at:dest_at + 4], 'little') == expected_value
    cases.append({'case': case, 'table_index': table_index, 'result': hex(expected_value), 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_read_relative_table_word_8c10d444.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against the retail SH-4 interpreter',
    'scope': 'Exact relative table lookup, index scaling, 0xdc fixed offset, output store, literal values, and zero return.',
    'limitations': ['The table’s higher-level semantic identity remains unknown.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'global_pointer': hex(GLOBAL_POINTER), 'table_offset': hex(TABLE_OFFSET), 'cases': cases,
}
(PROJECT / 'analysis' / 'read_relative_table_word_8c10d444_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} relative table lookup 0xdc RAM/CPU comparisons passed')
