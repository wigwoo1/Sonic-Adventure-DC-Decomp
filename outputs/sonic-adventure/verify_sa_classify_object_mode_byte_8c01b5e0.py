"""Compare the object mode byte classifier against retail SH-4 execution."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ADDRESS, LENGTH = 0x8c01b5e0, 38
BASE, RAM_SIZE = 0x8c010000, 16 * 1024 * 1024
LITERAL_SLOT = 0x8c01b5fc


def expected_steps(value):
    if 5 <= value <= 26:
        return 16 if (value & 1) or value == 26 else 17
    return 11


def main():
    project = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    original = bytearray(b'\xa5') * RAM_SIZE
    image_offset = BASE & 0x00ffffff
    original[image_offset:image_offset + len(binary)] = binary
    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/sa_classify_object_mode_byte_8c01b5e0.dll'))
    function = library.sa_classify_object_mode_byte_8c01b5e0
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None
    rng = random.Random(0x8c01b5e0)
    cases = []

    for case in range(512):
        value = case & 0xff
        global_cell = 0x8c720000 + case * 0x10
        object_address = 0x8c760000 + case * 0x40
        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.sr = rng.getrandbits(32)
        expected.pr = 0x8c00d000 + case * 4
        expected.pc = ADDRESS
        actual = State.from_buffer_copy(bytes(expected))
        retail_ram, actual_ram = bytearray(original), bytearray(original)
        for memory in (retail_ram, actual_ram):
            struct.pack_into('<I', memory, LITERAL_SLOT & 0xffffff, global_cell)
            struct.pack_into('<I', memory, global_cell & 0xffffff, object_address)
            memory[(object_address + 9) & 0xffffff] = value

        steps = reference(retail_ram, expected)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        function(ram_view, ctypes.byref(actual))
        assert steps == expected_steps(value), (case, value, steps)
        assert actual_ram == retail_ram, f'RAM mismatch for byte {value:#x}'
        assert bytes(actual) == bytes(expected), (
            f'CPU mismatch byte={value:#x}: '
            f'r0={actual.r[0]:#x}/{expected.r[0]:#x}, '
            f'r1={actual.r[1]:#x}/{expected.r[1]:#x}, '
            f'r4={actual.r[4]:#x}/{expected.r[4]:#x}, '
            f'sr={actual.sr:#x}/{expected.sr:#x}')
        cases.append({'case': case, 'input_byte': hex(value),
                      'result': hex(expected.r[0]), 'table_offset': hex(expected.r[1]),
                      'retail_steps': steps, 'full_state_and_ram_match': True})

    source = project / 'source/sa_classify_object_mode_byte_8c01b5e0.c'
    report = {
        'module': '1ST_READ.BIN', 'address': hex(ADDRESS),
        'end_exclusive': hex(ADDRESS + LENGTH), 'name': 'sa_classify_object_mode_byte_8c01b5e0',
        'retail_span_sha256': hashlib.sha256(binary[ADDRESS - BASE:ADDRESS - BASE + LENGTH]).hexdigest(),
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cases_passed': len(cases),
        'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state.',
        'scope': 'All 256 input bytes tested twice through distinct literal-to-global-to-object pointer chains.',
        'limitations': ['Verification uses the project SH-4 interpreter; callers and hardware execution are not covered.'],
        'cases': cases,
    }
    (project / 'analysis/sa_classify_object_mode_byte_8c01b5e0_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"Object mode byte classifier passed {len(cases)} full RAM/CPU comparisons")


if __name__ == '__main__':
    main()
