"""Compare the indexed input-record address helper against original SH-4 execution."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
BASE, ADDRESS, LENGTH = 0x8C010000, 0x8C646856, 18
LITERAL, RAM_SIZE = 0x8C6468B0, 16 * 1024 * 1024
BINARY = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
SPAN = BINARY[ADDRESS - BASE:ADDRESS - BASE + LENGTH]
assert len(SPAN) == LENGTH
LIBRARY = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/sa_input_record_address_by_index_8c646856.dll'))
FUNCTION = LIBRARY.sa_input_record_address_by_index_8c646856
FUNCTION.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
FUNCTION.restype = None
rng = random.Random(ADDRESS)
indices = [0, 1, 2, 3, 0x7FFFFFFF, 0x80000000, 0xAAAAAAAA,
           0x55555555, 0xFFFFFFFF, 0x40000000, 0x40000001]
indices += [rng.getrandbits(32) for _ in range(245)]
cases = []
for case, index in enumerate(indices):
    initial = bytearray(b'\xA5') * RAM_SIZE
    initial[BASE - 0x8C000000:BASE - 0x8C000000 + len(BINARY)] = BINARY
    table_base = [0x8C893BD0, 0x8C700000, 0x8CFFFFFF, 0x00000000][case % 4]
    struct.pack_into('<I', initial, LITERAL & 0xFFFFFF, table_base)
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = index
    state.r[15] = 0x8C800000 + case * 4
    state.sr = rng.getrandbits(32)
    state.pr = 0x8CFED000 + case * 4
    state.pc = ADDRESS
    retail_ram, native_ram = bytearray(initial), bytearray(initial)
    expected, actual = State.from_buffer_copy(bytes(state)), State.from_buffer_copy(bytes(state))
    steps = reference(retail_ram, expected)
    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
    FUNCTION(view, ctypes.byref(actual))
    assert steps == 9
    assert retail_ram == native_ram, f'RAM mismatch case {case}'
    assert bytes(expected) == bytes(actual), f'CPU mismatch case {case} index={index:08x}'
    cases.append({'case': case, 'index': hex(index), 'table_base': hex(table_base),
                  'retail_instructions': steps, 'full_ram_and_cpu_match': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(SPAN).hexdigest(),
    'binary_sha256': hashlib.sha256(BINARY).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source/sa_input_record_address_by_index_8c646856.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'Complete 16 MiB RAM and all GPR/SR/PR/PC state against retail SH-4 execution.',
    'scope': 'Computes base + index*0x18 using the retail shift/add sequence and preserves its final T flag.',
    'limitations': ['The broader input subsystem and record consumers are separate routines.', 'Verification uses the project SH-4 interpreter, not Dreamcast hardware or complete-game execution.'],
    'cases': cases,
}
(PROJECT / 'analysis/sa_input_record_address_by_index_8c646856_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{len(cases)} indexed input-record address RAM/CPU comparisons passed')