"""Full-state differential verification for the STG09 global FPU return leaf."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference
from verify_fpu_cleanup_transfer import Fpu

PROJECT = ROOT / 'outputs/sonic-adventure'
LOAD, ADDRESS, LENGTH = 0x0C900000, 0x0C91DCA8, 6
LITERAL, RAM_BASE, RAM_SIZE = 0x0C91DD10, 0x0C000000, 16 * 1024 * 1024
MODULE = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
SPAN = MODULE[ADDRESS - LOAD:ADDRESS - LOAD + LENGTH]

def offset(address):
    value = (address & 0x1FFFFFFF) - RAM_BASE
    assert 0 <= value < RAM_SIZE - 8, hex(address)
    return value

library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/stg09_float_global_return_0c91dca8.dll'))
native = library.sa_stg09_float_global_return_0c91dca8
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State), ctypes.POINTER(Fpu)]
native.restype = None

rng = random.Random(0x0C91DCA8)
cases = 0
for sz in range(2):
    for active in range(2):
        for pr in range(2):
            for case in range(4):
                ram = bytearray([0xA5]) * RAM_SIZE
                ram[LOAD - RAM_BASE:LOAD - RAM_BASE + len(MODULE)] = MODULE
                pointer = 0x0C200000 + cases * 8
                ram[offset(LITERAL):offset(LITERAL) + 4] = pointer.to_bytes(4, 'little')
                ram[offset(pointer):offset(pointer) + 8] = bytes(rng.getrandbits(8) for _ in range(8))
                expected_ram, actual_ram = bytearray(ram), bytearray(ram)
                initial = State()
                initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
                initial.sr = rng.getrandbits(32)
                initial.pr = 0x0CFE0000 + cases * 4
                initial.pc = ADDRESS
                expected_cpu, actual_cpu = State.from_buffer_copy(initial), State.from_buffer_copy(initial)
                fpu = Fpu()
                for bank in range(2):
                    fpu.banks[bank][:] = [rng.getrandbits(32) for _ in range(16)]
                fpu.fpscr = 0x40001 | (active << 21) | (sz << 20) | (pr << 19)
                fpu.fpul = rng.getrandbits(32)
                expected_fpu, actual_fpu = Fpu.from_buffer_copy(fpu), Fpu.from_buffer_copy(fpu)

                reference(expected_ram, expected_cpu, fpu=expected_fpu,
                          stop_pc=initial.pr, instruction_budget=32)
                view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
                native(view, ctypes.byref(actual_cpu), ctypes.byref(actual_fpu))

                assert actual_ram == expected_ram, f'RAM mismatch case={cases}'
                assert bytes(actual_cpu) == bytes(expected_cpu), f'CPU mismatch case={cases}'
                assert bytes(actual_fpu) == bytes(expected_fpu), f'FPU mismatch case={cases}'
                assert actual_cpu.r[3] == pointer, f'global pointer load mismatch case={cases}'
                cases += 1

source = PROJECT / 'source/sa_stg09_float_global_return_0c91dca8.c'
report = {
    'module': 'STG09.BIN', 'load_address': hex(LOAD), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(SPAN).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': cases,
    'comparison_level': 'Complete 16 MiB RAM and complete GPR/SR/PR/PC/FPSCR/FPUL/both FPU-bank comparisons against the retail instruction interpreter.',
    'scope': 'PC-relative global-pointer load, FMOV.S/FMOV.D return-register transfer, and RTS return for FPSCR.SZ=0/1 and both active FPU banks.',
    'limitations': ['The SH-4 subset interpreter is not a complete hardware emulator; this leaf uses only the modeled MOV.L, RTS, and FMOV operations.'],
}
path = PROJECT / 'analysis/common-overlays/STG09.BIN/float_global_return_0c91dca8_verification.json'
path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{cases} complete RAM/CPU/FPU comparisons passed across SZ, active-bank, and PR states')
