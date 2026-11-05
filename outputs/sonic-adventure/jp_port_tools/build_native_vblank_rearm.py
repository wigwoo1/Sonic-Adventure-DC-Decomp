"""Replace only the authored arming boundary; preserve all original SDK bytes."""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','baseline','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args()
    for n in vars(a):setattr(a,n,getattr(a,n).resolve())
    r,base,c,d=a.workspace,a.baseline,a.compiler,a.output;p=r/'outputs/sonic-adventure';b=read(base/'build_verification.json');fb=read(Path(b['frame_build'])/'build_verification.json')
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    assert sha(c)==b['compiler_sha256'] and all(x['exit_code']==x['diagnostic_bytes']==0 for x in b['commands'])
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(base/n)==h,n
    link=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b['present_DLL']));old=next(n for n in link if n.endswith('vblank_wait_owner.obj'));kept={n:sha(n) for n in link if n.endswith('.obj') and n!=old}
    d.mkdir();source=p/'native/jp_native_vblank_rearm.c';compile=list(b['commands'][0]['command']);compile[compile.index('-c')+1]=str(source);compile[-1]=str(d/'vblank_wait_owner.obj');commands=[compile]
    link[link.index(old)]=str(d/'vblank_wait_owner.obj');link[-1]=str(d/'jp_native_present.dll');commands.append(link);libraries={'present_DLL':d/'jp_native_present.lib'}
    for label,name,imports in [('authored_exit_DLL','jp_native_exit.dll',['present_DLL']),('DLL','jp_native_logo_start.dll',['present_DLL','authored_exit_DLL'])]:
        cmd=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b[label]))
        for n in imports:
            previous=str(Path(b[n]).with_suffix('.lib'));assert cmd.count(previous)==1;cmd[cmd.index(previous)]=str(libraries[n])
        cmd[-1]=str(d/name);libraries[label]=Path(cmd[-1]).with_suffix('.lib');commands.append(cmd)
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/('build_'+str(i)+'.txt');log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)));assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6000]
    for n,h in kept.items():assert sha(n)==h,n
    sources=dict(b['authored_source_sha256'])
    for path in [source,Path(__file__).resolve()]:sources[path.relative_to(r).as_posix()]=sha(path)
    for n,h in b['generated_sources_sha256'].items():target=d/n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(base/n,target);assert sha(target)==h
    shutil.copyfile(base/'vblank_chain_sdk.obj',d/'vblank_chain_sdk.obj');assert sha(d/'vblank_chain_sdk.obj')==b['private_chain_sdk_object_sha256'];shutil.copytree(base/'inputs',d/'inputs')
    shared=dict(status='retired_Ninja_blank_slot_reuse_boundary_linked_behavior_unverified',commands=results,authored_source_sha256=sources,kept_presentation_objects_sha256=kept,replaced_wait_object=dict(path=old,sha256=sha(old)),native_vblank_owner_sha256=sha(d/'vblank_wait_owner.obj'),previous_vblank_build_sha256=sha(base/'build_verification.json'),original_private_chain_SDK_object_bytes_preserved=True,retired_Ninja_slot_reuse_boundary_fixed=True)
    frame=dict(fb,**shared);frame.update(DLL=str(d/'jp_native_present.dll'),DLL_sha256=sha(d/'jp_native_present.dll'),present_DLL=str(d/'jp_native_present.dll'));fd=d/'frame_view';fd.mkdir()
    for n in b['generated_sources_sha256']:target=fd/n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(d/n,target)
    (fd/'build_verification.json').write_text(json.dumps(frame,indent=2)+'\n')
    result=dict(b,**shared);result.update(DLL=str(d/'jp_native_logo_start.dll'),DLL_sha256=sha(d/'jp_native_logo_start.dll'),present_DLL=frame['DLL'],authored_exit_DLL=str(d/'jp_native_exit.dll'),authored_exit_DLL_sha256=sha(d/'jp_native_exit.dll'),frame_build=str(fd),frame_build_sha256=sha(fd/'build_verification.json'))
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
