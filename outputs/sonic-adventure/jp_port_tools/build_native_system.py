"""Compile the real native core initializer and existing original boot graph."""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path
from build_native_texture_runtime import function
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','preserved','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();base=a.baseline.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build')
    b=read(base/'build_verification.json');history={Path(n).as_posix():v for n,v in read(a.preserved/'preserved_source.json').items()};sources={Path(n).as_posix():v for n,v in b['authored_source_sha256'].items()}
    for n,h in sources.items():
        if n in history:assert history[n]['sha256']==h and sha(history[n]['preserved_path'])==h,n
        else:assert sha(r/n)==h,n
    d.mkdir();updates={};copies={}
    for k,value in b.items():
        if (k=='DLL' or k.endswith('_DLL')) and isinstance(value,str) and Path(value).suffix=='.dll' and k not in ('DLL','authored_exit_DLL'):
            source=Path(value);assert sha(source)==b[k+'_sha256'];target=d/source.name
            if not target.exists():shutil.copyfile(source,target);shutil.copyfile(source.with_suffix('.lib'),target.with_suffix('.lib'))
            else:assert sha(target)==sha(source)
            updates[k]=str(target);updates[k+'_sha256']=sha(target);copies[k]=dict(path=str(target),sha256=sha(target))
    compiler=b['commands'][0]['command'][0];common=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror']
    peer=d/'system_signature_check.c';peer.write_text('#include "jp_native_system.h"\n#include "jp_native_input.h"\n_Static_assert(sizeof(struct jp_system_options)==96,"system options ABI");\n_Static_assert(sizeof(struct jp_system_info)==64,"system info ABI");\n_Static_assert(_Generic((jp_system_input_source)0,jp_input_source:1,default:0),"exact existing input callback type");\n')
    commands=[common+['-c',str(p/'native/jp_native_exit.c'),'-o',str(d/'exit.obj')],common+['-c',str(p/'native/jp_native_system.c'),'-o',str(d/'system.obj')],common+['-I'+str(p/'native'),'-c',str(peer),'-o',str(d/'signature.obj')]]
    libs=lambda names:[str(d/('jp_native_'+n+'.lib')) for n in names]
    commands.append(common[:2]+['-shared','-fuse-ld=lld',str(d/'exit.obj'),*libs(('frames','present','window_input','device','sdk_heap','rtc','gdfs')),'-Wl,kernel32.lib','-o',str(d/'jp_native_exit.dll')])
    commands.append(common[:2]+['-shared','-fuse-ld=lld',str(d/'system.obj'),*libs(('frames','window_input','device','sdk_heap','rtc','gdfs','exit')),'-Wl,kernel32.lib','-o',str(d/'jp_native_system.dll')])
    logo=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and Path(x['command'][-1]).name==Path(b['DLL']).name));assert sha(logo[-1])==b['DLL_sha256']
    for i,n in enumerate(logo):
        if Path(n).name in ('jp_native_frames.lib','jp_native_present.lib','jp_native_exit.lib'):logo[i]=str(d/Path(n).name)
    logo[-1]=str(d/'jp_native_logo_start.dll');commands.append(logo);results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n');assert not run.returncode and not log.stat().st_size,log.read_text()
    originals={};bodies={};git='C:/Program Files/Git/cmd/git.exe'
    for name in ('src/system/sbinit.c','src/system/cache.src','sdk/shinobi/syhw.c','src/main.c'):
        raw=(u/name).read_bytes();h=sha(u/name);assert hashlib.sha256(subprocess.check_output([git,'-C',str(u),'show','25a3d3154dbe657325e97dbe8c5291086f6af5c0:'+name])).hexdigest()==h,name;originals[name]=h
        if name.endswith('sbinit.c'):bodies['sbInitSystem']=hashlib.sha256(function(raw,'sbInitSystem')).hexdigest()
        if name.endswith('main.c'):bodies['system_boot_init']=hashlib.sha256(function(raw,'system_boot_init')).hexdigest()
    for n,h in b['generated_sources_sha256'].items():
        source=base/n;target=d/n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(source,target);assert sha(target)==h,n
    shutil.copytree(base/'inputs',d/'inputs');shutil.copyfile(base/'vblank_chain_sdk.obj',d/'vblank_chain_sdk.obj')
    for n in history:
        if n in sources:sources[n]=sha(r/n)
    for path in [p/'native/jp_native_system.c',p/'native/jp_native_system.h',Path(__file__).resolve()]:sources[path.relative_to(r).as_posix()]=sha(path)
    updates.update(status='compiled_native_sbInitSystem_real_core_owners_and_early_exit_linked_unverified',commands=results+b['commands'],authored_source_sha256=sources,
        superseded_authored_source=history,copied_DLLs_sha256=copies,System_DLL=str(d/'jp_native_system.dll'),System_DLL_sha256=sha(d/'jp_native_system.dll'),
        authored_exit_DLL=str(d/'jp_native_exit.dll'),authored_exit_DLL_sha256=sha(d/'jp_native_exit.dll'),DLL=str(d/'jp_native_logo_start.dll'),DLL_sha256=sha(d/'jp_native_logo_start.dll'),
        baseline_build_sha256=sha(base/'build_verification.json'),original_system_source_sha256=originals,original_system_function_sha256=bodies,
        native_core_resource_mask=245762,native_full_resource_mask=247798,console_hardware_success_stubs_added=False,unsafe_original_sbInitSystem_probe_linked=False,
        native_main_system_boot_init_linked=False,artifact_publishable=False,playable=False,overall_completion_percent=10)
    fv=d/'frame_view';fv.mkdir();frame=read(Path(b['frame_build'])/'build_verification.json');frame.update(updates)
    for n in b['generated_sources_sha256']:
        target=fv/n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(d/n,target)
    (fv/'build_verification.json').write_text(json.dumps(frame,indent=2)+'\n')
    hv=d/'heap_view';shutil.copytree(Path(b['heap_build']),hv);heap=read(hv/'build_verification.json');heap['DLL']=updates['SDK_heap_DLL'];heap['DLL_sha256']=updates['SDK_heap_DLL_sha256'];(hv/'build_verification.json').write_text(json.dumps(heap,indent=2)+'\n')
    result=dict(b,**updates,frame_build=str(fv),frame_build_sha256=sha(fv/'build_verification.json'),heap_build=str(hv),heap_build_sha256=sha(hv/'build_verification.json'))
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
