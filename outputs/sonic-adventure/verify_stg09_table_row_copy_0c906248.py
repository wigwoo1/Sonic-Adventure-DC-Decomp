"""Differentially verify STG09's indexed three-word table-row copy."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
LOAD, ADDRESS, LENGTH = 0x0C900000, 0x0C906248, 30
RAM_BASE, RAM_SIZE = 0x0C000000, 16 * 1024 * 1024
module = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
span = module[ADDRESS - LOAD:ADDRESS - LOAD + LENGTH]
assert len(span) == LENGTH

library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/stg09_table_row_copy_0c906248.dll'))
native = library.sa_stg09_table_row_copy_0c906248
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

def offset(address):
    value = (address & 0x1FFFFFFF) - RAM_BASE
    assert 0 <= value < RAM_SIZE - 4, hex(address)
    return value

records = []
rng = random.Random(0x0C906248)
for case in range(256):
    ram = bytearray(b'\xa5') * RAM_SIZE
    ram[LOAD - RAM_BASE:LOAD - RAM_BASE + len(module)] = module
    table = 0x0C800000 + case * 0x1000
    destination = 0x0C900000 + (case % 64) * 0x200
    selector = [0, 1, 2, 7, 31, 255, 0x7FFFFFFF, 0xFFFFFFFF][case % 8]
    # Keep pointers in RAM for the valid-row cases; separately test a broad
    # set of indices with a large fixture table to exercise scaled addressing.
    selector = case % 64 if case < 64 else rng.randrange(64)
    struct.pack_into('<I', ram, offset(0x0C906320), table)
    for index in range(64):
        for word in range(3):
            struct.pack_into('<I', ram, offset(table + index * 12 + word * 4),
                             rng.getrandbits(32))
    if case % 8 == 0:
        # Valid alias cases ensure store order is preserved when output overlaps
        # the selected row or its neighboring rows.
        destination = table + selector * 12 - 8
    initial = State()
    initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
    initial.r[4], initial.r[5] = destination, selector
    initial.r[15] = 0x0CF00000 + case * 0x100
    initial.sr = rng.getrandbits(32)
    initial.pr = 0x0CFF0000 + case * 4
    initial.pc = ADDRESS
    expected_ram, actual_ram = bytearray(ram), bytearray(ram)
    expected = State.from_buffer_copy(initial)
    actual = State.from_buffer_copy(initial)
    steps = reference(expected_ram, expected, stop_pc=initial.pr, instruction_budget=64)
    actual_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    native(actual_view, ctypes.byref(actual))
    assert actual_ram == expected_ram, f'RAM mismatch case={case}, selector={selector}'
    if bytes(actual) != bytes(expected):
        changes = {f'r{i}': (hex(expected.r[i]), hex(actual.r[i]))
                   for i in range(16) if expected.r[i] != actual.r[i]}
        changes.update({name:(hex(getattr(expected,name)),hex(getattr(actual,name)))
                        for name in ('sr','pr','pc') if getattr(expected,name)!=getattr(actual,name)})
        row_values=struct.unpack_from('<III',expected_ram,offset(table+selector*12))
        raise AssertionError(f'CPU mismatch case={case}, selector={selector}: {changes}; row={tuple(hex(x) for x in row_values)}; table={hex(table)}; destination={hex(destination)}')
    records.append({'case': case, 'selector': selector, 'destination': hex(destination),
                    'retail_instructions': steps, 'full_ram_cpu_match': True})

source = PROJECT / 'source/sa_stg09_table_row_copy_0c906248.c'
report = {
    'module': 'STG09.BIN', 'load_address': hex(LOAD), 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': len(records),
    'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state.',
    'scope': '30-byte table-row helper; scaled selector addressing, three word copies, selector byte, return delay slot.',
    'limitations': ['Table base is read from the retail literal-pool pointer at 0x0c906320.',
                    'Only selectors whose rows are mapped in test RAM are exercised.'],
    'cases': records,
}
(PROJECT / 'analysis/common-overlays/STG09.BIN/table_row_copy_0c906248_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{len(records)} STG09 table-row full RAM/CPU comparisons passed')
