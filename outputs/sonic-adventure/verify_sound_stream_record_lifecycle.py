"""Scripted native stream-record sequences versus uninterrupted retail RAM state.

The object method remains external: both paths execute SH-4 method instructions.
Compare callback-entry state as well as state after each recovered routine.
"""
import ctypes
import hashlib
import json
import random
import struct
from pathlib import Path

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
RAM_SIZE = 16 * 1024 * 1024
TABLE = 0x8C8A3D90
METHOD = 0x8C602618
RETURN = 0x8C74913C
SOURCES = {
    0x8C65BD2C: 'sa_audio_work_buffer_initialize_8c65bd2c.c',
    0x8C65BD46: 'sa_sound_stream_record_allocate_8c65bd46.c',
    0x8C65BF42: 'sa_stream_request_open_prepare_8c65bf42.c',
    0x8C600E7A: 'sa_stream_request_position_update_8c600e7a.c',
    0x8C65BFA4: 'sa_stream_request_seek_query_8c65bfa4.c',
    0x8C65BFC2: 'sa_stream_position_set_8c65bfc2.c',
}
SYMBOLS = {
    address: source.removesuffix('.c') for address, source in SOURCES.items()
}
binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll_path = ROOT / 'work/sonic-adventure/build/sound_stream_record_lifecycle.dll'
library = ctypes.CDLL(str(dll_path))
P = ctypes.POINTER(ctypes.c_uint8)
NativeMethod = ctypes.CFUNCTYPE(None, P, ctypes.POINTER(State), ctypes.c_uint32, ctypes.c_uint32)
native = {}
for address, symbol in SYMBOLS.items():
    function = getattr(library, symbol)
    extras = [NativeMethod] if address == 0x8C65BD46 else ([ctypes.c_void_p] if address == 0x8C65BFA4 else [])
    function.argtypes = [P, ctypes.POINTER(State)] + extras
    function.restype = ctypes.c_int if address == 0x8C65BFA4 else None
    native[address] = function


def put32(ram, address, value):
    struct.pack_into('<I', ram, address & 0xFFFFFF, value & 0xFFFFFFFF)


def callback_snapshot(ram, state):
    return {'cpu': bytes(state).hex(), 'ram_sha256': hashlib.sha256(ram).hexdigest()}


def run_sequence(alias):
    rng = random.Random(alias)
    expected_ram = bytearray(RAM_SIZE)
    expected_ram[0x10000:0x10000 + len(binary)] = binary
    expected_ram[TABLE & 0xFFFFFF:(TABLE & 0xFFFFFF) + 3200] = rng.randbytes(3200)
    request = alias | 0x760000
    obj = alias | 0x750000
    vtable = alias | 0x751000
    method_address = METHOD if alias == 0x8C000000 else 0x8C700000
    put32(expected_ram, obj, vtable)
    put32(expected_ram, vtable + 0x24, method_address)
    if method_address != METHOD:
        # Explicit test method: read saved PR through R15, write it through
        # R14 to the record's +0x20 field, and return it. This makes correct
        # callback frame setup observable in output RAM as well as snapshots.
        fixture = struct.pack('<4H', 0x60F2, 0x1E08, 0x000B, 0x0009)
        start = method_address & 0xFFFFFF
        expected_ram[start:start + len(fixture)] = fixture
    put32(expected_ram, request + 0x10, 0x4000)
    put32(expected_ram, request + 0x14, 64)
    struct.pack_into('<H', expected_ram, (request + 0x4C) & 0xFFFFFF, 0xFFFF)
    actual_ram = bytearray(expected_ram)
    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    expected = State()
    expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
    expected.sr = rng.getrandbits(32)
    actual = State.from_buffer_copy(expected)
    events = []
    expected_calls, actual_calls, callback_errors = [], [], []

    @NativeMethod
    def object_method(ram_pointer, state_pointer, object_address, argument):
        try:
            state = state_pointer.contents
            memory = (ctypes.c_uint8 * RAM_SIZE).from_address(ctypes.addressof(ram_pointer.contents))
            actual_calls.append(callback_snapshot(memory, state))
            assert state.r[4] == object_address and state.r[5] == argument
            # External service executes the selected SH-4 body; this is not
            # claimed as a recovered native object method.
            reference(memory, state)
        except Exception as error:
            callback_errors.append(repr(error))

    def step(address, arguments=()):
        expected_calls.clear()
        actual_calls.clear()
        callback_errors.clear()
        for state in (expected, actual):
            for index, value in enumerate(arguments, 4):
                state.r[index] = value & 0xFFFFFFFF
            state.r[15] = alias | 0x7FF000
            state.pr = RETURN
            state.pc = address

        def trace(pc, word, regs, sr, pr, fpu):
            if pc == method_address:
                entry = State()
                entry.r[:] = regs
                entry.sr, entry.pr, entry.pc = sr, pr, pc
                expected_calls.append(callback_snapshot(expected_ram, entry))

        reference(expected_ram, expected, trace=trace)
        extras = [object_method] if address == 0x8C65BD46 else ([None] if address == 0x8C65BFA4 else [])
        result = native[address](view, ctypes.byref(actual), *extras)
        if address == 0x8C65BFA4:
            assert result == 0, 'Native seek/query dispatch rejected retail targets'
        label = f'alias={alias:#x}, step={len(events)}, entry={address:#x}'
        assert not callback_errors, (label, callback_errors)
        assert actual_ram == expected_ram, f'{label}: complete RAM mismatch'
        assert bytes(actual) == bytes(expected), f'{label}: returned CPU mismatch'
        if actual_calls != expected_calls:
            actual_cpu = State.from_buffer_copy(bytes.fromhex(actual_calls[0]['cpu']))
            expected_cpu = State.from_buffer_copy(bytes.fromhex(expected_calls[0]['cpu']))
            differences = [(index, hex(actual_cpu.r[index]), hex(expected_cpu.r[index]))
                           for index in range(16) if actual_cpu.r[index] != expected_cpu.r[index]]
            raise AssertionError(f'{label}: callback entry mismatch: {differences}; '
                                 f'RAM match={actual_calls[0]["ram_sha256"] == expected_calls[0]["ram_sha256"]}')
        events.append({'entry': hex(address), 'return': hex(actual.r[0]),
                       'callback_entries_compared': len(actual_calls)})

    step(0x8C65BD2C)
    positions = [0, 1, 0x7FF, 0x800, 0xFFFFF801, 0xFFFFF800,
                 0x80000000, 0x80000001, 0x7FFFFFFF, 0xFFFFFFFF]
    for slot in range(32):
        step(0x8C65BD46, (request, slot * 0x800, positions[slot % len(positions)], obj))
        record = TABLE + slot * 100
        assert actual.r[0] == record
        step(0x8C600E7A, (request, slot * 71 if slot % 2 == 0 else -71, slot % 4))
        step(0x8C65BFA4, (record, slot * 19 if slot % 2 == 0 else -1))
        step(0x8C65BF42, (record,))
        step(0x8C65BFC2, (record, slot if slot % 2 == 0 else -1))
    step(0x8C65BD46, (request, 0, 0, obj))
    assert actual.r[0] == 0
    step(0x8C65BD2C)
    step(0x8C65BD46, (request, 0, 0xFFFFFFFF, obj))
    assert actual.r[0] == TABLE
    return {'ram_alias': hex(alias), 'method_address': hex(method_address),
            'method_kind': 'retail subtraction' if method_address == METHOD else 'explicit stack/record-write fixture',
            'steps_passed': len(events),
            'callback_entries_compared': sum(e['callback_entries_compared'] for e in events),
            'events': events}


def main():
    cases = [run_sequence(alias) for alias in (0x8C000000, 0xAC000000)]
    report = {
        'module': '1ST_READ.BIN', 'cases_passed': len(cases),
        'steps_passed': sum(case['steps_passed'] for case in cases),
        'callback_entries_compared': sum(case['callback_entries_compared'] for case in cases),
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'interpreter_sha256': hashlib.sha256((PROJECT / 'verify_startup.py').read_bytes()).hexdigest(),
        'verifier_sha256': hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
        'dll_sha256': hashlib.sha256(dll_path.read_bytes()).hexdigest(),
        'sources': {f'source/{source}': hashlib.sha256((PROJECT / 'source' / source).read_bytes()).hexdigest()
                    for source in SOURCES.values()},
        'comparison_level': 'Complete 16 MiB RAM and GPR/SR/PR/PC after every call, plus complete callback-entry CPU and RAM hashes.',
        'scope': 'Scripted persistent-RAM initialization, 32 consecutive allocations, request position updates, the original native seek/query caller with native callees, stream preparation, position setting, exhaustion, reset, and reuse in cached and uncached RAM aliases.',
        'limitations': [
            'Calls are sequenced by this test harness; the original game caller and runtime scheduler have not been recovered by this test.',
            'The object method remains external: one scenario interprets the retail subtraction helper and one interprets an explicit stack/record-write test fixture; neither is claimed as a native game object method.',
            'This is subsystem regression evidence, not hardware or whole-game execution.'
        ],
        'cases': cases,
    }
    output = PROJECT / 'analysis/sound_stream_record_lifecycle_verification.json'
    output.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'{report["steps_passed"]} connected stream steps and '
          f'{report["callback_entries_compared"]} callback-entry comparisons passed')


if __name__ == '__main__':
    main()
