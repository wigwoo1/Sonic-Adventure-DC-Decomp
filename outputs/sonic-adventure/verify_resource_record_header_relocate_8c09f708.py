import ctypes
import hashlib
import json
import random
import struct
from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C09F708
LENGTH = 52
binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'resource_record_header_relocate_8c09f708.dll'
library = ctypes.CDLL(str(library_path))
native = library.sa_resource_record_header_relocate_8c09f708
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None
rng = random.Random(FUNCTION)
cases = []
record_counts = [0, 1, 2, 3, 8, 17, 32, 64]
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    count = record_counts[case % len(record_counts)]
    start = 0x8C740000 + case * 0x1000
    end = start + count * 0x34
    base = [0, 1, 0x8C010000, 0x7fffffff, 0xffffffff, rng.getrandbits(32)][case % 6]
    for index in range(count):
        for field in range(13):
            value = [0, 1, 0xffffffff, rng.getrandbits(32)][(case + index * 3 + field) % 4]
            struct.pack_into('<I', ram, start - 0x8C000000 + index * 0x34 + field * 4, value)
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4], state.r[5], state.r[6] = base, start, end
    if case % 19 == 0:
        state.r[5], state.r[6] = end, start
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
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}, records={count}'
    assert bytes(actual) == bytes(expected), (
        f'CPU mismatch in case {case}: '
        f'r={[(i, hex(actual.r[i]), hex(expected.r[i])) for i in range(16) if actual.r[i] != expected.r[i]]}, '
        f'sr={actual.sr:#x}/{expected.sr:#x}, pr={actual.pr:#x}/{expected.pr:#x}, '
        f'pc={actual.pc:#x}/{expected.pc:#x}')
    cases.append({'case': case, 'record_count': count, 'base': hex(base),
                  'ram_and_cpu_match': True})

source_path = ROOT / 'source' / 'sa_resource_record_header_relocate_8c09f708.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE),
    'address': hex(FUNCTION), 'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source_path.read_bytes()).hexdigest(),
    'cases_passed': len(cases), 'comparison_level': 'full RAM and GPR/SR/PR/PC',
    'scope': 'Exact 52-byte retail 0x34-byte resource-record relocation routine; 128 complete interpreter/native comparisons over 0–64 records, sparse pointer fields, empty/reversed ranges, and wrapping relocation bases.',
    'limitations': ['The resource record schema and pointer-field semantics remain under analysis.'],
    'cases': cases,
}
(ROOT / 'analysis' / 'resource_record_header_relocate_8c09f708_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'Resource record header relocation at {FUNCTION:#x}: {len(cases)} full RAM/CPU cases passed')
