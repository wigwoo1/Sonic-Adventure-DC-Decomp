"""Extend the qualified authored GPU core, preserving the actual game owner.
Every predecessor source/object and proof remains intact in its old directory.
Only authored GPU/window code changes; linked game/SDK bodies stay private.
"""
import argparse,hashlib,json,re,shutil,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','baseline','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args()
    for n in vars(a):setattr(a,n,getattr(a,n).resolve())
    r,base,c,d=a.workspace,a.baseline,a.compiler,a.output;p=r/'outputs/sonic-adventure';b=read(base/'build_verification.json');gdir=Path(b['GPU_DLL']).parent;g=read(gdir/'build_verification.json');fb=read(Path(b['frame_build'])/'build_verification.json')
    assert b['diagnostic_only_fog_setters_removed'] and sha(c)==b['compiler_sha256']==g['compiler_sha256']
    for report in [b,g]:
        for n,h in report['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(b['frames_DLL'])==b['frames_DLL_sha256'] and sha(b['GPU_DLL'])==g['GPU_DLL_sha256']
    original=gdir/'authored_GPU_window_input.c';assert sha(original)==g['generated_sources_sha256'][original.name]
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists();d.mkdir()
    raw=original.read_text();before=raw;start=raw.index('static const char shader[]=');end=raw.index('static int fail(',start);raw=raw[:start]+'#include "jp_native_gpu_fog_shader.h"\n'+raw[end:]
    changes={
        'struct constants { float logical[2];uint32_t textured,shade,alpha,ignore_alpha,offset,punch;float threshold,padding[3]; };':'/* Constants and shaders are defined together by the authored fog header. */',
        'float x,y,q,depth,u,v,base[4],offset[4];':'float x,y,q,depth,u,v,base[4],offset[4],fogq;',
        '{"COLOR",1,DXGI_FORMAT_R32G32B32A32_FLOAT,0,40,D3D11_INPUT_PER_VERTEX_DATA,0}':'{"COLOR",1,DXGI_FORMAT_R32G32B32A32_FLOAT,0,40,D3D11_INPUT_PER_VERTEX_DATA,0},\n        {"TEXCOORD",3,DXGI_FORMAT_R32_FLOAT,0,56,D3D11_INPUT_PER_VERTEX_DATA,0}',
        'elements,6,ID3D10Blob_GetBufferPointer(v)':'elements,7,ID3D10Blob_GetBufferPointer(v)',
        'if((pcw&0x000300c0)||((tsp>>22)&3)!=2||(tsp&0x03000000)||':'if((pcw&0x000300c0)||(((tsp>>22)&3)!=2&&!g->snapshot)||(tsp&0x03000000)||',
        'out->u=v[i]->u;out->v=v[i]->v;':'out->u=v[i]->u;out->v=v[i]->v;out->fogq=(has_background&&bi==0)?background_depth:v[i]->inv_w;',
        'struct constants c={{cfg->logical_width,cfg->logical_height},cfg->textures&&!!(pcw&8),(tsp>>6)&3,!!(tsp&0x100000),!!(tsp&0x80000),!!(pcw&4),b->list==2,cfg->punch_alpha,{0}};':'struct constants c={.logical={cfg->logical_width,cfg->logical_height},.textured=cfg->textures&&!!(pcw&8),.shade=(tsp>>6)&3,.alpha=!!(tsp&0x100000),.ignore_alpha=!!(tsp&0x80000),.offset=!!(pcw&4),.punch=b->list==2,.threshold=cfg->punch_alpha,.fog_mode=(tsp>>22)&3};\n        jp_fog_bind_constants(&c,g->snapshot);',
        'a frame-state integration gate. No table fog or secondary buffers.':'a frame-state integration gate. Fog requires captured state; secondary buffers remain unsupported.'}
    for old,new in changes.items():assert raw.count(old)==1,old;raw=raw.replace(old,new)
    generated=d/'authored_GPU_fog.c';generated.write_text(raw);obj=d/'gpu_fog.obj';gpu=d/'jp_native_window_input.dll'
    compile=list(next(x['command'] for x in g['commands'] if '-c' in x['command'] and x['command'][-3]==str(original)));old_obj=compile[-1];compile[-3]=str(generated);compile[-1]=str(obj)
    link=list(next(x['command'] for x in g['commands'] if '-shared' in x['command'] and x['command'][-1]==b['GPU_DLL']));link[link.index(old_obj)]=str(obj)
    old=str(Path(g['frames_DLL']).with_suffix('.lib'));assert link.count(old)==1;link[link.index(old)]=str(Path(b['frames_DLL']).with_suffix('.lib'));link[-1]=str(gpu)
    dependencies={n:sha(n) for n in link if n.endswith('.obj') and n!=str(obj)};commands=[compile,link]
    newlibs={'GPU_DLL':gpu.with_suffix('.lib')}
    for label,name,imports in [('present_DLL','jp_native_present.dll',['GPU_DLL']),('authored_exit_DLL','jp_native_exit.dll',['GPU_DLL','present_DLL']),('DLL','jp_native_logo_start.dll',['present_DLL','authored_exit_DLL'])]:
        cmd=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b[label]))
        for n in imports:
            old=str(Path(b[n]).with_suffix('.lib'));assert cmd.count(old)==1;cmd[cmd.index(old)]=str(newlibs[n])
        cmd[-1]=str(d/name);newlibs[label]=Path(cmd[-1]).with_suffix('.lib');commands.append(cmd)
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/('build_'+str(i)+'.txt');log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6000]
    for n,h in dependencies.items():assert sha(n)==h,n
    for n,h in b['generated_sources_sha256'].items():shutil.copyfile(base/n,d/n);assert sha(d/n)==h
    shutil.copytree(base/'inputs',d/'inputs')
    authored=dict(b['authored_source_sha256'],**g['authored_source_sha256'])
    for f in [p/'native/jp_native_gpu_fog_shader.h',Path(__file__).resolve()]:authored[f.relative_to(r).as_posix()]=sha(f)
    sources=dict(b['generated_sources_sha256']);sources[generated.name]=sha(generated)
    fd=d/'frame_view';fd.mkdir()
    for n in sources:shutil.copyfile(d/n,fd/n)
    frame=dict(fb,commands=results,GPU_DLL=str(gpu),GPU_DLL_sha256=sha(gpu),DLL=str(d/'jp_native_present.dll'),DLL_sha256=sha(d/'jp_native_present.dll'),authored_source_sha256=authored,generated_sources_sha256=sources,visible_GPU_fog_implemented=True,game_containing_DLLs_publishable=False)
    (fd/'build_verification.json').write_text(json.dumps(frame,indent=2)+'\n')
    result=dict(b,status='captured_frame_GPU_fog_compiled_behavior_unverified',commands=results,DLL=str(d/'jp_native_logo_start.dll'),DLL_sha256=sha(d/'jp_native_logo_start.dll'),GPU_DLL=str(gpu),GPU_DLL_sha256=sha(gpu),present_DLL=str(d/'jp_native_present.dll'),authored_exit_DLL=str(d/'jp_native_exit.dll'),authored_exit_DLL_sha256=sha(d/'jp_native_exit.dll'),frame_build=str(fd),frame_build_sha256=sha(fd/'build_verification.json'),authored_source_sha256=authored,generated_sources_sha256=sources,previous_boot_fog_build_sha256=sha(base/'build_verification.json'),original_authored_GPU_source_sha256=sha(original),generated_GPU_source_sha256=sha(generated),authored_GPU_object_sha256=sha(obj),GPU_existing_objects_sha256=dependencies,exact_GPU_fog_adaptations=changes,shader_header_sha256=sha(p/'native/jp_native_gpu_fog_shader.h'),frame_owner_bytes_unchanged=sha(b['frames_DLL'])==b['frames_DLL_sha256'],visible_GPU_table_fog_implemented=True,visible_GPU_vertex_fog_implemented=True,visible_GPU_mode2_fog_implemented=True,legacy_public_API_ABI_unchanged=True,native_float_fog_policy=True,console_fixed_point_fog_precision_verified=False,authored_GPU_contains_game_or_SDK_bodies=False,artifact_publishable=False,game_containing_DLL_publishable=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
