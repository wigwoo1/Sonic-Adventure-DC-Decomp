"""Compare the global high-byte setter against retail SH-4 execution."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

ADDRESS, LENGTH = 0x8c042e20, 30
BASE, RAM_SIZE = 0x8c010000, 16 * 1024 * 1024
LITERAL_SLOT = 0x8c042f60


def main():
    project = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    original = bytearray(b'\xa5') * RAM_SIZE
    image_offset = BASE & 0x00ffffff
    original[image_offset:image_offset + len(binary)] = binary
    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/sa_set_global_byte_high_8c042e20.dll'))
    function = library.sa_set_global_byte_high_8c042e20
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None
    rng = random.Random(0x8c042e20)
    cases = []

    for case in range(256):
        byte_value = case
        input_value = ((rng.getrandbits(24) << 8) | byte_value) & 0xffffffff
        stack_pointer = 0x8c700000 + case * 0x100
        aliases_stack = case % 8 == 0
        target = stack_pointer - 4 if aliases_stack else 0x8c740000 + case * 4
        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.r[4] = input_value
        expected.r[15] = stack_pointer
        expected.sr = rng.getrandbits(32)
        expected.pr = 0x8c00f000 + case * 4
        expected.pc = ADDRESS
        actual = State.from_buffer_copy(bytes(expected))
        retail_ram, actual_ram = bytearray(original), bytearray(original)
        # Redirect the retail literal for controlled destinations, including stack alias cases.
        slot = LITERAL_SLOT & 0xffffff
        for memory in (retail_ram, actual_ram):
            memory[slot:slot + 4] = target.to_bytes(4, 'little')

        steps = reference(retail_ram, expected)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        function(ram_view, ctypes.byref(actual))
        assert steps == 15, (case, steps)
        assert actual_ram == retail_ram, f'RAM mismatch in case {case}'
        assert bytes(actual) == bytes(expected), (
            f'CPU mismatch input={input_value:#x}: '
            f'r0={actual.r[0]:#x}/{expected.r[0]:#x}, '
            f'r2={actual.r[2]:#x}/{expected.r[2]:#x}, '
            f'r3={actual.r[3]:#x}/{expected.r[3]:#x}, '
            f'r4={actual.r[4]:#x}/{expected.r[4]:#x}, '
            f'sr={actual.sr:#x}/{expected.sr:#x}')
        cases.append({'case': case, 'input': hex(input_value),
                      'destination': hex(target), 'destination_aliases_stack': aliases_stack,
                      'stored_value': hex(expected.r[3]), 'full_state_and_ram_match': True})

    source = project / 'source/sa_set_global_byte_high_8c042e20.c'
    report = {
        'module': '1ST_READ.BIN', 'address': hex(ADDRESS),
        'end_exclusive': hex(ADDRESS + LENGTH), 'name': 'sa_set_global_byte_high_8c042e20',
        'retail_span_sha256': hashlib.sha256(binary[ADDRESS - BASE:ADDRESS - BASE + LENGTH]).hexdigest(),
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cases_passed': len(cases),
        'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state.',
        'scope': 'Every possible low input byte tested with varied upper input bits, randomized CPU state, and destinations that either avoid or alias the stack temporary.',
        'limitations': ['Verification uses the project SH-4 interpreter; callers and hardware execution are not covered.'],
        'cases': cases,
    }
    (project / 'analysis/sa_set_global_byte_high_8c042e20_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"Global high-byte setter passed {len(cases)} full RAM/CPU comparisons")


if __name__ == '__main__':
    main()
