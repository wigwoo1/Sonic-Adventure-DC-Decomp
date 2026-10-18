"""Check two indexed indirect dispatchers including their delay slots."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

BASE, RAM_SIZE = 0x8c010000, 16 * 1024 * 1024
TABLE_POINTER = 0x8c10cd50
TABLE_BASE = 0x8c10cd98
ROUTINES = [
    (0x8c10cd38, 'sa_indirect_indexed_dispatch_8c10cd38', 0),
    (0x8c10cd44, 'sa_indirect_indexed_dispatch_8c10cd44', 3),
]


def put32(ram, address, value):
    struct.pack_into('<I', ram, address & 0xffffff, value & 0xffffffff)


def main():
    project = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    original = bytearray(b'\xa5') * RAM_SIZE
    original[BASE & 0xffffff:(BASE & 0xffffff) + len(binary)] = binary
    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/sa_indirect_indexed_dispatch_8c10cd38.dll'))
    functions = {}
    for _, name, _ in ROUTINES:
        fn = getattr(library, name)
        fn.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
        fn.restype = None
        functions[name] = fn
    rng = random.Random(0x8c10cd38)
    records = []

    for entry, name, delay_destination in ROUTINES:
        cases = []
        for index in range(128):
            case = len(records) * 128 + index
            table_index = rng.randrange(16) * 4
            target = 0x8c00e000 + case * 4
            return_pc = 0x8c00f000 + case * 4
            delay_address = 0x8c700000 + case * 0x100 + 0x40
            initial = bytearray(original)
            put32(initial, TABLE_POINTER, TABLE_BASE)
            put32(initial, TABLE_BASE + table_index, target)
            put32(initial, delay_address, rng.getrandbits(32))

            state = State()
            state.r[:] = [rng.getrandbits(32) for _ in range(16)]
            state.r[0] = table_index
            state.r[2] = (delay_address - ((table_index - 4) & 0xffffffff)) & 0xffffffff
            state.r[15] = 0x8c800000 + case * 0x100 + 0x40
            state.sr = rng.getrandbits(32)
            state.pr = return_pc
            state.pc = entry

            # Every sixteenth case aliases the stack push with the table row;
            # every next case aliases the delay-slot read with the table row;
            # the third case aliases the stack write and delay-slot read.
            alias = index % 16
            if alias == 0:
                state.r[15] = TABLE_BASE + table_index + 4
            elif alias == 1:
                delay_address = TABLE_BASE + table_index
                state.r[2] = (delay_address - ((table_index - 4) & 0xffffffff)) & 0xffffffff
            elif alias == 2:
                state.r[15] = delay_address + 4

            expected_target = state.r[3] if alias == 0 else target
            retail_ram, native_ram = bytearray(initial), bytearray(initial)
            expected = State.from_buffer_copy(bytes(state))
            actual = State.from_buffer_copy(bytes(state))
            observed = []

            def external_target(_ram, regs, sr):
                observed.append((list(regs), sr, expected.pr))
                return sr

            steps = reference(retail_ram, expected,
                              hooks={expected_target: external_target})
            ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
            functions[name](ram_view, ctypes.byref(actual))
            assert steps == 6, (name, index, steps)
            assert len(observed) == 1, (name, index, observed)
            expected_regs, expected_sr, expected_pr = observed[0]
            assert native_ram == retail_ram, f'{name} RAM mismatch case {index}'
            assert list(actual.r) == expected_regs, (
                f'{name} register mismatch case {index}: '
                f'{[hex(v) for v in actual.r]} != {[hex(v) for v in expected_regs]}')
            assert (actual.sr, actual.pr, actual.pc) == (
                expected_sr, expected_pr, expected_target), f'{name} control state mismatch case {index}'
            cases.append({
                'case': index, 'table_offset': hex(table_index),
                'target': hex(expected_target), 'alias_fixture': alias,
                'retail_steps': steps, 'full_ram_and_tail_state_match': True,
            })

        records.append({
            'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'name': name,
            'address': hex(entry), 'end_exclusive': hex(entry + 12),
            'retail_span_sha256': hashlib.sha256(
                binary[entry - BASE:entry - BASE + 12]).hexdigest(),
            'cases_passed': len(cases), 'cases': cases,
            'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state at the indexed tail-transfer boundary.',
            'limitations': ['The selected function-table targets are external to these dispatcher routines.'],
        })

    source = project / 'source/sa_indirect_indexed_dispatch_8c10cd38.c'
    report = {
        'module': '1ST_READ.BIN',
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cases_passed': sum(record['cases_passed'] for record in records),
        'scope': 'Two indexed dispatchers, 128 fixtures each, with table, stack, and delay-slot alias coverage.',
        'routines': records,
    }
    (project / 'analysis/sa_indirect_indexed_dispatch_8c10cd38_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"Two indexed dispatchers passed {report['cases_passed']} full RAM/CPU comparisons")


if __name__ == '__main__':
    main()
