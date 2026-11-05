"""Replace the one authored owner to support a direct SDK/font surface.
All genuine game/SDK/frame/GPU objects remain byte-for-byte unchanged.
"""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','baseline','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args()
    for n in vars(a):setattr(a,n,getattr(a,n).resolve())
    r,base,c,d=a.workspace,a.baseline,a.compiler,a.output;p=r/'outputs/sonic-adventure';b=read(base/'build_verification.json');fb=read(Path(b['frame_build'])/'build_verification.json')
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    assert sha(c)==b['compiler_sha256'] and all(row['exit_code']==row['diagnostic_bytes']==0 for row in b['commands'])
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(base/n)==h,n
    link=list(next(row['command'] for row in b['commands'] if '-shared' in row['command'] and row['command'][-1]==b['frames_DLL']));objects={n:sha(n) for n in link if n.endswith('.obj')};owners=[n for n in objects if Path(n).name=='texture_initialization_owner.obj'];assert len(owners)==1;old=owners[0]
    d.mkdir();(d/'frames').mkdir();common=list(b['commands'][0]['command']);common=common[:common.index('-c')];source=p/'native/jp_native_standalone_texture.c';commands=[common+['-c',str(source),'-o',str(d/'standalone_texture_owner.obj')]]
    link[link.index(old)]=str(d/'standalone_texture_owner.obj');kept={n:h for n,h in objects.items() if n!=old}
    existing={v.split('/export:',1)[1].split(',')[0] for v in link if '/export:' in v};at=link.index('-o');link[at:at]=['-Wl,/export:'+n for n in ['kmFreeTexture'] if n not in existing];link[-1]=str(d/'frames/jp_native_frames.dll');commands.append(link)
    newlibs={'frames_DLL':d/'frames/jp_native_frames.lib'}
    for label,name,imports in [('GPU_DLL','jp_native_window_input.dll',['frames_DLL']),('present_DLL','jp_native_present.dll',['frames_DLL','GPU_DLL']),('authored_exit_DLL','jp_native_exit.dll',['frames_DLL','GPU_DLL','present_DLL']),('DLL','jp_native_logo_start.dll',['frames_DLL','present_DLL','authored_exit_DLL'])]:
        cmd=list(next(row['command'] for row in b['commands'] if '-shared' in row['command'] and row['command'][-1]==b[label]))
        for n in imports:
            previous=str(Path(b[n]).with_suffix('.lib'));assert cmd.count(previous)==1;cmd[cmd.index(previous)]=str(newlibs[n])
        cmd[-1]=str(d/name);newlibs[label]=Path(cmd[-1]).with_suffix('.lib');commands.append(cmd)
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/('build_'+str(i)+'.txt');log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)));assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6000]
    for n,h in kept.items():assert sha(n)==h,n
    before={s['name'] for s in coff_symbols(Path(old)) if s['section']>0 and s['function']};after={s['name'] for s in coff_symbols(d/'standalone_texture_owner.obj') if s['section']>0 and s['function']};assert before<=after
    sources=dict(b['authored_source_sha256'])
    for path in [source,p/'native/jp_native_standalone_texture.h',Path(__file__).resolve()]:sources[path.relative_to(r).as_posix()]=sha(path)
    for n,h in b['generated_sources_sha256'].items():shutil.copyfile(base/n,d/n);assert sha(d/n)==h
    shutil.copytree(base/'inputs',d/'inputs')
    shared=dict(status='shared_texture_owner_direct_SDK_surface_lifetime_compiled_behavior_unverified',commands=results,exit_code=0,diagnostic_bytes=0,authored_source_sha256=sources,kept_frame_objects_sha256=kept,replaced_texture_owner_object=dict(path=old,sha256=objects[old]),standalone_texture_owner_object_sha256=sha(d/'standalone_texture_owner.obj'),old_authored_owner_functions_preserved=sorted(before),single_shared_native_texture_owner=True,standalone_SDK_surface_slots=1,one_nonrecycled_surface_ID_generator=True,record_table_reset_preserves_direct_surface=True,explicit_owner_reset_retires_direct_surface=True,standalone_bytes_in_owner_and_transient_budgets=True,previous_game_random_build_sha256=sha(base/'build_verification.json'),frames_DLL=str(d/'frames/jp_native_frames.dll'),frames_DLL_sha256=sha(d/'frames/jp_native_frames.dll'),GPU_DLL=str(d/'jp_native_window_input.dll'),GPU_DLL_sha256=sha(d/'jp_native_window_input.dll'),DLL=str(d/'jp_native_present.dll'),DLL_sha256=sha(d/'jp_native_present.dll'),all_genuine_game_and_SDK_objects_preserved=True,frame_owner_bytes_unchanged=False,genuine_print_family_compiled=False,game_containing_DLL_publishable=False,artifact_publishable=False,playable=False,overall_completion_percent=10)
    shared.update(all_prior_frame_objects_preserved=False,all_other_frame_objects_preserved=True,old_authored_texture_owner_preserved=False)
    frame=dict(fb,**shared);fd=d/'frame_view';fd.mkdir()
    for n in b['generated_sources_sha256']:shutil.copyfile(d/n,fd/n)
    (fd/'build_verification.json').write_text(json.dumps(frame,indent=2)+'\n')
    result=dict(b,**shared);result.update(DLL=str(d/'jp_native_logo_start.dll'),DLL_sha256=sha(d/'jp_native_logo_start.dll'),present_DLL=frame['DLL'],authored_exit_DLL=str(d/'jp_native_exit.dll'),authored_exit_DLL_sha256=sha(d/'jp_native_exit.dll'),frame_build=str(fd),frame_build_sha256=sha(fd/'build_verification.json'))
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
