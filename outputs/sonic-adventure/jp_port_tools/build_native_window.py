"""Extend the frozen authored renderer with bound state/scissor and Win32 DXGI.
Generated source is authored only; complete original game producers stay private.
"""
import argparse,hashlib,json,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','frames','gpu-baseline','compiler','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    frame=json.loads((a.frames/'build_verification.json').read_text());old=json.loads((a.gpu_baseline/'build_verification.json').read_text())
    authored={}
    for b in [frame,old]:
        assert b['exit_code']==b['diagnostic_bytes']==0 and b['compiler_sha256']==sha(a.compiler)
        for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h;authored[n]=h
    original=p/'native/jp_native_gpu.c'
    assert sha(original)==old['authored_source_sha256'][original.relative_to(r).as_posix()]
    raw=original.read_text();changes={
        '#include "jp_native_gpu.h"':'#include "jp_native_gpu.h"\n#include "jp_native_video_state.h"',
        'uint32_t texture_bindings[2],sampler_bindings[2];':'uint32_t texture_bindings[2],sampler_bindings[2];\n    const struct jp_video_snapshot *snapshot;unsigned window_refs,closing;',
        'if(!g)return;if(g->context)':'if(!g)return;if(g->window_refs){g->closing=1;return;}if(g->context)',
        'if(!g||!cfg||!rgba||!report':'if(!g||g->closing||!cfg||!rgba||!report',
        'rd.CullMode=D3D11_CULL_NONE;rd.DepthClipEnable=TRUE;':'rd.CullMode=D3D11_CULL_NONE;rd.DepthClipEnable=TRUE;rd.ScissorEnable=g->snapshot!=NULL;',
        'ID3D11DeviceContext_IASetInputLayout(g->context,g->layout);':'''if(g->snapshot){
        const struct jp_video_snapshot *s=g->snapshot;
        uint64_t l=s->clip_x&65535,r=s->clip_x>>16,t=s->clip_y&65535,b=s->clip_y>>16;
        if(s->global_clip){if(r>=s->global_clip_width)r=s->global_clip_width-1;if(b>=s->global_clip_height)b=s->global_clip_height-1;}
        D3D11_RECT clip={(LONG)(l*cfg->width/s->logical_width),(LONG)(t*cfg->height/s->logical_height),
            (LONG)(((r+1)*cfg->width+s->logical_width-1)/s->logical_width),(LONG)(((b+1)*cfg->height+s->logical_height-1)/s->logical_height)};
        if(l>r||t>b)clip.right=clip.left,clip.bottom=clip.top;
        if(clip.left>(LONG)cfg->width)clip.left=(LONG)cfg->width;if(clip.right>(LONG)cfg->width)clip.right=(LONG)cfg->width;
        if(clip.top>(LONG)cfg->height)clip.top=(LONG)cfg->height;if(clip.bottom>(LONG)cfg->height)clip.bottom=(LONG)cfg->height;
        ID3D11DeviceContext_RSSetScissorRects(g->context,1,&clip);
    }
    ID3D11DeviceContext_IASetInputLayout(g->context,g->layout);'''}
    for before,after in changes.items():assert raw.count(before)==1,before;raw=raw.replace(before,after)
    extension=p/'native/jp_native_window_extension.c';raw+='\n'+extension.read_text();d.mkdir(parents=True);source=d/'authored_GPU_window.c';source.write_text(raw)
    obj=d/'gpu_window.obj';dll=d/'jp_native_window.dll';dependencies=[Path(n) for n in old['existing_objects_sha256']]
    for path in dependencies:assert sha(path)==old['existing_objects_sha256'][str(path)]
    commands=[[str(a.compiler),'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-Wall','-Wextra','-Werror','-I'+str(p/'native'),'-c',str(source),'-o',str(obj)],
        [str(a.compiler),'-shared','-fuse-ld=lld',str(obj),*map(str,dependencies),str(a.frames/'jp_native_frames.lib'),'-Wl,d3d11.lib','-Wl,d3dcompiler.lib','-Wl,dxguid.lib','-Wl,user32.lib','-o',str(dll)]]
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        assert run.returncode==0 and not log.stat().st_size,log.read_text()[:5000]
    for path in [extension,p/'native/jp_native_window.h',Path(__file__).resolve()]:authored[path.relative_to(r).as_posix()]=sha(path)
    report=dict(status='native_bound_state_window_compiled_behavior_unverified',exit_code=0,diagnostic_bytes=0,commands=results,compiler_sha256=sha(a.compiler),authored_source_sha256=authored,
        original_GPU_source_sha256=sha(original),exact_GPU_adaptations=changes,generated_authored_source_sha256=sha(source),production_object_sha256=sha(obj),
        existing_objects_sha256={str(v):sha(v) for v in dependencies},frames_DLL=str(a.frames.resolve()/'jp_native_frames.dll'),frames_DLL_sha256=sha(a.frames/'jp_native_frames.dll'),
        DLL=str(dll),DLL_sha256=sha(dll),baseline_GPU_report_sha256=sha(a.gpu_baseline/'build_verification.json'),stateful_frame_report_sha256=sha(a.frames/'build_verification.json'),
        one_GPU_device_per_renderer=True,game_source_in_window_object=False,full_game_frame_loop_executed=False,Kamui_render_flip_scheduling_implemented=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(report['status'])
if __name__=='__main__':main()
