import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C65254E
LENGTH = 6
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('b3 04 0b 00 09 00')

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'cache_block_writeback_8c65254e.dll'))
WRITEBACK = ctypes.CFUNCTYPE(None, ctypes.c_uint32, ctypes.c_void_p)
native = library.sa_cache_block_writeback_8c65254e
native.argtypes = [ctypes.POINTER(State), WRITEBACK, ctypes.c_void_p]
native.restype = None

rng = random.Random(FUNCTION)
cases = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = [0, 0x8C000000, 0x8C010000, 0x8CFFFFFF, 0xA05F6800][case % 5] if case < 10 else rng.getrandbits(32)
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    expected_addresses = []
    reference(expected_ram, expected_state, cache_writeback=expected_addresses.append)
    actual_ram = bytearray(ram)
    native_addresses = []
    callback = WRITEBACK(lambda address, _context: native_addresses.append(address))
    actual_state = State.from_buffer_copy(state)
    native(ctypes.byref(actual_state), callback, None)
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch in case {case}'
    assert native_addresses == expected_addresses == [state.r[4]], f'cache service mismatch in case {case}'
    cases.append({'case': case, 'address': hex(state.r[4]), 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_cache_block_writeback_8c65254e.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM, all GPR/SR/PR/PC state, and cache-writeback service argument against the retail SH-4 interpreter',
    'scope': 'Issue one OCBWB service for the address in R4, then return without changing architectural registers.',
    'limitations': ['The emulator must implement the supplied cache writeback service correctly for its memory model.', 'Differential verification exercises the project SH-4 instruction model, not Dreamcast cache hardware or whole-game execution.'],
    'cases': cases,
}
(PROJECT / 'analysis' / 'cache_block_writeback_8c65254e_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} cache writeback service and RAM/CPU comparisons passed')
