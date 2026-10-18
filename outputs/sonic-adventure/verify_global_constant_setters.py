"""Verify literal global-state setters against retail SH-4 execution."""
import ctypes
import hashlib
import json
import random
import struct
from pathlib import Path

from verify_startup import ROOT, State, reference

SPECS = [(0x8c0509a0, 8, 0x8c050a20, 2, 3, 1),
         (0x8c0509a8, 8, 0x8c050a20, 0, 1, 0),
         (0x8c08a07c, 8, 0x8c08a0f4, 0, 3, 4)]
BASE, RAM_SIZE = 0x8c010000, 16 * 1024 * 1024


def main():
    out = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    binary_sha256 = hashlib.sha256(binary).hexdigest()
    lib = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/global_constant_setters.dll'))
    original = bytearray(b'\xa5') * RAM_SIZE
    original[0x10000:0x10000 + len(binary)] = binary
    rng = random.Random(0x434f4e5354)
    results = []

    for address, size, literal, pointer_register, value_register, value in SPECS:
        function = getattr(lib, f'sa_global_constant_setter_{address:08x}')
        function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
        function.restype = None
        cases = []
        retail_pointer = struct.unpack_from('<I', binary, literal - BASE)[0]
        for case in range(128):
            mode = case % 4
            if mode == 0:
                target = retail_pointer
            elif mode == 1:
                target = literal
            elif mode == 2:
                target = 0x8c700000 + ((case * 131) & 0x000fffff)
            else:
                target = 0x8cffffff
            expected = State()
            expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
            expected.sr = rng.getrandbits(32)
            expected.pr = 0x8c000200 + case * 4
            expected.pc = address
            actual = State.from_buffer_copy(bytes(expected))
            retail_ram, actual_ram = bytearray(original), bytearray(original)
            target_slot = target & 0x00ffffff
            if target != literal:
                sentinel = rng.randrange(256)
                retail_ram[target_slot] = sentinel
                actual_ram[target_slot] = sentinel
            literal_slot = literal & 0x00ffffff
            struct.pack_into('<I', retail_ram, literal_slot, target)
            struct.pack_into('<I', actual_ram, literal_slot, target)
            steps = reference(retail_ram, expected)
            view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
            function(view, ctypes.byref(actual))
            assert steps == 4, (hex(address), steps)
            assert actual_ram == retail_ram, f'RAM mismatch at {address:#x}, case {case}'
            assert bytes(actual) == bytes(expected), f'CPU mismatch at {address:#x}, case {case}'
            cases.append({'case': case, 'pointer_target': hex(target), 'retail_steps': steps,
                          'full_state_and_ram_match': True})
        offset = address - BASE
        results.append({'address': hex(address), 'end_exclusive': hex(address + size),
                        'literal_address': hex(literal), 'retail_pointer_value': hex(retail_pointer),
                        'pointer_register': pointer_register,
                        'value_register': value_register, 'value': value,
                        'retail_span_sha256': hashlib.sha256(
                            binary[offset:offset + size]).hexdigest(),
                        'cases_passed': len(cases), 'cases': cases})

    address, size, literal = 0x8c608e24, 18, 0x8c608e70
    function = lib.sa_global_five_word_initialize_8c608e24
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None
    cases = []
    retail_pointer = struct.unpack_from('<I', binary, literal - BASE)[0]
    for case in range(128):
        mode = case % 4
        if mode == 0:
            target = retail_pointer
        elif mode == 1:
            target = literal
        elif mode == 2:
            target = 0x8c700000 + ((case * 131) & 0x000fffff)
        else:
            target = 0x8cffffec
        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.sr = rng.getrandbits(32)
        expected.pr = 0x8c000a00 + case * 4
        expected.pc = address
        actual = State.from_buffer_copy(bytes(expected))
        retail_ram, actual_ram = bytearray(original), bytearray(original)
        struct.pack_into('<I', retail_ram, literal - 0x8c000000, target)
        struct.pack_into('<I', actual_ram, literal - 0x8c000000, target)
        steps = reference(retail_ram, expected)
        view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        function(view, ctypes.byref(actual))
        assert steps == 9, (hex(address), steps)
        assert actual_ram == retail_ram, f'RAM mismatch at {address:#x}, case {case}'
        assert bytes(actual) == bytes(expected), f'CPU mismatch at {address:#x}, case {case}'
        cases.append({'case': case, 'pointer_target': hex(target), 'retail_steps': steps,
                      'full_state_and_ram_match': True})
    results.append({'address': hex(address), 'end_exclusive': hex(address + size),
                    'name': 'sa_global_five_word_initialize_8c608e24',
                    'literal_address': hex(literal), 'retail_pointer_value': hex(retail_pointer),
                    'pointer_register': 4, 'value_register': 5,
                    'retail_span_sha256': hashlib.sha256(
                        binary[address - BASE:address - BASE + size]).hexdigest(),
                    'cases_passed': len(cases), 'cases': cases})

    source = out / 'source/global_constant_setters.c'
    report = {'functions_verified': len(results),
              'total_full_state_cases': sum(item['cases_passed'] for item in results),
              'binary_sha256': binary_sha256,
              'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
              'scope': 'Three eight-byte indexed literal-global setters and one 18-byte five-word global initializer verified over all retail instructions and full 16 MiB RAM plus GPR/SR/PR/PC equality in 128 cases per routine. Pointer targets include the retail target, a self-aliasing literal address, randomized mapped RAM, and a boundary-safe final RAM target.',
              'limitations': ['This group covers four leaf initializers; owning systems and other call paths remain incomplete.',
                              'Verification uses the project partial SH-4 interpreter and is not hardware or end-to-end game verification.'],
              'routines': results}
    (out / 'analysis/global_constant_setters_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"{len(results)} literal setters passed {report['total_full_state_cases']} full-state comparisons")


if __name__ == '__main__':
    main()
