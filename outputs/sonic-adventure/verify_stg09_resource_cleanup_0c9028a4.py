"""Compare the recovered STG09 resource cleanup wrapper against retail bytes."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference
from verify_fpu_cleanup_transfer import Fpu

PROJECT = ROOT / 'outputs/sonic-adventure'
LOAD, ADDRESS, LENGTH = 0x0C900000, 0x0C9028A4, 104
RAM_BASE, RAM_SIZE = 0x0C000000, 16 * 1024 * 1024
MODULE = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
SPAN = MODULE[ADDRESS - LOAD:ADDRESS - LOAD + LENGTH]
SERVICE_TARGETS = (0x8C608CCC, 0x8C639C70, 0x8C63A804, 0x8C63A1CC,
                   0x8C639EC8, 0x8C03700C, 0x8C639B98)
Service = ctypes.CFUNCTYPE(ctypes.c_int, ctypes.c_void_p, ctypes.c_uint32,
                           ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                           ctypes.POINTER(Fpu))

def offset(address):
    value = (address & 0x1FFFFFFF) - RAM_BASE
    assert 0 <= value < RAM_SIZE - 64, hex(address)
    return value

library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/stg09_resource_cleanup_0c9028a4.dll'))
native = library.sa_stg09_resource_cleanup_0c9028a4
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                   ctypes.POINTER(Fpu), Service, ctypes.c_void_p]
native.restype = None

def prepare(case, rng, refcount, field1c, field14):
    ram = bytearray([0xA5]) * RAM_SIZE
    ram[LOAD - RAM_BASE:LOAD - RAM_BASE + len(MODULE)] = MODULE
    object_address = 0x0C200000 + case * 0x100
    entity = 0x0C400000 + case * 0x100
    struct.pack_into('<I', ram, offset(object_address + 0x20), entity)
    ram[offset(entity)] = refcount & 0xFF
    struct.pack_into('<I', ram, offset(entity + 0x14), field14)
    struct.pack_into('<I', ram, offset(entity + 0x1C), field1c)
    cpu = State()
    cpu.r[:] = [rng.getrandbits(32) for _ in range(16)]
    cpu.r[4] = object_address
    cpu.r[14] = 0x0C800000 + case * 4
    cpu.r[15] = 0x0CF00000 + case * 0x100
    cpu.sr = rng.getrandbits(32)
    cpu.pr = 0x0CFE0000 + case * 4
    cpu.pc = ADDRESS
    fpu = Fpu()
    for bank in range(2):
        fpu.banks[bank][:] = [rng.getrandbits(32) for _ in range(16)]
    fpu.fpscr = 0x40001 | (rng.randrange(2) << 21)
    fpu.fpul = rng.getrandbits(32)
    return ram, cpu, fpu, object_address, entity

def reference_hook(target, fpu, calls, scratch_base, service_seed):
    def hook(memory, regs, sr):
        calls.append((target, tuple(regs), sr, regs[14], regs[15]))
        index = len(calls) - 1
        value = (service_seed ^ target ^ index) & 0xFFFFFFFF
        struct.pack_into('<I', memory, offset(scratch_base + index * 4), value)
        for reg in (1, 2, 3, 5, 6, 7):
            regs[reg] = (0xD0000000 | (index << 8) | reg) & 0xFFFFFFFF
        regs[0] = (0x80000000 | index) & 0xFFFFFFFF
        bank = (fpu.fpscr >> 21) & 1
        fpu.banks[bank][0] ^= (0x10203040 + index) & 0xFFFFFFFF
        fpu.fpul ^= (0xA5000000 | index)
        return sr ^ (0x10 + index)
    return hook

def native_service(call_log, service_seed, scratch_base):
    @Service
    def service(_context, target, memory_ptr, cpu_ptr, fpu_ptr):
        cpu, fpu = cpu_ptr.contents, fpu_ptr.contents
        call_log.append((target, tuple(cpu.r), cpu.sr, cpu.r[14], cpu.r[15]))
        index = len(call_log) - 1
        value = (service_seed ^ target ^ index) & 0xFFFFFFFF
        at = offset(scratch_base + index * 4)
        for byte in range(4):
            memory_ptr[at + byte] = (value >> (8 * byte)) & 0xFF
        for reg in (1, 2, 3, 5, 6, 7):
            cpu.r[reg] = (0xD0000000 | (index << 8) | reg) & 0xFFFFFFFF
        cpu.r[0] = (0x80000000 | index) & 0xFFFFFFFF
        bank = (fpu.fpscr >> 21) & 1
        fpu.banks[bank][0] ^= (0x10203040 + index) & 0xFFFFFFFF
        fpu.fpul ^= (0xA5000000 | index)
        cpu.sr ^= 0x10 + index
        return 0
    return service

rng = random.Random(0x0C9028A4)
records = []
branch_counts = {'zero_refcount': 0, 'nonzero_refcount': 0,
                 'field1c_present': 0, 'field14_present': 0}
for case in range(64):
    refcount = 0 if case % 2 == 0 else (0x7F, 0x80, 0xFF, 1, 2, 0x40, 0x81, 0xFE)[case % 8]
    field1c = 0 if case % 4 in (0, 1) else (0x0C500000 + case * 4)
    field14 = 0 if case % 4 in (0, 2) else (0x0C600000 + case * 4)
    ram, initial, initial_fpu, object_address, entity = prepare(
        case, rng, refcount, field1c, field14)
    expected_ram, actual_ram = bytearray(ram), bytearray(ram)
    expected = State.from_buffer_copy(initial)
    actual = State.from_buffer_copy(initial)
    expected_fpu = Fpu.from_buffer_copy(initial_fpu)
    actual_fpu = Fpu.from_buffer_copy(initial_fpu)
    calls_expected, calls_actual = [], []
    scratch_base = 0x0C700000 + case * 0x100
    service_seed = rng.getrandbits(32)
    hooks = {target: reference_hook(target, expected_fpu, calls_expected,
                                    scratch_base, service_seed)
             for target in SERVICE_TARGETS}
    steps = reference(expected_ram, expected, hooks=hooks, fpu=expected_fpu,
                      instruction_budget=256)
    actual_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    callback = native_service(calls_actual, service_seed, scratch_base)
    native(actual_view, ctypes.byref(actual), ctypes.byref(actual_fpu), callback, None)
    assert calls_actual == calls_expected, (
        f'call boundary mismatch case={case}: native={calls_actual!r}; retail={calls_expected!r}')
    assert bytes(actual) == bytes(expected), (
        f'CPU mismatch case={case}: native={[hex(x) for x in actual.r] + [hex(actual.sr),hex(actual.pr),hex(actual.pc)]}; '
        f'retail={[hex(x) for x in expected.r] + [hex(expected.sr),hex(expected.pr),hex(expected.pc)]}')
    assert bytes(actual_fpu) == bytes(expected_fpu), f'FPU mismatch case={case}'
    if actual_ram != expected_ram:
        diffs=[(i,expected_ram[i],actual_ram[i]) for i in range(len(ram)) if expected_ram[i]!=actual_ram[i]][:20]
        raise AssertionError(f'RAM mismatch case={case} diffs={diffs!r}')
    branch_counts['zero_refcount' if refcount == 0 else 'nonzero_refcount'] += 1
    branch_counts['field1c_present'] += int(field1c != 0)
    branch_counts['field14_present'] += int(field14 != 0)
    records.append({'case': case, 'refcount': refcount, 'field1c': bool(field1c),
                    'field14': bool(field14), 'retail_steps': steps,
                    'service_calls': len(calls_expected), 'full_state_match': True})

source = PROJECT / 'source/sa_stg09_resource_cleanup_0c9028a4.c'
report = {
    'module': 'STG09.BIN', 'load_address': hex(LOAD), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(SPAN).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': len(records), 'branch_counts': branch_counts,
    'comparison_level': 'Complete 16 MiB RAM and GPR/SR/PR/PC/FPSCR/FPUL/both FPU-bank comparisons, including exact service targets, entry registers, stack, PR, and callback order.',
    'scope': 'STG09 cleanup dispatcher: selector tail-call, optional entity-field releases, zero/nonzero reference-count release, decrement, saved register restoration, and final tail transfer.',
    'limitations': ['External cleanup/resource services are injected boundary callbacks; their callees are outside this routine.'],
    'cases': records,
}
(PROJECT / 'analysis/common-overlays/STG09.BIN/resource_cleanup_0c9028a4_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{len(records)} STG09 resource-cleanup full RAM/CPU/FPU/service-boundary comparisons passed")
