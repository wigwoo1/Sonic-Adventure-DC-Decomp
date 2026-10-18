import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C02CF48
LENGTH = 26
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
native = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'work_release_8c02cf48.dll')).sa_work_release_8c02cf48
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
    area = 0x8C740000 + case * 0x200
    node, next_node, previous_node = area + 0x20, area + 0x40, area + 0x60
    old_free = area + 0x80
    next_value = 0 if case % 3 == 0 else next_node
    previous_value = ((0 - 0x8C02D548) & 0xFFFFFFFF) if case % 2 else previous_node
    resource_value = 0 if case % 4 == 0 else area + 0xA0
    resource_link = 0 if case % 5 == 0 else area + 0xC0
    old_secondary = area + 0xE0
    put32(ram, 0x8C02D544, old_free)
    put32(ram, 0x8C02D540, old_secondary)
    put32(ram, node, next_value)
    put32(ram, node + 4, previous_value)
    put32(ram, node + 8, rng.getrandbits(32))
    put32(ram, node + 0x10, resource_value)
    put32(ram, node + 0x14, resource_link)
    put32(ram, next_node + 4, rng.getrandbits(32))
    put32(ram, previous_node, rng.getrandbits(32))
    put32(ram, resource_link or area + 0xC0, rng.getrandbits(32))
    put32(ram, old_free, rng.getrandbits(32))
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = node
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
    assert actual_ram == expected_ram, f'RAM mismatch case={case}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch case={case}: rdiff={[(i,hex(actual_state.r[i]),hex(expected_state.r[i])) for i in range(16) if actual_state.r[i]!=expected_state.r[i]]} sr={actual_state.sr:#x}/{expected_state.sr:#x}'
    cases.append({'case': case, 'resource_value': hex(resource_value), 'resource_link': hex(resource_link), 'ram_and_cpu_match': True})

source_file = PROJECT / 'source' / 'sa_work_release_8c02cf48.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH), 'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source_file.read_bytes()).hexdigest(), 'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state',
    'scope': 'Optional secondary-resource link, work-field clear, and linked free-list return; full RAM/CPU differential comparisons against retail interpreter.',
    'limitations': ['Invalid objects, malformed resource links, concurrency, and full gameplay remain unverified.'],
    'cases': cases,
}
(PROJECT / 'analysis' / 'work_release_8c02cf48_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} work-release RAM/CPU comparisons passed')
