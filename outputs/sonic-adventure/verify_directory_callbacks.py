"""Compare directory event callback traversal and order with retail SH-4 execution."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference


def main():
    out = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    old = bytearray(16 * 1024 * 1024)
    old[0x10000:0x10000 + len(binary)] = binary
    native = bytearray(old)
    view = (ctypes.c_uint8 * len(native)).from_buffer(native)
    dll_path = ROOT / "work/sonic-adventure/build/directory_callbacks.dll"
    fn = ctypes.CDLL(str(dll_path)).sa_directory_callback_dispatch_8c645afa
    callback_type = ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint32, ctypes.c_uint32)
    fn.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_int16, callback_type]
    fn.restype = None

    table_address = 0x8c893840
    table_offset = table_address - 0x8c000000
    region = 0x00a00000
    trace = region + 0x300
    callback_targets = (0x8c000200, 0x8c000210, 0x8c000220, 0x8c000230)
    rng = random.Random(0x8c645afa)
    cases = steps = 0
    for alias in (0x8c000000, 0xac000000):
        events = (-16, -1, 0, 1, 15, 16, 17, 31, 32, 47, 48, 63)
        for event in events:
            group = event >> 4
            for count in range(5):
                initial = bytes(rng.getrandbits(8) for _ in range(0x700))
                old[region:region + len(initial)] = initial
                native[region:region + len(initial)] = initial
                table_slot = table_offset + group * 4
                node_base = alias + region + 0x500
                heads = node_base if count else 0
                old[table_slot:table_slot + 4] = struct.pack("<I", heads)
                native[table_slot:table_slot + 4] = struct.pack("<I", heads)
                expected_calls = []
                for index in range(count):
                    node = region + 0x500 + index * 12
                    target = callback_targets[index % len(callback_targets)]
                    next_pointer = alias + region + 0x500 + (index + 1) * 12 if index + 1 < count else 0
                    argument = rng.getrandbits(32)
                    values = struct.pack("<III", target, next_pointer, argument)
                    old[node:node + 12] = values
                    native[node:node + 12] = values
                    expected_calls.append((target, argument))

                retail_calls = []
                hooks = {}
                for callback_index, target in enumerate(callback_targets):
                    def make_hook(target_value, slot):
                        def hook(ram, regs, sr):
                            call = (target_value, regs[4])
                            retail_calls.append(call)
                            ram[trace + slot] = regs[4] & 0xff
                            return sr
                        return hook
                    hooks[target] = make_hook(target, callback_index)

                regs = State()
                regs.r[:] = [rng.getrandbits(32) for _ in range(16)]
                regs.r[4], regs.r[15] = event & 0xffffffff, 0x8cf00000
                regs.pc, regs.pr = 0x8c645afa, 0x8c000100
                expected = State.from_buffer_copy(bytes(regs))
                steps += reference(old, expected, hooks=hooks)

                native_calls = []
                def host_dispatch(_ram_pointer, target, argument):
                    native_calls.append((target, argument))
                    slot = callback_targets.index(target)
                    native[trace + slot] = argument & 0xff
                dispatch = callback_type(host_dispatch)
                fn(view, event, dispatch)

                assert retail_calls == expected_calls == native_calls, (
                    alias, event, count, retail_calls, expected_calls, native_calls)
                assert native[region:region + len(initial)] == old[region:region + len(initial)]
                assert native[table_slot:table_slot + 4] == old[table_slot:table_slot + 4]
                cases += 1

    start, end = 0x8c645afa, 0x8c645b1e
    report = {
        "cases_passed": cases, "routine_start": hex(start), "routine_end_exclusive": hex(end),
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[start - 0x8c010000:end - 0x8c010000]).hexdigest(),
        "retail_instructions_executed": steps,
        "source": "source/directory_callbacks.c",
        "source_sha256": hashlib.sha256((out / "source/directory_callbacks.c").read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Callback traversal order and argument values for empty and 1-4-node chains selected by positive/negative event groups, with cached/uncached list pointers and test hooks for SH-4 callback targets.",
        "limitations": ["Callback bodies are represented by the host dispatcher; cyclic lists and invalid callback/node pointers are excluded."],
    }
    (out / "analysis/directory_callbacks_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{cases} directory callback lists matched retail")


if __name__ == "__main__":
    main()
