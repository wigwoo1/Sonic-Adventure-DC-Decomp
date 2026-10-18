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
    (0x8C091B54, 10, '47 53 31 85 04 cb 0b 00 31 81', 'sa_task_flag_set_0x04_8c091b54'),
    (0x8C091B5E, 10, '47 53 31 85 10 cb 0b 00 31 81', 'sa_task_flag_set_0x10_8c091b5e'),
    (0x8C091B68, 8, '47 50 01 85 0b 00 04 c9', 'sa_task_flag_test_0x04_8c091b68'),
]

binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
binary_sha256 = hashlib.sha256(binary).hexdigest()
for address, length, encoding, _ in FUNCTIONS:
    assert binary[address - BASE:address - BASE + length] == bytes.fromhex(encoding)

library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'task_flag_access_8c091b54.dll'
library = ctypes.CDLL(str(library_path))
native_functions = []
for _, _, _, name in FUNCTIONS:
    native = getattr(library, name)
    native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    native.restype = None
    native_functions.append(native)

def write32(ram, address, value):
    struct.pack_into('<I', ram, address - 0x8C000000, value)

def write16(ram, address, value):
    struct.pack_into('<H', ram, address - 0x8C000000, value)

rng = random.Random(0x8C091B54)
reports = []
for (function, length, _, name), native in zip(FUNCTIONS, native_functions):
    cases = 0
    for case in range(128):
        ram = bytearray(RAM_SIZE)
        ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
        object_address = 0x8C700000 + (case * 0x100)
        data_address = [0x8C710000 + (case * 8), function - 2, 0x8CFFFFFC][case % 3]
        write32(ram, object_address + 0x1C, data_address)
        field_address = data_address + 2
        overlaps_code = function <= field_address < function + length
        if not overlaps_code:
            write16(ram, field_address, rng.choice([
                0, 1, 4, 0x10, 0x14, 0x8000, 0xFFFF, rng.getrandbits(16)]))

        state = State()
        state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        state.r[4] = object_address
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
    'scope': 'The task flag accessors at 0x8c091b54, 0x8c091b5e, and 0x8c091b68 reach a status word through the object pointer at +0x1c and its field at +2. The setters OR masks 0x04 and 0x10, and the getter returns field & 0x04. Tests cover flag edge values, code-alias reads/writes, and the final aligned halfword in RAM.',
    'limitations': ['Uses the project SH-4 interpreter rather than Dreamcast hardware or end-to-end gameplay.', 'The higher-level task state names and lifecycle semantics remain unidentified.'],
    'binary_sha256': binary_sha256,
    'source_sha256': hashlib.sha256((ROOT / 'source' / 'sa_task_flag_access_8c091b54.c').read_bytes()).hexdigest(),
    'routines': reports,
}
(ROOT / 'analysis' / 'task_flag_access_8c091b54_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{report['total_full_state_cases']} task flag full RAM/CPU comparisons passed")
