import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C02CE18
LENGTH = 30
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
native = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'work_id_clear_8c02ce18.dll')).sa_work_id_clear_8c02ce18
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE - 128, hex(address)
    return result

def put32(memory, address, value):
    memory[offset(address):offset(address) + 4] = (value & 0xFFFFFFFF).to_bytes(4, 'little')

rng = random.Random(FUNCTION)
cases = []
for case in range(96):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    area = 0x8C740000 + case * 0x400
    length = case % 9
    nodes = [area + 0x40 + i * 0x20 for i in range(length)]
    for index, node in enumerate(nodes):
        put32(ram, node, nodes[index + 1] if index + 1 < length else 0)
        put32(ram, node + 8, rng.getrandbits(32))
    match_position = (case % length) if length and case % 3 else None
    target = rng.getrandbits(32)
    if match_position is not None:
        put32(ram, nodes[match_position] + 8, target)
        if case % 7 == 0 and match_position + 1 < length:
            put32(ram, nodes[match_position + 1] + 8, target)
    put32(ram, 0x8C02D548, nodes[0] if nodes else 0)
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = target
    state.r[15] = 0x8C7FF000
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION
    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    reference(expected_ram, expected_state)
    actual_ram = bytearray(ram)
    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(view, ctypes.byref(actual_state))
    assert actual_ram == expected_ram, f'RAM mismatch case={case}, length={length}, match={match_position}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch case={case}: rdiff={[(i,hex(actual_state.r[i]),hex(expected_state.r[i])) for i in range(16) if actual_state.r[i]!=expected_state.r[i]]} sr={actual_state.sr:#x}/{expected_state.sr:#x}'
    cases.append({'case': case, 'length': length, 'match_position': match_position, 'ram_and_cpu_match': True})

source_file = PROJECT / 'source' / 'sa_work_id_clear_8c02ce18.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH), 'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source_file.read_bytes()).hexdigest(), 'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state',
    'scope': 'Active-list search and first matching work-ID clear; lengths 0 through 8, first/middle/last/absent matches, duplicate IDs, complete RAM and CPU comparison against retail interpreter.',
    'limitations': ['Invalid list pointers, cycles, malformed lists, concurrency, and full gameplay remain unverified.'],
    'cases': cases,
}
(PROJECT / 'analysis' / 'work_id_clear_8c02ce18_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} work-ID clear RAM/CPU comparisons passed')
