"""Build the pinned unlit textured strip drawers and checked UV bridge."""
import argparse
import json
import re
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import PIN, owner_function
from build_native_simple_material import digest
from build_native_simple_unlit_clip import replace
from build_native_simple_unlit_strips import caller as strip_bridge
from build_native_simple_unlit_tex_clip import derive as fixed_source
from rewrite_sprite_float_operations import rewrite


def derive(raw):
    src=fixed_source(raw)
    start=src.index(b'Float *jp_simple_emit_unlit_tex(')
    end=src.index(b'#define CLIP_NEAR',start)
    src=src[:start]+src[end:src.index(b'Float *jp_simple_emit_unlit_tex_clip(')]
    a=src.index(b'    view_facing =')
    b=src.index(b'    output_count =',a)
    src=replace(src[:a]+src[b:],b'    Float view_facing;\n',b'')
    src+=(b'\n#define DRAW_NOOP() ((void)0)\n#define VERTEX_BYTES 32\n#define VERTEX_SHIFT 5\n'
          b'#define STRIP_COUNT 0x7fff\n'
          b'#define _NJM_SET_TEX_X_STRIP(v,c,uv) { NJS_TEX *const uv_once=(uv); EMIT_UNLIT_TEX(v,uv_once,c) }\n')
    for name in ('simple_draw_unlit_tex_strips_cull','simple_draw_unlit_tex_strips_near'):
        near=name.endswith('near')
        body=owner_function(raw,name).replace(b'register ',b'')
        body=re.sub(rb'/\*.*?\*/|^#pragma (?:asm|endasm)[^\r\n]*|^[ \t]*prefetch\(&vertices[^\r\n]*',b'',
                    body,flags=re.M|re.S)
        body=replace(body,('static void '+name+'(NJS_MESHSET *meshset)').encode(),
                     ('Float *'+name+'(NJS_MESHSET *meshset,Float *sq_cursor'+
                      (',Float projection_x,Float projection_y' if near else '')+')').encode())
        body=body.replace(b'(Uint32)run_start < (Uint32)sq_cursor',b'(uintptr_t)run_start < (uintptr_t)sq_cursor')
        body=body.replace(b'(Uint32)sq_cursor - (Uint32)run_start',b'(uintptr_t)sq_cursor - (uintptr_t)run_start')
        body=replace(body,b'        corner_uv += 2;\n        if ((uintptr_t)run_start < (uintptr_t)sq_cursor) {\n            corner_uv -= 2;',
                     b'        if ((uintptr_t)run_start < (uintptr_t)sq_cursor) {')
        body=replace(body,b'        }\n    } while (--strips_left',b'        } else corner_uv += 2;\n    } while (--strips_left')
        if near:
            body=replace(body,b'                Float view_z;\n',b'')
            body=replace(body,b'DECODE_VIEW_POINT(triangle[corner], view_points[corner],\n                                      view_z);',
                         b'decode_unlit_point(triangle[corner], &view_points[corner], projection_x, projection_y);')
            a=body.index(b'                corner_uv--;\n                clip_cull_requested')
            b=body.index(b'                run_start =',a)
            body=body[:a]+b'                sq_cursor=simple_clip_unlit_tex_triangle(v0,v1,v2,corner_uv-1,sq_cursor,projection_x,projection_y);\n'+body[b:]
        src+=body.rstrip()[:-1]+b'    return sq_cursor;\n}\n'
    return src


def caller(raw):
    raw=strip_bridge(raw)
    raw=raw.replace(b'simple_draw_unlit_strips_',b'simple_draw_unlit_tex_strips_')
    raw=raw.replace(b'jp_simple_draw_unlit_strips',b'jp_simple_draw_unlit_tex_strips')
    raw=replace(raw,b'size_t indices,size_t materials,',b'size_t indices,size_t materials,size_t uvs,')
    raw=replace(raw,b'    unsigned draw_limit=bound+corners;',b'    if(!mesh->vertuv || corners>uvs)return 100;\n    unsigned draw_limit=bound+corners;')
    raw=replace(raw,b'dispatch[4]==4',b'dispatch[4]==2')
    raw=replace(raw,b'    njSetkmModelAttr(dispatch[0]);',
                b'    if(njSetkmTextureNum(dispatch[2])!=1)return 102;\n    njSetkmModelAttr(dispatch[0]);')
    raw=replace(raw,b'    Float *cursor=lane==JP_QUEUE_TRANSLUCENT?njStartVertexNonTexTrans():njStartVertexNonTexOpaque();',
                b'    Uint32 *color=(Uint32 *)&_nj_light_material_.diffuse;\n'
                b'    Float *cursor=lane==JP_QUEUE_TRANSLUCENT?njStartVertexTexTransIntens(color):njStartVertexTexOpaqueIntens(color);')
    return replace(raw,
                   b'    if(dispatch[1]!=NJD_FLAG_IGNORE_LIGHT &&\n       dispatch[1]!=(NJD_FLAG_IGNORE_LIGHT|NJD_FLAG_USE_FLAT))return 100;',
                   b'    if(dispatch[1]!=(NJD_FLAG_IGNORE_LIGHT|NJD_FLAG_USE_TEXTURE) &&\n'
                   b'       dispatch[1]!=(NJD_FLAG_IGNORE_LIGHT|NJD_FLAG_USE_TEXTURE|NJD_FLAG_USE_FLAT))return 100;')


def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','output'):
        ap.add_argument('--'+n,type=Path,required=True)
    args=ap.parse_args()
    root,base,out=(x.resolve() for x in (args.workspace,args.baseline,args.output))
    assert out.is_relative_to(root/'work/sonic-adventure')
    b=json.loads((base/'build_verification.json').read_bytes())
    assert digest(b['frames_DLL'])==b['frames_DLL_sha256']
    old=b['simple_unlit_tex_clip']
    source='sdk/ninja/njModelS.c'
    raw=(args.upstream/source).read_bytes()
    assert raw==subprocess.check_output(['git','-C',str(args.upstream),'show',PIN+':'+source])
    wrapper=root/'outputs/sonic-adventure/native/jp_native_simple_unlit.c'
    assert digest(wrapper)==b['simple_unlit']['wrapper_sha256']
    recipe=next(x for x in old['commands'] if '-c' in x and Path(x[-1]).name=='tex_clip_caller.obj')
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
    native,stats=rewrite(src.read_bytes(),tree,['decode_unlit_point','simple_clip_unlit_tex_triangle',
                                              'simple_draw_unlit_tex_strips_cull','simple_draw_unlit_tex_strips_near'])
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
    link[link.index('-o'):link.index('-o')]=list(map(str,objects))
    link[-1]=str(dll)
    rsp=out/'link.rsp'
    rsp.write_text('\n'.join('"'+x.replace('\\','/')+'"' for x in link[1:])+'\n')
    run([link[0],'@'+str(rsp)],'link')
    b.update(frames_DLL=str(dll),frames_DLL_sha256=digest(dll),simple_unlit_tex_strips=dict(
        baseline_sha256=digest(base/'build_verification.json'),upstream_commit=PIN,arithmetic=stats,link=link,
        generated_sha256={f.name:digest(f) for f in (src,rz,ir,bridge,*objects)}))
    (out/'build_verification.json').write_text(json.dumps(b,indent=2)+'\n')
    print('Unlit textured strip drawers compiled and linked',flush=True)


if __name__=='__main__':
    main()
