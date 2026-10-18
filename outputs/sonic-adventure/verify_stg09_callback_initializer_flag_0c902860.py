"""Full-state verification for the STG09 callback/object initializer."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference
from verify_fpu_cleanup_transfer import Fpu

PROJECT = ROOT / 'outputs/sonic-adventure'
LOAD, ADDRESS, LENGTH = 0x0C900000, 0x0C902860, 30
RAM_BASE, RAM_SIZE = 0x0C000000, 16 * 1024 * 1024
SERVICE_LITERAL, SERVICE_TARGET = 0x0C902840, 0x8C02F4B0
ENTITY_GLOBAL, SCRATCH_BASE = 0x0CA0E9B0, 0x0C300000
MODULE = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
SPAN = MODULE[ADDRESS - LOAD:ADDRESS - LOAD + LENGTH]

Service = ctypes.CFUNCTYPE(ctypes.c_int, ctypes.c_void_p, ctypes.c_uint32,
                           ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                           ctypes.POINTER(Fpu))

def offset(address):
    value = (address & 0x1FFFFFFF) - RAM_BASE
    assert 0 <= value < RAM_SIZE - 64, hex(address)
    return value

library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/stg09_callback_initializer_flag_0c902860.dll'))
native = library.sa_stg09_callback_initializer_flag_0c902860
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                   ctypes.POINTER(Fpu), Service, ctypes.c_void_p]
native.restype = None

def prepare(case, active_bank, rng):
    ram = bytearray([0xA5]) * RAM_SIZE
    ram[LOAD - RAM_BASE:LOAD - RAM_BASE + len(MODULE)] = MODULE
    object_address = 0x0C200000 + case * 0x100
    entity = 0x0C400000 + case * 0x100
    struct.pack_into('<I', ram, offset(object_address + 0x20), entity)
    struct.pack_into('<I', ram, offset(ENTITY_GLOBAL), 0x0C500000 + case * 0x100)
    struct.pack_into('<I', ram, offset(0x0C500000 + case * 0x100), rng.getrandbits(32))
    cpu = State()
    cpu.r[:] = [rng.getrandbits(32) for _ in range(16)]
    cpu.r[4] = object_address
    cpu.r[13] = 0x0C800000 + case * 0x100
    cpu.r[14] = 0x0C810000 + case * 0x100
    cpu.r[15] = 0x0CF00000 + case * 0x100
    cpu.sr = rng.getrandbits(32)
    cpu.pr = 0x0CFE0000 + case * 4
    cpu.pc = ADDRESS
    fpu = Fpu()
    for bank in range(2):
        fpu.banks[bank][:] = [rng.getrandbits(32) for _ in range(16)]
    fpu.fpscr = 0x40001 | (active_bank << 21)
    fpu.fpul = rng.getrandbits(32)
    return ram, cpu, fpu, object_address, entity

def reference_hook(expected_fpu, call_log, ram_scratch, service_seed):
    def hook(memory, regs, sr):
        index = len(call_log)
        stack_arg = struct.unpack_from('<I', memory, offset(regs[15]))[0]
        call_log.append((SERVICE_TARGET, tuple(regs[2:8]), regs[13], regs[14],
                         regs[15], stack_arg, regs[4], regs[5], regs[6], regs[7]))
        struct.pack_into('<I', memory, offset(ram_scratch),
                         (service_seed ^ SERVICE_TARGET ^ index) & 0xFFFFFFFF)
        for reg in (0, 1, 2, 3, 5, 6, 7):
            regs[reg] = (0xD1000000 | (index << 8) | reg) & 0xFFFFFFFF
        active = (expected_fpu.fpscr >> 21) & 1
        expected_fpu.banks[active][0] ^= 0xA5010203
        expected_fpu.fpul ^= 0x5A5A1234
        return sr ^ 0x100
    return hook

def native_service(return_fpu, call_log, ram_scratch, service_seed):
    @Service
    def service(context, target, memory_ptr, cpu_ptr, fpu_ptr):
        cpu, fpu = cpu_ptr.contents, fpu_ptr.contents
        index = len(call_log)
        stack_arg = sum(int(memory_ptr[offset(cpu.r[15]) + i]) << (8 * i) for i in range(4))
        call_log.append((target, tuple(cpu.r[2:8]), cpu.r[13], cpu.r[14],
                         cpu.r[15], stack_arg, cpu.r[4], cpu.r[5], cpu.r[6], cpu.r[7]))
        value = (service_seed ^ target ^ index) & 0xFFFFFFFF
        at = offset(ram_scratch)
        for i in range(4):
            memory_ptr[at + i] = (value >> (8 * i)) & 0xFF
        for reg in (0, 1, 2, 3, 5, 6, 7):
            cpu.r[reg] = (0xD1000000 | (index << 8) | reg) & 0xFFFFFFFF
        active = (fpu.fpscr >> 21) & 1
        fpu.banks[active][0] ^= 0xA5010203
        fpu.fpul ^= 0x5A5A1234
        cpu.sr ^= 0x100
        return 0
    return service

rng = random.Random(0x0C902860)
records = []
for case in range(32):
    bank = case & 1
    ram, initial, fpu, object_address, entity = prepare(case, bank, rng)
    expected_ram, actual_ram = bytearray(ram), bytearray(ram)
    expected, actual = State.from_buffer_copy(initial), State.from_buffer_copy(initial)
    expected_fpu, actual_fpu = Fpu.from_buffer_copy(fpu), Fpu.from_buffer_copy(fpu)
    reference_calls, native_calls = [], []
    scratch = SCRATCH_BASE + case * 0x100
    seed = rng.getrandbits(32)
    hooks = {SERVICE_TARGET: reference_hook(expected_fpu, reference_calls, scratch, seed)}
    reference(expected_ram, expected, hooks=hooks, fpu=expected_fpu,
              stop_pc=initial.pr, instruction_budget=128)
    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    native(view, ctypes.byref(actual), ctypes.byref(actual_fpu),
           native_service(actual_fpu, native_calls, scratch, seed), None)

    assert actual_ram == expected_ram, (
        f'RAM mismatch case={case}; diffs={[hex(RAM_BASE+i) for i,(a,b) in enumerate(zip(actual_ram,expected_ram)) if a!=b][:24]}; '
        f'native_calls={native_calls}; reference_calls={reference_calls}')
    assert bytes(actual) == bytes(expected), (
        f'CPU mismatch case={case}; actual_r={[hex(v) for v in actual.r]}; '
        f'expected_r={[hex(v) for v in expected.r]}; '
        f'SR={actual.sr:08x}/{expected.sr:08x}; '
        f'PR={actual.pr:08x}/{expected.pr:08x}; PC={actual.pc:08x}/{expected.pc:08x}')
    assert bytes(actual_fpu) == bytes(expected_fpu), f'FPU mismatch case={case}'
    assert native_calls == reference_calls, f'callback ABI mismatch case={case}'
    assert len(native_calls) == 1
    records.append({'case': case, 'active_fpu_bank': bank,
                    'service_calls': 1, 'full_ram_cpu_fpu_match': True})

source = PROJECT / 'source/sa_stg09_callback_initializer_flag_0c902860.c'
report = {
    'module': 'STG09.BIN', 'load_address': hex(LOAD), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(SPAN).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': len(records),
    'comparison_level': 'Complete 16 MiB RAM and complete GPR/SR/PR/PC/FPSCR/FPUL/both FPU-bank comparisons, plus exact external-service target, entry registers, stack argument, and callback-visible RAM/FPU effects.',
    'scope': 'STG09 object callback initializer: service dispatch, stack-passed argument, delay-slot entity load, entity initialization fields, callback assignments, stack and callee-saved register restoration.',
    'limitations': ['The external service at 0x8c02f4b0 is an injected test service; its implementation is not recovered by this routine.',
                    'The service behavior is controlled by the verifier; this proves caller ABI/integration and this wrapper, not the callee body.'],
    'cases': records,
}
(PROJECT / 'analysis/common-overlays/STG09.BIN/callback_initializer_flag_0c902860_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{len(records)} complete RAM/CPU/FPU and service-boundary comparisons passed")
