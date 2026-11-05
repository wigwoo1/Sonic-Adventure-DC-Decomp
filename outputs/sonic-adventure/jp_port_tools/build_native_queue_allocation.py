"""Replace the single queue owner and relink the current native startup chain.
Game/SDK objects and linked game-containing DLLs remain private.
"""
import argparse, hashlib, json, shutil, subprocess
from pathlib import Path

def sha(p): return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p): return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','baseline','frame-baseline','compiler','output']:
        ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();base=a.baseline.resolve();lower=a.frame_baseline.resolve();c=a.compiler.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure'
    b=read(base/'build_verification.json');old=read(lower/'build_verification.json');fb=read(Path(b['frame_build'])/'build_verification.json')
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    assert sha(c)==b['compiler_sha256']==old['compiler_sha256']
    assert sha(b['frames_DLL'])==sha(old['frames_DLL'])==old['frames_DLL_sha256']
    for report,folder in [(b,base),(old,lower)]:
        assert all(x['exit_code']==x['diagnostic_bytes']==0 for x in report['commands'])
        for n,h in report['authored_source_sha256'].items(): assert sha(r/n)==h,n
        for n,h in report['generated_sources_sha256'].items(): assert sha(folder/n)==h,n
    frame_link=list(next(x['command'] for x in old['commands'] if '-shared' in x['command'] and x['command'][-1]==old['frames_DLL']))
    previous=next(n for n in frame_link if Path(n).name=='queue_direct.obj')
    kept={n:sha(n) for n in frame_link if n.endswith('.obj') and n!=previous}
    d.mkdir();(d/'frames').mkdir();source=p/'native/jp_native_queue_allocation.c'
    common=list(old['commands'][0]['command']);common=common[:common.index('-c')]
    owner=d/'queue_allocation.obj';commands=[common+['-c',str(source),'-o',str(owner)]]
    frame_link[frame_link.index(previous)]=str(owner);frame_link[-1]=str(d/'frames/jp_native_frames.dll');commands.append(frame_link)
    libraries={'frames_DLL':d/'frames/jp_native_frames.lib'}
    for label,name,imports,recipe in [
        ('GPU_DLL','jp_native_window_input.dll',['frames_DLL'],old),
        ('present_DLL','jp_native_present.dll',['frames_DLL','GPU_DLL'],b),
        ('authored_exit_DLL','jp_native_exit.dll',['frames_DLL','GPU_DLL','present_DLL'],b),
        ('DLL','jp_native_logo_start.dll',['frames_DLL','present_DLL','authored_exit_DLL'],b)]:
        cmd=list(next(x['command'] for x in recipe['commands'] if '-shared' in x['command'] and x['command'][-1]==recipe[label]))
        for n in imports:
            previous_lib=str(Path(recipe[n]).with_suffix('.lib'));assert cmd.count(previous_lib)==1,(n,previous_lib)
            cmd[cmd.index(previous_lib)]=str(libraries[n])
        cmd[-1]=str(d/name);libraries[label]=Path(cmd[-1]).with_suffix('.lib');commands.append(cmd)
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        assert run.returncode==0 and not log.stat().st_size,log.read_text()[:5000]
    for n,h in kept.items(): assert sha(n)==h,n
    sources=dict(b['authored_source_sha256'])
    for path in [source,p/'native/jp_native_queue_allocation.h',Path(__file__).resolve()]:sources[path.relative_to(r).as_posix()]=sha(path)
    for n,h in b['generated_sources_sha256'].items():
        target=d/n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(base/n,target);assert sha(target)==h
    shutil.copyfile(base/'vblank_chain_sdk.obj',d/'vblank_chain_sdk.obj');shutil.copytree(base/'inputs',d/'inputs')
    shared=dict(status='native_five_queue_allocation_linked_behavior_unverified',commands=results,exit_code=0,diagnostic_bytes=0,authored_source_sha256=sources,kept_frame_objects_sha256=kept,replaced_queue_object=dict(path=previous,sha256=sha(previous)),queue_owner_object=str(owner),queue_owner_object_sha256=sha(owner),previous_queue_build_sha256=sha(base/'build_verification.json'),previous_frame_build_sha256=sha(lower/'build_verification.json'),frames_DLL=str(d/'frames/jp_native_frames.dll'),frames_DLL_sha256=sha(d/'frames/jp_native_frames.dll'),GPU_DLL=str(d/'jp_native_window_input.dll'),GPU_DLL_sha256=sha(d/'jp_native_window_input.dll'),artifact_publishable=False,game_containing_DLL_publishable=False,playable=False,overall_completion_percent=10)
    frame=dict(fb,**shared);frame.update(DLL=str(d/'jp_native_present.dll'),DLL_sha256=sha(d/'jp_native_present.dll'),present_DLL=str(d/'jp_native_present.dll'))
    fd=d/'frame_view';fd.mkdir()
    for n in b['generated_sources_sha256']:
        target=fd/n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(d/n,target)
    (fd/'build_verification.json').write_text(json.dumps(frame,indent=2)+'\n')
    qd=d/'queue_view';qd.mkdir();queue=dict(frame,DLL=shared['frames_DLL'],DLL_sha256=shared['frames_DLL_sha256']);(qd/'build_verification.json').write_text(json.dumps(queue,indent=2)+'\n')
    result=dict(b,**shared);result.update(DLL=str(d/'jp_native_logo_start.dll'),DLL_sha256=sha(d/'jp_native_logo_start.dll'),present_DLL=frame['DLL'],authored_exit_DLL=str(d/'jp_native_exit.dll'),authored_exit_DLL_sha256=sha(d/'jp_native_exit.dll'),frame_build=str(fd),frame_build_sha256=sha(fd/'build_verification.json'))
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
