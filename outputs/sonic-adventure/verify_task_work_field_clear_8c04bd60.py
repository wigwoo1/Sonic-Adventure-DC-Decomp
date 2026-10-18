import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C04BD60
LENGTH = 16
binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'task_work_field_clear_8c04bd60.dll'
library = ctypes.CDLL(str(library_path))
native = library.sa_task_work_field_clear_8c04bd60
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(FUNCTION)
reports = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    object_address = [0x8C710000 + case * 0x100, FUNCTION,
                      0x8CFFFFC0][case % 3]
    pointer_address = object_address + 0x3C
    target = [0x8C720000 + case * 0x100, object_address,
              pointer_address, FUNCTION, 0x8CFFFF94][case % 5]
    if target > 0x8CFFFF94:
        target = 0x8C740000 + case * 0x100
    for offset in (0x5E, 0x64, 0x68):
        struct.pack_into('<I', ram, target - 0x8C000000 + offset,
                         rng.getrandbits(32))
    struct.pack_into('<I', ram, pointer_address - 0x8C000000, target)

    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = object_address
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
    assert bytes(actual) == bytes(expected), f'CPU state mismatch in case {case}'
    reports.append({'case': case, 'ram_and_cpu_match': True})

report = {
    'module': '1ST_READ.BIN',
    'load_address': hex(BASE),
    'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'binary_sha256': hashlib.sha256(binary).hexdigest(),
    'source_sha256': hashlib.sha256((ROOT / 'source' / 'sa_task_work_field_clear_8c04bd60.c').read_bytes()).hexdigest(),
    'cases_passed': len(reports),
    'comparison_level': 'Complete 16 MiB RAM and all GPRs, SR, PR, and return PC compared against the retail SH-4 interpreter.',
    'scope': 'Tests the task work state clear at 0x8c04bd60 with direct and self-referential pointers, code-backed storage, and the final aligned word boundary.',
    'limitations': ['Uses the project SH-4 interpreter rather than Dreamcast hardware or end-to-end gameplay.'],
    'cases': reports,
}
(ROOT / 'analysis' / 'task_work_field_clear_8c04bd60_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{len(reports)} task work field clear full RAM/CPU comparisons passed")
