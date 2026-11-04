"""Extend the preserved authored direct renderer with real GPU query ownership."""
import argparse,hashlib,json,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','baseline','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();baseline=a.baseline.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    b=json.loads((baseline/'build_verification.json').read_text());assert b['exit_code']==b['diagnostic_bytes']==0 and b['compiler_sha256']==sha(a.compiler)
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    source=baseline/'authored_GPU_direct_window.c';assert sha(source)==b['generated_authored_source_sha256']
    raw=source.read_text();changes={
        'const struct jp_video_snapshot *snapshot;unsigned window_refs,closing;':
        'const struct jp_video_snapshot *snapshot;unsigned window_refs,closing;\n    struct jp_gpu_job *jobs[4];unsigned job_refs;DWORD owner_thread;\n    ID3D11Query *measure_begin,*measure_end,*measure_disjoint;LARGE_INTEGER measure_prepared;',
        'D3D_FEATURE_LEVEL wanted=D3D_FEATURE_LEVEL_11_0;':'g->owner_thread=GetCurrentThreadId();\n    D3D_FEATURE_LEVEL wanted=D3D_FEATURE_LEVEL_11_0;',
        'if(g->window_refs){g->closing=1;return;}':'if(g->window_refs||g->job_refs){g->closing=1;return;}',
        'ID3D11DeviceContext_OMSetRenderTargets(g->context,1,&rtv,dsv);':
        '''if(g->measure_begin){
        if(!QueryPerformanceCounter(&g->measure_prepared)){code=JP_GPU_API;hr=E_FAIL;goto done;}
        ID3D11DeviceContext_Begin(g->context,(ID3D11Asynchronous *)g->measure_disjoint);
        ID3D11DeviceContext_End(g->context,(ID3D11Asynchronous *)g->measure_begin);
    }
    ID3D11DeviceContext_OMSetRenderTargets(g->context,1,&rtv,dsv);''',
        'if(!output){\n    ID3D11DeviceContext_CopyResource(g->context,(ID3D11Resource *)staging':
        '''if(g->measure_begin){
        ID3D11DeviceContext_End(g->context,(ID3D11Asynchronous *)g->measure_end);
        ID3D11DeviceContext_End(g->context,(ID3D11Asynchronous *)g->measure_disjoint);
    }
    if(!output){
    ID3D11DeviceContext_CopyResource(g->context,(ID3D11Resource *)staging''',
    }
    for before,after in changes.items():assert raw.count(before)==1,before;raw=raw.replace(before,after)
    extension=p/'native/jp_native_gpu_jobs_extension.c';raw+='\n'+extension.read_text();d.mkdir();generated=d/'authored_GPU_jobs.c';generated.write_text(raw)
    obj=d/'gpu_window.obj';dll=d/'jp_native_window.dll';dependencies=[Path(n) for n in b['existing_objects_sha256']]
    for path in dependencies:assert sha(path)==b['existing_objects_sha256'][str(path)]
    frames=Path(b['frames_DLL']);assert sha(frames)==b['frames_DLL_sha256']
    commands=[[str(a.compiler),'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-Wall','-Wextra','-Werror','-I'+str(p/'native'),'-c',str(generated),'-o',str(obj)],
        [str(a.compiler),'-shared','-fuse-ld=lld',str(obj),*map(str,dependencies),str(frames.with_suffix('.lib')),'-Wl,d3d11.lib','-Wl,d3dcompiler.lib','-Wl,dxguid.lib','-Wl,user32.lib','-o',str(dll)]]
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6000]
    authored=dict(b['authored_source_sha256'])
    for path in [extension,p/'native/jp_native_gpu_jobs.h',Path(__file__).resolve()]:authored[path.relative_to(r).as_posix()]=sha(path)
    report=dict(b,status='native_GPU_query_jobs_compiled_behavior_unverified',commands=results,authored_source_sha256=authored,generated_authored_source_sha256=sha(generated),
        production_object_sha256=sha(obj),DLL=str(dll),DLL_sha256=sha(dll),exact_job_adaptations=changes,direct_baseline_report_sha256=sha(baseline/'build_verification.json'),
        real_event_and_disjoint_timestamp_queries_implemented=True,maximum_outstanding_jobs=4,game_frame_waits_or_Kamui_scheduling_implemented=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(report['status'])
if __name__=='__main__':main()
