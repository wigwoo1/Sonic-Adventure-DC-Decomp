import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C647D14
LENGTH = 118
LITERAL = 0x8C647DA4
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('43 67 08 47 08 47 22 d3 08 47 e6 2f 00 47 3c 37 73 64 2c 74 00 ee 73 62 e0 24 39 72 01 74 22 34 f9 8b 73 64 3c 74 73 62 e0 24 49 72 01 74 22 34 f9 8b 53 60 53 64 0c 75 73 6e 52 34 09 8d 2c 7e 40 63 38 23 05 89 04 63 01 74 52 34 30 2e f7 8f 01 7e 63 6e 63 64 0c 76 73 65 62 34 09 8d 3c 75 40 62 28 22 05 89 e4 63 01 74 62 34 30 25 f7 8f 01 75 0b 00 f6 6e')

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'sa_record_bounded_text_fields_8c647d14.dll'))
native = library.sa_record_bounded_text_fields_8c647d14
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(FUNCTION)
strings = [b'\0', b'A\0tail', b'abcdefghijkl\0tail', b'longer than twelve\0', b'\xff\x80Z\0']
cases = []
for case in range(96):
    ram = bytearray([0xA5]) * RAM_SIZE
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = [0, 1, 2, 0x7FFF, 0x02000001][case % 5]
    state.r[5] = 0x8C710000 + (case % 16) * 0x100
    state.r[6] = 0x8C720000 + (case % 16) * 0x100
    state.r[15] = 0x8C800000 + rng.randrange(0x10000 // 4) * 4
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    table = [0x8C700000, 0x8C710000, 0x8C800000, LITERAL][case % 4] if case < 4 else 0x8C700000 + rng.randrange(0x10000 // 4) * 4
    ram[LITERAL - 0x8C000000:LITERAL - 0x8C000000 + 4] = table.to_bytes(4, 'little')
    effective = (table + ((state.r[4] << 7) & 0xFFFFFFFF)) & 0xFFFFFFFF
    src1, src2 = state.r[5], state.r[6]
    if case % 11 == 0:
        src1 = effective + 0x2C  # Clearing occurs before either source is copied.
        state.r[5] = src1
    if case % 13 == 0:
        src2 = effective + 0x3C
        state.r[6] = src2
    text1 = strings[case % len(strings)] if case < 20 else (rng.randbytes(rng.randrange(0, 20)).replace(b'\0', b'X') + b'\0tail')
    text2 = strings[(case + 2) % len(strings)] if case < 20 else (rng.randbytes(rng.randrange(0, 20)).replace(b'\0', b'Y') + b'\0tail')
    if not (case % 11 == 0):
        ram[src1 - 0x8C000000:src1 - 0x8C000000 + len(text1)] = text1
    if not (case % 13 == 0):
        ram[src2 - 0x8C000000:src2 - 0x8C000000 + len(text2)] = text2

    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    reference(expected_ram, expected_state)
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual_state))
    if actual_ram != expected_ram:
        diffs = [i for i, (a, e) in enumerate(zip(actual_ram, expected_ram)) if a != e][:12]
        raise AssertionError(f'RAM mismatch in case {case}: {[(hex(i), actual_ram[i:i+8].hex(), expected_ram[i:i+8].hex()) for i in diffs]}')
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch in case {case}: actual={list(actual_state.r)} expected={list(expected_state.r)}'
    cases.append({'case': case, 'table': hex(table), 'record': hex(effective), 'ram_and_cpu_match': True})

source = PROJECT / 'source' / 'sa_record_bounded_text_fields_8c647d14.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against the retail SH-4 interpreter',
    'scope': 'Clear the two 13-byte record text fields at offsets 0x2c and 0x3c, copy up to 12 nonzero bytes from each source in order, and match scratch-register and return state.',
    'limitations': ['Synthetic RAM and CPU states are used; no Dreamcast hardware or complete-game execution is included.'],
    'literal': hex(LITERAL), 'cases': cases,
}
(PROJECT / 'analysis' / 'sa_record_bounded_text_fields_8c647d14_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f"{len(cases)} bounded-text-field RAM/CPU comparisons passed")
