"""Compare both input flag helpers against retail SH-4 execution."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

BASE, RAM_SIZE = 0x8c010000, 16 * 1024 * 1024
SET_ADDRESS, CLEAR_ADDRESS, LENGTH = 0x8c0509b8, 0x8c0509da, 34
FIRST_SLOT, SECOND_SLOT = 0x8c050a24, 0x8c050a28


def main():
    project = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    original = bytearray(b'\xa5') * RAM_SIZE
    image_offset = BASE & 0x00ffffff
    original[image_offset:image_offset + len(binary)] = binary
    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/sa_input_flag_updates_8c0509b8.dll'))
    functions = {
        'set': (library.sa_set_input_flag_by_index_8c0509b8, SET_ADDRESS, 1),
        'clear': (library.sa_clear_input_flag_by_index_8c0509da, CLEAR_ADDRESS, 0),
    }
    for function, _, _ in functions.values():
        function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
        function.restype = None

    rng = random.Random(0x8c0509b8)
    routine_reports = []
    for label, (function, address, stored_value) in functions.items():
        cases = []
        step_counts = {}
        for case in range(256):
            index = case
            input_value = ((rng.getrandbits(24) << 8) | index) & 0xffffffff
            first_target = 0x8c740000 + case * 0x40
            second_target = 0x8c750000 + case * 0x40
            if case % 16 == 0:
                first_target = SECOND_SLOT
            elif case % 16 == 8:
                second_target = FIRST_SLOT
            elif case % 16 == 4:
                second_target = first_target

            expected = State()
            expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
            expected.r[4] = input_value
            expected.sr = rng.getrandbits(32)
            expected.pr = 0x8c00f800 + len(routine_reports) * 0x10000 + case * 4
            expected.pc = address
            actual = State.from_buffer_copy(bytes(expected))
            retail_ram, actual_ram = bytearray(original), bytearray(original)
            for memory in (retail_ram, actual_ram):
                struct.pack_into('<I', memory, FIRST_SLOT & 0xffffff, first_target)
                struct.pack_into('<I', memory, SECOND_SLOT & 0xffffff, second_target)

            steps = reference(retail_ram, expected)
            ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
            function(ram_view, ctypes.byref(actual))
            expected_steps = 10 if index == 0 else (13 if index == 1 else 12)
            assert steps == expected_steps, (label, case, steps, expected_steps)
            step_counts[str(steps)] = step_counts.get(str(steps), 0) + 1
            assert actual_ram == retail_ram, f'{label} RAM mismatch at input {index:#x}'
            assert bytes(actual) == bytes(expected), (
                f'{label} CPU mismatch input={input_value:#x}: '
                f'reg_differences={[(i, hex(actual.r[i]), hex(expected.r[i])) for i in range(16) if actual.r[i] != expected.r[i]]}, '
                f'r0={actual.r[0]:#x}/{expected.r[0]:#x}, '
                f'r3={actual.r[3]:#x}/{expected.r[3]:#x}, '
                f'r5={actual.r[5]:#x}/{expected.r[5]:#x}, '
                f'r6={actual.r[6]:#x}/{expected.r[6]:#x}, '
                f'r7={actual.r[7]:#x}/{expected.r[7]:#x}, '
                f'sr={actual.sr:#x}/{expected.sr:#x}')
            cases.append({'case': case, 'input': hex(input_value),
                          'first_target': hex(first_target), 'second_target': hex(second_target),
                          'result': hex(expected.r[0]), 'retail_steps': steps,
                          'full_state_and_ram_match': True})

        routine_reports.append({
            'module': '1ST_READ.BIN', 'address': hex(address),
            'end_exclusive': hex(address + LENGTH), 'name': f'sa_{label}_input_flag_by_index_{address:08x}',
            'retail_span_sha256': hashlib.sha256(binary[address - BASE:address - BASE + LENGTH]).hexdigest(),
            'cases_passed': len(cases), 'retail_instruction_counts': step_counts,
            'cases': cases,
        })

    source = project / 'source/sa_input_flag_updates_8c0509b8.c'
    report = {
        'module': '1ST_READ.BIN', 'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cases_passed': sum(item['cases_passed'] for item in routine_reports),
        'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state.',
        'scope': 'Both helpers tested for all 256 byte inputs with varied upper bits, randomized CPU state, and pointer targets that overlap either literal slot or each other.',
        'limitations': ['Verification uses the project SH-4 interpreter; callers and hardware execution are not covered.'],
        'routines': routine_reports,
    }
    (project / 'analysis/sa_input_flag_updates_8c0509b8_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print('Both input flag helpers passed 512 full RAM/CPU comparisons')


if __name__ == '__main__':
    main()
