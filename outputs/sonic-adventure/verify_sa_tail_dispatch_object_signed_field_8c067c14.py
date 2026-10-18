import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C067C14
LENGTH = 14
TARGET = 0x8C067D84
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('fc 7f 42 2f 48 54 47 85 03 64 b1 a0 04 7f')

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'sa_tail_dispatch_object_signed_field_8c067c14.dll'))
native = library.sa_tail_dispatch_object_signed_field_8c067c14
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(FUNCTION)
cases = []
for case in range(160):
    ram = bytearray([0xA5]) * RAM_SIZE
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = 0x8C700000 + rng.randrange(0x10000 // 4) * 4
    state.r[15] = 0x8C800000 + rng.randrange(0x10000 // 4) * 4
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION

    object_address = state.r[4]
    nested = 0x8C710000 + rng.randrange(0x10000 // 4) * 4
    field_value = rng.choice([0, 1, 0x7FFF, 0x8000, 0xFF80, 0xFFFF, rng.getrandbits(16)])
    ram[object_address + 0x20 - 0x8C000000:object_address + 0x24 - 0x8C000000] = nested.to_bytes(4, 'little')
    ram[nested + 14 - 0x8C000000:nested + 16 - 0x8C000000] = field_value.to_bytes(2, 'little')

    if case == 0:
        state.r[15] = object_address + 0x24  # saved object overwrites its nested-pointer field
    elif case == 1:
        state.r[15] = nested + 18  # saved object overwrites the signed field before it is read
    elif case == 2:
        nested = object_address
        ram[object_address + 0x20 - 0x8C000000:object_address + 0x24 - 0x8C000000] = nested.to_bytes(4, 'little')

    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    reference(expected_ram, expected_state, stop_pc=TARGET)

    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual_state))
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch in case {case}'
    assert actual_state.pc == TARGET and actual_state.r[15] == state.r[15]
    cases.append({'case': case, 'object': hex(object_address), 'nested': hex(nested), 'field': hex(field_value), 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_tail_dispatch_object_signed_field_8c067c14.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against retail SH-4 interpreter at direct tail-call target',
    'scope': 'Save R4 at R15-4, follow the object pointer at R4+0x20, sign-extend the halfword at nested-pointer+14 into R4, and branch to 0x8c067d84 while restoring R15 in the delay slot.',
    'limitations': ['Behavior of the destination routine at 0x8c067d84 is verified separately.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'target': hex(TARGET), 'cases': cases,
}
(PROJECT / 'analysis' / 'sa_tail_dispatch_object_signed_field_8c067c14_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} signed object-field tail-dispatch RAM/CPU comparisons passed')
