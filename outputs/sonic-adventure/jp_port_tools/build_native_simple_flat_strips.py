"""Recover the original flat-lit strip paths for both packet formats."""
import argparse
import json
import re
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import PIN, owner_function
from build_native_simple_material import digest
from build_native_simple_flat_color import derive as flat_color
from build_native_simple_flat_tex import derive as flat_tex, caller as tex_caller
from build_native_simple_smooth_color_strips import derive as smooth_color_strips
from build_native_simple_smooth_tex_strips import derive as smooth_tex_strips
from build_native_simple_unlit_clip import replace
from rewrite_sprite_float_operations import rewrite

functions=['flat_intensity_value','decode_unlit_point','emit_intensity',
           'simple_clip_constant_color_triangle','simple_clip_flat_tex_triangle',
           'simple_draw_flat_path_strips_cull','simple_draw_flat_path_strips_near',
           'simple_draw_flat_path_tex_strips_cull','simple_draw_flat_path_tex_strips_near']


def derive(raw):
    color=flat_color(raw)
    tex=flat_tex(raw)
    src=color[:color.index(b'Float *jp_simple_flat_triangles(')]
    a=tex.index(b'#define EMIT_FLAT_TEX_POSITION')
    b=tex.index(b'extern NJS_VECTOR light2;',a)
    c=tex.index(b'static Float *simple_clip_flat_tex_triangle',b)
    src+=tex[a:b]+tex[c:tex.index(b'Float *jp_simple_flat_tex_triangles(',c)]
    n=src.count(b'if (view_facing < 0.0F) {\n        return sq_cursor;\n    }')
    assert n==2
    # The strip code decides winding before calling either clip helper.
    src=re.sub(rb'    view_facing = .*?    output_count =',b'    output_count =',src,flags=re.S)
    src=src.replace(b'    Float view_facing;\n',b'')
    sc=smooth_color_strips(raw)
    st=smooth_tex_strips(raw)
    src+=sc[sc.index(b'static Float *emit_intensity'):sc.index(b'#define CLIP_NEAR')]
    src+=sc[sc.index(b'#include "../third_party'):sc.index(b'Float *simple_draw_smooth_strips_cull')]
    emit=st[st.index(b'static Float *emit_smooth_strip'):st.index(b'Float *simple_draw_smooth_tex_strips_cull')]
    src+=emit.replace(b'emit_smooth_strip',b'emit_smooth_tex_strip')
    for name in functions[-4:]:
        near=name.endswith('near')
        textured='_tex_' in name
        body=owner_function(raw,name).replace(b'register ',b'')
        body=re.sub(rb'/\*.*?\*/|^#pragma (?:asm|endasm)[^\r\n]*|^[ \t]*prefetch\(&vertices[^\r\n]*',b'',
                    body,flags=re.S|re.M)
        body=replace(body,('static void '+name+'(NJS_MESHSET *meshset)').encode(),
                     ('Float *'+name+'(NJS_MESHSET *meshset,Float *sq_cursor'+
                      (',Float projection_x,Float projection_y' if near else '')+',Float iamb)').encode())
        body=body.replace(b'(Uint32)run_start < (Uint32)sq_cursor',b'(uintptr_t)run_start < (uintptr_t)sq_cursor')
        body=body.replace(b'(Uint32)sq_cursor - (Uint32)run_start',b'(uintptr_t)sq_cursor - (uintptr_t)run_start')
        if textured:
            body=replace(body,b'        corner_uv += 2;\n        if ((uintptr_t)run_start < (uintptr_t)sq_cursor) {\n            corner_uv -= 2;',
                         b'        if ((uintptr_t)run_start < (uintptr_t)sq_cursor) {')
            body=replace(body,b'        }\n    } while (--strips_left',b'        } else corner_uv += 2;\n    } while (--strips_left')
        if near:
            body=replace(body,b'                Float view_z;\n',b'')
            body,n=re.subn(rb'DECODE_VIEW_POINT\(triangle\[corner\], view_points\[corner\],\s*view_z\);',
                           b'decode_unlit_point(triangle[corner], &view_points[corner], projection_x, projection_y);',body)
            assert n==1
            body=replace(body,b'                clip_cull_requested = 0;\n',b'')
            body=replace(body,b'SIMPLE_INTENSITY(flat_normal)',b'flat_intensity_value(flat_normal,iamb)')
            if textured:
                body=replace(body,b'                corner_uv--;\n                flat_normal.x',b'                flat_normal.x')
                body,n=re.subn(rb'                corner_uv_window\[0\].*?                corner_uv--;\n',
                               b'                sq_cursor=simple_clip_flat_tex_triangle(v0,v1,v2,corner_uv-1,flat_intensity,sq_cursor,projection_x,projection_y);\n'
                               b'                run_start=(Uint8 *)sq_cursor;\n',body,flags=re.S)
                assert n==1
            else:
                body=replace(body,b'simple_clip_constant_color_triangle(v0, v1, v2, &flat_color);',
                             b'sq_cursor=simple_clip_constant_color_triangle(v0,v1,v2,&flat_color,sq_cursor,projection_x,projection_y);')
        src+=body.rstrip()[:-1]+b'    return sq_cursor;\n}\n'
    return src


def caller(raw):
    raw=tex_caller(raw)
    a=raw.index(b'struct simple_mesh_span')
    for name in functions[-4:]:
        params=b',Float,Float,Float' if name.endswith('near') else b',Float'
        decl=b'Float *'+name.encode()+b'(NJS_MESHSET *,Float *'+params+b');\n'
        raw=raw[:a]+decl+raw[a:]
    raw=replace(raw,b'    if(strip){\n        if(!textured && !unlit)',
                b'    if(strip){\n'
                b'        if(!unlit && (flags&NJD_FLAG_USE_FLAT)) {\n'
                b'            if(textured)return clipped?simple_draw_flat_path_tex_strips_near(mesh,cursor,scales[0],scales[1],ambient):\n'
                b'                                       simple_draw_flat_path_tex_strips_cull(mesh,cursor,ambient);\n'
                b'            return clipped?simple_draw_flat_path_strips_near(mesh,cursor,scales[0],scales[1],ambient):\n'
                b'                           simple_draw_flat_path_strips_cull(mesh,cursor,ambient);\n'
                b'        }\n        if(!textured && !unlit)')
    return replace(raw,b'if((flags&NJD_FLAG_USE_ENV) || (!unlit && (flags&NJD_FLAG_USE_FLAT) && kind!=NJD_MESHSET_3))',
                   b'if(flags&NJD_FLAG_USE_ENV)')


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
    parent=b['simple_flat_tex']
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
    src=out/'flat_strips_original.c'
    src.write_bytes(derive(raw))
    tree=json.loads(run(sdk+['-Xclang','-ast-dump=json','-fsyntax-only',str(src)],'ast'))
    native,stats=rewrite(src.read_bytes(),tree,functions)
    rz=out/'flat_strips_rz.c'
    rz.write_bytes(native)
    ir=out/'flat_strips.ll'
    run(sdk+['-S','-emit-llvm',str(rz),'-o',str(ir)],'llvm')
    assert not re.search(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',ir.read_text())
    bridge=out/'flat_strips_caller.c'
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
    link=parent['link'].copy()
    previous=base/'flat_tex_caller.obj'
    assert digest(previous)==parent['generated_sha256']['flat_tex_caller.obj']
    index=link.index(str(previous))
    link[index:index+1]=list(map(str,objects))
    link[-1]=str(dll)
    rsp=out/'link.rsp'
    rsp.write_text('\n'.join('"'+x.replace('\\','/')+'"' for x in link[1:])+'\n')
    run([link[0],'@'+str(rsp)],'link')
    b.update(frames_DLL=str(dll),frames_DLL_sha256=digest(dll),simple_flat_strips=dict(
        baseline_sha256=digest(base/'build_verification.json'),upstream_commit=PIN,arithmetic=stats,link=link,
        generated_sha256={f.name:digest(f) for f in (src,rz,ir,bridge,*objects)}))
    (out/'build_verification.json').write_text(json.dumps(b,indent=2)+'\n')
    print('Flat color and textured strip drawers compiled and linked',flush=True)


if __name__=='__main__':
    main()
