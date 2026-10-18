"""Compare the early boot orchestration and retry paths with retail SH-4."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference


DISPATCH = ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                            ctypes.c_uint32, ctypes.c_void_p)
SERVICE_TARGETS = (0x8c6024d0, 0x8c02e720, 0x8c602128, 0x8c604702,
                   0x8c6024fa, 0x8c601a34, 0x8c10ba48, 0x8c600032,
                   0x8c60434c, 0x8c600ddc, 0x8c60164a, 0x8c10ba72,
                   0x8c601f84, 0x8c6052e0, 0x8c60215e, 0x8c60250a)
SCENARIOS = {
    "immediate_success": [0],
    "retry_success": [3, 0],
    "not_ready_then_success": [-0x17, 0],
    "device_error_then_success": [-0x21, 0],
    "exhausted_retries": [7] * 8,
    "repeated_not_ready": [-0x17] * 8,
    "success_on_final_attempt": [1] * 7 + [0],
}


def main():
    out = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    library = ctypes.CDLL(str(ROOT / "work/sonic-adventure/build/startup_boot.dll"))
    fn = library.sa_startup_boot_8c010180
    fn.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State), DISPATCH,
                   ctypes.c_void_p]
    fn.restype = None
    exception_library = ctypes.CDLL(str(ROOT / "work/sonic-adventure/build/startup_exception.dll"))
    exception_fn = exception_library.sa_startup_exception_8c01025a
    exception_fn.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State), DISPATCH,
                             ctypes.c_void_p]
    exception_fn.restype = None
    rng = random.Random(0x8c010180)
    records = []

    for case_index, (name, transfer_statuses) in enumerate(SCENARIOS.items()):
        original = bytearray(rng.randbytes(16 * 1024 * 1024))
        original[0x10000:0x10000 + len(binary)] = binary
        actual = bytearray(original)
        old_state = State()
        old_state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        old_state.sr = rng.getrandbits(32)
        old_state.pr = 0x8c00ffee
        old_state.pc = 0x8c010180
        old_state.r[15] = 0x8cf00000
        native_state = State.from_buffer_copy(bytes(old_state))
        native_status_index = [0]
        expected_status_index = [0]
        expected_calls = []
        native_calls = []
        expected_errors = []
        native_errors = []

        def apply(target, registers, sr, memory, pr, errors):
            if target == 0x8c600032:
                n = expected_status_index[0] if errors is expected_errors else native_status_index[0]
                if n >= len(transfer_statuses):
                    errors.append(f"unexpected extra transfer call #{n}")
                    result = 0xdeadbeef
                else:
                    result = transfer_statuses[n] & 0xffffffff
                if errors is expected_errors:
                    expected_status_index[0] += 1
                else:
                    native_status_index[0] += 1
            elif target == 0x8c60434c:
                result = 0x60434c01
            else:
                result = (target ^ registers[4] ^ 0x13579bdf) & 0xffffffff
            args = tuple(registers[index] for index in range(4, 8))
            call = (target, args, registers[15], pr)
            (expected_calls if errors is expected_errors else native_calls).append(call)
            marker = (target ^ args[0] ^ args[1] ^ args[2] ^ args[3]) & 0xffffffff
            for byte_index in range(4):
                memory[0xb40000 + byte_index] = (marker >> (byte_index * 8)) & 0xff
            registers[0] = result
            registers[1] ^= target & 0xffff
            registers[8] = (registers[8] + target + args[0]) & 0xffffffff
            return sr

        hooks = {target: (lambda target: lambda memory, regs, sr:
                          apply(target, regs, sr, memory, old_state.pr, expected_errors))(target)
                 for target in SERVICE_TARGETS}
        try:
            steps = reference(original, old_state, hooks=hooks)
        except Exception as exc:
            raise AssertionError(f"retail emulation failed for {name}: {exc}") from exc

        @DISPATCH
        def dispatch(memory, state_ptr, target, context):
            state = state_ptr.contents
            if target == 0x8c01025a:
                exception_fn(memory, state_ptr, dispatch, context)
                return
            # Keep the transfer script index independent from the retail path.
            if target == 0x8c600032:
                n = native_status_index[0]
                if n >= len(transfer_statuses):
                    native_errors.append(f"unexpected extra transfer call #{n}")
                    result = 0xdeadbeef
                else:
                    result = transfer_statuses[n] & 0xffffffff
                native_status_index[0] += 1
            elif target == 0x8c60434c:
                result = 0x60434c01
            else:
                result = (target ^ state.r[4] ^ 0x13579bdf) & 0xffffffff
            args = tuple(state.r[index] for index in range(4, 8))
            native_calls.append((target, args, state.r[15], state.pr))
            marker = (target ^ args[0] ^ args[1] ^ args[2] ^ args[3]) & 0xffffffff
            for byte_index in range(4):
                memory[0xb40000 + byte_index] = (marker >> (byte_index * 8)) & 0xff
            state.r[0] = result
            state.r[1] ^= target & 0xffff
            state.r[8] = (state.r[8] + target + args[0]) & 0xffffffff

        view = (ctypes.c_uint8 * len(actual)).from_buffer(actual)
        fn(view, ctypes.byref(native_state), dispatch, None)
        assert not expected_errors and not native_errors, (name, expected_errors, native_errors)
        if expected_calls != native_calls:
            for call_index, (expected, actual_call) in enumerate(zip(expected_calls, native_calls)):
                if expected != actual_call:
                    raise AssertionError(f"service call mismatch: {name} call {call_index}: "
                                         f"retail={expected!r}, native={actual_call!r}")
            raise AssertionError(f"service call count mismatch: {name}: "
                                 f"retail={expected_calls!r}, native={native_calls!r}")
        assert actual == original, f"RAM mismatch: {name}"
        if bytes(native_state) != bytes(old_state):
            diffs = {f"r{i}": (hex(old_state.r[i]), hex(native_state.r[i]))
                     for i in range(16) if old_state.r[i] != native_state.r[i]}
            for field in ("sr", "pr", "pc"):
                if getattr(old_state, field) != getattr(native_state, field):
                    diffs[field] = (hex(getattr(old_state, field)), hex(getattr(native_state, field)))
            raise AssertionError(f"CPU state mismatch: {name}: {diffs}")
        assert native_status_index[0] == expected_status_index[0]
        records.append({"scenario": name, "transfer_statuses": [hex(x & 0xffffffff)
                        for x in transfer_statuses], "retail_instructions_executed": steps,
                        "service_calls": len(expected_calls), "ram_cpu_and_calls_match": True})

    start, end = 0x8c010180, 0x8c01025a
    report = {
        "cases_passed": len(records), "routine_start": hex(start),
        "routine_end_exclusive": hex(end),
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[start - 0x8c010000:end - 0x8c010000]).hexdigest(),
        "source": "source/startup_boot.c",
        "source_sha256": hashlib.sha256((out / "source/startup_boot.c").read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256((ROOT / "work/sonic-adventure/build/startup_boot.dll").read_bytes()).hexdigest(),
        "scenarios": records,
        "scope": "Full 16 MiB RAM, GPR/SR/PR/PC, stack frame, subsystem call arguments, retry outcomes and service ordering compared with retail across seven success/retry/error scenarios.",
        "limitations": ["External firmware and subsystem service bodies are represented by deterministic host callbacks.",
                        "The local exception routine 0x8c01025a is executed from recovered C and separately compared with retail.",
                        "No Dreamcast hardware execution claim."],
    }
    (out / "analysis/startup_boot_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(records)} startup boot/retry scenarios matched retail")


if __name__ == "__main__":
    main()
