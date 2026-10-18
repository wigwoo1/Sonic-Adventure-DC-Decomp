import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C0509B0
GLOBAL = 0x8C19E3A8
LITERAL = 0x8C050A20

binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + 8]
assert span == bytes.fromhex('1b d3 30 60 0b 00 0c 60')
assert int.from_bytes(binary[LITERAL - BASE:LITERAL - BASE + 4], 'little') == GLOBAL
binary_sha256 = hashlib.sha256(binary).hexdigest()

library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'global_u8_get_8c0509b0.dll'
native = ctypes.CDLL(str(library_path)).sa_global_u8_get_8c0509b0
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(0x8C0509B0)
values = [0, 1, 0x7f, 0x80, 0x81, 0xff]
global_at = (GLOBAL & 0x1FFFFFFF) - 0x0C000000
cases = 0
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    ram[global_at] = values[case % len(values)] if case < len(values) else rng.randrange(256)
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.sr = rng.choice([0, 1, 0x70000000, 0xF0000001, rng.getrandbits(32)])
    state.pr = rng.choice([0x8CFED000, 0x8C010000, GLOBAL, GLOBAL + 1])
    state.pc = FUNCTION

    expected_ram = bytearray(ram)
    expected = State.from_buffer_copy(state)
    reference(expected_ram, expected)
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual))
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual) == bytes(expected), f'CPU mismatch in case {case}'
    cases += 1

report = {
    'cases_passed': cases,
    'comparison_level': 'Complete 16 MiB RAM and all GPRs, SR, PR, and return PC compared against the retail SH-4 interpreter.',
    'scope': 'The eight-byte routine at 0x8c0509b0 loads the fixed byte address from its PC-relative literal, reads a byte, then zero-extends R0 in the RTS delay slot. Tests cover all byte values by fixed boundaries plus random sampling and complete CPU/RAM equality.',
    'limitations': ['Uses the project SH-4 interpreter rather than Dreamcast hardware or end-to-end gameplay.', 'The global byte’s higher-level meaning remains unidentified.'],
    'binary_sha256': binary_sha256,
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'literal_address': hex(LITERAL),
    'literal_value': hex(GLOBAL),
    'source_sha256': hashlib.sha256((ROOT / 'source' / 'sa_global_u8_get_8c0509b0.c').read_bytes()).hexdigest(),
    'routines': [{
        'module': '1ST_READ.BIN',
        'load_address': hex(BASE),
        'address': hex(FUNCTION),
        'end_exclusive': hex(FUNCTION + 8),
        'retail_span_sha256': hashlib.sha256(span).hexdigest(),
        'cases_passed': cases,
    }],
}
(ROOT / 'analysis' / 'global_u8_get_8c0509b0_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{cases} global unsigned-byte getter full RAM/CPU comparisons passed')
