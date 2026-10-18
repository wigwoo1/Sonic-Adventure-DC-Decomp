"""Compare formatter text-emission helper against retail SH-4."""
import ctypes
import hashlib
import itertools
import json
import struct

from verify_fpu_cleanup_transfer import Fpu
from verify_startup import ROOT, State, reference
from verify_tikal_controller import Call, P

PROJECT = ROOT / 'outputs/sonic-adventure'
FUNCTION = 0x8C10E50E
BYTE_MOVE = 0x8C10D25C
RAM_SIZE = 16 * 1024 * 1024
library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/format_emit_text_8c10e50e.dll'))
NATIVE = library.sa_format_emit_text_8c10e50e
NATIVE.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(Fpu), P, P, P,
                   P, P, ctypes.c_void_p, Call]
NATIVE.restype = ctypes.c_int


def put32(ram, address, value):
    struct.pack_into('<I', ram, address & 0xFFFFFF, value & 0xFFFFFFFF)


def scenario_list():
    values = []
    for mode, length, stack_offset in itertools.product((0, 1, 2), (0, 1, 5), (0, 2)):
        fail_options = (None,) if mode == 1 else (None, 1, 3)
        for fail_on in fail_options:
            values.append((mode, length, stack_offset, fail_on))
    return values


def run_case(number, mode, length, stack_offset, fail_on):
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram = bytearray(RAM_SIZE)
    ram[0x10000:0x10000 + len(binary)] = binary
    context = 0x8CA10000 + number * 0x40
    output = 0x8CA30000 + number * 0x40
    source = 0x8CA40000 + number * 0x40
    initial_count = 0x20 + number
    put32(ram, context, mode)
    put32(ram, context + 4, output)
    put32(ram, context + 8, initial_count)
    put32(ram, context + 12, 0x8C600400)
    payload = bytes(((number * 31 + i * 73) & 0xFF) for i in range(max(length, 1)))
    ram[source & 0xFFFFFF:(source & 0xFFFFFF) + len(payload)] = payload
    ram[output & 0xFFFFFF:(output & 0xFFFFFF) + 16] = bytes([0xA5]) * 16

    initial = State()
    initial.r[:] = [((number + 11) * 0x13579BDF + i * 0x01020304) & 0xFFFFFFFF
                    for i in range(16)]
    initial.r[4] = source
    initial.r[5] = length
    initial.r[6] = context
    initial.r[14] = 0x8CE10000 + number * 16
    initial.r[15] = 0x8CF00000 - stack_offset
    initial.sr = 0x70000062 | (number & 1)
    initial.pr = 0x8C000200 + number * 4
    initial.pc = FUNCTION
    initial_fpu = Fpu()
    initial_fpu.fpscr = 0x200000 if number & 1 else 0
    initial_mac = 0xA1B2C3D4 ^ number
    expected_ram = bytearray(ram)
    expected = State.from_buffer_copy(initial)
    expected_fpu = Fpu.from_buffer_copy(initial_fpu)
    expected_events = []
    expected_calls = {'bytes': 0}

    def output_byte_hook(memory, regs, sr):
        expected_calls['bytes'] += 1
        expected_events.append((tuple(regs[4:8]), regs[15], expected.pr,
                                regs[4] & 0xFF, regs[5]))
        regs[0] = 0xFFFFFFFF if fail_on == expected_calls['bytes'] else 0
        return sr

    hooks = {0x8C600400: output_byte_hook}
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
    native_calls = {'bytes': 0}
    errors = []

    @Call
    def dispatch(_context, target, _memory, _fpu, _sr, regs, pr_ptr):
        if target != 0x8C600400:
            errors.append(hex(target))
            return -9
        native_calls['bytes'] += 1
        native_events.append((tuple(regs[i] for i in range(4, 8)), regs[15],
                              pr_ptr[0], regs[4] & 0xFF, regs[5]))
        regs[0] = 0xFFFFFFFF if fail_on == native_calls['bytes'] else 0
        return 0

    result = NATIVE(view, ctypes.byref(native_fpu), ctypes.byref(sr), native.r,
                    ctypes.byref(pr), ctypes.byref(fpul), ctypes.byref(macl),
                    None, dispatch)
    assert result == 0 and not errors, (result, errors)
    assert native_ram == expected_ram, (
        mode, length, fail_on,
        next((hex(i), actual, wanted) for i, (actual, wanted)
             in enumerate(zip(native_ram, expected_ram)) if actual != wanted),
        hex(initial.pr), hex(initial.r[15]),
        native_ram[0xEFFFE8:0xEFFFF8].hex(), expected_ram[0xEFFFE8:0xEFFFF8].hex())
    assert list(native.r) == list(expected.r), (
        mode, length, fail_on,
        [(i, hex(native.r[i]), hex(expected.r[i]))
         for i in range(16) if native.r[i] != expected.r[i]])
    assert sr.value == expected.sr and pr.value == expected.pr
    assert bytes(native_fpu) == bytes(expected_fpu)
    assert fpul.value == 0xCAFEBABE and macl.value == initial_mac
    assert native_events == expected_events, (native_events, expected_events)
    assert native_calls['bytes'] == expected_calls['bytes']
    return {'case': number, 'mode': mode, 'length': length,
            'callback_fail_on': fail_on, 'per_byte_calls': native_calls['bytes'],
            'bulk_copy_used': mode == 1,
            'full_ram_cpu_fpu_and_callback_trace_match': True}


def main():
    cases = [run_case(i, *args) for i, args in enumerate(scenario_list())]
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    source = PROJECT / 'source/sa_format_emit_text_8c10e50e.c'
    end = 0x8C10E566
    span = binary[FUNCTION - 0x8C010000:end - 0x8C010000]
    report = {
        'module': '1ST_READ.BIN', 'load_address': hex(0x8C010000),
        'address': hex(FUNCTION), 'end_exclusive': hex(end),
        'indexed_instruction_bytes': 88,
        'retail_span_sha256': hashlib.sha256(span).hexdigest(),
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cases_passed': len(cases),
        'comparison_level': 'Complete 16 MiB RAM, all GPR/SR/PR/FPU state, linked bulk-copy behavior, and per-byte callback arguments/order compared against retail SH-4 interpretation.',
        'scope': 'Recovers bulk and per-character output modes, length/status branches, callback failure exit, output/count advancement, stack saves/restores, and SR T-bit behavior.',
        'limitations': [
            'The per-character output callback is a synthetic sink; physical output-device behavior is not exercised.',
            'The recovered bulk byte-move helper is linked natively; the larger formatter caller remains unrecovered.'
        ],
        'cases': cases,
    }
    (PROJECT / 'analysis/format_emit_text_8c10e50e_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'{len(cases)} formatter text-emission retail comparisons passed')


if __name__ == '__main__':
    main()
