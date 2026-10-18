import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
POINTER_SLOT = 0x8C04FE8C
FUNCTIONS = [
    (0x8C04FE40, 10, 'sa_global_mode_is_0x10_8c04fe40'),
    (0x8C04FE4A, 32, 'sa_global_mode_is_0xf_4_or_2_8c04fe4a'),
]
binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
binary_sha256 = hashlib.sha256(binary).hexdigest()
library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'global_mode_predicates_8c04fe40.dll'
library = ctypes.CDLL(str(library_path))
native_functions = []
for _, _, name in FUNCTIONS:
    native = getattr(library, name)
    native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    native.restype = None
    native_functions.append(native)

rng = random.Random(0x8C04FE40)
reports = []
for (function, length, name), native in zip(FUNCTIONS, native_functions):
    cases = 0
    for case in range(128):
        ram = bytearray(RAM_SIZE)
        ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
        target = [0x8C7493B4, 0x8C730000 + case * 4,
                  function, POINTER_SLOT, 0x8CFFFFFC][case % 5]
        struct.pack_into('<I', ram, POINTER_SLOT - 0x8C000000, target)
        value = [0, 1, 2, 4, 15, 16, 0x7fff, 0x8000, 0xffff,
                 rng.getrandbits(32)][case % 10]
        overlaps_code = any(start <= target <= start + size - 4
                            for start, size, _ in FUNCTIONS)
        if target != POINTER_SLOT and not overlaps_code:
            struct.pack_into('<I', ram, target - 0x8C000000, value)

        state = State()
        state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        state.sr = rng.getrandbits(32)
        state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
        state.pc = function

        expected_ram = bytearray(ram)
        expected = State.from_buffer_copy(state)
        reference(expected_ram, expected)
        actual_ram = bytearray(ram)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        actual = State.from_buffer_copy(state)
        native(ram_view, ctypes.byref(actual))
        assert actual_ram == expected_ram, f'RAM mismatch at {function:#x}, case {case}'
        assert bytes(actual) == bytes(expected), (
            f'CPU state mismatch at {function:#x}, case {case}: '
            f'r={[(i, hex(actual.r[i]), hex(expected.r[i])) for i in range(16) if actual.r[i] != expected.r[i]]}, '
            f'sr={actual.sr:#x}/{expected.sr:#x}, pc={actual.pc:#x}/{expected.pc:#x}'
        )
        cases += 1

    span = binary[function - BASE:function - BASE + length]
    reports.append({
        'module': '1ST_READ.BIN',
        'load_address': hex(BASE),
        'address': hex(function),
        'end_exclusive': hex(function + length),
        'retail_span_sha256': hashlib.sha256(span).hexdigest(),
        'cases_passed': cases,
        'name': name,
    })

report = {
    'functions_verified': len(reports),
    'total_full_state_cases': sum(item['cases_passed'] for item in reports),
    'comparison_level': 'Complete 16 MiB RAM and all GPRs, SR, PR, and return PC compared against the retail SH-4 interpreter.',
    'scope': 'Tests the mode == 0x10 and mode in {0x0f, 4, 2} predicates with edge values, randomized field targets, code reads, the pointer literal, and the final aligned RAM word.',
    'limitations': ['Uses the project SH-4 interpreter rather than Dreamcast hardware or end-to-end gameplay.'],
    'binary_sha256': binary_sha256,
    'source_sha256': hashlib.sha256((ROOT / 'source' / 'sa_global_mode_predicates_8c04fe40.c').read_bytes()).hexdigest(),
    'routines': reports,
}
(ROOT / 'analysis' / 'global_mode_predicates_8c04fe40_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{report['total_full_state_cases']} global mode predicate full RAM/CPU comparisons passed")
