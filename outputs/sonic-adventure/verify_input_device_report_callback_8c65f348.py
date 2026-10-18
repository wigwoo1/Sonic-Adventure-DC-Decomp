"""Compare the recovered input-device report callback against retail SH-4."""
import ctypes
import hashlib
import json
import struct

from verify_fpu_cleanup_transfer import Fpu
from verify_startup import ROOT, State, reference
from verify_tikal_controller import Call, P

PROJECT = ROOT / 'outputs/sonic-adventure'
FUNCTION = 0x8C65F348
OBJECT = 0x8CA10000
REPORT = 0x8C8A71B8
CALLBACK_CELL = 0x8C67C268
REPORT_SERVICE = 0x8C10DBC4
OPTIONAL_CALLBACK = 0x8C600200
RAM_SIZE = 16 * 1024 * 1024

library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/input_device_report_callback_8c65f348.dll'))
NATIVE = library.sa_input_device_report_callback_8c65f348
NATIVE.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(Fpu), P, P, P,
                   P, P, ctypes.c_void_p, Call]
NATIVE.restype = ctypes.c_int

CASES = [
    {'name': 'report condition clear', 'condition': 0, 'callback': 0, 'stack_offset': 0},
    {'name': 'report with no optional callback', 'condition': 1, 'callback': 0, 'stack_offset': 0},
    {'name': 'report and optional callback', 'condition': 1,
     'callback': OPTIONAL_CALLBACK, 'stack_offset': 0},
    {'name': 'condition clear despite callback', 'condition': 0,
     'callback': OPTIONAL_CALLBACK, 'stack_offset': 0},
    {'name': 'alternate stack alignment', 'condition': 1,
     'callback': OPTIONAL_CALLBACK, 'stack_offset': 2},
]


def put32(ram, address, value):
    struct.pack_into('<I', ram, address & 0xFFFFFF, value & 0xFFFFFFFF)


def run_case(case_number, case):
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram = bytearray(RAM_SIZE)
    ram[0x10000:0x10000 + len(binary)] = binary
    put32(ram, 0x8C8A83B4, OBJECT)
    put32(ram, OBJECT + 0x10C8, case['condition'])
    put32(ram, REPORT, 0xAABBCCDD)
    put32(ram, CALLBACK_CELL, case['callback'])

    initial = State()
    initial.r[:] = [((case_number + 3) * 0x2468ACE1 + i * 0x1020304) & 0xFFFFFFFF
                    for i in range(16)]
    initial.r[14] = 0x8CE10000 + case_number * 16
    initial.r[15] = 0x8CF00000 - case['stack_offset']
    initial.sr = 0x70000062 | (case_number & 1)
    initial.pr = 0x8C000100
    initial.pc = FUNCTION
    put32(ram, initial.r[15], 0x12345678 + case_number)
    initial_fpu = Fpu()
    initial_fpu.fpscr = 0x200000 if case_number & 1 else 0
    initial_mac = 0xA1B2C3D4 ^ case_number

    expected_ram = bytearray(ram)
    expected = State.from_buffer_copy(initial)
    expected_fpu = Fpu.from_buffer_copy(initial_fpu)
    expected_events = []

    def hook_for(target):
        def hook(memory, regs, sr):
            expected_events.append((target, tuple(regs[4:8]), regs[15], expected.pr,
                                    struct.unpack_from('<I', memory,
                                                       (regs[15] + 8) & 0xFFFFFF)[0]))
            return sr
        return hook

    hooks = {REPORT_SERVICE: hook_for(REPORT_SERVICE)}
    if case['callback']:
        hooks[case['callback']] = hook_for(case['callback'])
    reference(expected_ram, expected, hooks=hooks, fpu=expected_fpu,
              special={'macl': initial_mac})

    native_ram = bytearray(ram)
    native = State.from_buffer_copy(initial)
    native_fpu = Fpu.from_buffer_copy(initial_fpu)
    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
    sr = ctypes.c_uint32(initial.sr)
    pr = ctypes.c_uint32(initial.pr)
    fpul = ctypes.c_uint32(0xCAFEBABE)
    macl = ctypes.c_uint32(initial_mac)
    native_events = []
    errors = []

    @Call
    def dispatch(_context, target, memory, _fpu, _sr, regs, pr_ptr):
        target &= 0xFFFFFFFF
        native_events.append((target, tuple(regs[i] for i in range(4, 8)), regs[15],
                              pr_ptr[0], struct.unpack_from('<I', native_ram,
                                                            (regs[15] + 8) & 0xFFFFFF)[0]))
        if target not in (REPORT_SERVICE, OPTIONAL_CALLBACK):
            errors.append(hex(target))
            return -9
        return 0

    result = NATIVE(view, ctypes.byref(native_fpu), ctypes.byref(sr), native.r,
                    ctypes.byref(pr), ctypes.byref(fpul), ctypes.byref(macl), None, dispatch)
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
    return {'name': case['name'], 'calls': [hex(event[0]) for event in native_events],
            'full_ram_gpr_sr_pr_fpu_and_call_trace_match': True}


def main():
    cases = [run_case(i, case) for i, case in enumerate(CASES)]
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    source = PROJECT / 'source/sa_input_device_report_callback_8c65f348.c'
    start, end = FUNCTION, 0x8C65F390
    span = binary[start - 0x8C010000:end - 0x8C010000]
    report = {
        'module': '1ST_READ.BIN', 'load_address': hex(0x8C010000),
        'address': hex(start), 'end_exclusive': hex(end),
        'indexed_instruction_bytes': 72,
        'retail_span_sha256': hashlib.sha256(span).hexdigest(),
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cases_passed': len(cases),
        'comparison_level': 'Complete 16 MiB RAM, all GPR/SR/PR/FPU state and external call arguments/order compared against retail SH-4 interpretation.',
        'scope': 'Recovers the report-condition gate, stack-alignment selection, report-service call, optional callback, and saved-register/PR effects.',
        'limitations': [
            'The wrapper at 0x8c10d400 is recovered and executed; its downstream formatter at 0x8c10dbc4 and the optional runtime callback are explicit no-effect fixtures.',
            'This verifies the callback wrapper and stack ABI in synthetic object state, not report rendering or complete game behavior.'
        ],
        'cases': cases,
    }
    (PROJECT / 'analysis/input_device_report_callback_8c65f348_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'{len(cases)} input-device report-callback retail comparisons passed')


if __name__ == '__main__':
    main()
