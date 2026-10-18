"""Compare the signed-byte stack/dispatch wrapper with retail SH-4 execution."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

ADDRESS, LENGTH = 0x8c669450, 12
BASE, RAM_SIZE = 0x8c010000, 16 * 1024 * 1024
TARGET_LITERAL = 0x8c66945c


def main():
    project = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    original = bytearray(b'\xa5') * RAM_SIZE
    original[BASE & 0xffffff:(BASE & 0xffffff) + len(binary)] = binary
    target = int.from_bytes(original[TARGET_LITERAL & 0xffffff:(TARGET_LITERAL & 0xffffff) + 4], 'little')
    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/sa_sign_extend_register_byte_dispatch_8c669450.dll'))
    function = library.sa_sign_extend_register_byte_dispatch_8c669450
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None
    rng = random.Random(0x8c669450)
    cases = []

    for case in range(256):
        return_pc = 0x8c00f000 + case * 4
        initial = bytearray(original)
        state = State()
        state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        state.r[6] = ((rng.getrandbits(24) << 8) | case) & 0xffffffff
        state.r[15] = 0x8c800000 + case * 0x100 + 0x40
        alias_literal = case % 16 == 0
        if alias_literal:
            state.r[15] = TARGET_LITERAL + 4
        state.sr = rng.getrandbits(32)
        state.pr = return_pc
        state.pc = ADDRESS
        retail_ram, native_ram = bytearray(initial), bytearray(initial)
        expected = State.from_buffer_copy(bytes(state))
        actual = State.from_buffer_copy(bytes(state))
        observed = []

        def external_target(_ram, regs, sr):
            observed.append((list(regs), sr, expected.pr))
            return sr

        steps = reference(retail_ram, expected, hooks={target: external_target})
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
        function(ram_view, ctypes.byref(actual))
        assert steps == 6, (case, steps)
        assert len(observed) == 1, (case, observed)
        expected_regs, expected_sr, expected_pr = observed[0]
        assert native_ram == retail_ram, f'RAM mismatch case {case}'
        assert list(actual.r) == expected_regs, (
            f'register mismatch case {case}: '
            f'native={[hex(v) for v in actual.r]} retail={[hex(v) for v in expected_regs]}')
        assert (actual.sr, actual.pr, actual.pc) == (expected_sr, expected_pr, target), (
            f'control state mismatch case {case}')
        cases.append({'case': case, 'r6_low_byte': hex(case),
                      'stack_aliases_target_literal': alias_literal,
                      'retail_steps': steps, 'full_ram_and_tail_state_match': True})

    source = project / 'source/sa_sign_extend_register_byte_dispatch_8c669450.c'
    report = {
        'module': '1ST_READ.BIN', 'address': hex(ADDRESS),
        'end_exclusive': hex(ADDRESS + LENGTH),
        'name': 'sa_sign_extend_register_byte_dispatch_8c669450',
        'retail_span_sha256': hashlib.sha256(binary[ADDRESS - BASE:ADDRESS - BASE + LENGTH]).hexdigest(),
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cases_passed': len(cases),
        'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state at the external dispatch boundary.',
        'scope': 'All 256 R6 low-byte values with randomized upper register and CPU state; stack/target-literal alias tested 16 times.',
        'limitations': ['The dispatch target at 0x8c66b1a0 remains a separate routine.'],
        'cases': cases,
    }
    (project / 'analysis/sa_sign_extend_register_byte_dispatch_8c669450_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"Signed-byte dispatcher passed {len(cases)} complete RAM/CPU comparisons")


if __name__ == '__main__':
    main()
