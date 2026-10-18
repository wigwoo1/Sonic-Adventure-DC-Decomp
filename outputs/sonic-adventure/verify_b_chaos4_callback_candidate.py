"""Verify the B_CHAOS4 task wrapper and its 1ST_READ.BIN slot callback."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

RAM_BASE, RAM_SIZE = 0x0c000000, 16 * 1024 * 1024
MAIN_BASE, OVERLAY_BASE = 0x8c010000, 0x0c900000
WRAPPER_ADDRESS, WRAPPER_END = 0x0c90701a, 0x0c907030
CALLBACK_ADDRESS, CALLBACK_END = 0x8c052a5e, 0x8c052a8a
OUTPUT_POINTER_GLOBAL, INPUT_BASE_GLOBAL = 0x8c052b60, 0x8c052b64
STACK_GUARD, CASE_COUNT = 0x2000, 128


def offset(address):
    return (address & 0x1fffffff) - RAM_BASE


def compare_ram(retail, native, stack_pointer, case, label):
    stack_start = stack_pointer - RAM_BASE - STACK_GUARD
    stack_end = stack_pointer - RAM_BASE + STACK_GUARD
    assert retail[:stack_start] == native[:stack_start], f'{label}: RAM before stack differs in case {case}'
    assert retail[stack_end:] == native[stack_end:], f'{label}: RAM after stack differs in case {case}'


def compare_abi(expected, actual, case, label):
    assert list(actual.r[8:15]) == list(expected.r[8:15]), f'{label}: callee-saved registers differ in case {case}'
    assert actual.r[15] == expected.r[15], f'{label}: SP differs in case {case}'
    assert actual.pr == expected.pr and actual.pc == expected.pc, f'{label}: return boundary differs in case {case}'


def main():
    out = ROOT / 'outputs/sonic-adventure'
    overlay = (ROOT / 'work/sonic-adventure/decoded/SONICADV/B_CHAOS4.BIN').read_bytes()
    main_binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    wrapper_overlay_offset = WRAPPER_ADDRESS - OVERLAY_BASE
    wrapper_length = WRAPPER_END - WRAPPER_ADDRESS
    assert len(overlay[wrapper_overlay_offset:wrapper_overlay_offset + wrapper_length]) == wrapper_length
    assert int.from_bytes(overlay[0x706c:0x7070], 'little') == CALLBACK_ADDRESS
    callback_file_offset = CALLBACK_ADDRESS - MAIN_BASE
    callback_length = CALLBACK_END - CALLBACK_ADDRESS
    assert len(main_binary[callback_file_offset:callback_file_offset + callback_length]) == callback_length

    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/b_chaos4_callback_candidate.dll'))
    wrapper = library.sa_b_chaos4_callback_candidate_c90701a
    wrapper.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    wrapper.restype = None
    callback = library.sa_callback_candidate_8c052a5e
    callback.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint32]
    callback.restype = None

    original = bytearray(b'\xa5') * RAM_SIZE
    main_offset = offset(MAIN_BASE)
    original[main_offset:main_offset + len(main_binary)] = main_binary
    overlay_offset = offset(OVERLAY_BASE)
    original[overlay_offset:overlay_offset + len(overlay)] = overlay

    rng = random.Random(0x4348414f53340101)
    wrapper_cases, callback_cases = [], []
    for case in range(CASE_COUNT):
        baseline = bytearray(original)
        input_base = 0x0c500000 + case * 0x4000
        output_table = 0x0c700000 + case * 0x1000
        task = 0x0c300000 + case * 0x40
        struct.pack_into('<I', baseline, offset(OUTPUT_POINTER_GLOBAL), output_table)
        struct.pack_into('<I', baseline, offset(INPUT_BASE_GLOBAL), input_base)
        for index in range(256):
            baseline[offset(output_table + index)] = rng.randrange(1, 256)

        if case % 5 == 0:
            value = 0xdead0000 + case
            matched_index = None
        else:
            matched_index = (0, 1, 127, 255)[case % 4]
            value = (input_base + matched_index * 0x34) & 0xffffffff
        struct.pack_into('<I', baseline, offset(task + 0x2c), value)

        # Verify the callback body directly against its retail SH-4 implementation.
        retail_callback_ram, native_callback_ram = bytearray(baseline), bytearray(baseline)
        callback_state = State()
        callback_state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        callback_state.r[4] = value
        callback_state.r[15] = 0x0cff0000
        callback_state.pr = 0x0c001000 + case * 4
        callback_state.pc = CALLBACK_ADDRESS
        expected_callback = State.from_buffer_copy(bytes(callback_state))
        callback_steps = reference(retail_callback_ram, expected_callback)
        native_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_callback_ram)
        callback(native_view, value)
        compare_ram(retail_callback_ram, native_callback_ram, callback_state.r[15], case, 'callback')

        # Verify the overlay wrapper plus callback as a composed operation.
        retail_wrapper_ram, native_wrapper_ram = bytearray(baseline), bytearray(baseline)
        initial = State()
        initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
        initial.r[4] = task
        initial.r[15] = 0x0cff0000
        initial.sr = rng.getrandbits(32)
        initial.pr = 0x0c002000 + case * 4
        initial.pc = WRAPPER_ADDRESS
        expected = State.from_buffer_copy(bytes(initial))
        actual = State.from_buffer_copy(bytes(initial))
        steps = reference(retail_wrapper_ram, expected)
        native_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_wrapper_ram)
        wrapper(native_view, ctypes.byref(actual))
        compare_ram(retail_wrapper_ram, native_wrapper_ram, initial.r[15], case, 'wrapper')
        compare_abi(expected, actual, case, 'wrapper')
        assert struct.unpack_from('<I', native_wrapper_ram, offset(task + 0x2c))[0] == 0
        if matched_index is not None:
            assert native_wrapper_ram[offset(output_table + matched_index)] == 0
        else:
            assert retail_wrapper_ram[offset(output_table):offset(output_table) + 256] == native_wrapper_ram[offset(output_table):offset(output_table) + 256]
        callback_cases.append({'case': case, 'matched_index': matched_index,
                               'retail_instructions': callback_steps, 'ram_and_abi_match': True})
        wrapper_cases.append({'case': case, 'matched_index': matched_index,
                              'retail_instructions': steps, 'ram_and_abi_match': True})

    source = out / 'source/b_chaos4_callback_candidate.c'
    report = {
        'functions_verified': 2,
        'total_abi_cases': CASE_COUNT,
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'comparison_level': f'All RAM compared except +/-{STACK_GUARD:#x} caller-stack scratch; preserved R8-R14, SP, PR and return PC compared.',
        'scope': 'Verifies the B_CHAOS4 task callback wrapper and the called 1ST_READ.BIN 256-slot lookup/clear callback independently and when composed.',
        'limitations': ['Uses the project partial SH-4 interpreter, not Dreamcast hardware or end-to-end gameplay.',
                        'The 256-slot input/output arrays and globals are synthetic RAM fixtures; retail code and instruction spans are unchanged.'],
        'dependencies': [
            {'module': 'B_CHAOS4.BIN', 'load_address': hex(OVERLAY_BASE),
             'sha256': hashlib.sha256(overlay).hexdigest(),
             'callback_pointer_address': hex(0x0c90706c), 'callback_target': hex(CALLBACK_ADDRESS)},
            {'module': '1ST_READ.BIN', 'load_address': hex(MAIN_BASE),
             'sha256': hashlib.sha256(main_binary).hexdigest(),
             'callback_start': hex(CALLBACK_ADDRESS), 'callback_end_exclusive': hex(CALLBACK_END),
             'output_pointer_global': hex(OUTPUT_POINTER_GLOBAL),
             'input_base_global': hex(INPUT_BASE_GLOBAL)},
        ],
        'routines': [
            {'module': 'B_CHAOS4.BIN', 'load_address': hex(OVERLAY_BASE),
             'address': hex(WRAPPER_ADDRESS), 'end_exclusive': hex(WRAPPER_END),
             'name': 'task_callback_candidate', 'cases_passed': len(wrapper_cases),
             'comparison_level': f'All RAM except +/-{STACK_GUARD:#x} caller-stack scratch; preserved R8-R14, SP, PR and return PC compared.',
             'retail_span_sha256': hashlib.sha256(overlay[wrapper_overlay_offset:wrapper_overlay_offset + wrapper_length]).hexdigest(),
             'cases': wrapper_cases},
            {'module': '1ST_READ.BIN', 'load_address': hex(MAIN_BASE),
             'address': hex(CALLBACK_ADDRESS), 'end_exclusive': hex(CALLBACK_END),
             'name': 'callback_candidate', 'cases_passed': len(callback_cases),
             'comparison_level': f'All RAM except +/-{STACK_GUARD:#x} caller-stack scratch; direct helper CPU-state ABI is not compared.',
             'retail_span_sha256': hashlib.sha256(main_binary[callback_file_offset:callback_file_offset + callback_length]).hexdigest(),
             'cases': callback_cases},
        ],
    }
    report_path = out / 'analysis/common-overlays/B_CHAOS4.BIN/task_callback_candidate_verification.json'
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'B_CHAOS4 callback wrapper and main callback passed {CASE_COUNT} composed and direct cases each')


if __name__ == '__main__':
    main()
