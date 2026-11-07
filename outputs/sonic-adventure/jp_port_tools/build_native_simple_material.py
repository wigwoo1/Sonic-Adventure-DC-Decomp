"""Extract the pinned Simple model material stage; keep queue I/O at the boundary."""
import argparse
import hashlib
import json
import re
import subprocess
from pathlib import Path

from build_native_read_queue import PIN, owner_function
from rewrite_sprite_float_operations import rewrite


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def derive(raw):
    body = owner_function(raw, '_njSimpleDrawModelSub')
    body = body[body.index(b'            material_flags = material->attrflags;'):
                body.index(b'            previous_material = material_id;')]
    # The material stage returns requested I/O; it does not submit GPU commands.
    body = body.replace(b'njSetkmTextureNum(material->attr_texId & MATERIAL_TEXTURE);',
                        b'out[2] = material->attr_texId & MATERIAL_TEXTURE;')
    body = body.replace(b'njSetkmModelAttr(material_flags);', b'out[0] = material_flags;')
    body = re.sub(rb'njSetkmColorMode\((\w+)\);', rb'out[3] = \1;', body)
    body, starts = re.subn(rb'sq_cursor = \(Float \*\)njStartVertex(\w+)\(.*?\);',
                           rb'out[4] = START_\1;', body, flags=re.S)
    assert starts == 12 and b'njSetkm' not in body and b'sq_cursor' not in body
    # Byte-to-float casts are exact. Freeze the original binary32 scale before
    # rewriting the runtime operators (the literal is the rounded 1/255).
    definitions = raw[raw.index(b'#define MATERIAL_TEXTURE'):raw.index(b'#define EVAL_UNIT')]
    modes = raw[raw.index(b'#define MODE_TEX_OPAQUE'):raw.index(b'#define MODE_FLAT_TRANS')]
    modes += raw[raw.index(b'#define MODE_FLAT_TRANS'):].splitlines()[0] + b'\n'
    specular = owner_function(raw, 'set_material_specular')
    specular = specular.replace(b'(NJS_MATERIAL *material)', b'(NJS_MATERIAL *material, NJS_LIGHT_MATERIAL *light)')
    specular = specular.replace(b'_nj_light_material_.', b'light->')
    body = body.replace(b'_nj_light_material_.', b'light->')
    body = body.replace(b'set_material_specular(material);', b'set_material_specular(material, light);')
    return (b'#include "jp_native_simple_calc.h"\n#include "jp_native_sprite_numeric.h"\n'
            b'#define COLOR_SCALE 0x1.010102p-8f\n' + definitions + modes +
            b'enum { START_TexOpaqueIntens=1, START_TexTransIntens, START_NonTexOpaque, START_NonTexTrans };\n' +
            specular + b'\n__declspec(dllexport) void jp_simple_material_stage(NJS_MATERIAL *material,'
            b'NJS_LIGHT_MATERIAL *light, Uint32 _nj_control_3d_flag_, Uint32 _nj_constant_attr_and_,'
            b'Uint32 _nj_constant_attr_or_, NJS_ARGB _nj_constant_material_, Uint32 *out)\n'
            b'{\nUint32 material_flags, drawer_flags;\nout[2] = 0xffffffffu;\n' +
            body + b'out[1] = drawer_flags;\n}\n')


def main():
    ap = argparse.ArgumentParser()
    for n in ('workspace', 'upstream', 'baseline', 'output'):
        ap.add_argument('--' + n, type=Path, required=True)
    a = ap.parse_args()
    root = a.workspace.resolve()
    base = a.baseline.resolve()
    d = a.output.resolve()
    assert d.is_relative_to(root / 'work/sonic-adventure')
    b = json.loads((base / 'build_verification.json').read_bytes())
    for path, sha in b['authored_source_sha256'].items():
        assert digest(root / path) == sha, path
    source = 'sdk/ninja/njModelS.c'
    raw = (a.upstream / source).read_bytes()
    assert raw == subprocess.check_output(['git', '-C', str(a.upstream), 'show', PIN + ':' + source])
    recipe = next(x['command'] for x in b['commands'] if '-c' in x['command'] and
                  Path(x['command'][-1]).name == 'simple_calc_original.obj')
    sdk = recipe[:recipe.index('-c')]
    d.mkdir()
    commands = []

    def run(args, name):
        proc = subprocess.run(sdk + args, capture_output=True)
        (d / (name + '.stderr')).write_bytes(proc.stderr)
        commands.append(sdk + args)
        if proc.returncode or proc.stderr:
            raise RuntimeError(proc.stderr.decode(errors='replace'))
        return proc.stdout

    generated = d / 'simple_material.c'
    generated.write_bytes(derive(raw))
    tree = json.loads(run(['-Xclang', '-ast-dump=json', '-fsyntax-only', str(generated)], 'ast'))
    rewritten, stats = rewrite(generated.read_bytes(), tree, ['set_material_specular', 'jp_simple_material_stage'])
    rz = d / 'simple_material_rz.c'
    rz.write_bytes(rewritten)
    ir = d / 'simple_material.ll'
    run(['-S', '-emit-llvm', str(rz), '-o', str(ir)], 'llvm')
    assert not re.search(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b', ir.read_text())
    obj = d / 'simple_material.obj'
    run(['-c', str(rz), '-o', str(obj)], 'compile')
    report = dict(upstream_commit=PIN, upstream_source_sha256=hashlib.sha256(raw).hexdigest(),
                  baseline_sha256=digest(base / 'build_verification.json'),
                  generated={f.name: digest(f) for f in (generated, rz, ir, obj)},
                  arithmetic=stats, commands=commands, runtime_verified=False)
    (d / 'build.json').write_text(json.dumps(report, indent=2) + '\n')
    print(stats, flush=True)


if __name__ == '__main__':
    main()
