"""Link the material stage with the verified renderer and check its output words."""
import argparse
import ctypes as c
from fractions import Fraction
import itertools
import json
import shutil
import struct
import subprocess
from pathlib import Path

from build_native_simple_material import digest


U = c.c_uint32


class Color(c.Structure):
    _fields_ = [('a', c.c_float), ('r', c.c_float), ('g', c.c_float), ('b', c.c_float)]


class Light(c.Structure):
    _fields_ = [('ambient', Color), ('diffuse', Color), ('specular', Color), ('exponent', c.c_float)]


class Material(c.Structure):
    _fields_ = [('diffuse', U), ('specular', U), ('exponent', c.c_float), ('texture', U), ('flags', U)]


def words(buf):
    return struct.unpack('<' + 'I' * (len(buf) // 4), buf)


def bits(f):
    return words(struct.pack('<f', f))[0]


def value(word):
    return struct.unpack('<f', struct.pack('<I', word))[0]


def rz(exact):
    word = bits(float(exact))
    return word - (abs(value(word)) > abs(exact))


def main():
    ap = argparse.ArgumentParser()
    for n in ('baseline', 'build', 'output'):
        ap.add_argument('--' + n, type=Path, required=True)
    a = ap.parse_args()
    base, build, out = (p.resolve() for p in (a.baseline, a.build, a.output))
    b = json.loads((base / 'build_verification.json').read_bytes())
    built = json.loads((build / 'build.json').read_bytes())
    assert digest(base / 'build_verification.json') == built['baseline_sha256']
    obj = build / 'simple_material.obj'
    assert digest(obj) == built['generated'][obj.name]
    link = list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and
                     Path(x['command'][-1]).parent == base and
                     Path(x['command'][-1]).name == 'jp_native_frames.dll'))
    out.mkdir()
    dll = out / 'jp_native_frames.dll'
    for dep in base.glob('*.dll'):
        if dep.name != dll.name:
            shutil.copyfile(dep, out / dep.name)
    for key, path in b.items():
        if key.endswith('_DLL') and isinstance(path, str) and path.endswith('.dll'):
            assert digest(path) == b[key + '_sha256'], key
    link[link.index('-o'):link.index('-o')] = [str(obj)]
    link[-1] = str(dll)
    rsp = out / 'link.rsp'
    rsp.write_text('\n'.join('"' + x.replace('\\', '/') + '"' for x in link[1:]) + '\n')
    proc = subprocess.run([link[0], '@' + str(rsp)], capture_output=True)
    (out / 'link.stderr').write_bytes(proc.stderr)
    if proc.returncode or proc.stderr:
        raise RuntimeError(proc.stderr.decode(errors='replace'))
    stage = c.CDLL(str(dll)).jp_simple_material_stage
    stage.argtypes = [c.POINTER(Material), c.POINTER(Light), U, U, U, Color, c.POINTER(U)]
    stage.restype = None
    const = Color(.125, -.25, .375, -.5)
    constant = (Fraction(1, 8), Fraction(-1, 4), Fraction(3, 8), Fraction(-1, 2))
    scale = Fraction(value(0x3b808081))
    colors = [rz(i * scale) for i in range(256)]
    offset_colors = [[rz(Fraction(value(v)) + k) for v in colors] for k in constant]
    cases = 0
    controls = (4, 16, 32, 512)
    attrs = (0x100000, 0x200000, 0x400000, 0x1000000, 0x2000000)
    for flags, control, mask_case, sample in itertools.product(range(32), range(16), range(2), range(8)):
        attr = sum(v for i, v in enumerate(attrs) if flags >> i & 1) | 0x800
        ctrl = sum(v for i, v in enumerate(controls) if control >> i & 1)
        mask, force = [(0xffffffff, 0), (0xffdfffff, 0x100000)][mask_case]
        effective = (attr & mask) | force if ctrl & 4 else attr
        channels = [(sample * 37 + i * 83) & 255 for i in range(4)]
        packed = sum(v << (8 * (3 - i)) for i, v in enumerate(channels))
        material = Material(packed, packed ^ 0xffffffff, 6.5, 0xc0012345, attr)
        seed = bytes.fromhex('00004040') * 13
        light = Light.from_buffer_copy(seed)
        dispatch = (U * 5)()
        before = bytes(material)
        stage(c.byref(material), c.byref(light), ctrl, mask, force, const, dispatch)
        tex, alpha, flat = bool(effective & 0x200000), bool(effective & 0x100000), bool(effective & 0x3000000)
        mode = (0x02100002 if alpha else 0x00080002) | (0x28 if tex else 0x10)
        if flat:
            mode &= ~2
        assert list(dispatch) == [effective, effective & 0x3600000, 0x12345 if tex else 0xffffffff,
                                  mode, (2 if alpha else 1) if tex else (4 if alpha else 3)]
        if tex and ctrl & 512:
            channels[1:] = [255] * 3
        diffuse = [bits(k) for k in constant] if ctrl & 16 else [
            offset_colors[i][v] if ctrl & 32 else colors[v] for i, v in enumerate(channels)]
        specular = [colors[255 - ((packed >> shift) & 255)] for shift in (16, 8, 0)]
        assert words(bytes(light)) == (*words(seed[:16]), *diffuse, bits(3), *specular, bits(6.5))
        assert bytes(material) == before
        cases += 1
    report = dict(status='compiled_linked_and_verified_material_stage', cases=cases,
                  independent_exact_rational_round_toward_zero_oracle=True,
                  build_sha256=digest(build / 'build.json'), dll_sha256=digest(dll),
                  link=link, original_sh4_execution_compared=False,
                  draws_or_submits_gpu_commands=False, playable=False)
    (out / 'verification.json').write_text(json.dumps(report, indent=2) + '\n')
    print(report['status'], cases, 'cases', flush=True)


if __name__ == '__main__':
    main()
