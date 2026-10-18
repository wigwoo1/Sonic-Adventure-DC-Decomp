import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C10BA1E
LENGTH = 22
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
assert int.from_bytes(binary[0x8C10BA38 - BASE:0x8C10BA3C - BASE], 'little') == 0x53454741
assert int.from_bytes(binary[0x8C10BA40 - BASE:0x8C10BA44 - BASE], 'little') == 0x8C008200
assert int.from_bytes(binary[0x8C10BA44 - BASE:0x8C10BA48 - BASE], 'little') == 0x8C008204

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'platform_signature_value_8c10ba1e.dll'))
native = library.sa_platform_signature_value_8c10ba1e
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE, hex(address)
    return result

rng = random.Random(FUNCTION)
MAGIC = 0x53454741
cases = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    value = rng.getrandbits(32)
    signature = MAGIC if case % 2 == 0 else rng.choice([0, 1, 0xFFFFFFFF, rng.getrandbits(32)])
    if case == 1:
        signature = MAGIC ^ 0x01000000
    value_at, signature_at = offset(0x8C008200), offset(0x8C008204)
    ram[value_at:value_at + 4] = value.to_bytes(4, 'little')
    ram[signature_at:signature_at + 4] = signature.to_bytes(4, 'little')

    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
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
    assert bytes(actual_state) == bytes(expected_state), f'CPU state mismatch in case {case}, signature={signature:#x}'
    cases.append({'case': case, 'signature': hex(signature), 'platform_value': hex(value), 'ram_and_cpu_match': True})

source_path = PROJECT / 'source' / 'sa_platform_signature_value_8c10ba1e.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source_path.read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against the retail SH-4 interpreter',
    'scope': 'AGES signature check, status-dependent platform-value read, return value, and final register/flag effects.',
    'limitations': [
        'The meaning and lifetime of the platform value are not established by this helper alone.',
        'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'
    ],
    'cases': cases,
}
(PROJECT / 'analysis' / 'platform_signature_value_8c10ba1e_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} platform-signature complete RAM/CPU comparisons passed')
