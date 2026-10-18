"""Differentially verify the local startup exception helper against retail SH-4."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

DISPATCH = ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                            ctypes.c_uint32, ctypes.c_void_p)
TARGETS = (0x8c60164a, 0x8c10ba72, 0x8c601f84, 0x8c6052e0, 0x8c60215e, 0x8c60250a)


def main():
    out = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/startup_exception.dll'))
    fn = library.sa_startup_exception_8c01025a
    fn.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State), DISPATCH,
                   ctypes.c_void_p]
    fn.restype = None
    rng = random.Random(0x8c01025a)
    records = []
    for case in range(16):
        original = bytearray(rng.randbytes(16 * 1024 * 1024))
        original[0x10000:0x10000 + len(binary)] = binary
        actual = bytearray(original)
        expected_state = State()
        expected_state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected_state.sr = rng.getrandbits(32)
        expected_state.pr = 0x8c00f000 + case * 4
        expected_state.pc = 0x8c01025a
        expected_state.r[15] = 0x8cf00000
        native_state = State.from_buffer_copy(bytes(expected_state))
        expected_calls, native_calls = [], []

        def mutate(target, regs, sr, memory, pr, calls):
            args = tuple(regs[i] for i in range(4, 8))
            calls.append((target, args, regs[15], pr))
            result = (target ^ args[0] ^ (len(calls) * 0x10203)) & 0xffffffff
            marker = (target ^ args[0] ^ args[1] ^ args[2] ^ args[3]) & 0xffffffff
            for i in range(4):
                memory[0xb40000 + i] = (marker >> (8 * i)) & 0xff
            regs[0] = result
            regs[1] ^= target & 0xffff
            regs[8] = (regs[8] + target + args[0]) & 0xffffffff
            return sr

        hooks = {target: (lambda target: lambda memory, regs, sr:
                          mutate(target, regs, sr, memory, expected_state.pr,
                                 expected_calls))(target) for target in TARGETS}
        steps = reference(original, expected_state, hooks=hooks)

        @DISPATCH
        def dispatch(memory_ptr, state_ptr, target, context):
            state = state_ptr.contents
            args = tuple(state.r[i] for i in range(4, 8))
            native_calls.append((target, args, state.r[15], state.pr))
            result = (target ^ args[0] ^ (len(native_calls) * 0x10203)) & 0xffffffff
            marker = (target ^ args[0] ^ args[1] ^ args[2] ^ args[3]) & 0xffffffff
            for i in range(4):
                memory_ptr[0xb40000 + i] = (marker >> (8 * i)) & 0xff
            state.r[0] = result
            state.r[1] ^= target & 0xffff
            state.r[8] = (state.r[8] + target + args[0]) & 0xffffffff

        view = (ctypes.c_uint8 * len(actual)).from_buffer(actual)
        fn(view, ctypes.byref(native_state), dispatch, None)
        assert expected_calls == native_calls, (expected_calls, native_calls)
        assert actual == original, f'RAM mismatch in randomized case {case}'
        if bytes(native_state) != bytes(expected_state):
            diffs = {f'r{i}': (hex(expected_state.r[i]), hex(native_state.r[i]))
                     for i in range(16) if expected_state.r[i] != native_state.r[i]}
            for field in ('sr', 'pr', 'pc'):
                if getattr(expected_state, field) != getattr(native_state, field):
                    diffs[field] = (hex(getattr(expected_state, field)), hex(getattr(native_state, field)))
            raise AssertionError(f'CPU state mismatch in case {case}: {diffs}')
        records.append({'case': case, 'retail_instructions_executed': steps,
                        'service_calls': len(expected_calls), 'ram_cpu_and_calls_match': True})

    start, end = 0x8c01025a, 0x8c010294
    report = {
        'cases_passed': len(records), 'routine_start': hex(start),
        'routine_end_exclusive': hex(end),
        'retail_binary_sha256': hashlib.sha256(binary).hexdigest(),
        'retail_span_sha256': hashlib.sha256(binary[start - 0x8c010000:end - 0x8c010000]).hexdigest(),
        'source': 'source/startup_exception.c',
        'source_sha256': hashlib.sha256((out / 'source/startup_exception.c').read_bytes()).hexdigest(),
        'dll_sha256': hashlib.sha256((ROOT / 'work/sonic-adventure/build/startup_exception.dll').read_bytes()).hexdigest(),
        'scenarios': records,
        'scope': 'Six indirect service calls, stack PR save/restore, register clobbers, full 16 MiB RAM, CPU state and return address compared for 16 randomized states.',
        'limitations': ['Six external service bodies are modeled by deterministic callbacks.',
                       'No Dreamcast hardware execution claim.']
    }
    (out / 'analysis/startup_exception_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"{len(records)} startup exception states matched retail")


if __name__ == '__main__':
    main()
