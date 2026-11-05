"""Replace only the qualified input object; retain original game/SDK objects."""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','input','preserved','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();base=a.baseline.resolve();p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build')
    b=read(base/'build_verification.json');i=read(a.input/'build_verification.json');history={Path(n).as_posix():v for n,v in read(a.preserved/'preserved_source.json').items()}
    sources={Path(n).as_posix():v for n,v in b['authored_source_sha256'].items()}
    for n,h in sources.items():
        if n in history:assert history[n]['sha256']==h and sha(history[n]['preserved_path'])==h,n
        else:assert sha(r/n)==h,n
    for n,h in i['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(a.input/'input.obj')==i['object_sha256'];d.mkdir()
    common=i['commands'][0]['command'];common=common[:common.index('-c')];compiler=common[0]
    bridge=p/'native/jp_native_peripheral_cards.c';cc=common+['-c',str(bridge),'-o',str(d/'peripheral_cards.obj')]
    fl=list(next(row['command'] for row in b['commands'] if '-shared' in row['command'] and Path(row['command'][-1]).name=='jp_native_frames.dll'))
    normalize=lambda n:str(Path(n).resolve())
    previous={normalize(n):h for n,h in b['kept_frame_objects_sha256'].items()}
    assert {normalize(n) for n in fl if n.endswith('.obj')}==set(previous)
    for n,h in previous.items():assert sha(n)==h,n
    old=next(n for n in fl if Path(n).name=='input.obj');fl[fl.index(old)]=str((a.input/'input.obj').resolve());fl.insert(fl.index('-o'),str(d/'peripheral_cards.obj'));fl[-1]=str(d/'jp_native_frames.dll')
    results=[]
    for j,cmd in enumerate((cc,fl)):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{j}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n')
        assert not run.returncode and not log.stat().st_size,log.read_text()
    updates=dict(frames_DLL=str(d/'jp_native_frames.dll'),frames_DLL_sha256=sha(d/'jp_native_frames.dll'));copies={}
    for k,v in b.items():
        if (k=='DLL' or k.endswith('_DLL')) and k!='frames_DLL' and isinstance(v,str) and Path(v).suffix=='.dll':
            source=Path(v);assert sha(source)==b[k+'_sha256'];target=d/source.name
            shutil.copyfile(source,target);shutil.copyfile(source.with_suffix('.lib'),target.with_suffix('.lib'));updates[k]=str(target);updates[k+'_sha256']=sha(target);copies[k]=dict(path=str(target),sha256=sha(target))
    for n,h in b['generated_sources_sha256'].items():
        target=d/n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(base/n,target);assert sha(target)==h,n
    shutil.copytree(base/'inputs',d/'inputs');shutil.copyfile(base/'vblank_chain_sdk.obj',d/'vblank_chain_sdk.obj')
    for n in history:sources[n]=sha(r/n)
    sources.update(i['authored_source_sha256'])
    for f in (bridge,p/'native/jp_native_peripheral_cards.h',Path(__file__).resolve()):sources[f.relative_to(r).as_posix()]=sha(f)
    objects={n:sha(n) for n in fl if n.endswith('.obj')}
    updates.update(status='SDK_peripheral_bootstrap_real_PC_input_and_virtual_card_metadata_linked_unverified',commands=results+b['commands'],authored_source_sha256=sources,
        superseded_authored_source=history,kept_frame_objects_sha256=objects,copied_DLLs_sha256=copies,input_object_sha256=i['object_sha256'],
        input_build=str(a.input.resolve()),input_build_sha256=sha(a.input/'build_verification.json'),baseline_build_sha256=sha(base/'build_verification.json'),
        SDK_peripheral_init_export_available=True,shared_input_owner_count=1,original_game_and_SDK_objects_unchanged=True,
        negative_button_logic_supported=True,virtual_card_metadata_from_actual_attachment_owner=True,artifact_publishable=False,playable=False,overall_completion_percent=10)
    fv=d/'frame_view';fv.mkdir();frame=read(Path(b['frame_build'])/'build_verification.json');frame.update(updates)
    for n in b['generated_sources_sha256']:
        target=fv/n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(d/n,target)
    (fv/'build_verification.json').write_text(json.dumps(frame,indent=2)+'\n')
    hv=d/'heap_view';shutil.copytree(Path(b['heap_build']),hv);heap=read(hv/'build_verification.json');heap['DLL']=updates['SDK_heap_DLL'];heap['DLL_sha256']=updates['SDK_heap_DLL_sha256']
    heap['authored_source_sha256']={Path(n).as_posix():(sha(r/n) if Path(n).as_posix() in history else h) for n,h in heap['authored_source_sha256'].items()};heap['superseded_authored_source']=history;(hv/'build_verification.json').write_text(json.dumps(heap,indent=2)+'\n')
    result=dict(b,**updates,frame_build=str(fv),frame_build_sha256=sha(fv/'build_verification.json'),heap_build=str(hv),heap_build_sha256=sha(hv/'build_verification.json'))
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
