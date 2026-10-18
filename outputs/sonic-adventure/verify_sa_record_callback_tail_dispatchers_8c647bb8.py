"""Differentially verify the two indexed record callback tail dispatchers."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
BASE, RAM_SIZE, LITERAL = 0x8C010000, 16 * 1024 * 1024, 0x8C647C4C
BINARY = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
LIBRARY = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/sa_record_callback_tail_dispatchers_8c647bb8.dll'))


def put32(ram, address, value):
    struct.pack_into('<I', ram, address & 0xFFFFFF, value & 0xFFFFFFFF)


def read32(ram, address):
    return struct.unpack_from('<I', ram, address & 0xFFFFFF)[0]


def verify_one(name, address, length, offset, has_argument, seed):
    function = getattr(LIBRARY, name)
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None
    rng = random.Random(seed)
    cases = []
    indexes = [0, 1, 2, 3, 0x7F, 0x100, 0x0FFFF,
               0x10000] + [rng.randrange(0x10000) for _ in range(150)]
    for case, index in enumerate(indexes):
        table = 0x8C700000 + case * 0x400
        initial = bytearray(b'\xA5') * RAM_SIZE
        initial[BASE - 0x8C000000:BASE - 0x8C000000 + len(BINARY)] = BINARY
        put32(initial, LITERAL, table)
        state = State()
        state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        state.r[4] = index
        state.r[14] = rng.getrandbits(32)
        state.r[15] = 0x8C800000 + case * 0x100 + 0x40
        state.sr = rng.getrandbits(32)
        state.pr = 0x8CFED000 + case * 4
        state.pc = address

        entry = (table + ((index << 7) & 0xFFFFFFFF)) & 0xFFFFFFFF
        field = (entry + offset) & 0xFFFFFFFF
        target = 0 if case % 3 == 0 else 0x8C00E000 + case * 4
        argument = rng.getrandbits(32)
        put32(initial, field, target)
        if has_argument:
            put32(initial, entry + 0x7C, argument)

        alias = case % 7
        if alias == 0:
            # The save overwrites the callback slot before the routine reads it.
            state.r[15] = field + 4
        elif alias == 1 and has_argument:
            # The save overwrites the argument slot before its delay-slot load.
            state.r[15] = entry + 0x80
        elif alias == 2:
            # The save overwrites the table-base literal before it is loaded.
            state.r[15] = LITERAL + 4
            state.r[14] = 0x8C720000 + case * 0x400
            table = state.r[14]
            entry = (table + ((index << 7) & 0xFFFFFFFF)) & 0xFFFFFFFF
            field = (entry + offset) & 0xFFFFFFFF
            target = 0 if case % 3 == 0 else 0x8C00E000 + case * 4
            put32(initial, entry + 0x74, 0)
            put32(initial, entry + 0x78, 0)
            put32(initial, entry + 0x7C, argument)
            put32(initial, field, target)
        elif alias == 3:
            # Alias the save with the loaded callback target memory.
            target = state.r[15] - 4
            put32(initial, field, target)

        prepared = bytearray(initial)
        put32(prepared, state.r[15] - 4, state.r[14])
        effective_table = read32(prepared, LITERAL)
        effective_entry = (effective_table + ((index << 7) & 0xFFFFFFFF)) & 0xFFFFFFFF
        target = read32(prepared, effective_entry + offset)

        retail_ram, native_ram = bytearray(initial), bytearray(initial)
        expected = State.from_buffer_copy(bytes(state))
        actual = State.from_buffer_copy(bytes(state))
        observed = []
        hooks = {}
        if target != 0:
            def callback(_ram, regs, sr):
                observed.append((list(regs), sr))
                return sr
            hooks[target] = callback
        reference_steps = reference(retail_ram, expected, hooks=hooks)
        native_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
        function(native_view, ctypes.byref(actual))
        assert native_ram == retail_ram, f'{name} RAM mismatch in case {case}, alias={alias}'
        if target:
            assert observed, f'{name} callback not reached in case {case}'
            expected_regs, expected_sr = observed[0]
            assert list(actual.r) == expected_regs, f'{name} register mismatch in case {case}'
            assert (actual.sr, actual.pr, actual.pc) == (expected_sr, state.pr, target), f'{name} tail state mismatch in case {case}'
        else:
            assert bytes(actual) == bytes(expected), f'{name} return state mismatch in case {case}'
        cases.append({'case': case, 'index': hex(index), 'alias_fixture': alias,
                      'callback_target': hex(target), 'retail_instructions': reference_steps,
                      'full_ram_and_cpu_match': True})

    span = BINARY[address - BASE:address - BASE + length]
    assert len(span) == length
    report = {
        'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(address),
        'end_exclusive': hex(address + length), 'name': name,
        'retail_span_sha256': hashlib.sha256(span).hexdigest(),
        'binary_sha256': hashlib.sha256(BINARY).hexdigest(),
        'source_sha256': hashlib.sha256((PROJECT / 'source/sa_record_callback_tail_dispatchers_8c647bb8.c').read_bytes()).hexdigest(),
        'cases_passed': len(cases),
        'comparison_level': 'Complete 16 MiB RAM and all GPR/SR/PR/PC state at return or indirect tail-call boundary against retail SH-4 execution.',
        'scope': 'Indexed record callback dispatch with empty and non-empty callback paths, varied indices, and stack-save alias cases.',
        'limitations': ['The invoked callback bodies are separate routines.', 'Verification uses the project SH-4 interpreter, not Dreamcast hardware or complete-game execution.'],
        'cases': cases,
    }
    report_path = PROJECT / 'analysis' / f'{name}_verification.json'
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    return len(cases)


passed = 0
passed += verify_one('sa_record_callback_arg_tail_dispatch_8c647bb8', 0x8C647BB8, 38, 0x74, True, 0x8C647BB8)
passed += verify_one('sa_record_callback_tail_dispatch_8c647bde', 0x8C647BDE, 34, 0x78, False, 0x8C647BDE)
print(f'{passed} indexed record callback dispatch RAM/CPU comparisons passed')