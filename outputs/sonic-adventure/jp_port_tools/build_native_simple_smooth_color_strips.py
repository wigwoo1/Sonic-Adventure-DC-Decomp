"""Build the pinned smooth color strip drawers and their FIPR emitter."""
import argparse
import json
import re
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import PIN, owner_function
from build_native_simple_material import digest
from build_native_simple_unlit_clip import replace
from build_native_simple_smooth_clip import derive as fixed_source
from rewrite_sprite_float_operations import rewrite


def derive(raw):
    src=fixed_source(raw)
    start=src.index(b'__declspec(dllexport) int jp_simple_prepare_base_light')
    end=src.index(b'extern NJS_VECTOR light2;',start)
    src=src[:start]+src[end:]
    a=src.index(b'static Float *jp_simple_emit_smooth_vertex(')
    b=src.index(b'#define CLIP_NEAR',a)
    src=src[:a]+src[b:src.index(b'Float *jp_simple_emit_smooth_triangles_clip(')]
    a=src.index(b'    view_facing =')
    b=src.index(b'    output_count =',a)
    src=replace(src[:a]+src[b:],b'    Float view_facing;\n',b'')
    src+=b'''
#include "../third_party/skmp-sh4-fpu/sh4_fpu_approx.h"
#include <string.h>
#define DRAW_NOOP() ((void)0)
#define VERTEX_BYTES 32
#define VERTEX_SHIFT 5
#define STRIP_COUNT 0x7fff
static Float *emit_smooth_strip(struct calc_vertex *vertex,Uint32 command,Float *sq_cursor,Float iamb)
{
    uint32_t light[4]={0},normal[4]={0},word;
    Float dot;
    memcpy(light,&light2,12);
    memcpy(normal,&vertex->normal,12);
    word=sh4_fipr(normal,light,1);
    memcpy(&dot,&word,4);
    vertex->intensity=jp_sprite_subtract(iamb,dot);
    return emit_intensity(vertex,command,sq_cursor);
}
#define NJM_SET_POLY_STRIP(v,c) (sq_cursor=emit_smooth_strip(v,c,sq_cursor,iamb))
'''
    for name in ('simple_draw_smooth_strips_cull','simple_draw_smooth_strips_near'):
        near=name.endswith('near')
        body=owner_function(raw,name).replace(b'register ',b'')
        body=re.sub(rb'/\*.*?\*/|^#pragma (?:asm|endasm)[^\r\n]*|^[ \t]*prefetch\(&vertices[^\r\n]*',b'',
                    body,flags=re.M|re.S)
        body=replace(body,('static void '+name+'(NJS_MESHSET *meshset)').encode(),
                     ('Float *'+name+'(NJS_MESHSET *meshset,Float *sq_cursor'+
                      (',Float projection_x,Float projection_y' if near else '')+',Float iamb)').encode())
        body=body.replace(b'(Uint32)run_start < (Uint32)sq_cursor',b'(uintptr_t)run_start < (uintptr_t)sq_cursor')
        body=body.replace(b'(Uint32)sq_cursor - (Uint32)run_start',b'(uintptr_t)sq_cursor - (uintptr_t)run_start')
        if near:
            body=replace(body,b'                Float view_z;\n',b'')
            body=replace(body,b'DECODE_VIEW_POINT(triangle[corner], view_points[corner],\n                                      view_z);',
                         b'decode_unlit_point(triangle[corner], &view_points[corner], projection_x, projection_y);')
            body=replace(body,b'clip_cull_requested = 0;\n                simple_clip_smooth_triangle(v0, v1, v2);',
                         b'sq_cursor=simple_clip_smooth_triangle(v0,v1,v2,sq_cursor,projection_x,projection_y,iamb);')
        src+=body.rstrip()[:-1]+b'    return sq_cursor;\n}\n'
    return src


def caller(raw):
    raw=replace(raw,b'Float *simple_draw_unlit_strips_cull(NJS_MESHSET *,Float *);',
                b'Float *simple_draw_unlit_strips_cull(NJS_MESHSET *,Float *);\n'
                b'Float *simple_draw_smooth_strips_cull(NJS_MESHSET *,Float *,Float);\n'
                b'Float *simple_draw_smooth_strips_near(NJS_MESHSET *,Float *,Float,Float,Float);')
    raw=replace(raw,b'    if(strip){\n        if(!textured)',
                b'    if(strip){\n        if(!textured && !unlit)\n'
                b'            return clipped?simple_draw_smooth_strips_near(mesh,cursor,scales[0],scales[1],ambient):\n'
                b'                           simple_draw_smooth_strips_cull(mesh,cursor,ambient);\n'
                b'        if(!textured)')
    return replace(raw,b' ||\n           (!unlit && !textured && kind==NJD_MESHSET_TRIMESH)',b'')


def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','output'):
        ap.add_argument('--'+n,type=Path,required=True)
    args=ap.parse_args()
    root,base,out=(x.resolve() for x in (args.workspace,args.baseline,args.output))
    assert out.is_relative_to(root/'work/sonic-adventure')
    b=json.loads((base/'build_verification.json').read_bytes())
    assert digest(b['frames_DLL'])==b['frames_DLL_sha256']
    old=b['simple_model']
    source='sdk/ninja/njModelS.c'
    raw=(args.upstream/source).read_bytes()
    assert raw==subprocess.check_output(['git','-C',str(args.upstream),'show',PIN+':'+source])
    wrapper=root/'outputs/sonic-adventure/native/jp_native_simple_model.c'
    assert digest(wrapper)==old['source_sha256']
    recipe=old['commands'][0]
    sdk=recipe[:recipe.index('-c')]
    out.mkdir()
    def run(cmd,tag):
        p=subprocess.run(cmd,capture_output=True)
        (out/(tag+'.stderr')).write_bytes(p.stderr)
        if p.returncode or p.stderr:
            raise RuntimeError(p.stderr.decode(errors='replace'))
        return p.stdout
    src=out/'strip_original.c'
    src.write_bytes(derive(raw))
    tree=json.loads(run(sdk+['-Xclang','-ast-dump=json','-fsyntax-only',str(src)],'ast'))
    native,stats=rewrite(src.read_bytes(),tree,['smooth_intensity','emit_intensity','decode_unlit_point','simple_clip_smooth_triangle',
                                              'simple_draw_smooth_strips_cull','simple_draw_smooth_strips_near'])
    rz=out/'strip_rz.c'
    rz.write_bytes(native)
    ir=out/'strip.ll'
    run(sdk+['-S','-emit-llvm',str(rz),'-o',str(ir)],'llvm')
    assert not re.search(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',ir.read_text())
    bridge=out/'strip_caller.c'
    bridge.write_bytes(caller(wrapper.read_bytes()))
    objects=[]
    for f in (rz,bridge):
        obj=f.with_suffix('.obj')
        run(sdk+['-c',str(f),'-o',str(obj)],f.stem)
        objects.append(obj)
    dll=out/'jp_native_frames.dll'
    for dep in base.glob('*.dll'):
        if dep.name!=dll.name:
            shutil.copyfile(dep,out/dep.name)
    link=old['link'].copy()
    previous=base/'simple_model.obj'
    assert digest(previous)==old['object_sha256']
    index=link.index(str(previous))
    link[index:index+1]=list(map(str,objects))
    link[-1]=str(dll)
    rsp=out/'link.rsp'
    rsp.write_text('\n'.join('"'+x.replace('\\','/')+'"' for x in link[1:])+'\n')
    run([link[0],'@'+str(rsp)],'link')
    b.update(frames_DLL=str(dll),frames_DLL_sha256=digest(dll),simple_smooth_color_strips=dict(
        baseline_sha256=digest(base/'build_verification.json'),upstream_commit=PIN,arithmetic=stats,link=link,
        generated_sha256={f.name:digest(f) for f in (src,rz,ir,bridge,*objects)}))
    (out/'build_verification.json').write_text(json.dumps(b,indent=2)+'\n')
    print('Smooth color strip drawers compiled and linked',flush=True)


if __name__=='__main__':
    main()
