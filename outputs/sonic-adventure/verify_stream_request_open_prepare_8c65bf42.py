import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
HELPER = 0x8C65BF42
HELPER_END = 0x8C65BF60
QUERY = 0x8C600EC6
QUERY_END = 0x8C600EDC
CALLBACK_SLOT = 0x8C65BFEC
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
assert struct.unpack_from('<I', binary, CALLBACK_SLOT - BASE)[0] == QUERY

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'stream_request_open_prepare_8c65bf42.dll'))
native_helper = library.sa_stream_request_open_prepare_8c65bf42
native_query = library.sa_stream_request_query_8c600ec6
for function in (native_helper, native_query):
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None

def seed_ram():
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    return ram

def compare(ram, state, native, label):
    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    reference(expected_ram, expected_state)
    actual_ram = bytearray(ram)
    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(view, ctypes.byref(actual_state))
    assert actual_ram == expected_ram, f'{label}: RAM mismatch'
    assert bytes(actual_state) == bytes(expected_state), (
        f'{label}: CPU mismatch: actual={[hex(v) for v in actual_state.r]}, '
        f'expected={[hex(v) for v in expected_state.r]}, '
        f'SR={actual_state.sr:#x}/{expected_state.sr:#x}'
    )

rng = random.Random(HELPER)
cases = []
# Verify the callback itself over null, unavailable, and ready request records.
for case in range(64):
    ram = seed_ram()
    request = 0 if case % 4 == 0 else 0x8C790000 + case * 0x80
    if request:
        struct.pack_into('<H', ram, request - 0x8C000000 + 0x4C, 0 if case % 4 == 1 else 1)
        struct.pack_into('<I', ram, request - 0x8C000000 + 0x14,
                         0xFFFFFFFF if case % 4 == 2 else rng.getrandbits(32))
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = request
    state.r[15] = 0x8C7FF000
    state.sr = rng.getrandbits(32)
    state.pr = 0x8C74913C
    state.pc = QUERY
    compare(ram, state, native_query, f'callback case {case}')
    cases.append({'case': case, 'routine': hex(QUERY), 'request': hex(request), 'matched': True})

# Verify the stream-open wrapper and its indirect callback call as one retail execution.
for case in range(128):
    ram = seed_ram()
    obj = 0x8C780400 + case * 0x100
    request = 0 if case % 5 == 0 else 0x8C790000 + case * 0x80
    struct.pack_into('<I', ram, obj - 0x8C000000 + 8, request)
    if request:
        struct.pack_into('<H', ram, request - 0x8C000000 + 0x4C,
                         0 if case % 5 == 1 else (case & 0xFFFF))
        struct.pack_into('<I', ram, request - 0x8C000000 + 0x14, rng.getrandbits(32))
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = obj
    state.r[15] = 0x8C7FF000
    state.sr = rng.getrandbits(32)
    state.pr = 0x8C74913C
    state.pc = HELPER
    compare(ram, state, native_helper, f'wrapper case {case}')
    cases.append({'case': case, 'routine': hex(HELPER), 'request': hex(request), 'matched': True})

source_path = PROJECT / 'source' / 'sa_stream_request_open_prepare_8c65bf42.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE),
    'address': hex(HELPER), 'end_exclusive': hex(HELPER_END),
    'indexed_instruction_bytes': 30,
    'related_callback': {'address': hex(QUERY), 'end_exclusive': hex(QUERY_END),
                         'indexed_instruction_bytes': 22},
    'callback_literal': hex(CALLBACK_SLOT),
    'retail_span_sha256': hashlib.sha256(binary[HELPER - BASE:HELPER_END - BASE]).hexdigest(),
    'callback_span_sha256': hashlib.sha256(binary[QUERY - BASE:QUERY_END - BASE]).hexdigest(),
    'source_sha256': hashlib.sha256(source_path.read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against the retail SH-4 interpreter',
    'scope': 'Prepare the stream-open record and run its indirect request-readiness callback, including both helper and callback register effects.',
    'limitations': [
        'Only the stream-open helper and its request-query callback are covered; broader stream lifecycle behavior remains unverified.',
        'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'
    ],
    'cases': cases,
}
(PROJECT / 'analysis' / 'stream_request_open_prepare_8c65bf42_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{len(cases)} stream callback and helper RAM/CPU comparisons passed")
