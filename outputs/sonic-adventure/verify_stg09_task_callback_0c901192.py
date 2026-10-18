"""Differentially verify the STG09 0x1234 task callback against retail."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
LOAD, ADDRESS, LENGTH = 0x0C900000, 0x0C901192, 38
RAM_SIZE = 16 * 1024 * 1024
module = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
span = module[ADDRESS - LOAD:ADDRESS - LOAD + LENGTH]
assert span == bytes.fromhex('22 4f fc 7f 16 d3 03 e5 14 d6 40 2f 0b 43 02 e4 '
                             '08 54 12 e0 18 40 34 70 43 81 f0 60 43 80 04 7f '
                             '26 4f 0b 00 09 00')
assert struct.unpack_from('<I', module, 0x11ec)[0] == 0x0C90107E
assert struct.unpack_from('<I', module, 0x11f0)[0] == 0x8C09852E

library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/stg09_task_callback_0c901192.dll'))
native = library.sa_stg09_task_callback_0c901192
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

records = []
rng = random.Random(0xC901192)
for case in range(96):
    ram = bytearray(RAM_SIZE)
    ram[LOAD - 0x0C000000:LOAD - 0x0C000000 + len(module)] = module
    stack = 0x0CF00000 + case * 0x100
    task = 0x0C800000 + case * 0x100
    work = 0x0C810000 + case * 0x100
    if case == 80:
        work = stack - 8
    elif case == 81:
        work = stack - 11
    elif case == 82:
        work = stack - 4
    struct.pack_into('<I', ram, offset(task + 0x20), work)

    initial = State()
    initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
    initial.r[4] = rng.getrandbits(32)
    initial.r[15] = stack
    initial.sr = rng.getrandbits(32)
    initial.pr = 0x0CFF0000 + case * 4
    initial.pc = ADDRESS
    expected_ram, actual_ram = bytearray(ram), bytearray(ram)
    expected, actual = State.from_buffer_copy(initial), State.from_buffer_copy(initial)
    expected_calls, actual_calls = [], []

    def hook(_ram, regs, sr):
        expected_calls.append((0x8C09852E, tuple(regs), sr, expected.pr))
        regs[0] = task
        return sr

    steps = reference(expected_ram, expected, hooks={0x8C09852E: hook},
                      stop_pc=0x0C9011B4)
    actual_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)

    @CALLBACK
    def capture(_ram, cpu_pointer, target, _context):
        cpu = cpu_pointer.contents
        actual_calls.append((target, tuple(cpu.r), cpu.sr, cpu.pr))
        cpu.r[0] = task

    native(actual_view, ctypes.byref(actual), capture, None)
    assert actual.pc == actual.pr, f'return target mismatch case={case}'
    actual.pc = 0x0C9011B4
    assert actual_calls == expected_calls, f'allocator boundary mismatch case={case}'
    assert bytes(actual) == bytes(expected), f'full CPU state mismatch case={case}'
    assert actual_ram == expected_ram, f'full RAM mismatch case={case}'
    records.append({'case': case, 'argument_byte': initial.r[4] & 255,
                    'task': hex(task), 'work': hex(work), 'retail_steps': steps,
                    'full_ram_cpu_match': True})

source = PROJECT / 'source/sa_stg09_task_callback_0c901192.c'
report = {
    'module': 'STG09.BIN', 'load_address': hex(LOAD), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': len(records),
    'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state.',
    'scope': '38-byte task callback; allocator arguments, work-area marker 0x1234, saved argument byte and return frame.',
    'limitations': ['The task allocator body is intercepted at its call boundary.',
                    'Aliased work-area cases stop at the RTS boundary after comparing the restored/corrupted PR.'],
    'cases': records,
}
(PROJECT / 'analysis/common-overlays/STG09.BIN/task_callback_0c901192_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{len(records)} STG09 0x1234 task-callback full RAM/CPU/service-boundary comparisons passed')
