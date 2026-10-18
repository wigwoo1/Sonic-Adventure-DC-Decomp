"""Compare the index boundary predicate against retail SH-4 execution."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ADDRESS, LENGTH = 0x8c021868, 28
BASE, RAM_SIZE = 0x8c010000, 16 * 1024 * 1024


def main():
    project = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    original = bytearray(b'\xa5') * RAM_SIZE
    image_offset = BASE & 0x00ffffff
    original[image_offset:image_offset + len(binary)] = binary
    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/sa_index_before_table_tail_8c021868.dll'))
    function = library.sa_index_before_table_tail_8c021868
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None

    rng = random.Random(0x8c021868)
    cases_to_run = []
    for count in (0, 1, 2, 3, 4, 5, 0x7fff, 0x8000, 0xfffd, 0xfffe, 0xffff):
        limit = (count - 3) & 0xffffffff
        signed_limit = limit if limit < 0x80000000 else limit - 0x100000000
        values = {0, 1, 2, signed_limit - 1, signed_limit, signed_limit + 1,
                  0x7fffffff, 0x80000000, 0xffffffff}
        cases_to_run.extend((count, value & 0xffffffff) for value in sorted(values))
    while len(cases_to_run) < 160:
        cases_to_run.append((rng.randrange(0x10000), rng.getrandbits(32)))

    cases = []
    step_counts = {}
    for case, (count, value) in enumerate(cases_to_run):
        table_object = 0x8c700000 + (case % 32) * 0x40
        count_record = 0x8c710000 + (case % 32) * 0x40
        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.r[4] = value
        expected.r[5] = rng.getrandbits(32)
        expected.r[6] = table_object
        expected.sr = rng.getrandbits(32)
        expected.pr = 0x8c00e000 + case * 4
        expected.pc = ADDRESS
        actual = State.from_buffer_copy(bytes(expected))
        retail_ram, actual_ram = bytearray(original), bytearray(original)
        struct.pack_into('<I', retail_ram, (table_object + 4) & 0xffffff, count_record)
        struct.pack_into('<I', actual_ram, (table_object + 4) & 0xffffff, count_record)
        struct.pack_into('<H', retail_ram, (count_record + 2) & 0xffffff, count)
        struct.pack_into('<H', actual_ram, (count_record + 2) & 0xffffff, count)

        steps = reference(retail_ram, expected)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        function(ram_view, ctypes.byref(actual))
        assert steps in (9, 11, 12), (case, steps)
        step_counts[str(steps)] = step_counts.get(str(steps), 0) + 1
        assert actual_ram == retail_ram, f'RAM mismatch in case {case}'
        assert bytes(actual) == bytes(expected), (
            f'CPU mismatch count={count:#x} index={value:#x}: '
            f'r0={actual.r[0]:#x}/{expected.r[0]:#x}, '
            f'r5={actual.r[5]:#x}/{expected.r[5]:#x}, '
            f'sr={actual.sr:#x}/{expected.sr:#x}')
        cases.append({'case': case, 'table_count': hex(count), 'index': hex(value),
                      'result': hex(expected.r[0]), 'retail_steps': steps,
                      'full_state_and_ram_match': True})

    source = project / 'source/sa_index_before_table_tail_8c021868.c'
    report = {
        'module': '1ST_READ.BIN', 'address': hex(ADDRESS),
        'end_exclusive': hex(ADDRESS + LENGTH), 'name': 'sa_index_before_table_tail_8c021868',
        'retail_span_sha256': hashlib.sha256(binary[ADDRESS - BASE:ADDRESS - BASE + LENGTH]).hexdigest(),
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cases_passed': len(cases), 'retail_instruction_counts': step_counts,
        'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state.',
        'scope': 'Signed index boundaries, zero special case, unsigned 16-bit count boundaries, and randomized pairs compared against retail.',
        'limitations': ['Verification uses the project SH-4 interpreter; callers and hardware execution are not covered.'],
        'cases': cases,
    }
    (project / 'analysis/sa_index_before_table_tail_8c021868_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"Index boundary predicate passed {len(cases)} full RAM/CPU comparisons")


if __name__ == '__main__':
    main()
