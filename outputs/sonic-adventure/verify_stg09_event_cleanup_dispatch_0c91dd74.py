"""Full-state and service-boundary verification for the STG09 event cleanup wrapper."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference
from verify_fpu_cleanup_transfer import Fpu

PROJECT = ROOT / 'outputs/sonic-adventure'
LOAD, ADDRESS, LENGTH = 0x0C900000, 0x0C91DD74, 60
RAM_BASE, RAM_SIZE = 0x0C000000, 16 * 1024 * 1024
STATUS_LITERAL, NOTIFY_LITERAL, REMOVE_LITERAL = 0x0C91DE48, 0x0C91DE60, 0x0C91DE64
STATUS_TARGET, NOTIFY_TARGET, REMOVE_TARGET = 0x0CF10000, 0x0CF11000, 0x0CF12000
MODULE = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
SPAN = MODULE[ADDRESS - LOAD:ADDRESS - LOAD + LENGTH]
Service = ctypes.CFUNCTYPE(ctypes.c_int, ctypes.c_void_p, ctypes.c_uint32,
                           ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                           ctypes.POINTER(Fpu))

def offset(address):
    value = (address & 0x1FFFFFFF) - RAM_BASE
    assert 0 <= value < RAM_SIZE - 64, hex(address)
    return value

library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/stg09_event_cleanup_dispatch_0c91dd74.dll'))
native = library.sa_stg09_event_cleanup_dispatch_0c91dd74
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                   ctypes.POINTER(Fpu), Service, ctypes.c_void_p]
native.restype = None

def prepare_case(case, return_zero, field10_present, fieldc_present, sz, active, rng):
    ram = bytearray([0xA5]) * RAM_SIZE
    ram[LOAD - RAM_BASE:LOAD - RAM_BASE + len(MODULE)] = MODULE
    struct.pack_into('<I', ram, offset(STATUS_LITERAL), STATUS_TARGET)
    struct.pack_into('<I', ram, offset(NOTIFY_LITERAL), NOTIFY_TARGET)
    struct.pack_into('<I', ram, offset(REMOVE_LITERAL), REMOVE_TARGET)
    object_address = 0x0C200000 + case * 0x100
    entity = 0x0C400000 + case * 0x100
    field10 = 0x0C500000 + case * 0x100 if field10_present else 0
    fieldc = 0x0C600000 + case * 0x100 if fieldc_present else 0
    struct.pack_into('<I', ram, offset(object_address + 0x20), entity)
    struct.pack_into('<I', ram, offset(entity + 0x10), field10)
    struct.pack_into('<I', ram, offset(entity + 0x0C), fieldc)
    state = State()
    state.r[:] = [0x0C200000 + rng.randrange(0x1000) for _ in range(16)]
    state.r[4] = object_address
    state.r[14] = 0x0C800000 + case * 4
    state.r[15] = 0x0CF00000 + case * 0x100
    state.sr = rng.getrandbits(32)
    state.pr = 0x0CFE0000 + case * 4
    state.pc = ADDRESS
    fpu = Fpu()
    for bank in range(2):
        fpu.banks[bank][:] = [rng.getrandbits(32) for _ in range(16)]
    fpu.fpscr = 0x40001 | (sz << 20) | (active << 21)
    fpu.fpul = rng.getrandbits(32)
    return ram, state, fpu, return_zero, field10, fieldc

def make_reference_hook(target, expected, expected_fpu, return_zero, call_log,
                        ram_scratch_base, service_seed):
    def hook(memory, regs, sr):
        index = len(call_log)
        call_log.append((target, tuple(regs[4:8]), regs[14], regs[15], expected.pr,
                         expected_fpu.fpscr, expected_fpu.fpul))
        struct.pack_into('<I', memory, offset(ram_scratch_base + index * 4),
                         (service_seed ^ index ^ target) & 0xFFFFFFFF)
        for reg in (1, 2, 3, 5, 6, 7):
            regs[reg] = (0xD0000000 | (index << 8) | reg) & 0xFFFFFFFF
        active_bank = (expected_fpu.fpscr >> 21) & 1
        expected_fpu.banks[active_bank][0] ^= (0x10203040 + index) & 0xFFFFFFFF
        expected_fpu.fpul ^= (0xA5000000 | index)
        result = (0 if return_zero else 0x12340001) if target == STATUS_TARGET else 0x80000000 | index
        regs[0] = result
        return sr ^ (0x10 + index)
    return hook

def make_native_service(return_zero, call_log, service_seed, ram_scratch_base):
    @Service
    def service(context, target, memory_ptr, cpu_ptr, fpu_ptr):
        index = len(call_log)
        cpu, fpu = cpu_ptr.contents, fpu_ptr.contents
        call_log.append((target, tuple(cpu.r[4:8]), cpu.r[14], cpu.r[15], cpu.pr,
                         fpu.fpscr, fpu.fpul))
        scratch = offset(ram_scratch_base + index * 4)
        value = (service_seed ^ index ^ target) & 0xFFFFFFFF
        for b in range(4):
            memory_ptr[scratch + b] = (value >> (8 * b)) & 0xFF
        for reg in (1, 2, 3, 5, 6, 7):
            cpu.r[reg] = (0xD0000000 | (index << 8) | reg) & 0xFFFFFFFF
        active_bank = (fpu.fpscr >> 21) & 1
        fpu.banks[active_bank][0] ^= (0x10203040 + index) & 0xFFFFFFFF
        fpu.fpul ^= (0xA5000000 | index)
        cpu.r[0] = (0 if return_zero else 0x12340001) if target == STATUS_TARGET else 0x80000000 | index
        cpu.sr ^= 0x10 + index
        return 0
    return service

rng = random.Random(0x0C91DD74)
case_records = []
branch_counts = {'early_return': 0, 'optional_notification': 0,
                 'field10_zero': 0, 'fieldc_zero': 0, 'tail_transfer': 0}
cases = 0
for return_zero in (False, True):
    for field10_present in (False, True):
        for fieldc_present in (False, True):
            for sz in range(2):
                for active in range(2):
                    case = cases
                    ram, initial, fpu, return_zero, field10, fieldc = prepare_case(
                        case, return_zero, field10_present, fieldc_present, sz, active, rng)
                    expected_ram, actual_ram = bytearray(ram), bytearray(ram)
                    expected, actual = State.from_buffer_copy(initial), State.from_buffer_copy(initial)
                    expected_fpu, actual_fpu = Fpu.from_buffer_copy(fpu), Fpu.from_buffer_copy(fpu)
                    reference_calls, native_calls = [], []
                    scratch_base, service_seed = 0x0C300000 + case * 0x100, rng.getrandbits(32)
                    hooks = {
                        target: make_reference_hook(target, expected, expected_fpu, return_zero,
                                                    reference_calls, scratch_base, service_seed)
                        for target in (STATUS_TARGET, NOTIFY_TARGET, REMOVE_TARGET)
                    }
                    reference(expected_ram, expected, hooks=hooks, fpu=expected_fpu,
                              stop_pc=initial.pr, instruction_budget=128)
                    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
                    service = make_native_service(return_zero, native_calls,
                                                  service_seed, scratch_base)
                    native(view, ctypes.byref(actual), ctypes.byref(actual_fpu), service, None)

                    assert actual_ram == expected_ram, f'RAM mismatch case={case}'
                    assert bytes(actual) == bytes(expected), f'CPU mismatch case={case}'
                    assert bytes(actual_fpu) == bytes(expected_fpu), f'FPU mismatch case={case}'
                    assert native_calls == reference_calls, f'service sequence/ABI mismatch case={case}'
                    assert actual.r[15] == initial.r[15] and actual.r[14] == initial.r[14]
                    assert len(native_calls) == len(reference_calls)
                    branch_counts['early_return'] += int(not return_zero)
                    branch_counts['optional_notification'] += int(return_zero and field10_present and fieldc_present)
                    branch_counts['field10_zero'] += int(return_zero and not field10_present)
                    branch_counts['fieldc_zero'] += int(return_zero and field10_present and not fieldc_present)
                    branch_counts['tail_transfer'] += int(return_zero)
                    case_records.append({
                        'case': case, 'status_zero': return_zero,
                        'field10_present': field10_present, 'fieldc_present': fieldc_present,
                        'sz': sz, 'active_bank': active,
                        'service_targets': [hex(event[0]) for event in native_calls],
                        'full_ram_cpu_fpu_service_boundary_match': True,
                    })
                    cases += 1

assert cases == 32 and branch_counts['early_return'] > 0
assert branch_counts['optional_notification'] > 0 and branch_counts['field10_zero'] > 0
assert branch_counts['fieldc_zero'] > 0 and branch_counts['tail_transfer'] > 0
source = PROJECT / 'source/sa_stg09_event_cleanup_dispatch_0c91dd74.c'
report = {
    'module': 'STG09.BIN', 'load_address': hex(LOAD), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(SPAN).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': cases, 'branch_counts': branch_counts,
    'comparison_level': 'Complete 16 MiB RAM and complete GPR/SR/PR/PC/FPSCR/FPUL/both FPU-bank comparisons, including external-service call order, target, argument registers, stack, PR, and callback-visible state.',
    'scope': 'Status-service early return, conditional notification service, two shared field-removal calls with a final tail transfer, R14/PR save and restore, and all retail delay-slot effects.',
    'limitations': ['External status, notification, and field-removal target bodies are explicit injected services and are not recovered by this wrapper.',
                    'Service test behavior is controlled; it validates call boundaries and caller integration rather than the external services themselves.'],
    'cases': case_records,
}
(PROJECT / 'analysis/common-overlays/STG09.BIN/event_cleanup_dispatch_0c91dd74_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{cases} complete RAM/CPU/FPU comparisons passed; service paths {branch_counts}')
