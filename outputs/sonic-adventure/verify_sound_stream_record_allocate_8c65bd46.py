import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C65BD46
END = 0x8C65BDCA
TABLE = 0x8C8A3D90
TABLE_SIZE = 32 * 100
OBJECT = 0x8C7F0000
VTABLE = 0x8C7EF000
METHOD = 0x8C602618
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:END - BASE]
assert len(span) == 132

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'sound_stream_record_allocate_8c65bd46.dll'))
NativeMethod = ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State), ctypes.c_uint32, ctypes.c_uint32)
native = library.sa_sound_stream_record_allocate_8c65bd46
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State), NativeMethod]
native.restype = None
native_entries = []

def test_object_method(_ram, state_pointer, object_address, argument):
    # The test vtable selects retail sa_subtract_arguments_8c602618.
    state = state_pointer.contents
    memory = (ctypes.c_uint8 * RAM_SIZE).from_address(ctypes.addressof(_ram.contents))
    native_entries.append((bytes(state), hashlib.sha256(memory).hexdigest()))
    state.r[0] = (argument - object_address) & 0xFFFFFFFF
    state.pc = state.pr

method = NativeMethod(test_object_method)
rng = random.Random(FUNCTION)
position_values = [
    0, 1, 0x7FF, 0x800, 0xFFFFF801, 0xFFFFF800,
    0x80000000, 0x80000001, 0x7FFFFFFF, 0xFFFFFFFF,
]
cases = []

for case in range(64):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    table_at = TABLE - 0x8C000000
    table_bytes = bytearray(rng.getrandbits(8) for _ in range(TABLE_SIZE))
    free_slot = case if case < 32 else (None if case < 48 else rng.randrange(32))
    for index in range(32):
        table_bytes[index * 100] = (rng.randrange(1, 256) if index != free_slot else 0)
    ram[table_at:table_at + TABLE_SIZE] = table_bytes

    struct.pack_into('<I', ram, OBJECT - 0x8C000000, VTABLE)
    struct.pack_into('<I', ram, VTABLE - 0x8C000000 + 0x24, METHOD)
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = rng.getrandbits(32)
    state.r[5] = rng.getrandbits(32)
    state.r[6] = position_values[case % len(position_values)]
    state.r[7] = OBJECT
    state.r[15] = 0x8C7FF000 - case * 0x20
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION

    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    expected_entries = []

    def trace(pc, word, registers, sr, pr, fpu):
        if pc == METHOD:
            entry = State()
            entry.r[:] = registers
            entry.sr, entry.pr, entry.pc = sr, pr, pc
            expected_entries.append((bytes(entry), hashlib.sha256(expected_ram).hexdigest()))

    reference(expected_ram, expected_state, trace=trace)
    actual_ram = bytearray(ram)
    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native_entries.clear()
    native(view, ctypes.byref(actual_state), method)
    assert native_entries == expected_entries, f'Callback entry mismatch in case {case}'
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}, free_slot={free_slot}'
    assert bytes(actual_state) == bytes(expected_state), (
        f'CPU mismatch in case {case}, free_slot={free_slot}: '
        f'actual={[hex(v) for v in actual_state.r]}, '
        f'expected={[hex(v) for v in expected_state.r]}, '
        f'SR={actual_state.sr:#x}/{expected_state.sr:#x}'
    )
    cases.append({
        'case': case, 'free_slot': free_slot,
        'param3': hex(state.r[6]), 'matched': True,
    })

source_path = PROJECT / 'source' / 'sa_sound_stream_record_allocate_8c65bd46.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE),
    'address': hex(FUNCTION), 'end_exclusive': hex(END),
    'indexed_instruction_bytes': len(span),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source_path.read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'callback_entries_compared': sum(case['free_slot'] is not None for case in cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against the retail SH-4 interpreter after return and at every object callback entry',
    'scope': 'Find one of 32 stream records, initialize its fields and rounded position, call the selected object vtable method, and return the record; also verify full-table failure.',
    'test_vtable_method': hex(METHOD),
    'limitations': [
        'The indirect object method is tested with the retail two-argument subtraction helper; other vtable implementations are external behavior and are supplied through the native callback interface.',
        'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'
    ],
    'cases': cases,
}
(PROJECT / 'analysis' / 'sound_stream_record_allocate_8c65bd46_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{len(cases)} sound stream record allocator RAM/CPU comparisons passed')

