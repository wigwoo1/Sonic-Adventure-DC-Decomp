"""Compare the full startup initializer with retail SH-4 execution."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference


class DispatchContext(ctypes.Structure):
    _fields_ = [("calls", ctypes.c_uint32), ("targets", ctypes.c_uint32 * 3),
                ("arguments", ctypes.c_uint32 * 3)]


DISPATCH = ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                            ctypes.c_uint32, ctypes.c_void_p)


def main():
    out = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    dll_path = ROOT / "work/sonic-adventure/build/startup_initialize.dll"
    fn = ctypes.CDLL(str(dll_path)).sa_startup_initialize_8c0100a0
    fn.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State), DISPATCH,
                   ctypes.c_void_p]
    fn.restype = None
    rng = random.Random(0x8c0100a0)
    cases = []
    for case_index in range(24):
        old = bytearray(rng.randbytes(16 * 1024 * 1024))
        old[0x10000:0x10000 + len(binary)] = binary

        # Bound all four section ranges while covering empty, reverse, overlap,
        # aliasing with descriptors, and randomized source/destination contents.
        starts = [0x8c900000 + index * 0x1000 for index in range(4)]
        lengths = [rng.randrange(0, 40) for _ in range(4)]
        sources = [0x8ca00000, 0x8ca01000]
        if case_index == 0:
            lengths = [0, 0, 0, 0]
        elif case_index == 1:
            lengths = [-1, -1, -1, -1]
        elif case_index == 2:
            sources = [starts[1] - 2, starts[3] - 2]
        elif case_index == 3:
            starts = [0x8c1111b8, 0x8c1111a8, 0x8c66c2f8, 0x8c66c2e4]
            lengths = [3, 3, 3, 3]
        descriptors = [
            (0x8c1111ac, starts[0]), (0x8c1111b8, starts[0] + lengths[0]),
            (0x8c11119c, starts[1]), (0x8c111194, sources[0]),
            (0x8c1111a8, starts[1] + lengths[1]),
            (0x8c66c2ec, starts[2]), (0x8c66c2f8, starts[2] + lengths[2]),
            (0x8c66c2d8, starts[3]), (0x8c66c2d0, sources[1]),
            (0x8c66c2e4, starts[3] + lengths[3]),
            (0x8c66c3ac, 0x8c700000), (0x8c700000, 0x8c700100),
            (0x8c66c2c0, rng.getrandbits(32)),
        ]
        for address, value in descriptors:
            struct.pack_into("<I", old, address - 0x8c000000, value & 0xffffffff)
        for source in sources:
            old[source - 0x8c000000:source - 0x8c000000 + 128] = rng.randbytes(128)

        native = bytearray(old)
        old_state = State()
        old_state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        old_state.r[15] = 0x8cf00000
        old_state.sr = rng.getrandbits(32)
        old_state.pr = 0x8c00ffee
        old_state.pc = 0x8c0100a0
        native_state = State.from_buffer_copy(bytes(old_state))
        expected_calls = []
        targets = (0x8c5ff8b8, 0x8c053940, 0x8c5ff936)

        def make_hook(target):
            def hook(memory, registers, sr):
                expected_calls.append((target, registers[4]))
                marker = (target ^ registers[4]) & 0xffffffff
                struct.pack_into("<I", memory, 0xb00000, marker)
                registers[8] = (registers[8] + target + registers[4]) & 0xffffffff
                return sr
            return hook

        hooks = {target: make_hook(target) for target in targets}
        expected_steps = reference(old, old_state, hooks=hooks)
        native_context = DispatchContext()

        @DISPATCH
        def dispatch(memory, state_ptr, target, user_context):
            context = ctypes.cast(user_context, ctypes.POINTER(DispatchContext)).contents
            index = context.calls
            assert index < 3
            context.targets[index] = target
            context.arguments[index] = state_ptr.contents.r[4]
            context.calls += 1
            marker = (target ^ state_ptr.contents.r[4]) & 0xffffffff
            target_offset = 0xb00000
            for byte_index in range(4):
                memory[target_offset + byte_index] = (marker >> (byte_index * 8)) & 0xff
            state_ptr.contents.r[8] = (state_ptr.contents.r[8] + target +
                                       state_ptr.contents.r[4]) & 0xffffffff

        memory_view = (ctypes.c_uint8 * len(native)).from_buffer(native)
        fn(memory_view, ctypes.byref(native_state), dispatch,
           ctypes.cast(ctypes.pointer(native_context), ctypes.c_void_p))
        assert native == old, f"RAM mismatch in case {case_index}"
        if bytes(native_state) != bytes(old_state):
            actual_regs = list(native_state.r)
            expected_regs = list(old_state.r)
            differences = {f"r{index}": (hex(actual_regs[index]), hex(expected_regs[index]))
                           for index in range(16) if actual_regs[index] != expected_regs[index]}
            for field in ("sr", "pr", "pc"):
                actual_value, expected_value = getattr(native_state, field), getattr(old_state, field)
                if actual_value != expected_value:
                    differences[field] = (hex(actual_value), hex(expected_value))
            raise AssertionError(f"CPU state mismatch in case {case_index}: {differences}")
        actual_calls = [(native_context.targets[i], native_context.arguments[i])
                        for i in range(native_context.calls)]
        assert actual_calls == expected_calls, f"service dispatch mismatch in case {case_index}"
        cases.append({"case": case_index, "retail_instructions_executed": expected_steps,
                      "callback_calls": len(expected_calls), "ram_cpu_and_dispatch_match": True})

    start, end = 0x8c0100a0, 0x8c0100c8
    report = {
        "cases_passed": len(cases), "routine_start": hex(start),
        "routine_end_exclusive": hex(end),
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[start - 0x8c010000:end - 0x8c010000]).hexdigest(),
        "source": "source/startup_initialize.c",
        "source_sha256": hashlib.sha256((out / "source/startup_initialize.c").read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "cases": cases,
        "scope": "Full 16 MiB RAM, GPR/SR/PR/PC, stack, section writes, and three service targets/arguments compared with retail under bounded linker descriptors.",
        "limitations": ["External startup services are deterministic test callbacks, not Dreamcast hardware or game-level service implementations.",
                        "Malformed pointers outside mapped RAM are not tested."],
    }
    (out / "analysis/startup_initialize_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} full startup-initializer cases matched retail")


if __name__ == "__main__":
    main()
