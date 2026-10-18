"""Compare the primary callback wrapper with retail, hooking installer removal only."""
import ctypes
import hashlib
import itertools
import json
import random
import struct

from verify_startup import ROOT, State, reference

P = ctypes.POINTER(ctypes.c_uint32)
Remove = ctypes.CFUNCTYPE(None, ctypes.c_void_p, ctypes.c_uint32,
                          ctypes.c_uint32, P)


def main():
    out = ROOT / "outputs/sonic-adventure"
    binary = (ROOT / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
    old = bytearray(16 * 1024 * 1024)
    old[0x10000:0x10000 + len(binary)] = binary
    native = bytearray(old)
    view = (ctypes.c_uint8 * len(native)).from_buffer(native)
    dll_path = ROOT / "work/sonic-adventure/build/callback_install.dll"
    dll = ctypes.CDLL(str(dll_path))
    fn = dll.sa_callback_register_primary_8c64133e
    fn.argtypes = [ctypes.POINTER(ctypes.c_uint8), P, ctypes.c_uint32,
                   ctypes.c_uint32, ctypes.c_void_p, Remove]
    fn.restype = ctypes.c_int
    calls, expected_calls, errors = [], [], []
    sr = ctypes.c_uint32()

    @Remove
    def remove(context, driver, index, status):
        calls.append((context, driver, index, status[0]))
        if context != 0x1234:
            errors.append("wrong host service context")
        status[0] ^= 0x010000a1
        driver_base = (driver & 0x1fffffff) - 0x0c000000
        struct.pack_into("<I", native, driver_base + 0x230, 0xdeadbeef)

    def remove_hook(memory, regs, status):
        expected_calls.append((0x1234, regs[4], regs[5], status))
        status ^= 0x010000a1
        driver_base = (regs[4] & 0x1fffffff) - 0x0c000000
        struct.pack_into("<I", memory, driver_base + 0x230, 0xdeadbeef)
        return status

    cases = 0
    ram_base = 0x0c000000
    context_slot = ((0x8c67c004 & 0x1fffffff) - ram_base)
    for driver, callback, argument, initial_sr, mutate in itertools.product(
            (0x8ca00000, 0xaca00000),
            (0x8c645b1e, 0xffffffff), (0, 0xffffffff, 0x12345678),
            (0, 1, 0x60, 0xf0, 0x700083f3), (False, True)):
        for memory in (old, native):
            memory[0xa00000:0xa01000] = b"\xa5" * 4096
            struct.pack_into("<I", memory, context_slot, driver)
        entry = State()
        rng = random.Random(driver ^ callback ^ argument ^ initial_sr ^ int(mutate))
        entry.r[:] = [rng.getrandbits(32) for _ in range(16)]
        entry.r[4], entry.r[5] = callback, argument
        entry.r[15] = 0x8cf00000
        entry.pc, entry.pr, entry.sr = 0x8c64133e, 0x8c000100, initial_sr
        original_registers = list(entry.r)
        retail_state = State.from_buffer_copy(bytes(entry))
        retail_ram = bytearray(old)
        expected_calls.clear()
        steps = reference(retail_ram, retail_state)

        registers = (ctypes.c_uint32 * 16)(*original_registers)
        sr.value = initial_sr
        calls.clear()
        result = fn(view, ctypes.byref(sr), callback, argument, 0x1234, remove)
        assert result == 0
        callback_slot = (driver & 0x1fffffff) - 0x0c000000 + 0x230
        assert native[callback_slot:callback_slot + 8] == retail_ram[callback_slot:callback_slot + 8], (
            hex(driver), hex(callback), hex(argument), hex(callback_slot),
            native[callback_slot:callback_slot + 8].hex(),
            retail_ram[callback_slot:callback_slot + 8].hex(),
            native[context_slot:context_slot + 4].hex())
        assert sr.value == retail_state.sr, (hex(sr.value), hex(retail_state.sr))
        assert not calls and not expected_calls
        assert not errors, errors
        assert registers[15] == original_registers[15]
        cases += 1

    start, end = 0x8c64133e, 0x8c64135a
    report = {
        "cases_passed": cases,
        "routine_start": hex(start), "routine_end_exclusive": hex(end),
        "retail_binary_sha256": hashlib.sha256(binary).hexdigest(),
        "retail_span_sha256": hashlib.sha256(binary[start - 0x8c010000:
                                                      end - 0x8c010000]).hexdigest(),
        "source": "source/callback_install.c",
        "source_sha256": hashlib.sha256((out / "source/callback_install.c").read_bytes()).hexdigest(),
        "dll_sha256": hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        "scope": "Full 16 MiB RAM and SR comparison against retail for three aliased driver bases, callback null/non-null/wrap values, arguments, interrupt masks and both default/removal paths; default callback registration runs original retail code. Zero-callback external removal routine is modeled by a host callback that mutates SR and RAM.",
        "retail_instructions_executed": steps,
        "limitations": ["CPU exceptions and invalid unmapped driver pointers are not modeled.",
                        "Hardware interrupt delivery is outside this leaf-wrapper comparison."],
    }
    (out / "analysis/callback_register_primary_verification.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{cases} primary callback wrapper full-state comparisons passed")


if __name__ == "__main__":
    main()
