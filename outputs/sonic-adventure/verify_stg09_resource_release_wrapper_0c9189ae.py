"""Differentially verify the STG09 resource-release caller and recovered callee."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
LOAD, ADDRESS, LENGTH = 0x0C900000, 0x0C9189AE, 32
RAM_BASE, RAM_SIZE = 0x0C000000, 16 * 1024 * 1024
GLOBAL_DATA, REFCOUNT, SERVICE_TARGET = 0x0CB1CD20, 0x0CB1CD24, 0x8C098B42
module = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
span = module[ADDRESS - LOAD:ADDRESS - LOAD + LENGTH]
assert len(span) == LENGTH

Service = ctypes.CFUNCTYPE(None, ctypes.c_void_p, ctypes.c_uint32,
                           ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State))
library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/stg09_resource_release_wrapper_0c9189ae.dll'))
native = library.sa_stg09_resource_release_wrapper_0c9189ae
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State), Service, ctypes.c_void_p]
native.restype = None

def offset(address):
    value = (address & 0x1FFFFFFF) - RAM_BASE
    assert 0 <= value < RAM_SIZE - 4, hex(address)
    return value

def put32(memory, address, value):
    struct.pack_into('<I', memory, offset(address), value & 0xFFFFFFFF)

def service_effect(memory, registers, sr, scratch, seed, calls):
    index = len(calls)
    calls.append((SERVICE_TARGET, tuple(registers), sr))
    struct.pack_into('<I', memory, offset(scratch), (seed ^ SERVICE_TARGET ^ index) & 0xFFFFFFFF)
    for register in range(8):
        registers[register] = (0xA7000000 | (index << 8) | register) & 0xFFFFFFFF
    return sr ^ 0x21

rng = random.Random(ADDRESS)
cases = []
for field in (0, 1, 0x12345678):
    for global_object, count in ((0, 0), (0x0C420000, 0), (0x0C420000, 1),
                                 (0x0C420000, 2), (0x0C420000, 0xFFFFFFFF)):
        cases.append((field, global_object, count))
cases.extend((rng.getrandbits(32) or 1, 0x0C500000 + i * 0x100, rng.getrandbits(32))
             for i in range(60))
records = []

for case, (field, global_object, initial_count) in enumerate(cases):
    ram = bytearray(b'\xa5') * RAM_SIZE
    ram[LOAD - RAM_BASE:LOAD - RAM_BASE + len(module)] = module
    object_address = 0x0C600000 + case * 0x100
    put32(ram, object_address + 8, field)
    put32(ram, GLOBAL_DATA, global_object)
    put32(ram, REFCOUNT, initial_count)
    initial = State()
    initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
    initial.r[4] = object_address
    initial.r[14] = 0x0C800000 + case * 4
    initial.r[15] = 0x0CF00000 + case * 0x100
    initial.pr = 0x0CFF0000 + case * 4
    initial.pc = ADDRESS
    initial.sr = rng.getrandbits(32)

    expected_ram, actual_ram = bytearray(ram), bytearray(ram)
    expected, actual = State.from_buffer_copy(initial), State.from_buffer_copy(initial)
    expected_calls, actual_calls = [], []
    scratch, seed = 0x0C300000 + case * 0x100, rng.getrandbits(32)

    def make_hook():
        def hook(memory, registers, sr):
            return service_effect(memory, registers, sr, scratch, seed, expected_calls)
        return hook

    steps = reference(expected_ram, expected, hooks={SERVICE_TARGET: make_hook()},
                      stop_pc=initial.pr, instruction_budget=128)

    @Service
    def service(_context, target, memory_ptr, cpu_ptr):
        cpu = cpu_ptr.contents
        index = len(actual_calls)
        actual_calls.append((target, tuple(cpu.r), cpu.sr))
        value = (seed ^ target ^ index) & 0xFFFFFFFF
        at = offset(scratch)
        for byte in range(4):
            memory_ptr[at + byte] = (value >> (8 * byte)) & 0xFF
        for register in range(8):
            cpu.r[register] = (0xA7000000 | (index << 8) | register) & 0xFFFFFFFF
        cpu.sr ^= 0x21

    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    native(view, ctypes.byref(actual), service, None)
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual) == bytes(expected), (
        f'CPU mismatch in case {case}: r={[(i, hex(actual.r[i]), hex(expected.r[i])) for i in range(16) if actual.r[i] != expected.r[i]]}, '
        f'sr={actual.sr:#x}/{expected.sr:#x}, pr={actual.pr:#x}/{expected.pr:#x}, pc={actual.pc:#x}/{expected.pc:#x}')
    assert actual_calls == expected_calls, f'callback entry mismatch in case {case}'
    assert struct.unpack_from('<I', actual_ram, offset(object_address + 8))[0] == (0 if field else 0)
    records.append({'case': case, 'field': field, 'global_object': hex(global_object),
                    'initial_refcount': hex(initial_count), 'service_calls': len(actual_calls),
                    'retail_instructions': steps, 'full_ram_cpu_match': True})

source = PROJECT / 'source/sa_stg09_resource_release_wrapper_0c9189ae.c'
report = {
    'module': 'STG09.BIN', 'load_address': hex(LOAD), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH), 'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(), 'cases_passed': len(records),
    'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state, including the nested recovered helper and mocked service boundary.',
    'scope': '32-byte conditional resource-release wrapper: input field guard, helper call, input-field clear, stack restoration, and return state.',
    'limitations': ['External service 0x8c098b42 is modeled with deterministic test effects; its implementation and allocator behavior are outside this comparison.'],
    'service_target': hex(SERVICE_TARGET), 'cases': records,
}
(PROJECT / 'analysis/common-overlays/STG09.BIN/resource_release_wrapper_0c9189ae_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{len(records)} STG09 resource-release wrapper full RAM/CPU comparisons passed")
