"""Build the pinned unclipped unlit triangle branch and its bounded caller."""
import argparse
import json
import re
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import PIN, owner_function
from build_native_simple_material import digest
from rewrite_sprite_float_operations import rewrite


def derive(raw):
    vertex = raw.split(b'struct calc_vertex {', 1)[1].split(b'};', 1)[0]
    emit = raw.split(b'#define EMIT_UNLIT_COLOR', 1)[1].split(b'/*', 1)[0]
    branch = owner_function(raw, 'simple_draw_unlit_polygons')
    branch = branch.split(b'    case NJD_MESHSET_3:', 1)[1].split(b'        break;', 1)[0]
    return (b'#include <kamui.h>\n#include "jp_native_simple_calc.h"\n'
            b'#include "jp_native_sprite_numeric.h"\n#define VERTEX_WORDS 8\n'
            b'struct calc_vertex {' + vertex + b'};\n'
            b'_Static_assert(sizeof(struct calc_vertex)==32,"Simple record size");\n'
            b'#define WORK_VERTEX(i) (&((struct calc_vertex *)_nj_vertex_buf_)[(Uint16)(i)])\n'
            b'#define prefetch(p) ((void)(p))\n#define EMIT_UNLIT_COLOR' + emit +
            b'Float *jp_simple_emit_unlit_triangles(NJS_MESHSET *meshset,Float *sq_cursor)\n'
            b'{\nSint16 *mesh_cursor=meshset->meshes;\n'
            b'struct calc_vertex *v0,*v1,*v2;\nSint32 polygon;Float facing;\n' +
            branch + b'    return sq_cursor;\n}\n')


def main():
    ap = argparse.ArgumentParser()
    for n in ('workspace', 'upstream', 'baseline', 'output'):
        ap.add_argument('--' + n, type=Path, required=True)
    a = ap.parse_args()
    root, base, d = (p.resolve() for p in (a.workspace, a.baseline, a.output))
    assert d.is_relative_to(root / 'work/sonic-adventure')
    b = json.loads((base / 'build_verification.json').read_bytes())
    assert digest(b['frames_DLL']) == b['frames_DLL_sha256']
    source = 'sdk/ninja/njModelS.c'
    raw = (a.upstream / source).read_bytes()
    assert raw == subprocess.check_output(['git', '-C', str(a.upstream), 'show', PIN + ':' + source])
    old = b['simple_lease']['compile']
    sdk = old[:old.index('-c')]
    d.mkdir()
    commands = []

    def run(cmd, tag):
        proc = subprocess.run(cmd, capture_output=True)
        commands.append(cmd)
        (d / (tag + '.stderr')).write_bytes(proc.stderr)
        if proc.returncode or proc.stderr:
            raise RuntimeError(proc.stderr.decode(errors='replace'))
        return proc.stdout

    orig = d / 'simple_unlit_original.c'
    orig.write_bytes(derive(raw))
    ast = json.loads(run(sdk + ['-Xclang', '-ast-dump=json', '-fsyntax-only', str(orig)], 'ast'))
    rewritten, stats = rewrite(orig.read_bytes(), ast, ['jp_simple_emit_unlit_triangles'])
    rz = d / 'simple_unlit_rz.c'
    rz.write_bytes(rewritten)
    ir = d / 'simple_unlit.ll'
    run(sdk + ['-S', '-emit-llvm', str(rz), '-o', str(ir)], 'llvm')
    assert not re.search(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b', ir.read_text())
    objects = []
    wrapper = root / 'outputs/sonic-adventure/native/jp_native_simple_unlit.c'
    for path, tag in [(rz, 'emitter'), (wrapper, 'caller')]:
        obj = d / (tag + '.obj')
        run(sdk + ['-c', str(path), '-o', str(obj)], tag)
        objects.append(obj)
    for dep in base.glob('*.dll'):
        if dep.name != 'jp_native_frames.dll':
            shutil.copyfile(dep, d / dep.name)
    link = list(b['simple_lease']['link'])
    dll = d / 'jp_native_frames.dll'
    link[link.index('-o'):link.index('-o')] = list(map(str, objects))
    link[-1] = str(dll)
    rsp = d / 'link.rsp'
    rsp.write_text('\n'.join('"' + x.replace('\\', '/') + '"' for x in link[1:]) + '\n')
    run([link[0], '@' + str(rsp)], 'link')
    b.update(frames_DLL=str(dll), frames_DLL_sha256=digest(dll), simple_unlit=dict(
        upstream_commit=PIN, upstream_sha256=digest(a.upstream / source),
        baseline_sha256=digest(base / 'build_verification.json'), arithmetic=stats,
        generated_sha256={x.name: digest(x) for x in [orig, rz, ir, *objects]},
        wrapper_sha256=digest(wrapper), commands=commands, link=link))
    (d / 'build_verification.json').write_text(json.dumps(b, indent=2) + '\n')
    print('Unclipped unlit triangle drawer compiled and linked', flush=True)


if __name__ == '__main__':
    main()
