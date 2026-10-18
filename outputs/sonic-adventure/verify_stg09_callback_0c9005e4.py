"""Full-state comparison of the STG09 callback against retail SH-4 execution."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference
from verify_fpu_cleanup_transfer import Fpu

PROJECT = ROOT / 'outputs/sonic-adventure'
LOAD, ADDRESS, LENGTH = 0x0C900000, 0x0C9005E4, 74
RAM_SIZE = 16 * 1024 * 1024
module = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
span = module[ADDRESS - LOAD:ADDRESS - LOAD + LENGTH]
assert len(span) == LENGTH

library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/stg09_callback_0c9005e4.dll'))
native = library.sa_stg09_callback_0c9005e4
CALLBACK = ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_uint8),
                            ctypes.POINTER(State), ctypes.POINTER(Fpu),
                            ctypes.c_uint32, ctypes.c_void_p)
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                   ctypes.POINTER(Fpu), CALLBACK, ctypes.c_void_p]
native.restype = None

def offset(address):
    value = (address & 0x1FFFFFFF) - 0x0C000000
    assert 0 <= value < RAM_SIZE - 4, hex(address)
    return value

def read32(ram, address):
    return struct.unpack_from('<I', ram, offset(address))[0]

records = []
for case in range(192):
    rng = random.Random(0xC9005E4 + case)
    ram = bytearray(RAM_SIZE)
    ram[LOAD - 0x0C000000:LOAD - 0x0C000000 + len(module)] = module
    targets = [0x8C04F7E0 + case * 0x20,
               0x8C098786 + case * 0x20,
               0x8C07C65E + case * 0x20]
    for literal, target in zip((0x0C900690, 0x0C900694, 0x0C900698), targets):
        struct.pack_into('<I', ram, offset(literal), target)

    callback_values = [0, 1, 2, 3, 0x7f, 0x80, 0xff]
    callback_state = callback_values[case % len(callback_values)] if case < 21 else rng.randrange(256)
    task = 0x0C800000 + case * 0x100
    work = 0x0C810000 + case * 0x100
    timers = [0, 1, 2, 127, 128, 254, 255]
    timer = timers[(case // 7) % len(timers)] if case < 49 else rng.randrange(256)
    struct.pack_into('<I', ram, offset(task + 0x20), work)
    ram[offset(work + 3)] = timer

    initial = State()
    initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
    initial.r[15] = 0x0CF00000 + case * 0x100
    initial.r[4] = task
    initial.r[14] = rng.getrandbits(32)
    initial.sr = rng.getrandbits(32)
    initial.pr = 0x0CFFFFFC
    initial.pc = ADDRESS
    fpu = Fpu()
    fpu.fpscr = 0x40000 | ((case & 1) << 21) | (case & 1)
    for bank in range(2):
        fpu.banks[bank][:] = [rng.getrandbits(32) for _ in range(16)]

    expected_ram, actual_ram = bytearray(ram), bytearray(ram)
    expected = State.from_buffer_copy(initial)
    actual = State.from_buffer_copy(initial)
    expected_fpu = Fpu.from_buffer_copy(fpu)
    actual_fpu = Fpu.from_buffer_copy(fpu)
    expected_calls = []

    def hook_for(target):
        def hook(_ram, regs, sr):
            if target == targets[0]:
                regs[0] = callback_state
            expected_calls.append((target, tuple(regs), sr, expected.pr,
                                   bytes(expected_fpu)))
            return sr
        return hook

    hooks = {target: hook_for(target) for target in targets}
    steps = reference(expected_ram, expected, hooks=hooks, fpu=expected_fpu,
                      stop_pc=initial.pr)

    actual_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_calls = []

    @CALLBACK
    def capture(_ram, cpu_pointer, fpu_pointer, target, _context):
        cpu = cpu_pointer.contents
        if target == targets[0]:
            cpu.r[0] = callback_state
        actual_calls.append((target, tuple(cpu.r), cpu.sr, cpu.pr,
                             bytes(fpu_pointer.contents)))

    native(actual_view, ctypes.byref(actual), ctypes.byref(actual_fpu), capture, None)
    # Model the final indirect tail transfer as a boundary callback and return.
    if actual.pc == targets[1]:
        actual_calls.append((actual.pc, tuple(actual.r), actual.sr, actual.pr,
                             bytes(actual_fpu)))
        actual.pc = actual.pr

    if actual_calls != expected_calls:
        detail = []
        for index, (got, want) in enumerate(zip(actual_calls, expected_calls)):
            changed = [(reg, got[1][reg], want[1][reg]) for reg in range(16)
                       if got[1][reg] != want[1][reg]]
            detail.append({'index': index, 'target': (hex(got[0]), hex(want[0])),
                           'changed_registers': changed,
                           'sr': (hex(got[2]), hex(want[2])),
                           'pr': (hex(got[3]), hex(want[3]))})
        raise AssertionError(f'call boundaries differ case={case} lengths={len(actual_calls)}/{len(expected_calls)}: {detail}')
    assert bytes(actual) == bytes(expected), f'CPU state differs case={case}'
    assert bytes(actual_fpu) == bytes(expected_fpu), f'FPU state differs case={case}'
    assert actual_ram == expected_ram, f'full RAM differs case={case}'
    records.append({'case': case, 'callback_state': callback_state,
                    'initial_timer': timer, 'retail_steps': steps,
                    'calls': len(actual_calls), 'full_state_match': True})

source = PROJECT / 'source/sa_stg09_callback_0c9005e4.c'
report = {
    'module': 'STG09.BIN', 'load_address': hex(LOAD),
    'address': hex(ADDRESS), 'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': len(records),
    'comparison_level': 'All 16 MiB RAM, GPR/SR/PR/PC and both FPU banks/FPSCR/FPUL.',
    'scope': '74-byte STG09 callback state test, countdown byte update/wrap, dispatch call, and tail transfer.',
    'limitations': ['Service routine bodies and final dispatcher body are intercepted at their boundaries.',
                    'The SH-4 interpreter is partial and this does not verify Dreamcast hardware behavior.'],
    'cases': records,
}
(PROJECT / 'analysis/common-overlays/STG09.BIN/callback_0c9005e4_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{len(records)} STG09 callback full RAM/CPU/FPU/service-boundary comparisons passed')
