"""Verify the STG09 leaf's stack-adjust/RTS-delay-slot net effect."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
LOAD, ADDRESS, LENGTH = 0x0C900000, 0x0C901D20, 6
RAM_SIZE = 16 * 1024 * 1024
module = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
span = module[ADDRESS - LOAD:ADDRESS - LOAD + LENGTH]
assert span == bytes.fromhex('d4 7f 0b 00 2c 7f')

library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/stg09_leaf_0c901d20.dll'))
native = library.sa_stg09_leaf_0c901d20
native.argtypes = [ctypes.POINTER(State)]
native.restype = None
rng = random.Random(0xC901D20)
records = []
for case in range(64):
    ram = bytearray(b'\xa5') * RAM_SIZE
    ram[LOAD - 0x0C000000:LOAD - 0x0C000000 + len(module)] = module
    initial = State()
    initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
    initial.r[15] = 0x0CF00000 + case * 0x100
    initial.sr = rng.getrandbits(32)
    initial.pr = 0x0CFF0000 + case * 4
    initial.pc = ADDRESS
    expected_ram, actual_ram = bytearray(ram), bytearray(ram)
    expected, actual = State.from_buffer_copy(initial), State.from_buffer_copy(initial)
    steps = reference(expected_ram, expected)
    native(ctypes.byref(actual))
    assert bytes(actual) == bytes(expected), f'CPU mismatch case={case}'
    assert actual_ram == expected_ram, f'RAM mismatch case={case}'
    records.append({'case': case, 'retail_steps': steps, 'full_state_match': True})

source = PROJECT / 'source/sa_stg09_leaf_0c901d20.c'
report = {
    'module': 'STG09.BIN', 'load_address': hex(LOAD), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': len(records),
    'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state.',
    'scope': 'Six-byte leaf; 44-byte stack allocation and restoration in the RTS delay slot.',
    'limitations': ['Only integer CPU state and architectural RAM are modeled; Dreamcast hardware is not involved.'],
    'cases': records,
}
(PROJECT / 'analysis/common-overlays/STG09.BIN/leaf_0c901d20_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{len(records)} STG09 leaf full RAM/CPU comparisons passed')
