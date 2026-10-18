"""Differentially verify the input metadata four-byte word reader."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
BASE, ADDRESS, LENGTH = 0x8C010000, 0x8C646B72, 74
RAM_BASE, RAM_SIZE = 0x8C000000, 32 * 1024 * 1024
SOURCE = PROJECT / 'source/sa_input_metadata_read_word_8c646b72.c'
BINARY = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
SPAN = BINARY[ADDRESS - BASE:ADDRESS - BASE + LENGTH]
assert len(SPAN) == LENGTH
LIBRARY = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/sa_input_metadata_read_word_8c646b72.dll'))
FUNCTION = LIBRARY.sa_input_metadata_read_word_8c646b72
FUNCTION.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
FUNCTION.restype = None

rng = random.Random(ADDRESS)
scenarios = []
for bit in range(32):
    scenarios.append((1 << bit, bit))
scenarios.extend((rng.getrandbits(32) | 1, 32 + i) for i in range(160))
scenarios.extend((rng.getrandbits(32) & 0xFFFFFFFE, 192 + i) for i in range(64))
cases = []
for case, (flags, alias) in enumerate(scenarios):
    initial = bytearray(b'\xA5') * RAM_SIZE
    initial[BASE - RAM_BASE:BASE - RAM_BASE + len(BINARY)] = BINARY
    source_address = 0x8C720000 + case * 0x100
    destination = 0x8C730000 + case * 0x100
    struct.pack_into('<I', initial, source_address - RAM_BASE, flags)
    slot_index = (flags & 0xFFFFFFFE).bit_count()
    slot = source_address + 4 + slot_index * 4
    slot_bytes = bytes(rng.getrandbits(8) for _ in range(4))
    initial[slot - RAM_BASE:slot - RAM_BASE + 4] = slot_bytes

    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4], state.r[5] = destination, source_address
    state.r[15] = 0x8C800000 + case * 0x100 + 0x40
    state.sr = rng.getrandbits(32)
    state.pr = 0x8CFED000 + case * 4
    state.pc = ADDRESS
    if alias % 6 == 1:
        state.r[15] = source_address + 12  # Saved output pointer overwrites flags.
    elif alias % 6 == 2:
        state.r[15] = source_address + 8  # Saved input pointer overwrites flags.
    elif alias % 6 == 3:
        state.r[15] = slot + 12  # Saved output pointer overwrites selected data.
    elif alias % 6 == 4:
        state.r[15] = destination + 12  # Frame aliases the output word.
    elif alias % 6 == 5:
        state.r[15] = source_address + 16  # Frame aliases compact metadata.

    retail_ram, native_ram = bytearray(initial), bytearray(initial)
    expected, actual = State.from_buffer_copy(bytes(state)), State.from_buffer_copy(bytes(state))
    steps = reference(retail_ram, expected)
    native_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
    FUNCTION(native_view, ctypes.byref(actual))
    assert retail_ram == native_ram, f'RAM mismatch case={case} alias={alias}'
    if bytes(expected) != bytes(actual):
        diffs = {f'r{i}': (hex(expected.r[i]), hex(actual.r[i])) for i in range(16) if expected.r[i] != actual.r[i]}
        diffs.update({name: (hex(getattr(expected, name)), hex(getattr(actual, name)))
                      for name in ('sr', 'pr', 'pc') if getattr(expected, name) != getattr(actual, name)})
        raise AssertionError(f'CPU mismatch case={case} flags={flags:08x} alias={alias}: {diffs}')
    cases.append({'case': case, 'flags': hex(flags), 'alias_fixture': alias % 6,
                  'retail_instructions': steps, 'full_ram_and_cpu_match': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(SPAN).hexdigest(),
    'binary_sha256': hashlib.sha256(BINARY).hexdigest(),
    'source_sha256': hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'Complete 32 MiB RAM and all GPR/SR/PR/PC state against retail SH-4 execution, including the bit-locator helper.',
    'scope': 'Locates compact data for metadata flag bit zero, assembles four bytes in big-endian field order, writes the output word, and returns zero; returns 0xffffffff when bit zero is absent.',
    'limitations': ['The shared helper is verified separately as well as included in these wrapper comparisons.',
                    'Verification uses the project SH-4 interpreter, not Dreamcast hardware or complete-game execution.'],
    'cases': cases,
}
(PROJECT / 'analysis/sa_input_metadata_read_word_8c646b72_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{len(cases)} input metadata word reader RAM/CPU comparisons passed')
