"""Verify composed input-to-bitset setter wrappers."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

BASE, RAM_SIZE = 0x8c010000, 16 * 1024 * 1024
STACK_TOP = 0x8cfff000
STACK_OFFSET = (STACK_TOP & 0x1fffffff) - 0x0c000000
MODE_POINTER_LITERAL, MODE_STORAGE, CALLBACK_LITERAL = 0x8c0a69e0, 0x8c760970, 0x8c0a69e4
CALLBACK_TARGET = 0x8c0115d0
SPECS = [(0x8c0a6a88, 22, 'index_set'), (0x8c0a6ad2, 30, 'mapping_set')]
DISPATCH = ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_uint8),
                            ctypes.POINTER(State), ctypes.c_uint32, ctypes.c_void_p)


class Scenario(ctypes.Structure):
    _fields_ = [('case', ctypes.c_uint32), ('calls', ctypes.c_uint32)]


def callback_effect(target, memory, regs, case):
    marker = (target ^ (case * 0x10203) ^ 0x6b17c2a5) & 0xffffffff
    struct.pack_into('<I', memory, 0xb80000 + case * 4, marker)
    for register in range(8):
        regs[register] = (marker + register * 0x01010101) & 0xffffffff
    return marker


def cases_for(kind, rng):
    cases = []
    if kind == 'index_set':
        values = [0, 1, 2, 3, 4, 5, 6, 7, 8, 14, 15, 16, 0x20, 0x21,
                  0x7f, 0x80, 0x100, 0x1ff, 0x3ff, 0x1000]
        for value in values:
            cases.extend([(value, 0, 0, 0), (value, 5, 0, 0)])
        while len(cases) < 64:
            cases.append((rng.randrange(0x1000), 5 if len(cases) % 2 else 0, 0, 0))
    else:
        for row in range(16):
            for code in (1, 4, 8, 14):
                cases.append((row, code, ((row + code) & 3) - 1, 0))
        for code in (0, 15, 34, 35, 36, 38, 39, 41, 42, 43, 100):
            cases.extend([(0, code, 0, 5), (15, code, 2, 5)])
        while len(cases) < 112:
            cases.append((rng.randrange(16), rng.choice((0, 1, 7, 14, 15, 34, 35, 38, 39, 42, 100)),
                          rng.randrange(-2, 4), 5 if len(cases) % 2 else 0))
    return cases


def main():
    out = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    assert struct.unpack_from('<I', binary, MODE_POINTER_LITERAL - BASE)[0] == MODE_STORAGE
    assert struct.unpack_from('<I', binary, CALLBACK_LITERAL - BASE)[0] == CALLBACK_TARGET
    lib = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/input_mapping_set.dll'))
    rng = random.Random(0x494E505554534554)
    routines = []

    symbols = {'index_set': 'index_bitset_set', 'mapping_set': 'mapping_bitset_set'}
    for address, span, kind in SPECS:
        function = getattr(lib, f"sa_input_{symbols[kind]}_8c0a{address & 0xffff:04x}")
        function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                             DISPATCH, ctypes.c_void_p]
        function.restype = None
        case_specs = cases_for(kind, rng)
        cases = []

        for case, (first, second, third, fourth) in enumerate(case_specs):
            mode = second if kind == 'index_set' else fourth
            original = bytearray(b'\xa5') * RAM_SIZE
            original[0x10000:0x10000 + len(binary)] = binary
            struct.pack_into('<I', original, MODE_STORAGE & 0xffffff, mode)
            if case % 2 == 0:
                original[0x100200:0x100300] = bytes(0x100)
            else:
                original[0x100200:0x100300] = bytes(rng.getrandbits(8) for _ in range(0x100))
            expected = State()
            expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
            expected.r[4] = 0x8c100000
            if kind == 'index_set':
                expected.r[5] = first
            else:
                expected.r[5], expected.r[6], expected.r[7] = first, second, third & 0xffffffff
            expected.r[15] = STACK_TOP
            expected.sr = rng.getrandbits(32)
            expected.pr = 0x8c002800 + case * 4
            expected.pc = address
            actual = State.from_buffer_copy(bytes(expected))
            retail_ram, actual_ram = bytearray(original), bytearray(original)
            expected_trace = []

            def hook(memory, regs, sr):
                marker = callback_effect(CALLBACK_TARGET, memory, regs, case)
                expected_trace.append((CALLBACK_TARGET, marker))
                return sr

            steps = reference(retail_ram, expected,
                              hooks={CALLBACK_TARGET: hook} if mode == 5 else None)
            scenario = Scenario(case, 0)
            actual_trace = []

            @DISPATCH
            def dispatch(memory_pointer, state_pointer, target, context):
                settings = ctypes.cast(context, ctypes.POINTER(Scenario)).contents
                assert target == CALLBACK_TARGET
                memory = (ctypes.c_uint8 * RAM_SIZE).from_address(
                    ctypes.addressof(memory_pointer.contents))
                state = state_pointer.contents
                marker = (target ^ (settings.case * 0x10203) ^ 0x6b17c2a5) & 0xffffffff
                ctypes.memmove(ctypes.addressof(memory_pointer.contents) + 0xb80000 +
                               settings.case * 4, struct.pack('<I', marker), 4)
                for register in range(8):
                    state.r[register] = (marker + register * 0x01010101) & 0xffffffff
                settings.calls += 1
                actual_trace.append((target, marker))

            memory_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
            function(memory_view, ctypes.byref(actual), dispatch, ctypes.byref(scenario))
            stack_low = STACK_OFFSET - 0x100
            assert actual_ram[:stack_low] == retail_ram[:stack_low], f'RAM mismatch below stack {kind}:{case}'
            assert actual_ram[STACK_OFFSET:] == retail_ram[STACK_OFFSET:], f'RAM mismatch above stack {kind}:{case}'
            assert list(actual.r[8:]) == list(expected.r[8:]), f'callee-saved mismatch {kind}:{case}'
            assert actual.pr == expected.pr and actual.pc == expected.pc, f'control mismatch {kind}:{case}'
            assert actual_trace == expected_trace, f'callback trace mismatch {kind}:{case}'
            assert scenario.calls == (1 if mode == 5 else 0), f'callback count mismatch {kind}:{case}'
            cases.append({'case': case, 'mode': mode, 'retail_instructions': steps,
                          'callback_count': scenario.calls,
                          'abi_state_and_ram_match_outside_stack': True})

        off = address - BASE
        routines.append({'address': hex(address), 'end_exclusive': hex(address + span),
                         'kind': kind, 'retail_span_sha256': hashlib.sha256(binary[off:off + span]).hexdigest(),
                         'cases_passed': len(cases), 'cases': cases})

    source = out / 'source/input_mapping_set.c'
    report = {
        'functions_verified': len(routines),
        'total_cases': sum(item['cases_passed'] for item in routines),
        'comparison_level': 'Preserved R8-R15, PR/PC, all RAM outside a 256-byte volatile stack region, bitset writes and callback target/count. Caller-clobbered registers and SR are excluded except where overwritten by the deterministic callback boundary.',
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'scope': 'Two input-to-bitset setters: the simple 0x8c0a6992 index mapping wrapper and the ROM-table-driven 0x8c0a69a8 mapping wrapper. Both were checked with callback mode disabled and enabled.',
        'limitations': [
            'The callback at 0x8c0115d0 is modeled as a deterministic dispatch boundary in this composition check; its implementation and dependencies have a separate verification record.',
            'Volatile stack bytes and caller-clobbered registers are not compared byte-for-byte.',
            'Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or end-to-end game execution.'
        ],
        'routines': routines,
    }
    (out / 'analysis/input_mapping_set_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"{len(routines)} input mapping setters passed {report['total_cases']} ABI-level comparisons")


if __name__ == '__main__':
    main()
