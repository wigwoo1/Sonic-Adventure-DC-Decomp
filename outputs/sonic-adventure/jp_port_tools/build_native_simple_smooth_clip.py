"""Extend the pinned smooth color triangle path with original clipping."""
import argparse
import json
import re
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import PIN, owner_function
from build_native_simple_material import digest
from build_native_simple_unlit_clip import derive as clip_source, replace
from build_native_simple_smooth import derive as smooth_source, bridge as smooth_bridge
from rewrite_sprite_float_operations import rewrite


def derive(raw):
    plain=clip_source(raw)
    decoder=plain[plain.index(b'#define CLIP_NEAR'):plain.index(b'static Float *simple_clip_unlit_triangle')]
    smooth=smooth_source(raw)
    a=smooth.index(b'static Float *jp_simple_emit_smooth_vertex(')
    b=smooth.index(b'Float *jp_simple_emit_smooth_triangles(',a)
    emitter=smooth[a:b]
    i=emitter.index(b'vertex->intensity = ')
    j=emitter.index(b';',i)
    intensity=emitter[i+len(b'vertex->intensity = '):j]
    emit=emitter[j+1:emitter.rindex(b'}')]
    smooth=(smooth[:a]+b'static Float smooth_intensity(struct calc_vertex *vertex,Float iamb)\n'
            b'{return '+intensity+b';}\n'
            b'static Float *emit_intensity(struct calc_vertex *vertex,Uint32 command,Float *sq_cursor)\n'
            b'{'+emit+b'}\n'+emitter[:i]+b'vertex->intensity=smooth_intensity(vertex,iamb);\n'
            b'return emit_intensity(vertex,command,sq_cursor);\n}\n'+smooth[b:])
    body=owner_function(raw,'simple_clip_smooth_triangle')
    body=replace(body,b'static void ',b'static Float *')
    body=replace(body,b'struct calc_vertex *v2)',
                 b'struct calc_vertex *v2,Float *sq_cursor,Float projection_x,Float projection_y,Float iamb)')
    body=replace(body,b'    Float view_z;\n',b'    struct calc_vertex clipped_vertices[4];\n')
    body=replace(body,b'SIMPLE_INTENSITY(vertices[corner]->normal);',b'smooth_intensity(vertices[corner],iamb);')
    body=replace(body,b'DECODE_VIEW_POINT(vertices[corner], view_points[corner], view_z);',
                 b'decode_unlit_point(vertices[corner], &view_points[corner], projection_x, projection_y);')
    body=replace(body,b'if ((view_facing < 0.0F) & clip_cull_requested) {\n        return;\n    }\n    clip_cull_requested = 0;',
                 b'if (view_facing < 0.0F) {\n        return sq_cursor;\n    }')
    body,n=re.subn(rb'EMIT_COLOR_INTENSITY\(([^,]+), (KM_VERTEXPARAM_\w+)\);',
                   rb'sq_cursor=emit_intensity(\1,\2,sq_cursor);',body)
    assert n==7
    branch=owner_function(raw,'simple_draw_smooth_polygons_clip')
    branch=branch.split(b'    case NJD_MESHSET_3:',1)[1].split(b'        break;',1)[0]
    branch,n=re.subn(rb'EMIT_SMOOTH_POLYGON\((v[012]), (KM_VERTEXPARAM_\w+)\);',
                     rb'sq_cursor=jp_simple_emit_smooth_vertex(\1,\2,sq_cursor,iamb);',branch)
    assert n==3
    branch=replace(branch,b'clip_cull_requested = 1;\n                simple_clip_smooth_triangle(v0, v1, v2);',
                   b'sq_cursor=simple_clip_smooth_triangle(v0,v1,v2,sq_cursor,projection_x,projection_y,iamb);')
    return (smooth+decoder+body.rstrip()[:-1]+b'    return sq_cursor;\n}\n'
            b'Float *jp_simple_emit_smooth_triangles_clip(NJS_MESHSET *meshset,Float *sq_cursor,Float projection_x,Float projection_y,Float iamb)\n'
            b'{\nSint16 *mesh_cursor=meshset->meshes;\n'
            b'struct calc_vertex *v0,*v1,*v2;\nSint32 polygon;Float facing;Uint32 clip_and,clip_or;\n'+
            branch+b'    return sq_cursor;\n}\n')


def bridge(raw):
    raw=smooth_bridge(raw)
    raw=replace(raw,b'#include <limits.h>',b'#include <limits.h>\n#include <string.h>')
    raw=replace(raw,b'Float *jp_simple_emit_smooth_triangles(NJS_MESHSET *,Float *,Float);',
                b'Float *jp_simple_emit_smooth_triangles(NJS_MESHSET *,Float *,Float);\n'
                b'Float *jp_simple_emit_smooth_triangles_clip(NJS_MESHSET *,Float *,Float,Float,Float);')
    raw=replace(raw,b'    if(projection[0])return 101; /* requires the original clipped drawer */\n',b'')
    raw=replace(raw,b'1u+3u*model->meshsets->nbMesh',b'1u+(projection[0]?4u:3u)*model->meshsets->nbMesh')
    raw=replace(raw,b'    cursor=jp_simple_emit_smooth_triangles(model->meshsets,cursor,draw->ambient);',
                b'    if(projection[0]) {\n'
                b'        Float scales[2];memcpy(scales,projection+4,sizeof scales);\n'
                b'        cursor=jp_simple_emit_smooth_triangles_clip(model->meshsets,cursor,scales[0],scales[1],draw->ambient);\n'
                b'    } else cursor=jp_simple_emit_smooth_triangles(model->meshsets,cursor,draw->ambient);')
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

    src=out/'smooth_clip_original.c'
    src.write_bytes(derive(raw))
    tree=json.loads(run(sdk+['-Xclang','-ast-dump=json','-fsyntax-only',str(src)],'ast'))
    native,stats=rewrite(src.read_bytes(),tree,['jp_simple_prepare_base_light','smooth_intensity','emit_intensity','jp_simple_emit_smooth_vertex','jp_simple_emit_smooth_triangles','decode_unlit_point','simple_clip_smooth_triangle','jp_simple_emit_smooth_triangles_clip'])
    rz=out/'smooth_clip_rz.c'
    rz.write_bytes(native)
    ir=out/'smooth_clip.ll'
    run(sdk+['-S','-emit-llvm',str(rz),'-o',str(ir)],'llvm')
    assert not re.search(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',ir.read_text())
    caller=out/'smooth_clip_caller.c'
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
    link=b['simple_smooth']['link'].copy()
    for prior,obj in zip(('smooth_rz.obj','smooth_caller.obj'),objects):
        old=base/prior
        assert digest(old)==b['simple_smooth']['generated_sha256'][prior]
        link[link.index(str(old))]=str(obj)
    link[-1]=str(dll)
    rsp=out/'link.rsp'
    rsp.write_text('\n'.join('"'+x.replace('\\','/')+'"' for x in link[1:])+'\n')
    run([link[0],'@'+str(rsp)],'link')
    b.update(frames_DLL=str(dll),frames_DLL_sha256=digest(dll),simple_smooth_clip=dict(
        baseline_sha256=digest(base/'build_verification.json'),upstream_commit=PIN,arithmetic=stats,
        commands=commands,link=link,
        generated_sha256={f.name:digest(f) for f in (src,rz,ir,caller,*objects)}))
    (out/'build_verification.json').write_text(json.dumps(b,indent=2)+'\n')
    print('Clipped smooth-lit triangle drawer compiled and linked',flush=True)


if __name__=='__main__':
    main()
