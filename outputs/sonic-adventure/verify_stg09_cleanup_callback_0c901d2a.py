"""Verify STG09 task cleanup, optional resources, calls, and tail transfer."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
LOAD, ADDRESS, LENGTH = 0x0C900000, 0x0C901D2A, 100
RAM_SIZE = 16 * 1024 * 1024
module = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
span = module[ADDRESS - LOAD:ADDRESS - LOAD + LENGTH]
assert len(span) == LENGTH
assert [struct.unpack_from('<I', module, address - LOAD)[0]
        for address in range(0x0C901EE0, 0x0C901F00, 4)] == [
            0x0C900C14, 0x8C639C70, 0x8C63A804, 0x8C63A1CC,
            0x8C639EC8, 0x8C639F5C, 0x8C03700C, 0x8C639B98]

library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/stg09_cleanup_callback_0c901d2a.dll'))
native = library.sa_stg09_cleanup_callback_0c901d2a
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

rng = random.Random(0xC901D2A)
records = []
for case in range(128):
    ram = bytearray(b'\xa5') * RAM_SIZE
    ram[LOAD - 0x0C000000:LOAD - 0x0C000000 + len(module)] = module
    context = 0x0C800000 + case * 0x100
    work = 0x0C810000 + case * 0x100
    struct.pack_into('<I', ram, offset(context + 0x20), work)
    fields = [rng.getrandbits(32) for _ in range(8)]
    # Exercise all optional service combinations and a wraparound-zero sum.
    flags = case & 7
    fields[7] = 0 if flags & 1 else (rng.getrandbits(32) or 1)  # work+0x1c
    fields[5] = 0 if flags & 2 else (rng.getrandbits(32) or 1)  # work+0x14
    if flags & 4:
        fields[6], fields[3] = 0xFFFFFFFF, 1  # work+0x18 + work+0x0c wraps to 0
    else:
        fields[6], fields[3] = 5 + case, 7
    fields[2] = rng.getrandbits(32)  # work+8
    for displacement, value in zip((0, 4, 8, 12, 16, 20, 24, 28), fields):
        struct.pack_into('<I', ram, offset(work + displacement), value)

    targets = [0x8C720000 + case * 0x80 + index * 4 for index in range(8)]
    for address, target in zip(range(0x0C901EE0, 0x0C901F00, 4), targets):
        struct.pack_into('<I', ram, offset(address), target)

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

    def make_hook(target):
        def hook(_ram, regs, sr):
            expected_calls.append((target, tuple(regs), sr, expected.pr))
            return sr
        return hook

    hooks = {target: make_hook(target) for target in targets}
    steps = reference(expected_ram, expected, hooks=hooks)
    actual_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)

    @CALLBACK
    def capture(_ram, cpu_pointer, target, _context):
        cpu = cpu_pointer.contents
        actual_calls.append((target, tuple(cpu.r), cpu.sr, cpu.pr))

    native(actual_view, ctypes.byref(actual), capture, None)
    assert actual_calls == expected_calls, f'call boundary mismatch case={case}'
    assert bytes(actual) == bytes(expected), f'CPU mismatch case={case}'
    assert actual_ram == expected_ram, f'RAM mismatch case={case}'
    records.append({'case': case, 'optional_service_flags': flags,
                    'services_seen': len(actual_calls), 'retail_steps': steps,
                    'full_ram_cpu_match': True})

source = PROJECT / 'source/sa_stg09_cleanup_callback_0c901d2a.c'
report = {
    'module': 'STG09.BIN', 'load_address': hex(LOAD), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': len(records),
    'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state.',
    'scope': '100-byte cleanup callback; saved R14/PR frame, conditional resource service calls, final service arguments and tail-transfer boundary.',
    'limitations': ['Service bodies, including the initial stage selector and final dispatcher, are intercepted at their call boundaries.'],
    'cases': records,
}
(PROJECT / 'analysis/common-overlays/STG09.BIN/cleanup_callback_0c901d2a_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{len(records)} STG09 cleanup-callback full RAM/CPU/service-boundary comparisons passed')
