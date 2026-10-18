import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
FUNCTIONS = [
    (0x8C047D90, 18, 'sa_vector_copy_to_offset_0x68_8c047d90'),
    (0x8C047DA2, 18, 'sa_vector_copy_from_offset_0x68_8c047da2'),
]
binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
binary_sha256 = hashlib.sha256(binary).hexdigest()
library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'task_vector_pair_copy_8c047d90.dll'
library = ctypes.CDLL(str(library_path))
native_functions = []
for _, _, name in FUNCTIONS:
    native = getattr(library, name)
    native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    native.restype = None
    native_functions.append(native)

rng = random.Random(0x8C047D90)
reports = []
for function_index, ((function, length, name), native) in enumerate(zip(FUNCTIONS, native_functions)):
    cases = 0
    for case in range(128):
        ram = bytearray(RAM_SIZE)
        ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
        if case % 8 == 0:
            source = function
        elif case % 8 == 5:
            source = 0x8CFFFF8C if function_index else 0x8CFFFFD4
        else:
            source = 0x8C710000 + case * 0x100

        if function_index == 0:
            destination = (source if case % 8 == 1 else
                           source - 0x48 if case % 8 == 2 else
                           source - 0x44 if case % 8 == 3 else
                           0x8CFFFF8C if case % 8 == 6 else
                           0x8C740000 + case * 0x100)
            source_reg, destination_reg = 4, 5
            source_offsets, destination_offsets = (0x20, 0x24, 0x28), (0x68, 0x6C, 0x70)
        else:
            destination = (source if case % 8 == 1 else
                           source + 0x48 if case % 8 == 2 else
                           source + 0x44 if case % 8 == 3 else
                           0x8CFFFFD4 if case % 8 == 6 else
                           0x8C740000 + case * 0x100)
            source_reg, destination_reg = 5, 4
            source_offsets, destination_offsets = (0x68, 0x6C, 0x70), (0x20, 0x24, 0x28)

        if source != function:
            for offset in source_offsets:
                struct.pack_into('<I', ram, source - 0x8C000000 + offset, rng.getrandbits(32))
        if not (source <= destination + max(destination_offsets) + 3 and
                destination <= source + max(source_offsets) + 3):
            for offset in destination_offsets:
                struct.pack_into('<I', ram, destination - 0x8C000000 + offset, rng.getrandbits(32))

        state = State()
        state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        state.r[source_reg] = source
        state.r[destination_reg] = destination
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
        assert bytes(actual) == bytes(expected), f'CPU state mismatch at {function:#x}, case {case}'
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
    'scope': 'Tests both directions of the three-word task vector transfer with independent and overlapping buffers, code-backed source data, and final aligned source and destination ranges in RAM.',
    'limitations': ['Uses the project SH-4 interpreter rather than Dreamcast hardware or end-to-end gameplay.'],
    'binary_sha256': binary_sha256,
    'source_sha256': hashlib.sha256((ROOT / 'source' / 'sa_task_vector_pair_copy_8c047d90.c').read_bytes()).hexdigest(),
    'routines': reports,
}
(ROOT / 'analysis' / 'task_vector_pair_copy_8c047d90_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{report['total_full_state_cases']} task vector pair full RAM/CPU comparisons passed")
