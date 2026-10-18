import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
LOAD_ADDRESS = 0x0C900000
FUNCTION = 0x0C9005BA
LENGTH = 42
RAM_SIZE = 16 * 1024 * 1024
MODULE_PATH = ROOT / 'work' / 'sonic-adventure' / 'decoded' / 'SONICADV' / 'STG09.BIN'
module = MODULE_PATH.read_bytes()
span = module[FUNCTION - LOAD_ADDRESS:FUNCTION - LOAD_ADDRESS + LENGTH]
assert span == bytes.fromhex(
    '22 4f 30 d2 0b 42 09 00 32 e4 2f d3 18 44 c7 74 0b 43 09 00 '
    '2e d2 00 e5 0b 42 53 64 33 bd 00 e4 31 bd 01 e4 2b d3 2b 43 26 4f'
)

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' /
                          'stg09_callback_0c9005ba.dll'))
native = library.sa_stg09_callback_0c9005ba


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


def read32(ram, address):
    return struct.unpack_from('<I', ram, offset(address))[0]


original_literals = [struct.unpack_from('<I', module, address - LOAD_ADDRESS)[0]
                     for address in (0x0C900680, 0x0C900684,
                                     0x0C900688, 0x0C90068C)]
rng = random.Random(FUNCTION)
cases = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    module_at = LOAD_ADDRESS - 0x0C000000
    ram[module_at:module_at + len(module)] = module

    if case < 4:
        targets = list(original_literals)
    else:
        targets = [0x8C700000 + case * 0x20,
                   0x0C901000 + case * 0x20,
                   0x8C710000 + case * 0x20,
                   0x0C902000 + case * 0x20]
        for address, target in zip((0x0C900680, 0x0C900684,
                                    0x0C900688, 0x0C90068C), targets):
            struct.pack_into('<I', ram, offset(address), target)

    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[15] = 0x8CF00000 + case * 0x100
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8C010000, 0x8C74913C, 0x8CFED000])
    state.pc = FUNCTION

    expected_ram = bytearray(ram)
    expected = State.from_buffer_copy(state)
    expected_calls = []

    def make_hook(target):
        def hook(_hook_ram, regs, sr):
            expected_calls.append((target, tuple(regs), sr, expected.pr))
            return sr
        return hook

    hook_targets = set(targets[:3]) | {0x0C900040}
    hooks = {target: make_hook(target) for target in hook_targets}
    reference(expected_ram, expected, hooks=hooks, stop_pc=targets[3])

    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual = State.from_buffer_copy(state)
    actual_calls = []

    @CALLBACK
    def capture_call(_native_ram, cpu_pointer, target, _context):
        cpu = cpu_pointer.contents
        actual_calls.append((target, tuple(cpu.r), cpu.sr, cpu.pr))

    native(ram_view, ctypes.byref(actual), capture_call, None)
    assert actual_calls == expected_calls, (
        f'call boundary mismatch case={case}: {actual_calls!r} != {expected_calls!r}')
    assert bytes(actual) == bytes(expected), f'CPU mismatch case={case}'
    assert actual_ram == expected_ram, f'RAM mismatch case={case}'
    assert actual.pc == targets[3], f'tail target mismatch case={case}'
    cases.append({'case': case, 'external_calls': len(actual_calls),
                  'tail_target': hex(actual.pc), 'full_ram_cpu_match': True})

source = PROJECT / 'source' / 'sa_stg09_callback_0c9005ba.c'
report = {
    'module': 'STG09.BIN',
    'load_address': hex(LOAD_ADDRESS),
    'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state',
    'scope': ('42-byte retail callback; exact indirect/direct call sequence, '
              'arguments, delay-slot effects, saved/restored PR, and tail target'),
    'external_boundary': ('Five service calls are intercepted at their retail targets; '
                          'service bodies and the final tail target are outside this routine.'),
    'cases': cases,
}
(PROJECT / 'analysis' / 'stg09_callback_0c9005ba_verification.json').write_text(
    json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} STG09 callback full RAM/CPU/service-boundary comparisons passed')
