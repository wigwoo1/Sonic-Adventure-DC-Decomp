"""Compare native njMultiMatrix to actual Japanese opcodes, including aliasing.

The SH-4 instruction executor uses the pinned numerical model for FIPR/FTRV.
Independent exact integer matrix cases do not use that model as their oracle.
"""
import argparse
import ctypes as C
import hashlib
import json
import random
import struct
import sys
from pathlib import Path


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def pack(words):
    return struct.pack('<' + 'I' * len(words), *words)


def integer_matrix(values):
    return struct.pack('<16f', *values)


def exact_product(left, right):
    return integer_matrix([sum(left[4 * row + i] * right[4 * i + column] for i in range(4))
                           for row in range(4) for column in range(4)])


def main():
    ap = argparse.ArgumentParser()
    for n in ['workspace', 'upstream', 'build', 'binary', 'numeric-dll', 'report']:
        ap.add_argument('--' + n, dest=n.replace('-', '_'), type=Path, required=True)
    a = ap.parse_args()
    r, d, u = a.workspace.resolve(), a.build.resolve(), a.upstream.resolve()
    p = r / 'outputs/sonic-adventure'
    assert a.report.resolve().is_relative_to(r / 'work/sonic-adventure') and not a.report.exists()
    pin_path = p / 'jp_port_tools/jp_multi_matrix_external_inputs.json'
    pin = json.loads(pin_path.read_text())
    build = json.loads((d / 'build_verification.json').read_text())
    assert build['exit_code'] == build['diagnostic_bytes'] == 0
    assert build['Ninja_matrix_numerical_mode'] == 'toward_zero_DN1_exceptions_disabled'
    dll = Path(build['DLL'])
    assert sha(dll) == build['DLL_sha256']
    assert sha(a.binary) == pin['original_binary_sha256']
    assert sha(a.numeric_dll) == pin['numerical_model_DLL_sha256']
    for name, h in pin['files_sha256'].items():
        assert sha(u / name) == h
    for name, h in build['authored_source_sha256'].items():
        assert sha(r / name) == h
    sys.path.insert(0, str(p))
    from verify_startup import State, reference
    from verify_fpu_cleanup_transfer import Fpu
    lib = C.CDLL(str(dll))
    for name, result, args in [
        ('njInitMatrix', None, [C.c_void_p, C.c_int32, C.c_int32]),
        ('njPushMatrix', C.c_int, [C.c_void_p]), ('njMultiMatrix', None, [C.c_void_p, C.c_void_p]),
        ('njGetMatrix', None, [C.c_void_p]), ('jp_shz_load', None, [C.c_void_p]),
        ('jp_shz_store', None, [C.c_void_p]), ('jp_shz_store_from_other_unit', None, [C.c_void_p]),
        ('jp_ninja_current_matrix_storage', C.c_void_p, []), ('shz_xmtrx_state_', C.c_void_p, [])]:
        fn = getattr(lib, name)
        fn.restype, fn.argtypes = result, args
    numeric_lib = C.CDLL(str(a.numeric_dll.resolve()))
    numeric = numeric_lib.sa_sh4_vector_numeric
    numeric.restype, numeric.argtypes = C.c_int, [C.c_uint32, C.POINTER(Fpu), C.c_uint32]
    rng = random.Random(0x4d554c54)
    BASE, ARENA, STACK = 0x8c000000, 0x8cd10000, 0x8cd20000
    stack_offset, capacity = 64, 6
    arena = (C.c_ubyte * 512)()
    stack = (C.c_ubyte * 512)()
    current = (C.c_ubyte * 64)()
    ram = bytearray(16 << 20)
    binary = a.binary.read_bytes()
    ram[0x10000:0x10000 + len(binary)] = binary
    entry = int(pin['original_entry_point'], 16)
    checks, calls, path_differences = 0, 0, 0
    sections = {}
    def check(value, label):
        nonlocal checks
        assert value, label
        checks += 1
    def store(address, raw):
        ram[address - BASE:address - BASE + len(raw)] = raw
    def read(address, count):
        return bytes(ram[address - BASE:address - BASE + count])
    def service(instruction, fp):
        assert numeric(instruction, C.byref(fp), fp.fpul) == 0
    def random_bytes(count):
        return rng.randbytes(count)
    def scenario(bank, implicit, left, right, delta=32, unaligned=False, source_stack=False, dest_stack=False):
        nonlocal calls
        lib.njInitMatrix(C.addressof(stack) + stack_offset, capacity, calls % 2)
        for unused in range(calls % capacity):
            check(lib.njPushMatrix(None) == 1, 'setup active matrix stack depth')
        # All 512 bytes, including guards, are compared with original RAM.
        raw_arena, raw_stack = bytearray(random_bytes(512)), bytearray(random_bytes(512))
        dest_offset = (33 if unaligned else 32) * 4
        source_offset = dest_offset + 4 * delta
        if source_stack: source_offset = stack_offset + 64
        if dest_stack: dest_offset = stack_offset + 64 * 2
        if not implicit:
            (raw_stack if dest_stack else raw_arena)[dest_offset:dest_offset + 64] = right
        (raw_stack if source_stack else raw_arena)[source_offset:source_offset + 64] = left
        C.memmove(arena, bytes(raw_arena), 512)
        C.memmove(stack, bytes(raw_stack), 512)
        store(ARENA, raw_arena)
        store(STACK, raw_stack)
        current_pointer = lib.jp_ninja_current_matrix_storage()
        C.memmove(current_pointer, right, 64)
        before_scalars = {n: C.c_int32.in_dll(lib, n).value for n in
                          ['_stack_size_', '_max_stack_size_', '_nj_matrix_flag_']}
        before_pointers = {n: C.c_void_p.in_dll(lib, n).value for n in
                           ['_nj_current_matrix_ptr_', '_nj_base_matrix_ptr_']}
        f = Fpu()
        f.fpscr = 0x40001 | (bank << 21)
        f.banks[bank][:] = [rng.getrandbits(32) for unused in range(16)]
        f.banks[bank ^ 1][:] = struct.unpack('<16I', right)
        guest_source = (STACK if source_stack else ARENA) + source_offset
        guest_destination = 0 if implicit else (STACK if dest_stack else ARENA) + dest_offset
        host_source = C.addressof(stack if source_stack else arena) + source_offset
        host_destination = None if implicit else C.addressof(stack if dest_stack else arena) + dest_offset
        s = State()
        s.r[:] = [0x34560000 + i for i in range(16)]
        s.r[4], s.r[5], s.r[15] = guest_destination, guest_source, 0x8cf00000
        s.pc, s.pr, s.sr = entry, 0x8cfed000, 0x70000000
        reference(ram, s, fpu=f, vector_instruction=service, instruction_budget=1000)
        check(s.r[15] == 0x8cf00000 and (f.fpscr & 0x380000) == (bank << 21),
              'original executor returns with restored stack and bank/width/precision')
        lib.njMultiMatrix(host_destination, host_source)
        check(bytes(arena) == read(ARENA, 512), 'all parameter memory and guards equal original')
        check(bytes(stack) == read(STACK, 512), 'all deferred stack memory and guards equal original')
        expected_current = pack(list(f.banks[bank ^ 1]))
        for name in ['njGetMatrix', 'jp_shz_store', 'jp_shz_store_from_other_unit']:
            getattr(lib, name)(C.byref(current))
            check(bytes(current) == expected_current, 'same active matrix visible through ' + name)
        check(lib.jp_ninja_current_matrix_storage() == lib.shz_xmtrx_state_() == current_pointer,
              'one stable active-state instance')
        for n, value in before_scalars.items():
            check(C.c_int32.in_dll(lib, n).value == value, 'unchanged native matrix scalar ' + n)
        for n, value in before_pointers.items():
            check(C.c_void_p.in_dll(lib, n).value == value, 'unchanged native saved-slot pointer ' + n)
        calls += 1
        return expected_current if implicit else read(guest_destination, 64)
    def family(name, cases, generator, independent=False):
        nonlocal path_differences
        before_checks, before_calls = checks, calls
        for case in range(cases):
            left, right = generator(case)
            expected = None
            if independent:
                expected = exact_product(list(struct.unpack('<16f', left)), list(struct.unpack('<16f', right)))
            for bank in [0, 1]:
                stored = scenario(bank, False, left, right)
                active = scenario(bank, True, left, right)
                path_differences += stored != active
                if expected is not None:
                    check(stored == expected and active == expected, 'independent exact integer product')
        sections[name] = dict(checks=checks - before_checks, original_calls=calls - before_calls)
    family('independent_exact_integer_products', 64,
           lambda unused: (integer_matrix([rng.randrange(-8, 9) for i in range(16)]),
                           integer_matrix([rng.randrange(-8, 9) for i in range(16)])), True)
    family('finite_normal_products', 256,
           lambda unused: (pack([(rng.getrandbits(1) << 31) | (rng.randrange(104, 150) << 23) | rng.getrandbits(23) for i in range(16)]),
                           pack([(rng.getrandbits(1) << 31) | (rng.randrange(104, 150) << 23) | rng.getrandbits(23) for i in range(16)])))
    special = [0, 0x80000000, 1, 0x80000001, 0x007fffff, 0x00800000, 0x7f7fffff,
               0xff7fffff, 0x3f800000, 0xbf800000, 0x7f800000, 0xff800000,
               0x7fc12345, 0xffc54321, 0x7f800001, 0xff800001]
    family('special_and_subnormal_operand_bits_DN1', 64,
           lambda case: (pack([special[(i + case) % len(special)] for i in range(16)]),
                         pack([special[(3 * i + case // 4) % len(special)] for i in range(16)])))
    family('arbitrary_operand_words', 128,
           lambda unused: (random_bytes(64), random_bytes(64)))
    check(path_differences > 0, 'cases distinguish original stored FIPR operand order from implicit FTRV')
    before_checks, before_calls = checks, calls
    for bank in [0, 1]:
        for delta in range(-15, 16):
            for unaligned in [False, True]:
                scenario(bank, False, random_bytes(64), random_bytes(64), delta, unaligned)
        for implicit in [False, True]:
            for i in range(16):
                scenario(bank, implicit, random_bytes(64), random_bytes(64), source_stack=True)
        for i in range(16):
            scenario(bank, False, random_bytes(64), random_bytes(64), dest_stack=True)
    sections['overlapping_four_byte_aligned_and_saved_slot_operands'] = dict(checks=checks-before_checks, original_calls=calls-before_calls)
    before_checks = checks
    pointer = lib.jp_ninja_current_matrix_storage()
    check(pointer > 0xffffffff and pointer % 64 == 0, 'aligned full-width native current pointer')
    for case in range(64):
        values = [rng.randrange(-4, 5) for i in range(16)]
        raw = integer_matrix(values)
        buffer = (C.c_ubyte * 64).from_buffer_copy(raw)
        lib.jp_shz_load(C.byref(buffer))
        lib.njMultiMatrix(None, pointer)
        expected = exact_product(values, values)
        for name in ['njGetMatrix', 'jp_shz_store', 'jp_shz_store_from_other_unit']:
            getattr(lib, name)(C.byref(current))
            check(bytes(current) == expected, 'native active-source alias with independent square oracle')
    sections['native_active_source_alias_and_cross_renderer_visibility'] = dict(checks=checks-before_checks, original_calls=0)
    report = dict(status='native_toward_zero_multi_matrix_contracts_verified_against_original_Japanese_opcodes',
                  numerical_policy='toward_zero_DN1_exceptions_disabled',
                  checks=checks, original_instruction_contract_comparisons=calls, sections=sections,
                  stored_and_implicit_path_difference_cases=path_differences,
                  original_binary_sha256=sha(a.binary), original_entry_point=pin['original_entry_point'],
                  numerical_model_DLL_sha256=sha(a.numeric_dll), external_input_manifest_sha256=sha(pin_path),
                  DLL_sha256=sha(dll), build_report_sha256=sha(d / 'build_verification.json'),
                  verifier_sha256=sha(Path(__file__)), executor_sha256=sha(p / 'verify_startup.py'),
                  FPU_layout_support_sha256=sha(p / 'verify_fpu_cleanup_transfer.py'),
                  limitations=['Original instruction execution uses the shared pinned SH-4 numerical model; this is not new independent hardware validation.',
                               'Fixed toward-zero/DN=1/disabled-exception value policy; native code does not reproduce FPSCR or scratch-register side effects.',
                               'Original comparisons use both active banks with SZ=PR=0. Whole-game entry FPSCR policy remains untraced.',
                               'Valid sixteen-word host operands, including overlapping memory; null source and malformed pointers are outside the API contract.',
                               'Native active-storage source alias is independently tested; SH-4 XMTRX is not addressable RAM.',
                               'No frame rasterization, gameplay, English localization or later-platform qualification.'],
                  game_source_linked=False, retail_assets_published=False, overall_completion_percent=10,
                  goal_paused=False, playable=False)
    a.report.write_text(json.dumps(report, indent=2) + '\n')
    print(calls, 'original calls;', checks, 'checks;', path_differences, 'stored/current differing products')


if __name__ == '__main__':
    main()
