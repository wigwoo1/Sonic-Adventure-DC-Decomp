import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C037618
LENGTH = 18
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('04 d1 12 58 04 d9 99 28 04 d9 09 29 9b 28 0b 00 82 11')

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'global_masked_update_8c037618.dll'))
native = library.sa_global_masked_update_8c037618
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE, hex(address)
    return result

rng = random.Random(FUNCTION)
base_pointer = struct.unpack_from('<I', binary, 0x8C03762C - BASE)[0]
mask_a = struct.unpack_from('<I', binary, 0x8C037630 - BASE)[0]
mask_b = struct.unpack_from('<I', binary, 0x8C037634 - BASE)[0]
target = base_pointer + 8
offset(target)
cases = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    initial = rng.getrandbits(32)
    struct.pack_into('<I', ram, offset(target), initial)
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    if case < 8:
        state.r[0] = [0, 1, 0xFFFFFFFF, mask_a, mask_b, mask_a & mask_b, 0x80000000, 0x55555555][case]
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
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch in case {case}'
    cases.append({'case': case, 'input_r0': hex(state.r[0]), 'initial_word': hex(initial), 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_global_masked_update_8c037618.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against the retail SH-4 interpreter',
    'scope': 'Exact 18-byte routine; applies the retail literal masks to the word at base+8 using R0 and checks RAM and all modeled register effects.',
    'limitations': ['The global structure field semantics and caller-level meaning of R0 remain unidentified.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'literal_values': {'base_pointer': hex(base_pointer), 'mask_a': hex(mask_a), 'mask_b': hex(mask_b)},
    'cases': cases,
}
(PROJECT / 'analysis' / 'global_masked_update_8c037618_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} global masked-update RAM/CPU comparisons passed')
