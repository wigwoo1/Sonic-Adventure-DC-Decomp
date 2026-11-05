"""Add RTC cleanup authority to the qualified SDK-heap/device boot graph."""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','baseline','rtc','preserved','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();base=a.baseline.resolve();rtc=a.rtc.resolve();p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build')
    b=read(base/'build_verification.json');t=read(rtc/'build_verification.json');history=read(a.preserved/'preserved_source.json')
    normal=lambda values:{Path(n).as_posix():v for n,v in values.items()}
    history=normal(history);sources=normal(b['authored_source_sha256'])
    for n,h in sources.items():
        if n in history:assert history[n]['sha256']==h and sha(history[n]['preserved_path'])==h,n
        else:assert sha(r/n)==h,n
    for n,h in t['authored_source_sha256'].items():assert sha(r/n)==h,n
    d.mkdir();updates={};copies={}
    for k in ('frames','present','GPU','vertex_lifecycle','device','SDK_heap','fixture'):
        source=Path(b[k+'_DLL']);assert sha(source)==b[k+'_DLL_sha256'];target=d/source.name
        shutil.copyfile(source,target);shutil.copyfile(source.with_suffix('.lib'),target.with_suffix('.lib'))
        updates[k+'_DLL']=str(target);updates[k+'_DLL_sha256']=sha(target);copies[str(source)]=dict(path=str(target),sha256=sha(target))
    for key in ('RTC_DLL','client_DLL'):
        source=Path(t[key]);assert sha(source)==t[key+'_sha256'];target=d/source.name
        shutil.copyfile(source,target);shutil.copyfile(source.with_suffix('.lib'),target.with_suffix('.lib'))
        updates[key]=str(target);updates[key+'_sha256']=sha(target)
    compiler=t['commands'][0]['command'][0]
    cc=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-c',str(p/'native/jp_native_exit.c'),'-o',str(d/'exit.obj')]
    link=[compiler,'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(d/'exit.obj'),*[str(d/('jp_native_'+n+'.lib')) for n in ('frames','present','window_input','device','sdk_heap','rtc')],'-Wl,kernel32.lib','-o',str(d/'jp_native_exit.dll')]
    logo=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b['DLL']))
    for i,n in enumerate(logo):
        if Path(n).name in ('jp_native_frames.lib','jp_native_present.lib','jp_native_exit.lib'):logo[i]=str(d/Path(n).name)
    logo[-1]=str(d/'jp_native_logo_start.dll');commands=[]
    for i,cmd in enumerate((cc,link,logo)):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        commands.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(commands,indent=2)+'\n')
        assert not run.returncode and not log.stat().st_size,log.read_text()
    for n,h in b['generated_sources_sha256'].items():
        source=base/n;target=d/n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(source,target);assert sha(target)==h,n
    shutil.copytree(base/'inputs',d/'inputs');shutil.copyfile(base/'vblank_chain_sdk.obj',d/'vblank_chain_sdk.obj')
    for n in history:
        if n in sources:sources[n]=sha(r/n)
    sources.update(t['authored_source_sha256']);sources[Path(__file__).resolve().relative_to(r).as_posix()]=sha(__file__)
    updates.update(status='actual_RTC_before_wait_shutdown_in_native_exit_linked_unverified',commands=commands+b['commands'],
        authored_source_sha256=sources,superseded_authored_source=history,copied_DLLs_sha256=copies,
        authored_exit_DLL=str(d/'jp_native_exit.dll'),authored_exit_DLL_sha256=sha(d/'jp_native_exit.dll'),
        DLL=str(d/'jp_native_logo_start.dll'),DLL_sha256=sha(d/'jp_native_logo_start.dll'),
        RTC_build=str(rtc),RTC_build_sha256=sha(rtc/'build_verification.json'),baseline_build_sha256=sha(base/'build_verification.json'),
        RTC_closes_before_device_quiescence_or_wait=True,RTC_exit_mask=65536,device_heap_RTC_mask=116726,
        artifact_publishable=False,playable=False,overall_completion_percent=10)
    fv=d/'frame_view';fv.mkdir();frame=read(Path(b['frame_build'])/'build_verification.json');frame.update(updates)
    for n in b['generated_sources_sha256']:
        target=fv/n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(d/n,target)
    (fv/'build_verification.json').write_text(json.dumps(frame,indent=2)+'\n')
    hv=d/'heap_view';shutil.copytree(Path(b['heap_build']),hv);heap=read(hv/'build_verification.json');heap['DLL']=updates['SDK_heap_DLL'];heap['DLL_sha256']=updates['SDK_heap_DLL_sha256'];(hv/'build_verification.json').write_text(json.dumps(heap,indent=2)+'\n')
    result=dict(b,**updates,frame_build=str(fv),frame_build_sha256=sha(fv/'build_verification.json'),heap_build=str(hv),heap_build_sha256=sha(hv/'build_verification.json'))
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
