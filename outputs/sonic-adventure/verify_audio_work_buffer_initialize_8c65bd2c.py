import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C65BD2C
END = 0x8C65BD3E
RAM_SIZE = 16 * 1024 * 1024
BUFFER = 0x8C8A3D90
LENGTH = 0xC80
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:END - BASE]
assert len(span) == 18
assert int.from_bytes(binary[0x8C65BE48 - BASE:0x8C65BE4C - BASE], 'little') == 0x8C10D31C
assert int.from_bytes(binary[0x8C65BE3E - BASE:0x8C65BE40 - BASE], 'little') == LENGTH
assert int.from_bytes(binary[0x8C65BE44 - BASE:0x8C65BE48 - BASE], 'little') == BUFFER

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'audio_work_buffer_initialize_8c65bd2c.dll'))
native = library.sa_audio_work_buffer_initialize_8c65bd2c
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(FUNCTION)
cases = []
for case in range(96):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    buffer_at = BUFFER - 0x8C000000
    ram[buffer_at:buffer_at + LENGTH] = bytes(rng.getrandbits(8) for _ in range(LENGTH))
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[15] = 0x8C780FF0 - case * 0x10
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION

    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    reference(expected_ram, expected_state)
    actual_ram = bytearray(ram)
    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(view, ctypes.byref(actual_state))
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual_state) == bytes(expected_state), (
        f'CPU mismatch in case {case}: actual={[hex(v) for v in actual_state.r]}, '
        f'expected={[hex(v) for v in expected_state.r]}, '
        f'SR={actual_state.sr:#x}/{expected_state.sr:#x}'
    )
    cases.append({'case': case, 'matched': True})

source_path = PROJECT / 'source' / 'sa_audio_work_buffer_initialize_8c65bd2c.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE),
    'address': hex(FUNCTION), 'end_exclusive': hex(END),
    'indexed_instruction_bytes': len(span),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source_path.read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against the retail SH-4 interpreter',
    'scope': 'Clear the 0xc80-byte audio work buffer through the retail fill routine and return one, including callback register and stack effects.',
    'limitations': [
        'This buffer initializer does not establish the remaining sound-driver state lifecycle.',
        'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'
    ],
    'cases': cases,
}
(PROJECT / 'analysis' / 'audio_work_buffer_initialize_8c65bd2c_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{len(cases)} audio work-buffer initializer RAM/CPU comparisons passed')
