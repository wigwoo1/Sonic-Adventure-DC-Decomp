import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C10D334
LENGTH = 30
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
assert int.from_bytes(binary[0x8C10D352 - BASE:0x8C10D354 - BASE], 'little') == 0x3039
assert int.from_bytes(binary[0x8C10D354 - BASE:0x8C10D358 - BASE], 'little') == 0x7FFF
assert int.from_bytes(binary[0x8C10D358 - BASE:0x8C10D35C - BASE], 'little') == 0x8C5FBB08
assert int.from_bytes(binary[0x8C10D35C - BASE:0x8C10D360 - BASE], 'little') == 0x41C64E6D

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'random_u15_8c10d334.dll'))
native = library.sa_random_u15_8c10d334
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State), ctypes.POINTER(ctypes.c_uint32)]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE, hex(address)
    return result

rng = random.Random(FUNCTION)
seed_address = 0x8C5FBB08
known_seeds = [0, 1, 0xFFFFFFFF, 0x12345678, 0x80000000, 0x7FFFFFFF]
cases = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    seed = known_seeds[case] if case < len(known_seeds) else rng.getrandbits(32)
    at = offset(seed_address)
    ram[at:at + 4] = seed.to_bytes(4, 'little')

    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[15] = 0x8C7F0000 - case * 0x100
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    initial_macl = rng.getrandbits(32)

    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    expected_special = {'macl': initial_macl}
    reference(expected_ram, expected_state, special=expected_special)
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    actual_macl = ctypes.c_uint32(initial_macl)
    native(ram_view, ctypes.byref(actual_state), ctypes.byref(actual_macl))

    assert actual_ram == expected_ram, f'RAM mismatch in case {case}, seed={seed:#x}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU state mismatch in case {case}, seed={seed:#x}'
    assert actual_macl.value == expected_special['macl'], f'MACL mismatch in case {case}'
    cases.append({'case': case, 'initial_seed': hex(seed), 'initial_macl': hex(initial_macl), 'ram_cpu_macl_match': True})

source_path = PROJECT / 'source' / 'sa_random_u15_8c10d334.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source_path.read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM, all GPR/SR/PR/PC state, and MACL against the retail SH-4 interpreter',
    'scope': 'Global 32-bit linear-congruential seed update and masked output, including the visible stack save and restoration of MACL.',
    'limitations': [
        'The random helper is verified against the project SH-4 instruction interpreter, not Dreamcast hardware or whole-game random sequences.',
        'Call-site-specific random-seed lifecycle and gameplay distribution are not validated.'
    ],
    'cases': cases,
}
(PROJECT / 'analysis' / 'random_u15_8c10d334_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} PRNG complete RAM/CPU/MACL comparisons passed')
