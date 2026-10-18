import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
SETTER = 0x8C099D38
GETTER = 0x8C099D44
ADDRESS_GETTER = 0x8C099D2A
AUX_SETTER = 0x8C099D80
LITERAL = 0x8C099D64
AUX_LITERAL = 0x8C099D8C
GLOBAL_POINTER_SLOT = 0x8C7A0A08

binary_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN'
binary = binary_path.read_bytes()
assert binary[SETTER - BASE:SETTER - BASE + 12] == bytes.fromhex('48 24 01 8b 09 d2 22 64 0b 00 58 14')
assert binary[GETTER - BASE:GETTER - BASE + 12] == bytes.fromhex('48 24 01 8b 06 d2 22 64 0b 00 48 50')
assert binary[ADDRESS_GETTER - BASE:ADDRESS_GETTER - BASE + 14] == bytes.fromhex('48 24 01 8b 0d d2 22 64 43 60 0b 00 24 70')
assert binary[AUX_SETTER - BASE:AUX_SETTER - BASE + 12] == bytes.fromhex('48 24 01 8b 01 d2 22 64 0b 00 56 14')
assert struct.unpack_from('<I', binary, LITERAL - BASE)[0] == GLOBAL_POINTER_SLOT
assert struct.unpack_from('<I', binary, AUX_LITERAL - BASE)[0] == GLOBAL_POINTER_SLOT
binary_sha256 = hashlib.sha256(binary).hexdigest()

library_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'build' / 'task_work_field_8c099d38.dll'
library = ctypes.CDLL(str(library_path))
setter = library.sa_task_work_set_8c099d38
getter = library.sa_task_work_get_8c099d44
address_getter = library.sa_task_work_address_get_8c099d2a
aux_setter = library.sa_task_work_aux_set_8c099d80
for native in (setter, getter, address_getter, aux_setter):
    native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    native.restype = None

rng = random.Random(0x8C099D38)
object_bases = [
    0x8C700000,
    0x8C75A120,
    0x8C099D44 - 0x20,
    LITERAL - 0x20,
    GLOBAL_POINTER_SLOT - 0x20,
    0x8CFFFFDC,
]
reports = []
for function, native, operation in (
    (ADDRESS_GETTER, address_getter, 'address_get'),
    (SETTER, setter, 'set'),
    (GETTER, getter, 'get'),
    (AUX_SETTER, aux_setter, 'set_aux'),
):
    cases = 0
    for case in range(128):
        ram = bytearray(RAM_SIZE)
        ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
        object_base = (object_bases[case % len(object_bases)] if case < 48
                       else 0x8C700000 + rng.randrange(0x8FFFD9))
        object_base &= 0xFFFFFFFC
        if case % 2 == 0:
            struct.pack_into('<I', ram, GLOBAL_POINTER_SLOT - 0x8C000000, object_base)
            argument = 0
        else:
            argument = object_base
            struct.pack_into('<I', ram, GLOBAL_POINTER_SLOT - 0x8C000000,
                             rng.choice(object_bases) & 0xFFFFFFFC)
        if operation in ('set', 'get', 'set_aux'):
            field_offset = 0x18 if operation == 'set_aux' else 0x20
            field_address = (object_base + field_offset) & 0xFFFFFFFF
            code_length = 14 if function == ADDRESS_GETTER else 12
            overlaps_current_code = function <= field_address < function + code_length
            if operation == 'get' and field_address != GLOBAL_POINTER_SLOT and not overlaps_current_code:
                struct.pack_into('<I', ram, field_address - 0x8C000000, rng.getrandbits(32))

        state = State()
        state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        state.r[4] = argument
        state.r[5] = rng.getrandbits(32)
        state.sr = rng.getrandbits(32)
        state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C, 0x8C099D64])
        state.pc = function

        expected_ram = bytearray(ram)
        expected = State.from_buffer_copy(state)
        reference(expected_ram, expected)
        actual_ram = bytearray(ram)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        actual = State.from_buffer_copy(state)
        native(ram_view, ctypes.byref(actual))
        assert actual_ram == expected_ram, f'RAM mismatch at {function:#x}, case {case}'
        differences = [(index, hex(actual.r[index]), hex(expected.r[index]))
                       for index in range(16) if actual.r[index] != expected.r[index]]
        assert bytes(actual) == bytes(expected), (
            f'CPU mismatch at {function:#x}, case {case}; GPRs={differences}; '
            f'SR={actual.sr:#x}/{expected.sr:#x}; '
            f'PR={actual.pr:#x}/{expected.pr:#x}; PC={actual.pc:#x}/{expected.pc:#x}')
        cases += 1

    length = 14 if function == ADDRESS_GETTER else 12
    span = binary[function - BASE:function - BASE + length]
    reports.append({
        'module': '1ST_READ.BIN',
        'load_address': hex(BASE),
        'address': hex(function),
        'end_exclusive': hex(function + length),
        'retail_span_sha256': hashlib.sha256(span).hexdigest(),
        'cases_passed': cases,
        'name': {'address_get': 'sa_task_work_address_get_8c099d2a',
                 'set': 'sa_task_work_set_8c099d38',
                 'get': 'sa_task_work_get_8c099d44',
                 'set_aux': 'sa_task_work_aux_set_8c099d80'}[operation],
    })

report = {
    'functions_verified': len(reports),
    'total_full_state_cases': sum(item['cases_passed'] for item in reports),
    'comparison_level': 'Complete 16 MiB RAM and all GPRs, SR, PR, and return PC compared against the retail SH-4 interpreter.',
    'scope': 'The accessors at 0x8c099d2a, 0x8c099d38, 0x8c099d44, and 0x8c099d80 resolve a null object argument through global pointer slot 0x8c7a0a08 and update SR.T from the null test. The address getter returns object+0x24; setters at 0x8c099d38 and 0x8c099d80 write offsets +0x20 and +0x18, respectively, while the getter reads +0x20. Tests cover null/non-null objects, aliased code/literal/global storage, random object RAM, and the final aligned RAM word.',
    'limitations': ['Uses the project SH-4 interpreter rather than Dreamcast hardware or end-to-end gameplay.', 'The higher-level ownership and specific work-field type remain unidentified.'],
    'binary_sha256': binary_sha256,
    'literal_address': hex(LITERAL),
    'global_pointer_slot': hex(GLOBAL_POINTER_SLOT),
    'routines': reports,
}
(ROOT / 'analysis' / 'task_work_field_8c099d38_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f"{report['total_full_state_cases']} task work access full RAM/CPU comparisons passed")
