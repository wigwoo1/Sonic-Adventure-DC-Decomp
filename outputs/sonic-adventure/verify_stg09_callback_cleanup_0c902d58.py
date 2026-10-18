"""Full-state differential tests for the 154-byte STG09 callback cleanup."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference
from verify_fpu_cleanup_transfer import Fpu

PROJECT = ROOT / 'outputs/sonic-adventure'
LOAD, ADDRESS, LENGTH = 0x0C900000, 0x0C902D58, 154
RAM_BASE, RAM_SIZE = 0x0C000000, 16 * 1024 * 1024
MODULE = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
SERVICE_TARGETS = (0x8C608CCC, 0x8C639C70, 0x8C63A804, 0x8C63A1CC,
                   0x8C639F5C, 0x8C639EC8, 0x8C03700C, 0x8C639B98)
Service = ctypes.CFUNCTYPE(ctypes.c_int, ctypes.c_void_p, ctypes.c_uint32,
                           ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                           ctypes.POINTER(Fpu))

def offset(address):
    value = (address & 0x1FFFFFFF) - RAM_BASE
    assert 0 <= value < RAM_SIZE - 64, hex(address)
    return value

library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/stg09_callback_cleanup_0c902d58.dll'))
native = library.sa_stg09_callback_cleanup_0c902d58
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                   ctypes.POINTER(Fpu), Service, ctypes.c_void_p]
native.restype = None

def prepare(case, rng, release_flag, reference_byte, optional_mask):
    ram = bytearray([0xA5]) * RAM_SIZE
    ram[LOAD - RAM_BASE:LOAD - RAM_BASE + len(MODULE)] = MODULE
    obj = 0x0C200000 + case * 0x100
    entity = 0x0C400000 + case * 0x100
    struct.pack_into('<I', ram, offset(obj + 0x20), entity)
    struct.pack_into('<H', ram, offset(entity + 4), release_flag)
    ram[offset(entity + 1)] = reference_byte
    fields = (0x1C, 0x18, 0x14)
    values = []
    for bit, field in enumerate(fields):
        value = 0x0C500000 + case * 0x100 + bit * 0x20 if optional_mask & (1 << bit) else 0
        struct.pack_into('<I', ram, offset(entity + field), value)
        values.append(value)
    cpu = State()
    cpu.r[:] = [rng.getrandbits(32) for _ in range(16)]
    cpu.r[4] = obj
    cpu.r[13] = 0x0C800000 + case * 4
    cpu.r[14] = 0x0C810000 + case * 4
    cpu.r[15] = 0x0CF00000 + case * 0x100
    cpu.sr = rng.getrandbits(32)
    cpu.pr = 0x0CFE0000 + case * 4
    cpu.pc = ADDRESS
    fpu = Fpu()
    for bank in range(2):
        fpu.banks[bank][:] = [rng.getrandbits(32) for _ in range(16)]
    fpu.fpscr = 0x40001 | (rng.randrange(2) << 21)
    fpu.fpul = rng.getrandbits(32)
    return ram, cpu, fpu, obj, entity, values

def mutate(memory, regs, sr, fpu, target, calls, scratch_base, service_seed, pr):
    index = len(calls)
    calls.append((target, tuple(regs), sr, pr, regs[13], regs[14], regs[15], fpu.fpscr, fpu.fpul))
    value = (service_seed ^ target ^ index) & 0xFFFFFFFF
    struct.pack_into('<I', memory, offset(scratch_base + index * 4), value)
    for reg in range(8):
        regs[reg] = (0xC3000000 | (index << 8) | reg) & 0xFFFFFFFF
    bank = (fpu.fpscr >> 21) & 1
    fpu.banks[bank][0] ^= (0x11223344 + index) & 0xFFFFFFFF
    fpu.fpul ^= (0x5C000000 | index)
    return sr ^ (0x31 + index)

rng = random.Random(0x0C902D58)
records = []
branch_counts = {'flag_off_zero': 0, 'flag_off_nonzero': 0,
                 'flag_on_zero': 0, 'flag_on_nonzero': 0,
                 'field_1c': 0, 'field_18': 0, 'field_14': 0}
for case in range(64):
    release_flag = 0x200 if (case // 8) & 1 else 0
    ref_values = (0, 1, 0x7F, 0x80, 0xFF, 2, 0x40, 0x81)
    reference_byte = ref_values[case % len(ref_values)]
    optional_mask = (case // 4) & 7
    ram, initial, initial_fpu, obj, entity, fields = prepare(
        case, rng, release_flag, reference_byte, optional_mask)
    expected_ram, actual_ram = bytearray(ram), bytearray(ram)
    expected, actual = State.from_buffer_copy(initial), State.from_buffer_copy(initial)
    expected_fpu = Fpu.from_buffer_copy(initial_fpu)
    actual_fpu = Fpu.from_buffer_copy(initial_fpu)
    calls_expected, calls_actual = [], []
    scratch_base, service_seed = 0x0C700000 + case * 0x100, rng.getrandbits(32)

    hooks = {}
    for target in SERVICE_TARGETS:
        def make_hook(service_target):
            def hook(memory, regs, sr):
                return mutate(memory, regs, sr, expected_fpu, service_target,
                              calls_expected, scratch_base, service_seed, expected.pr)
            return hook
        hooks[target] = make_hook(target)
    steps = reference(expected_ram, expected, hooks=hooks, fpu=expected_fpu,
                      instruction_budget=512)

    @Service
    def service(_context, target, memory_ptr, cpu_ptr, fpu_ptr):
        cpu, fpu = cpu_ptr.contents, fpu_ptr.contents
        index = len(calls_actual)
        calls_actual.append((target, tuple(cpu.r), cpu.sr, cpu.pr, cpu.r[13],
                             cpu.r[14], cpu.r[15], fpu.fpscr, fpu.fpul))
        value = (service_seed ^ target ^ index) & 0xFFFFFFFF
        at = offset(scratch_base + index * 4)
        for byte in range(4):
            memory_ptr[at + byte] = (value >> (8 * byte)) & 0xFF
        for reg in range(8):
            cpu.r[reg] = (0xC3000000 | (index << 8) | reg) & 0xFFFFFFFF
        bank = (fpu.fpscr >> 21) & 1
        fpu.banks[bank][0] ^= (0x11223344 + index) & 0xFFFFFFFF
        fpu.fpul ^= (0x5C000000 | index)
        cpu.sr ^= 0x31 + index
        return 0

    actual_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    native(actual_view, ctypes.byref(actual), ctypes.byref(actual_fpu), service, None)
    assert calls_actual == calls_expected, (
        f'callback boundary mismatch case={case}: native={calls_actual!r}; retail={calls_expected!r}')
    assert bytes(actual) == bytes(expected), f'CPU mismatch case={case}'
    assert bytes(actual_fpu) == bytes(expected_fpu), f'FPU mismatch case={case}'
    assert actual_ram == expected_ram, f'RAM mismatch case={case}'
    flag_on, ref_zero = bool(release_flag & 0x200), reference_byte == 0
    branch_counts[('flag_on_' if flag_on else 'flag_off_') + ('zero' if ref_zero else 'nonzero')] += 1
    for bit, field in enumerate(('field_1c', 'field_18', 'field_14')):
        branch_counts[field] += int(fields[bit] != 0)
    records.append({'case': case, 'release_flag_0x200': flag_on,
                    'reference_byte': hex(reference_byte), 'optional_field_mask': optional_mask,
                    'retail_steps': steps, 'service_calls': len(calls_expected),
                    'full_state_match': True})

source = PROJECT / 'source/sa_stg09_callback_cleanup_0c902d58.c'
span = MODULE[ADDRESS - LOAD:ADDRESS - LOAD + LENGTH]
report = {
    'module': 'STG09.BIN', 'load_address': hex(LOAD), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': len(records), 'branch_counts': branch_counts,
    'comparison_level': 'Complete 16 MiB RAM and GPR/SR/PR/PC/FPSCR/FPUL/both-FPU-bank comparisons; exact external call targets, callback entry registers, stack, PR, order, and callback effects.',
    'scope': 'STG09 callback cleanup: selector dispatch, three optional resource releases, flag/reference-byte-dependent cleanup, signed-byte decrement, callee-saved restoration, and final tail transfer.',
    'limitations': ['External resource and cleanup services are injected callbacks; their callee bodies are outside this wrapper.'],
    'cases': records,
}
(PROJECT / 'analysis/common-overlays/STG09.BIN/callback_cleanup_0c902d58_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{len(records)} STG09 callback-cleanup full RAM/CPU/FPU/service-boundary comparisons passed")
