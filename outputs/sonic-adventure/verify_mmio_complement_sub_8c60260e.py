import ctypes
import hashlib
import json
import random
import struct
from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C60260E
LENGTH = 10
POINTER_LITERAL = 0x8C60271C
DEFAULT_MMIO = 0xFFD8000C
binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'mmio_complement_sub_8c60260e.dll'
library = ctypes.CDLL(str(library_path))
CALLBACK = ctypes.CFUNCTYPE(ctypes.c_uint32, ctypes.c_void_p, ctypes.c_uint32)
native = library.sa_mmio_complement_sub_8c60260e
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State), ctypes.c_void_p, CALLBACK]
native.restype = None
rng = random.Random(FUNCTION)
reports = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    address = DEFAULT_MMIO if case % 4 else DEFAULT_MMIO + (case % 16) * 4
    struct.pack_into('<I', ram, POINTER_LITERAL - 0x8C000000, address)
    value = [0, 1, 0xffffffff, 0x80000000, 0x7fffffff][case % 5] if case < 20 else rng.getrandbits(32)
    observed = {'address': None, 'calls': 0}
    context_value = ctypes.c_uint32(value)
    @CALLBACK
    def read_register(context, requested_address):
        observed['address'] = requested_address
        observed['calls'] += 1
        return ctypes.cast(context, ctypes.POINTER(ctypes.c_uint32)).contents.value
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    expected_ram = bytearray(ram)
    expected = State.from_buffer_copy(state)
    reference(expected_ram, expected, mmio={address: value})
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual), ctypes.byref(context_value), read_register)
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual) == bytes(expected), (
        f'CPU mismatch in case {case}: '
        f'r={[(i, hex(actual.r[i]), hex(expected.r[i])) for i in range(16) if actual.r[i] != expected.r[i]]}, '
        f'sr={actual.sr:#x}/{expected.sr:#x}, pc={actual.pc:#x}/{expected.pc:#x}'
    )
    assert observed == {'address': address, 'calls': 1}, f'MMIO callback mismatch in case {case}: {observed}'
    reports.append({'case': case, 'mmio_address': hex(address), 'mmio_value': hex(value),
                    'ram_cpu_and_callback_match': True})

report = {
    'module': '1ST_READ.BIN',
    'load_address': hex(BASE),
    'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'binary_sha256': hashlib.sha256(binary).hexdigest(),
    'source_sha256': hashlib.sha256((ROOT / 'source' / 'sa_mmio_complement_sub_8c60260e.c').read_bytes()).hexdigest(),
    'cases_passed': len(reports),
    'comparison_level': 'Complete 16 MiB RAM and all GPRs, SR, PR, and return PC compared against the retail SH-4 interpreter; host MMIO callback address/count also checked.',
    'scope': 'Tests literal-loaded MMIO addresses, exactly one bus read, modulo-32-bit -1-value result, R2/R3/R0 register effects, and boundary values.',
    'limitations': ['The register is modeled through a host callback; its Dreamcast hardware semantics and game-level purpose require hardware or authoritative peripheral documentation.'],
    'cases': reports,
}
(ROOT / 'analysis' / 'mmio_complement_sub_8c60260e_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{len(reports)} MMIO complement/subtract full CPU/RAM comparisons passed")
