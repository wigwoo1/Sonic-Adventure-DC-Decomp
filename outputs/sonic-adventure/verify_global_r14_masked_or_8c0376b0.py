import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C0376B0
LENGTH = 18
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('04 d1 05 d0 e9 20 12 68 8b 20 04 d1 12 61 0b 00 02 21')

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'global_r14_masked_or_8c0376b0.dll'))
native = library.sa_global_r14_masked_or_8c0376b0
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE, hex(address)
    return result

def put32(ram, address, value):
    at = offset(address)
    ram[at:at + 4] = (value & 0xFFFFFFFF).to_bytes(4, 'little')

source_address = int.from_bytes(binary[0x8C0376C4 - BASE:0x8C0376C4 - BASE + 4], 'little')
pointer_slot = int.from_bytes(binary[0x8C0376CC - BASE:0x8C0376CC - BASE + 4], 'little')
value_mask = int.from_bytes(binary[0x8C0376C8 - BASE:0x8C0376C8 - BASE + 4], 'little')
rng = random.Random(FUNCTION)
cases = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    target = 0x8C750000 + case * 0x20
    old_value = rng.getrandbits(32)
    initial_target = rng.getrandbits(32)
    put32(ram, source_address, old_value)
    put32(ram, pointer_slot, target)
    put32(ram, target, initial_target)
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[14] = rng.choice([0, 1, 0xFFFFFFFF, value_mask, 0x80000000, rng.getrandbits(32)]) if case < 24 else rng.getrandbits(32)
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    reference(expected_ram, expected_state)
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual_state))
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU state mismatch in case {case}'
    cases.append({'case': case, 'r14': hex(state.r[14]), 'old_source_word': hex(old_value), 'target': hex(target), 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_global_r14_masked_or_8c0376b0.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against the retail SH-4 interpreter',
    'scope': 'Exact 18-byte routine; merges the global word with a masked R14 value, then stores through a second global pointer in the RTS delay slot.',
    'limitations': ['The meaning and ownership of the global word and R14 value remain unidentified.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'source_address': hex(source_address), 'pointer_slot': hex(pointer_slot), 'value_mask': hex(value_mask),
    'cases': cases,
}
(PROJECT / 'analysis' / 'global_r14_masked_or_8c0376b0_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} global R14 masked-OR RAM/CPU comparisons passed')
