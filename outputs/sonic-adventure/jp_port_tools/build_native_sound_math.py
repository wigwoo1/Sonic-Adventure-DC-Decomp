"""Complete genuine sound math dependencies and four native shared-state imports."""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path
from sound_state_imports import prepare
PIN='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();base=a.baseline.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir();b=read(base/'build_verification.json')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(base/'external_game_sound.c')==b['spatial_generated_source_sha256']
    for src in base.iterdir():
        if src.is_file() and src.suffix in ('.c','.h','.obj','.dll','.lib'):shutil.copyfile(src,d/src.name)
    for n in ('include','inputs'):shutil.copytree(base/n,d/n)
    for n,h in b['generated_sources_sha256'].items():
        src=base/n;dst=d/n;assert src.resolve().is_relative_to(base) and dst.resolve().is_relative_to(d) and sha(src)==h,n
        dst.parent.mkdir(parents=True,exist_ok=True)
        if dst.exists():assert sha(dst)==h,n
        else:shutil.copyfile(src,dst)
    imports=d/'sound_imports';originals,headers=prepare(u,imports)
    for n in ('sdk/ninja/njSin.c','sdk/ninja/njCos.c','sdk/ninja/njSqrt.c','src/collision/mesh.src'):originals[n]=sha(u/n)
    git='C:/Program Files/Git/cmd/git.exe'
    for n,h in originals.items():assert hashlib.sha256(subprocess.check_output([git,'-C',str(u),'show',PIN+':'+n])).hexdigest()==h,n
    preserved={};updates={}
    for key,value in b.items():
        if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and Path(value).suffix=='.dll':
            assert sha(value)==b[key+'_sha256'];preserved[key]=dict(path=value,sha256=sha(value));updates[key]=str(d/Path(value).name);updates[key+'_sha256']=sha(updates[key])
    source=d/'external_game_sound.c';source.write_bytes(source.read_bytes()+b'\n'+(p/'jp_port_tools/sound_state_observation.inc').read_bytes())
    def compile_row(name):return list(next(row['command'] for row in b['commands'] if '-c' in row['command'] and Path(row['command'][row['command'].index('-c')+1])==base/name))
    sdk=compile_row('external_game_sound.c');owner=compile_row('sound_owner_qsound.c')
    spatial=list(next(row['command'] for row in b['commands'] if '-c' in row['command'] and Path(row['command'][row['command'].index('-c')+1])==p/'native/jp_native_sound_spatial.c'))
    link=list(next(row['command'] for row in b['commands'] if '-shared' in row['command'] and Path(row['command'][-1])==Path(b['game_sound_DLL'])))
    sdk,owner,spatial,link=([v.replace(str(base),str(d)) for v in cmd] for cmd in (sdk,owner,spatial,link))
    # Import overlays must precede all original game include directories.
    sdk[1:1]=['-I'+str(imports/'src'),'-I'+str(imports)]
    native=p/'native/jp_native_sound_math.c';math=[sdk[0],'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-Wall','-Wextra','-Werror','-c',str(native),'-o',str(d/'sound_math.obj')]
    link[4:4]=[str(d/'sound_math.obj')];commands=[]
    for i,cmd in enumerate((sdk,owner,spatial,math,link)):
        run=subprocess.run(cmd,capture_output=True);log=d/f'math_build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        commands.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'math_compile_attempt.json').write_text(json.dumps(commands,indent=2)+'\n')
        assert not run.returncode and not log.stat().st_size,(log,log.read_text())
    for key in ('game_sound_DLL','sound_SDK_DLL'):updates[key+'_sha256']=sha(updates[key])
    authored=[native,p/'native/jp_native_sound_math.h',p/'jp_port_tools/sound_state_imports.py',p/'jp_port_tools/sound_state_observation.inc',Path(__file__).resolve()]
    updates.update(status='original_sound_math_and_actual_shared_state_imports_linked_unverified',commands=commands+b['commands'],
        authored_source_sha256=dict(b['authored_source_sha256'],**{x.relative_to(r).as_posix():sha(x) for x in authored}),
        sound_math_baseline_sha256=sha(base/'build_verification.json'),sound_math_preserved_prior_DLLs=preserved,
        sound_math_original_source_sha256=originals,sound_state_import_header_sha256=headers,
        sound_math_generated_source_sha256=sha(source),spatial_generated_source_sha256=sha(source),qsound_generated_source_sha256=sha(source),game_sound_generated_source_sha256=sha(source),
        sound_math_numeric_policy='toward_zero_DN1_exceptions_disabled_value_only',sound_state_imports=['camera_work','event_skipping','frame_redrawing','pad_records'],
        full_game_queue_streams_or_main_gameplay_verified=False,public_artifact_qualified=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print(updates['status'],flush=True)
if __name__=='__main__':main()
