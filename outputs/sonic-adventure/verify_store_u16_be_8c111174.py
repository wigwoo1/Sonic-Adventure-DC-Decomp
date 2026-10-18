import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C111174
LENGTH = 30
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'store_u16_be_8c111174.dll'))
native = library.sa_store_u16_be_8c111174
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE - 2, hex(address)
    return result

rng = random.Random(FUNCTION)
cases = []
values = [0, 1, 0x7F, 0x80, 0xFF, 0x100, 0x7FFF, 0x8000, 0x8001, 0xFFFF]
for case in range(96):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    src = 0x8C740000 + case * 0x100
    dst = [0x8C750000 + case * 2, src, src + 1, src - 1, FUNCTION][case % 5]
    raw = values[case % len(values)] if case < 50 else rng.getrandbits(16)
    source_at = offset(src)
    ram[source_at:source_at + 2] = raw.to_bytes(2, 'little')
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
    assert actual_ram == expected_ram, f'RAM mismatch case={case} value={raw:#x} src={src:#x} dst={dst:#x}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch case={case}: rdiff={[(i,hex(actual_state.r[i]),hex(expected_state.r[i])) for i in range(16) if actual_state.r[i]!=expected_state.r[i]]} sr={actual_state.sr:#x}/{expected_state.sr:#x}'
    cases.append({'case': case, 'input_u16': hex(raw), 'source': hex(src), 'destination': hex(dst), 'ram_and_cpu_match': True})

source = PROJECT / 'source' / 'sa_store_u16_be_8c111174.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH), 'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(), 'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state',
    'scope': 'Exact 30-byte 16-bit-to-big-endian-byte helper; tests signed halfword boundaries, aliasing/overlap, code-backed destinations, all RAM and CPU registers.',
    'limitations': ['The caller-specific meaning of the converted field remains unidentified.'],
    'cases': cases,
}
(PROJECT / 'analysis' / 'store_u16_be_8c111174_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} 16-bit big-endian store RAM/CPU comparisons passed')
