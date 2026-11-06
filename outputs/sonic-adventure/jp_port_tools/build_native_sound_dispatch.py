"""Replace scoped frame sound diagnostics with real owner-bound dispatch."""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path
from build_native_texture_runtime import function
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def read(path):return json.loads(Path(path).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','baseline','sdk','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';base=a.baseline.resolve();d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    b=read(base/'build_verification.json');sdk=read(a.sdk/'build_verification.json')
    assert sha(b['present_DLL'])==b['present_DLL_sha256'] and sha(sdk['DLL'])==sdk['DLL_sha256']
    source=base/'present_with_card_lcd.c';assert sha(source)==b['generated_sources_sha256'][source.name]
    compile_row=next(x for x in b['commands'] if '-c' in x['command'] and Path(x['command'][x['command'].index('-c')+1]).name==source.name and sha(x['command'][x['command'].index('-c')+1])==sha(source))
    compile_command=list(compile_row['command']);old_object=Path(compile_command[-1]);old_source=Path(compile_command[compile_command.index('-c')+1])
    link_row=next(x for x in b['commands'] if '-shared' in x['command'] and Path(x['command'][-1]).name==Path(b['present_DLL']).name and Path(x['command'][-1]).exists() and sha(x['command'][-1])==b['present_DLL_sha256'])
    link=list(link_row['command']);assert str(old_object) in link
    raw=source.read_bytes();changes={}
    for name,new in [('sdSysServer',b'SDE_ERR sdSysServer(void)\n{\n    int32_t result=SDE_ERR_NO_INIT;event(3);\n    (void)jp_sound_dispatch_service(&result);return (SDE_ERR)result;\n}'),
        ('sound_send_queue',b'void sound_send_queue(void)\n{\n    int32_t result=0;event(14);(void)jp_sound_dispatch_queue(&result);\n}')]:
        prefix=(b'SDE_ERR '+name.encode()+b'(void){') if name=='sdSysServer' else (b'void '+name.encode()+b'(void){')
        lines=[line for line in raw.splitlines() if line.startswith(prefix)];assert len(lines)==1,name
        old=lines[0];assert raw.count(old)==1;raw=raw.replace(old,new);changes[name]=dict(before_sha256=hashlib.sha256(old).hexdigest(),after_sha256=hashlib.sha256(new).hexdigest())
    generated=d/source.name;generated.write_bytes(b'#include "jp_native_sound_dispatch.h"\n'+raw)
    compile_command.insert(1,'-I'+str(old_source.parent));compile_command[compile_command.index('-c')+1]=str(generated);compile_command[-1]=str(d/old_object.name)
    compiler=compile_command[0];authored=p/'native/jp_native_sound_dispatch.c';header=authored.with_suffix('.h')
    native=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-c',str(authored),'-o',str(d/'sound_dispatch.obj')]
    link[link.index(str(old_object))]=str(d/old_object.name);link.insert(3,str(d/'sound_dispatch.obj'));link[-1]=str(d/'jp_native_present.dll')
    link[3:3]=['-Wl,/export:sdSysServer','-Wl,/export:sound_send_queue']
    commands=[native,compile_command,link];results=[]
    for index,command in enumerate(commands):
        run=subprocess.run(command,capture_output=True);log=d/f'build_{index}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=command,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n')
        assert not run.returncode and not log.stat().st_size,(log,log.read_text())
    updates={};preserved={}
    for key,value in b.items():
        if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and Path(value).suffix=='.dll':
            source_path=Path(value);target=d/source_path.name
            if key=='present_DLL':updates[key]=str(d/'jp_native_present.dll');updates[key+'_sha256']=sha(d/'jp_native_present.dll');continue
            assert sha(source_path)==b[key+'_sha256'];shutil.copyfile(source_path,target)
            library=source_path.with_suffix('.lib')
            if library.exists():shutil.copyfile(library,target.with_suffix('.lib'))
            updates[key]=str(target);updates[key+'_sha256']=sha(target);preserved[key]=dict(path=str(source_path),sha256=sha(source_path))
    for name,h in sdk['dependencies'].items():assert sha(a.sdk/name)==h;shutil.copyfile(a.sdk/name,d/name)
    shutil.copyfile(sdk['DLL'],d/'jp_native_sd_commands.dll')
    shutil.copyfile(Path(sdk['DLL']).with_suffix('.lib'),d/'jp_native_sd_commands.lib')
    generated_hashes={}
    for name,h in b['generated_sources_sha256'].items():
        original=base/name;assert sha(original)==h,name
        target=d/name
        if name!=generated.name:
            target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(original,target)
        generated_hashes[name]=sha(target)
    shutil.copytree(base/'inputs',d/'inputs')
    updates.update(status='real_owner_bound_frame_sound_dispatch_linked_unverified',commands=results+b['commands'],
        baseline_build_sha256=sha(base/'build_verification.json'),sound_SDK_build_sha256=sha(a.sdk/'build_verification.json'),
        dispatch_source_sha256=sha(authored),dispatch_header_sha256=sha(header),builder_sha256=sha(__file__),
        dispatch_generated_source_sha256=sha(generated),dispatch_function_adaptations=changes,preserved_prior_DLLs=preserved,
        generated_sources_sha256=generated_hashes,
        sound_SDK_DLL=str(d/'jp_native_sd_commands.dll'),sound_SDK_DLL_sha256=sha(d/'jp_native_sd_commands.dll'),
        unbound_sound_service_returns_success=False,original_game_sound_constructor_or_queue_implemented=False,
        public_artifact_qualified=False,playable=False,overall_completion_percent=10)
    updates['authored_source_sha256']=dict(b['authored_source_sha256'],**{x.relative_to(r).as_posix():sha(x) for x in (authored,header,Path(__file__).resolve())})
    result=dict(b,**updates);(d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(updates['status'],flush=True)
if __name__=='__main__':main()
