"""Compare the recovered input-device wait/reset routine against retail SH-4."""
import ctypes
import hashlib
import json
import struct

from verify_fpu_cleanup_transfer import Fpu
from verify_startup import State, ROOT, reference
from verify_tikal_controller import Call, P

PROJECT = ROOT / 'outputs/sonic-adventure'
BASE = 0x8C000000
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C65F2A4
RECORD = 0x8CA20000
OBJECT = 0x8CA10000
MMIO_ADDRESS = 0xFFD8000C
TIME = 0x8C60260E
ELAPSED = 0x8C602618

library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/input_device_wait_reset_8c65f2a4.dll'))
NATIVE = library.sa_input_device_wait_reset_8c65f2a4
NATIVE.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(Fpu), P, P, P,
                   P, P, ctypes.c_void_p, Call]
NATIVE.restype = ctypes.c_int


def put32(ram, address, value):
    struct.pack_into('<I', ram, address & 0xFFFFFF, value & 0xFFFFFFFF)


CASES = [
    {'name': 'state clear immediately', 'initial': 0, 'times': [100, 200], 'actions': {}},
    {'name': 'first wait timeout', 'initial': 1, 'times': [100, 100101, 200], 'actions': {}},
    {'name': 'first wait signal then second timeout', 'initial': 1,
     'times': [100, 101, 200, 200 + 0x4269],
     'actions': {2: 'clear', 3: 'set'}},
    {'name': 'second wait signal release', 'initial': 0,
     'times': [100, 200, 300, 301], 'actions': {2: 'set', 3: 'clear'}},
]


def run_case(case_number, case):
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram = bytearray(RAM_SIZE)
    ram[0x10000:0x10000 + len(binary)] = binary
    put32(ram, 0x8C8A83B4, OBJECT)
    put32(ram, OBJECT + 0x10F4, RECORD)
    put32(ram, RECORD + 0x04, 0x11112222)
    put32(ram, RECORD + 0x10, 0x33334444)
    put32(ram, RECORD + 0x14, 0x55556666)
    put32(ram, RECORD + 0x18, case['initial'])
    put32(ram, RECORD + 0x80, 0x77778888)
    put32(ram, RECORD + 0x8C, 0x9999AAAA)
    put32(ram, 0x8C60271C, MMIO_ADDRESS)

    initial = State()
    initial.r[:] = [((case_number + 1) * 0x13579BDF + i * 0x1020304) & 0xFFFFFFFF
                    for i in range(16)]
    initial.r[14] = 0x8CE10000 + case_number * 16
    initial.r[15] = 0x8CF00000
    initial.sr = 0x70000062 | (case_number & 1)
    initial.pr = 0x8C000100
    initial.pc = FUNCTION
    initial_fpu = Fpu()
    initial_fpu.fpscr = 0x200000 if case_number & 1 else 0
    initial_mac = 0xA1B2C3D4 ^ case_number
    expected_ram = bytearray(ram)
    expected = State.from_buffer_copy(initial)
    expected_fpu = Fpu.from_buffer_copy(initial_fpu)
    expected_events = []

    def effect(target, memory, regs, pr, cursor):
        event = (target, tuple(regs[4:8]), regs[15], pr)
        if target == TIME:
            index = cursor['clock']
            if index >= len(case['times']):
                raise AssertionError(f"{case['name']}: unexpected clock read {index + 1}")
            value = case['times'][index] & 0xFFFFFFFF
            cursor['clock'] += 1
            regs[2] = struct.unpack_from('<I', memory, 0x60271C)[0]
            regs[3] = (~value) & 0xFFFFFFFF
            regs[0] = value
            action = case['actions'].get(cursor['clock'])
            if action == 'clear':
                put32(memory, RECORD + 0x18, 0)
            elif action == 'set':
                put32(memory, RECORD + 0x18, 1)
        elif target == ELAPSED:
            regs[0] = (regs[5] - regs[4]) & 0xFFFFFFFF
        else:
            raise AssertionError(f'Unexpected helper {target:#x}')
        return event

    expected_cursor = {'clock': 0}
    def make_hook(target):
        def hook(memory, regs, sr):
            expected_events.append(effect(target, memory, regs, expected.pr,
                                          expected_cursor))
            return sr
        return hook

    reference(expected_ram, expected,
              hooks={TIME: make_hook(TIME), ELAPSED: make_hook(ELAPSED)},
              fpu=expected_fpu, special={'macl': initial_mac})

    native_ram = bytearray(ram)
    native = State.from_buffer_copy(initial)
    native_fpu = Fpu.from_buffer_copy(initial_fpu)
    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
    sr = ctypes.c_uint32(initial.sr)
    pr = ctypes.c_uint32(initial.pr)
    fpul = ctypes.c_uint32(0xCAFEBABE)
    macl = ctypes.c_uint32(initial_mac)
    native_events = []
    native_cursor = {'clock': 0}
    errors = []

    @Call
    def dispatch(_context, target, _memory, _fpu, _sr, regs, pr_ptr):
        try:
            native_events.append(effect(target, native_ram,
                                        [regs[i] for i in range(16)], pr_ptr[0],
                                        native_cursor))
            if target == TIME:
                value = case['times'][native_cursor['clock'] - 1] & 0xFFFFFFFF
                regs[2] = struct.unpack_from('<I', native_ram, 0x60271C)[0]
                regs[3] = (~value) & 0xFFFFFFFF
                regs[0] = value
            elif target == ELAPSED:
                regs[0] = (regs[5] - regs[4]) & 0xFFFFFFFF
            return 0
        except Exception as exc:
            errors.append(str(exc))
            return -9

    result = NATIVE(view, ctypes.byref(native_fpu), ctypes.byref(sr), native.r,
                    ctypes.byref(pr), ctypes.byref(fpul), ctypes.byref(macl),
                    None, dispatch)
    assert result == 0, (case['name'], result, errors)
    assert not errors, (case['name'], errors)
    assert native_ram == expected_ram, case['name'] + ': complete RAM mismatch'
    assert list(native.r) == list(expected.r), (
        case['name'], [(i, hex(native.r[i]), hex(expected.r[i]))
                       for i in range(16) if native.r[i] != expected.r[i]])
    assert sr.value == expected.sr, (case['name'], hex(sr.value), hex(expected.sr))
    assert pr.value == expected.pr
    assert bytes(native_fpu) == bytes(expected_fpu)
    assert fpul.value == 0xCAFEBABE and macl.value == initial_mac
    assert native_events == expected_events, (case['name'], native_events, expected_events)
    return {'name': case['name'], 'clock_reads': native_cursor['clock'],
            'calls': [hex(event[0]) for event in native_events],
            'full_ram_gpr_sr_pr_fpu_match': True}


def main():
    cases = [run_case(i, case) for i, case in enumerate(CASES)]
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    source = PROJECT / 'source/sa_input_device_wait_reset_8c65f2a4.c'
    start, end = FUNCTION, 0x8C65F348
    span = binary[start - 0x8C010000:end - 0x8C010000]
    report = {
        'module': '1ST_READ.BIN', 'load_address': hex(0x8C010000),
        'address': hex(start), 'end_exclusive': hex(end),
        'indexed_instruction_bytes': 164,
        'retail_span_sha256': hashlib.sha256(span).hexdigest(),
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cases_passed': len(cases),
        'comparison_level': 'Complete 16 MiB RAM, all GPR/SR/PR/FPU state and helper call traces compared against retail SH-4 interpretation.',
        'scope': 'Recovers both status-bit waits and signed timeout tests, the device-record reset writes/constants, stack saves/restores, and helper-call register effects.',
        'limitations': [
            'Clock-read and elapsed-time helper behavior is provided by deterministic fixtures; each helper has separate recovery/verification evidence.',
            'The asynchronous status transition is simulated by the fixture callback; no physical device or complete game session is run.'
        ],
        'cases': cases,
    }
    (PROJECT / 'analysis/input_device_wait_reset_8c65f2a4_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'{len(cases)} input-device wait/reset retail comparisons passed')


if __name__ == '__main__':
    main()
