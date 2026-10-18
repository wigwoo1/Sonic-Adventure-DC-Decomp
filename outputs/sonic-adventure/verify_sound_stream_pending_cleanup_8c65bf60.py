"""Native cleanup caller, native timer/subtraction, external retail abort service."""
import ctypes
import hashlib
import json
import random
import struct
from pathlib import Path

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
ENTRY, END = 0x8C65BF60, 0x8C65BFA4
CLOCK, ABORT, SUBTRACT = 0x8C60260E, 0x8C6013D2, 0x8C602618
MMIO = 0xFFD8000C
RAM_SIZE = 16 * 1024 * 1024
SOURCES = ['sa_sound_stream_pending_cleanup_8c65bf60.c',
           'sa_mmio_complement_sub_8c60260e.c', 'retail_scalar_helpers.c']
binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll = ROOT / 'work/sonic-adventure/build/sound_stream_pending_cleanup_8c65bf60.dll'
library = ctypes.CDLL(str(dll))
P = ctypes.POINTER(ctypes.c_uint8)
Read = ctypes.CFUNCTYPE(ctypes.c_uint32, ctypes.c_void_p, ctypes.c_uint32)
Abort = ctypes.CFUNCTYPE(ctypes.c_int, P, ctypes.POINTER(State), ctypes.c_void_p)
Observe = ctypes.CFUNCTYPE(None, P, ctypes.POINTER(State))
native = library.sa_sound_stream_pending_cleanup_8c65bf60
native.argtypes = [P, ctypes.POINTER(State), ctypes.c_void_p, Read, Abort, Observe]
native.restype = ctypes.c_int


class Timer(dict):
    def __init__(self, samples):
        super().__init__({MMIO: 0})
        self.samples, self.reads = samples, []

    def __getitem__(self, address):
        assert address == MMIO and len(self.reads) < len(self.samples)
        value = self.samples[len(self.reads)]
        self.reads.append((address, value))
        return value


def put32(ram, address, value):
    struct.pack_into('<I', ram, address & 0xFFFFFF, value & 0xFFFFFFFF)


def snapshot(ram, state):
    return (bytes(state), hashlib.sha256(ram).hexdigest())


def main():
    rng = random.Random(ENTRY)
    cases = []
    for case in range(96):
        alias = 0x8C000000 if case % 2 == 0 else 0xAC000000
        record, request, driver = alias | 0x780000, alias | 0x790000, alias | 0x7A0000
        ram = bytearray(RAM_SIZE)
        ram[0x10000:0x10000 + len(binary)] = binary
        ram[0x780000:0x780064] = rng.randbytes(100)
        flag = [1, 1, 1, 0, 2, 0xFF][case % 6]
        null_request = case % 12 == 0
        ram[0x780002] = flag
        put32(ram, record + 8, 0 if null_request else request)
        put32(ram, request, driver)
        # Modes 2/4 exercise driver-owner rejection without entering device waits.
        mode = [0, 1, 2, 3, 4, 5, 0xFFFF][(case // 3) % 7]
        active = [0, 1, 0xFFFF][(case // 6) % 3]
        struct.pack_into('<H', ram, (request + 0x4C) & 0xFFFFFF, active)
        struct.pack_into('<H', ram, (request + 0x50) & 0xFFFFFF, mode)
        put32(ram, driver + 0x1C, 0)  # Deliberately not this request.
        put32(ram, 0x8C67C1F0, rng.getrandbits(32))
        # Guard both the caller's reserved stack word and nested service spills.
        ram[0x7FEF00:0x7FF000] = rng.randbytes(256)
        state = State()
        state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        state.r[4], state.r[15] = record, alias | 0x7FF000
        state.sr, state.pr, state.pc = rng.getrandbits(32), 0x8C74913C, ENTRY
        samples = [(0, 0xFFFFFFFF), (0xFFFFFFFF, 0), (0x80000000, 0x7FFFFFFF),
                   (rng.getrandbits(32), rng.getrandbits(32))][case % 4]
        expected_ram, expected = bytearray(ram), State.from_buffer_copy(state)
        expected_entries = []
        expected_timer = Timer(samples)

        def trace(pc, word, registers, sr, pr, fpu):
            if pc in (CLOCK, ABORT, SUBTRACT):
                entry = State()
                entry.r[:] = registers
                entry.sr, entry.pr, entry.pc = sr, pr, pc
                expected_entries.append(snapshot(expected_ram, entry))

        reference(expected_ram, expected, mmio=expected_timer, trace=trace, instruction_budget=100000)
        actual_ram, actual = bytearray(ram), State.from_buffer_copy(state)
        actual_entries, errors = [], []
        actual_timer = Timer(samples)

        @Read
        def read_register(context, address):
            try:
                return actual_timer[address]
            except Exception as error:
                errors.append(repr(error))
                return 0

        @Abort
        def abort_request(ram_pointer, state_pointer, context):
            try:
                memory = (ctypes.c_uint8 * RAM_SIZE).from_address(ctypes.addressof(ram_pointer.contents))
                reference(memory, state_pointer.contents, mmio=actual_timer, instruction_budget=100000)
                return 0
            except Exception as error:
                errors.append(repr(error))
                return -9

        @Observe
        def observe(ram_pointer, state_pointer):
            memory = (ctypes.c_uint8 * RAM_SIZE).from_address(ctypes.addressof(ram_pointer.contents))
            actual_entries.append(snapshot(memory, state_pointer.contents))

        view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        assert native(view, ctypes.byref(actual), None, read_register, abort_request, observe) == 0
        assert not errors, (case, errors)
        assert actual_entries == expected_entries, f'case {case}: service entry differs'
        assert actual_timer.reads == expected_timer.reads, f'case {case}: timer trace differs'
        assert actual_ram == expected_ram, f'case {case}: complete RAM differs'
        assert bytes(actual) == bytes(expected), f'case {case}: complete CPU differs'
        invoked = flag == 1 and not null_request
        assert len(expected_entries) == (4 if invoked else 0)
        assert len(expected_timer.reads) == (2 if invoked else 0)
        elapsed = struct.unpack_from('<I', actual_ram, 0x67C1F0)[0]
        if invoked:
            assert elapsed == (samples[0] - samples[1]) & 0xFFFFFFFF
            assert actual_ram[0x780002] == 0
        cases.append({'case': case, 'pending_flag': flag, 'null_request': null_request,
                      'request_active': active, 'request_mode': mode,
                      'service_entries': len(actual_entries), 'timer_reads': len(actual_timer.reads),
                      'elapsed_word': hex(elapsed), 'matched': True})

    # Unsupported targets and missing required services reject before mutation.
    rejection_cases = 0
    for slot in (0x8C65BFF0, 0x8C65BFF4, 0x8C65BFF8, None):
        rejected_ram = bytearray(ram)
        rejected_ram[0x780002] = 1
        put32(rejected_ram, record + 8, request)
        if slot is not None:
            put32(rejected_ram, slot, 0x8C700000)
        rejected = State.from_buffer_copy(state)
        before = bytes(rejected_ram)
        view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(rejected_ram)
        result = native(view, ctypes.byref(rejected), None, Read(), Abort(), Observe())
        assert result == (-1 if slot is not None else -2)
        assert bytes(rejected) == bytes(state) and bytes(rejected_ram) == before
        rejection_cases += 1

    report = {
        'module': '1ST_READ.BIN', 'load_address': '0x8c010000',
        'address': hex(ENTRY), 'end_exclusive': hex(END), 'indexed_instruction_bytes': END - ENTRY,
        'retail_span_sha256': hashlib.sha256(binary[ENTRY-0x8C010000:END-0x8C010000]).hexdigest(),
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256((PROJECT / 'source' / SOURCES[0]).read_bytes()).hexdigest(),
        'sources': {f'source/{source}': hashlib.sha256((PROJECT / 'source' / source).read_bytes()).hexdigest()
                    for source in SOURCES},
        'dll_sha256': hashlib.sha256(dll.read_bytes()).hexdigest(),
        'interpreter_sha256': hashlib.sha256((PROJECT / 'verify_startup.py').read_bytes()).hexdigest(),
        'verifier_sha256': hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
        'cases_passed': len(cases),
        'service_entries_compared': sum(c['service_entries'] for c in cases),
        'timer_reads_compared': sum(c['timer_reads'] for c in cases),
        'host_rejections_passed': rejection_cases,
        'comparison_level': 'Full 16 MiB RAM and GPR/SR/PR/PC after return and at every service entry, plus exact MMIO timer reads.',
        'scope': 'Pending-I/O cleanup caller with native timer and subtraction, external abort dependency, elapsed-word wraparound, skipped-work paths, and stack traffic.',
        'limitations': [
            'Request-abort service 0x8c6013d2 remains external and is interpreted in this verifier; tests cover inactive/nonwaiting modes and driver-owner rejection for modes 2/4.',
            'Abort device-wait, timeout, matching-driver, and hardware I/O paths remain unverified by these scenarios.',
            'Only the original three literal targets are dispatched; unsupported targets or missing required services reject atomically with a separate host error.',
            'This is interpreter differential evidence, not hardware, complete sound-system, or whole-game execution.'
        ],
        'cases': cases,
    }
    (PROJECT / 'analysis/sound_stream_pending_cleanup_8c65bf60_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'{len(cases)} cleanup cases, {report["service_entries_compared"]} service entries, '
          f'{report["timer_reads_compared"]} timer reads, and {rejection_cases} host rejections passed')


if __name__ == '__main__':
    main()
