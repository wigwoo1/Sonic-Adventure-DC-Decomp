import ctypes
import hashlib
import json
import random
import struct
from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C10BA12
LENGTH = 12
VALUE_LITERAL = 0x8C10BA38
HALFWORD_POINTER_LITERAL = 0x8C10BA40
WORD_POINTER_LITERAL = 0x8C10BA44
binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'global_pair_initialize_8c10ba12.dll'
library = ctypes.CDLL(str(library_path))
native = library.sa_global_pair_initialize_8c10ba12
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None
rng = random.Random(FUNCTION)
halfword_targets = [0x8C720000, HALFWORD_POINTER_LITERAL, 0x8C000000,
                    0x8CFFFFFE, 0x8C10BA0C, 0x8C730004]
word_targets = [0x8C720004, WORD_POINTER_LITERAL, 0x8C000000,
                0x8CFFFFFC, 0x8C10BA0C, 0x8C730000]
reports = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    halfword_target = halfword_targets[case % len(halfword_targets)] if case < 12 else 0x8C740000 + (case - 12) * 4
    word_target = word_targets[(case // 2) % len(word_targets)] if case < 24 else 0x8C750000 + (case - 24) * 4
    value = [0, 0xffffffff, 0x12345678, 0x53454741][case % 4] if case < 16 else rng.getrandbits(32)
    for address in set([halfword_target, word_target]):
        offset = (address & 0x1fffffff) - 0x0c000000
        if 0 <= offset <= RAM_SIZE - 4 and not BASE <= address < BASE + len(binary):
            struct.pack_into('<I', ram, offset, rng.getrandbits(32))
    struct.pack_into('<I', ram, VALUE_LITERAL - 0x8C000000, value)
    struct.pack_into('<I', ram, HALFWORD_POINTER_LITERAL - 0x8C000000, halfword_target)
    struct.pack_into('<I', ram, WORD_POINTER_LITERAL - 0x8C000000, word_target)
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = rng.getrandbits(32)
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
    reports.append({'case': case, 'halfword_target': hex(halfword_target),
                    'word_target': hex(word_target), 'ram_and_cpu_match': True})

report = {
    'module': '1ST_READ.BIN',
    'load_address': hex(BASE),
    'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'binary_sha256': hashlib.sha256(binary).hexdigest(),
    'source_sha256': hashlib.sha256((ROOT / 'source' / 'sa_global_pair_initialize_8c10ba12.c').read_bytes()).hexdigest(),
    'cases_passed': len(reports),
    'comparison_level': 'Complete 16 MiB RAM and all GPRs, SR, PR, and return PC compared against the retail SH-4 interpreter.',
    'scope': 'Tests both literal-loaded target pointers, the literal word value, ordered halfword and longword stores, overlapping/code/literal aliases, and RAM boundaries.',
    'limitations': ['The two initialized globals and their runtime semantics remain unidentified.'],
    'cases': reports,
}
(ROOT / 'analysis' / 'global_pair_initialize_8c10ba12_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{len(reports)} global pair initializer full RAM/CPU comparisons passed")
