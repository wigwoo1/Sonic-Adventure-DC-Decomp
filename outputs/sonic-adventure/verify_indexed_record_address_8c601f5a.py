import ctypes
import hashlib
import json
import random
import struct
from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C601F5A
LENGTH = 12
BASE_LITERAL = 0x8C601F70
binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'indexed_record_address_8c601f5a.dll'
library = ctypes.CDLL(str(library_path))
native = library.sa_indexed_record_address_8c601f5a
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State), ctypes.POINTER(ctypes.c_uint32)]
native.restype = None
rng = random.Random(FUNCTION)
indices = [0, 1, 2, 0x7fffff, 0x800000, 0xffffffff, 0x12345678, 0x01010101]
reports = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    base = [0x8C000000, 0x8CFFFFFC, 0x8C010000, 0x8C601F50,
            0x8C601F70, 0x8C710000 + case * 4][case % 6]
    struct.pack_into('<I', ram, BASE_LITERAL - 0x8C000000, base)
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = indices[case % len(indices)] if case < 32 else rng.getrandbits(32)
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    expected_ram = bytearray(ram)
    expected = State.from_buffer_copy(state)
    expected_special = {'macl': rng.getrandbits(32)}
    reference(expected_ram, expected, special=expected_special)
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual = State.from_buffer_copy(state)
    actual_macl = ctypes.c_uint32(rng.getrandbits(32))
    native(ram_view, ctypes.byref(actual), ctypes.byref(actual_macl))
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual) == bytes(expected), (
        f'CPU mismatch in case {case}: '
        f'r={[(i, hex(actual.r[i]), hex(expected.r[i])) for i in range(16) if actual.r[i] != expected.r[i]]}, '
        f'sr={actual.sr:#x}/{expected.sr:#x}, pc={actual.pc:#x}/{expected.pc:#x}'
    )
    assert actual_macl.value == expected_special['macl'], (
        f'MACL mismatch in case {case}: {actual_macl.value:#x}/{expected_special["macl"]:#x}'
    )
    reports.append({'case': case, 'index': hex(state.r[4]), 'base': hex(base),
                    'macl': hex(actual_macl.value), 'ram_cpu_macl_match': True})

report = {
    'module': '1ST_READ.BIN',
    'load_address': hex(BASE),
    'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'binary_sha256': hashlib.sha256(binary).hexdigest(),
    'source_sha256': hashlib.sha256((ROOT / 'source' / 'sa_indexed_record_address_8c601f5a.c').read_bytes()).hexdigest(),
    'cases_passed': len(reports),
    'comparison_level': 'Complete 16 MiB RAM, all GPRs, SR, PR, return PC, and MACL compared against the retail SH-4 interpreter.',
    'scope': 'Tests the literal-loaded base, modulo-32-bit MUL.L by 0x34, MACL result, R3/R0 effects, and wrapping address addition across boundary and randomized inputs.',
    'limitations': ['The indexed record table and caller-valid index range remain unidentified.'],
    'cases': reports,
}
(ROOT / 'analysis' / 'indexed_record_address_8c601f5a_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{len(reports)} indexed record address full RAM/CPU/MACL comparisons passed")
