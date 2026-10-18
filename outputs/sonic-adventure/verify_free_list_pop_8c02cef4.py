import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C02CEF4
LENGTH = 36
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
native = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'free_list_pop_8c02cef4.dll')).sa_free_list_pop_8c02cef4
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE - 64, hex(address)
    return result

def put32(memory, address, value):
    memory[offset(address):offset(address) + 4] = (value & 0xFFFFFFFF).to_bytes(4, 'little')

rng = random.Random(FUNCTION)
cases = []
for case in range(96):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    sentinel = 0x8C740000 + case * 0x100
    node = sentinel + 0x20
    next_node = sentinel + 0x40
    active = sentinel + 0x60 if case % 3 else 0
    free_next = 0 if case % 4 == 0 else next_node
    put32(ram, 0x8C02D544, sentinel)
    put32(ram, 0x8C02D548, active)
    put32(ram, sentinel, free_next)
    put32(ram, sentinel + 4, rng.getrandbits(32))
    put32(ram, node, next_node)
    put32(ram, node + 4, rng.getrandbits(32))
    put32(ram, next_node + 4, rng.getrandbits(32))
    if active:
        put32(ram, active, rng.getrandbits(32))
        put32(ram, active + 4, rng.getrandbits(32))
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
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
    assert actual_ram == expected_ram, f'RAM mismatch case={case} active={active:#x} free_next={free_next:#x}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch case={case}: rdiff={[(i,hex(actual_state.r[i]),hex(expected_state.r[i])) for i in range(16) if actual_state.r[i]!=expected_state.r[i]]} sr={actual_state.sr:#x}/{expected_state.sr:#x}'
    cases.append({'case': case, 'active_head': hex(active), 'free_next': hex(free_next), 'ram_and_cpu_match': True})

source_file = PROJECT / 'source' / 'sa_free_list_pop_8c02cef4.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH), 'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source_file.read_bytes()).hexdigest(), 'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state',
    'scope': 'Free-list node pop and active-list insertion; empty/nonempty free list, empty/nonempty active list, preserved surrounding memory, and full RAM/CPU differential comparisons.',
    'limitations': ['List invariants outside the tested linked-list shapes, invalid pointers, concurrency, and full gameplay remain unverified.'],
    'cases': cases,
}
(PROJECT / 'analysis' / 'free_list_pop_8c02cef4_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} free-list RAM/CPU comparisons passed')
