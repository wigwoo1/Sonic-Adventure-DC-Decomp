"""Compare native value/stack behavior to the supplied Japanese machine code.

Uses the existing subset SH-4 executor and pinned numerical instruction model.
This is a function-contract comparison, not native/SH-4 register-ABI equality
or a new hardware validation of the shared approximation model.
"""
import argparse
import ctypes as C
import hashlib
import json
import math
import random
import re
import struct
import subprocess
import sys
from pathlib import Path

def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--workspace', required=True, type=Path)
    ap.add_argument('--upstream', required=True, type=Path)
    ap.add_argument('--dll', required=True, type=Path)
    ap.add_argument('--build', required=True, type=Path)
    ap.add_argument('--binary', required=True, type=Path)
    ap.add_argument('--numeric-dll', required=True, type=Path)
    ap.add_argument('--report', required=True, type=Path)
    a = ap.parse_args()
    root = a.workspace.resolve()
    p = root / 'outputs/sonic-adventure'
    w = root / 'work/sonic-adventure'
    build = json.loads((a.build / 'build_verification.json').read_text())
    assert build['exit_code'] == build['diagnostic_bytes'] == 0
    assert build['Ninja_matrix_numerical_mode'] == 'toward_zero_DN1_exceptions_disabled'
    assert sha(a.dll) == build['DLL_sha256']
    for name, digest in build['authored_source_sha256'].items():
        assert sha(root / name) == digest, name
    sys.path.insert(0, str(p))
    from verify_startup import State, reference
    from verify_fpu_cleanup_transfer import Fpu
    historical = json.loads((p / 'analysis/jp_ninja_math_v2_verification.json').read_text())
    binary_path = a.binary.resolve()
    assert sha(binary_path) == historical['original_binary_sha256']
    binary = binary_path.read_bytes()
    symbols = a.upstream / 'config/1st_read/symbols.txt'
    pinned = {'source_sha256': dict(historical['source_reference_sha256'], **{'config/1st_read/symbols.txt': historical['original_symbols_sha256']})}
    assert sha(symbols) == pinned['source_sha256']['config/1st_read/symbols.txt']
    names = ['njInitMatrix', 'njGetMatrix', 'njSetMatrix', 'njUnitMatrix',
             'njPushMatrix', 'njPopMatrix', 'njTranslateV', 'njRotateX',
             'njRotateY', 'njRotateZ', 'njCalcVector', 'njAddVector']
    table = {name: int(address, 16) for address, name in re.findall(
        r'^(0x[0-9a-fA-F]+)\s+(\S+)', symbols.read_text(), re.M)}
    source_hashes = {}
    for name in names:
        source = a.upstream / f'sdk/ninja/{name}.c'
        assert sha(source) == pinned['source_sha256'][f'sdk/ninja/{name}.c']
        source_hashes[f'sdk/ninja/{name}.c'] = sha(source)
    dll = a.dll.resolve()
    lib = C.CDLL(str(dll))
    numeric_path = a.numeric_dll.resolve()
    numeric_pin = json.loads((p / 'jp_port_tools/jp_multi_matrix_external_inputs.json').read_text())
    assert sha(numeric_path) == numeric_pin['numerical_model_DLL_sha256']
    numeric_lib = C.CDLL(str(numeric_path))
    numeric = numeric_lib.sa_sh4_vector_numeric
    numeric.argtypes = [C.c_uint32, C.POINTER(Fpu), C.c_uint32]
    numeric.restype = C.c_int
    Matrix = C.c_float * 16
    class Vector(C.Structure):
        _fields_ = [('x', C.c_float), ('y', C.c_float), ('z', C.c_float)]
    for name in names:
        fn = getattr(lib, name)
        fn.restype = C.c_int if name in ['njPushMatrix', 'njPopMatrix'] else None
        fn.argtypes = {
            'njInitMatrix': [C.c_void_p, C.c_int32, C.c_int32],
            'njGetMatrix': [C.c_void_p], 'njSetMatrix': [C.c_void_p, C.c_void_p],
            'njUnitMatrix': [C.c_void_p], 'njPushMatrix': [C.c_void_p],
            'njPopMatrix': [C.c_uint32], 'njTranslateV': [C.c_void_p, C.c_void_p],
            'njCalcVector': [C.c_void_p, C.c_void_p, C.c_void_p],
            'njAddVector': [C.c_void_p, C.c_void_p],
        }.get(name, [C.c_void_p, C.c_int32])
    BASE, STACK, MEMORY, VIN, VOUT = 0x8c000000, 0x8cd00040, 0x8cd01000, 0x8cd01200, 0x8cd01220
    capacity = 5
    counts = dict.fromkeys(names, 0)
    independent_checks = 0
    rng = random.Random(0x1007)
    ram = bytearray(16 << 20)
    ram[0x10000:0x10000 + len(binary)] = binary
    f = Fpu()
    native_storage = (C.c_ubyte * (64 * (capacity + 2)))()
    native_stack = C.addressof(native_storage) + 64
    matrix = Matrix()
    vin, vout = Vector(), Vector()
    intervals = [(STACK - 64, len(native_storage), C.addressof(native_storage)),
                 (MEMORY, 64, C.addressof(matrix)), (VIN, 12, C.addressof(vin)),
                 (VOUT, 12, C.addressof(vout))]
    def store(address, data):
        ram[address - BASE:address - BASE + len(data)] = data
    def read(address, length):
        return bytes(ram[address - BASE:address - BASE + length])
    def word(address):
        return struct.unpack('<I', read(address, 4))[0]
    def sync():
        for address, length, native in intervals:
            C.memmove(native, read(address, length), length)
    def service(instruction, fp):
        assert numeric(instruction, C.byref(fp), fp.fpul) == 0
    def check(name, args, native_args):
        s = State()
        s.r[:] = [0x22340000 + i for i in range(16)]
        for index, value in enumerate(args):
            s.r[4 + index] = value & 0xffffffff
        s.r[15] = 0x8cf00000
        s.pc, s.pr, s.sr = table[name], 0x8cfed000, 0x70000000
        reference(ram, s, fpu=f, vector_instruction=service, instruction_budget=10000)
        result = getattr(lib, name)(*native_args)
        if name in ['njPushMatrix', 'njPopMatrix']:
            assert result == s.r[0], (name, 'return', result, s.r[0])
        for address, length, native in intervals:
            assert C.string_at(native, length) == read(address, length), (name, 'memory', hex(address))
        current = Matrix()
        lib.njGetMatrix(C.byref(current))
        opposite = ((f.fpscr >> 21) & 1) ^ 1
        assert bytes(current) == struct.pack('<16I', *f.banks[opposite]), (name, 'current matrix')
        for symbol in ['_stack_size_', '_max_stack_size_', '_nj_matrix_flag_']:
            assert C.c_uint32.in_dll(lib, symbol).value == word(table[symbol]), (name, symbol)
        for symbol in ['_nj_current_matrix_ptr_', '_nj_base_matrix_ptr_']:
            pointer = C.c_void_p.in_dll(lib, symbol).value
            assert pointer - native_stack == word(table[symbol]) - STACK, (name, symbol)
        counts[name] += 1
    angles = [0, 1, -1, 0x4000, 0x8000, 0xc000, 0xffff, 0x10000,
              0x12345678, -0x12345678]
    for bank in [0, 1]:
        for normalize in [0, 1]:
            for case in range(16):
                f.fpscr = 0x40001 | bank << 21
                f.fpul = 0
                for index in range(len(native_storage)):
                    ram[STACK - BASE - 64 + index] = (index * 17 + 3) & 255
                data = [rng.uniform(-4, 4) for _ in range(16)]
                if case == 0:
                    data = [float(i % 5 == 0) for i in range(16)]
                store(MEMORY, struct.pack('<16f', *data))
                store(VIN, struct.pack('<3f', 0.75, -0.5, 1.25))
                store(VOUT, struct.pack('<3f', -2, 0.5, 3))
                sync()
                check('njInitMatrix', [STACK, capacity, normalize], [native_stack, capacity, normalize])
                current = Matrix()
                lib.njGetMatrix(C.byref(current))
                assert tuple(current) == (-1,0,0,0, 0,-1,0,0, 0,0,1,0, 0,0,0,1)
                independent_checks += 1
                check('njGetMatrix', [MEMORY], [C.byref(matrix)])
                # Restore a non-affine matrix to expose translation/rotation row layout.
                store(MEMORY, struct.pack('<16f', *data)); sync()
                check('njSetMatrix', [0, MEMORY], [None, C.byref(matrix)])
                check('njSetMatrix', [MEMORY, MEMORY], [C.byref(matrix), C.byref(matrix)])
                for explicit in [False, True]:
                    guest, host = (MEMORY, C.byref(matrix)) if explicit else (0, None)
                    check('njTranslateV', [guest, VIN], [host, C.byref(vin)])
                    for axis in ['njRotateX', 'njRotateY', 'njRotateZ']:
                        for angle in angles:
                            check(axis, [guest, angle], [host, angle])
                    check('njCalcVector', [guest, VIN, VOUT], [host, C.byref(vin), C.byref(vout)])
                    check('njCalcVector', [guest, VIN, VIN], [host, C.byref(vin), C.byref(vin)])
                    check('njUnitMatrix', [guest], [host])
                check('njAddVector', [VOUT, VIN], [C.byref(vout), C.byref(vin)])
                check('njAddVector', [VIN, VIN], [C.byref(vin), C.byref(vin)])
                # Give current and replacement different matrices. Non-null push
                # must save current in the old slot and leave the new slot stale.
                store(MEMORY, struct.pack('<16f', *data)); sync()
                check('njSetMatrix', [0, MEMORY], [None, C.byref(matrix)])
                replacement = [x * -0.5 + 0.25 for x in data]
                store(MEMORY, struct.pack('<16f', *replacement)); sync()
                check('njPushMatrix', [MEMORY], [C.byref(matrix)])
                check('njTranslateV', [0, VIN], [None, C.byref(vin)])
                # Zero pop reloads even the stale saved slot, rather than a no-op.
                check('njPopMatrix', [0], [0])
                check('njSetMatrix', [0, MEMORY], [None, C.byref(matrix)])
                for _ in range(capacity - 1):
                    check('njPushMatrix', [0], [None])
                # Full-stack rejection must retain an unsaved current matrix.
                check('njTranslateV', [0, VIN], [None, C.byref(vin)])
                check('njPushMatrix', [MEMORY], [C.byref(matrix)])
                check('njPopMatrix', [0], [0])
                check('njPopMatrix', [2], [2])
                check('njPopMatrix', [2], [2])
                check('njPopMatrix', [1], [1])
                # Deliberate native safety policy, separately from original behavior.
                before = bytes(native_storage)
                for invalid in [2, 0xffffffff]:
                    assert lib.njPopMatrix(invalid) == 0
                    assert bytes(native_storage) == before
                    independent_checks += 1
    # New cases exercise scalar rounding, DN, overflow and partial aliases.
    adversarial_start = sum(counts.values())
    edge = [0, 0x80000000, 1, 0x80000001, 0x007fffff, 0x00800000,
            0x3f800000, 0xbf800000, 0x3f800001, 0x33800000, 0x7f7fffff, 0xff7fffff]
    for bank in [0, 1]:
        f.fpscr = 0x40001 | bank << 21
        check('njInitMatrix', [STACK, capacity, 0], [native_stack, capacity, 0])
        for case in range(512):
            values = [(rng.getrandbits(1) << 31) | (rng.randrange(255) << 23) | rng.getrandbits(23) for i in range(16)]
            if case < 144:
                values = [edge[(case + i * 7) % len(edge)] for i in range(16)]
                values[4:7] = [edge[case % 12]] * 3
                values[8:11] = [edge[case // 12]] * 3
            store(MEMORY, struct.pack('<16I', *values)); sync()
            delta = [4, -2, -1, 0, 1, 2][case % 6]
            check('njAddVector', [MEMORY + 16, MEMORY + 16 + 4 * delta],
                  [C.addressof(matrix) + 16, C.addressof(matrix) + 16 + 4 * delta])
        # A stored njCalcVector ignores its matrix's fourth row entirely.
        # Keep normalization off so the original scalar executor need not
        # classify nonfinite intermediate products in unrelated instructions.
        for case in range(32):
            data = [rng.uniform(-3, 3) for i in range(12)]
            special_row = [0x7f800000, 0xff800000, 0x7fc12345, 0xff800001]
            store(MEMORY, struct.pack('<12f', *data) + struct.pack('<4I', *special_row))
            store(VIN, struct.pack('<3f', 0.75, -1.25, 2.5)); sync()
            where = VOUT if case % 2 else MEMORY + 4
            host = C.byref(vout) if case % 2 else C.addressof(matrix) + 4
            check('njCalcVector', [MEMORY, VIN, where], [C.byref(matrix), C.byref(vin), host])
    adversarial_calls = sum(counts.values()) - adversarial_start
    # Independent geometric checks don't use the SH-4 numerical DLL.
    lib.njInitMatrix(native_stack, capacity, 0)
    lib.njUnitMatrix(None)
    vec = Vector(2, 3, 4)
    lib.njTranslateV(None, C.byref(vec))
    result = Matrix(); lib.njGetMatrix(C.byref(result))
    assert list(result)[12:16] == [2, 3, 4, 1]
    independent_checks += 1
    for axis in ['njRotateX', 'njRotateY', 'njRotateZ']:
        lib.njUnitMatrix(None); getattr(lib, axis)(None, 0x2000)
        source, target = Vector(1, 2, 3), Vector()
        lib.njCalcVector(None, C.byref(source), C.byref(target))
        c = s = math.sqrt(0.5)
        expected_vector = {
            'njRotateX': (1, 2*c-3*s, 2*s+3*c),
            'njRotateY': (c+3*s, 2, -s+3*c),
            'njRotateZ': (c-2*s, s+2*c, 3),
        }[axis]
        assert max(abs(x-y) for x,y in zip((target.x,target.y,target.z), expected_vector)) < 0.00001
        independent_checks += 1
    report = dict(status='native_Ninja_toward_zero_math_contracts_verified_against_Japanese_machine_code',
                  numerical_policy='toward_zero_DN1_exceptions_disabled',
                  build_report_sha256=sha(a.build / 'build_verification.json'),
                  original_binary_sha256=sha(binary_path), DLL_sha256=sha(dll),
                  original_symbols_sha256=sha(symbols), source_reference_sha256=source_hashes,
                  original_instruction_contract_comparisons=sum(counts.values()), per_function=counts,
                  independent_geometry_initialization_and_native_safety_checks=independent_checks,
                  SH4_active_banks_tested=[0,1], normalization_flags_tested=[0,1],
                  added_scalar_alias_DN_overflow_and_ignored_row_original_calls=adversarial_calls,
                  numerical_model_DLL_sha256=sha(numeric_path),
                  verifier_sha256=sha(Path(__file__)),
                  scope='Exact matrix/vector output bits, guarded stack and parameter memory, depth/capacity/normalization, translated pointer offsets and push/pop return values. Both implicit-current and explicit matrices; angles wrap at 16 bits; self-aliasing, full-stack no-op, zero pop, base-pop rejection, multi-pop. Original CPU/FPU scratch-register side effects are outside native API contract.',
                  limitations=['Shared SH-4 approximation core supplies original instruction numerics; not independent new hardware validation.',
                               'Finite normal inputs and exact zeros, toward-zero rounding, DN=1 and disabled exceptions; nonfinite/subnormal input and alternate FPU environment not qualified.',
                               'Invalid pop above depth is deliberately rejected, unlike original unsigned underflow.',
                               'Global state assumes one game thread; init needs a valid nonempty caller-owned stack.',
                               'Task/collision/event integration, heap lifecycle and full gameplay remain unverified.'],
                  original_game_source_linked=False, retail_assets_published=False,
                  overall_completion_percent=10, goal_paused=False, playable=False)
    target = a.report.resolve()
    assert target.is_relative_to(w)
    target.parent.mkdir(parents=True, exist_ok=True)
    assert not target.exists()
    target.write_text(json.dumps(report, indent=2) + '\n')
    print(sum(counts.values()), 'original instruction comparisons;', independent_checks, 'independent checks passed')

if __name__ == '__main__':
    main()
