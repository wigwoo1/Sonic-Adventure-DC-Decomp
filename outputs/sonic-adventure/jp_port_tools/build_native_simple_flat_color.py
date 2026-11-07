"""Recover original flat-lit color triangles, including near-plane clipping."""
import argparse
import json
import re
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import PIN, owner_function
from build_native_simple_material import digest
from build_native_simple_unlit import derive as plain
from build_native_simple_unlit_clip import derive as clip_source, replace
from build_native_simple_smooth_color_strips import caller as strip_caller
from rewrite_sprite_float_operations import rewrite


def derive(raw):
    prelude=plain(raw).split(b'Float *jp_simple_emit_unlit_triangles(',1)[0]
    dec=clip_source(raw)
    decoder=dec[dec.index(b'#define CLIP_NEAR'):dec.index(b'static Float *simple_clip_unlit_triangle')]
    macros=raw[raw.index(b'#define EMIT_FLAT_POSITION'):raw.index(b'static void simple_clip_constant_color_triangle(',raw.index(b'#define EMIT_FLAT_POSITION'))]
    intensity=raw.split(b'#define SIMPLE_INTENSITY(normal)',1)[1].split(b'/*',1)[0]
    intensity=re.sub(rb'\\[ \t]*\r?\n',b'\n',intensity).strip()
    clip=owner_function(raw,'simple_clip_constant_color_triangle')
    clip=replace(clip,b'static void ',b'static Float *')
    clip=replace(clip,b'NJS_ARGB *color)',b'NJS_ARGB *color,Float *sq_cursor,Float projection_x,Float projection_y)')
    clip=replace(clip,b'    Float view_z;\n',b'    struct calc_vertex clipped_vertices[4];\n')
    clip=replace(clip,b'DECODE_VIEW_POINT(vertices[corner], view_points[corner], view_z);',
                 b'decode_unlit_point(vertices[corner], &view_points[corner], projection_x, projection_y);')
    clip=replace(clip,b'if ((view_facing < 0.0F) & clip_cull_requested) {\n        return;\n    }\n    clip_cull_requested = 0;',
                 b'if (view_facing < 0.0F) {\n        return sq_cursor;\n    }')
    src=(prelude+decoder+macros+b'\nextern NJS_VECTOR light2;\n'
         b'static Float flat_intensity_value(NJS_VECTOR normal,Float iamb)\n{return '+intensity+b';}\n'
         +clip.rstrip()[:-1]+b'    return sq_cursor;\n}\n')
    for suffix in ('','_clip'):
        branch=owner_function(raw,'simple_draw_flat_polygons'+suffix)
        branch=branch.split(b'    case NJD_MESHSET_3:',1)[1].split(b'        break;',1)[0]
        branch=branch.replace(b'SIMPLE_INTENSITY(flat_normal)',b'flat_intensity_value(flat_normal,iamb)')
        sig='Float *jp_simple_flat_triangles'+suffix+'(NJS_MESHSET *meshset,Float *sq_cursor,'+(
            'Float projection_x,Float projection_y,' if suffix else '')+'Float iamb)\n'
        src+=sig.encode()+b'{\nSint16 *mesh_cursor=meshset->meshes;\n'
        src+=b'struct calc_vertex *v0,*v1,*v2;\nSint32 polygon;Float facing,flat_intensity;\nNJS_VECTOR flat_normal;NJS_ARGB flat_color;\n'
        if suffix:
            branch=replace(branch,b'                clip_cull_requested = 1;\n',b'')
            branch=replace(branch,b'                simple_clip_constant_color_triangle(v0, v1, v2, &flat_color);',
                           b'                sq_cursor=simple_clip_constant_color_triangle(v0,v1,v2,&flat_color,sq_cursor,projection_x,projection_y);')
            src+=b'struct calc_vertex *v0_copy;Float v1_x;Uint32 clip_and,clip_or;\n'
        src+=branch+b'    return sq_cursor;\n}\n'
    return src


def caller(raw):
    raw=strip_caller(raw)
    raw=replace(raw,b'Float *jp_simple_emit_smooth_triangles(NJS_MESHSET *,Float *,Float);',
                b'Float *jp_simple_emit_smooth_triangles(NJS_MESHSET *,Float *,Float);\n'
                b'Float *jp_simple_flat_triangles(NJS_MESHSET *,Float *,Float);\n'
                b'Float *jp_simple_flat_triangles_clip(NJS_MESHSET *,Float *,Float,Float,Float);')
    raw=replace(raw,b'    if(unlit && !textured)\n',
                b'    if(!unlit && (flags&NJD_FLAG_USE_FLAT))\n'
                b'        return clipped?jp_simple_flat_triangles_clip(mesh,cursor,scales[0],scales[1],ambient):\n'
                b'                       jp_simple_flat_triangles(mesh,cursor,ambient);\n'
                b'    if(unlit && !textured)\n')
    return replace(
        raw,b'(!unlit && (flags&NJD_FLAG_USE_FLAT)))return SIMPLE_UNSUPPORTED;',
        b'(!unlit && (flags&NJD_FLAG_USE_FLAT) && (textured || kind!=NJD_MESHSET_3)))return SIMPLE_UNSUPPORTED;')


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
    parent=b['simple_smooth_color_strips']
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
    src=out/'flat_original.c'
    src.write_bytes(derive(raw))
    tree=json.loads(run(sdk+['-Xclang','-ast-dump=json','-fsyntax-only',str(src)],'ast'))
    native,stats=rewrite(src.read_bytes(),tree,['flat_intensity_value','decode_unlit_point','simple_clip_constant_color_triangle',
                                              'jp_simple_flat_triangles','jp_simple_flat_triangles_clip'])
    rz=out/'flat_rz.c'
    rz.write_bytes(native)
    ir=out/'flat.ll'
    run(sdk+['-S','-emit-llvm',str(rz),'-o',str(ir)],'llvm')
    assert not re.search(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',ir.read_text())
    bridge=out/'flat_caller.c'
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
    previous=base/'strip_caller.obj'
    assert digest(previous)==parent['generated_sha256']['strip_caller.obj']
    index=link.index(str(previous))
    link[index:index+1]=list(map(str,objects))
    link[-1]=str(dll)
    rsp=out/'link.rsp'
    rsp.write_text('\n'.join('"'+x.replace('\\','/')+'"' for x in link[1:])+'\n')
    run([link[0],'@'+str(rsp)],'link')
    b.update(frames_DLL=str(dll),frames_DLL_sha256=digest(dll),simple_flat_color=dict(
        baseline_sha256=digest(base/'build_verification.json'),upstream_commit=PIN,arithmetic=stats,link=link,
        generated_sha256={f.name:digest(f) for f in (src,rz,ir,bridge,*objects)}))
    (out/'build_verification.json').write_text(json.dumps(b,indent=2)+'\n')
    print('Flat color triangle drawers compiled and linked',flush=True)


if __name__=='__main__':
    main()
