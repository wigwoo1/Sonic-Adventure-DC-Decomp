"""Link genuine njInitTexture with the single native texture/frame owner.
External SDK bodies remain private. Preserve the previous runtime and proofs.
"""
import argparse,hashlib,json,shutil,subprocess
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
    r,u,base,c,d=a.workspace,a.upstream,a.baseline,a.compiler,a.output;p=r/'outputs/sonic-adventure';b=read(base/'build_verification.json');fb=read(Path(b['frame_build'])/'build_verification.json')
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    assert sha(c)==b['compiler_sha256'] and all(row['exit_code']==row['diagnostic_bytes']==0 for row in b['commands'])
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(base/n)==h,n
    assert subprocess.check_output(['git','-C',str(u),'rev-parse','HEAD'],text=True).strip()=='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
    assert not subprocess.check_output(['git','-C',str(u),'status','--porcelain','--untracked-files=no'],text=True).strip()
    link=list(next(row['command'] for row in b['commands'] if '-shared' in row['command'] and row['command'][-1]==b['frames_DLL']))
    objects={n:sha(n) for n in link if n.endswith('.obj')};owners=[n for n in objects if Path(n).name=='textures_move.obj'];assert len(owners)==1
    owner=owners[0];assert not any(s['section']>0 and s['name'] in ['njInitTexture','_nj_current_texture_path','_nj_tex_err_surface'] for n in objects for s in coff_symbols(Path(n)))
    d.mkdir();(d/'frames').mkdir();raw=function((u/'sdk/ninja/njTexture.c').read_bytes(),'njInitTexture');adapted=raw.replace(b'{',b'{\n    if (!jp_texture_begin_sdk_init(addr,n)) return;',1)
    at=adapted.rfind(b'}');adapted=adapted[:at]+b'    jp_texture_finish_sdk_init();\n'+adapted[at:]
    sdk=d/'external_texture_init.c';sdk.write_bytes(b'#include "jp_native_texture_initialization.h"\n#include <kamui.h>\n#include <sg_xpt.h>\n'+adapted)
    common=list(b['commands'][0]['command']);common=common[:common.index('-c')];src=p/'native/jp_native_texture_initialization.c'
    commands=[common+['-c',str(src),'-o',str(d/'texture_initialization_owner.obj')],common+['-c',str(sdk),'-o',str(d/'texture_initialization_sdk.obj')]]
    link[link.index(owner)]=str(d/'texture_initialization_owner.obj');kept={n:h for n,h in objects.items() if n!=owner}
    data=['_nj_current_texture_path','_nj_tex_err_surface','_nj_texerr_','_nj_current_texlist','_nj_texmemlist','_nj_texmemsize']
    link[link.index('-o'):link.index('-o')]=[str(d/'texture_initialization_sdk.obj'),'-Wl,/export:njInitTexture','-Wl,/export:njInitTexMemList','-Wl,/export:jp_texture_renderer_select',*[arg for name in data for arg in ['-Xlinker','/export:'+name+',DATA']]];link[-1]=str(d/'frames/jp_native_frames.dll');commands.append(link)
    newlibs={'frames_DLL':d/'frames/jp_native_frames.lib'}
    for label,name,imports in [('GPU_DLL','jp_native_window_input.dll',['frames_DLL']),('present_DLL','jp_native_present.dll',['frames_DLL','GPU_DLL']),('authored_exit_DLL','jp_native_exit.dll',['frames_DLL','GPU_DLL','present_DLL']),('DLL','jp_native_logo_start.dll',['frames_DLL','present_DLL','authored_exit_DLL'])]:
        cmd=list(next(row['command'] for row in b['commands'] if '-shared' in row['command'] and row['command'][-1]==b[label]))
        for n in imports:
            old=str(Path(b[n]).with_suffix('.lib'));assert cmd.count(old)==1;cmd[cmd.index(old)]=str(newlibs[n])
        cmd[-1]=str(d/name);newlibs[label]=Path(cmd[-1]).with_suffix('.lib');commands.append(cmd)
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/('build_'+str(i)+'.txt');log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6000]
    for n,h in kept.items():assert sha(n)==h,n
    oldfunctions={s['name'] for s in coff_symbols(Path(owner)) if s['section']>0 and s['function']}
    newfunctions={s['name'] for s in coff_symbols(d/'texture_initialization_owner.obj') if s['section']>0 and s['function']};assert oldfunctions<=newfunctions
    authored=dict(b['authored_source_sha256'])
    for path in [src,p/'native/jp_native_texture_initialization.h',Path(__file__).resolve()]:authored[path.relative_to(r).as_posix()]=sha(path)
    generated=dict(b['generated_sources_sha256']);generated[sdk.name]=sha(sdk)
    for n,h in b['generated_sources_sha256'].items():shutil.copyfile(base/n,d/n);assert sha(d/n)==h
    shutil.copytree(base/'inputs',d/'inputs')
    shared=dict(status='genuine_texture_initializer_and_caller_owned_native_record_table_linked_behavior_unverified',exit_code=0,diagnostic_bytes=0,commands=results,authored_source_sha256=authored,generated_sources_sha256=generated,kept_frame_objects_sha256=kept,replaced_texture_owner_object=dict(path=owner,sha256=objects[owner]),old_owner_external_functions_preserved=sorted(oldfunctions),authored_texture_initialization_owner_sha256=sha(d/'texture_initialization_owner.obj'),external_initializer_function_sha256=hashlib.sha256(raw).hexdigest(),external_initializer_native_adaptation_sha256=hashlib.sha256(adapted).hexdigest(),external_texture_source_sha256=sha(u/'sdk/ninja/njTexture.c'),original_source_files_modified=False,single_shared_native_texture_owner=True,caller_owned_tables_supported=True,SDK_zero_record_initialization_supported=True,old_game_GPU_objects_preserved=True,frame_owner_bytes_unchanged=False,previous_display_setup_build_sha256=sha(base/'build_verification.json'),frames_DLL=str(d/'frames/jp_native_frames.dll'),frames_DLL_sha256=sha(d/'frames/jp_native_frames.dll'),GPU_DLL=str(d/'jp_native_window_input.dll'),GPU_DLL_sha256=sha(d/'jp_native_window_input.dll'),DLL=str(d/'jp_native_present.dll'),DLL_sha256=sha(d/'jp_native_present.dll'),game_containing_DLL_publishable=False,artifact_publishable=False,playable=False,overall_completion_percent=10)
    frame=dict(fb,**shared);fd=d/'frame_view';fd.mkdir()
    for n in generated:shutil.copyfile(d/n,fd/n)
    (fd/'build_verification.json').write_text(json.dumps(frame,indent=2)+'\n')
    result=dict(b,**shared);result.update(DLL=str(d/'jp_native_logo_start.dll'),DLL_sha256=sha(d/'jp_native_logo_start.dll'),present_DLL=frame['DLL'],authored_exit_DLL=str(d/'jp_native_exit.dll'),authored_exit_DLL_sha256=sha(d/'jp_native_exit.dll'),frame_build=str(fd),frame_build_sha256=sha(fd/'build_verification.json'))
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
