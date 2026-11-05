"""Integrate the original SDK heap with actual boot/exit ownership.
Keep qualified graphics/device/SDK algorithm objects and all prior artifacts.
"""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','baseline','heap','preserved','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();base=a.baseline.resolve();old=a.heap.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build')
    b=read(base/'build_verification.json');h=read(old/'build_verification.json')
    normalize=lambda values:{Path(n).as_posix():v for n,v in values.items()}
    history=normalize(read(a.preserved/'preserved_source.json'))
    sources=normalize(b['authored_source_sha256']);sources.update(normalize(h['authored_source_sha256']))
    for n,value in sources.items():
        if n in history:assert history[n]['sha256']==value and sha(history[n]['preserved_path'])==value,n
        else:assert sha(r/n)==value,n
    assert sha(h['DLL'])==h['DLL_sha256']
    for n,value in b['kept_frame_objects_sha256'].items():assert sha(n)==value,n
    d.mkdir();shared={};copied={};commands=[]
    for k in ('frames','present','GPU','vertex_lifecycle','device'):
        source=Path(b[k+'_DLL']);assert sha(source)==b[k+'_DLL_sha256'];target=d/source.name
        shutil.copyfile(source,target);shutil.copyfile(source.with_suffix('.lib'),target.with_suffix('.lib'))
        shared[k+'_DLL']=str(target);shared[k+'_DLL_sha256']=sha(target);copied[str(source)]=dict(path=str(target),sha256=sha(target))
    hc=list(next(x['command'] for x in h['commands'] if '-c' in x['command'] and x['command'][x['command'].index('-c')+1].endswith('jp_native_sdk_heap.c')));hc[-1]=str(d/'sdk_heap_owner.obj');compiler=hc[0]
    hl=list(next(x['command'] for x in h['commands'] if '-shared' in x['command'] and x['command'][-1]==h['DLL']))
    old_owner=next(n for n in hl if Path(n).name=='sdk_heap_owner.obj');hl[hl.index(old_owner)]=hc[-1];hl[-1]=str(d/'jp_native_sdk_heap.dll')
    ec=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-c',str(p/'native/jp_native_exit.c'),'-o',str(d/'exit.obj')]
    el=[compiler,'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(d/'exit.obj'),*[str(Path(shared[k+'_DLL']).with_suffix('.lib')) for k in ('frames','present','GPU','device')],str(d/'jp_native_sdk_heap.lib'),'-Wl,kernel32.lib','-o',str(d/'jp_native_exit.dll')]
    logo=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b['DLL']))
    for i,n in enumerate(logo):
        for key in ('frames','present'):
            if Path(n).name=='jp_native_'+key+'.lib':logo[i]=str(Path(shared[key+'_DLL']).with_suffix('.lib'))
        if Path(n).name=='jp_native_exit.lib':logo[i]=str(d/'jp_native_exit.lib')
    logo[-1]=str(d/'jp_native_logo_start.dll')
    common=list(h['commands'][0]['command']);common=common[:common.index('-c')]
    fixture=p/'jp_port_tools/sdk_heap_exit_fixture.c';fc=common+['-c',str(fixture),'-o',str(d/'sdk_heap_exit_fixture.obj')]
    fl=[compiler,'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(d/'sdk_heap_exit_fixture.obj'),str(d/'jp_native_sdk_heap.lib'),'-o',str(d/'sdk_heap_exit_fixture.dll')]
    kept={n:sha(n) for cmd in (hl,logo) for n in cmd if n.endswith('.obj') and Path(n).resolve().parent!=d}
    for i,cmd in enumerate((hc,hl,ec,el,logo,fc,fl)):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr);commands.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size))
        (d/'compile_attempt.json').write_text(json.dumps(commands,indent=2)+'\n');assert not run.returncode and not log.stat().st_size,log.read_text()
    for n,value in kept.items():assert sha(n)==value,n
    generated=dict(b['generated_sources_sha256']);generated.update(h['generated_sources_sha256'])
    for n,value in generated.items():
        source=old/n if n in h['generated_sources_sha256'] else base/n;target=d/n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(source,target);assert sha(target)==value,n
    shutil.copytree(base/'inputs',d/'inputs');shutil.copyfile(base/'vblank_chain_sdk.obj',d/'vblank_chain_sdk.obj')
    for n in history:sources[n]=sha(r/n)
    sources[Path(__file__).resolve().relative_to(r).as_posix()]=sha(__file__);sources[fixture.relative_to(r).as_posix()]=sha(fixture)
    shared.update(status='actual_SDK_heap_bootstrap_single_exit_authority_linked_unverified',commands=commands+b['commands'],authored_source_sha256=sources,superseded_authored_source=history,
        generated_sources_sha256=generated,preserved_original_objects_sha256=kept,copied_DLLs_sha256=copied,
        SDK_heap_DLL=str(d/'jp_native_sdk_heap.dll'),SDK_heap_DLL_sha256=sha(d/'jp_native_sdk_heap.dll'),
        authored_exit_DLL=str(d/'jp_native_exit.dll'),authored_exit_DLL_sha256=sha(d/'jp_native_exit.dll'),DLL=str(d/'jp_native_logo_start.dll'),DLL_sha256=sha(d/'jp_native_logo_start.dll'),
        fixture_DLL=str(d/'sdk_heap_exit_fixture.dll'),fixture_DLL_sha256=sha(d/'sdk_heap_exit_fixture.dll'),SDK_heap_baseline_build_sha256=sha(old/'build_verification.json'),
        native_CRT_allocators_replaced=False,SDK_heap_closes_after_all_other_selected_owners=True,classic_exit_ABI_and_masks_preserved=True,
        artifact_publishable=False,playable=False,overall_completion_percent=10)
    # Dedicated allocator view keeps the established original-machine verifier
    # and exact API client workflow reusable without any test-body replacement.
    hv=d/'heap_view';hv.mkdir()
    for n in h['generated_sources_sha256']:shutil.copyfile(d/n,hv/n)
    shutil.copyfile(d/'jp_native_sdk_heap.lib',hv/'jp_native_sdk_heap.lib')
    heap_sources=normalize(h['authored_source_sha256'])
    for n in history:
        if n in heap_sources:heap_sources[n]=sha(r/n)
    heap_sources[Path(__file__).resolve().relative_to(r).as_posix()]=sha(__file__)
    heap_meta=dict(h,status='actual_SDK_heap_with_terminal_exit_delegation_linked_unverified',commands=commands[:2]+h['commands'],DLL=shared['SDK_heap_DLL'],DLL_sha256=shared['SDK_heap_DLL_sha256'],authored_source_sha256=heap_sources,superseded_authored_source=history)
    (hv/'build_verification.json').write_text(json.dumps(heap_meta,indent=2)+'\n')
    prior=read(Path(b['frame_build'])/'build_verification.json');frame=dict(prior,**shared);fd=d/'frame_view';fd.mkdir()
    for n in generated:
        target=fd/n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(d/n,target)
    (fd/'build_verification.json').write_text(json.dumps(frame,indent=2)+'\n')
    result=dict(b,**shared);result.update(frame_build=str(fd),frame_build_sha256=sha(fd/'build_verification.json'),heap_build=str(hv),heap_build_sha256=sha(hv/'build_verification.json'))
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
