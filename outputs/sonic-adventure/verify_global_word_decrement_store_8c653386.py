import ctypes
import hashlib
import json
import random
import struct
from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C653386
LENGTH = 10
POINTER_LITERAL = 0x8C6533D0
binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'global_word_decrement_store_8c653386.dll'
library = ctypes.CDLL(str(library_path))
native = library.sa_global_word_decrement_store_8c653386
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None
rng = random.Random(FUNCTION)
pointers = [0x8C800000, 0x8C6533C8, FUNCTION, 0x8C000000,
            0x8CFFFFFC, 0x8C700000, 0x8C6533D0]
values = [0, 1, 2, 0xffffffff, 0x80000000, 0x7fffffff]
reports = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    pointer = pointers[case % len(pointers)] if case < len(pointers) else 0x8C710000 + (case - len(pointers)) * 4
    target = pointer
    target_offset = (target & 0x1fffffff) - 0x0c000000
    assert 0 <= target_offset <= RAM_SIZE - 4, (case, hex(pointer), hex(target))
    if not BASE <= target < BASE + len(binary) and target != POINTER_LITERAL:
        struct.pack_into('<I', ram, target_offset, rng.getrandbits(32))
    struct.pack_into('<I', ram, POINTER_LITERAL - 0x8C000000, pointer)
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = values[case % len(values)] if case < 36 else rng.getrandbits(32)
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
    if actual_ram != expected_ram:
        at = next(i for i, (left, right) in enumerate(zip(actual_ram, expected_ram)) if left != right)
        raise AssertionError(f'RAM mismatch case={case} pointer={pointer:#x} target={target:#x} offset={at:#x} actual={actual_ram[at:at+8].hex()} expected={expected_ram[at:at+8].hex()} regs={hex(actual.r[3])}/{hex(actual.r[4])}/{hex(actual.r[0])}')
    assert bytes(actual) == bytes(expected), (
        f'CPU mismatch in case {case}: '
        f'r={[(i, hex(actual.r[i]), hex(expected.r[i])) for i in range(16) if actual.r[i] != expected.r[i]]}, '
        f'sr={actual.sr:#x}/{expected.sr:#x}, pc={actual.pc:#x}/{expected.pc:#x}'
    )
    reports.append({'case': case, 'pointer': hex(pointer), 'target': hex(target),
                    'input': hex(state.r[4]), 'ram_and_cpu_match': True})

report = {
    'module': '1ST_READ.BIN',
    'load_address': hex(BASE),
    'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'binary_sha256': hashlib.sha256(binary).hexdigest(),
    'source_sha256': hashlib.sha256((ROOT / 'source' / 'sa_global_word_decrement_store_8c653386.c').read_bytes()).hexdigest(),
    'cases_passed': len(reports),
    'comparison_level': 'Complete 16 MiB RAM and all GPRs, SR, PR, and return PC compared against the retail SH-4 interpreter.',
    'scope': 'Tests the PC-relative pointer, direct destination through R3, modulo-32-bit decrement, R3/R4/R0 results, code/literal aliases, and first/last aligned RAM words.',
    'limitations': ['The destination field purpose and runtime-valid values remain unidentified.'],
    'cases': reports,
}
(ROOT / 'analysis' / 'global_word_decrement_store_8c653386_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{len(reports)} decrement/store full RAM/CPU comparisons passed")
