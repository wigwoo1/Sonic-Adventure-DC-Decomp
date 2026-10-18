"""Exhaustively compare two STG09 conditional byte-clear leaf routines."""
import ctypes
import hashlib
import json
import struct

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
LOAD, RAM_BASE, RAM_SIZE = 0x0C900000, 0x0C000000, 16 * 1024 * 1024
MODULE = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
CASES = {
    0x0C903A60: ('sa_stg09_flagged_byte_clear_0c903a60', 8),
    0x0C903A6E: ('sa_stg09_flagged_byte_clear_0c903a6e', 9),
}
library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/stg09_flagged_byte_clear_0c903a60.dll'))

def offset(address):
    value = (address & 0x1FFFFFFF) - RAM_BASE
    assert 0 <= value < RAM_SIZE - 64, hex(address)
    return value

results = {}
for address, (name, clear_offset) in CASES.items():
    native = getattr(library, name)
    native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    native.restype = None
    records = []
    for case, byte_value in enumerate(range(256)):
        ram = bytearray([0xA5]) * RAM_SIZE
        ram[LOAD - RAM_BASE:LOAD - RAM_BASE + len(MODULE)] = MODULE
        base = 0x0C200000 + case * 0x100
        ram[offset(base + (0 if address == 0x0C903A60 else 1))] = byte_value
        ram[offset(base + clear_offset)] = 0x5A
        initial = State()
        initial.r[:] = [((0x91370000 + i * 0x10203 + case * 0x1001) & 0xFFFFFFFF)
                        for i in range(16)]
        initial.r[4] = base
        initial.r[0] = (0xDEADBEEF ^ (case * 0x01010101)) & 0xFFFFFFFF
        initial.r[15] = 0x0CF00000 + case * 0x100
        initial.sr = (0xA5A50000 | ((case * 37) & 0xFFFF)) & 0xFFFFFFFF
        initial.pr = 0x0CFE0000 + case * 4
        initial.pc = address
        expected_ram, actual_ram = bytearray(ram), bytearray(ram)
        expected, actual = State.from_buffer_copy(initial), State.from_buffer_copy(initial)
        steps = reference(expected_ram, expected, instruction_budget=32)
        actual_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        native(actual_view, ctypes.byref(actual))
        assert bytes(actual) == bytes(expected), (
            f'{name}: CPU mismatch for input byte {byte_value:#04x}: '
            f'native={[hex(v) for v in actual.r] + [hex(actual.sr),hex(actual.pr),hex(actual.pc)]}; '
            f'retail={[hex(v) for v in expected.r] + [hex(expected.sr),hex(expected.pr),hex(expected.pc)]}')
        assert actual_ram == expected_ram, f'{name}: RAM mismatch for input byte {byte_value:#04x}'
        records.append({'input_byte': byte_value, 'cleared': byte_value & 3 == 0,
                        'retail_steps': steps, 'full_ram_cpu_match': True})
    start = address - LOAD
    span = MODULE[start:start + 14]
    source = PROJECT / 'source/sa_stg09_flagged_byte_clear_0c903a60.c'
    results[name] = {
        'module': 'STG09.BIN', 'load_address': hex(LOAD), 'address': hex(address),
        'end_exclusive': hex(address + 14),
        'retail_span_sha256': hashlib.sha256(span).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cases_passed': len(records),
        'branch_counts': {'clear': sum(row['cleared'] for row in records),
                          'preserve': sum(not row['cleared'] for row in records)},
        'comparison_level': 'Complete 16 MiB RAM and complete GPR/SR/PR/PC comparison for every possible input byte.',
        'scope': f'{name}: signed byte load, low-two-bit test, conditional byte clear at parameter offset +{clear_offset}, and RTS delay slot.',
        'limitations': [], 'cases': records,
    }
    path = PROJECT / f"analysis/common-overlays/STG09.BIN/{name}_verification.json"
    path.write_text(json.dumps(results[name], indent=2) + '\n', encoding='utf-8')
    print(f'{name}: 256 exhaustive full RAM/CPU comparisons passed')
