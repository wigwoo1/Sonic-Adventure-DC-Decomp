import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C10F9A8
LENGTH = 14
RAM_SIZE = 16 * 1024 * 1024
HARDWARE_REGISTER = 0xA05F688C
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('03 d2 22 60 01 c8 29 03 32 24 0b 00 00 e0')
assert int.from_bytes(binary[0x8C10F9B8 - BASE:0x8C10F9B8 - BASE + 4], 'little') == HARDWARE_REGISTER

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'read_hardware_flag_boolean_8c10f9a8.dll'))
native = library.sa_read_hardware_flag_boolean_8c10f9a8
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State), ctypes.c_uint32]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE, hex(address)
    return result

rng = random.Random(FUNCTION)
flags = [0, 1, 2, 3, 0xFFFFFFFF] + [rng.getrandbits(32) for _ in range(91)]
cases = []
for case, flag in enumerate(flags):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    destination = 0x8C200000 + rng.randrange(0x800000 // 4) * 4
    at = offset(destination)
    ram[at:at + 4] = rng.randbytes(4)
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = destination
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    mmio = {HARDWARE_REGISTER: flag}
    reference(expected_ram, expected_state, mmio=dict(mmio))
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual_state), flag)
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch in case {case}'
    expected_boolean = int((flag & 1) == 0)
    assert int.from_bytes(actual_ram[at:at + 4], 'little') == expected_boolean
    cases.append({'case': case, 'hardware_register_value': hex(flag), 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_read_hardware_flag_boolean_8c10f9a8.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state with MMIO input against the retail SH-4 interpreter',
    'scope': 'Exact hardware word read at A05F688C, inverse low-bit Boolean output store, T flag and zero return value.',
    'limitations': ['The hardware register meaning and physical-device behavior have not been established.', 'Differential verification uses the project SH-4 interpreter with explicit MMIO values, not Dreamcast hardware or whole-game execution.'],
    'hardware_register': hex(HARDWARE_REGISTER), 'cases': cases,
}
(PROJECT / 'analysis' / 'read_hardware_flag_boolean_8c10f9a8_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} hardware flag Boolean RAM/CPU/MMIO comparisons passed')
