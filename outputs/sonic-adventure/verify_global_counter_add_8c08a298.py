import ctypes
import hashlib
import json
import random
import struct
from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C08A298
LENGTH = 10
POINTER_LITERAL = 0x8C08A34C
binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'global_counter_add_8c08a298.dll'
library = ctypes.CDLL(str(library_path))
native = library.sa_global_counter_add_8c08a298
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None
rng = random.Random(FUNCTION)
targets = [0x8C730000, POINTER_LITERAL, FUNCTION, 0x8C000000, 0x8CFFFFFC,
           0x8C720000, 0x8C08A2A0]
deltas = [0, 1, 0xffffffff, 0x7fffffff, 0x80000000, 0x10000]
reports = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    target = targets[case % len(targets)] if case < len(targets) else 0x8C740000 + (case - len(targets)) * 4
    target &= 0xffffffff
    offset = (target & 0x1fffffff) - 0x0c000000
    assert 0 <= offset <= RAM_SIZE - 4
    if not BASE <= target < BASE + len(binary) and target != POINTER_LITERAL:
        struct.pack_into('<I', ram, offset, rng.getrandbits(32))
    struct.pack_into('<I', ram, POINTER_LITERAL - 0x8C000000, target)
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = deltas[case % len(deltas)] if case < 24 else rng.getrandbits(32)
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    expected_ram = bytearray(ram)
    expected = State.from_buffer_copy(state)
    reference(expected_ram, expected)
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual))
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual) == bytes(expected), (
        f'CPU mismatch in case {case}: '
        f'r={[(i, hex(actual.r[i]), hex(expected.r[i])) for i in range(16) if actual.r[i] != expected.r[i]]}, '
        f'sr={actual.sr:#x}/{expected.sr:#x}, pc={actual.pc:#x}/{expected.pc:#x}'
    )
    reports.append({'case': case, 'target': hex(target), 'delta': hex(state.r[4]), 'ram_and_cpu_match': True})

report = {
    'module': '1ST_READ.BIN',
    'load_address': hex(BASE),
    'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'binary_sha256': hashlib.sha256(binary).hexdigest(),
    'source_sha256': hashlib.sha256((ROOT / 'source' / 'sa_global_counter_add_8c08a298.c').read_bytes()).hexdigest(),
    'cases_passed': len(reports),
    'comparison_level': 'Complete 16 MiB RAM and all GPRs, SR, PR, and return PC compared against the retail SH-4 interpreter.',
    'scope': 'Tests the PC-relative pointer load, unsigned wrapping addition, scratch register results, RTS-delay-slot store, self-pointer and code aliases, and first/last aligned RAM words.',
    'limitations': ['The counter global’s higher-level meaning and valid arithmetic range remain unidentified.'],
    'cases': reports,
}
(ROOT / 'analysis' / 'global_counter_add_8c08a298_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{len(reports)} global counter add full RAM/CPU comparisons passed")
