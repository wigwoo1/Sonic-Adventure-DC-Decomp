import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C099A1C
GLOBAL_CURSOR_SLOT = 0x8C7A0A08
LITERAL = 0x8C099A4C

binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
assert binary[FUNCTION - BASE:FUNCTION - BASE + 38] == bytes.fromhex(
    '0b d6 62 65 53 53 38 23 01 89 0a a0 53 54 02 a0 '
    '52 64 42 53 32 64 42 55 58 25 02 89 53 53 40 33 '
    'f7 89 0b 00 42 26')
assert struct.unpack_from('<I', binary, LITERAL - BASE)[0] == GLOBAL_CURSOR_SLOT
binary_sha256 = hashlib.sha256(binary).hexdigest()

library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'task_cursor_advance_8c099a1c.dll'
native = ctypes.CDLL(str(library_path)).sa_task_cursor_advance_8c099a1c
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def write_ram32(ram, address, value):
    offset = (address & 0x1FFFFFFF) - 0x0C000000
    struct.pack_into('<I', ram, offset, value)

rng = random.Random(FUNCTION)
cases = 0
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    root = 0x8C700000 + (case // 4) * 0x1000
    first = root + 0x100
    parent1 = root + 0x200
    second = root + 0x300
    parent2 = root + 0x400
    third = root + 0x500
    parent3 = root + 0x600

    write_ram32(ram, GLOBAL_CURSOR_SLOT, root)
    write_ram32(ram, root, first)
    if case % 4 == 0:
        # Current task already has a child: take that child directly.
        write_ram32(ram, root + 0x0C, third)
    else:
        write_ram32(ram, root + 0x0C, 0)
        write_ram32(ram, first + 8, root if case % 4 == 1 else parent1)
        if case % 4 == 1:
            # One comparison fails and leaves the first node as the cursor.
            write_ram32(ram, root + 0x0C, 0)
        else:
            write_ram32(ram, parent1, second)
            write_ram32(ram, parent1 + 0x0C, first)
            write_ram32(ram, second + 8, parent2)
            write_ram32(ram, parent2 + 0x0C, second)
            write_ram32(ram, parent2, third)
            if case % 4 == 2:
                # A matching child link loops once, then a null parent ends it.
                write_ram32(ram, third + 8, 0)
            else:
                # A matching child link loops once, then a mismatch ends it.
                write_ram32(ram, third + 8, parent3)
                write_ram32(ram, parent3 + 0x0C, 0)

    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C, 0x8C099A4C])
    state.pc = FUNCTION
    expected_ram = bytearray(ram)
    expected = State.from_buffer_copy(state)
    reference(expected_ram, expected)
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual))
    if actual_ram != expected_ram:
        changed = [index for index, (actual_byte, expected_byte)
                   in enumerate(zip(actual_ram, expected_ram))
                   if actual_byte != expected_byte]
        slot_offset = GLOBAL_CURSOR_SLOT - 0x8C000000
        actual_cursor = struct.unpack_from('<I', actual_ram, slot_offset)[0]
        expected_cursor = struct.unpack_from('<I', expected_ram, slot_offset)[0]
        raise AssertionError(
            f'RAM mismatch in case {case}; changed offsets={changed[:16]}; '
            f'cursor={actual_cursor:#x}/{expected_cursor:#x}')
    if bytes(actual) != bytes(expected):
        differences = [(index, hex(actual.r[index]), hex(expected.r[index]))
                       for index in range(16) if actual.r[index] != expected.r[index]]
        raise AssertionError(
            f'CPU mismatch in case {case}; GPRs={differences}; '
            f'SR={actual.sr:#x}/{expected.sr:#x}; '
            f'PR={actual.pr:#x}/{expected.pr:#x}; PC={actual.pc:#x}/{expected.pc:#x}')
    cases += 1

span = binary[FUNCTION - BASE:FUNCTION - BASE + 38]
report = {
    'cases_passed': cases,
    'comparison_level': 'Complete 16 MiB RAM and all GPRs, SR, PR, and return PC compared against the retail SH-4 interpreter.',
    'scope': 'The 38-byte task cursor routine at 0x8c099a1c loads and advances the current task-list cursor through child/sibling-parent links, stores the selected node to 0x8c7a0a08, and returns through PR. Tests cover direct-child selection, the no-child walk, loop continuation, null-parent exit, and mismatch exit.',
    'limitations': ['Uses the project SH-4 interpreter rather than Dreamcast hardware or end-to-end gameplay.', 'Tests exercise representative valid task-link graphs; all higher-level task lifecycle behavior remains outside this routine.'],
    'binary_sha256': binary_sha256,
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'literal_address': hex(LITERAL),
    'global_cursor_slot': hex(GLOBAL_CURSOR_SLOT),
    'source_sha256': hashlib.sha256((ROOT / 'source' / 'sa_task_cursor_advance_8c099a1c.c').read_bytes()).hexdigest(),
    'routines': [{
        'module': '1ST_READ.BIN',
        'load_address': hex(BASE),
        'address': hex(FUNCTION),
        'end_exclusive': hex(FUNCTION + len(span)),
        'retail_span_sha256': hashlib.sha256(span).hexdigest(),
        'cases_passed': cases,
        'name': 'sa_task_cursor_advance_8c099a1c',
    }],
}
(ROOT / 'analysis' / 'task_cursor_advance_8c099a1c_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{cases} task cursor full RAM/CPU comparisons passed')
