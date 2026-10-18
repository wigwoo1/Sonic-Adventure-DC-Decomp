"""Verify five compact input callback/status helpers against original SH-4 code."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
BASE, RAM_SIZE = 0x8C010000, 16 * 1024 * 1024
BINARY = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
LIBRARY = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/sa_input_callback_and_status_helpers.dll'))
SPECS = [
    ('sa_input_request_completion_thunk_8c646884', 0x8C646884, 6, 'tail', 0x8C6468C0),
    ('sa_input_device_busy_thunk_8c646b36', 0x8C646B36, 6, 'tail', 0x8C646C34),
    ('sa_input_device_request_thunk_8c646b3c', 0x8C646B3C, 6, 'tail', 0x8C646C38),
    ('sa_input_device_state_get_8c646eda', 0x8C646EDA, 6, 'get', 0x8C646F58),
    ('sa_input_status_table_store_8c646eee', 0x8C646EEE, 16, 'store', 0x8C646F64),
]


def verify_tail(spec, function, rng):
    name, address, length, _, literal = spec
    cases = []
    for case in range(128):
        initial = bytearray(b'\xA5') * RAM_SIZE
        initial[BASE - 0x8C000000:BASE - 0x8C000000 + len(BINARY)] = BINARY
        target = 0x8C00E000 + case * 4
        struct.pack_into('<I', initial, literal & 0xFFFFFF, target)
        state = State()
        state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        state.sr = rng.getrandbits(32)
        state.pr = 0x8CFED000 + case * 4
        state.pc = address
        retail_ram, native_ram = bytearray(initial), bytearray(initial)
        expected, actual = State.from_buffer_copy(bytes(state)), State.from_buffer_copy(bytes(state))
        observed = []
        def callback(_ram, regs, sr):
            observed.append((list(regs), sr))
            return sr
        steps = reference(retail_ram, expected, hooks={target: callback})
        view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
        function(view, ctypes.byref(actual))
        assert steps == 3 and len(observed) == 1
        assert retail_ram == native_ram, f'{name} RAM mismatch case={case}'
        assert list(actual.r) == observed[0][0], f'{name} register mismatch case={case}'
        assert (actual.sr, actual.pr, actual.pc) == (observed[0][1], state.pr, target), f'{name} tail state mismatch case={case}'
        cases.append({'case': case, 'target': hex(target), 'retail_instructions': steps, 'full_ram_and_tail_state_match': True})
    return cases


def verify_getter(spec, function, rng):
    name, address, length, _, literal = spec
    cases = []
    for case in range(128):
        initial = bytearray(b'\xA5') * RAM_SIZE
        initial[BASE - 0x8C000000:BASE - 0x8C000000 + len(BINARY)] = BINARY
        pointer = 0x8C700000 + case * 4
        value = rng.getrandbits(32)
        struct.pack_into('<I', initial, literal & 0xFFFFFF, pointer)
        struct.pack_into('<I', initial, pointer & 0xFFFFFF, value)
        state = State()
        state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        state.sr = rng.getrandbits(32)
        state.pr = 0x8CFED000 + case * 4
        state.pc = address
        retail_ram, native_ram = bytearray(initial), bytearray(initial)
        expected, actual = State.from_buffer_copy(bytes(state)), State.from_buffer_copy(bytes(state))
        steps = reference(retail_ram, expected)
        view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
        function(view, ctypes.byref(actual))
        assert steps == 3
        assert retail_ram == native_ram, f'{name} RAM mismatch case={case}'
        assert bytes(expected) == bytes(actual), f'{name} CPU mismatch case={case}'
        cases.append({'case': case, 'pointer': hex(pointer), 'value': hex(value), 'retail_instructions': steps, 'full_ram_and_cpu_match': True})
    return cases


def verify_store(spec, function, rng):
    name, address, length, _, literal = spec
    cases = []
    indices = list(range(40)) + [rng.getrandbits(32) for _ in range(88)]
    for case, index in enumerate(indices):
        initial = bytearray(b'\xA5') * RAM_SIZE
        initial[BASE - 0x8C000000:BASE - 0x8C000000 + len(BINARY)] = BINARY
        if index < 32 and case % 3 == 1:
            # Once loaded, the table base may alias the literal being read.
            table_base = (literal - index * 4) & 0xFFFFFFFF
        elif index < 32 and case % 3 == 2:
            table_base = 0x8CFFFF00
        else:
            table_base = 0x8C700000 + (case % 32) * 0x1000
        struct.pack_into('<I', initial, literal & 0xFFFFFF, table_base)
        state = State()
        state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        state.r[4] = index
        state.r[5] = rng.getrandbits(32)
        state.sr = rng.getrandbits(32)
        state.pr = 0x8CFED000 + case * 4
        state.pc = address
        retail_ram, native_ram = bytearray(initial), bytearray(initial)
        expected, actual = State.from_buffer_copy(bytes(state)), State.from_buffer_copy(bytes(state))
        steps = reference(retail_ram, expected)
        view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
        function(view, ctypes.byref(actual))
        assert retail_ram == native_ram, f'{name} RAM mismatch case={case}, index={index:08x}'
        assert bytes(expected) == bytes(actual), f'{name} CPU mismatch case={case}, index={index:08x}'
        cases.append({'case': case, 'index': hex(index), 'table_base': hex(table_base), 'retail_instructions': steps, 'full_ram_and_cpu_match': True})
    return cases


rng = random.Random(0x8C646800)
for spec in SPECS:
    name, address, length, kind, _literal = spec
    function = getattr(LIBRARY, name)
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None
    if kind == 'tail':
        cases = verify_tail(spec, function, rng)
        comparison = 'Complete 16 MiB RAM and GPR/SR/PR/PC state at the indirect tail-call boundary against retail SH-4 execution.'
        scope = 'Loads the callback target from its retail literal, puts it in R3, and tail-jumps while preserving caller PR and stack.'
        limitations = ['The target callback body is a separate routine.', 'Verification uses the project SH-4 interpreter, not Dreamcast hardware or complete-game execution.']
    elif kind == 'get':
        cases = verify_getter(spec, function, rng)
        comparison = 'Complete 16 MiB RAM and all GPR/SR/PR/PC state against retail SH-4 execution.'
        scope = 'Loads a data pointer from its literal, reads the pointed-to word into R0 in the RTS delay slot, and returns.'
        limitations = ['The broader input subsystem and consumers are separate routines.', 'Verification uses the project SH-4 interpreter, not Dreamcast hardware or complete-game execution.']
    else:
        cases = verify_store(spec, function, rng)
        comparison = 'Complete 16 MiB RAM and all GPR/SR/PR/PC state against retail SH-4 execution.'
        scope = 'Stores a value into a 32-entry status table only when the unsigned index is below 32.'
        limitations = ['The larger status-table subsystem is separate.', 'Verification uses the project SH-4 interpreter, not Dreamcast hardware or complete-game execution.']
    span = BINARY[address - BASE:address - BASE + length]
    assert len(span) == length
    source = PROJECT / 'source/sa_input_callback_and_status_helpers.c'
    report = {
        'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(address),
        'end_exclusive': hex(address + length), 'name': name,
        'retail_span_sha256': hashlib.sha256(span).hexdigest(),
        'binary_sha256': hashlib.sha256(BINARY).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cases_passed': len(cases), 'comparison_level': comparison, 'scope': scope,
        'limitations': limitations, 'cases': cases,
    }
    (PROJECT / 'analysis' / f'{name}_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(f'{sum(len(json.loads((PROJECT / "analysis" / f"{spec[0]}_verification.json").read_text())["cases"]) for spec in SPECS)} input callback/status helper RAM/CPU comparisons passed')