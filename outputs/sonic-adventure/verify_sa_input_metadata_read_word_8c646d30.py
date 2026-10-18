"""Differentially verify this metadata flag's four-byte word reader."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
BASE, ADDRESS, LENGTH = 0x8C010000, 0x8C646D30, 74
RAM_BASE, RAM_SIZE = 0x8C000000, 32 * 1024 * 1024
SOURCE = PROJECT / 'source/sa_input_metadata_read_word_8c646d30.c'
BINARY = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
SPAN = BINARY[ADDRESS - BASE:ADDRESS - BASE + LENGTH]
assert len(SPAN) == LENGTH
LIBRARY = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/sa_input_metadata_read_word_8c646d30.dll'))
FUNCTION = LIBRARY.sa_input_metadata_read_word_8c646d30
FUNCTION.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
FUNCTION.restype = None

rng = random.Random(ADDRESS)
scenarios = []
for bit in range(32):
    scenarios.append((0x20 | (1 << bit), bit))
scenarios.extend((rng.getrandbits(32) | 0x20, 32 + i) for i in range(160))
scenarios.extend((rng.getrandbits(32) & 0xFFFFFFDF, 192 + i) for i in range(64))
cases = []
for case, (flags, alias) in enumerate(scenarios):
    initial = bytearray(b'\xA5') * RAM_SIZE
    initial[BASE - RAM_BASE:BASE - RAM_BASE + len(BINARY)] = BINARY
    source_address = 0x8C720000 + case * 0x100
    destination = 0x8C730000 + case * 0x100
    struct.pack_into('<I', initial, source_address - RAM_BASE, flags)
    slot = source_address + 4 + ((flags >> 6).bit_count() * 4)
    initial[slot - RAM_BASE:slot - RAM_BASE + 4] = bytes(rng.getrandbits(8) for _ in range(4))

    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4], state.r[5] = destination, source_address
    state.r[15] = 0x8C800000 + case * 0x100 + 0x40
    state.sr = rng.getrandbits(32)
    state.pr = 0x8CFED000 + case * 4
    state.pc = ADDRESS
    alias_kind = alias % 6
    if alias_kind == 1:
        state.r[15] = source_address + 12  # Saved output pointer overwrites the flag word.
    elif alias_kind == 2:
        state.r[15] = source_address + 8  # Saved values and PR overlap the flag area.
    elif alias_kind == 3:
        state.r[15] = slot + 12  # The call frame overlaps the selected compact record.
    elif alias_kind == 4:
        state.r[15] = destination + 12  # Output overlaps the saved output-pointer slot.
    elif alias_kind == 5:
        state.r[4] = slot  # Output aliases all four metadata bytes; reads happen first.

    retail_ram, native_ram = bytearray(initial), bytearray(initial)
    expected, actual = State.from_buffer_copy(bytes(state)), State.from_buffer_copy(bytes(state))
    steps = reference(retail_ram, expected)
    native_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
    FUNCTION(native_view, ctypes.byref(actual))
    assert retail_ram == native_ram, f'RAM mismatch case={case} alias={alias_kind}'
    if bytes(expected) != bytes(actual):
        diffs = {f'r{i}': (hex(expected.r[i]), hex(actual.r[i])) for i in range(16) if expected.r[i] != actual.r[i]}
        diffs.update({name: (hex(getattr(expected, name)), hex(getattr(actual, name)))
                      for name in ('sr', 'pr', 'pc') if getattr(expected, name) != getattr(actual, name)})
        raise AssertionError(f'CPU mismatch case={case} flags={flags:08x} alias={alias_kind}: {diffs}')
    cases.append({'case': case, 'flags': hex(flags), 'alias_fixture': alias_kind,
                  'retail_instructions': steps, 'full_ram_and_cpu_match': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(SPAN).hexdigest(),
    'binary_sha256': hashlib.sha256(BINARY).hexdigest(),
    'source_sha256': hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'Complete 32 MiB RAM and all GPR/SR/PR/PC state against retail SH-4 execution, including the bit-locator helper.',
    'scope': 'Finds compact metadata flag bit five, assembles its four record bytes into a big-endian 32-bit value, writes that word to the caller output, and returns 0xffffffff if absent.',
    'limitations': ['The shared bit-locator helper is separately registered and verified.',
                    'Verification uses the project SH-4 interpreter, not Dreamcast hardware or complete-game execution.'],
    'cases': cases,
}
(PROJECT / 'analysis/sa_input_metadata_read_word_8c646d30_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{len(cases)} metadata word reader RAM/CPU comparisons passed')
