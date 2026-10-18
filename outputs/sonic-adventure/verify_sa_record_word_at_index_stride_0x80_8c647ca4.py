import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C647CA4
LENGTH = 16
LITERAL = 0x8C647D10
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('08 44 1a d3 08 44 08 44 00 44 3c 34 0b 00 41 50')
assert int.from_bytes(binary[LITERAL - BASE:LITERAL - BASE + 4], 'little') == 0x8C894138

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'sa_record_word_at_index_stride_0x80_8c647ca4.dll'))
native = library.sa_record_word_at_index_stride_0x80_8c647ca4
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(FUNCTION)
indices = [0, 1, 2, 3, 0x7F, 0x100, 0x7FFF, 0xFFFF, 0x02000000, 0x02000001, 0x06000000] + [rng.randrange(0x10000) for _ in range(149)]
bases = [0x8C700000, 0x8C710000, 0x8CFFFFF8, LITERAL]
cases = []
for case, index_value in enumerate(indices):
    if case == 2:
        index_value = 0  # exercise the last valid RAM word with a boundary table base
    ram = bytearray([0xA5]) * RAM_SIZE
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = index_value
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    table = bases[case % len(bases)] if case < len(bases) else (0x8C700000 + rng.randrange(0x10000 // 4) * 4)
    literal_at = LITERAL - 0x8C000000
    ram[literal_at:literal_at + 4] = table.to_bytes(4, 'little')
    entry_address = (table + ((index_value << 7) & 0xFFFFFFFF) + 4) & 0xFFFFFFFF
    assert 0x8C000000 <= entry_address <= 0x8CFFFFFC, f'fixture address out of RAM: {entry_address:08x}'
    entry_offset = entry_address - 0x8C000000
    value = rng.getrandbits(32)
    ram[entry_offset:entry_offset + 4] = value.to_bytes(4, 'little')

    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    reference(expected_ram, expected_state)

    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual_state))
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch in case {case}'
    assert actual_state.r[3] == table and actual_state.r[4] == (table + (index_value << 7)) & 0xFFFFFFFF
    assert actual_state.r[0] == value and actual_state.pc == state.pr
    cases.append({'case': case, 'index': hex(index_value), 'table': hex(table), 'entry': hex(entry_address), 'value': hex(value), 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_record_word_at_index_stride_0x80_8c647ca4.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against retail SH-4 interpreter through RTS delay-slot load',
    'scope': 'Read table base from 0x8c647d10, compute R4=base+index*0x80, load the word at R4+4 into R0 in the RTS delay slot, and return.',
    'limitations': ['The meaning and population of the indexed records are still being mapped.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'literal': hex(LITERAL), 'cases': cases,
}
(PROJECT / 'analysis' / 'sa_record_word_at_index_stride_0x80_8c647ca4_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} indexed record accessor RAM/CPU comparisons passed')
