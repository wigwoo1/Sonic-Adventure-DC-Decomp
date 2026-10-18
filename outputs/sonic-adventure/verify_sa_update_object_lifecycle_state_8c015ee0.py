"""Compare the object lifecycle state helper against retail SH-4 execution."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

ADDRESS, LENGTH = 0x8c015ee0, 60
BASE, RAM_SIZE = 0x8c010000, 16 * 1024 * 1024


def expected_steps(state, flags, timer):
    signed_state = state if state < 128 else state - 256
    if signed_state != 0:
        return 7 if signed_state == 1 else 9
    if flags & 0x100:
        return 15 if flags & 0x40 else 18
    return 19 if timer > 600 else 17


def main():
    project = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    original = bytearray(b'\xa5') * RAM_SIZE
    image_offset = BASE & 0x00ffffff
    original[image_offset:image_offset + len(binary)] = binary
    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/sa_update_object_lifecycle_state_8c015ee0.dll'))
    function = library.sa_update_object_lifecycle_state_8c015ee0
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None

    rng = random.Random(0x8c015ee0)
    cases_to_run = []
    # Exercise every possible byte state.
    for state in range(256):
        cases_to_run.append((state, rng.choice([0, 0x40, 0x100, 0x140, 0x300]),
                             rng.choice([-32768, -1, 0, 599, 600, 601, 32767])))
    # Exhaust the controlling flag combinations at the signed timer boundaries.
    for flags in (0, 0x40, 0x100, 0x140, 0x200, 0x240, 0xffff):
        for timer in (-32768, -1, 0, 599, 600, 601, 32767):
            cases_to_run.append((0, flags, timer))
    while len(cases_to_run) < 384:
        cases_to_run.append((0, rng.getrandbits(16), rng.randint(-32768, 32767)))

    cases = []
    step_counts = {}
    for case, (state_byte, flags, timer) in enumerate(cases_to_run):
        object_address = 0x8c700000 + (case % 64) * 0x40
        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.r[5] = object_address
        expected.sr = rng.getrandbits(32)
        expected.pr = 0x8c00c000 + case * 4
        expected.pc = ADDRESS
        actual = State.from_buffer_copy(bytes(expected))
        retail_ram, actual_ram = bytearray(original), bytearray(original)
        at = object_address & 0xffffff
        for memory in (retail_ram, actual_ram):
            memory[at] = state_byte
            struct.pack_into('<H', memory, at + 6, flags)
            struct.pack_into('<h', memory, at + 12, timer)

        steps = reference(retail_ram, expected)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        function(ram_view, ctypes.byref(actual))
        want_steps = expected_steps(state_byte, flags, timer)
        assert steps == want_steps, (case, state_byte, flags, timer, steps, want_steps)
        step_counts[str(steps)] = step_counts.get(str(steps), 0) + 1
        assert actual_ram == retail_ram, f'RAM mismatch in case {case}'
        assert bytes(actual) == bytes(expected), (
            f'CPU mismatch state={state_byte:#x} flags={flags:#x} timer={timer}: '
            f'reg_differences={[(i, hex(actual.r[i]), hex(expected.r[i])) for i in range(16) if actual.r[i] != expected.r[i]]}, '
            f'sr={actual.sr:#x}/{expected.sr:#x}')
        cases.append({'case': case, 'initial_state_byte': hex(state_byte),
                      'flags': hex(flags), 'signed_timer': timer,
                      'result_state_byte': hex(retail_ram[at]), 'retail_steps': steps,
                      'full_state_and_ram_match': True})

    source = project / 'source/sa_update_object_lifecycle_state_8c015ee0.c'
    report = {
        'module': '1ST_READ.BIN', 'address': hex(ADDRESS),
        'end_exclusive': hex(ADDRESS + LENGTH), 'name': 'sa_update_object_lifecycle_state_8c015ee0',
        'retail_span_sha256': hashlib.sha256(binary[ADDRESS - BASE:ADDRESS - BASE + LENGTH]).hexdigest(),
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cases_passed': len(cases), 'retail_instruction_counts': step_counts,
        'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state.',
        'scope': 'All 256 possible state bytes, controlling flag combinations, signed timer boundaries, and randomized state/flag/timer fixtures compared against retail.',
        'limitations': ['Verification uses the project SH-4 interpreter; callers and hardware execution are not covered.'],
        'cases': cases,
    }
    (project / 'analysis/sa_update_object_lifecycle_state_8c015ee0_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"Object lifecycle state helper passed {len(cases)} full RAM/CPU comparisons")


if __name__ == '__main__':
    main()
