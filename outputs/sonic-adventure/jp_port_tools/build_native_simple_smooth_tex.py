"""Build original smooth-lit textured triangle and near-plane paths."""
import argparse
import json
import re
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import PIN, owner_function
from build_native_simple_material import digest
from build_native_simple_unlit_clip import derive as clip_source, replace
from build_native_simple_unlit import derive as plain
from build_native_simple_smooth_clip import derive as smooth_clip_source, bridge as smooth_clip_bridge
from rewrite_sprite_float_operations import rewrite


def derive(raw):
    prelude=plain(raw).split(b'Float *jp_simple_emit_unlit_triangles(',1)[0]
    dec=clip_source(raw)
    decoder=dec[dec.index(b'#define CLIP_NEAR'):dec.index(b'static Float *simple_clip_unlit_triangle')]
    smooth=smooth_clip_source(raw)
    intensity=smooth[smooth.index(b'static Float smooth_intensity'):smooth.index(b'static Float *emit_intensity')]
    macros=[]
    for name in (b'EMIT_TEX_INTENSITY',b'EMIT_SMOOTH_TEX_POLYGON'):
        macro=raw.split(b'#define '+name+b'(',1)[1]
        macro=b'#define '+name+b'('+macro[:re.search(rb'\n    }[ \t]*\r?\n',macro).end()]
        macro=macro.replace(b'SIMPLE_INTENSITY((vertex)->normal)',b'smooth_intensity(vertex,iamb)')
        for axis in (b'u',b'v'):
            macro,n=re.subn(rb'\(Float\)(\((?:texcoord|uv_cursor)\)->(?:tex\.)?'+axis+rb') \* TEX_SCALE',
                           rb'jp_sprite_multiply((Float)\1,0x1p-8f)',macro)
            assert n==1
        macros.append(macro)
    clip=owner_function(raw,'simple_clip_smooth_tex_triangle')
    clip=replace(clip,b'static void ',b'static Float *')
    clip=replace(clip,b'NJS_COLOR *corner_uv)',b'NJS_COLOR *corner_uv,Float *sq_cursor,Float projection_x,Float projection_y,Float iamb)')
    clip=replace(clip,b'    Float view_z;\n',b'    struct calc_vertex clipped_vertices[4];\n    NJS_TEX clipped_uv[4];\n')
    clip=replace(clip,b'SIMPLE_INTENSITY(vertices[corner]->normal);',b'smooth_intensity(vertices[corner],iamb);')
    clip=replace(clip,b'DECODE_VIEW_POINT(vertices[corner], view_points[corner], view_z);',
                 b'decode_unlit_point(vertices[corner], &view_points[corner], projection_x, projection_y);')
    clip=replace(clip,b'if ((view_facing < 0.0F) & clip_cull_requested) {\n        return;\n    }\n    clip_cull_requested = 0;',
                 b'if (view_facing < 0.0F) {\n        return sq_cursor;\n    }')
    clip,n=re.subn(rb'(clipped_uv\[output_count\]\.[uv] =)\s*(corner_uv\[previous\].*?edge_t\));',
                   rb'\1 (Sint16)jp_sdk_motion_to_int(\2);',clip,flags=re.S)
    assert n==2
    result=prelude+decoder+b'\nextern NJS_VECTOR light2;\nint32_t jp_sdk_motion_to_int(float);\n'+intensity+b''.join(macros)
    result+=clip.rstrip()[:-1]+b'    return sq_cursor;\n}\n'
    for clipped in (False,True):
        suffix='_clip' if clipped else ''
        branch=owner_function(raw,'simple_draw_smooth_tex_polygons'+suffix)
        branch=branch.split(b'    case NJD_MESHSET_3:',1)[1].split(b'        break;',1)[0]
        if clipped:
            a=branch.index(b'                clip_cull_requested = 1;')
            old=b'                simple_clip_smooth_tex_triangle(v0, v1, v2, corner_uv_window);'
            z=branch.index(old,a)
            branch=branch[:a]+b'                sq_cursor=simple_clip_smooth_tex_triangle(v0,v1,v2,corner_uv,sq_cursor,projection_x,projection_y,iamb);\n                corner_uv+=3;'+branch[z+len(old):]
        sig='Float *jp_simple_emit_smooth_tex'+suffix+'(NJS_MESHSET *meshset,Float *sq_cursor,'+(
            'Float projection_x,Float projection_y,' if clipped else '')+'Float iamb)\n'
        result+=sig.encode()+b'{\nSint16 *mesh_cursor=meshset->meshes;\nNJS_COLOR *corner_uv=meshset->vertuv;\n'
        result+=b'struct calc_vertex *v0,*v1,*v2;\nSint32 polygon;Float facing;\n'
        if clipped:result+=b'Uint32 clip_and,clip_or;\n'
        result+=branch+b'    return sq_cursor;\n}\n'
    return result


def bridge(raw):
    raw=smooth_clip_bridge(raw)
    raw=raw.replace(b'jp_simple_emit_smooth_triangles',b'jp_simple_emit_smooth_tex')
    raw=raw.replace(b'jp_simple_draw_smooth_triangles',b'jp_simple_draw_smooth_tex_triangles')
    raw=replace(raw,b'size_t indices,size_t materials,',b'size_t indices,size_t materials,size_t uvs,')
    raw=replace(raw,b'    unsigned corners=3u*mesh->nbMesh;',b'    unsigned corners=3u*mesh->nbMesh;\n    if(!mesh->vertuv || corners>uvs)return 100;')
    raw=replace(raw,b'dispatch[4]==4',b'dispatch[4]==2')
    raw=replace(raw,b'    njSetkmModelAttr(dispatch[0]);',
                b'    if(njSetkmTextureNum(dispatch[2])!=1)return 102;\n    njSetkmModelAttr(dispatch[0]);')
    raw=replace(raw,b'    Float *cursor=lane==JP_QUEUE_TRANSLUCENT?njStartVertexNonTexTrans():njStartVertexNonTexOpaque();',
                b'    Uint32 *color=(Uint32 *)&_nj_light_material_.diffuse;\n'
                b'    Float *cursor=lane==JP_QUEUE_TRANSLUCENT?njStartVertexTexTransIntens(color):njStartVertexTexOpaqueIntens(color);')
    return replace(raw,b'if(dispatch[1])return 100;',b'if(dispatch[1]!=NJD_FLAG_USE_TEXTURE)return 100;')


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

    src=out/'smooth_tex_original.c'
    src.write_bytes(derive(raw))
    tree=json.loads(run(sdk+['-Xclang','-ast-dump=json','-fsyntax-only',str(src)],'ast'))
    native,stats=rewrite(src.read_bytes(),tree,['smooth_intensity','decode_unlit_point','simple_clip_smooth_tex_triangle','jp_simple_emit_smooth_tex','jp_simple_emit_smooth_tex_clip'])
    rz=out/'smooth_tex_rz.c'
    rz.write_bytes(native)
    ir=out/'smooth_tex.ll'
    run(sdk+['-S','-emit-llvm',str(rz),'-o',str(ir)],'llvm')
    assert not re.search(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',ir.read_text())
    caller=out/'smooth_tex_caller.c'
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
    link=b['simple_smooth_clip']['link'].copy()
    link[link.index('-o'):link.index('-o')]=list(map(str,objects))
    link[-1]=str(dll)
    rsp=out/'link.rsp'
    rsp.write_text('\n'.join('"'+x.replace('\\','/')+'"' for x in link[1:])+'\n')
    run([link[0],'@'+str(rsp)],'link')
    b.update(frames_DLL=str(dll),frames_DLL_sha256=digest(dll),simple_smooth_tex=dict(
        baseline_sha256=digest(base/'build_verification.json'),upstream_commit=PIN,arithmetic=stats,
        commands=commands,link=link,
        generated_sha256={f.name:digest(f) for f in (src,rz,ir,caller,*objects)}))
    (out/'build_verification.json').write_text(json.dumps(b,indent=2)+'\n')
    print('Smooth-lit textured triangle drawer compiled and linked',flush=True)


if __name__=='__main__':
    main()
