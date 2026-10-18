"""Differential full-state verification for four external overlay tail thunks."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
RAM_BASE, RAM_SIZE = 0x0C000000, 16 * 1024 * 1024
SPECS = [
    ('AL_MAIN.BIN', 0x0C900000, 0x0C901AE6, 6, 'sa_al_main_thunk_ext_fun_8c010b00', 0x2A, 3),
    ('B_CHAOS0.BIN', 0x0C900000, 0x0C9002F6, 6, 'sa_b_chaos0_thunk_ext_fun_8c107aaa', 0x0B, 3),
    ('B_CHAOS6.BIN', 0x0C900000, 0x0C9030BC, 6, 'sa_b_chaos6_thunk_ext_fun_8c049a4e', 0x16, 3),
    ('B_EGM2.BIN', 0x0C900000, 0x0C906D6C, 6, 'sa_b_egm2_thunk_ext_fun_8c0606c6', 0x0C, 2),
    ('ADVERTISE.BIN', 0x8C900000, 0x8C901356, 6, 'sa_advertise_thunk_ext_fun_8c01140a', 0x15, 3),
    ('ADVERTISE.BIN', 0x8C900000, 0x8C90135C, 6, 'sa_advertise_thunk_ext_fun_8c0114f0', 0x14, 3),
    ('ADVERTISE.BIN', 0x8C900000, 0x8C901362, 6, 'sa_advertise_thunk_ext_fun_8c011606', 0x14, 3),
    ('ADVERTISE.BIN', 0x8C900000, 0x8C9013C0, 6, 'sa_advertise_thunk_ext_fun_8c011238', 0x07, 3),
    ('ADVERTISE.BIN', 0x8C900000, 0x8C9013C6, 6, 'sa_advertise_thunk_ext_fun_8c088680', 0x07, 3),
    ('ADVERTISE.BIN', 0x8C900000, 0x8C9018A4, 6, 'sa_advertise_thunk_ext_fun_8c107ada', 0x1D, 1),
    ('ADVERTISE.BIN', 0x8C900000, 0x8C9018AA, 6, 'sa_advertise_thunk_ext_fun_8c107af6', 0x1D, 1),
    ('ADVERTISE.BIN', 0x8C900000, 0x8C9024C6, 6, 'sa_advertise_thunk_ext_fun_8c010b00', 0x55, 3),
]

def offset(address):
    value = (address & 0x1FFFFFFF) - RAM_BASE
    assert 0 <= value < RAM_SIZE - 8, hex(address)
    return value

def literal_address(address, displacement):
    return ((address + 4) & ~3) + displacement * 4

library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/external_overlay_tail_thunks.dll'))
rng = random.Random(0xE771F00D)
records = []
for module_name, load, address, length, name, displacement, register in SPECS:
    module = (ROOT / f'work/sonic-adventure/decoded/SONICADV/{module_name}').read_bytes()
    span = module[address - load:address - load + length]
    assert len(span) == length
    literal = literal_address(address, displacement)
    target = int.from_bytes(module[literal - load:literal - load + 4], 'little')
    assert target != 0, f'{module_name} {address:08x}: null target literal'
    native = getattr(library, name)
    native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    native.restype = None
    case_records = []
    for case in range(128):
        ram = bytearray([0xA5]) * RAM_SIZE
        load_offset = (load & 0x1FFFFFFF) - RAM_BASE
        ram[load_offset:load_offset + len(module)] = module
        expected_ram, actual_ram = bytearray(ram), bytearray(ram)
        initial = State()
        initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
        initial.sr = rng.getrandbits(32)
        initial.pr = 0x0CF00000 + case * 4
        initial.pc = address
        expected, actual = State.from_buffer_copy(initial), State.from_buffer_copy(initial)
        reference(expected_ram, expected, stop_pc=target, instruction_budget=8)
        view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        native(view, ctypes.byref(actual))
        assert actual_ram == expected_ram, f'{module_name} RAM mismatch case={case}'
        assert bytes(actual) == bytes(expected), f'{module_name} CPU mismatch case={case}'
        assert actual.r[register] == target, f'{module_name} target register mismatch case={case}'
        case_records.append({'case': case, 'full_ram_cpu_match': True})
    records.append({
        'module': module_name, 'load_address': hex(load), 'name': name,
        'address': hex(address), 'end_exclusive': hex(address + length),
        'retail_span_sha256': hashlib.sha256(span).hexdigest(),
        'literal_address': hex(literal), 'target': hex(target),
        'target_register': register, 'cases_passed': len(case_records),
        'comparison_level': 'Complete 16 MiB RAM and complete GPR/SR/PR/PC comparisons at the external tail target.',
        'scope': 'PC-relative MOV.L target load, indirect JMP, NOP delay slot, and PR preservation.',
        'limitations': ['The external target body is outside this thunk and is not included in these comparisons.'],
        'cases': case_records,
    })

source_hash = hashlib.sha256((PROJECT / 'source/external_overlay_tail_thunks.c').read_bytes()).hexdigest()
for record in records:
    record['source_sha256'] = source_hash
report = {'routines': records, 'total_cases_passed': sum(r['cases_passed'] for r in records)}
(PROJECT / 'analysis/external_overlay_tail_thunks_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{len(records)} overlay tail thunks passed {report['total_cases_passed']} complete RAM/CPU comparisons")
