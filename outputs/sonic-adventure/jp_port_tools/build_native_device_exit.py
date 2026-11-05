"""Compose native device and terminal exit with one cleanup authority.
Reuse qualified shared objects and private original game/SDK artifacts.
"""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','baseline','device','preserved','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();base=a.baseline.resolve();old=a.device.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build')
    b=read(base/'build_verification.json');v=read(old/'build_verification.json');history=read(a.preserved/'preserved_source.json')
    sources=dict(b['authored_source_sha256']);sources.update(v['authored_source_sha256'])
    for n,h in sources.items():
        if n in history:assert history[n]['sha256']==h and sha(history[n]['preserved_path'])==h,n
        else:assert sha(r/n)==h,n
    for k in ('frames','present','GPU','vertex_lifecycle'):
        assert b[k+'_DLL']==v[k+'_DLL'] and sha(v[k+'_DLL'])==v[k+'_DLL_sha256']
    assert sha(v['device_DLL'])==v['device_DLL_sha256']
    for n,h in v['kept_frame_objects_sha256'].items():assert sha(n)==h,n
    d.mkdir();copied={};shared={}
    for k in ('frames','present','GPU','vertex_lifecycle'):
        source=Path(v[k+'_DLL']);target=d/source.name;shutil.copyfile(source,target)
        shutil.copyfile(source.with_suffix('.lib'),target.with_suffix('.lib'))
        copied[str(source)]=dict(path=str(target),sha256=sha(target));assert sha(target)==v[k+'_DLL_sha256']
        shared[k+'_DLL']=str(target);shared[k+'_DLL_sha256']=sha(target)
    dc=list(next(x['command'] for x in v['commands'] if '-c' in x['command'] and x['command'][x['command'].index('-c')+1].endswith('jp_native_device.c')))
    dc[-1]=str(d/'device.obj')
    link=list(next(x['command'] for x in v['commands'] if '-shared' in x['command'] and x['command'][-1]==v['device_DLL']))
    prior=next(n for n in link if Path(n).name=='device.obj');link[link.index(prior)]=dc[-1];link[-1]=str(d/'jp_native_device.dll')
    compiler=dc[0];ec=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-c',str(p/'native/jp_native_exit.c'),'-o',str(d/'exit.obj')]
    el=[compiler,'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(d/'exit.obj'),*[str(Path(shared[k+'_DLL']).with_suffix('.lib')) for k in ('frames','present','GPU')],str(d/'jp_native_device.lib'),'-Wl,kernel32.lib','-o',str(d/'jp_native_exit.dll')]
    logo=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b['DLL']))
    for i,n in enumerate(logo):
        if n.endswith('jp_native_frames.lib'):logo[i]=str(Path(shared['frames_DLL']).with_suffix('.lib'))
        elif n.endswith('jp_native_present.lib'):logo[i]=str(Path(shared['present_DLL']).with_suffix('.lib'))
        elif n.endswith('jp_native_exit.lib'):logo[i]=str(d/'jp_native_exit.lib')
    logo[-1]=str(d/'jp_native_logo_start.dll');commands=[]
    kept={n:sha(n) for n in link+logo if n.endswith('.obj') and Path(n).resolve().parent!=d}
    for i,cmd in enumerate((dc,link,ec,el,logo)):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        commands.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(commands,indent=2)+'\n')
        assert not run.returncode and not log.stat().st_size,log.read_text()
    for n,h in kept.items():assert sha(n)==h,n
    generated=dict(b['generated_sources_sha256']);generated.update(v['generated_sources_sha256'])
    for n,h in generated.items():
        source=old/n if n in v['generated_sources_sha256'] else base/n;target=d/n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(source,target);assert sha(target)==h,n
    shutil.copytree(base/'inputs',d/'inputs');shutil.copyfile(base/'vblank_chain_sdk.obj',d/'vblank_chain_sdk.obj')
    for n in history:sources[n]=sha(r/n)
    sources[Path(__file__).resolve().relative_to(r).as_posix()]=sha(__file__)
    shared.update(status='actual_native_device_and_single_terminal_exit_authority_linked_unverified',commands=commands+b['commands'],authored_source_sha256=sources,
        superseded_authored_source=history,generated_sources_sha256=generated,kept_frame_objects_sha256=v['kept_frame_objects_sha256'],
        kept_presentation_objects_sha256=v['kept_present_objects_sha256'],preserved_original_objects_sha256=kept,copied_DLLs_sha256=copied,
        device_DLL=str(d/'jp_native_device.dll'),device_DLL_sha256=sha(d/'jp_native_device.dll'),
        authored_exit_DLL=str(d/'jp_native_exit.dll'),authored_exit_DLL_sha256=sha(d/'jp_native_exit.dll'),
        DLL=str(d/'jp_native_logo_start.dll'),DLL_sha256=sha(d/'jp_native_logo_start.dll'),
        borrowed_device_handles_never_independently_owned_by_exit=True,classic_exit_ABI_and_ALL_mask_preserved=True,
        device_baseline_build_sha256=sha(old/'build_verification.json'),startup_baseline_build_sha256=sha(base/'build_verification.json'),
        artifact_publishable=False,playable=False,overall_completion_percent=10)
    prior=read(Path(b['frame_build'])/'build_verification.json');frame=dict(prior,**shared);fd=d/'frame_view';fd.mkdir()
    for n in generated:
        target=fd/n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(d/n,target)
    (fd/'build_verification.json').write_text(json.dumps(frame,indent=2)+'\n')
    result=dict(b,**shared);result.update(frame_build=str(fd),frame_build_sha256=sha(fd/'build_verification.json'))
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
