import ctypes
import hashlib
import json
import random
import struct
from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C05F458
LENGTH = 64
TABLE_LITERAL = 0x8C05F500
binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'slot_list_available_8c05f458.dll'
library = ctypes.CDLL(str(library_path))
native = library.sa_slot_list_available_8c05f458
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None
rng = random.Random(FUNCTION)
cases = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    entries = 1 + case % 12
    cursor = 0x8C740000 + case * 0x40
    table = 0x8C760000 + case * 0x400
    values = []
    for index in range(entries):
        slot = rng.randrange(0x200)
        flagged = bool(rng.getrandbits(1))
        values.append(slot | (0x4000 if flagged else 0))
        occupied = rng.randrange(2)
        ram[(table - 0x8C000000) + slot] = occupied
    struct.pack_into('<' + 'h' * (entries + 1), ram, cursor - 0x8C000000,
                     *values, -1)
    struct.pack_into('<I', ram, TABLE_LITERAL - 0x8C000000, table)
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = cursor
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION

    expected_ram = bytearray(ram)
    expected = State.from_buffer_copy(state)
    reference(expected_ram, expected, instruction_budget=10000)
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual))
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual) == bytes(expected), (
        f'CPU mismatch in case {case}: '
        f'r={[(i, hex(actual.r[i]), hex(expected.r[i])) for i in range(16) if actual.r[i] != expected.r[i]]}, '
        f'sr={actual.sr:#x}/{expected.sr:#x}, pr={actual.pr:#x}/{expected.pr:#x}, '
        f'pc={actual.pc:#x}/{expected.pc:#x}')
    cases.append({'case': case, 'entry_count': entries, 'returned_available': actual.r[0],
                  'ram_and_cpu_match': True})

source_path = ROOT / 'source' / 'sa_slot_list_available_8c05f458.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE),
    'address': hex(FUNCTION), 'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source_path.read_bytes()).hexdigest(),
    'cases_passed': len(cases), 'comparison_level': 'full RAM and GPR/SR/PR/PC',
    'scope': 'Exact 64-byte retail slot-list availability check; 128 interpreter/native comparisons over varied sentinel-terminated lists and occupancy states.',
    'limitations': ['The allocator/slot lifecycle callers and list validity constraints remain unidentified.'],
    'cases': cases,
}
(ROOT / 'analysis' / 'slot_list_available_8c05f458_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'Slot list availability at {FUNCTION:#x}: {len(cases)} full RAM/CPU cases passed')
