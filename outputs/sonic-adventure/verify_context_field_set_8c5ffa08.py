import ctypes
import hashlib
import json
import random
import struct
from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C5FFA08
LENGTH = 10
POINTER_LITERAL = 0x8C5FFC0C
DEFAULT_POINTER_SLOT = 0x8C673414
binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'context_field_set_8c5ffa08.dll'
library = ctypes.CDLL(str(library_path))
native = library.sa_context_field_set_8c5ffa08
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None
rng = random.Random(FUNCTION)
reports = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    if case % 8 == 0:
        context = 0x8CFFFFCC  # context + 0x30 is the last aligned RAM word
    elif case % 8 == 1:
        context = 0x8BFFFFD0  # context + 0x30 wraps to the first RAM word
    elif case % 8 == 2:
        context = FUNCTION - 0x30  # store overwrites already-fetched function bytes
    elif case % 8 == 3:
        context = POINTER_LITERAL - 0x30  # target aliases the pointer literal
    else:
        context = 0x8C720000 + case * 0x100
    pointer_slot = [DEFAULT_POINTER_SLOT, 0x8C710000 + case * 16][case % 2]
    struct.pack_into('<I', ram, pointer_slot - 0x8C000000, context)
    struct.pack_into('<I', ram, POINTER_LITERAL - 0x8C000000, pointer_slot)
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = rng.getrandbits(32)
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    expected_ram = bytearray(ram)
    expected = State.from_buffer_copy(state)
    try:
        reference(expected_ram, expected)
    except Exception as error:
        raise AssertionError(f'case={case} pointer_slot={pointer_slot:#x} context={context:#x}') from error
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
    reports.append({'case': case, 'pointer_slot': hex(pointer_slot), 'context': hex(context),
                    'ram_and_cpu_match': True})

report = {
    'module': '1ST_READ.BIN',
    'load_address': hex(BASE),
    'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'binary_sha256': hashlib.sha256(binary).hexdigest(),
    'source_sha256': hashlib.sha256((ROOT / 'source' / 'sa_context_field_set_8c5ffa08.c').read_bytes()).hexdigest(),
    'cases_passed': len(reports),
    'comparison_level': 'Complete 16 MiB RAM and all GPRs, SR, PR, and return PC compared against the retail SH-4 interpreter.',
    'scope': 'Tests the pointer literal and indirect context pointer, R2/R3 scratch results, R0=1 return value, arbitrary 32-bit field values, pointer aliases, and first/last aligned RAM destinations.',
    'limitations': ['The context field meaning and runtime initialization lifecycle are not yet identified.'],
    'cases': reports,
}
(ROOT / 'analysis' / 'context_field_set_8c5ffa08_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{len(reports)} context field setter full RAM/CPU comparisons passed")
