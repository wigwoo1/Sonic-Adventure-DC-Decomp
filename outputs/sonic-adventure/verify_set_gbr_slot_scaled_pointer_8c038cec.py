import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C038CEC
LENGTH = 14
RAM_SIZE = 16 * 1024 * 1024
DESTINATION_BASE = 0x8C038F60
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('18 44 00 44 08 44 02 d0 4c 30 0b 00 16 c2')
assert int.from_bytes(binary[0x8C038CFC - BASE:0x8C038CFC - BASE + 4], 'little') == DESTINATION_BASE

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'set_gbr_slot_scaled_pointer_8c038cec.dll'))
native = library.sa_set_gbr_slot_scaled_pointer_8c038cec
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State), ctypes.c_uint32]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE, hex(address)
    return result

rng = random.Random(FUNCTION)
cases = []
for case in range(96):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    initial_r4 = rng.getrandbits(32)
    gbr = 0x8C100000 + rng.randrange(0x3B0000 // 4) * 4
    at = offset(gbr + 0x58)
    ram[at:at + 4] = rng.randbytes(4)
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = initial_r4
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    special = {'gbr': gbr}
    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    expected_special = dict(special)
    reference(expected_ram, expected_state, special=expected_special)
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual_state), gbr)
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual_state) == bytes(expected_state), (
        f'CPU mismatch in case {case}: actual_r={list(actual_state.r)!r}, '
        f'expected_r={list(expected_state.r)!r}, actual_sr={actual_state.sr:08x}, '
        f'expected_sr={expected_state.sr:08x}, actual_pc={actual_state.pc:08x}, '
        f'expected_pc={expected_state.pc:08x}')
    assert actual_ram[at:at + 4] == ((DESTINATION_BASE + ((initial_r4 << 11) & 0xFFFFFFFF)) & 0xFFFFFFFF).to_bytes(4, 'little')
    assert expected_special['gbr'] == gbr, f'GBR changed in reference case {case}'
    cases.append({'case': case, 'gbr': hex(gbr), 'initial_r4': hex(initial_r4), 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_set_gbr_slot_scaled_pointer_8c038cec.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state, plus explicit GBR, against the retail SH-4 interpreter',
    'scope': 'Exact 14-byte routine; R4 scaled by 0x800, literal-derived pointer stored at GBR+0x58 in the RTS delay slot.',
    'limitations': ['The semantic identity of the GBR slot and pointed-to table remains unresolved.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'cases': cases,
}
(PROJECT / 'analysis' / 'set_gbr_slot_scaled_pointer_8c038cec_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} GBR scaled-pointer slot RAM/CPU comparisons passed')
