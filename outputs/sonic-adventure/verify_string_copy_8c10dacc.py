import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C10DACC
LENGTH = 74
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert len(span) == LENGTH
literal = int.from_bytes(binary[0x8C10DB18 - BASE:0x8C10DB1C - BASE], 'little')
assert literal == 0x8C10EF20

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'string_copy_8c10dacc.dll'))
native = library.sa_string_copy_8c10dacc
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE, hex(address)
    return result

rng = random.Random(FUNCTION)
lengths = [0, 1, 2, 3, 4, 5, 7, 8, 15, 16, 31, 32, 63, 64, 127, 255]
alignment_pairs = [(0, 0), (1, 1), (2, 2), (3, 3), (0, 1), (1, 0), (0, 2), (3, 0)]
cases = []
for case in range(64):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    source_alignment, destination_alignment = alignment_pairs[case % len(alignment_pairs)]
    if case == 0:
        source_base, destination_base = 0x8C700000, 0x8C710000
    elif case == 1:
        source_base, destination_base = 0x8C000000, 0x8C001000
    elif case == 2:
        source_base, destination_base = 0x8C10F000, 0x8C10F400
    else:
        source_base = 0x8C740000 + case * 0x1000
        destination_base = 0x8C780000 + case * 0x1000
    source = source_base + source_alignment
    destination = destination_base + destination_alignment
    length = lengths[case] if case < len(lengths) else rng.randrange(0, 512)
    payload = bytes(rng.choice([1, 2, 0x41, 0x7F, 0x80, 0xFE, 0xFF]) for _ in range(length))
    src_at, dst_at = offset(source), offset(destination)
    ram[src_at:src_at + length] = payload
    ram[src_at + length] = 0

    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[0], state.r[1] = destination, source
    state.r[15] = 0x8C7F0000 - case * 0x100
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION

    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    reference(expected_ram, expected_state, instruction_budget=length * 24 + 512)
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual_state))

    assert actual_ram == expected_ram, f'RAM mismatch in case {case}, alignments {source_alignment}/{destination_alignment}, length {length}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU state mismatch in case {case}, alignments {source_alignment}/{destination_alignment}, length {length}'
    assert actual_ram[dst_at:dst_at + length + 1] == payload + b'\0'
    cases.append({
        'case': case, 'string_length': length,
        'source': hex(source), 'destination': hex(destination),
        'source_alignment': source_alignment, 'destination_alignment': destination_alignment,
        'ram_and_cpu_match': True,
    })

source_path = PROJECT / 'source' / 'sa_string_copy_8c10dacc.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source_path.read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against the retail SH-4 interpreter',
    'scope': 'String-copy wrapper with aligned optimized helper and unaligned byte-copy paths, including visible stack writes and return state.',
    'limitations': [
        'Overlapping source and destination behavior is not characterized.',
        'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'
    ],
    'cases': cases,
}
(PROJECT / 'analysis' / 'string_copy_8c10dacc_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} string-copy wrapper complete RAM/CPU comparisons passed')
