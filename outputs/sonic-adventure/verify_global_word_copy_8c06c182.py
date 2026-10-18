import ctypes
import hashlib
import json
import random
import struct
from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C06C182
LENGTH = 10
SOURCE_LITERAL = 0x8C06C214
DESTINATION_LITERAL = 0x8C06C204
binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'global_word_copy_8c06c182.dll'
library = ctypes.CDLL(str(library_path))
native = library.sa_global_word_copy_8c06c182
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None
rng = random.Random(FUNCTION)
reports = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    source = [0x8C710000 + case * 16, 0x8C06C214, 0x8C06C182,
              0x8C000000, 0x8CFFFFFC, 0x8C700000 + case * 16][case % 6]
    destination = [0x8C720000 + case * 16, 0x8C06C204, 0x8C06C182,
                   0x8C000000, 0x8CFFFFFC, 0x8C700000 + case * 16][(case // 2) % 6]
    for address in set([source, destination, 0x8C710000 + case * 16, 0x8C720000 + case * 16]):
        offset = (address & 0x1fffffff) - 0x0c000000
        if 0 <= offset <= RAM_SIZE - 4 and not BASE <= address < BASE + len(binary):
            struct.pack_into('<I', ram, offset, rng.getrandbits(32))
    struct.pack_into('<I', ram, SOURCE_LITERAL - 0x8C000000, source)
    struct.pack_into('<I', ram, DESTINATION_LITERAL - 0x8C000000, destination)
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    expected_ram = bytearray(ram)
    expected = State.from_buffer_copy(state)
    try:
        reference(expected_ram, expected)
    except Exception as error:
        raise AssertionError(f'case={case} source={source:#x} destination={destination:#x} source_literal={struct.unpack_from("<I", ram, SOURCE_LITERAL - 0x8C000000)[0]:#x} destination_literal={struct.unpack_from("<I", ram, DESTINATION_LITERAL - 0x8C000000)[0]:#x}') from error
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
    reports.append({'case': case, 'source': hex(source), 'destination': hex(destination), 'ram_and_cpu_match': True})

report = {
    'module': '1ST_READ.BIN',
    'load_address': hex(BASE),
    'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'binary_sha256': hashlib.sha256(binary).hexdigest(),
    'source_sha256': hashlib.sha256((ROOT / 'source' / 'sa_global_word_copy_8c06c182.c').read_bytes()).hexdigest(),
    'cases_passed': len(reports),
    'comparison_level': 'Complete 16 MiB RAM and all GPRs, SR, PR, and return PC compared against the retail SH-4 interpreter.',
    'scope': 'Tests both literal-loaded pointers, pointer-slot/code aliases, self-copy and overlapping targets, randomized values, and the first/last aligned RAM words.',
    'limitations': ['The semantic meaning and higher-level ownership of the copied global word remain unidentified.'],
    'cases': reports,
}
(ROOT / 'analysis' / 'global_word_copy_8c06c182_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{len(reports)} global word copy full RAM/CPU comparisons passed")
