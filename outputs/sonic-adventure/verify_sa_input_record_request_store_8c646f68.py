"""Differentially verify the input request record writer against retail SH-4."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
BASE, ADDRESS, LENGTH = 0x8C010000, 0x8C646F68, 36
POINTER_CELL, GLOBAL_CELL, LOOKUP_BASE_CELL = 0x8C647070, 0x8C8A83B4, 0x8C647074
RAM_SIZE = 16 * 1024 * 1024
BINARY = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
SPAN = BINARY[ADDRESS - BASE:ADDRESS - BASE + LENGTH]
assert len(SPAN) == LENGTH
LIBRARY = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/sa_input_record_request_store_8c646f68.dll'))
FUNCTION = LIBRARY.sa_input_record_request_store_8c646f68
FUNCTION.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
FUNCTION.restype = None


def put32(ram, address, value):
    struct.pack_into('<I', ram, address & 0xFFFFFF, value & 0xFFFFFFFF)


def put16(ram, address, value):
    struct.pack_into('<H', ram, address & 0xFFFFFF, value & 0xFFFF)


rng = random.Random(ADDRESS)
cases = []
for case in range(256):
    table = 0x8C700000 + (case % 32) * 0x10000
    index = (case * 7) & 0x3FFF
    slot = rng.randrange(0x100)
    if case == 0:
        index, slot = 0, 0
    elif case == 1:
        index, slot = 2, 5
    elif case == 2:
        index, slot = 0x40000000, 0
    elif case == 3:
        index, slot = 0x40000001, 0
    elif case == 4:
        index, slot = 0x7FFFFFFF, 0
    elif case == 5:
        index, slot = 0x80000000, 0
    lookup_entry = (table + 0xFC0) & 0xFFFFFFFF
    offset_value = ((index << 1) + slot) & 0xFFFFFFFF
    record = (0x8C730000 + (case % 16) * 0x1000 + (offset_value * 12 & 0xFFFFFFFF)) & 0xFFFFFFFF
    alias = case % 6
    if alias == 1:
        # The record writes replace the lookup word after the routine has read it.
        record = (lookup_entry - 0x6C) & 0xFFFFFFFF
    elif alias == 2:
        # The first record word replaces the global pointer cell after both reads.
        index, slot = 0, 0
        offset_value = 0
        record = GLOBAL_CELL
    elif alias == 3:
        # The record overwrites the destination-base literal after it has been loaded.
        record = LOOKUP_BASE_CELL
    if case in (2, 3, 4, 5):
        record = 0x8C740000 + (case - 2) * 0x100

    initial = bytearray(b'\xA5') * RAM_SIZE
    initial[BASE - 0x8C000000:BASE - 0x8C000000 + len(BINARY)] = BINARY
    put32(initial, GLOBAL_CELL, table)
    put32(initial, lookup_entry, slot)
    put32(initial, LOOKUP_BASE_CELL, (record - (offset_value * 12 & 0xFFFFFFFF)) & 0xFFFFFFFF)
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = index
    state.r[5] = rng.getrandbits(32)
    state.r[6] = rng.getrandbits(32)
    state.sr = rng.getrandbits(32)
    state.pr = 0x8CFED000 + case * 4
    state.pc = ADDRESS

    retail_ram, native_ram = bytearray(initial), bytearray(initial)
    expected, actual = State.from_buffer_copy(bytes(state)), State.from_buffer_copy(bytes(state))
    steps = reference(retail_ram, expected)
    native_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
    FUNCTION(native_view, ctypes.byref(actual))
    assert retail_ram == native_ram, f'RAM mismatch case={case} alias={alias}'
    assert bytes(expected) == bytes(actual), f'CPU mismatch case={case} alias={alias}'
    cases.append({'case': case, 'index': hex(index), 'lookup_value': hex(slot),
                  'record_address': hex(expected.r[7]), 'alias_fixture': alias,
                  'retail_instructions': steps, 'full_ram_and_cpu_match': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(SPAN).hexdigest(),
    'binary_sha256': hashlib.sha256(BINARY).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source/sa_input_record_request_store_8c646f68.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'Complete 16 MiB RAM and all GPR/SR/PR/PC state against retail SH-4 execution.',
    'scope': 'Computes a 12-byte input request record from the request index and global lookup table, then stores index and two request arguments in retail order.',
    'limitations': ['The parent service routine and scheduler calls are separate functions.', 'Verification uses the project SH-4 interpreter, not Dreamcast hardware or complete-game execution.'],
    'cases': cases,
}
(PROJECT / 'analysis/sa_input_record_request_store_8c646f68_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{len(cases)} input request record writer RAM/CPU comparisons passed')