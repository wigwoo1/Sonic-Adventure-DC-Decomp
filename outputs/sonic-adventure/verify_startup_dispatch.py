"""Compare startup tail-dispatch wrappers with retail SH-4 execution."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference


class DispatchContext(ctypes.Structure):
    _fields_ = [("target", ctypes.c_uint32), ("argument", ctypes.c_uint32),
                ("callback_count", ctypes.c_uint32)]


DISPATCH = ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State),
                            ctypes.c_uint32, ctypes.c_void_p)


def main():
    out = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    original = bytearray(16 * 1024 * 1024)
    original[0x10000:0x10000 + len(binary)] = binary
    dll_path = ROOT / "work/sonic-adventure/build/startup_dispatch.dll"
    dll = ctypes.CDLL(str(dll_path))
    rng = random.Random(0x8c0100c8)
    cases = []
    descriptions = [(0x8c0100c8, 0x8c0100d0, "sa_startup_dispatch_8c0100c8", 0x8c5ff8b8),
                    (0x8c0100d0, 0x8c0100d8, "sa_startup_dispatch_8c0100d0", 0x8c5ff936)]
    for start, end, symbol, target in descriptions:
        fn = getattr(dll, symbol)
        fn.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State), DISPATCH,
                       ctypes.c_void_p]
        fn.restype = None
        for index in range(48):
            old = bytearray(original)
            native = bytearray(old)
            argument = rng.getrandbits(32)
            struct.pack_into("<I", old, 0x66c2c0, argument)
            struct.pack_into("<I", native, 0x66c2c0, argument)
            old_state = State()
            old_state.r[:] = [rng.getrandbits(32) for _ in range(16)]
            old_state.sr, old_state.pr, old_state.pc = (
                rng.getrandbits(32), rng.getrandbits(32), start)
            native_state = State.from_buffer_copy(bytes(old_state))
            observed = []

            def hook(memory, registers, sr):
                observed.append((target, registers[4]))
                memory[0xb40000:0xb40004] = struct.pack(
                    "<I", (target + registers[4]) & 0xffffffff)
                registers[9] ^= target
                return sr

            steps = reference(old, old_state, hooks={target: hook})
            context = DispatchContext()

            @DISPATCH
            def dispatch(memory, state_ptr, target_arg, user_context):
                ctx = ctypes.cast(user_context, ctypes.POINTER(DispatchContext)).contents
                ctx.target = target_arg
                ctx.argument = state_ptr.contents.r[4]
                ctx.callback_count += 1
                value = (target_arg + state_ptr.contents.r[4]) & 0xffffffff
                for byte_index in range(4):
                    memory[0xb40000 + byte_index] = (value >> (byte_index * 8)) & 0xff
                state_ptr.contents.r[9] ^= target_arg

            memory_view = (ctypes.c_uint8 * len(native)).from_buffer(native)
            fn(memory_view, ctypes.byref(native_state), dispatch,
               ctypes.cast(ctypes.pointer(context), ctypes.c_void_p))
            assert native == old, f"RAM mismatch at {start:#x}, case {index}"
            assert bytes(native_state) == bytes(old_state), f"CPU mismatch at {start:#x}, case {index}"
            assert observed == [(context.target, context.argument)]
            assert context.callback_count == 1
            cases.append({"routine_start": hex(start), "case": index,
                          "retail_instructions_executed": steps,
                          "target_argument_ram_and_cpu_match": True})

    report = {
        "cases_passed": len(cases),
        "routine_ranges": [{"start": hex(start), "end_exclusive": hex(end),
                            "symbol": symbol, "target": hex(target)}
                           for start, end, symbol, target in descriptions],
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "source": "source/startup_dispatch.c",
        "source_sha256": hashlib.sha256((out / "source/startup_dispatch.c").read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "cases": cases,
        "scope": "Two startup tail-call wrappers; full RAM and GPR/SR/PR/PC plus service target and argument checked against retail under deterministic callbacks.",
        "limitations": ["Service implementation and hardware effects are supplied by the native host dispatcher."],
    }
    (out / "analysis/startup_dispatch_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{len(cases)} startup tail-dispatch cases matched retail")


if __name__ == "__main__":
    main()
