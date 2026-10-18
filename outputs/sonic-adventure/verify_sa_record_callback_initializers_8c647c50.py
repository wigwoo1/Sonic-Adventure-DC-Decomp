"""Compare both indexed callback initializers against original SH-4 execution."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
BASE, RAM_SIZE = 0x8C010000, 16 * 1024 * 1024
BINARY = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
LIBRARY = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/sa_record_callback_initializers_8c647c50.dll'))
SPECS = [
    ('sa_record_callback_arg_initialize_8c647c50', 0x8C647C50, 28, 0x74),
    ('sa_record_callback_initialize_8c647c6c', 0x8C647C6C, 28, 0x78),
]


def put16(ram, address, value):
    struct.pack_into('<H', ram, address & 0xFFFFFF, value & 0xFFFF)


def put32(ram, address, value):
    struct.pack_into('<I', ram, address & 0xFFFFFF, value & 0xFFFFFFFF)


def run_one(spec):
    name, address, length, field_offset = spec
    function = getattr(LIBRARY, name)
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None
    rng = random.Random(address)
    cases = []
    bounds = [(0, 0x20), (1, 0x20), (0x20, 0x20), (0x21, 0x20),
              (0x80, 0x40), (0x400, 0x80), (0x401, 0x80),
              (0x300, 0x100), (0x10, 0x100), (0x180, 0x60)]
    for case in range(128):
        count, stride = bounds[case % len(bounds)] if case < len(bounds) else (rng.randrange(0x500), rng.choice([0x20, 0x40, 0x60, 0x80, 0x100]))
        table = 0x8C700000 + (case % 32) * 0x10000
        input_callback = rng.getrandbits(32)
        initial = bytearray(b'\xA5') * RAM_SIZE
        initial[BASE - 0x8C000000:BASE - 0x8C000000 + len(BINARY)] = BINARY
        put32(initial, 0x8C647D10, table)
        put16(initial, 0x8C647D0C, stride)
        put16(initial, 0x8C647D0E, count)
        state = State()
        state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        state.r[4] = input_callback
        state.r[15] = 0x8C800000 + case * 0x100 + 0x40
        state.sr = rng.getrandbits(32)
        state.pr = 0x8CFED000 + case * 4
        state.pc = address

        alias = case % 5
        if alias == 1:
            # The local stack save aliases a callback slot, then the loop overwrites it.
            state.r[15] = table + field_offset + 4
        elif alias == 2:
            # Pointer, stride, and count are loaded before the local store changes this literal.
            state.r[15] = 0x8C647D10 + 4
        elif alias == 3:
            state.r[15] = 0x8C647D0C + 4
        elif alias == 4:
            state.r[15] = table + stride + field_offset + 4

        retail_ram, native_ram = bytearray(initial), bytearray(initial)
        expected, actual = State.from_buffer_copy(bytes(state)), State.from_buffer_copy(bytes(state))
        steps = reference(retail_ram, expected)
        native_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
        function(native_view, ctypes.byref(actual))
        assert retail_ram == native_ram, f'{name} RAM mismatch case={case} alias={alias} count={count:x} stride={stride:x}'
        assert bytes(expected) == bytes(actual), f'{name} CPU mismatch case={case} alias={alias} count={count:x} stride={stride:x}'
        cases.append({'case': case, 'count_bytes': hex(count), 'stride_bytes': hex(stride),
                      'table': hex(table), 'alias_fixture': alias, 'retail_instructions': steps,
                      'full_ram_and_cpu_match': True})

    report = {
        'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(address),
        'end_exclusive': hex(address + length), 'name': name,
        'retail_span_sha256': hashlib.sha256(BINARY[address - BASE:address - BASE + length]).hexdigest(),
        'binary_sha256': hashlib.sha256(BINARY).hexdigest(),
        'source_sha256': hashlib.sha256((PROJECT / 'source/sa_record_callback_initializers_8c647c50.c').read_bytes()).hexdigest(),
        'cases_passed': len(cases),
        'comparison_level': 'Complete 16 MiB RAM and all GPR/SR/PR/PC state against retail SH-4 execution.',
        'scope': 'Record callback slot initialization across varied bounds and strides, including do-while edge cases and stack-write aliases.',
        'limitations': ['The callbacks installed in record slots are separate routines.', 'Verification uses the project SH-4 interpreter, not Dreamcast hardware or complete-game execution.'],
        'cases': cases,
    }
    (PROJECT / 'analysis' / f'{name}_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    return len(cases)


passed = sum(run_one(spec) for spec in SPECS)
print(f'{passed} indexed record callback initialization RAM/CPU comparisons passed')