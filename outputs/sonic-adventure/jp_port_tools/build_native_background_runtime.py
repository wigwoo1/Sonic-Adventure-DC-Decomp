"""Extend one frame/GPU owner with real SDK background packets and rendering.
All external SDK bodies and game-containing diagnostic DLLs remain private.
"""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
from build_native_texture_runtime import function
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','baseline','gpu-baseline','compiler','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';u=a.upstream.resolve();base=a.baseline.resolve();gpu=a.gpu_baseline.resolve();d=a.output.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    b=read(base/'build_verification.json');g=read(gpu/'build_verification.json')
    for report in [b,g]:
        assert report['exit_code']==report['diagnostic_bytes']==0 and report['compiler_sha256']==sha(a.compiler)
        for n,h in report['authored_source_sha256'].items():assert sha(r/n)==h,n
    audit=read(r/'work/sonic-adventure/JP_NEXT_NATIVE_BACKGROUND_FADE_SOURCE_AUDIT.json')
    for n,h in audit['external_files_sha256'].items():assert sha(u/n)==h,n
    d.mkdir();(d/'frames').mkdir();commands=[];common=list(b['commands'][0]['command']);common=common[:common.index('-c')]
    raw=(u/'sdk/ninja/bgplane_kamui_lib.c').read_bytes();original_bg_sha=hashlib.sha256(raw).hexdigest()
    repaired_uv=repaired_offset=0;body_repairs={}
    for typ in [4,6,8,12]:
        name='kmiProcessVertex_'+str(typ).zfill(2);body=function(raw,name);adapted=body
        for vertex in [1,2]:
            start=adapted.index(('    PUT_POSITION(next, points['+str(vertex)+']);').encode());end=adapted.find(b'    PUT_POSITION(next, points[',start+8)
            if end<0:end=adapted.index(b'    END_PACKET(',start)
            part=adapted[start:end]
            if typ==4:
                old=b'background_put_offset(isp, next, points[0]->';count=part.count(old);assert count==1
                part=part.replace(old,('background_put_offset(isp, next, points['+str(vertex)+']->').encode());repaired_offset+=count
            else:
                old=b'background_put_uv16(isp, next, points[0]->';count=part.count(old);assert count<=1
                part=part.replace(old,('background_put_uv16(isp, next, points['+str(vertex)+']->').encode())
                repaired_uv+=count
            adapted=adapted[:start]+part+adapted[end:]
        body_repairs[name]=dict(original_sha256=hashlib.sha256(body).hexdigest(),adapted_sha256=hashlib.sha256(adapted).hexdigest());raw=raw.replace(body,adapted)
    assert repaired_uv==5 and repaired_offset==2
    body=function(raw,'kmiProcessVertex_07');duplicate=b'    PACKET[next++] = PACK_INTENSITY(points[0]->fOffsetIntensity);';assert body.count(duplicate)==1;adapted=body.replace(duplicate,b'    /* Native repair: omit the documented duplicate offset word. */')
    body_repairs['kmiProcessVertex_07']=dict(original_sha256=hashlib.sha256(body).hexdigest(),adapted_sha256=hashlib.sha256(adapted).hexdigest());raw=raw.replace(body,adapted)
    start=raw.index(b'#define PACK_ARGB');end=raw.index(b'#define PACK_INTENSITY',start)
    macro=b'#define PACK_ARGB(a,r,g,b) ('+b' | '.join(b'((KMDWORD)(KMINT32)jp_sprite_multiply(('+n+b'),255.0F) << '+str(shift).encode()+b')' for n,shift in [(b'a',24),(b'r',16),(b'g',8),(b'b',0)])+b')\n'
    raw=b'#include "jp_native_sprite_numeric.h"\n'+raw[:start]+macro+raw[end:]
    km=(u/'sdk/ninja/kmglobal_kamui_lib.c').read_bytes();ninja=(u/'sdk/ninja/njSystem.c').read_bytes();wrapper=function(km,'kmSetBackGroundPlane')
    wrapper=wrapper.replace(b'kmSetBackGroundPlane(',b'static_sdk_background_plane(',1)
    sizes=b','.join(b'sizeof(KMVERTEX'+str(i).encode()+b')' for i in range(15))
    raw+=b'\n#include <ninja.h>\n'+wrapper+b'\nstatic const unsigned vertex_sizes[15]={'+sizes+b'};\n'+b'''
__declspec(dllexport) unsigned jp_background_vertex_size(unsigned type)
{return type<15?vertex_sizes[type]:0;}
KMSTATUS kmSetBackGroundPlane(PVOID vertices[3],KMVERTEXTYPE type,KMINT32 bytes)
{
    if((unsigned)type>=15)return KMSTATUS_INVALID_VERTEX_TYPE;
    if(!vertices||!vertices[0]||!vertices[1]||!vertices[2]||bytes<(KMINT32)vertex_sizes[type])return KMSTATUS_INVALID_PARAMETER;
    return static_sdk_background_plane(vertices,type,bytes);
}
'''+function(ninja,'njSetBackColor')+b'\n'
    sdk=d/'external_background_sdk.c';sdk.write_bytes(raw);sdk_obj=d/'background_sdk.obj'
    commands.append(common+['-c',str(sdk),'-o',str(sdk_obj)])
    commands.append(common+['-S','-emit-llvm',str(sdk),'-o',str(d/'external_background_sdk.ll')])
    link=list(next(row['command'] for row in b['commands'] if '-shared' in row['command'] and row['command'][-1]==b['frames_DLL']))
    old=[Path(v) for v in link if v.endswith('.obj') and Path(v).name=='frames.obj'];assert len(old)==1;old=old[0]
    fsource=old.parent/'authored_stateful_frames.c';fb=read(old.parent/'build_verification.json');assert sha(fsource)==fb['generated_authored_source_sha256'];fraw=fsource.read_text()
    before='    unsigned char *bytes=allocation?allocation+32:NULL;';assert fraw.count(before)==1
    pin='''    if(video.background_bytes && (video.background_packet[0]&0x02000000)){
        if(video.background_bytes<12 || video.background_bytes>sizeof video.background_packet){_aligned_free(allocation);return fail(JP_FRAME_STATE);}
        uint32_t before_background=recorded;
        int rc=jp_frames_record_begin(1,video.background_packet[2]&0x1fffff);
        if(rc){_aligned_free(allocation);return rc;}
        jp_frames_record_commit();
        if(!room(cost)){while(recorded>before_background)unpin(recording[--recorded]);_aligned_free(allocation);return fail(JP_FRAME_BUDGET);}
    }
'''
    fraw=fraw.replace(before,pin+before);fpath=d/'authored_background_frames.c';fpath.write_text(fraw);fobj=d/'frames_background.obj';commands.append(common+['-c',str(fpath),'-o',str(fobj)])
    index=next(i for i,v in enumerate(link) if v.endswith('.obj') and Path(v).resolve()==old.resolve());link[index]=str(fobj)
    kept={str(Path(v).resolve()):sha(v) for v in link if v.endswith('.obj') and Path(v)!=fobj}
    exports=['njSetBackColor','kmSetBackGroundPlane','kmiInitializeBackGroundPlane']+['kmiProcessVertex_'+str(i).zfill(2) for i in range(18)]
    at=link.index('-o');link[at:at]=[str(sdk_obj)]+['-Wl,/export:'+n for n in exports];link[link.index('-o')+1]=str(d/'frames/jp_native_frames.dll');commands.append(link)
    source=gpu/'authored_GPU_jobs.c';assert sha(source)==g['generated_authored_source_sha256'];original=source.read_text();new='#include "jp_native_background.h"\n'+original
    modifications={
      's.background_texture||s.background_bytes||':'(s.background_texture && !s.background_bytes)||',
      '    struct jp_gpu_report result={0};':'    int has_background=g->snapshot && g->snapshot->background_bytes;float background_depth=0;\n    if(has_background){\n        struct jp_packet_plan *with_background=NULL;int bg=jp_background_prepend(g->snapshot,plan,cfg->budget,&with_background);\n        if(bg){jp_packets_destroy(plan);return fail(failure,bg==JP_PACKET_BUDGET?JP_GPU_BUDGET:JP_GPU_STATE,0,E_INVALIDARG);}\n        memcpy(&background_depth,&g->snapshot->background_packet[5],4);jp_packets_destroy(plan);plan=with_background;\n    }\n    struct jp_gpu_report result={0};',
      '    int exponent;frexpf(maximum,&exponent);':'    if(has_background && background_depth>maximum)maximum=background_depth;\n    int exponent;frexpf(maximum,&exponent);',
      'unsigned cull=(isp>>27)&3;':'unsigned cull=(has_background && bi==0)?0:(isp>>27)&3;',
      'out->depth=v[i]->inv_w*scale;':'out->depth=((has_background && bi==0)?background_depth:v[i]->inv_w)*scale;',
      'out->base[ch]=clamp(color->base[ch])':'out->base[ch]=((has_background && bi==0)?color->base[ch]:clamp(color->base[ch]))',
      'out->offset[ch]=clamp(color->offset[ch])':'out->offset[ch]=((has_background && bi==0)?color->offset[ch]:clamp(color->offset[ch]))',
      'depth.DepthFunc=(D3D11_COMPARISON_FUNC)(((isp>>29)&7)+1);':'depth.DepthFunc=(D3D11_COMPARISON_FUNC)(((isp>>29)&7)+1);\n        if(has_background && bi==0){depth.DepthFunc=D3D11_COMPARISON_ALWAYS;depth.DepthWriteMask=D3D11_DEPTH_WRITE_MASK_ALL;}',
    }
    for before,after in modifications.items():assert new.count(before)==1,before;new=new.replace(before,after)
    generated=d/'authored_GPU_background.c';generated.write_text(new);gobj=d/'gpu_background.obj';bgobj=d/'background_decode.obj'
    gflags=[str(a.compiler),'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-fno-strict-aliasing','-Wall','-Wextra','-Werror','-I'+str(p/'native')]
    commands.extend([gflags+['-c',str(generated),'-o',str(gobj)],gflags+['-c',str(p/'native/jp_native_background.c'),'-o',str(bgobj)]])
    dependencies=[Path(n) for n in g['existing_objects_sha256']]
    for path in dependencies:assert sha(path)==g['existing_objects_sha256'][str(path)]
    gdll=d/'jp_native_window.dll';commands.append([str(a.compiler),'-shared','-fuse-ld=lld',str(gobj),str(bgobj),*map(str,dependencies),str(d/'frames/jp_native_frames.lib'),'-Wl,d3d11.lib','-Wl,d3dcompiler.lib','-Wl,dxguid.lib','-Wl,user32.lib','-o',str(gdll)])
    results=[]
    for i,command in enumerate(commands):
        run=subprocess.run(command,cwd=r,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=command,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)));assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6000]
    assert not re.findall(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',(d/'external_background_sdk.ll').read_text())
    authored=dict(b['authored_source_sha256'],**g['authored_source_sha256'])
    for path in [p/'native/jp_native_background.h',p/'native/jp_native_background.c',Path(__file__).resolve()]:authored[path.relative_to(r).as_posix()]=sha(path)
    report=dict(b,status='native_background_SDK_frame_retention_and_GPU_plane_compiled_behavior_unverified',exit_code=0,diagnostic_bytes=0,commands=results,authored_source_sha256=authored,kept_frame_owner_objects_sha256=kept,replaced_frame_owner={str(old):sha(old)},production_objects_sha256={n:sha(d/n) for n in ['frames_background.obj','background_sdk.obj','gpu_background.obj','background_decode.obj']},frames_DLL=str(d/'frames/jp_native_frames.dll'),frames_DLL_sha256=sha(d/'frames/jp_native_frames.dll'),GPU_DLL=str(gdll),GPU_DLL_sha256=sha(gdll),GPU_existing_objects_sha256={str(v):sha(v) for v in dependencies},external_background_source_sha256=original_bg_sha,external_source_sha256=dict(b['external_source_sha256'],**audit['external_files_sha256']),documented_SDK_bug_body_repairs=body_repairs,wrong_vertex_UV_sites_repaired=repaired_uv,wrong_vertex_offset_sites_repaired=repaired_offset,duplicate_offset_words_removed=1,background_SDK_macros_use_existing_RZ_numeric_provider=True,residual_native_SDK_FP_arithmetic=0,private_generated_SDK_sha256=sha(sdk),generated_GPU_source_sha256=sha(generated),generated_frame_owner_sha256=sha(fpath),GPU_adaptations=modifications,frame_resource_pin_adaptation=pin,all_18_SDK_background_dispatch_entries=True,vertex_types_15_to_17_remain_invalid=True,single_frame_texture_queue_video_device_owner=True,original_sources_and_proofs_modified=False,game_containing_DLL_publishable=False,full_game_boot_executed=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(report['status'])
if __name__=='__main__':main()
