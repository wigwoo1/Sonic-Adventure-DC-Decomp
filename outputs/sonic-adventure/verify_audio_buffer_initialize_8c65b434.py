import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C65B434
END = 0x8C65B44E
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:END - BASE]
assert len(span) == 26

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'audio_buffer_initialize_8c65b434.dll'))
native = library.sa_audio_buffer_initialize_8c65b434
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
native.restype = None

rng = random.Random(FUNCTION)
cases = []
for case in range(128):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    # Randomize the exact target memory to ensure the initializer clears it all.
    buffer_at = 0x8A3810
    ram[buffer_at:buffer_at + 0x100] = bytes(rng.getrandbits(8) for _ in range(0x100))
    for address in (0x8C67C1E8, 0x8C67C1EC):
        at = address - 0x8C000000
        ram[at:at + 4] = rng.getrandbits(32).to_bytes(4, 'little')
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[15] = 0x8C780FF0 - case * 0x10
    state.sr = rng.getrandbits(32)
    state.pr = rng.choice([0x8CFED000, 0x8C010000, 0x8C74913C])
    state.pc = FUNCTION

    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    reference(expected_ram, expected_state)
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual_state))
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual_state) == bytes(expected_state), (
        f'CPU mismatch in case {case}: actual={[hex(v) for v in actual_state.r]}, '
        f'expected={[hex(v) for v in expected_state.r]}, '
        f'SR={actual_state.sr:#x}/{expected_state.sr:#x}'
    )
    cases.append({'case': case, 'matched': True})

source_path = PROJECT / 'source' / 'sa_audio_buffer_initialize_8c65b434.c'
report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE),
    'address': hex(FUNCTION), 'end_exclusive': hex(END),
    'indexed_instruction_bytes': len(span),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source_path.read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state against the retail SH-4 interpreter',
    'scope': 'Clear the 0x100-byte sound buffer and two associated globals through the retail initialization sequence, including callback register state and saved-PR stack traffic.',
    'limitations': [
        'The initializer behavior of this one sound buffer does not establish the rest of the audio driver lifecycle.',
        'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'
    ],
    'cases': cases,
}
(PROJECT / 'analysis' / 'audio_buffer_initialize_8c65b434_verification.json').write_text(
    json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{len(cases)} audio-buffer initializer RAM/CPU comparisons passed')
