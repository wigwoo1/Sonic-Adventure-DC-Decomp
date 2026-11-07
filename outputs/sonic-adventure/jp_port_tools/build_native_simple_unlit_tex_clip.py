"""Recover the pinned unlit textured fixed-triangle clip path."""
import argparse
import json
import re
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import PIN, owner_function
from build_native_simple_material import digest
from build_native_simple_unlit_clip import derive as clip_source, replace
from build_native_simple_unlit_tex import derive as tex_source, bridge as tex_bridge
from rewrite_sprite_float_operations import rewrite


def derive(raw):
    plain=clip_source(raw)
    decoder=plain[plain.index(b'#define CLIP_NEAR'):plain.index(b'static Float *simple_clip_unlit_triangle')]
    emit=raw.split(b'#define EMIT_UNLIT_TEX(',1)[1].split(b'#define',1)[0]
    for axis in (b'u',b'v'):
        emit=replace(emit,b'(Float)(texcoord)->'+axis+b' * TEX_SCALE',
                     b'jp_sprite_multiply((Float)(texcoord)->'+axis+b',0x1p-8f)')
    body=owner_function(raw,'simple_clip_unlit_tex_triangle')
    body=replace(body,b'static void ',b'static Float *')
    body=replace(body,b'NJS_COLOR *corner_uv)',b'NJS_COLOR *corner_uv,Float *sq_cursor,Float projection_x,Float projection_y)')
    body=replace(body,b'    Float view_z;\n',b'    struct calc_vertex clipped_vertices[4];\n    NJS_TEX clipped_uv[4];\n')
    body=replace(body,b'DECODE_VIEW_POINT(vertices[corner], view_points[corner], view_z);',
                 b'decode_unlit_point(vertices[corner], &view_points[corner], projection_x, projection_y);')
    body=replace(body,b'if ((view_facing < 0.0F) & clip_cull_requested) {\n        return;\n    }\n    clip_cull_requested = 0;',
                 b'if (view_facing < 0.0F) {\n        return sq_cursor;\n    }')
    # Explicit SH-4 FTRC conversion before storage into the signed UV halfword.
    body,n=re.subn(rb'(clipped_uv\[output_count\]\.[uv] =)\s*(corner_uv\[previous\].*?edge_t\));',
                   rb'\1 (Sint16)jp_sdk_motion_to_int(\2);',body,flags=re.S)
    assert n==2
    branch=owner_function(raw,'simple_draw_unlit_tex_polygons_clip')
    branch=branch.split(b'    case NJD_MESHSET_3:',1)[1].split(b'        break;',1)[0]
    a=branch.index(b'                clip_cull_requested = 1;')
    z=branch.index(b'                simple_clip_unlit_tex_triangle(v0, v1, v2, corner_uv_window);',a)
    # Each original temporary UV makes an exact int16/float/int16 round trip.
    branch=branch[:a]+b'                sq_cursor=simple_clip_unlit_tex_triangle(v0,v1,v2,corner_uv,sq_cursor,projection_x,projection_y);\n                corner_uv+=3;'+branch[z+len(b'                simple_clip_unlit_tex_triangle(v0, v1, v2, corner_uv_window);'):]
    return (tex_source(raw)+decoder+b'\nint32_t jp_sdk_motion_to_int(float);\n#define EMIT_UNLIT_TEX('+emit+
            body.rstrip()[:-1]+b'    return sq_cursor;\n}\n'
            b'Float *jp_simple_emit_unlit_tex_clip(NJS_MESHSET *meshset,Float *sq_cursor,Float projection_x,Float projection_y)\n'
            b'{\nSint16 *mesh_cursor=meshset->meshes;\nNJS_COLOR *corner_uv=meshset->vertuv;\n'
            b'struct calc_vertex *v0,*v1,*v2;\nSint32 polygon;Float facing;Uint32 clip_and,clip_or;\n'+
            branch+b'    return sq_cursor;\n}\n')


def bridge(raw):
    raw=tex_bridge(raw)
    raw=replace(raw,b'#include <limits.h>',b'#include <limits.h>\n#include <string.h>')
    raw=replace(raw,b'Float *jp_simple_emit_unlit_tex(NJS_MESHSET *,Float *);',
                b'Float *jp_simple_emit_unlit_tex(NJS_MESHSET *,Float *);\n'
                b'Float *jp_simple_emit_unlit_tex_clip(NJS_MESHSET *,Float *,Float,Float);')
    raw=replace(raw,b'    if(projection[0])return 101; /* requires the original clipped drawer */\n',b'')
    raw=replace(raw,b'1u+3u*model->meshsets->nbMesh',b'1u+(projection[0]?4u:3u)*model->meshsets->nbMesh')
    raw=replace(raw,b'    cursor=jp_simple_emit_unlit_tex(model->meshsets,cursor);',
                b'    if(projection[0]) {\n'
                b'        Float scales[2];memcpy(scales,projection+4,sizeof scales);\n'
                b'        cursor=jp_simple_emit_unlit_tex_clip(model->meshsets,cursor,scales[0],scales[1]);\n'
                b'    } else cursor=jp_simple_emit_unlit_tex(model->meshsets,cursor);')
    return replace(raw,b'_nj_draw_polygon_>INT_MAX-(int)corners',b'_nj_draw_polygon_>INT_MAX-(int)(4u*mesh->nbMesh)')


def main():
    ap = argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','output'):
        ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args()
    root,base,out=(x.resolve() for x in (a.workspace,a.baseline,a.output))
    assert out.is_relative_to(root/'work/sonic-adventure')
    b=json.loads((base/'build_verification.json').read_bytes())
    assert digest(b['frames_DLL'])==b['frames_DLL_sha256']
    source='sdk/ninja/njModelS.c'
    raw=(a.upstream/source).read_bytes()
    assert raw==subprocess.check_output(['git','-C',str(a.upstream),'show',PIN+':'+source])
    wrapper=root/'outputs/sonic-adventure/native/jp_native_simple_unlit.c'
    assert digest(wrapper)==b['simple_unlit']['wrapper_sha256']
    recipe=next(x for x in b['simple_unlit']['commands'] if '-c' in x and Path(x[-1]).name=='caller.obj')
    sdk=recipe[:recipe.index('-c')]
    out.mkdir()
    commands=[]

    def run(cmd,tag):
        p=subprocess.run(cmd,capture_output=True)
        commands.append(cmd)
        (out/(tag+'.stderr')).write_bytes(p.stderr)
        if p.returncode or p.stderr:
            raise RuntimeError(p.stderr.decode(errors='replace'))
        return p.stdout

    src=out/'tex_clip_original.c'
    src.write_bytes(derive(raw))
    tree=json.loads(run(sdk+['-Xclang','-ast-dump=json','-fsyntax-only',str(src)],'ast'))
    native,stats=rewrite(src.read_bytes(),tree,['jp_simple_emit_unlit_tex','decode_unlit_point','simple_clip_unlit_tex_triangle','jp_simple_emit_unlit_tex_clip'])
    rz=out/'tex_clip_rz.c'
    rz.write_bytes(native)
    ir=out/'tex_clip.ll'
    run(sdk+['-S','-emit-llvm',str(rz),'-o',str(ir)],'llvm')
    assert not re.search(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',ir.read_text())
    caller=out/'tex_clip_caller.c'
    caller.write_bytes(bridge(wrapper.read_bytes()))
    objects=[]
    for f in (rz,caller):
        obj=f.with_suffix('.obj')
        run(sdk+['-c',str(f),'-o',str(obj)],f.stem)
        objects.append(obj)
    dll=out/'jp_native_frames.dll'
    for dep in base.glob('*.dll'):
        if dep.name!=dll.name:
            shutil.copyfile(dep,out/dep.name)
    link=b['simple_unlit_tex']['link'].copy()
    for prior,obj in zip(('tex_rz.obj','tex_caller.obj'),objects):
        old=base/prior
        assert digest(old)==b['simple_unlit_tex']['generated_sha256'][prior]
        link[link.index(str(old))]=str(obj)
    link[-1]=str(dll)
    rsp=out/'link.rsp'
    rsp.write_text('\n'.join('"'+x.replace('\\','/')+'"' for x in link[1:])+'\n')
    run([link[0],'@'+str(rsp)],'link')
    b.update(frames_DLL=str(dll),frames_DLL_sha256=digest(dll),simple_unlit_tex_clip=dict(
        baseline_sha256=digest(base/'build_verification.json'),upstream_commit=PIN,arithmetic=stats,
        commands=commands,link=link,
        generated_sha256={f.name:digest(f) for f in (src,rz,ir,caller,*objects)}))
    (out/'build_verification.json').write_text(json.dumps(b,indent=2)+'\n')
    print('Clipped textured triangle drawer compiled and linked',flush=True)


if __name__=='__main__':
    main()
