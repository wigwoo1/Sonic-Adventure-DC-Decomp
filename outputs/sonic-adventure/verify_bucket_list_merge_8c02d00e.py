import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C02D00E
LENGTH = 58
RAM_SIZE = 16 * 1024 * 1024
TABLE = 0x8C02D560
native = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'bucket_list_merge_8c02d00e.dll')).sa_bucket_list_merge_8c02d00e
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE - 4096, hex(address)
    return result

def put32(memory, address, value):
    memory[offset(address):offset(address) + 4] = (value & 0xFFFFFFFF).to_bytes(4, 'little')

rng = random.Random(FUNCTION)
cases = []
for case in range(96):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    table_bytes = bytes(128 * 8)
    table_offset = offset(TABLE)
    ram[table_offset:table_offset + len(table_bytes)] = table_bytes
    count = [0, 1, 2, 4, 12, 32][case % 6]
    buckets = sorted(rng.sample(range(128), count)) if count else []
    area = 0x8C740000 + case * 0x1000
    chain_lengths = {}
    node_by_bucket = {}
    for n, bucket in enumerate(buckets):
        chain_length = 1 + rng.randrange(4)
        chain_lengths[bucket] = chain_length
        nodes = [area + 0x100 + (n * 4 + j) * 0x20 for j in range(chain_length)]
        node_by_bucket[bucket] = nodes
        for j, node in enumerate(nodes):
            put32(ram, node, nodes[j + 1] if j + 1 < chain_length else 0)
            previous = nodes[j - 1] if j else ((0 - 0x8C02D548) & 0xFFFFFFFF)
            put32(ram, node + 4, previous)
        entry = TABLE + bucket * 8
        put32(ram, entry, nodes[0])
        put32(ram, entry + 4, nodes[-1])
    work = area + 0x2800
    put32(ram, work + 0x10, rng.getrandbits(32))
    put32(ram, work + 0x14, rng.getrandbits(32))
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = work
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
    assert actual_ram == expected_ram, f'RAM mismatch case={case}, buckets={buckets}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch case={case}: rdiff={[(i,hex(actual_state.r[i]),hex(expected_state.r[i])) for i in range(16) if actual_state.r[i]!=expected_state.r[i]]} sr={actual_state.sr:#x}/{expected_state.sr:#x}'
    cases.append({'case': case, 'nonempty_buckets': buckets, 'chain_lengths': chain_lengths, 'ram_and_cpu_match': True})

source_file = PROJECT / 'source' / 'sa_bucket_list_merge_8c02d00e.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH), 'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source_file.read_bytes()).hexdigest(), 'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state',
    'scope': 'Merge nonempty bucket lists across a fixed 128-entry table into a work record; empty, single, sparse, and multi-bucket patterns; one- through four-node chains; complete RAM and CPU differential comparisons.',
    'limitations': ['Malformed pointers, concurrency, and full gameplay remain unverified.'],
    'cases': cases,
}
(PROJECT / 'analysis' / 'bucket_list_merge_8c02d00e_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} bucket-list merge RAM/CPU comparisons passed')
