"""Differentially verify the shared input metadata bit locator."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
BASE, ADDRESS, LENGTH = 0x8C010000, 0x8C646B42, 48
MASK_LITERAL, RAM_SIZE = 0x8C646C3C, 16 * 1024 * 1024
BINARY = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
SPAN = BINARY[ADDRESS - BASE:ADDRESS - BASE + LENGTH]
assert len(SPAN) == LENGTH
assert int.from_bytes(BINARY[MASK_LITERAL - BASE:MASK_LITERAL - BASE + 4], 'little') == 0x80000000
LIBRARY = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/sa_input_metadata_bit_locator_8c646b42.dll'))
FUNCTION = LIBRARY.sa_input_metadata_bit_locator_8c646b42
FUNCTION.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
FUNCTION.restype = None
rng = random.Random(ADDRESS)
scenarios = [(0xFFFFFFFF, 0x80000000 >> bit) for bit in range(32)]
scenarios += [(0, 0x80000000 >> bit) for bit in range(32)]
scenarios += [(0xAAAAAAAA, 0), (0x55555555, 0xFFFFFFFF), (0xFFFFFFFF, 0x12345678)]
scenarios += [(rng.getrandbits(32), 0x80000000 >> rng.randrange(32)) for _ in range(157)]
cases = []
for case, (flags, target_mask) in enumerate(scenarios):
    initial = bytearray(b'\xA5') * RAM_SIZE
    initial[BASE - 0x8C000000:BASE - 0x8C000000 + len(BINARY)] = BINARY
    flags_address = 0x8C720000 + case * 0x100
    struct.pack_into('<I', initial, flags_address & 0xFFFFFF, flags)
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = flags_address
    state.r[5] = target_mask
    state.r[15] = 0x8C800000 + case * 0x100 + 0x40
    state.sr = rng.getrandbits(32)
    state.pr = 0x8CFED000 + case * 4
    state.pc = ADDRESS
    alias = case % 5
    if alias == 1:
        # The prologue saves R12 over the flags word before it is read.
        state.r[15] = flags_address + 4
    elif alias == 2:
        # The saved R12 value replaces the mask literal before it is loaded.
        state.r[15] = MASK_LITERAL + 4
    elif alias == 3:
        # The saved value aliases the first compact metadata slot.
        state.r[15] = flags_address + 8
    elif alias == 4:
        state.r[15] = flags_address + 4
        state.r[12] = rng.choice([0, 0x80000000, 0x40000000, 0xFFFFFFFF])

    retail_ram, native_ram = bytearray(initial), bytearray(initial)
    expected, actual = State.from_buffer_copy(bytes(state)), State.from_buffer_copy(bytes(state))
    steps = reference(retail_ram, expected)
    native_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
    FUNCTION(native_view, ctypes.byref(actual))
    assert retail_ram == native_ram, f'RAM mismatch case={case} alias={alias}'
    if bytes(expected) != bytes(actual):
        diffs = {f'r{i}': (hex(expected.r[i]), hex(actual.r[i])) for i in range(16) if expected.r[i] != actual.r[i]}
        diffs.update({name: (hex(getattr(expected, name)), hex(getattr(actual, name))) for name in ('sr', 'pr', 'pc') if getattr(expected, name) != getattr(actual, name)})
        raise AssertionError(f'CPU mismatch case={case} alias={alias}, flags={flags:08x}, mask={target_mask:08x}, savedR12={state.r[12]:08x}: {diffs}')
    cases.append({'case': case, 'flags': hex(flags), 'target_mask': hex(target_mask),
                  'alias_fixture': alias, 'retail_instructions': steps,
                  'full_ram_and_cpu_match': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(SPAN).hexdigest(),
    'binary_sha256': hashlib.sha256(BINARY).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source/sa_input_metadata_bit_locator_8c646b42.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'Complete 16 MiB RAM and all GPR/SR/PR/PC state against retail SH-4 execution.',
    'scope': 'Scans the 32-bit metadata flag word from the high bit down and returns the corresponding compact-entry pointer for a selected flag, or null when absent.',
    'limitations': ['Metadata decoding callers are separate routines.', 'Verification uses the project SH-4 interpreter, not Dreamcast hardware or complete-game execution.'],
    'cases': cases,
}
(PROJECT / 'analysis/sa_input_metadata_bit_locator_8c646b42_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{len(cases)} input metadata bit locator RAM/CPU comparisons passed')