"""Differentially verify the short STG09 Tikal status-flag helper."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
LOAD, ADDRESS, LENGTH = 0x0C900000, 0x0C903C4E, 32
RAM_BASE, RAM_SIZE = 0x0C000000, 16 * 1024 * 1024
GLOBAL_POINTER = 0x8C754DA8
module = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
span = module[ADDRESS - LOAD:ADDRESS - LOAD + LENGTH]
assert span == bytes.fromhex('2c d0 02 60 04 50 04 c8 08 89 13 e0 00 e2 ff e3 '
                             '24 04 42 85 18 43 ff 73 39 20 42 81 0b 00 09 00')

library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/stg09_clear_tikal_flag_0c903c4e.dll'))
native = library.sa_stg09_clear_tikal_flag_0c903c4e
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    value = (address & 0x1FFFFFFF) - RAM_BASE
    assert 0 <= value < RAM_SIZE - 4, hex(address)
    return value

rng = random.Random(0x0C903C4E)
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
    ram[offset(object + 0x13)] = rng.randrange(256)

    initial = State()
    initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
    initial.r[4] = object
    initial.r[15] = 0x0CF00000 + case * 0x100
    initial.sr = rng.getrandbits(32)
    initial.pr = 0x0CFF0000 + case * 4
    initial.pc = ADDRESS
    expected_ram, actual_ram = bytearray(ram), bytearray(ram)
    expected = State.from_buffer_copy(initial)
    actual = State.from_buffer_copy(initial)
    steps = reference(expected_ram, expected, stop_pc=initial.pr, instruction_budget=64)
    actual_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    native(actual_view, ctypes.byref(actual))
    assert actual_ram == expected_ram, f'RAM mismatch case={case}'
    if bytes(actual) != bytes(expected):
        changes={f'r{i}':(hex(expected.r[i]),hex(actual.r[i])) for i in range(16)
                 if expected.r[i]!=actual.r[i]}
        changes.update({k:(hex(getattr(expected,k)),hex(getattr(actual,k)))
                        for k in ('sr','pr','pc') if getattr(expected,k)!=getattr(actual,k)})
        raise AssertionError(f'CPU mismatch case={case}: {changes}')
    records.append({'case': case, 'global_flag_set': bool(state_word & 4),
                    'retail_instructions': steps, 'full_ram_cpu_match': True})

source = PROJECT / 'source/sa_stg09_clear_tikal_flag_0c903c4e.c'
report = {
    'module': 'STG09.BIN', 'load_address': hex(LOAD), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': len(records),
    'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state.',
    'scope': '32-byte call-free Tikal status helper; global test, conditional object-byte clear, status-bit clear, and return delay slot.',
    'limitations': ['The global pointer target and object layout are preserved from retail RAM.',
                    'Verification covers the helper, not its caller or full gameplay state machine.'],
    'cases': records,
}
(PROJECT / 'analysis/common-overlays/STG09.BIN/clear_tikal_flag_0c903c4e_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{len(records)} STG09 Tikal-flag full RAM/CPU comparisons passed')
