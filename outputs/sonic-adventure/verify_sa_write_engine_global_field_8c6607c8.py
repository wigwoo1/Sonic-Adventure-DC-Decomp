"""Compare the global field writer with retail SH-4 execution."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ADDRESS, LENGTH = 0x8c6607c8, 12
BASE, RAM_SIZE = 0x8c010000, 16 * 1024 * 1024
BASE_LITERAL, OFFSET_LITERAL, VALUE_LITERAL = 0x8c6607e4, 0x8c6607d6, 0x8c6607f0


def put32(ram, address, value):
    struct.pack_into('<I', ram, address & 0xffffff, value & 0xffffffff)


def main():
    project = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    original = bytearray(b'\xa5') * RAM_SIZE
    original[BASE & 0xffffff:(BASE & 0xffffff) + len(binary)] = binary
    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/sa_write_engine_global_field_8c6607c8.dll'))
    function = library.sa_write_engine_global_field_8c6607c8
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None
    rng = random.Random(0x8c6607c8)
    cases = []
    global_cell = struct.unpack_from('<I', original, BASE_LITERAL & 0xffffff)[0]
    offset = struct.unpack_from('<h', original, OFFSET_LITERAL & 0xffffff)[0]

    for case in range(256):
        alias = case % 8
        if alias == 0:
            destination = 0x8c700000 + case * 0x20
        elif alias == 1:
            destination = global_cell
        elif alias == 2:
            destination = BASE_LITERAL
        elif alias == 3:
            destination = OFFSET_LITERAL
        elif alias == 4:
            destination = VALUE_LITERAL
        elif alias == 5:
            destination = 0x8c800000 + case * 0x20
        else:
            destination = 0x8c730000 + rng.randrange(0, 0x1000) * 4

        initial = bytearray(original)
        put32(initial, global_cell, (destination - offset) & 0xffffffff)
        retail_ram, native_ram = bytearray(initial), bytearray(initial)
        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.sr = rng.getrandbits(32)
        expected.pr = 0x8c00e000 + case * 4
        expected.pc = ADDRESS
        actual = State.from_buffer_copy(bytes(expected))
        steps = reference(retail_ram, expected)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
        function(ram_view, ctypes.byref(actual))
        assert steps == 6, (case, steps)
        assert native_ram == retail_ram, f'RAM mismatch case {case}, destination={destination:#x}'
        assert bytes(actual) == bytes(expected), (
            f'CPU mismatch case {case}: '
            f'native={[hex(v) for v in actual.r[:4]]} '
            f'retail={[hex(v) for v in expected.r[:4]]}')
        cases.append({'case': case, 'destination': hex(destination),
                      'alias_fixture': alias, 'retail_steps': steps,
                      'full_ram_and_cpu_match': True})

    source = project / 'source/sa_write_engine_global_field_8c6607c8.c'
    report = {
        'module': '1ST_READ.BIN', 'address': hex(ADDRESS),
        'end_exclusive': hex(ADDRESS + LENGTH),
        'name': 'sa_write_engine_global_field_8c6607c8',
        'retail_span_sha256': hashlib.sha256(binary[ADDRESS - BASE:ADDRESS - BASE + LENGTH]).hexdigest(),
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cases_passed': len(cases),
        'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state.',
        'scope': '256 randomized CPU fixtures across normal destinations and aliases with the global pointer cell and literal data.',
        'limitations': ['Verification uses the project SH-4 interpreter; callers and console hardware are not covered.'],
        'cases': cases,
    }
    (project / 'analysis/sa_write_engine_global_field_8c6607c8_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"Engine global field writer passed {len(cases)} complete RAM/CPU comparisons")


if __name__ == '__main__':
    main()
