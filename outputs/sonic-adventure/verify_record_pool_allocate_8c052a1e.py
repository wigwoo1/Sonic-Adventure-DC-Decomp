import ctypes
import hashlib
import json
import random
import struct
from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C052A1E
LENGTH = 64
BITMAP_LITERAL = 0x8C052B60
POOL_LITERAL = 0x8C052B64
binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'record_pool_allocate_8c052a1e.dll'
library = ctypes.CDLL(str(library_path))
native = library.sa_record_pool_allocate_8c052a1e
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None
rng = random.Random(FUNCTION)
cases = []
for case in range(80):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    bitmap = 0x8C740000 + case * 0x200
    pool = 0x8C760000 + case * 0x2000
    if case % 4 == 0:
        used = 0
    elif case % 4 == 1:
        used = 255
    elif case % 4 == 2:
        used = rng.randrange(1, 256)
    else:
        used = 256
    for index in range(256):
        ram[(bitmap - 0x8C000000) + index] = 1 if index >= 256 - used else 0
    for index in range(256):
        record = pool + index * 0x34
        for offset in (0, 4, 0x2c, 0x30):
            struct.pack_into('<I', ram, record - 0x8C000000 + offset, rng.getrandbits(32))
    struct.pack_into('<I', ram, BITMAP_LITERAL - 0x8C000000, bitmap)
    struct.pack_into('<I', ram, POOL_LITERAL - 0x8C000000, pool)
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
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
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}, used={used}'
    assert bytes(actual) == bytes(expected), f'CPU mismatch in case {case}, used={used}: r={[(i, hex(actual.r[i]), hex(expected.r[i])) for i in range(16) if actual.r[i] != expected.r[i]]}, sr={actual.sr:#x}/{expected.sr:#x}, pr={actual.pr:#x}/{expected.pr:#x}, pc={actual.pc:#x}/{expected.pc:#x}'
    cases.append({'case': case, 'used_slots': used, 'allocated_pointer': hex(actual.r[0]),
                  'ram_and_cpu_match': True})

source_path = ROOT / 'source' / 'sa_record_pool_allocate_8c052a1e.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE),
    'address': hex(FUNCTION), 'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source_path.read_bytes()).hexdigest(),
    'cases_passed': len(cases), 'comparison_level': 'full RAM and GPR/SR/PR/PC',
    'scope': 'Exact 64-byte retail first-free allocation routine; 80 full interpreter/native comparisons covering empty, partially occupied and full 256-slot pools with randomized untouched fields.',
    'limitations': ['Pool and bitmap are relocated from image literals; higher-level record ownership and release behavior are not covered.'],
    'cases': cases,
}
(ROOT / 'analysis' / 'record_pool_allocate_8c052a1e_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'Record pool allocator at {FUNCTION:#x}: {len(cases)} full RAM/CPU cases passed')
