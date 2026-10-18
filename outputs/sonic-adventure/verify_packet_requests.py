"""Differentially test the packet read/write request wrappers against retail."""
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
    dll_path = ROOT / "work/sonic-adventure/build/packet_requests.dll"
    lib = ctypes.CDLL(str(dll_path))
    functions = {
        "packet_read_request": (0x8C6041D4, lib.sa_packet_read_request_8c6041d4),
        "packet_write_request": (0x8C604210, lib.sa_packet_write_request_8c604210),
    }
    for _, fn in functions.values():
        fn.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_int32, ctypes.c_uint32]
        fn.restype = ctypes.c_uint32
    clear_fn = lib.sa_packet_context_clear_8c6042e4
    clear_fn.argtypes = [ctypes.POINTER(ctypes.c_uint8)]
    clear_fn.restype = None

    active_slot = 0x8C673450
    region = 0x00A00000
    context_addr = 0x8C000000 + region
    cases = {name: 0 for name in functions}
    steps_by_name = {name: 0 for name in functions}
    rng = random.Random(0x8C6041D4)
    fixtures = []
    for state in (0, 1, 2, 0xffffffff):
        for limit in (0, 1, 4, 0x7fffffff):
            fixtures.append((context_addr, state, limit, 0))
    fixtures.extend((0, 0, 4, idx) for idx in (-1, 0, 3, 4, 0x7fffffff, -0x80000000))
    fixtures.extend((context_addr, 0, 4, idx) for idx in (-1, 0, 1, 3, 4, 0x7fffffff, -0x80000000))
    fixtures.extend((context_addr, rng.getrandbits(32), rng.getrandbits(32), rng.randint(-5, 8))
                    for _ in range(80))

    for name, (entry, fn) in functions.items():
        for alias in (0x8C000000, 0xAC000000):
            for configured_context, state, limit, packet_index in fixtures:
                if configured_context:
                    context = alias + region
                    slot_value = context
                else:
                    context = 0
                    slot_value = 0
                rng_state = bytes(rng.getrandbits(8) for _ in range(0x100))
                old[region:region + len(rng_state)] = rng_state
                native[region:region + len(rng_state)] = rng_state
                old[active_slot - 0x8C000000:active_slot - 0x8C000000 + 4] = struct.pack("<I", slot_value)
                native[active_slot - 0x8C000000:active_slot - 0x8C000000 + 4] = struct.pack("<I", slot_value)
                if context:
                    offset = region
                    old[offset + 0x14:offset + 0x18] = struct.pack("<I", limit)
                    old[offset + 0x1c:offset + 0x20] = struct.pack("<I", state)
                    old[offset + 0x24:offset + 0x28] = struct.pack("<I", 0xabcdef01)
                    native[offset:offset + 0x100] = old[offset:offset + 0x100]
                regs = State()
                regs.r[:] = [rng.getrandbits(32) for _ in range(16)]
                callback = rng.getrandbits(32)
                regs.r[4], regs.r[5], regs.r[15] = packet_index & 0xffffffff, callback, 0x8CF00000
                regs.pc, regs.pr = entry, 0x8C000100
                expected = State.from_buffer_copy(bytes(regs))
                steps_by_name[name] += reference(old, expected)
                actual = fn(view, packet_index, callback)
                assert actual == expected.r[0], (name, alias, state, limit, packet_index,
                                                  hex(actual), hex(expected.r[0]))
                assert native[region:region + 0x100] == old[region:region + 0x100], (name, alias, packet_index)
                slot_offset = active_slot - 0x8C000000
                assert native[slot_offset:slot_offset + 4] == old[slot_offset:slot_offset + 4]
                cases[name] += 1

    clear_cases = clear_steps = 0
    for _ in range(64):
        old_value = rng.choice((0, context_addr, 0x8c010000, 0xffffffff, rng.getrandbits(32)))
        old[active_slot - 0x8C000000:active_slot - 0x8C000000 + 4] = struct.pack("<I", old_value)
        native[active_slot - 0x8C000000:active_slot - 0x8C000000 + 4] = struct.pack("<I", old_value)
        regs = State()
        regs.r[:] = [rng.getrandbits(32) for _ in range(16)]
        regs.r[15] = 0x8CF00000
        regs.pc, regs.pr = 0x8C6042E4, 0x8C000100
        expected = State.from_buffer_copy(bytes(regs))
        clear_steps += reference(old, expected)
        clear_fn(view)
        slot_offset = active_slot - 0x8C000000
        assert native[slot_offset:slot_offset + 4] == old[slot_offset:slot_offset + 4] == b"\x00\x00\x00\x00"
        clear_cases += 1

    source_hash = hashlib.sha256((out / "source/packet_requests.c").read_bytes()).hexdigest()
    dll_hash = hashlib.sha256(dll_path.read_bytes()).hexdigest()
    routine_defs = [
        ("packet_read_request", "0x8c6041d4", "0x8c604210",
         "Index validation, active-context and busy-state errors, and read-request field updates."),
        ("packet_write_request", "0x8c604210", "0x8c604250",
         "Index validation, active-context and busy-state errors, write-request field updates, and retry-field clearing."),
        ("packet_context_clear", "0x8c6042e4", "0x8c6042ec",
         "Clearing the active packet context global from null, valid, arbitrary, and sentinel initial values."),
    ]
    cases["packet_context_clear"] = clear_cases
    steps_by_name["packet_context_clear"] = clear_steps
    report = {
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "source": "source/packet_requests.c", "source_sha256": source_hash,
        "dll_sha256": dll_hash,
        "routines": [{"routine_start": start, "routine_end_exclusive": end,
                      "name": name, "cases_passed": cases[name],
                      "retail_instructions_executed": steps_by_name[name],
                      "retail_span_sha256": hashlib.sha256(binary[int(start, 16) - 0x8C010000:int(end, 16) - 0x8C010000]).hexdigest(),
                      "scope": scope} for name, start, end, scope in routine_defs],
        "limitations": ["Unmapped pointers and hardware exception behavior are not tested."],
    }
    (out / "analysis/packet_requests_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{sum(cases.values())} packet request cases matched retail: {cases}")


if __name__ == "__main__":
    main()
