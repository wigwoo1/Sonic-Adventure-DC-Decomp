import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C03767C
LENGTH = 32
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('07 d1 10 58 12 59 07 da a9 28 07 da a9 29 07 da 09 2a ab 28 06 db 09 2b bb 29 80 11 0b 00 92 11')

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'global_pair_masked_update_8c03767c.dll'))
native = library.sa_global_pair_masked_update_8c03767c
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE, hex(address)
    return result

rng = random.Random(FUNCTION)
base_pointer = int.from_bytes(binary[0x8C03769C - BASE:0x8C03769C - BASE + 4], 'little')
target_a, target_b = base_pointer, base_pointer + 8
offset(target_a)
offset(target_b)
cases = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    old_a, old_b = rng.getrandbits(32), rng.getrandbits(32)
    ram[offset(target_a):offset(target_a) + 4] = old_a.to_bytes(4, 'little')
    ram[offset(target_b):offset(target_b) + 4] = old_b.to_bytes(4, 'little')
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    if case < 12:
        state.r[0] = [0, 1, 0xFFFFFFFF, 0x00FFFFFF, 0xFF00FF00, 0xAAAAAAAA,
                      0x55555555, 0x80000000, 0x0000003F, 0x001800C0,
                      0x0200003F, 0x40000000][case]
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
    cases.append({'case': case, 'input_r0': hex(state.r[0]), 'initial_words': [hex(old_a), hex(old_b)], 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_global_pair_masked_update_8c03767c.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against the retail SH-4 interpreter',
    'scope': 'Exact 32-byte routine; applies four distinct literal masks to the two words at base and base+8, including the second store in the RTS delay slot.',
    'limitations': ['The two structure fields and caller-level meaning of R0 remain unidentified.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'base_pointer': hex(base_pointer),
    'literal_masks': {hex(address): hex(int.from_bytes(binary[address - BASE:address - BASE + 4], 'little')) for address in [0x8C0376A0, 0x8C0376A4, 0x8C0376A8, 0x8C0376AC]},
    'cases': cases,
}
(PROJECT / 'analysis' / 'global_pair_masked_update_8c03767c_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} global pair masked-update RAM/CPU comparisons passed')
