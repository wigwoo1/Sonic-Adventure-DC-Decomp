"""Compare the debug-log forwarding wrapper against retail SH-4."""
import ctypes
import hashlib
import json
import struct

from verify_fpu_cleanup_transfer import Fpu
from verify_startup import ROOT, State, reference
from verify_tikal_controller import Call, P

PROJECT = ROOT / 'outputs/sonic-adventure'
FUNCTION = 0x8C10D400
FORMATTER = 0x8C10DBC4
RAM_SIZE = 16 * 1024 * 1024
library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/debug_log_forward_8c10d400.dll'))
NATIVE = library.sa_debug_log_forward_8c10d400
NATIVE.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(Fpu), P, P, P,
                   P, P, ctypes.c_void_p, Call]
NATIVE.restype = ctypes.c_int


def run_case(case_number, stack_offset):
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram = bytearray(RAM_SIZE)
    ram[0x10000:0x10000 + len(binary)] = binary
    initial = State()
    initial.r[:] = [((case_number + 5) * 0x13579BDF + i * 0x1020304) & 0xFFFFFFFF
                    for i in range(16)]
    initial.r[4:7] = [0x8CA10000 + case_number * 0x20,
                      0x11223344 ^ case_number,
                      0x8CA20000 + case_number * 0x40]
    initial.r[14] = 0x8CE10000 + case_number * 16
    initial.r[15] = 0x8CF00000 - stack_offset
    initial.sr = 0x70000062 | (case_number & 1)
    initial.pr = 0x8C000100 + case_number * 4
    initial.pc = FUNCTION
    initial_fpu = Fpu()
    initial_fpu.fpscr = 0x200000 if case_number & 1 else 0
    initial_mac = 0xA1B2C3D4 ^ case_number
    expected_ram = bytearray(ram)
    expected = State.from_buffer_copy(initial)
    expected_fpu = Fpu.from_buffer_copy(initial_fpu)
    expected_events = []

    def formatter_hook(memory, regs, sr):
        stack = tuple(struct.unpack_from('<I', memory,
                                         (regs[15] + offset) & 0xFFFFFF)[0]
                      for offset in (0, 4, 8, 12))
        expected_events.append((tuple(regs[4:8]), regs[15], expected.pr, stack))
        return sr

    reference(expected_ram, expected, hooks={FORMATTER: formatter_hook},
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
    errors = []

    @Call
    def dispatch(_context, target, _memory, _fpu, _sr, regs, pr_ptr):
        if target != FORMATTER:
            errors.append(hex(target))
            return -9
        stack = tuple(struct.unpack_from('<I', native_ram,
                                         (regs[15] + offset) & 0xFFFFFF)[0]
                      for offset in (0, 4, 8, 12))
        native_events.append((tuple(regs[i] for i in range(4, 8)), regs[15],
                              pr_ptr[0], stack))
        return 0

    result = NATIVE(view, ctypes.byref(native_fpu), ctypes.byref(sr), native.r,
                    ctypes.byref(pr), ctypes.byref(fpul), ctypes.byref(macl),
                    None, dispatch)
    assert result == 0 and not errors, (result, errors)
    assert native_ram == expected_ram, f'RAM mismatch in case {case_number}'
    assert list(native.r) == list(expected.r), (
        case_number, [(i, hex(native.r[i]), hex(expected.r[i]))
                      for i in range(16) if native.r[i] != expected.r[i]])
    assert sr.value == expected.sr and pr.value == expected.pr
    assert bytes(native_fpu) == bytes(expected_fpu)
    assert fpul.value == 0xCAFEBABE and macl.value == initial_mac
    assert native_events == expected_events, (native_events, expected_events)
    return {'case': case_number, 'stack_offset': stack_offset,
            'formatter_arguments': [hex(value) for value in native_events[0][0]],
            'stack_words': [hex(value) for value in native_events[0][3]],
            'full_state_and_call_trace_match': True}


def main():
    cases = [run_case(i, offset) for i, offset in enumerate((0, 2, 4, 8, 12, 16))]
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    source = PROJECT / 'source/sa_debug_log_forward_8c10d400.c'
    end = 0x8C10D422
    span = binary[FUNCTION - 0x8C010000:end - 0x8C010000]
    report = {
        'module': '1ST_READ.BIN', 'load_address': hex(0x8C010000),
        'address': hex(FUNCTION), 'end_exclusive': hex(end),
        'indexed_instruction_bytes': 34,
        'retail_span_sha256': hashlib.sha256(span).hexdigest(),
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cases_passed': len(cases),
        'comparison_level': 'Complete 16 MiB RAM, all GPR/SR/PR/FPU state and downstream formatter arguments/stack values compared against retail SH-4 interpretation.',
        'scope': 'Recovers the wrapper stack frame, fixed mode arguments, forwarding of saved register arguments, formatter call target, and PR/stack restoration.',
        'limitations': [
            'The downstream formatter at 0x8c10dbc4 is an explicit no-effect fixture and is not recovered by this routine.',
            'The runtime function pointer is read from its retail literal cell; the actual formatter behavior is outside this check.'
        ],
        'cases': cases,
    }
    (PROJECT / 'analysis/debug_log_forward_8c10d400_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'{len(cases)} debug-log wrapper retail comparisons passed')


if __name__ == '__main__':
    main()
