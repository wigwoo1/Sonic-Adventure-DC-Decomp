import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference


PROJECT = ROOT / 'outputs' / 'sonic-adventure'
FUNCTION = 0x8C644DE4
TARGET = 0x8C644D98
BASE = 0x8C010000
LENGTH = 8
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('09 d4 08 d5 d6 af 01 e6')

native = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' /
                         'tail_wrapper_8c644de4.dll')).sa_tail_wrapper_8c644de4
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

cases = 0
for case in range(128):
    rng = random.Random(FUNCTION + case)
    expected_ram = bytearray(RAM_SIZE)
    expected_ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    if case % 2:
        struct.pack_into('<I', expected_ram, 0x644e0c, rng.getrandbits(32))
        struct.pack_into('<I', expected_ram, 0x644e08, rng.getrandbits(32))
    actual_ram = bytearray(expected_ram)
    actual_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)

    expected = State()
    expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
    expected.sr = rng.getrandbits(32)
    expected.pr = rng.choice([0x8C000100, 0x8C7FF000, 0x8CFED000])
    expected.pc = FUNCTION
    actual = State.from_buffer_copy(expected)

    reference(expected_ram, expected, stop_pc=TARGET)
    native(actual_view, ctypes.byref(actual))
    assert bytes(actual) == bytes(expected), f'CPU mismatch case={case}'
    assert actual_ram == expected_ram, f'RAM mismatch case={case}'
    assert actual.pc == TARGET
    cases += 1

source = PROJECT / 'source' / 'sa_tail_wrapper_8c644de4.c'
report = {
    'module': '1ST_READ.BIN',
    'load_address': hex(BASE),
    'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': cases,
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state',
    'scope': 'Both PC-relative literal loads, R4/R5 arguments, R6 delay-slot constant, and tail-transfer PC',
    'limitations': ['Behavior of the tail target at 0x8c644d98 is outside this wrapper.'],
    'cases': cases,
}
(PROJECT / 'analysis' / 'tail_wrapper_8c644de4_verification.json').write_text(
    json.dumps(report, indent=2) + '\n')
print(f'{cases} tail-wrapper full RAM/CPU comparisons passed')
