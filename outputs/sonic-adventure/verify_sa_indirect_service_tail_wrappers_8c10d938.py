"""Check seven indirect service tail wrappers against retail SH-4 execution."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

BASE, RAM_SIZE = 0x8c010000, 16 * 1024 * 1024
POINTER_CELL = 0x8c0000bc
ENTRIES = [0x8c10d938, 0x8c10d94c, 0x8c10d960, 0x8c10d974,
           0x8c10d988, 0x8c10d9b0, 0x8c10d9c4]
NAMES = [f'sa_indirect_service_tail_{entry:08x}' for entry in ENTRIES]


def put32(ram, address, value):
    struct.pack_into('<I', ram, address & 0xffffff, value & 0xffffffff)


def main():
    project = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    original = bytearray(b'\xa5') * RAM_SIZE
    original[BASE & 0xffffff:(BASE & 0xffffff) + len(binary)] = binary
    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/sa_indirect_service_tail_wrappers_8c10d938.dll'))
    functions = {}
    for name in NAMES:
        function = getattr(library, name)
        function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
        function.restype = None
        functions[name] = function
    rng = random.Random(0x8c10d938)
    results = []

    for entry, name in zip(ENTRIES, NAMES):
        cases = []
        for index in range(96):
            case = len(results) * 96 + index
            target = 0x8c00e000 + case * 4
            return_pc = 0x8c00f000 + case * 4
            retail_ram = bytearray(original)
            put32(retail_ram, POINTER_CELL, target)
            expected = State()
            expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
            expected.sr = rng.getrandbits(32)
            expected.pr = return_pc
            expected.pc = entry
            actual_ram = bytearray(retail_ram)
            actual = State.from_buffer_copy(bytes(expected))
            observed = []

            def external_service(_ram, regs, sr, _entry_pc=entry):
                observed.append((list(regs), sr, expected.pr, target, _entry_pc))
                return sr

            steps = reference(retail_ram, expected, hooks={target: external_service})
            native_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
            functions[name](native_view, ctypes.byref(actual))
            assert steps == 6, (name, index, steps)
            assert len(observed) == 1, (name, index, observed)
            expected_regs, expected_sr, expected_pr, expected_target, _ = observed[0]
            assert actual_ram == retail_ram, f'{name} RAM mismatch in case {index}'
            assert list(actual.r) == expected_regs, (
                f'{name} register mismatch in case {index}: '
                f'{[hex(v) for v in actual.r]} != {[hex(v) for v in expected_regs]}')
            assert actual.sr == expected_sr, (name, index, 'SR mismatch')
            assert actual.pr == expected_pr, (name, index, 'PR mismatch')
            assert actual.pc == expected_target, (name, index, 'tail target mismatch')
            cases.append({'case': index, 'target': hex(target), 'retail_steps': steps,
                          'full_ram_and_tail_state_match': True})

        start = entry - BASE
        length = 12
        results.append({
            'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'name': name,
            'address': hex(entry), 'end_exclusive': hex(entry + length),
            'retail_span_sha256': hashlib.sha256(binary[start:start + length]).hexdigest(),
            'cases_passed': len(cases),
            'comparison_level': 'Full 16 MiB RAM and complete GPR/SR/PR/PC state at the indirect tail-transfer boundary.',
            'limitations': ['The service reached by the retail function pointer remains an external caller/service boundary.'],
            'cases': cases,
        })

    source = project / 'source/sa_indirect_service_tail_wrappers_8c10d938.c'
    report = {
        'module': '1ST_READ.BIN', 'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cases_passed': sum(record['cases_passed'] for record in results),
        'comparison_level': 'Full 16 MiB RAM and complete GPR/SR/PR/PC state at the indirect tail-transfer boundary.',
        'scope': 'Seven wrappers, each tested with 96 randomized CPU fixtures and distinct indirect service targets.',
        'routines': results,
    }
    (project / 'analysis/sa_indirect_service_tail_wrappers_8c10d938_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"Seven indirect service tail wrappers passed {report['cases_passed']} full RAM/CPU comparisons")


if __name__ == '__main__':
    main()
