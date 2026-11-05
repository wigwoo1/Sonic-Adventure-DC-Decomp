"""Extend the real window owner and replace frame-wait peripheral traces.

All game-containing frame/presenter DLLs remain private. Existing qualified
sources and objects are preserved. No new input, clock or game-state owner.
"""
import argparse,hashlib,json,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','baseline','gpu-baseline','compiler','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args()
    for n in vars(a):setattr(a,n,getattr(a,n).resolve())
    r,bdir,gdir,c,d=a.workspace,a.baseline,a.gpu_baseline,a.compiler,a.output;p=r/'outputs/sonic-adventure'
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    b=read(bdir/'build_verification.json');g=read(gdir/'build_verification.json')
    assert b['original_pad_scan_linked'] and b['original_recording_update_linked'] and not b['frame_wait_input_trace_replaced']
    for report in [b,g]:
        assert sha(c)==report['compiler_sha256']
        for n,h in report['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(b['frames_DLL'])==b['frames_DLL_sha256'] and sha(gdir/'authored_GPU_background.c')==g['generated_GPU_source_sha256']
    old=r/'work/sonic-adventure/jp-native-task-presentation-runtime-v6';t=read(old/'build_verification.json')
    source=old/'present_diagnostic.c';assert sha(source)==t['generated_sources_sha256'][source.name]
    d.mkdir();(d/'frames').mkdir();gpu=d/'authored_GPU_window_input.c';gpu.write_bytes((gdir/'authored_GPU_background.c').read_bytes()+b'\n#include "jp_native_window_input_extension.c"\n')
    diag=d/'present_real_input.c';raw=source.read_bytes();old_stub=b'void pdExecPeripheralServer(void){event(12);}'
    assert raw.count(old_stub)==1;raw=raw.replace(old_stub,b'/* Peripheral service supplied by the shared frame/input owner. */')
    assert raw.count(b'    pdExecPeripheralServer();')==2
    raw=raw.replace(b'    pdExecPeripheralServer();',b'    pdExecPeripheralServer();\n    event(12); /* passive observation after actual polling */\n    if (jp_input_status()) return;')
    diag.write_bytes(b'static void event(unsigned);\n__declspec(dllimport) int jp_input_status(void);\n'+raw)
    gcompile=list(next(x['command'] for x in g['commands'] if '-c' in x['command'] and x['command'][-1].endswith('gpu_background.obj')))
    gcompile[-3]=str(gpu);gcompile[-1]=str(d/'gpu_window_input.obj')
    fcompile=list(next(x['command'] for x in t['commands'] if '-c' in x['command'] and x['command'][-1].endswith('diagnostic.obj')))
    fcompile.insert(1,'-I'+str(old));fcompile[-3]=str(diag);fcompile[-1]=str(d/'frame_real_input.obj')
    glink=list(next(x['command'] for x in g['commands'] if '-shared' in x['command'] and x['command'][-1]==g['GPU_DLL']))
    glink[glink.index(str(gdir/'gpu_background.obj'))]=str(d/'gpu_window_input.obj')
    frames=d/'frames/jp_native_frames.dll'
    flink=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b['frames_DLL']))
    frame_objects={str(Path(v).resolve()):sha(v) for v in flink if v.endswith('.obj')}
    flink[-1]=str(frames)
    glink[glink.index(str(gdir/'frames/jp_native_frames.lib'))]=str(frames.with_suffix('.lib'))
    gdll=d/'jp_native_window_input.dll';glink[-1]=str(gdll)
    plink=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b['DLL']))
    plink[plink.index(str(old/'diagnostic.obj'))]=str(d/'frame_real_input.obj')
    plink[plink.index(str(Path(b['frames_DLL']).with_suffix('.lib')))]=str(frames.with_suffix('.lib'))
    libraries=[i for i,x in enumerate(plink) if x.endswith('jp_native_window.lib')];assert len(libraries)==1
    plink[libraries[0]]=str(gdll.with_suffix('.lib'));plink[-1]=str(d/'jp_native_present.dll')
    plink.insert(plink.index('-o'),'-Wl,/export:pdExecPeripheralServer=jp_native_frames.pdExecPeripheralServer')
    commands=[fcompile,gcompile,flink,glink,plink];results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,cwd=r,capture_output=True);log=d/('build_'+str(i)+'.txt');log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6000]
    for n,h in frame_objects.items():assert sha(n)==h,n
    result=dict(b,status='real_window_handle_and_frame_wait_polling_linked_behavior_unverified',commands=results,exit_code=0,diagnostic_bytes=0,DLL=str(d/'jp_native_present.dll'),DLL_sha256=sha(d/'jp_native_present.dll'),GPU_DLL=str(gdll),GPU_DLL_sha256=sha(gdll),frames_DLL=str(frames),frames_DLL_sha256=sha(frames),baseline_build_sha256=sha(bdir/'build_verification.json'),GPU_baseline_build_sha256=sha(gdir/'build_verification.json'),frame_owner_bytes_unchanged=sha(frames)==b['frames_DLL_sha256'],relinked_frame_objects_sha256=frame_objects,frame_wait_input_trace_replaced=True,input_trace_is_passive_after_actual_service=True,frame_wait_returns_on_input_error=True,window_handle_borrowed_from_actual_owner=True,game_containing_DLLs_publishable=False,artifact_publishable=False,original_logo_step_executed=False,playable=False,overall_completion_percent=10)
    result['authored_source_sha256']=dict(b['authored_source_sha256'])
    for f in [p/'native/jp_native_window_input.h',p/'native/jp_native_window_input_extension.c',Path(__file__).resolve()]:result['authored_source_sha256'][f.relative_to(r).as_posix()]=sha(f)
    result['generated_sources_sha256']={f.name:sha(f) for f in [gpu,diag]}
    result['preserved_frame_build']=str(bdir);result['preserved_frame_build_sha256']=sha(bdir/'build_verification.json')
    result['input_object_sha256']=b['input_object_sha256']
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'])
if __name__=='__main__':main()
