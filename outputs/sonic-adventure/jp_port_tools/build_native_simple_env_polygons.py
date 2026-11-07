'''Recover original environment-mapped quads and N-gons.'''
import argparse
import json
import re
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import PIN, owner_function
from build_native_simple_material import digest
from build_native_simple_env_triangles import derive as env_triangles, modes
from build_native_simple_unlit_clip import replace
from build_native_simple_flat_polygons import caller as flat_caller
from rewrite_sprite_float_operations import rewrite

functions=['flat_intensity_value','smooth_intensity','decode_unlit_point','env_column','emit_env']
functions += ['simple_clip_'+mode+'_env_triangle' for mode in modes]
functions += ['jp_simple_'+mode+'_env_polygons'+suffix for mode in modes for suffix in ('','_clip')]


def derive(raw):
    src=env_triangles(raw)
    for mode in modes:
        for suffix in ('','_clip'):
            src=replace(src,owner_function(src,'jp_simple_'+mode+'_env_triangles'+suffix),b'')
        name='simple_clip_'+mode+'_env_triangle'
        clip=owner_function(src,name)
        end=clip.index(b'\n{')
        assert clip[end-1:end]==b')'
        adjusted=clip[:end-1]+b',int *cull)'+clip[end:]
        adjusted=replace(adjusted,b'if (view_facing < 0.0F) {\n        return sq_cursor;\n    }',
                         b'if ((view_facing < 0.0F) & *cull) {\n        return sq_cursor;\n    }\n    *cull=0;')
        src=replace(src,clip,adjusted)
        for suffix in ('','_clip'):
            body=owner_function(raw,'simple_draw_'+mode+'_env_polygons'+suffix)
            body=body[body.index(b'    case NJD_MESHSET_4:'):body.index(b'    case NJD_MESHSET_TRIMESH:')]
            body=body.replace(b'SIMPLE_INTENSITY(flat_normal)',b'flat_intensity_value(flat_normal,iamb)')
            for macro,value,write in [(b'EMIT_FLAT_ENV_POSITION',b'0',b'0'),
                                      (b'EMIT_SMOOTH_ENV_POLYGON',rb'smooth_intensity(\1,iamb)',b'1')]:
                body=re.sub(macro+rb'\((v[0-3]),\s*(KM_VERTEXPARAM_\w+)\);',
                            rb'sq_cursor=emit_env(\1,\2,'+value+b','+write+b',sq_cursor);',body)
            body=re.sub(rb'EMIT_FLAT_ENV_INTENSITY\((v[0-3]),\s*(KM_VERTEXPARAM_\w+),\s*([^\)]+)\);',
                        rb'sq_cursor=emit_env(\1,\2,\3,1,sq_cursor);',body)
            if suffix:
                body=body.replace(b'clip_cull_requested',b'cull')
                if mode=='flat':
                    body=replace(body,b'#ifdef GCC\n',b'')
                    body=replace(body,b'#endif\n',b'')
                body,n=re.subn(name.encode()+rb'\((v[012], v[123], v[23](?:,\s*flat_intensity)?)\);',
                               b'sq_cursor='+name.encode()+rb'(\1,sq_cursor,projection_x,projection_y'+
                               (b',iamb' if mode=='smooth' else b'')+b',&cull);',body)
                assert n==3,(mode,n)
            sig='Float *jp_simple_'+mode+'_env_polygons'+suffix+'(NJS_MESHSET *meshset,Float *sq_cursor,'+(
                'Float projection_x,Float projection_y,' if suffix else '')+'Float iamb)\n'
            src+=sig.encode()+b'{\n(void)iamb;\nSint16 *mesh_cursor=meshset->meshes;\n'
            src+=b'struct calc_vertex *v0,*v1,*v2,*v3;\nSint32 polygon,corner,corner_count;Float facing;\n'
            if mode=='flat':src+=b'NJS_VECTOR flat_normal;Float flat_intensity;\n'
            if suffix:
                src+=b'Uint32 clip_and,clip_or;int cull;\n'
                if mode=='flat':src+=b'Float v1_x;\n'
            src+=b'switch(meshset->type_matId&NJD_MESHSET_MASK){\n'+body+b'}\nreturn sq_cursor;\n}\n'
    return src


def caller(raw):
    raw=flat_caller(raw)
    a=raw.index(b'struct simple_mesh_span')
    for name in functions[-6:]:
        args=b',Float,Float,Float' if name.endswith('_clip') else b',Float'
        raw=raw[:a]+b'Float *'+name.encode()+b'(NJS_MESHSET *,Float *'+args+b');\n'+raw[a:]
    code=b'    if((flags&NJD_FLAG_USE_ENV) && !strip && (mesh->type_matId&NJD_MESHSET_MASK)!=NJD_MESHSET_3){\n'
    for mode,condition in [('unlit','if(unlit)'),('flat','if(flags&NJD_FLAG_USE_FLAT)'),('smooth','')]:
        name='jp_simple_'+mode+'_env_polygons'
        code+=f'        {condition}return clipped?{name}_clip(mesh,cursor,scales[0],scales[1],ambient):{name}(mesh,cursor,ambient);\n'.encode()
    raw=replace(raw,b'    if((flags&NJD_FLAG_USE_ENV) && strip){',
                code+b'    }\n    if((flags&NJD_FLAG_USE_ENV) && strip){')
    return replace(raw,b'        if((kind==NJD_MESHSET_4 || kind==NJD_MESHSET_N) && (flags&NJD_FLAG_USE_ENV))return SIMPLE_UNSUPPORTED;\n',b'')


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
    parent=b['simple_flat_polygons']
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
    src=out/'env_polygons_original.c'
    src.write_bytes(derive(raw))
    tree=json.loads(run(sdk+['-Xclang','-ast-dump=json','-fsyntax-only',str(src)],'ast'))
    native,stats=rewrite(src.read_bytes(),tree,functions)
    rz=out/'env_polygons_rz.c'
    rz.write_bytes(native)
    ir=out/'tex_polygons.ll'
    run(sdk+['-S','-emit-llvm',str(rz),'-o',str(ir)],'llvm')
    assert not re.search(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',ir.read_text())
    bridge=out/'env_polygons_caller.c'
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
    name='flat_polygons_caller.obj'
    previous=base/name
    assert digest(previous)==parent['generated_sha256'][name]
    index=link.index(str(previous))
    link[index:index+1]=list(map(str,objects))
    link[-1]=str(dll)
    rsp=out/'link.rsp'
    rsp.write_text('\n'.join('"'+x.replace('\\','/')+'"' for x in link[1:])+'\n')
    run([link[0],'@'+str(rsp)],'link')
    b.update(frames_DLL=str(dll),frames_DLL_sha256=digest(dll),simple_env_polygons=dict(
        baseline_sha256=digest(base/'build_verification.json'),upstream_commit=PIN,arithmetic=stats,link=link,
        generated_sha256={f.name:digest(f) for f in (src,rz,ir,bridge,*objects)}))
    (out/'build_verification.json').write_text(json.dumps(b,indent=2)+'\n')
    print('Environment quad and N-gon drawers compiled and linked',flush=True)


if __name__=='__main__':
    main()
