import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C65BFC2
LENGTH = 32
END = 0x8C65C01C
SPAN_LENGTH = END - FUNCTION
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:END - BASE]
assert len(span) == SPAN_LENGTH
assert int.from_bytes(binary[0x8C65C0F2 - BASE:0x8C65C0F4 - BASE], 'little') == 0x07FF

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'stream_position_set_8c65bfc2.dll'))
native = library.sa_stream_position_set_8c65bfc2
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE, hex(address)
    return result

rng = random.Random(FUNCTION)
position_values = [
    0, 1, 0x7FF, 0x800, 0x7FFFFFFF,
    0x80000000, 0x80000001, 0xFFFFF800, 0xFFFFFFFF,
    0xFFFFF7FF, 0xFFFFF000,
]
offset_values = [
    0, 1, 0x7FF, 0x800, 0xFFF, 0x1000,
    0x7FFFFFFF, 0x80000000, 0x80000001,
    0xFFFFF000, 0xFFFFF801, 0xFFFFF800,
    0xFFFFF7FF, 0xFFFFFFFF,
]
cases = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    obj = 0x8C780400 + case * 0x100
    record = bytearray(rng.getrandbits(8) for _ in range(0x40))
    prior = offset_values[case % len(offset_values)] if case < 56 else rng.getrandbits(32)
    struct.pack_into('<I', record, 0x14, prior)
    record_at = offset(obj)
    ram[record_at:record_at + len(record)] = record
    input_value = position_values[case % len(position_values)] if case < len(position_values) else rng.getrandbits(32)

    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = obj
    state.r[5] = input_value
    state.r[15] = 0x8C781F00 - case * 8
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION

    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    reference(expected_ram, expected_state)
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual_state))

    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual_state) == bytes(expected_state), (
        f'CPU state mismatch in case {case}, input={input_value:#x}, prior={prior:#x}, '
        f'registers={[ (i, hex(actual_state.r[i]), hex(expected_state.r[i])) for i in range(16) if actual_state.r[i] != expected_state.r[i] ]}, '
        f'sr={actual_state.sr:#x}/{expected_state.sr:#x}, '
        f'pr={actual_state.pr:#x}/{expected_state.pr:#x}, pc={actual_state.pc:#x}/{expected_state.pc:#x}'
    )
    result = struct.unpack_from('<I', actual_ram, record_at + 0x30)[0]
    cases.append({
        'case': case, 'input': hex(input_value), 'prior_position': hex(prior),
        'stored_result': hex(result), 'ram_and_cpu_match': True,
    })

source_path = PROJECT / 'source' / 'sa_stream_position_set_8c65bfc2.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(END), 'indexed_instruction_bytes': LENGTH,
    'instruction_body_ranges': [
        {'start': '0x8c65bfc2', 'end_exclusive': '0x8c65bfca'},
        {'start': '0x8c65c004', 'end_exclusive': '0x8c65c01c'},
    ],
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source_path.read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against the retail SH-4 interpreter',
    'scope': 'Store a direct nonnegative position or round a negative byte position into the stream’s 2 KiB units, including wraparound, state flags, and record memory effects.',
    'limitations': [
        'The meaning of the complete stream record and its caller-side range requirements are not established by this setter alone.',
        'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'
    ],
    'cases': cases,
}
(PROJECT / 'analysis' / 'stream_position_set_8c65bfc2_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} stream-position complete RAM/CPU comparisons passed')
