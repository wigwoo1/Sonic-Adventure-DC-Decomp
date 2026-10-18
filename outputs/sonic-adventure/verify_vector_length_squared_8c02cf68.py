import ctypes
import hashlib
import json
import random

from verify_fpu_cleanup_transfer import Fpu
from verify_startup import ROOT, State, reference


PROJECT = ROOT / 'outputs' / 'sonic-adventure'
FUNCTION = 0x8C02CF68
BASE = 0x8C010000
LENGTH = 8
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('8d f7 ed f5 0b 00 7c f0')

native = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' /
                         'vector_length_squared_8c02cf68.dll'))
service_type = ctypes.CFUNCTYPE(ctypes.c_int, ctypes.c_void_p, ctypes.c_uint32,
                                ctypes.POINTER(Fpu))
function = native.sa_vector_length_squared_8c02cf68
function.argtypes = [ctypes.POINTER(State), ctypes.POINTER(Fpu), ctypes.c_void_p,
                     service_type]
function.restype = ctypes.c_int

cases = 0
for active_bank in range(2):
    for sz in range(2):
        for seed in range(64):
            rng = random.Random((active_bank << 16) | (sz << 8) | seed)
            old = bytearray(RAM_SIZE)
            old[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
            actual = bytearray(old)
            actual_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual)

            expected_fpu = Fpu()
            expected_fpu.fpscr = (0x40000 | (active_bank << 21) | (sz << 20) |
                                  rng.randrange(4))
            expected_fpu.fpul = rng.getrandbits(32)
            for bank in range(2):
                expected_fpu.banks[bank][:] = [rng.getrandbits(32) for _ in range(16)]
            actual_fpu = Fpu.from_buffer_copy(expected_fpu)

            expected_state = State()
            expected_state.r[:] = [rng.getrandbits(32) for _ in range(16)]
            expected_state.sr = rng.getrandbits(32)
            expected_state.pr = rng.choice([0x8C000100, 0x8C7FF000, 0x8CFED000])
            expected_state.pc = FUNCTION
            actual_state = State.from_buffer_copy(expected_state)

            injected_result = rng.getrandbits(32)
            expected_events = []
            actual_events = []

            def model_fipr(word, fp):
                expected_events.append((word, bytes(fp)))
                assert word == 0xF5ED
                bank = (fp.fpscr >> 21) & 1
                fp.banks[bank][7] = injected_result
                fp.fpscr ^= 0x1000

            reference(old, expected_state, fpu=expected_fpu,
                     vector_instruction=model_fipr)

            @service_type
            def native_fipr(context, word, fp_pointer):
                fp = fp_pointer.contents
                actual_events.append((word, bytes(fp)))
                if word != 0xF5ED:
                    return -1
                bank = (fp.fpscr >> 21) & 1
                fp.banks[bank][7] = injected_result
                fp.fpscr ^= 0x1000
                return 0

            assert function(ctypes.byref(actual_state), ctypes.byref(actual_fpu),
                            None, native_fipr) == 0
            assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch bank={active_bank} SZ={sz} seed={seed}'
            assert bytes(actual_fpu) == bytes(expected_fpu), f'FPU mismatch bank={active_bank} SZ={sz} seed={seed}'
            assert actual == old, f'RAM changed bank={active_bank} SZ={sz} seed={seed}'
            assert actual_events == expected_events, f'FIPR boundary mismatch bank={active_bank} SZ={sz} seed={seed}'
            cases += 1

source = PROJECT / 'source' / 'sa_vector_length_squared_8c02cf68.c'
report = {
    'module': '1ST_READ.BIN',
    'load_address': hex(BASE),
    'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
    'cases_passed': cases,
    'comparison_level': 'complete CPU/FPU state, RAM, and FIPR service boundary',
    'scope': 'FPU bank selection, FLDI0 FR7, FIPR FV4,FV4 service dispatch, RTS delay-slot FMOV FR7,FR0, and return PC',
    'limitations': [
        'FIPR numerical arithmetic is delegated to the SH-4 vector instruction service; these tests inject its result.',
        'Enabled FPU exceptions, FPSCR.PR=1, and FPSCR.FD=1 execution are outside this wrapper test.',
    ],
    'cases': cases,
}
(PROJECT / 'analysis' / 'vector_length_squared_8c02cf68_verification.json').write_text(
    json.dumps(report, indent=2) + '\n')
print(f'{cases} vector-length-squared CPU/FPU/RAM comparisons passed')
