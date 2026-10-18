"""Compare the compact STG09 selector and its tail-call boundaries to retail."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
LOAD, ADDRESS, LENGTH = 0x0C900000, 0x0C900C14, 30
RAM_SIZE = 16 * 1024 * 1024
module = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
span = module[ADDRESS - LOAD:ADDRESS - LOAD + LENGTH]
assert span == bytes.fromhex('43 60 0c 88 03 8b 0d d2 0b d4 2b 42 09 00 22 88 '
                             '03 8b 0a d3 0a d4 2b 43 09 00 fe af 09 00')

library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/stg09_dispatch_helper_0c900c14.dll'))
native = library.sa_stg09_dispatch_helper_0c900c14
CALLBACK = ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_uint8),
                            ctypes.POINTER(State), ctypes.c_uint32,
                            ctypes.c_void_p)
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                   CALLBACK, ctypes.c_void_p]
native.restype = None

def offset(address):
    result = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= result < RAM_SIZE - 4, hex(address)
    return result

records = []
rng = random.Random(0xC900C14)
values = [0, 1, 11, 12, 13, 33, 34, 35, 0x7fffffff, 0x80000000,
          0xffffffff] + [rng.getrandbits(32) for _ in range(245)]
for case, selector in enumerate(values):
    ram = bytearray(RAM_SIZE)
    ram[LOAD - 0x0C000000:LOAD - 0x0C000000 + len(module)] = module
    literals = [0x0CA00000 + case * 8, 0x8C600000 + case * 4,
                0x0CB00000 + case * 8]
    for address, value in zip((0x0C900C4C, 0x0C900C50, 0x0C900C54), literals):
        struct.pack_into('<I', ram, offset(address), value)

    initial = State()
    initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
    initial.r[4] = selector
    initial.sr = rng.getrandbits(32)
    initial.pr = 0x0CFF0000 + case * 4
    initial.pc = ADDRESS
    expected_ram, actual_ram = bytearray(ram), bytearray(ram)
    expected = State.from_buffer_copy(initial)
    actual = State.from_buffer_copy(initial)
    calls_expected, calls_actual = [], []

    def hook(target):
        def capture(_ram, regs, sr):
            calls_expected.append((target, tuple(regs), sr, expected.pr))
            return sr
        return capture

    hooks = {literals[1]: hook(literals[1])}
    stop = 0x0C900C2E if selector not in (12, 34) else initial.pr
    steps = reference(expected_ram, expected, hooks=hooks, stop_pc=stop,
                      instruction_budget=64)

    actual_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)

    @CALLBACK
    def tail(_ram, cpu_pointer, target, _context):
        cpu = cpu_pointer.contents
        calls_actual.append((target, tuple(cpu.r), cpu.sr, cpu.pr))

    native(actual_view, ctypes.byref(actual), tail, None)
    if actual.pc == literals[1]:
        actual.pc = actual.pr

    assert calls_actual == calls_expected, (f'tail-call boundary mismatch case={case}: '
        f'actual={calls_actual!r} expected={calls_expected!r}')
    assert bytes(actual) == bytes(expected), f'CPU state mismatch case={case}'
    assert actual_ram == expected_ram, f'RAM mismatch case={case}'
    records.append({'case': case, 'selector': hex(selector), 'retail_steps': steps,
                    'tail_calls': len(calls_actual), 'full_state_match': True})

source = PROJECT / 'source/sa_stg09_dispatch_helper_0c900c14.c'
report = {
    'module': 'STG09.BIN', 'load_address': hex(LOAD), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': len(records),
    'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state.',
    'scope': '30-byte selector; exact selector comparisons, argument setup, tail-call target, and wrong-selector loop entry.',
    'limitations': ['The shared callee bodies and the intentional self-loop body are outside this routine.'],
    'cases': records,
}
(PROJECT / 'analysis/common-overlays/STG09.BIN/dispatch_helper_0c900c14_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{len(records)} STG09 dispatch-helper full RAM/CPU/service-boundary comparisons passed')
