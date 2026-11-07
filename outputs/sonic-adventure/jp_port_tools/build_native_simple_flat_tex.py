"""Recover the original flat-lit textured triangle branches."""
import argparse
import json
import re
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import PIN, owner_function
from build_native_simple_material import digest
from build_native_simple_flat_color import derive as flat_color, caller as color_caller
from build_native_simple_unlit_clip import replace
from rewrite_sprite_float_operations import rewrite


def derive(raw):
    color=flat_color(raw)
    prelude=color[:color.index(b'#define EMIT_FLAT_POSITION')]
    intensity=color[color.index(b'extern NJS_VECTOR light2;'):color.index(b'static Float *simple_clip_constant_color_triangle')]
    start=raw.index(b'#define EMIT_FLAT_TEX_POSITION')
    end=raw.index(b'static void simple_clip_flat_tex_triangle',start)
    macros=raw[start:end]
    macros,n=re.subn(rb'\(Float\)(\(texcoord\)->(?:tex\.)?[uv]) \* TEX_SCALE',
                     rb'jp_sprite_multiply((Float)\1,0x1p-8f)',macros)
    assert n==8
    clip=owner_function(raw,'simple_clip_flat_tex_triangle')
    clip=replace(clip,b'static void ',b'static Float *')
    clip=replace(clip,b'NJS_COLOR *corner_uv, Float intensity)',
                 b'NJS_COLOR *corner_uv, Float intensity,Float *sq_cursor,Float projection_x,Float projection_y)')
    clip=replace(clip,b'    Float view_z;\n',b'    struct calc_vertex clipped_vertices[4];\n    NJS_TEX clipped_uv[4];\n')
    clip=replace(clip,b'DECODE_VIEW_POINT(vertices[corner], view_points[corner], view_z);',
                 b'decode_unlit_point(vertices[corner], &view_points[corner], projection_x, projection_y);')
    clip=replace(clip,b'if ((view_facing < 0.0F) & clip_cull_requested) {\n        return;\n    }\n    clip_cull_requested = 0;',
                 b'if (view_facing < 0.0F) {\n        return sq_cursor;\n    }')
    clip,n=re.subn(rb'(clipped_uv\[output_count\]\.[uv] =)\s*(corner_uv\[previous\].*?edge_t\));',
                   rb'\1 (Sint16)jp_sdk_motion_to_int(\2);',clip,flags=re.S)
    assert n==2
    src=prelude+macros+b'\nint32_t jp_sdk_motion_to_int(float);\n'+intensity+clip.rstrip()[:-1]+b'    return sq_cursor;\n}\n'
    for suffix in ('','_clip'):
        branch=owner_function(raw,'simple_draw_flat_tex_polygons'+suffix)
        branch=branch.split(b'    case NJD_MESHSET_3:',1)[1].split(b'        break;',1)[0]
        branch=branch.replace(b'SIMPLE_INTENSITY(flat_normal)',b'flat_intensity_value(flat_normal,iamb)')
        sig='Float *jp_simple_flat_tex_triangles'+suffix+'(NJS_MESHSET *meshset,Float *sq_cursor,'+(
            'Float projection_x,Float projection_y,' if suffix else '')+'Float iamb)\n'
        src+=sig.encode()+b'{\nSint16 *mesh_cursor=meshset->meshes;\nNJS_COLOR *corner_uv=meshset->vertuv;\n'
        src+=b'struct calc_vertex *v0,*v1,*v2;\nSint32 polygon;Float facing,flat_intensity;\nNJS_VECTOR flat_normal;\n'
        if suffix:
            branch,n=re.subn(rb'                NJS_TEX \*first_uv;.*?                clip_cull_requested = 1;\n',b'',branch,flags=re.S)
            assert n==1
            branch,n=re.subn(rb'                first_uv = .*?simple_clip_flat_tex_triangle\(v0, v1, v2, corner_uv_window,\s*flat_intensity\);',
                             b'                sq_cursor=simple_clip_flat_tex_triangle(v0,v1,v2,corner_uv,flat_intensity,sq_cursor,projection_x,projection_y);\n'
                             b'                corner_uv+=3;',branch,flags=re.S)
            assert n==1
            branch=replace(branch,b'                if (0) {\n                    last_uv->u = 0;\n                }\n',b'')
            src+=b'struct calc_vertex *v1_copy;Uint32 clip_and,clip_or;\n'
        src+=branch+b'    return sq_cursor;\n}\n'
    return src


def caller(raw):
    raw=color_caller(raw)
    raw=replace(raw,b'Float *jp_simple_flat_triangles(NJS_MESHSET *,Float *,Float);',
                b'Float *jp_simple_flat_triangles(NJS_MESHSET *,Float *,Float);\n'
                b'Float *jp_simple_flat_tex_triangles(NJS_MESHSET *,Float *,Float);\n'
                b'Float *jp_simple_flat_tex_triangles_clip(NJS_MESHSET *,Float *,Float,Float,Float);')
    raw=replace(raw,b'    if(!unlit && (flags&NJD_FLAG_USE_FLAT))\n',
                b'    if(!unlit && textured && (flags&NJD_FLAG_USE_FLAT))\n'
                b'        return clipped?jp_simple_flat_tex_triangles_clip(mesh,cursor,scales[0],scales[1],ambient):\n'
                b'                       jp_simple_flat_tex_triangles(mesh,cursor,ambient);\n'
                b'    if(!unlit && (flags&NJD_FLAG_USE_FLAT))\n')
    return replace(raw,b'(textured || kind!=NJD_MESHSET_3)',b'kind!=NJD_MESHSET_3')


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
    parent=b['simple_flat_color']
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
    src=out/'flat_tex_original.c'
    src.write_bytes(derive(raw))
    tree=json.loads(run(sdk+['-Xclang','-ast-dump=json','-fsyntax-only',str(src)],'ast'))
    native,stats=rewrite(src.read_bytes(),tree,['flat_intensity_value','decode_unlit_point','simple_clip_flat_tex_triangle',
                                              'jp_simple_flat_tex_triangles','jp_simple_flat_tex_triangles_clip'])
    rz=out/'flat_tex_rz.c'
    rz.write_bytes(native)
    ir=out/'flat_tex.ll'
    run(sdk+['-S','-emit-llvm',str(rz),'-o',str(ir)],'llvm')
    assert not re.search(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',ir.read_text())
    bridge=out/'flat_tex_caller.c'
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
    previous=base/'flat_caller.obj'
    assert digest(previous)==parent['generated_sha256']['flat_caller.obj']
    index=link.index(str(previous))
    link[index:index+1]=list(map(str,objects))
    link[-1]=str(dll)
    rsp=out/'link.rsp'
    rsp.write_text('\n'.join('"'+x.replace('\\','/')+'"' for x in link[1:])+'\n')
    run([link[0],'@'+str(rsp)],'link')
    b.update(frames_DLL=str(dll),frames_DLL_sha256=digest(dll),simple_flat_tex=dict(
        baseline_sha256=digest(base/'build_verification.json'),upstream_commit=PIN,arithmetic=stats,link=link,
        generated_sha256={f.name:digest(f) for f in (src,rz,ir,bridge,*objects)}))
    (out/'build_verification.json').write_text(json.dumps(b,indent=2)+'\n')
    print('Flat textured triangle drawers compiled and linked',flush=True)


if __name__=='__main__':
    main()
