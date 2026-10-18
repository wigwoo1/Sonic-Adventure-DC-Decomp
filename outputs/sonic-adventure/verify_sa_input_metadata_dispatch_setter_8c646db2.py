"""Verify the input metadata setter dispatcher together with its retail callees."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
BASE, ADDRESS, LENGTH = 0x8C010000, 0x8C646DB2, 168
RAM_BASE, RAM_SIZE = 0x8C000000, 32 * 1024 * 1024
POINTER_SLOT = 0x8C8A83B4
SOURCE = PROJECT / 'source/sa_input_metadata_dispatch_setter_8c646db2.c'
BINARY = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
SPAN = BINARY[ADDRESS - BASE:ADDRESS - BASE + LENGTH]
assert len(SPAN) == LENGTH
LIBRARY = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/sa_input_metadata_dispatch_setter_8c646db2.dll'))
FUNCTION = LIBRARY.sa_input_metadata_dispatch_setter_8c646db2
FUNCTION.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
FUNCTION.restype = None

rng = random.Random(ADDRESS)
selectors = list(range(10)) + [10, 0xFFFFFFFF]
selectors.extend(rng.choice(list(range(10)) + [10, 0xFFFFFFFF, 0x80000000]) for _ in range(244))
cases = []
for case, selector in enumerate(selectors):
    initial = bytearray(b'\xA5') * RAM_SIZE
    initial[BASE - RAM_BASE:BASE - RAM_BASE + len(BINARY)] = BINARY
    record = 0x8C720400 + case * 0x3000
    struct.pack_into('<I', initial, POINTER_SLOT - RAM_BASE, record)
    # Distinct data around each record makes the selected setter's write visible.
    initial[record - RAM_BASE:record - RAM_BASE + 0x1400] = bytes(
        ((i * 29 + case * 17) & 0xFF) for i in range(0x1400))

    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = selector
    state.r[5] = rng.getrandbits(32)
    state.r[15] = 0x8C800000 + case * 0x100 + 0x40
    state.sr = rng.getrandbits(32)
    state.pr = 0x8CFED000 + case * 4
    state.pc = ADDRESS

    retail_ram, native_ram = bytearray(initial), bytearray(initial)
    expected, actual = State.from_buffer_copy(bytes(state)), State.from_buffer_copy(bytes(state))
    steps = reference(retail_ram, expected)
    native_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
    FUNCTION(native_view, ctypes.byref(actual))
    assert retail_ram == native_ram, f'RAM mismatch case={case} selector={selector:08x}'
    if bytes(expected) != bytes(actual):
        diffs = {f'r{i}': (hex(expected.r[i]), hex(actual.r[i])) for i in range(16) if expected.r[i] != actual.r[i]}
        diffs.update({name: (hex(getattr(expected, name)), hex(getattr(actual, name)))
                      for name in ('sr', 'pr', 'pc') if getattr(expected, name) != getattr(actual, name)})
        raise AssertionError(f'CPU mismatch case={case} selector={selector:08x}: {diffs}')
    cases.append({'case': case, 'selector': hex(selector), 'retail_instructions': steps,
                  'full_ram_and_cpu_match': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(SPAN).hexdigest(),
    'binary_sha256': hashlib.sha256(BINARY).hexdigest(),
    'source_sha256': hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'Complete 32 MiB RAM and all GPR/SR/PR/PC state against retail SH-4 execution, including the selected retail callback routine.',
    'scope': 'Dispatches selectors zero through nine to their matching field setter, returns 0xffffffff for other values, and includes the callback effects in every full-state comparison.',
    'limitations': ['Runtime record pointer is initialized to a valid test record because the uninitialized retail image contains a placeholder value.',
                    'Verification uses the project SH-4 interpreter, not Dreamcast hardware or complete-game execution.'],
    'cases': cases,
}
(PROJECT / 'analysis/sa_input_metadata_dispatch_setter_8c646db2_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{len(cases)} metadata setter-dispatch RAM/CPU comparisons passed')
