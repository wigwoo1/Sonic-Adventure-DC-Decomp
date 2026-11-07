'''Recover original unlit untextured quads and N-gons.'''
import argparse
import json
import re
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import PIN, owner_function
from build_native_simple_material import digest
from build_native_simple_unlit_clip import derive as triangle_source, replace
from build_native_simple_env_strips import caller as strip_caller
from rewrite_sprite_float_operations import rewrite

functions=['decode_unlit_point','simple_clip_unlit_triangle',
           'jp_simple_emit_unlit_polygons','jp_simple_emit_unlit_polygons_clip']


def derive(raw):
    src=triangle_source(raw)
    start=src.index(b'Float *jp_simple_emit_unlit_triangles(')
    end=src.index(b'#define CLIP_NEAR',start)
    src=src[:start]+src[end:src.index(b'Float *jp_simple_emit_unlit_triangles_clip(')]
    src=replace(src,b'Float projection_y)\n{\n    struct calc_vertex *vertices[3];',
                b'Float projection_y,int *cull)\n{\n    struct calc_vertex *vertices[3];')
    src=replace(src,b'if (view_facing < 0.0F) {\n        return sq_cursor;\n    }',
                b'if ((view_facing < 0.0F) & *cull) {\n        return sq_cursor;\n    }\n    *cull=0;')
    for suffix in ('','_clip'):
        original=owner_function(raw,'simple_draw_unlit_polygons'+suffix)
        body=original[original.index(b'    case NJD_MESHSET_4:'):original.index(b'    case NJD_MESHSET_TRIMESH:')]
        if suffix:
            body=body.replace(b'clip_cull_requested',b'cull')
            body,n=re.subn(rb'simple_clip_unlit_triangle\((v[012], v[123], v[23])\);',
                           rb'sq_cursor=simple_clip_unlit_triangle(\1,sq_cursor,projection_x,projection_y,&cull);',body)
            assert n==3
        sig='Float *jp_simple_emit_unlit_polygons'+suffix+'(NJS_MESHSET *meshset,Float *sq_cursor'+(
            ',Float projection_x,Float projection_y' if suffix else '')+')\n'
        src+=sig.encode()+b'{\nSint16 *mesh_cursor=meshset->meshes;\n'
        src+=b'struct calc_vertex *v0,*v1,*v2,*v3;\nSint32 polygon,corner,corner_count;Float facing;\n'
        if suffix:src+=b'Uint32 clip_and,clip_or;int cull;\n'
        src+=b'switch(meshset->type_matId&NJD_MESHSET_MASK){\n'+body+b'}\nreturn sq_cursor;\n}\n'
    return src


def caller(raw):
    raw=strip_caller(raw)
    raw=replace(raw,b'struct simple_mesh_span {',
                b'Float *jp_simple_emit_unlit_polygons(NJS_MESHSET *,Float *);\n'
                b'Float *jp_simple_emit_unlit_polygons_clip(NJS_MESHSET *,Float *,Float,Float);\n'
                b'struct simple_mesh_span {')
    raw=replace(raw,b'    int strip=(mesh->type_matId&NJD_MESHSET_MASK)==NJD_MESHSET_TRIMESH;\n    if(!mesh->meshes',
                b'    unsigned kind=mesh->type_matId&NJD_MESHSET_MASK;\n    if(!mesh->meshes')
    raw=replace(raw,b'        unsigned n=3;\n        if(strip){\n            if(pos>=span->indices)return SIMPLE_INVALID;\n            n=(Uint16)mesh->meshes[pos++]&0x7fff;\n        }',
                b'        unsigned n=kind==NJD_MESHSET_4?4:3;\n'
                b'        if(kind==NJD_MESHSET_TRIMESH || kind==NJD_MESHSET_N){\n'
                b'            if(pos>=span->indices)return SIMPLE_INVALID;\n'
                b'            n=(Uint16)mesh->meshes[pos++];\n'
                b'            if(kind==NJD_MESHSET_TRIMESH)n&=0x7fff;\n'
                b'        }')
    raw=replace(raw,b'    if(unlit && !textured)\n',
                b'    if((mesh->type_matId&NJD_MESHSET_MASK)!=NJD_MESHSET_3)\n'
                b'        return clipped?jp_simple_emit_unlit_polygons_clip(mesh,cursor,scales[0],scales[1]):\n'
                b'                       jp_simple_emit_unlit_polygons(mesh,cursor);\n'
                b'    if(unlit && !textured)\n')
    raw=replace(raw,b'        if(kind!=NJD_MESHSET_3 && kind!=NJD_MESHSET_TRIMESH)return SIMPLE_UNSUPPORTED;\n',b'')
    return replace(raw,b'        if((flags&NJD_FLAG_USE_ENV) && !textured)',
                   b'        if((kind==NJD_MESHSET_4 || kind==NJD_MESHSET_N) && (!unlit || textured))return SIMPLE_UNSUPPORTED;\n'
                   b'        if((flags&NJD_FLAG_USE_ENV) && !textured)')


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
    parent=b['simple_env_strips']
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
    src=out/'polygons_original.c'
    src.write_bytes(derive(raw))
    tree=json.loads(run(sdk+['-Xclang','-ast-dump=json','-fsyntax-only',str(src)],'ast'))
    native,stats=rewrite(src.read_bytes(),tree,functions)
    rz=out/'polygons_rz.c'
    rz.write_bytes(native)
    ir=out/'polygons.ll'
    run(sdk+['-S','-emit-llvm',str(rz),'-o',str(ir)],'llvm')
    assert not re.search(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',ir.read_text())
    bridge=out/'polygons_caller.c'
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
    name='env_strips_caller.obj'
    previous=base/name
    assert digest(previous)==parent['generated_sha256'][name]
    index=link.index(str(previous))
    link[index:index+1]=list(map(str,objects))
    link[-1]=str(dll)
    rsp=out/'link.rsp'
    rsp.write_text('\n'.join('"'+x.replace('\\','/')+'"' for x in link[1:])+'\n')
    run([link[0],'@'+str(rsp)],'link')
    b.update(frames_DLL=str(dll),frames_DLL_sha256=digest(dll),simple_color_polygons=dict(
        baseline_sha256=digest(base/'build_verification.json'),upstream_commit=PIN,arithmetic=stats,link=link,
        generated_sha256={f.name:digest(f) for f in (src,rz,ir,bridge,*objects)}))
    (out/'build_verification.json').write_text(json.dumps(b,indent=2)+'\n')
    print('Unlit quad and N-gon drawers compiled and linked',flush=True)


if __name__=='__main__':
    main()
