import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
THUNKS = [
    (0x8C65BCB4, 0x8C65BD1C, 'sa_sound_work_callback_thunk_8c65bcb4'),
    (0x8C65BCC0, 0x8C65BD20, 'sa_sound_work_callback_thunk_8c65bcc0'),
    (0x8C65BCCC, 0x8C65BD24, 'sa_sound_work_callback_thunk_8c65bccc'),
    (0x8C65BCD8, 0x8C65BD28, 'sa_sound_work_callback_thunk_8c65bcd8'),
]
LENGTH = 12  # Actual executed bytes; Ghidra's 16-byte size overlaps the next entry by four bytes.
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'sound_work_callback_thunks.dll'))

functions = {}
for address, slot, name in THUNKS:
    native = getattr(library, name)
    native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    native.restype = None
    functions[address] = native
    assert binary[address - BASE:address - BASE + LENGTH] == bytes.fromhex(
        'fc 7f 42 2f') + binary[address - BASE + 4:address - BASE + 8] + bytes.fromhex('2b 43 04 7f')

rng = random.Random(0x8C65BCB4)
cases = []
for address, slot, name in THUNKS:
    for case in range(128):
        ram = bytearray(RAM_SIZE)
        ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
        callback = 0x8C700000 + rng.randrange(0x10000 // 2) * 2
        struct.pack_into('<I', ram, slot - 0x8C000000, callback)
        obj = 0x8C780000 + case * 0x100
        argument = rng.getrandbits(32)
        struct.pack_into('<I', ram, obj - 0x8C000000 + 4, argument)
        state = State()
        state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        state.r[4] = obj
        state.r[15] = 0x8C7FF000 - case * 8
        state.sr = rng.getrandbits(32)
        state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
        state.pc = address

        expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
        reference(expected_ram, expected_state, stop_pc=callback)
        actual_ram = bytearray(ram)
        view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        actual_state = State.from_buffer_copy(state)
        functions[address](view, ctypes.byref(actual_state))
        assert actual_ram == expected_ram, f'{name} RAM mismatch in case {case}'
        assert bytes(actual_state) == bytes(expected_state), f'{name} CPU mismatch in case {case}'
        cases.append({'routine': hex(address), 'case': case, 'callback': hex(callback), 'matched': True})

source_path = PROJECT / 'source' / 'sa_sound_work_callback_thunks.c'
reports = []
for address, slot, name in THUNKS:
    body = binary[address - BASE:address - BASE + LENGTH]
    reports.append({
        'name': name, 'address': hex(address), 'end_exclusive': hex(address + LENGTH),
        'literal_slot': hex(slot), 'indexed_code_bytes_verified': LENGTH,
        'retail_body_sha256': hashlib.sha256(body).hexdigest(),
    })
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE),
    'cases_passed': len(cases), 'routines': reports,
    'source_sha256': hashlib.sha256(source_path.read_bytes()).hexdigest(),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against the retail SH-4 interpreter at the callback entry',
    'scope': 'Load the per-thunk callback pointer, pass the object field at +4 in R4, preserve PR, and tail-jump while restoring R15 in the delay slot.',
    'limitations': [
        'The callback bodies are outside these 12-byte wrappers. The Ghidra draft index reports 16 bytes per entry because its guessed ranges overlap the following function; instruction traces confirm the executed wrapper ends after the 12th byte.',
        'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'
    ],
    'cases': cases,
}
(PROJECT / 'analysis' / 'sound_work_callback_thunks_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{len(cases)} sound-work tail-call RAM/CPU comparisons passed')

