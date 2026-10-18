import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C110C9C
LENGTH = 28
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'store_u32_be_8c110c9c.dll'))
native = library.sa_store_u32_be_8c110c9c
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE - 4, hex(address)
    return result

rng = random.Random(FUNCTION)
cases = []
values = [0, 1, 0x7F, 0x80, 0xFFFF, 0x10000, 0x7FFFFFFF, 0x80000000, 0x80000001, 0xFFFFFFFF, 0x12345678]
for case in range(96):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    src = 0x8C740000 + case * 0x100
    dst = [0x8C750000 + case * 4, src, src + 1, src + 2, src + 3, src - 1, FUNCTION][case % 7]
    value = values[case % len(values)] if case < 55 else rng.getrandbits(32)
    source_at = offset(src)
    ram[source_at:source_at + 4] = value.to_bytes(4, 'little')
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4], state.r[5] = dst, src
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    reference(expected_ram, expected_state)
    actual_ram = bytearray(ram)
    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(view, ctypes.byref(actual_state))
    assert actual_ram == expected_ram, f'RAM mismatch case={case} value={value:#x} src={src:#x} dst={dst:#x}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch case={case}'
    cases.append({'case': case, 'input_u32': hex(value), 'source': hex(src), 'destination': hex(dst), 'ram_and_cpu_match': True})

source = PROJECT / 'source' / 'sa_store_u32_be_8c110c9c.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH), 'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(), 'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state',
    'scope': 'Exact 28-byte 32-bit-to-big-endian-byte helper; tests boundary words, all source/destination overlaps, code-backed destinations, complete RAM and CPU state.',
    'limitations': ['The caller-specific meaning of the converted field remains unidentified.'],
    'cases': cases,
}
(PROJECT / 'analysis' / 'store_u32_be_8c110c9c_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} 32-bit big-endian store RAM/CPU comparisons passed')
