"""Link the entire unchanged original save.c with genuine shared progress data.
Split the old boot owner to keep its real defaults/ring/demo/slot state without
duplicating save definitions. Original game source and DLLs stay private.
"""
import argparse,hashlib,json,re,shutil,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','baseline','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args()
    for n in vars(a):setattr(a,n,getattr(a,n).resolve())
    r,u,base,c,d=a.workspace,a.upstream,a.baseline,a.compiler,a.output;p=r/'outputs/sonic-adventure'
    b=read(base/'build_verification.json');fb=read(Path(b['frame_build'])/'build_verification.json')
    assert b['original_boot_init_body_unchanged'] and sha(Path(b['frame_build'])/'build_verification.json')==b['frame_build_sha256'] and sha(c)==b['compiler_sha256']
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(base/n)==h,n
    assert subprocess.check_output(['git','-C',str(u),'rev-parse','HEAD'],text=True).strip()=='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
    assert not subprocess.check_output(['git','-C',str(u),'status','--porcelain','--untracked-files=no'],text=True).strip()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists();d.mkdir();(d/'frames').mkdir()
    raw=(base/'external_boot_save_state.c').read_bytes();assert raw.count(b'struct save saves[SAVE_COUNT];')==1;raw=raw.replace(b'struct save saves[SAVE_COUNT];',b'/* saves is defined once by the complete original save source. */')
    removed=['reset_shooting_scores','reset_mole_scores','reset_trial_times','save_checksum','save_reset_all','save_reset','save_check_all']
    for n in removed:
        body=function(raw,n);assert raw.count(body)==1;raw=raw.replace(body,b'')
    needle=rb'^__declspec\(dllexport\) unsigned jp_save_checksum\([^\r\n]*\r?\n';assert len(re.findall(needle,raw,re.M))==1;raw=re.sub(needle,b'',raw,flags=re.M)
    retained=d/'external_retained_boot_owner.c';retained.write_bytes(raw)
    original=(u/'src/save/save.c').read_bytes();save=d/'external_complete_save.c';save.write_bytes(original+b'\n__declspec(dllexport) unsigned jp_save_checksum(const void *data){return save_checksum((struct save *)data);}\n')
    progress=(u/'src/progress/progress.c').read_bytes();stage=(u/'src/stage/stage.c').read_bytes()
    fields=['stage_clears','event_flags','story_records','dat_8c7a2718','events_played','lives','play_time'];declarations=[]
    for name in fields:
        src=stage if name in ['lives','play_time'] else progress
        found=re.findall(rb'^[A-Za-z_][^;\r\n]*\b'+name.encode()+rb'(?:\[[^;\r\n]*\])?;\r?\n',src,re.M);assert len(found)==1,(name,found);declarations.append(found[0])
    state=d/'external_shared_save_progress.c';state.write_bytes(b'#include "progress/progress.h"\n#include "stage/stage.h"\n'+b'\n'.join(declarations)+b'\n_Static_assert(sizeof(event_flags)==512 && sizeof(events_played)==512 && sizeof(stage_clears)==8*43 && sizeof(story_records)==8*12,"original progress scalar layouts");\n')
    common=list(b['commands'][0]['command']);common=common[:common.index('-c')]
    commands=[common+['-c',str(retained),'-o',str(d/'retained_boot.obj')],common+['-Wno-unused-variable','-c',str(save),'-o',str(d/'complete_save.obj')],common+['-c',str(state),'-o',str(d/'shared_save_progress.obj')]]
    fl=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b['frames_DLL']))
    previous=str(base/'boot_save_state.obj');assert fl.count(previous)==1;index=fl.index(previous);fl[index:index+1]=[str(d/'retained_boot.obj'),str(d/'complete_save.obj'),str(d/'shared_save_progress.obj')]
    kept={str(Path(n).resolve()):sha(n) for n in fl if n.endswith('.obj') and Path(n).parent!=d}
    assert not any(s['section']>0 and s['name'] in fields+['saves','stage_last_cleared'] for n in kept for s in coff_symbols(Path(n)))
    at=fl.index('-o');fl[at:at]=['-Wl,/export:'+n for n in ['save_store','save_update','save_load']]+[v for n in fields+['stage_last_cleared'] for v in ['-Xlinker','/export:'+n+',DATA']]
    fl[-1]=str(d/'frames/jp_native_frames.dll');commands.append(fl)
    pl=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b['present_DLL']))
    old=str(Path(b['frames_DLL']).with_suffix('.lib'));assert pl.count(old)==1;pl[pl.index(old)]=str(d/'frames/jp_native_frames.lib');pl[-1]=str(d/'jp_native_present.dll');commands.append(pl)
    el=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b['authored_exit_DLL']))
    for old,new in [(Path(b['frames_DLL']).with_suffix('.lib'),d/'frames/jp_native_frames.lib'),(Path(b['present_DLL']).with_suffix('.lib'),d/'jp_native_present.lib')]:assert el.count(str(old))==1;el[el.index(str(old))]=str(new)
    el[-1]=str(d/'jp_native_exit.dll');commands.append(el)
    ll=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b['DLL']))
    for old,new in [(Path(b['frames_DLL']).with_suffix('.lib'),d/'frames/jp_native_frames.lib'),(Path(b['present_DLL']).with_suffix('.lib'),d/'jp_native_present.lib'),(Path(b['authored_exit_DLL']).with_suffix('.lib'),d/'jp_native_exit.lib')]:assert ll.count(str(old))==1;ll[ll.index(str(old))]=str(new)
    ll[-1]=str(d/'jp_native_logo_start.dll');commands.append(ll)
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,cwd=r,capture_output=True);log=d/('build_'+str(i)+'.txt');log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6500]
    for n,h in kept.items():assert sha(n)==h,n
    sources=[retained,save,state];frame=dict(fb,status='entire_original_save_source_and_genuine_shared_progress_state_linked_behavior_unverified',commands=results,exit_code=0,diagnostic_bytes=0,frames_DLL=str(d/'frames/jp_native_frames.dll'),frames_DLL_sha256=sha(d/'frames/jp_native_frames.dll'),DLL=str(d/'jp_native_present.dll'),DLL_sha256=sha(d/'jp_native_present.dll'),kept_frame_objects_sha256=kept,complete_original_save_source_linked=True,save_store_load_update_linked=True,shared_progress_state_fields=fields,saves_defined_only_by_original_complete_save_source=True,original_save_source_sha256=sha(u/'src/save/save.c'),original_progress_source_sha256=sha(u/'src/progress/progress.c'),original_stage_source_sha256=sha(u/'src/stage/stage.c'),original_save_source_body_rewritten=False,warning_exception='unused save_stage_lists original table, documented by upstream',artifact_publishable=False,playable=False,overall_completion_percent=10)
    frame['authored_source_sha256']=dict(b['authored_source_sha256']);frame['authored_source_sha256'][Path(__file__).resolve().relative_to(r).as_posix()]=sha(Path(__file__))
    frame['generated_sources_sha256']={f.name:sha(f) for f in sources};fd=d/'frame_view';fd.mkdir()
    for f in sources:shutil.copyfile(f,fd/f.name)
    (fd/'build_verification.json').write_text(json.dumps(frame,indent=2)+'\n')
    shutil.copytree(base/'inputs',d/'inputs')
    result=dict(b,status='full_original_save_store_load_update_with_shared_native_boot_and_exit_linked_behavior_unverified',commands=results,exit_code=0,diagnostic_bytes=0,DLL=str(d/'jp_native_logo_start.dll'),DLL_sha256=sha(d/'jp_native_logo_start.dll'),frames_DLL=frame['frames_DLL'],frames_DLL_sha256=frame['frames_DLL_sha256'],present_DLL=frame['DLL'],authored_exit_DLL=str(d/'jp_native_exit.dll'),authored_exit_DLL_sha256=sha(d/'jp_native_exit.dll'),frame_build=str(fd),frame_build_sha256=sha(fd/'build_verification.json'),previous_boot_build_sha256=sha(base/'build_verification.json'),complete_original_save_source_linked=True,save_store_load_update_linked=True,shared_progress_state_fields=fields,disk_persistence_implemented=False,artifact_publishable=False,playable=False,overall_completion_percent=10)
    result['authored_source_sha256']=frame['authored_source_sha256'];result['generated_sources_sha256']={f.name:sha(f) for f in sources}
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'])
if __name__=='__main__':main()
