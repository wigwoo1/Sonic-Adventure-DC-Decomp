"""Compare the STG09 indirect resource tail wrapper against retail instructions."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference
from verify_fpu_cleanup_transfer import Fpu

PROJECT = ROOT / 'outputs/sonic-adventure'
LOAD, ADDRESS, LENGTH = 0x0C900000, 0x0C905180, 24
RAM_BASE, RAM_SIZE = 0x0C000000, 16 * 1024 * 1024
MODULE = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
SPAN = MODULE[ADDRESS - LOAD:ADDRESS - LOAD + LENGTH]
FIRST_TARGET, TAIL_TARGET = 0x8C052940, 0x8C052A5E
SERVICE_TARGETS = (FIRST_TARGET, TAIL_TARGET)
Service = ctypes.CFUNCTYPE(ctypes.c_int, ctypes.c_void_p, ctypes.c_uint32,
                           ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                           ctypes.POINTER(Fpu))

def offset(address):
    value = (address & 0x1FFFFFFF) - RAM_BASE
    assert 0 <= value < RAM_SIZE - 64, hex(address)
    return value

library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/stg09_indirect_resource_tail_0c905180.dll'))
native = library.sa_stg09_indirect_resource_tail_0c905180
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                   ctypes.POINTER(Fpu), Service, ctypes.c_void_p]
native.restype = None

def prepare(case, rng):
    ram = bytearray([0xA5]) * RAM_SIZE
    ram[LOAD - RAM_BASE:LOAD - RAM_BASE + len(MODULE)] = MODULE
    object_address = 0x0C200000 + case * 0x100
    entity = 0x0C400000 + case * 0x100
    resource = 0x0C500000 + case * 0x100
    struct.pack_into('<I', ram, offset(object_address + 0x20), entity)
    struct.pack_into('<I', ram, offset(entity + 8), resource)
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
    return ram, cpu, fpu, object_address, resource

def mutate(memory, regs, sr, fpu, target, calls, scratch_base, service_seed):
    index = len(calls)
    calls.append((target, tuple(regs), sr, regs[14], regs[15], regs[4], regs[5], regs[3]))
    value = (service_seed ^ target ^ index) & 0xFFFFFFFF
    struct.pack_into('<I', memory, offset(scratch_base + index * 4), value)
    for reg in (0, 1, 2, 3, 5, 6, 7):
        regs[reg] = (0xA6000000 | (index << 8) | reg) & 0xFFFFFFFF
    bank = (fpu.fpscr >> 21) & 1
    fpu.banks[bank][0] ^= (0x13579BDF + index) & 0xFFFFFFFF
    fpu.fpul ^= (0x5A000000 | index)
    return sr ^ (0x21 + index)

rng = random.Random(0x0C902D40)
records = []
for case in range(64):
    ram, initial, initial_fpu, object_address, resource_value = prepare(case, rng)
    expected_ram, actual_ram = bytearray(ram), bytearray(ram)
    expected, actual = State.from_buffer_copy(initial), State.from_buffer_copy(initial)
    expected_fpu = Fpu.from_buffer_copy(initial_fpu)
    actual_fpu = Fpu.from_buffer_copy(initial_fpu)
    calls_expected, calls_actual = [], []
    scratch_base, service_seed = 0x0C700000 + case * 0x100, rng.getrandbits(32)

    def make_reference_hook(target):
        def hook(memory, regs, sr):
            return mutate(memory, regs, sr, expected_fpu, target, calls_expected,
                          scratch_base, service_seed)
        return hook
    hooks = {target: make_reference_hook(target) for target in SERVICE_TARGETS}
    steps = reference(expected_ram, expected, hooks=hooks, fpu=expected_fpu,
                      instruction_budget=128)

    @Service
    def service(_context, target, memory_ptr, cpu_ptr, fpu_ptr):
        cpu, fpu = cpu_ptr.contents, fpu_ptr.contents
        index = len(calls_actual)
        calls_actual.append((target, tuple(cpu.r), cpu.sr, cpu.r[14], cpu.r[15],
                             cpu.r[4], cpu.r[5], cpu.r[3]))
        value = (service_seed ^ target ^ index) & 0xFFFFFFFF
        at = offset(scratch_base + index * 4)
        for byte in range(4):
            memory_ptr[at + byte] = (value >> (8 * byte)) & 0xFF
        for reg in (0, 1, 2, 3, 5, 6, 7):
            cpu.r[reg] = (0xA6000000 | (index << 8) | reg) & 0xFFFFFFFF
        bank = (fpu.fpscr >> 21) & 1
        fpu.banks[bank][0] ^= (0x13579BDF + index) & 0xFFFFFFFF
        fpu.fpul ^= (0x5A000000 | index)
        cpu.sr ^= 0x21 + index
        return 0

    actual_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    native(actual_view, ctypes.byref(actual), ctypes.byref(actual_fpu), service, None)
    assert calls_actual == calls_expected, (
        f'callback boundary mismatch case={case}: native={calls_actual!r}; retail={calls_expected!r}')
    assert bytes(actual) == bytes(expected), f'CPU mismatch case={case}'
    assert bytes(actual_fpu) == bytes(expected_fpu), f'FPU mismatch case={case}'
    assert actual_ram == expected_ram, f'RAM mismatch case={case}'
    records.append({'case': case, 'resource_pointer': hex(resource_value), 'retail_steps': steps,
                    'service_calls': len(calls_expected), 'full_state_match': True})

source = PROJECT / 'source/sa_stg09_indirect_resource_tail_0c905180.c'
report = {
    'module': 'STG09.BIN', 'load_address': hex(LOAD), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(SPAN).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': len(records),
    'comparison_level': 'Complete 16 MiB RAM and GPR/SR/PR/PC/FPSCR/FPUL/both FPU-bank comparisons, including exact direct-call and tail-call service states and callback effects.',
    'scope': 'STG09 0x0c905180 24-byte resource wrapper: saved R14/PR, entity/resource pointer loads, one ordinary service call, indirect tail transfer, and delay-slot restoration.',
    'limitations': ['Both resource services are injected callback boundaries; their callee bodies are outside this wrapper.'],
    'cases': records,
}
(PROJECT / 'analysis/common-overlays/STG09.BIN/indirect_resource_tail_0c905180_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{len(records)} STG09 indirect resource-tail full RAM/CPU/FPU/service-boundary comparisons passed')
