"""Differentially verify the STG09 recursive child/sibling tree counter."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
LOAD, ADDRESS, LENGTH = 0x0C900000, 0x0C920D4C, 46
RAM_BASE, RAM_SIZE = 0x0C000000, 16 * 1024 * 1024
module = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
span = module[ADDRESS - LOAD:ADDRESS - LOAD + LENGTH]
assert len(span) == LENGTH

library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/stg09_recursive_tree_node_count_0c920d4c.dll'))
native = library.sa_stg09_recursive_tree_node_count_0c920d4c
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    value = (address & 0x1FFFFFFF) - RAM_BASE
    assert 0 <= value < RAM_SIZE - 4, hex(address)
    return value

def put32(memory, address, value):
    at = offset(address)
    memory[at:at + 4] = (value & 0xFFFFFFFF).to_bytes(4, 'little')

def make_tree(memory, base, case, rng, shape):
    nodes = []
    def allocate(depth, children_spec):
        address = base + len(nodes) * 0x80
        node = {'address': address, 'children': [], 'flags': 0}
        nodes.append(node)
        if children_spec is not None:
            for spec in children_spec:
                node['children'].append(allocate(depth + 1, spec))
        node['flags'] = (0x10 if rng.randrange(5) == 0 else 0) | (rng.getrandbits(32) & 0x0F)
        return node

    root = allocate(0, shape)
    if case == 3:
        root['flags'] &= ~0x10
    elif case == 8:
        root['flags'] |= 0x10
    for node in nodes:
        address = node['address']
        children = node['children']
        put32(memory, address, node['flags'])
        put32(memory, address + 0x2c, children[0]['address'] if children else 0)
        for current, following in zip(children, children[1:]):
            put32(memory, current['address'] + 0x30, following['address'])
        if children:
            put32(memory, children[-1]['address'] + 0x30, 0)
    return root['address'], len(nodes)

rng = random.Random(ADDRESS)
fixed_shapes = [None, [], [None], [None, None], [[None]],
                [[None, None], [None]], [[], [None]],
                [[[], None], [None, [None]]], [None, None]]
cases = []
for shape in fixed_shapes:
    cases.append(shape)
for _ in range(64):
    def random_shape(depth=0):
        if depth >= 3 or rng.randrange(4) == 0:
            return None
        return [random_shape(depth + 1) for _ in range(rng.randrange(0, 4))]
    cases.append(random_shape())

records = []
for case, shape in enumerate(cases):
    ram = bytearray(b'\xa5') * RAM_SIZE
    ram[LOAD - RAM_BASE:LOAD - RAM_BASE + len(module)] = module
    root, nodes = make_tree(ram, 0x0C300000 + case * 0x2000, case, rng, shape)

    initial = State()
    initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
    initial.r[4] = root
    initial.r[13] = rng.getrandbits(32)
    initial.r[14] = 0x0C800000 + case * 0x100
    initial.r[15] = 0x0CF00000 + case * 0x1000
    initial.pr = 0x0CFF0000 + case * 4
    initial.pc = ADDRESS
    initial.sr = rng.getrandbits(32)
    expected_ram, actual_ram = bytearray(ram), bytearray(ram)
    expected = State.from_buffer_copy(initial)
    actual = State.from_buffer_copy(initial)
    steps = reference(expected_ram, expected, stop_pc=initial.pr, instruction_budget=10000)
    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    native(view, ctypes.byref(actual))
    assert actual_ram == expected_ram, f'RAM mismatch case={case}, nodes={nodes}'
    if bytes(actual) != bytes(expected):
        raise AssertionError(
            f'CPU mismatch case={case}, nodes={nodes}: '
            f'r={[(i, hex(actual.r[i]), hex(expected.r[i])) for i in range(16) if actual.r[i] != expected.r[i]]}, '
            f'sr={actual.sr:#x}/{expected.sr:#x}, pr={actual.pr:#x}/{expected.pr:#x}, '
            f'pc={actual.pc:#x}/{expected.pc:#x}')
    records.append({'case': case, 'nodes_allocated': nodes, 'retail_instructions': steps,
                    'return_count': actual.r[0], 'full_ram_cpu_match': True})

source = PROJECT / 'source/sa_stg09_recursive_tree_node_count_0c920d4c.c'
report = {
    'module': 'STG09.BIN', 'load_address': hex(LOAD), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': len(records),
    'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state against retail SH-4 execution.',
    'scope': '46-byte recursive child/sibling tree count, including flag-controlled child suppression, recursive call stack, sibling traversal, result accumulation, and restored PR/R13/R14/SP.',
    'limitations': ['Input is an acyclic child/sibling tree with valid in-RAM nodes and null-terminated sibling chains.'],
    'cases': records,
}
(PROJECT / 'analysis/common-overlays/STG09.BIN/recursive_tree_node_count_0c920d4c_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{len(records)} STG09 recursive tree-counter full RAM/CPU comparisons passed")
