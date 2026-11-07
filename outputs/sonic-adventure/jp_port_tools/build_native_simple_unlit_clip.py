"""Extend the pinned unlit triangle drawer with its original clipped branch."""
import argparse
import json
import re
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import PIN, owner_function
from build_native_simple_material import digest
from build_native_simple_unlit import derive as unclipped
from rewrite_sprite_float_operations import rewrite


def replace(raw, old, new):
    assert raw.count(old) == 1, old
    return raw.replace(old, new)


def derive(raw):
    clip = owner_function(raw, 'simple_clip_unlit_triangle')
    clip = replace(clip, b'static void simple_clip_unlit_triangle(', b'static Float *simple_clip_unlit_triangle(')
    clip = replace(clip, b'struct calc_vertex *v2)', b'struct calc_vertex *v2, Float *sq_cursor, Float projection_x, Float projection_y)')
    clip = replace(clip, b'    Float view_z;\n', b'')
    clip = replace(clip, b'DECODE_VIEW_POINT(vertices[corner], view_points[corner], view_z);',
                   b'decode_unlit_point(vertices[corner], &view_points[corner], projection_x, projection_y);')
    clip = replace(clip, b'    Float view_x;', b'    struct calc_vertex clipped_vertices[4];\n    Float view_x;')
    # This fixed-triangle caller always sets clip_cull_requested to one.
    clip = replace(clip, b'if ((view_facing < 0.0F) & clip_cull_requested) {\n        return;\n    }\n    clip_cull_requested = 0;',
                   b'if (view_facing < 0.0F) {\n        return sq_cursor;\n    }')
    clip = clip.rstrip()
    assert clip.endswith(b'}')
    clip = clip[:-1] + b'    return sq_cursor;\n}\n'
    decode = raw.split(b'#define DECODE_VIEW_POINT(vertex, point, depth)', 1)[1].split(b'/*', 1)[0]
    decode = re.sub(rb'\\[ \t]*\r?\n', b'\n', decode).strip()
    decode = decode.replace(b'(point).', b'point->').replace(b'(vertex)->', b'vertex->').replace(b'(depth)', b'depth')
    clip_branch = owner_function(raw, 'simple_draw_unlit_polygons_clip')
    clip_branch = clip_branch.split(b'    case NJD_MESHSET_3:', 1)[1].split(b'        break;', 1)[0]
    clip_branch = replace(clip_branch, b'clip_cull_requested = 1;\n                simple_clip_unlit_triangle(v0, v1, v2);',
                          b'sq_cursor = simple_clip_unlit_triangle(v0,v1,v2,sq_cursor,projection_x,projection_y);')
    return (unclipped(raw) + b'\n#define CLIP_NEAR 1\n#define CLIP_ALL 0x3f\n'
            b'static void decode_unlit_point(struct calc_vertex *vertex,NJS_POINT3 *point,Float projection_x,Float projection_y)\n'
            b'{ Float depth;\n' + decode + b'\n}\n' + clip +
            b'Float *jp_simple_emit_unlit_triangles_clip(NJS_MESHSET *meshset,Float *sq_cursor,Float projection_x,Float projection_y)\n'
            b'{\nSint16 *mesh_cursor=meshset->meshes;\n'
            b'struct calc_vertex *v0,*v1,*v2;\nSint32 polygon;Float facing;Uint32 clip_and,clip_or;\n' +
            clip_branch + b'    return sq_cursor;\n}\n')


def derive_caller(raw):
    raw = replace(raw, b'Float *jp_simple_emit_unlit_triangles(NJS_MESHSET *,Float *);',
                  b'Float *jp_simple_emit_unlit_triangles(NJS_MESHSET *,Float *);\n'
                  b'Float *jp_simple_emit_unlit_triangles_clip(NJS_MESHSET *,Float *,Float,Float);')
    raw = replace(raw, b'#include <limits.h>', b'#include <limits.h>\n#include <string.h>')
    raw = replace(raw, b'    if(projection[0])return 101; /* requires the original clipped drawer */\n', b'')
    raw = replace(raw, b'1u+3u*model->meshsets->nbMesh', b'1u+(projection[0]?4u:3u)*model->meshsets->nbMesh')
    raw = replace(raw, b'    cursor=jp_simple_emit_unlit_triangles(model->meshsets,cursor);',
                  b'    if(projection[0]) {\n'
                  b'        Float scales[2];memcpy(scales,projection+4,sizeof scales);\n'
                  b'        cursor=jp_simple_emit_unlit_triangles_clip(model->meshsets,cursor,scales[0],scales[1]);\n'
                  b'    } else cursor=jp_simple_emit_unlit_triangles(model->meshsets,cursor);')
    return replace(raw, b'_nj_draw_polygon_>INT_MAX-(int)corners',
                   b'_nj_draw_polygon_>INT_MAX-(int)(4u*mesh->nbMesh)')


def main():
    ap = argparse.ArgumentParser()
    for n in ('workspace', 'upstream', 'baseline', 'output'):
        ap.add_argument('--' + n, type=Path, required=True)
    a = ap.parse_args()
    root, base, d = (p.resolve() for p in (a.workspace, a.baseline, a.output))
    assert d.is_relative_to(root / 'work/sonic-adventure')
    b = json.loads((base / 'build_verification.json').read_bytes())
    old = b['simple_unlit']
    assert digest(b['frames_DLL']) == b['frames_DLL_sha256']
    source = a.upstream / 'sdk/ninja/njModelS.c'
    raw = source.read_bytes()
    assert raw == subprocess.check_output(['git', '-C', str(a.upstream), 'show', PIN + ':sdk/ninja/njModelS.c'])
    wrapper = root / 'outputs/sonic-adventure/native/jp_native_simple_unlit.c'
    assert digest(wrapper) == old['wrapper_sha256']
    recipe = next(x for x in old['commands'] if Path(x[-1]).name == 'caller.obj')
    sdk = recipe[:recipe.index('-c')]
    d.mkdir()
    commands = []

    def run(cmd, tag):
        proc = subprocess.run(cmd, capture_output=True)
        commands.append(cmd)
        (d / (tag + '.stderr')).write_bytes(proc.stderr)
        if proc.returncode or proc.stderr:
            raise RuntimeError(proc.stderr.decode(errors='replace'))
        return proc.stdout

    orig = d / 'simple_unlit_clip_original.c'
    orig.write_bytes(derive(raw))
    ast = json.loads(run(sdk + ['-Xclang', '-ast-dump=json', '-fsyntax-only', str(orig)], 'ast'))
    generated, stats = rewrite(orig.read_bytes(), ast, [
        'jp_simple_emit_unlit_triangles', 'decode_unlit_point',
        'simple_clip_unlit_triangle', 'jp_simple_emit_unlit_triangles_clip'])
    rz = d / 'simple_unlit_clip_rz.c'
    rz.write_bytes(generated)
    ir = d / 'simple_unlit_clip.ll'
    run(sdk + ['-S', '-emit-llvm', str(rz), '-o', str(ir)], 'llvm')
    assert not re.search(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b', ir.read_text())
    caller = d / 'simple_unlit_clip_caller.c'
    caller.write_bytes(derive_caller(wrapper.read_bytes()))
    for path, tag in [(rz, 'emitter'), (caller, 'caller')]:
        run(sdk + ['-c', str(path), '-o', str(d / (tag + '.obj'))], tag)
    for dep in base.glob('*.dll'):
        if dep.name != 'jp_native_frames.dll':
            shutil.copyfile(dep, d / dep.name)
    link = old['link'].copy()
    for name in ('emitter.obj', 'caller.obj'):
        previous = base / name
        assert digest(previous) == old['generated_sha256'][name]
        link[link.index(str(previous))] = str(d / name)
    dll = d / 'jp_native_frames.dll'
    link[-1] = str(dll)
    rsp = d / 'link.rsp'
    rsp.write_text('\n'.join('"' + x.replace('\\', '/') + '"' for x in link[1:]) + '\n')
    run([link[0], '@' + str(rsp)], 'link')
    b.update(frames_DLL=str(dll), frames_DLL_sha256=digest(dll), simple_unlit_clip=dict(
        upstream_commit=PIN, upstream_sha256=digest(source), arithmetic=stats, commands=commands, link=link,
        baseline_sha256=digest(base / 'build_verification.json'),
        generated_sha256={p.name: digest(p) for p in (orig, rz, ir, caller, d/'emitter.obj', d/'caller.obj')}))
    (d / 'build_verification.json').write_text(json.dumps(b, indent=2) + '\n')
    print('Clipped unlit triangle branch compiled and linked', flush=True)


if __name__ == '__main__':
    main()
