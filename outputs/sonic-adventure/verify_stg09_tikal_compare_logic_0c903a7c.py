"""Compare native Tikal byte-matching logic with retail code and injected service results."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call, P

PROJECT = ROOT / 'outputs/sonic-adventure'
LOAD, ADDRESS, LENGTH = 0x0C900000, 0x0C903A7C, 296
RAM_BASE, RAM_SIZE = 0x0C000000, 16 * 1024 * 1024
TABLES = (0x0CAF97D4, 0x0CAF97E4, 0x0CAF97F4)
Sample = ctypes.CFUNCTYPE(ctypes.c_uint32, ctypes.c_void_p, ctypes.c_uint32)
module = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
span = module[ADDRESS - LOAD:ADDRESS - LOAD + LENGTH]

library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/stg09_tikal_compare_logic_0c903a7c.dll'))
native = library.sa_stg09_tikal_compare_logic_0c903a7c
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                   Sample, ctypes.c_void_p]
native.restype = None

def offset(address):
    value = (address & 0x1FFFFFFF) - RAM_BASE
    assert 0 <= value < RAM_SIZE - 32, hex(address)
    return value

def make_case(rng, case):
    ram = bytearray(b'\xa5') * RAM_SIZE
    ram[LOAD - RAM_BASE:LOAD - RAM_BASE + len(module)] = module
    object = 0x0C800000 + case * 0x100
    tables = [bytearray((base + index) & 255 for index in range(16))
              for base in (0x10, 0x40, 0x70)]
    indices = [rng.randrange(0x1000) for _ in range(3)]
    i, j, k = (x & 15 for x in indices)

    # Force a distinct retail match class every six cases, plus randomized inputs.
    if case % 6 == 0:
        value = 0x64 if case % 12 == 0 else 0x37
        tables[0][(i + 1) & 15] = tables[1][(j + 1) & 15] = tables[2][(k + 1) & 15] = value
    elif case % 6 == 1:
        value = 0x64 if case % 12 == 1 else 0x39
        tables[0][i] = tables[1][j] = tables[2][k] = value
    elif case % 6 == 2:
        value = 0x64 if case % 12 == 2 else 0x3b
        tables[0][(i - 1) & 15] = tables[1][(j - 1) & 15] = tables[2][(k - 1) & 15] = value
    elif case % 6 == 3:
        value = 0x64 if case % 12 == 3 else 0x3d
        tables[0][(i + 1) & 15] = tables[1][j] = tables[2][(k - 1) & 15] = value
    elif case % 6 == 4:
        value = 0x64 if case % 12 == 4 else 0x3f
        tables[2][(k + 1) & 15] = tables[1][j] = tables[0][(i - 1) & 15] = value

    for address, table in zip(TABLES, tables):
        ram[offset(address):offset(address) + 16] = table
    ram[offset(object):offset(object) + 32] = bytes(rng.randrange(256) for _ in range(32))
    return ram, object, indices

rng = random.Random(0x0C903A7C)
records = []
branch_counts = {str(i): 0 for i in range(5)}
branch_counts['no_match'] = 0
for case in range(512):
    ram, object, indices = make_case(rng, case)
    expected_ram, actual_ram = bytearray(ram), bytearray(ram)
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = object
    state.r[15] = 0x0CF00000 + case * 0x100
    state.pr = 0x0CFF0000 + case * 4
    state.pc = ADDRESS
    state.sr = rng.getrandbits(32)
    expected = State.from_buffer_copy(state)
    actual = State.from_buffer_copy(state)
    service_values = iter(indices)
    calls = []

    def service(memory, regs, sr):
        value = next(service_values)
        calls.append(value)
        regs[0] = value
        return sr

    reference(expected_ram, expected, hooks={0x8C10CA5C: service},
              stop_pc=state.pr, instruction_budget=512)
    service_values = iter(indices)
    requested_bounds = []

    @Sample
    def sample(context, bound):
        requested_bounds.append(bound)
        return next(service_values)

    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    native(view, ctypes.byref(actual), sample, None)
    assert requested_bounds == [0x1000, 0, 0], f'sample-call ABI mismatch case={case}'
    begin = offset(object)
    assert actual_ram[begin:begin + 32] == expected_ram[begin:begin + 32], f'object mismatch case={case}'
    frame = offset(state.r[15]) - 28
    assert actual_ram[frame:frame + 28] == expected_ram[frame:frame + 28], f'callee-save frame mismatch case={case}'
    exclusions = sorted(((begin, begin + 32), (frame - 20, frame + 28)))
    cursor = 0
    for start, end in exclusions:
        assert actual_ram[cursor:start] == expected_ram[cursor:start], f'unexpected RAM write before excluded region case={case}'
        cursor = end
    assert actual_ram[cursor:] == expected_ram[cursor:], f'unexpected RAM write after excluded region case={case}'
    for register in (4, 8, 9, 11, 12, 13, 14, 15):
        assert actual.r[register] == expected.r[register], f'preserved R{register} mismatch case={case}'
    assert (actual.pr, actual.pc) == (expected.pr, expected.pc), f'return state mismatch case={case}'
    if case % 6 < 5:
        expected_class = case % 6
        assert actual_ram[begin + 0x10] == expected_class, f'match class {expected_class} missed case={case}'
        branch_counts[str(expected_class)] += 1
    else:
        assert actual_ram[begin + 0x10] == ram[begin + 0x10], f'unexpected class update case={case}'
        assert actual_ram[begin + 0x12] == ram[begin + 0x12], f'unexpected selected-byte update case={case}'
        branch_counts['no_match'] += 1
    for address in TABLES:
        at = offset(address)
        assert actual_ram[at:at + 16] == expected_ram[at:at + 16], f'table changed case={case}'
    assert len(calls) == 3, f'expected three service calls, got {len(calls)} case={case}'
    records.append({'case': case, 'indices': indices,
                    'object_bytes_match': True, 'tables_unchanged': True,
                    'retail_service_calls': len(calls)})

# Connect the retail worker to the separately verified native signed-division
# implementation instead of injecting service outputs for this second batch.
main_binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
division = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/signed_division.dll')).sa_signed_division
division.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(Fpu),
                     P, P, P, P, P, ctypes.c_void_p, Call]
division.restype = ctypes.c_int

@Call
def unexpected_service(*args):
    raise AssertionError('signed division requested an unexpected lower service')

runtime_records = []
for case in range(64):
    ram, object, _ = make_case(rng, 1024 + case)
    expected_ram, actual_ram = bytearray(ram), bytearray(ram)
    # SH-4 physical alias 0x8c010000 maps to this RAM offset for retail code.
    expected_ram[0x10000:0x10000 + len(main_binary)] = main_binary
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = object
    state.r[15] = 0x0CF00000 + case * 0x100
    state.pr = 0x0CFE0000 + case * 4
    state.pc = ADDRESS
    state.sr = rng.getrandbits(32)
    expected = State.from_buffer_copy(state)
    fpu = Fpu()
    fpu.fpscr = rng.choice((0, 0x380003, 0x40001, 0x200000))
    fpu.fpul = rng.getrandbits(32)
    for bank in range(2):
        fpu.banks[bank][:] = [rng.getrandbits(32) for _ in range(16)]
    indices = []

    def native_division_service(memory, regs, sr):
        memory_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(memory)
        native_regs = (ctypes.c_uint32 * 16)(*regs)
        native_sr, native_pr = ctypes.c_uint32(sr), ctypes.c_uint32(expected.pr)
        fpul, macl = ctypes.c_uint32(fpu.fpul), ctypes.c_uint32()
        result = division(memory_view, ctypes.byref(fpu), ctypes.byref(native_sr),
                          native_regs, ctypes.byref(native_pr), ctypes.byref(fpul),
                          ctypes.byref(macl), None, unexpected_service)
        assert result == 0
        regs[:] = list(native_regs)
        expected.pr = native_pr.value
        fpu.fpul = fpul.value
        indices.append(native_regs[0])
        return native_sr.value

    reference(expected_ram, expected,
              hooks={0x8C10CA5C: native_division_service}, fpu=fpu,
              stop_pc=state.pr, instruction_budget=4096)
    assert len(indices) == 3, f'expected three connected service calls, got {len(indices)}'
    service_values = iter(indices)
    requested_bounds = []

    @Sample
    def sample_connected(context, bound):
        requested_bounds.append(bound)
        return next(service_values)

    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual = State.from_buffer_copy(state)
    original_sp = state.r[15]
    native(view, ctypes.byref(actual), sample_connected, None)
    assert requested_bounds == [0x1000, 0, 0], f'connected sample-call ABI mismatch case={case}'
    begin = offset(object)
    assert actual_ram[begin:begin + 32] == expected_ram[begin:begin + 32], f'connected object mismatch case={case}'
    stack = offset(original_sp) - 28
    assert actual_ram[stack:stack + 28] == expected_ram[stack:stack + 28], f'connected stack frame mismatch case={case}'
    for register in (4, 8, 9, 11, 12, 13, 14, 15):
        assert actual.r[register] == expected.r[register], f'connected preserved R{register} mismatch case={case}'
    assert (actual.pr, actual.pc) == (expected.pr, expected.pc), f'connected return state mismatch case={case}'
    for address in TABLES:
        at = offset(address)
        assert actual_ram[at:at + 16] == expected_ram[at:at + 16], f'connected table changed case={case}'
    runtime_records.append({'case': case, 'service_indices': indices,
                            'object_bytes_match': True, 'service_calls': 3,
                            'service': 'verified native signed-division implementation'})

source = PROJECT / 'source/sa_stg09_tikal_compare_logic_0c903a7c.c'
report = {
    'module': 'STG09.BIN', 'load_address': hex(LOAD), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': len(records), 'runtime_service_cases': len(runtime_records),
    'runtime_service_records': runtime_records, 'match_branch_cases': branch_counts,
    'comparison_level': '512 controlled-index cases compare object bytes +0..+31, all other RAM outside the 48-byte local-stack scratch/save area, exact 28-byte callee-save stack frame, preserved R4/R8/R9/R11-R15, and PR/PC; all three 16-byte tables are checked unchanged. 64 more retail runs use separately verified native signed division at 0x8c10ca5c and compare object/table effects, callee-save frame/registers, and PR/PC.',
    'scope': 'Tikal three-table byte matching, match-class assignment, selected-byte storage, and active-byte clearing.',
    'limitations': ['The 20-byte local stack spill content is excluded; the native implementation writes the exact 28-byte callee-save frame. In connected-service cases, additional service stack effects are outside comparison.',
                    'Caller-scratch registers R0-R3/R5-R7/R10 and SR are not compared; preserved registers, SP, PR, and PC are compared.',
                    'The connected 0x8c10ca5c service is native signed division already verified separately; its retail instructions are not executed inline by the outer interpreter.'],
    'cases': records,
}
(PROJECT / 'analysis/common-overlays/STG09.BIN/tikal_compare_logic_0c903a7c_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{len(records)} controlled and {len(runtime_records)} connected-service Tikal logic cases passed; all retail match branches exercised')
