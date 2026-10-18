"""Verify the STG09 status wrapper and both internal byte-clear helpers."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
LOAD, ADDRESS, LENGTH = 0x0C900000, 0x0C903C0C, 40
RAM_BASE, RAM_SIZE = 0x0C000000, 16 * 1024 * 1024
GLOBAL_POINTER = 0x8C754DA8
module = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
span = module[ADDRESS - LOAD:ADDRESS - LOAD + LENGTH]
assert span == bytes.fromhex('3c d0 e6 2f 22 4f 02 60 04 50 04 c8 06 8d 43 6e '
                             'fc e3 e2 85 18 43 ff 73 39 20 e2 81 1a bf e3 64 '
                             '26 4f e3 64 1d af f6 6e')

library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/stg09_tikal_status_and_byte_cleanup_0c903c0c.dll'))
native = library.sa_stg09_tikal_status_and_byte_cleanup_0c903c0c
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    value = (address & 0x1FFFFFFF) - RAM_BASE
    assert 0 <= value < RAM_SIZE - 4, hex(address)
    return value

rng = random.Random(0x0C903C0C)
records = []
for case in range(256):
    ram = bytearray(b'\xa5') * RAM_SIZE
    ram[LOAD - RAM_BASE:LOAD - RAM_BASE + len(module)] = module
    object = 0x0C800000 + case * 0x100
    global_state = 0x0C810000 + case * 0x100
    state_word = rng.getrandbits(32) & ~4
    if case & 1:
        state_word |= 4
    struct.pack_into('<I', ram, offset(0x0C903D00), GLOBAL_POINTER)
    struct.pack_into('<I', ram, offset(GLOBAL_POINTER), global_state)
    struct.pack_into('<I', ram, offset(global_state + 0x10), state_word)
    struct.pack_into('<H', ram, offset(object + 4), rng.getrandbits(16))
    ram[offset(object)] = case & 0xff
    ram[offset(object + 1)] = (case * 3) & 0xff
    ram[offset(object + 8)] = rng.randrange(256)
    ram[offset(object + 9)] = rng.randrange(256)

    initial = State()
    initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
    initial.r[4] = object
    initial.r[14] = rng.getrandbits(32)
    initial.r[15] = 0x0CF00000 + case * 0x100
    initial.sr = rng.getrandbits(32)
    initial.pr = 0x0CFF0000 + case * 4
    initial.pc = ADDRESS
    expected_ram, actual_ram = bytearray(ram), bytearray(ram)
    expected = State.from_buffer_copy(initial)
    actual = State.from_buffer_copy(initial)
    steps = reference(expected_ram, expected, stop_pc=initial.pr, instruction_budget=256)
    actual_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    native(actual_view, ctypes.byref(actual))
    assert actual_ram == expected_ram, f'RAM mismatch case={case}'
    assert bytes(actual) == bytes(expected), f'CPU mismatch case={case}'
    records.append({'case': case, 'global_flag_set': bool(state_word & 4),
                    'primary_byte_low_bits': ram[offset(object)] & 3,
                    'shifted_byte_low_bits': ram[offset(object + 1)] & 3,
                    'retail_instructions': steps, 'full_ram_cpu_match': True})

source = PROJECT / 'source/sa_stg09_tikal_status_and_byte_cleanup_0c903c0c.c'
report = {
    'module': 'STG09.BIN', 'load_address': hex(LOAD), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': len(records),
    'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state across wrapper and both leaf callees.',
    'scope': '40-byte Tikal status helper; conditional 0x0400 clear, saved PR/r14 frame, and the primary and shifted-byte leaves.',
    'limitations': ['Global pointer contents, object bytes, and stack region are initialized per case.',
                    'Verification covers this connected helper path, not the entire Tikal gameplay state machine.'],
    'cases': records,
}
(PROJECT / 'analysis/common-overlays/STG09.BIN/tikal_status_and_byte_cleanup_0c903c0c_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{len(records)} STG09 Tikal status/two-leaf full RAM/CPU comparisons passed')
