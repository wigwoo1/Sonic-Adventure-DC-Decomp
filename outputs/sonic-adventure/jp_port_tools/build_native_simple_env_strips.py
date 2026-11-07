'''Recover the original environment strip drawers and local FTRV matrix.'''
import argparse
import json
import re
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import PIN, owner_function
from build_native_simple_material import digest
from build_native_simple_env_triangles import derive as env_triangles, caller as triangle_caller, functions as triangle_functions
from build_native_simple_unlit_clip import replace
from rewrite_sprite_float_operations import rewrite

strips=[f'simple_draw_{mode}_env_strips_{path}' for mode in ('unlit','smooth','flat_path') for path in ('cull','near')]
functions=triangle_functions+strips


def derive(raw):
    src=env_triangles(raw)
    for mode in ('unlit','smooth','flat'):
        name='simple_clip_'+mode+'_env_triangle'
        clip=owner_function(src,name)
        start=clip.index(b'\n{')
        shared=clip[:start-1]+b',int cull)'+clip[start:]
        shared=replace(shared,b'if (view_facing < 0.0F)',b'if (cull && view_facing < 0.0F)')
        src=replace(src,clip,shared)
        src,n=re.subn(name.encode()+rb'\((v0, v1, v2[^;\n]+)\);',name.encode()+rb'(\1,1);',src)
        assert n==1
    src+=b'''
#include "../third_party/skmp-sh4-fpu/sh4_fpu_approx.h"
#include <string.h>
#define DRAW_NOOP() ((void)0)
#define VERTEX_BYTES 32
#define VERTEX_SHIFT 5
#define STRIP_COUNT 0x7fff

/* Apply the original START row changes to a local copy of XMTRX. */
static void environment_matrix(uint32_t matrix[16],Float ambient,int lit)
{
    Float values[16];
    memcpy(values,jp_ninja_current_matrix_storage(),sizeof values);
    for(unsigned i=0;i<3;i++){
        values[4*i]=jp_sprite_multiply(values[4*i],0.5f);
        values[4*i+1]=jp_sprite_multiply(values[4*i+1],0.5f);
    }
    values[12]=values[13]=0.5f;
    if(lit){
        values[2]=jp_sprite_negate(light2.x);
        values[6]=jp_sprite_negate(light2.y);
        values[10]=jp_sprite_negate(light2.z);
        values[14]=ambient;
    }
    memcpy(matrix,values,sizeof values);
}

static Float *emit_env_strip(struct calc_vertex *vertex,Uint32 command,Float *cursor,
                             uint32_t matrix[16],int lit)
{
    uint32_t vector[4],positions[3];
    memcpy(vector,&vertex->normal,12);
    vector[3]=0x3f800000u;
    sh4_ftrv(matrix,vector,1);
    if(!lit)vector[2]=0x3f800000u;
    positions[0]=command;
    memcpy(positions+1,&vertex->sx,8);
    memcpy(cursor,positions,12);
    cursor[3]=vertex->oow;
    memcpy(cursor+4,vector,12);
    return cursor+8;
}
#define NJM_SET_ENV_STRIP(v,c) (sq_cursor=emit_env_strip(v,c,sq_cursor,mapping,1))
#define NJM_SET_ENV_X_STRIP(v,c) (sq_cursor=emit_env_strip(v,c,sq_cursor,mapping,0))
'''
    for name in strips:
        near=name.endswith('near')
        unlit='_unlit_' in name
        body=owner_function(raw,name).replace(b'register ',b'')
        body=re.sub(rb'/\*.*?\*/|^#pragma (?:asm|endasm)[^\r\n]*|^[ \t]*prefetch\(&vertices[^\r\n]*',b'',
                    body,flags=re.S|re.M)
        body=replace(body,('static void '+name+'(NJS_MESHSET *meshset)').encode(),
                     ('Float *'+name+'(NJS_MESHSET *meshset,Float *sq_cursor'+
                      (',Float projection_x,Float projection_y' if near else '')+',Float iamb)').encode())
        body=replace(body,b'    DRAW_NOOP();',b'    uint32_t mapping[16];environment_matrix(mapping,iamb,'+
                     (b'0' if unlit else b'1')+b');')
        body=body.replace(b'(Uint32)run_start < (Uint32)sq_cursor',b'(uintptr_t)run_start < (uintptr_t)sq_cursor')
        body=body.replace(b'(Uint32)sq_cursor - (Uint32)run_start',b'(uintptr_t)sq_cursor - (uintptr_t)run_start')
        if near:
            body=replace(body,b'                Float view_z;\n',b'')
            body,n=re.subn(rb'DECODE_VIEW_POINT\(triangle\[corner\], view_points\[corner\],\s*view_z\);',
                           b'decode_unlit_point(triangle[corner], &view_points[corner], projection_x, projection_y);',body)
            assert n==1
            body=replace(body,b'                clip_cull_requested = 0;\n',b'')
            body=body.replace(b'SIMPLE_INTENSITY(flat_normal)',b'flat_intensity_value(flat_normal,iamb)')
            mode='unlit' if unlit else 'flat' if '_flat_' in name else 'smooth'
            clip=('simple_clip_'+mode+'_env_triangle').encode()
            args=b'v0, v1, v2'+(b', flat_intensity' if mode=='flat' else b'')
            body=replace(body,clip+b'('+args+b');',b'sq_cursor='+clip+b'('+args+
                         b',sq_cursor,projection_x,projection_y'+(b',iamb' if mode=='smooth' else b'')+b',0);')
        src+=body.rstrip()[:-1]+b'    return sq_cursor;\n}\n'
    return src


def caller(raw):
    raw=triangle_caller(raw)
    a=raw.index(b'struct simple_mesh_span')
    for name in strips:
        params=b',Float,Float,Float' if name.endswith('near') else b',Float'
        raw=raw[:a]+b'Float *'+name.encode()+b'(NJS_MESHSET *,Float *'+params+b');\n'+raw[a:]
    code=b'    if((flags&NJD_FLAG_USE_ENV) && strip){\n'
    for mode,condition in [('unlit','if(unlit)'),('flat_path','if(flags&NJD_FLAG_USE_FLAT)'),('smooth','')]:
        prefix='simple_draw_'+mode+'_env_strips_'
        code+=f'        {condition}return clipped?{prefix}near(mesh,cursor,scales[0],scales[1],ambient):{prefix}cull(mesh,cursor,ambient);\n'.encode()
    raw=replace(raw,b'    if(flags&NJD_FLAG_USE_ENV){',code+b'    }\n    if(flags&NJD_FLAG_USE_ENV){')
    return replace(raw,b'(!textured || kind!=NJD_MESHSET_3)',b'!textured')


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
    parent=b['simple_env_triangles']
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
    src=out/'env_strips_original.c'
    src.write_bytes(derive(raw))
    tree=json.loads(run(sdk+['-Xclang','-ast-dump=json','-fsyntax-only',str(src)],'ast'))
    native,stats=rewrite(src.read_bytes(),tree,functions)
    rz=out/'env_strips_rz.c'
    rz.write_bytes(native)
    ir=out/'env_strips.ll'
    run(sdk+['-S','-emit-llvm',str(rz),'-o',str(ir)],'llvm')
    assert not re.search(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',ir.read_text())
    bridge=out/'env_strips_caller.c'
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
    for name,obj in zip(('env_rz.obj','env_caller.obj'),objects):
        previous=base/name
        assert digest(previous)==parent['generated_sha256'][name]
        link[link.index(str(previous))]=str(obj)
    link[-1]=str(dll)
    rsp=out/'link.rsp'
    rsp.write_text('\n'.join('"'+x.replace('\\','/')+'"' for x in link[1:])+'\n')
    run([link[0],'@'+str(rsp)],'link')
    b.update(frames_DLL=str(dll),frames_DLL_sha256=digest(dll),simple_env_strips=dict(
        baseline_sha256=digest(base/'build_verification.json'),upstream_commit=PIN,arithmetic=stats,link=link,
        generated_sha256={f.name:digest(f) for f in (src,rz,ir,bridge,*objects)}))
    (out/'build_verification.json').write_text(json.dumps(b,indent=2)+'\n')
    print('Environment triangle and strip drawers compiled and linked',flush=True)


if __name__=='__main__':
    main()
