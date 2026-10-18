"""Full-state verification for the STG09 clamped global-value update leaf."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference
from verify_fpu_cleanup_transfer import Fpu

PROJECT = ROOT / 'outputs/sonic-adventure'
LOAD, ADDRESS, LENGTH = 0x0C900000, 0x0C91DCAE, 36
RAM_BASE, RAM_SIZE = 0x0C000000, 16 * 1024 * 1024
VALUE_LITERAL, FLAG_LITERAL = 0x0C91DD10, 0x0C91DD18
MODULE = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
SPAN = MODULE[ADDRESS - LOAD:ADDRESS - LOAD + LENGTH]
VALUES = [0x00000000, 0x80000000, 0x3F000000, 0x3F800000,
          0x3FC00000, 0xBF000000, 0x7FC00001, 0xFF800000]

def offset(address):
    value = (address & 0x1FFFFFFF) - RAM_BASE
    assert 0 <= value < RAM_SIZE - 16, hex(address)
    return value

library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/stg09_clamped_global_value_update_0c91dcae.dll'))
native = library.sa_stg09_clamped_global_value_update_0c91dcae
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State), ctypes.POINTER(Fpu)]
native.restype = None

rng = random.Random(0x0C91DCAE)
records = []
branches = {'over_one': 0, 'below_zero': 0, 'unchanged': 0, 'updated': 0}
cases = 0
for sz in range(2):
    for active in range(2):
        for value in VALUES:
            for old_value in VALUES[::2]:
                ram = bytearray([0xA5]) * RAM_SIZE
                ram[LOAD - RAM_BASE:LOAD - RAM_BASE + len(MODULE)] = MODULE
                value_pointer = 0x0C200000 + cases * 0x40
                flag_pointer = value_pointer + 0x20
                struct.pack_into('<I', ram, offset(VALUE_LITERAL), value_pointer)
                struct.pack_into('<I', ram, offset(FLAG_LITERAL), flag_pointer)
                ram[offset(value_pointer):offset(value_pointer) + 8] = rng.randbytes(8)
                ram[offset(flag_pointer):offset(flag_pointer) + 16] = rng.randbytes(16)
                # Select representative finite, signed-zero, infinity and quiet-NaN input values.
                fpu = Fpu()
                for bank in range(2):
                    fpu.banks[bank][:] = [rng.getrandbits(32) for _ in range(16)]
                fpu.fpscr = 0x40001 | (sz << 20) | (active << 21)
                fpu.fpul = rng.getrandbits(32)
                fpu.banks[active][4] = value
                if sz == 0:
                    struct.pack_into('<I', ram, offset(value_pointer), old_value)
                else:
                    struct.pack_into('<2I', ram, offset(value_pointer), old_value,
                                     rng.choice(VALUES))

                expected_ram, actual_ram = bytearray(ram), bytearray(ram)
                initial = State()
                initial.r[:] = [0x0C200000 + rng.randrange(0x1000) for _ in range(16)]
                initial.pc = ADDRESS
                initial.pr = 0x0CFE0000 + cases * 4
                initial.sr = rng.getrandbits(32)
                expected_cpu, actual_cpu = State.from_buffer_copy(initial), State.from_buffer_copy(initial)
                expected_fpu, actual_fpu = Fpu.from_buffer_copy(fpu), Fpu.from_buffer_copy(fpu)
                trace = []
                reference(expected_ram, expected_cpu, fpu=expected_fpu,
                          stop_pc=initial.pr, instruction_budget=64,
                          trace=lambda pc, word, regs, sr, pr, fp: trace.append((pc, word)))
                view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
                native(view, ctypes.byref(actual_cpu), ctypes.byref(actual_fpu))

                if actual_ram != expected_ram:
                    first = next(i for i, (a, e) in enumerate(zip(actual_ram, expected_ram)) if a != e)
                    raise AssertionError(f'RAM mismatch case={cases} first_offset={first:#x} '
                                         f'actual={actual_ram[first:first+16].hex()} '
                                         f'expected={expected_ram[first:first+16].hex()} '
                                         f'R2/R3/R4={expected_cpu.r[2]:08x}/{expected_cpu.r[3]:08x}/{expected_cpu.r[4]:08x}')
                assert bytes(actual_cpu) == bytes(expected_cpu), f'CPU mismatch case={cases}'
                assert bytes(actual_fpu) == bytes(expected_fpu), f'FPU mismatch case={cases}'
                pcs = {pc for pc, _ in trace}
                branches['over_one'] += int(0x0C91DCB6 in pcs)
                branches['below_zero'] += int(0x0C91DCBC in pcs)
                changed = expected_ram[offset(value_pointer):offset(value_pointer) + (8 if sz else 4)] != ram[offset(value_pointer):offset(value_pointer) + (8 if sz else 4)]
                flag_changed = expected_ram[offset(flag_pointer):offset(flag_pointer) + 4] != ram[offset(flag_pointer):offset(flag_pointer) + 4]
                branches['updated'] += int(changed and flag_changed)
                branches['unchanged'] += int(not changed and not flag_changed)
                cases += 1
                records.append({'case': cases - 1, 'sz': sz, 'active_bank': active,
                                'input_bits': hex(value), 'old_value_bits': hex(old_value),
                                'instructions': len(trace), 'full_ram_cpu_fpu_match': True})

assert branches['over_one'] > 0 and branches['below_zero'] > 0
assert branches['updated'] > 0 and branches['unchanged'] > 0
source = PROJECT / 'source/sa_stg09_clamped_global_value_update_0c91dcae.c'
report = {
    'module': 'STG09.BIN', 'load_address': hex(LOAD), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(SPAN).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': cases, 'branch_counts': branches,
    'comparison_level': 'Complete 16 MiB RAM and complete GPR/SR/PR/PC/FPSCR/FPUL/both-bank FPU comparisons against the retail subset interpreter.',
    'scope': 'FPU clamp/update leaf at 0x0c91dcae: FLDI1, FCMP.GT/EQ, BF/S delay-slot behavior, FMOV register/memory forms, pointer literal loads, conditional change-flag store, and RTS.',
    'limitations': ['FPSCR.PR=1 is not tested because the retail leaf begins with a single-precision FLDI instruction and the subset interpreter models its PR=1 illegal-operation trap as unsupported.',
                    'Enabled invalid-operation exceptions and DN0 denormal traps are outside the supported retail subset comparison.'],
    'cases': records,
}
(PROJECT / 'analysis/common-overlays/STG09.BIN/clamped_global_value_update_0c91dcae_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{cases} full RAM/CPU/FPU comparisons passed; branches {branches}")
