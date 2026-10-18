"""Compare the object callback tail dispatcher with retail SH-4 execution."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ADDRESS, LENGTH = 0x8c0663ca, 12
BASE, RAM_SIZE = 0x8c010000, 16 * 1024 * 1024


def put32(ram, address, value):
    struct.pack_into('<I', ram, address & 0xffffff, value & 0xffffffff)


def main():
    project = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    original = bytearray(b'\xa5') * RAM_SIZE
    original[BASE & 0xffffff:(BASE & 0xffffff) + len(binary)] = binary
    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/sa_object_callback_tail_dispatch_8c0663ca.dll'))
    function = library.sa_object_callback_tail_dispatch_8c0663ca
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None
    rng = random.Random(0x8c0663ca)
    cases = []

    for case in range(256):
        obj = 0x8c720000 + case * 0x40
        target = 0x8c00e000 + case * 4
        return_pc = 0x8c00f000 + case * 4
        initial = bytearray(original)
        put32(initial, obj + 0x14, target)
        state = State()
        state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        state.r[4] = obj
        alias = case % 16
        state.r[15] = 0x8c800000 + case * 0x100 + 0x40
        if alias == 0:
            state.r[15] = obj + 0x18  # stack slot aliases the callback field
        elif alias == 1:
            state.r[15] = target + 4  # stack slot aliases callback memory
        state.sr = rng.getrandbits(32)
        state.pr = return_pc
        state.pc = ADDRESS

        retail_ram, native_ram = bytearray(initial), bytearray(initial)
        expected = State.from_buffer_copy(bytes(state))
        actual = State.from_buffer_copy(bytes(state))
        observed = []

        def external_callback(_ram, regs, sr):
            observed.append((list(regs), sr, expected.pr))
            return sr

        steps = reference(retail_ram, expected, hooks={target: external_callback})
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
        function(ram_view, ctypes.byref(actual))
        assert steps == 6, (case, steps)
        assert len(observed) == 1, (case, observed)
        expected_regs, expected_sr, expected_pr = observed[0]
        assert native_ram == retail_ram, f'RAM mismatch case {case}, alias={alias}'
        assert list(actual.r) == expected_regs, (
            f'register mismatch case {case}: '
            f'native={[hex(v) for v in actual.r]} retail={[hex(v) for v in expected_regs]}')
        assert (actual.sr, actual.pr, actual.pc) == (expected_sr, expected_pr, target), (
            f'control-state mismatch case {case}')
        cases.append({'case': case, 'target': hex(target), 'alias_fixture': alias,
                      'retail_steps': steps, 'full_ram_and_tail_state_match': True})

    source = project / 'source/sa_object_callback_tail_dispatch_8c0663ca.c'
    report = {
        'module': '1ST_READ.BIN', 'address': hex(ADDRESS),
        'end_exclusive': hex(ADDRESS + LENGTH),
        'name': 'sa_object_callback_tail_dispatch_8c0663ca',
        'retail_span_sha256': hashlib.sha256(binary[ADDRESS - BASE:ADDRESS - BASE + LENGTH]).hexdigest(),
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cases_passed': len(cases),
        'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state at the indirect callback boundary.',
        'scope': '256 randomized register fixtures with stack writes aliasing the object callback field and target memory.',
        'limitations': ['The selected object callback target remains a separate routine.'],
        'cases': cases,
    }
    (project / 'analysis/sa_object_callback_tail_dispatch_8c0663ca_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"Object callback dispatcher passed {len(cases)} complete RAM/CPU comparisons")


if __name__ == '__main__':
    main()
