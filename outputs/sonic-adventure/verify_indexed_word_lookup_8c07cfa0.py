import ctypes
import hashlib
import json
import random
import struct
from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C07CFA0
LENGTH = 10
LITERAL = 0x8C07D040
binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'indexed_word_lookup_8c07cfa0.dll'
library = ctypes.CDLL(str(library_path))
native = library.sa_indexed_word_lookup_8c07cfa0
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None
rng = random.Random(FUNCTION)
reports = []
for case in range(512):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    index_byte = case & 0xff if case < 256 else rng.getrandbits(8)
    if case < 256:
        table = [0x8C000000, 0x8CFFFC00][case // 128]
    elif case % 4 == 0:
        table = 0x8C010000
    elif case % 4 == 1:
        table = FUNCTION + 0x20
    elif case % 4 == 2:
        table = 0x8C710000 + (case - 256) * 0x100
    else:
        table = 0x8C422B44
    struct.pack_into('<I', ram, LITERAL - 0x8C000000, table)
    result_address = (table + (index_byte * 4)) & 0xffffffff
    result_offset = (result_address & 0x1fffffff) - 0x0c000000
    assert 0 <= result_offset <= RAM_SIZE - 4, (case, hex(table), signed_index, hex(result_address))
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = ((rng.getrandbits(24) << 8) | index_byte) if case < 256 else rng.getrandbits(32)
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
    reports.append({'case': case, 'index_byte': hex(index_byte), 'table': hex(table),
                    'result_address': hex(result_address), 'ram_and_cpu_match': True})

report = {
    'module': '1ST_READ.BIN',
    'load_address': hex(BASE),
    'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'binary_sha256': hashlib.sha256(binary).hexdigest(),
    'source_sha256': hashlib.sha256((ROOT / 'source' / 'sa_indexed_word_lookup_8c07cfa0.c').read_bytes()).hexdigest(),
    'cases_passed': len(reports),
    'comparison_level': 'Complete 16 MiB RAM and all GPRs, SR, PR, and return PC compared against the retail SH-4 interpreter.',
    'scope': 'Tests all 256 zero-extended input byte values, high-bit truncation, unchanged R4, R1/R0 results, table literal handling, code-backed reads, and first/last aligned RAM words.',
    'limitations': ['The table purpose and legal index range are not yet identified; boundary tests keep effective addresses inside mapped RAM.'],
    'cases': reports,
}
(ROOT / 'analysis' / 'indexed_word_lookup_8c07cfa0_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{len(reports)} indexed word lookup full RAM/CPU comparisons passed")
