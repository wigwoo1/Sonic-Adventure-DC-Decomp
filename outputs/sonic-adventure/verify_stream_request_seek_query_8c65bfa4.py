"""Verify an original retail caller with both recovered native callees linked."""
import ctypes
import hashlib
import json
import random
import struct
from pathlib import Path

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
ENTRY, END = 0x8C65BFA4, 0x8C65BFBC
UPDATE, QUERY = 0x8C600E7A, 0x8C600EC6
RAM_SIZE = 16 * 1024 * 1024
RETURN = 0x8C74913C
SOURCES = ['sa_stream_request_seek_query_8c65bfa4.c',
           'sa_stream_request_position_update_8c600e7a.c',
           'sa_stream_request_open_prepare_8c65bf42.c']
binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll = ROOT / 'work/sonic-adventure/build/stream_request_seek_query_8c65bfa4.dll'
library = ctypes.CDLL(str(dll))
P = ctypes.POINTER(ctypes.c_uint8)
Observe = ctypes.CFUNCTYPE(None, P, ctypes.POINTER(State))
native = library.sa_stream_request_seek_query_8c65bfa4
native.argtypes = [P, ctypes.POINTER(State), Observe]
native.restype = ctypes.c_int


def put32(ram, address, value):
    struct.pack_into('<I', ram, address & 0xFFFFFF, value & 0xFFFFFFFF)


def snapshot(ram, state):
    return (bytes(state), hashlib.sha256(ram).hexdigest())


def main():
    rng = random.Random(ENTRY)
    values = [0, 1, 99, 100, 101, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF]
    cases, results = [], {}
    for case in range(160):
        alias = 0x8C000000 if case % 2 == 0 else 0xAC000000
        record = alias | 0x780000
        request = 0 if case % 10 == 0 else alias | 0x790000
        ram = bytearray(RAM_SIZE)
        ram[0x10000:0x10000 + len(binary)] = binary
        ram[record & 0xFFFFFF:(record & 0xFFFFFF) + 100] = rng.randbytes(100)
        put32(ram, record + 8, request)
        status = [0, 1, 0xFFFF, 0x8000][(case // 2) % 4]
        maximum = values[(case // 4) % len(values)] if case < 128 else rng.getrandbits(32)
        prior = rng.getrandbits(32)
        if request:
            struct.pack_into('<H', ram, (request + 0x4C) & 0xFFFFFF, status)
            put32(ram, request + 0x10, maximum)
            put32(ram, request + 0x14, prior)
        state = State()
        state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        state.r[4] = record
        state.r[5] = values[case % len(values)] if case < 128 else rng.getrandbits(32)
        state.r[15] = alias | 0x7FF000
        state.sr, state.pr, state.pc = rng.getrandbits(32), RETURN, ENTRY
        expected_ram, expected = bytearray(ram), State.from_buffer_copy(state)
        expected_entries = []

        def trace(pc, word, registers, sr, pr, fpu):
            if pc in (UPDATE, QUERY):
                entry = State()
                entry.r[:] = registers
                entry.sr, entry.pr, entry.pc = sr, pr, pc
                expected_entries.append(snapshot(expected_ram, entry))

        reference(expected_ram, expected, trace=trace)
        actual_ram, actual = bytearray(ram), State.from_buffer_copy(state)
        actual_entries = []

        @Observe
        def observe(ram_pointer, state_pointer):
            memory = (ctypes.c_uint8 * RAM_SIZE).from_address(ctypes.addressof(ram_pointer.contents))
            actual_entries.append(snapshot(memory, state_pointer.contents))

        view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        assert native(view, ctypes.byref(actual), observe) == 0
        assert actual_entries == expected_entries, f'case {case}: native callee-entry state differs'
        assert len(actual_entries) == 2
        assert actual_ram == expected_ram, f'case {case}: full RAM differs'
        assert bytes(actual) == bytes(expected), f'case {case}: returned CPU differs'
        # Also exercise the production path with no observer for the same input.
        quiet_ram, quiet = bytearray(ram), State.from_buffer_copy(state)
        quiet_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(quiet_ram)
        assert native(quiet_view, ctypes.byref(quiet), Observe()) == 0
        assert quiet_ram == expected_ram and bytes(quiet) == bytes(expected)
        seek_result = State.from_buffer_copy(expected_entries[1][0]).r[0]
        results[hex(seek_result)] = results.get(hex(seek_result), 0) + 1
        cases.append({'case': case, 'request': hex(request), 'status': status,
                      'maximum': hex(maximum), 'offset': hex(state.r[5]),
                      'seek_status': hex(seek_result), 'returned_position': hex(actual.r[0]),
                      'matched': True})

    assert all(code in results for code in ('0x0', '0xfffffff6', '0xffffffef')), results
    # Explicit host contract: unsupported retargeting is rejected atomically.
    for slot in (0x8C65C000, 0x8C65BFEC):
        rejected_ram = bytearray(ram)
        put32(rejected_ram, slot, 0x8C700000)
        before = bytes(rejected_ram)
        rejected = State.from_buffer_copy(state)
        view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(rejected_ram)
        assert native(view, ctypes.byref(rejected), Observe()) == -1
        assert bytes(rejected) == bytes(state) and bytes(rejected_ram) == before

    report = {
        'module': '1ST_READ.BIN', 'load_address': '0x8c010000',
        'address': hex(ENTRY), 'end_exclusive': hex(END), 'indexed_instruction_bytes': END - ENTRY,
        'retail_span_sha256': hashlib.sha256(binary[ENTRY - 0x8C010000:END - 0x8C010000]).hexdigest(),
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256((PROJECT / 'source' / SOURCES[0]).read_bytes()).hexdigest(),
        'sources': {f'source/{source}': hashlib.sha256((PROJECT / 'source' / source).read_bytes()).hexdigest()
                    for source in SOURCES},
        'dll_sha256': hashlib.sha256(dll.read_bytes()).hexdigest(),
        'interpreter_sha256': hashlib.sha256((PROJECT / 'verify_startup.py').read_bytes()).hexdigest(),
        'verifier_sha256': hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
        'cases_passed': len(cases), 'callee_entries_compared': len(cases) * 2,
        'uninstrumented_cases_passed': len(cases), 'unsupported_target_cases_passed': 2,
        'seek_status_distribution': results,
        'comparison_level': 'Complete 16 MiB RAM and GPR/SR/PR/PC after return and at both native callee entries; also checked without instrumentation.',
        'scope': 'Original 0x8c65bfa4 caller invokes the recovered native absolute-position updater and tail-query helper, preserving stack effects and querying even after seek rejection.',
        'limitations': [
            'Both callees run natively; literal-slot targets must match the original retail functions. Other retargetings return host status -1 without mutation.',
            'The wrapper return in R0 is the queried position, not the preceding seek status; the host return code is separate.',
            'Interpreter differential evidence is not Dreamcast hardware or whole-game execution.'
        ],
        'cases': cases,
    }
    path = PROJECT / 'analysis/stream_request_seek_query_8c65bfa4_verification.json'
    path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'{len(cases)} seek/query cases, {len(cases) * 2} native callee entries, '
          f'{len(cases)} uninstrumented cases, and 2 target rejections passed')


if __name__ == '__main__':
    main()
