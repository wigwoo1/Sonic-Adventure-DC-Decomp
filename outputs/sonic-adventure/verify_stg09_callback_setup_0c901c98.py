"""Differentially verify the STG09 callback setup against retail."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
LOAD, ADDRESS, LENGTH = 0x0C900000, 0x0C901C98, 44
RAM_SIZE = 16 * 1024 * 1024
module = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
span = module[ADDRESS - LOAD:ADDRESS - LOAD + LENGTH]
assert span == bytes.fromhex('e6 2f 01 e3 22 4f 19 d5 04 e6 19 d2 43 6e 36 2f '
                             '33 67 0b 42 e3 64 e8 53 3c e0 17 d2 33 80 15 d3 '
                             '34 1e 26 1e 04 7f 26 4f 0b 00 f6 6e')

library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/stg09_callback_setup_0c901c98.dll'))
native = library.sa_stg09_callback_setup_0c901c98
CALLBACK = ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_uint8),
                            ctypes.POINTER(State), ctypes.c_uint32,
                            ctypes.c_void_p)
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                   CALLBACK, ctypes.c_void_p]
native.restype = None

def offset(address):
    value = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= value < RAM_SIZE - 4, hex(address)
    return value

rng = random.Random(0xC901C98)
records = []
for case in range(128):
    ram = bytearray(b'\xa5') * RAM_SIZE
    ram[LOAD - 0x0C000000:LOAD - 0x0C000000 + len(module)] = module
    context = 0x0C800000 + case * 0x100
    work = 0x0C810000 + case * 0x100
    struct.pack_into('<I', ram, offset(context + 0x20), work)

    initial = State()
    initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
    initial.r[4] = context
    initial.r[15] = 0x0CF00000 + case * 0x100
    initial.sr = rng.getrandbits(32)
    initial.pr = 0x0CFF0000 + case * 4
    initial.pc = ADDRESS
    expected_ram, actual_ram = bytearray(ram), bytearray(ram)
    expected, actual = State.from_buffer_copy(initial), State.from_buffer_copy(initial)
    expected_calls, actual_calls = [], []

    def hook(_ram, regs, sr):
        expected_calls.append((0x8C02F4B0, tuple(regs), sr, expected.pr))
        return sr

    steps = reference(expected_ram, expected, hooks={0x8C02F4B0: hook})
    actual_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)

    @CALLBACK
    def capture(_ram, cpu_pointer, target, _context):
        cpu = cpu_pointer.contents
        actual_calls.append((target, tuple(cpu.r), cpu.sr, cpu.pr))

    native(actual_view, ctypes.byref(actual), capture, None)
    assert actual_calls == expected_calls, f'service boundary mismatch case={case}'
    assert bytes(actual) == bytes(expected), f'full CPU mismatch case={case}'
    assert actual_ram == expected_ram, f'full RAM mismatch case={case}'
    records.append({'case': case, 'context': hex(context), 'work': hex(work),
                    'retail_steps': steps, 'full_ram_cpu_match': True})

source = PROJECT / 'source/sa_stg09_callback_setup_0c901c98.c'
report = {
    'module': 'STG09.BIN', 'load_address': hex(LOAD), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': len(records),
    'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state.',
    'scope': '44-byte callback; service arguments, saved R14/PR/R3 stack frame, work timer, callback pointers and return.',
    'limitations': ['The initializer service body is intercepted at its call boundary.'],
    'cases': records,
}
(PROJECT / 'analysis/common-overlays/STG09.BIN/callback_setup_0c901c98_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{len(records)} STG09 callback-setup full RAM/CPU/service-boundary comparisons passed')
