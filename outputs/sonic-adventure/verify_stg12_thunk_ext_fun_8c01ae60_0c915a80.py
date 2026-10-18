import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
FUNCTION = 0x0C915A80
LENGTH = 6
LOAD_ADDRESS = 0x0C900000
LITERAL = 0x0C915BC0
TARGET = 0x8C01AE60
RAM_SIZE = 16 * 1024 * 1024
module = (ROOT / 'work' / 'sonic-adventure' / 'decoded' / 'SONICADV' / 'STG12.BIN').read_bytes()
primary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
offset = FUNCTION - LOAD_ADDRESS
span = module[offset:offset + LENGTH]
assert span == bytes.fromhex('4f d3 2b 43 09 00')
literal_offset = LITERAL - LOAD_ADDRESS
assert int.from_bytes(module[literal_offset:literal_offset + 4], 'little') == TARGET
assert FUNCTION + 4 + 0x4f * 4 == LITERAL

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'stg12_thunk_ext_fun_8c01ae60_0c915a80.dll'))
native = library.sa_stg12_thunk_ext_fun_8c01ae60_0c915a80
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(FUNCTION)
cases = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[0x010000:0x010000 + len(primary)] = primary
    ram[LOAD_ADDRESS - 0x0C000000:LOAD_ADDRESS - 0x0C000000 + len(module)] = module
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.sr = rng.getrandbits(32)
    state.pr = rng.getrandbits(32)
    state.pc = FUNCTION
    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    reference(expected_ram, expected_state, stop_pc=TARGET)
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual_state))
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch in case {case}'
    assert actual_state.r[3] == TARGET and actual_state.pc == TARGET
    cases.append({'case': case, 'target': hex(TARGET), 'matched': True})

report = {
    'module': 'STG12.BIN', 'load_address': hex(LOAD_ADDRESS),
    'address': hex(FUNCTION), 'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'decoded_module_sha256': hashlib.sha256(module).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_stg12_thunk_ext_fun_8c01ae60_0c915a80.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against the retail SH-4 interpreter at the loaded target',
    'scope': 'MOV.L loads the fixed literal at 0x0c915bc0 into R3; JMP @R3 transfers to 0x8c01ae60 with a NOP delay slot.',
    'limitations': ['Target body behavior is outside this six-byte overlay thunk.', 'Verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'target': hex(TARGET), 'literal': hex(LITERAL), 'cases': cases,
}
(PROJECT / 'analysis' / 'stg12_thunk_ext_fun_8c01ae60_0c915a80_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} STG12 thunk RAM/CPU comparisons passed')
