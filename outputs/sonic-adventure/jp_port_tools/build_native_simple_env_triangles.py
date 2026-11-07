'''Adapt original environment-mapped triangles.'''
import argparse
import json
import re
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import PIN, owner_function
from build_native_simple_material import digest
from build_native_simple_flat_tex import derive as flat_tex
from build_native_simple_smooth_tex import derive as smooth_tex
from build_native_simple_unlit_tex_clip import derive as unlit_tex
from build_native_simple_flat_strips import caller as strip_caller
from build_native_simple_unlit_clip import replace
from rewrite_sprite_float_operations import rewrite

modes=('unlit','smooth','flat')
functions=['flat_intensity_value','smooth_intensity','decode_unlit_point','env_column','emit_env']
functions += ['simple_clip_'+mode+'_env_triangle' for mode in modes]
functions += ['jp_simple_'+mode+'_env_triangles'+suffix for mode in modes for suffix in ('','_clip')]


def derive(raw):
    src=flat_tex(raw).split(b'static Float *simple_clip_flat_tex_triangle',1)[0]
    smooth=smooth_tex(raw)
    src+=smooth[smooth.index(b'static Float smooth_intensity'):smooth.index(b'static Float *simple_clip_smooth_tex_triangle')]
    unlit=unlit_tex(raw)
    src+=unlit[unlit.index(b'#define EMIT_UNLIT_TEX('):unlit.index(b'static Float *simple_clip_unlit_tex_triangle')]
    dot=raw.split(b'#define ENV_COLUMN_DOT(normal, column)',1)[1].split(b'/*',1)[0]
    dot=re.sub(rb'\\[ \t]*\r?\n',b'\n',dot).strip()
    src+=b'\nfloat *jp_ninja_current_matrix_storage(void);\nstatic Float env_column(NJS_VECTOR normal,unsigned column)\n{Float *mp=jp_ninja_current_matrix_storage();return '+dot+b';}\n'
    src+=b'''static Float *emit_env(struct calc_vertex *vertex,Uint32 command,Float intensity,int write_intensity,Float *sq_cursor)
{
    *(Uint32 *)sq_cursor=command;
    sq_cursor[1]=vertex->sx;
    sq_cursor[2]=vertex->sy;
    sq_cursor[3]=vertex->oow;
    sq_cursor[4]=env_column(vertex->normal,0)*0.5F+0.5F;
    sq_cursor[5]=env_column(vertex->normal,1)*0.5F+0.5F;
    if(write_intensity)sq_cursor[6]=intensity;
    return sq_cursor+8;
}
'''
    for mode in modes:
        name='simple_clip_'+mode+'_env_triangle'
        clip=owner_function(raw,name)
        clip=replace(clip,b'static void ',b'static Float *')
        clip,n=re.subn(rb'(struct calc_vertex \*v2(?:,\s+Float intensity)?)\)\n\{',
                       rb'\1,Float *sq_cursor,Float projection_x,Float projection_y'+
                       (b',Float iamb' if mode=='smooth' else b'')+rb')\n{',clip)
        assert n==1,(mode,n)
        clip=replace(clip,b'    Float view_z;\n',b'    struct calc_vertex clipped_vertices[4];\n    NJS_TEX clipped_uv[4];\n')
        clip=clip.replace(b'ENV_COLUMN_DOT(',b'env_column(')
        clip=clip.replace(b'SIMPLE_INTENSITY(vertex->normal)',b'smooth_intensity(vertex,iamb)')
        clip=clip.replace(b'SIMPLE_INTENSITY(vertices[corner]->normal)',b'smooth_intensity(vertices[corner],iamb)')
        clip=replace(clip,b'DECODE_VIEW_POINT(vertices[corner], view_points[corner], view_z);',
                     b'decode_unlit_point(vertices[corner], &view_points[corner], projection_x, projection_y);')
        clip=replace(clip,b'if ((view_facing < 0.0F) & clip_cull_requested) {\n        return;\n    }\n    clip_cull_requested = 0;',
                     b'if (view_facing < 0.0F) {\n        return sq_cursor;\n    }')
        clip,n=re.subn(rb'(quantized_uv\[corner\]\.[uv] =)\s*(?:\(Sint16\)\()?(\(env_column\([^\n;]+? \+ 128\.0F)\)?;',
                       rb'\1 (Sint16)jp_sdk_motion_to_int(\2);',clip)
        assert n==2,(mode,n)
        clip,n=re.subn(rb'(clipped_uv\[output_count\]\.[uv] =)\s*(quantized_uv\[previous\].*?edge_t\));',
                       rb'\1 (Sint16)jp_sdk_motion_to_int(\2);',clip,flags=re.S)
        assert n==2
        src+=clip.rstrip()[:-1]+b'    return sq_cursor;\n}\n'
        for suffix in ('','_clip'):
            branch=owner_function(raw,'simple_draw_'+mode+'_env_polygons'+suffix)
            branch=branch.split(b'    case NJD_MESHSET_3:',1)[1].split(b'        break;',1)[0]
            branch=branch.replace(b'SIMPLE_INTENSITY(flat_normal)',b'flat_intensity_value(flat_normal,iamb)')
            for macro,value,write in [(b'EMIT_FLAT_ENV_POSITION',b'0',b'0'),(b'EMIT_SMOOTH_ENV_POLYGON',rb'smooth_intensity(\1,iamb)',b'1')]:
                branch=re.sub(macro+rb'\((v[012]),\s*(KM_VERTEXPARAM_\w+)\);',
                              rb'sq_cursor=emit_env(\1,\2,'+value+b','+write+b',sq_cursor);',branch)
            branch=re.sub(rb'EMIT_FLAT_ENV_INTENSITY\((v[012]),\s*(KM_VERTEXPARAM_\w+),\s*([^\)]+)\);',
                          rb'sq_cursor=emit_env(\1,\2,\3,1,sq_cursor);',branch)
            if suffix:
                branch=replace(branch,b'                clip_cull_requested = 1;\n',b'')
                branch,n=re.subn(name.encode()+rb'\((v0, v1, v2(?:, flat_intensity)?)\);',
                                 b'sq_cursor='+name.encode()+rb'(\1,sq_cursor,projection_x,projection_y'+
                                 (b',iamb' if mode=='smooth' else b'')+b');',branch)
                assert n==1
            sig='Float *jp_simple_'+mode+'_env_triangles'+suffix+'(NJS_MESHSET *meshset,Float *sq_cursor,'+(
                'Float projection_x,Float projection_y,' if suffix else '')+'Float iamb)\n'
            src+=sig.encode()+b'{\n(void)iamb;\nSint16 *mesh_cursor=meshset->meshes;\n'
            src+=b'struct calc_vertex *v0,*v1,*v2;\nSint32 polygon;Float facing;\n'
            if mode=='flat':src+=b'NJS_VECTOR flat_normal;Float flat_intensity;\n'
            if suffix:
                src+=b'Uint32 clip_and,clip_or;\n'
                if mode=='flat':src+=b'struct calc_vertex *v0_copy;\n'
            src+=branch+b'    return sq_cursor;\n}\n'
    return src


def caller(raw):
    raw=strip_caller(raw)
    a=raw.index(b'struct simple_mesh_span')
    for name in functions[-6:]:
        params=b',Float,Float,Float' if name.endswith('_clip') else b',Float'
        raw=raw[:a]+b'Float *'+name.encode()+b'(NJS_MESHSET *,Float *'+params+b');\n'+raw[a:]
    code=b'    if(flags&NJD_FLAG_USE_ENV){\n'
    for mode,condition in [('unlit','if(unlit)'),('flat','if(flags&NJD_FLAG_USE_FLAT)'),('smooth','')]:
        prefix='jp_simple_'+mode+'_env_triangles'
        code+=f'        {condition}return clipped?{prefix}_clip(mesh,cursor,scales[0],scales[1],ambient):{prefix}(mesh,cursor,ambient);\n'.encode()
    raw=replace(raw,b'    if(strip){\n        if(!unlit && (flags&NJD_FLAG_USE_FLAT))',
                code+b'    }\n    if(strip){\n        if(!unlit && (flags&NJD_FLAG_USE_FLAT))')
    raw=replace(raw,b'if(flags&NJD_FLAG_USE_ENV)return SIMPLE_UNSUPPORTED;',
                b'if((flags&NJD_FLAG_USE_ENV) && (!textured || kind!=NJD_MESHSET_3))return SIMPLE_UNSUPPORTED;')
    raw=replace(raw,b'if(!unlit && !model->normals)',b'if((!unlit || (flags&NJD_FLAG_USE_ENV)) && !model->normals)')
    return replace(raw,b'if(!mesh->vertuv || corners>spans[i].uvs)',b'if(!(flags&NJD_FLAG_USE_ENV) && (!mesh->vertuv || corners>spans[i].uvs))')


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
    parent=b['simple_flat_strips']
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
    src=out/'env_original.c'
    src.write_bytes(derive(raw))
    tree=json.loads(run(sdk+['-Xclang','-ast-dump=json','-fsyntax-only',str(src)],'ast'))
    native,stats=rewrite(src.read_bytes(),tree,functions)
    rz=out/'env_rz.c'
    rz.write_bytes(native)
    ir=out/'env.ll'
    run(sdk+['-S','-emit-llvm',str(rz),'-o',str(ir)],'llvm')
    assert not re.search(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',ir.read_text())
    bridge=out/'env_caller.c'
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
    previous=base/'flat_strips_caller.obj'
    assert digest(previous)==parent['generated_sha256']['flat_strips_caller.obj']
    index=link.index(str(previous))
    link[index:index+1]=list(map(str,objects))
    link[-1]=str(dll)
    rsp=out/'link.rsp'
    rsp.write_text('\n'.join('"'+x.replace('\\','/')+'"' for x in link[1:])+'\n')
    run([link[0],'@'+str(rsp)],'link')
    b.update(frames_DLL=str(dll),frames_DLL_sha256=digest(dll),simple_env_triangles=dict(
        baseline_sha256=digest(base/'build_verification.json'),upstream_commit=PIN,arithmetic=stats,link=link,
        generated_sha256={f.name:digest(f) for f in (src,rz,ir,bridge,*objects)}))
    (out/'build_verification.json').write_text(json.dumps(b,indent=2)+'\n')
    print('Environment triangle drawers compiled and linked',flush=True)


if __name__=='__main__':
    main()
