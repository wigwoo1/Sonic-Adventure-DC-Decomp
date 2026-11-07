'''Recover flat-lit color and textured quads and N-gons.'''
import argparse
import json
import re
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import PIN, owner_function
from build_native_simple_material import digest
from build_native_simple_flat_color import derive as color_source
from build_native_simple_flat_tex import derive as texture_source
from build_native_simple_unlit_clip import replace
from build_native_simple_smooth_polygons import caller as smooth_caller
from rewrite_sprite_float_operations import rewrite

functions=['decode_unlit_point','flat_intensity_value','simple_clip_constant_color_triangle','simple_clip_flat_tex_triangle']
functions += ['jp_simple_flat_'+mode+'_polygons'+suffix for mode in ('color','tex') for suffix in ('','_clip')]


def derive(raw):
    src=color_source(raw).split(b'Float *jp_simple_flat_triangles(',1)[0]
    tex=texture_source(raw)
    a=tex.index(b'#define EMIT_FLAT_TEX_POSITION')
    b=tex.index(b'extern NJS_VECTOR light2;',a)
    c=tex.index(b'static Float *simple_clip_flat_tex_triangle',b)
    src+=tex[a:b]+tex[c:tex.index(b'Float *jp_simple_flat_tex_triangles(',c)]
    for textured in (False,True):
        mode='tex' if textured else 'color'
        original='simple_draw_flat_'+('tex_' if textured else '')+'polygons'
        clipname='simple_clip_flat_tex_triangle' if textured else 'simple_clip_constant_color_triangle'
        clip=owner_function(src,clipname)
        adjusted=replace(clip,b'Float projection_y)\n{',b'Float projection_y,int *cull)\n{')
        adjusted=replace(adjusted,b'if (view_facing < 0.0F) {\n        return sq_cursor;\n    }',
                         b'if ((view_facing < 0.0F) & *cull) {\n        return sq_cursor;\n    }\n    *cull=0;')
        src=replace(src,clip,adjusted)
        for suffix in ('','_clip'):
            body=owner_function(raw,original+suffix)
            body=body[body.index(b'    case NJD_MESHSET_4:'):body.index(b'    case NJD_MESHSET_TRIMESH:')]
            body=body.replace(b'SIMPLE_INTENSITY(flat_normal)',b'flat_intensity_value(flat_normal,iamb)')
            if suffix:
                body=body.replace(b'clip_cull_requested',b'cull')
                # Preserve upstream GCC's repair for an unset N-gon normal.
                body=replace(body,b'#ifdef GCC\n',b'')
                body=replace(body,b'#endif\n',b'')
                pattern=clipname.encode()+rb'\(\s*(v[012], v[123], v[23],\s*[^;]+)\);'
                body,n=re.subn(pattern,b'sq_cursor='+clipname.encode()+rb'(\1,sq_cursor,projection_x,projection_y,&cull);',body)
                assert n==3,(mode,n)
                body=body.replace(b'(Sint16)(Float)corner_uv->tex.',b'corner_uv->tex.')
            sig='Float *jp_simple_flat_'+mode+'_polygons'+suffix+'(NJS_MESHSET *meshset,Float *sq_cursor,'+(
                'Float projection_x,Float projection_y,' if suffix else '')+'Float iamb)\n'
            src+=sig.encode()+b'{\nSint16 *mesh_cursor=meshset->meshes;\n'
            src+=b'struct calc_vertex *v0,*v1,*v2,*v3;\nSint32 polygon,corner,corner_count;Float facing,flat_intensity;\nNJS_VECTOR flat_normal;\n'
            src+=b'NJS_COLOR *corner_uv=meshset->vertuv;\n' if textured else b'NJS_ARGB flat_color;\n'
            if suffix:
                src+=b'Uint32 clip_and,clip_or;int cull;\n'
                src+=b'NJS_COLOR corner_uv_window[4];\n' if textured else b'struct calc_vertex *v1_copy;\n'
            src+=b'switch(meshset->type_matId&NJD_MESHSET_MASK){\n'+body+b'}\nreturn sq_cursor;\n}\n'
    return src


def caller(raw):
    raw=smooth_caller(raw)
    a=raw.index(b'struct simple_mesh_span')
    for name in functions[-4:]:
        args=b',Float,Float,Float' if name.endswith('_clip') else b',Float'
        raw=raw[:a]+b'Float *'+name.encode()+b'(NJS_MESHSET *,Float *'+args+b');\n'+raw[a:]
    raw=replace(raw,b'    if(!unlit && (mesh->type_matId&NJD_MESHSET_MASK)!=NJD_MESHSET_3){\n',
                b'    if(!unlit && (mesh->type_matId&NJD_MESHSET_MASK)!=NJD_MESHSET_3){\n'
                b'        if(flags&NJD_FLAG_USE_FLAT){\n'
                b'            if(textured)return clipped?jp_simple_flat_tex_polygons_clip(mesh,cursor,scales[0],scales[1],ambient):jp_simple_flat_tex_polygons(mesh,cursor,ambient);\n'
                b'            return clipped?jp_simple_flat_color_polygons_clip(mesh,cursor,scales[0],scales[1],ambient):jp_simple_flat_color_polygons(mesh,cursor,ambient);\n'
                b'        }\n')
    raw=replace(raw,b'    if(!unlit && textured && (flags&NJD_FLAG_USE_FLAT))\n',
                b'    if(!unlit && textured && (flags&NJD_FLAG_USE_FLAT) && (mesh->type_matId&NJD_MESHSET_MASK)==NJD_MESHSET_3)\n')
    raw=replace(raw,b'    if(!unlit && (flags&NJD_FLAG_USE_FLAT))\n',
                b'    if(!unlit && (flags&NJD_FLAG_USE_FLAT) && (mesh->type_matId&NJD_MESHSET_MASK)==NJD_MESHSET_3)\n')
    return replace(raw,b'&& ((!unlit && (flags&NJD_FLAG_USE_FLAT)) || (flags&NJD_FLAG_USE_ENV)))return SIMPLE_UNSUPPORTED;',
                   b'&& (flags&NJD_FLAG_USE_ENV))return SIMPLE_UNSUPPORTED;')


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
    parent=b['simple_smooth_polygons']
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
    src=out/'flat_polygons_original.c'
    src.write_bytes(derive(raw))
    tree=json.loads(run(sdk+['-Xclang','-ast-dump=json','-fsyntax-only',str(src)],'ast'))
    native,stats=rewrite(src.read_bytes(),tree,functions)
    rz=out/'flat_polygons_rz.c'
    rz.write_bytes(native)
    ir=out/'tex_polygons.ll'
    run(sdk+['-S','-emit-llvm',str(rz),'-o',str(ir)],'llvm')
    assert not re.search(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',ir.read_text())
    bridge=out/'flat_polygons_caller.c'
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
    name='smooth_polygons_caller.obj'
    previous=base/name
    assert digest(previous)==parent['generated_sha256'][name]
    index=link.index(str(previous))
    link[index:index+1]=list(map(str,objects))
    link[-1]=str(dll)
    rsp=out/'link.rsp'
    rsp.write_text('\n'.join('"'+x.replace('\\','/')+'"' for x in link[1:])+'\n')
    run([link[0],'@'+str(rsp)],'link')
    b.update(frames_DLL=str(dll),frames_DLL_sha256=digest(dll),simple_flat_polygons=dict(
        baseline_sha256=digest(base/'build_verification.json'),upstream_commit=PIN,arithmetic=stats,link=link,
        generated_sha256={f.name:digest(f) for f in (src,rz,ir,bridge,*objects)}))
    (out/'build_verification.json').write_text(json.dumps(b,indent=2)+'\n')
    print('Flat-lit quad and N-gon drawers compiled and linked',flush=True)


if __name__=='__main__':
    main()
