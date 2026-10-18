import ctypes
import hashlib
import json
import random
import struct
from types import SimpleNamespace

from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C09E760
LENGTH = 6
TARGET = 0x8C66C314
binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'global_fpu_store_8c09e760.dll'
library = ctypes.CDLL(str(library_path))

class FpuState(ctypes.Structure):
    _fields_ = [('fpscr', ctypes.c_uint32), ('banks', (ctypes.c_uint32 * 16) * 2)]

native = library.sa_global_fpu_store_8c09e760
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State), ctypes.POINTER(FpuState)]
native.restype = None
rng = random.Random(FUNCTION)
reports = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    # The retail literal targets executable-memory-backed global storage.
    for offset in range(16):
        ram[TARGET - 0x8C000000 + offset] = rng.getrandbits(8)
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    fpscr = ((case & 1) << 20) | (((case >> 1) & 1) << 21)
    fpu = FpuState()
    fpu.fpscr = fpscr
    py_fpu = SimpleNamespace(fpscr=fpscr, banks=[[rng.getrandbits(32) for _ in range(16)] for _ in range(2)])
    for bank in range(2):
        fpu.banks[bank][:] = py_fpu.banks[bank]
    expected_ram = bytearray(ram)
    expected = State.from_buffer_copy(state)
    reference(expected_ram, expected, fpu=py_fpu)
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual), ctypes.byref(fpu))
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual) == bytes(expected), (
        f'CPU mismatch in case {case}: '
        f'r={[(i, hex(actual.r[i]), hex(expected.r[i])) for i in range(16) if actual.r[i] != expected.r[i]]}, '
        f'sr={actual.sr:#x}/{expected.sr:#x}, pc={actual.pc:#x}/{expected.pc:#x}'
    )
    reports.append({'case': case, 'fpscr': hex(fpscr), 'ram_and_cpu_match': True})

report = {
    'module': '1ST_READ.BIN',
    'load_address': hex(BASE),
    'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'binary_sha256': hashlib.sha256(binary).hexdigest(),
    'source_sha256': hashlib.sha256((ROOT / 'source' / 'sa_global_fpu_store_8c09e760.c').read_bytes()).hexdigest(),
    'cases_passed': len(reports),
    'comparison_level': 'Complete 16 MiB RAM and all GPRs, SR, PR, and return PC compared against the retail SH-4 interpreter; FPU banks are varied across FR and DR modes.',
    'scope': 'Tests the PC-relative destination literal and delay-slot FMOV.S/FMOV.D stores for both FR banks and both FPSCR size modes.',
    'limitations': ['The FPU store instruction behavior is modeled by the project SH-4 interpreter and is not independently validated on Dreamcast hardware.', 'The purpose and higher-level ownership of global 0x8c66c314 remain unidentified.'],
    'cases': reports,
}
(ROOT / 'analysis' / 'global_fpu_store_8c09e760_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{len(reports)} global FPU store full RAM/CPU comparisons passed")
