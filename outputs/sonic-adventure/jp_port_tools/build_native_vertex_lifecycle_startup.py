"""Compose qualified real lifecycle/frame/presentation with original boot/logo.
External game/SDK bodies and linked DLLs stay private. Old inputs are preserved.
"""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','baseline','lifecycle','original-proof','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();base=a.baseline.resolve();life=a.lifecycle.resolve();d=a.output.resolve();b=read(base/'build_verification.json');v=read(life/'verification.json');original=read(a.original_proof/'verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build')
    assert original['build_proof_sha256']==sha(life/'verification.json') and original['original_entry_calls']==130 and original['checks']==2081 and not original['original_initialization_or_float_operations_hooked']
    assert v['checks']==25 and v['source_sha256']==sha(r/'outputs/sonic-adventure/native/jp_native_vertex_lifecycle.c') and v['verifier_sha256']==sha(r/'outputs/sonic-adventure/jp_port_tools/verify_native_vertex_lifecycle.py')
    assert all(x['exit_code']==x['diagnostic_bytes']==0 for x in v['commands'])
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(base/n)==h,n
    for name in ['texture_cache','vblank_cancel']:
        assert v[('cache' if name=='texture_cache' else 'wait')+'_source_sha256']==sha(r/f'outputs/sonic-adventure/native/jp_native_{name}.c')
    d.mkdir();commands=[];libraries={'frames_DLL':life/'jp_native_frames.lib','present_DLL':life/'jp_native_present.lib','GPU_DLL':Path(b['GPU_DLL']).with_suffix('.lib')}
    for label,name,imports in [('authored_exit_DLL','jp_native_exit.dll',['frames_DLL','GPU_DLL','present_DLL']),('DLL','jp_native_logo_start.dll',['frames_DLL','present_DLL','authored_exit_DLL'])]:
        cmd=list(next(x['command'] for x in b['commands'] if x['command'][-1]==b[label]))
        for n in imports:
            prior=str(Path(b[n]).with_suffix('.lib'));assert cmd.count(prior)==1;cmd[cmd.index(prior)]=str(libraries[n])
        cmd[-1]=str(d/name);libraries[label]=Path(cmd[-1]).with_suffix('.lib')
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{len(commands)}.txt';log.write_bytes(run.stdout+run.stderr);commands.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));assert run.returncode==0 and not log.stat().st_size,log.read_text()
    sources=dict(b['authored_source_sha256'])
    for name in ['jp_native_texture_budget','jp_native_texture_space','jp_native_texture_cache','jp_native_vertex_lists','jp_native_vertex_lifecycle','jp_native_vblank_cancel']:
        for ext in ['.c','.h']:
            path=r/'outputs/sonic-adventure/native'/(name+ext)
            if path.exists():sources[path.relative_to(r).as_posix()]=sha(path)
    for name in ['verify_native_texture_budget','verify_native_texture_space','verify_native_vertex_lists','verify_native_vertex_lifecycle','verify_native_vertex_lifecycle_original','build_native_vertex_lifecycle_startup']:
        path=r/'outputs/sonic-adventure/jp_port_tools'/(name+'.py');sources[path.relative_to(r).as_posix()]=sha(path)
    generated=dict(b['generated_sources_sha256'])
    for n,h in generated.items():target=d/n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(base/n,target);assert sha(target)==h
    # The exact newly linked original query SDK object remains in its retained
    # original build directory; record its source without publishing its body.
    sdk=next(Path(n) for n in v['commands'][1]['command'] if Path(n).name=='texture_space_sdk.obj')
    for name,path in [('lifecycle/jp_vertex_lifecycle_imports.h',life/'jp_vertex_lifecycle_imports.h'),('lifecycle/external_texture_space.c',sdk.parent/'external_texture_space.c')]:
        target=d/name;target.parent.mkdir(exist_ok=True);shutil.copyfile(path,target);generated[name]=sha(target)
    shutil.copytree(base/'inputs',d/'inputs');shutil.copyfile(base/'vblank_chain_sdk.obj',d/'vblank_chain_sdk.obj')
    frame_objects={n:sha(n) for n in v['commands'][1]['command'] if n.endswith('.obj')}
    present_objects={n:sha(n) for n in v['commands'][3]['command'] if n.endswith('.obj')}
    shared=dict(status='qualified_native_vertex_lifecycle_original_boot_startup_linked',commands=v['commands']+commands,exit_code=0,diagnostic_bytes=0,authored_source_sha256=sources,generated_sources_sha256=generated,kept_frame_objects_sha256=frame_objects,kept_presentation_objects_sha256=present_objects,frames_DLL=str(life/'jp_native_frames.dll'),frames_DLL_sha256=sha(life/'jp_native_frames.dll'),vertex_lifecycle_DLL=str(life/'jp_native_vertex_lifecycle.dll'),vertex_lifecycle_DLL_sha256=sha(life/'jp_native_vertex_lifecycle.dll'),present_DLL=str(life/'jp_native_present.dll'),lifecycle_proof_sha256=sha(life/'verification.json'),original_scalar_proof_sha256=sha(a.original_proof/'verification.json'),original_njCalcTexture_object_sha256=sha(sdk),artifact_publishable=False,game_containing_DLL_publishable=False,playable=False,overall_completion_percent=10)
    fb=read(Path(b['frame_build'])/'build_verification.json');frame=dict(fb,**shared);frame.update(DLL=shared['present_DLL'],DLL_sha256=sha(shared['present_DLL']));fd=d/'frame_view';fd.mkdir()
    for name in generated:target=fd/name;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(d/name,target)
    (fd/'build_verification.json').write_text(json.dumps(frame,indent=2)+'\n')
    result=dict(b,**shared);result.update(DLL=str(d/'jp_native_logo_start.dll'),DLL_sha256=sha(d/'jp_native_logo_start.dll'),authored_exit_DLL=str(d/'jp_native_exit.dll'),authored_exit_DLL_sha256=sha(d/'jp_native_exit.dll'),frame_build=str(fd),frame_build_sha256=sha(fd/'build_verification.json'))
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
