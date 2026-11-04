"""Reuse the frozen window renderer with one shared target/readback core.

Checked transformations preserve predecessor source/proofs and legacy ABI.
Only authored source is generated here; no original game bodies are packaged.
"""
import argparse,hashlib,json,subprocess
from pathlib import Path

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','baseline','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    baseline=a.baseline.resolve();b=json.loads((baseline/'build_verification.json').read_text())
    assert b['exit_code']==b['diagnostic_bytes']==0 and b['compiler_sha256']==sha(a.compiler)
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    source=baseline/'authored_GPU_window.c';assert sha(source)==b['generated_authored_source_sha256']
    raw=source.read_text();changes={
        'int jp_gpu_draw_frame(struct jp_gpu *g,uint64_t frame,const struct jp_gpu_config *cfg,\n    void *rgba,size_t bytes,struct jp_gpu_report *report,struct jp_gpu_failure *failure)':
        'static int gpu_render_frame(struct jp_gpu *g,uint64_t frame,const struct jp_gpu_config *cfg,\n    void *rgba,size_t bytes,struct jp_gpu_report *report,struct jp_gpu_failure *failure,ID3D11Texture2D *output)',
        'if(!g||g->closing||!cfg||!rgba||!report':'if(!g||g->closing||!cfg||(!rgba && !output)||!report',
        'if(bytes<pixels*4)':'if(!output && bytes<pixels*4)',
        '!account(&cost,pixels,12,cfg->budget)':'!account(&cost,pixels,output?8:12,cfg->budget)',
        'hr=ID3D11Device_CreateTexture2D(g->device,&desc,NULL,&target);if(FAILED(hr))goto done;':
        'if(output){target=output;ID3D11Texture2D_AddRef(target);}\n    else {hr=ID3D11Device_CreateTexture2D(g->device,&desc,NULL,&target);if(FAILED(hr))goto done;}',
        'hr=ID3D11Device_CreateTexture2D(g->device,&desc,NULL,&staging);if(FAILED(hr))goto done;':
        'if(!output){hr=ID3D11Device_CreateTexture2D(g->device,&desc,NULL,&staging);if(FAILED(hr))goto done;}',
        'ID3D11DeviceContext_CopyResource(g->context,(ID3D11Resource *)staging,(ID3D11Resource *)target);D3D11_MAPPED_SUBRESOURCE mapped={0};':
        'if(!output){\n    ID3D11DeviceContext_CopyResource(g->context,(ID3D11Resource *)staging,(ID3D11Resource *)target);D3D11_MAPPED_SUBRESOURCE mapped={0};',
        'ID3D11DeviceContext_Unmap(g->context,(ID3D11Resource *)staging,0);':
        'ID3D11DeviceContext_Unmap(g->context,(ID3D11Resource *)staging,0);\n    }\n    hr=ID3D11Device_GetDeviceRemovedReason(g->device);if(FAILED(hr))goto done;',
        'int jp_gpu_draw_bound_frame(struct jp_gpu *g,uint64_t frame,const struct jp_gpu_config *settings,\n    void *rgba,size_t bytes,struct jp_gpu_report *report,struct jp_gpu_failure *failure)':
        'static int gpu_draw_bound_target(struct jp_gpu *g,uint64_t frame,const struct jp_gpu_config *settings,\n    void *rgba,size_t bytes,struct jp_gpu_report *report,struct jp_gpu_failure *failure,ID3D11Texture2D *output)',
        'if(!g||g->closing||!settings||!rgba||!report)':'if(!g||g->closing||!settings||(!rgba && !output)||!report)',
        'int result=jp_gpu_draw_frame(g,frame,&config,rgba,bytes,report,failure);':
        'int result=gpu_render_frame(g,frame,&config,rgba,bytes,report,failure,output);',
    }
    for before,after in changes.items():assert raw.count(before)==1,before;raw=raw.replace(before,after)
    # Public functions retain their original ABI and synchronous readback.
    raw+='''
int jp_gpu_draw_frame(struct jp_gpu *g,uint64_t frame,const struct jp_gpu_config *cfg,
    void *rgba,size_t bytes,struct jp_gpu_report *report,struct jp_gpu_failure *failure)
{return gpu_render_frame(g,frame,cfg,rgba,bytes,report,failure,NULL);}
int jp_gpu_draw_bound_frame(struct jp_gpu *g,uint64_t frame,const struct jp_gpu_config *cfg,
    void *rgba,size_t bytes,struct jp_gpu_report *report,struct jp_gpu_failure *failure)
{return gpu_draw_bound_target(g,frame,cfg,rgba,bytes,report,failure,NULL);}
'''
    extension=p/'native/jp_native_direct_window_extension.c';raw+='\n'+extension.read_text()
    d.mkdir();generated=d/'authored_GPU_direct_window.c';generated.write_text(raw);obj=d/'gpu_window.obj';dll=d/'jp_native_window.dll'
    dependencies=[Path(n) for n in b['existing_objects_sha256']]
    for path in dependencies:assert sha(path)==b['existing_objects_sha256'][str(path)]
    frames=Path(b['frames_DLL']);assert sha(frames)==b['frames_DLL_sha256']
    commands=[[str(a.compiler),'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-Wall','-Wextra','-Werror','-I'+str(p/'native'),'-c',str(generated),'-o',str(obj)],
        [str(a.compiler),'-shared','-fuse-ld=lld',str(obj),*map(str,dependencies),str(frames.with_suffix('.lib')),'-Wl,d3d11.lib','-Wl,d3dcompiler.lib','-Wl,dxguid.lib','-Wl,user32.lib','-o',str(dll)]]
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        assert run.returncode==0 and not log.stat().st_size,log.read_text()[:5000]
    authored=dict(b['authored_source_sha256'])
    for path in [extension,p/'native/jp_native_direct_window.h',Path(__file__).resolve()]:authored[path.relative_to(r).as_posix()]=sha(path)
    report=dict(b,status='direct_native_backbuffer_compiled_behavior_unverified',commands=results,authored_source_sha256=authored,
        generated_authored_source_sha256=sha(generated),production_object_sha256=sha(obj),DLL=str(dll),DLL_sha256=sha(dll),
        exact_direct_target_adaptations=changes,window_baseline_report_sha256=sha(baseline/'build_verification.json'),
        direct_path_has_no_CPU_image_transfer=True,direct_success_means_GPU_submission_only=True,direct_rendering_behavior_verified=False,
        legacy_readback_API_preserved=True,Kamui_render_flip_scheduling_implemented=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(report['status'])
if __name__=='__main__':main()
