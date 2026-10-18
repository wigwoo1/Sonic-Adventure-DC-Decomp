"""Differentially verify the STG09 counter wrapper with its tail worker mocked."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
LOAD, ADDRESS, LENGTH = 0x0C900000, 0x0C903BE0, 44
RAM_BASE, RAM_SIZE = 0x0C000000, 16 * 1024 * 1024
module = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
span = module[ADDRESS - LOAD:ADDRESS - LOAD + LENGTH]
assert span == bytes.fromhex(
    '43 84 0c 60 03 c8 0f 8b 4a 84 08 20 0c 89 13 e0 4c 00 '
    '13 e1 4c 31 01 70 00 21 0e 60 03 88 01 8b 00 e0 4a 80 3a af 09 00 0b 00 09 00')

library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/stg09_tikal_compare_counter_0c903be0.dll'))
native = library.sa_stg09_tikal_compare_counter_0c903be0
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    value = (address & 0x1FFFFFFF) - RAM_BASE
    assert 0 <= value < RAM_SIZE - 4, hex(address)
    return value

rng = random.Random(0x0C903BE0)
records = []
for case in range(512):
    ram = bytearray(b'\xa5') * RAM_SIZE
    ram[LOAD - RAM_BASE:LOAD - RAM_BASE + len(module)] = module
    object = 0x0C800000 + case * 0x100
    flags = rng.randrange(256)
    active = rng.randrange(256)
    counter = rng.randrange(256)
    ram[offset(object + 3)] = flags
    ram[offset(object + 10)] = active
    ram[offset(object + 19)] = counter
    initial = State()
    initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
    initial.r[4] = object
    initial.pr = 0x0CFF0000 + case * 4
    initial.pc = ADDRESS
    initial.sr = rng.getrandbits(32)
    expected_ram, actual_ram = bytearray(ram), bytearray(ram)
    expected = State.from_buffer_copy(initial)
    actual = State.from_buffer_copy(initial)
    steps = reference(expected_ram, expected,
                      hooks={0x0C903A7C: lambda memory, regs, sr: sr},
                      stop_pc=initial.pr, instruction_budget=128)
    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    native(view, ctypes.byref(actual))
    assert actual_ram == expected_ram, f'RAM mismatch case={case}'
    if bytes(actual) != bytes(expected):
        print('CPU diff', case, [(i, hex(actual.r[i]), hex(expected.r[i])) for i in range(16) if actual.r[i] != expected.r[i]],
              hex(actual.sr), hex(expected.sr), hex(actual.pr), hex(expected.pr), hex(actual.pc), hex(expected.pc),
              'inputs', flags, active, counter)
        raise AssertionError(f'CPU mismatch case={case}')
    records.append({'case': case, 'flags': flags, 'active': active,
                    'counter': counter, 'retail_instructions': steps,
                    'full_ram_cpu_match': True})

source = PROJECT / 'source/sa_stg09_tikal_compare_counter_0c903be0.c'
report = {
    'module': 'STG09.BIN', 'load_address': hex(LOAD), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': len(records),
    'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state with the 0x0c903a7c tail worker replaced by a no-op return at its retail entry.',
    'scope': '44-byte Tikal compare-counter wrapper: flag guard, byte counter increment, third-count clear, output registers, T bit, and tail transfer.',
    'limitations': ['The comparison worker at 0x0c903a7c is an explicit no-op test stub; its gameplay effects are not covered by this verification.',
                    'This verifies the wrapper boundary, not the complete Tikal comparison behavior or stage.'],
    'cases': records,
}
(PROJECT / 'analysis/common-overlays/STG09.BIN/tikal_compare_counter_0c903be0_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{len(records)} STG09 Tikal counter-wrapper full RAM/CPU comparisons passed (worker mocked)')
