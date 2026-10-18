"""Compare the recovered mode/table updater with retail SH-4 execution."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ADDRESS, LENGTH = 0x8c01b3e0, 38
BASE, RAM_SIZE = 0x8c010000, 16 * 1024 * 1024
FIRST_SLOT, TABLE_SLOT, SECOND_SLOT = 0x8c01b4e0, 0x8c01b4e4, 0x8c01b4e8


def put32(ram, address, value):
    struct.pack_into('<I', ram, address & 0xffffff, value & 0xffffffff)


def main():
    project = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    original = bytearray(b'\xa5') * RAM_SIZE
    original[BASE & 0xffffff:(BASE & 0xffffff) + len(binary)] = binary
    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/sa_update_object_mode_and_table_8c01b3e0.dll'))
    function = library.sa_update_object_mode_and_table_8c01b3e0
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None
    rng = random.Random(0x8c01b3e0)
    passed = []

    # Every byte is tested with a signed-index table entry. Additional cases
    # vary pointer aliasing and upper input bits to exercise literal ordering.
    cases = [(value, False) for value in range(256)]
    cases += [(rng.randrange(256), True) for _ in range(256)]
    for case, (byte_value, alias_case) in enumerate(cases):
        first_cell = 0x8c720000 + case * 0x20
        table_cell = 0x8c730000 + case * 0x20
        second_cell = first_cell if alias_case and case % 2 == 0 else 0x8c740000 + case * 0x20
        first_object = 0x8c780000 + case * 0x40
        second_object = first_object if alias_case and case % 2 else 0x8c790000 + case * 0x40
        table_base = 0x8c900000 + case * 0x400
        state = State()
        state.r[:] = [rng.getrandbits(32) for _ in range(16)]
        state.r[4] = (rng.getrandbits(24) << 8) | byte_value
        state.sr = rng.getrandbits(32)
        state.pr = 0x8c00d000 + case * 4
        state.pc = ADDRESS
        expected = State.from_buffer_copy(bytes(state))
        initial = bytearray(original)
        for memory in (initial,):
            put32(memory, FIRST_SLOT, first_cell)
            put32(memory, TABLE_SLOT, table_cell)
            put32(memory, SECOND_SLOT, second_cell)
            put32(memory, first_cell, first_object)
            put32(memory, table_cell, table_base)
            put32(memory, second_cell, second_object)
            memory[(first_object + 9) & 0xffffff] = (byte_value ^ 0x5a) & 0xff
            for idx in range(-128, 128):
                put32(memory, table_base + idx * 8 + 4,
                      rng.getrandbits(32))
        retail_ram, native_ram = bytearray(initial), bytearray(initial)
        steps = reference(retail_ram, expected)
        native = State.from_buffer_copy(bytes(state))
        view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
        function(view, ctypes.byref(native))
        assert steps == 19, (case, steps)
        assert native_ram == retail_ram, f'RAM differs in case {case}, byte={byte_value:#x}'
        assert list(native.r) == list(expected.r) and (native.sr, native.pr, native.pc) == (expected.sr, expected.pr, expected.pc), (
            f'CPU differs in case {case}, byte={byte_value:#x}: '
            f'native={[hex(x) for x in native.r]} sr={native.sr:#x} pr={native.pr:#x} pc={native.pc:#x}, '
            f'retail={[hex(x) for x in expected.r]} sr={expected.sr:#x} pr={expected.pr:#x} pc={expected.pc:#x}')
        passed.append({'case': case, 'mode_byte': hex(byte_value),
                       'alias_fixture': alias_case, 'retail_steps': steps,
                       'full_state_and_ram_match': True})

    source = project / 'source/sa_update_object_mode_and_table_8c01b3e0.c'
    report = {
        'module': '1ST_READ.BIN', 'address': hex(ADDRESS),
        'end_exclusive': hex(ADDRESS + LENGTH),
        'name': 'sa_update_object_mode_and_table_8c01b3e0',
        'retail_span_sha256': hashlib.sha256(binary[ADDRESS - BASE:ADDRESS - BASE + LENGTH]).hexdigest(),
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cases_passed': len(passed),
        'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state.',
        'scope': 'All mode byte values and randomized fixtures, including signed table indexes, distinct pointer chains, and object alias cases.',
        'limitations': ['Verification uses the project SH-4 interpreter; callers and hardware execution are not covered.'],
        'cases': passed,
    }
    (project / 'analysis/sa_update_object_mode_and_table_8c01b3e0_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"Object mode/table helper passed {len(passed)} full RAM/CPU comparisons")


if __name__ == '__main__':
    main()
