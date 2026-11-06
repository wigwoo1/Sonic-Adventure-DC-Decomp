"""Actual original qsound_place with shared live matrix and exact value policy."""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path
from build_native_texture_runtime import function
PIN='25a3d3154dbe657325e97dbe8c5291086f6af5c0'

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for name in ('workspace','upstream','baseline','output'):ap.add_argument('--'+name,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();base=a.baseline.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir();b=read(base/'build_verification.json')
    for name,expected in b['authored_source_sha256'].items():assert sha(r/name)==expected,name
    assert sha(base/'external_game_sound.c')==b['qsound_generated_source_sha256']
    assert sha(base/'sound_owner_qsound.c')==b['qsound_owner_generated_sha256']
    originals={name:sha(u/name) for name in ('src/sound/sound.c','src/collision/mesh.src','sdk/ninja/njUnitVector.c','sdk/include/ninja_zam.h')}
    git='C:/Program Files/Git/cmd/git.exe'
    for name,h in originals.items():assert hashlib.sha256(subprocess.check_output([git,'-C',str(u),'show',PIN+':'+name])).hexdigest()==h,name
    for source in base.iterdir():
        if source.is_file() and source.suffix in ('.dll','.lib','.c','.h','.obj'):shutil.copyfile(source,d/source.name)
    for name in ('include','inputs'):shutil.copytree(base/name,d/name)
    # Frozen inherited verifiers also inspect nested source evidence for the
    # unchanged runtime/lifecycle DLLs. Preserve every named input by hash.
    for name,expected in b['generated_sources_sha256'].items():
        source=base/name;target=d/name
        assert source.resolve().is_relative_to(base) and target.resolve().is_relative_to(d)
        assert sha(source)==expected,name
        target.parent.mkdir(parents=True,exist_ok=True)
        if target.exists():assert sha(target)==expected,name
        else:shutil.copyfile(source,target)
    preserved={};updates={}
    for key,value in b.items():
        if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and Path(value).suffix=='.dll':
            assert sha(value)==b[key+'_sha256'];preserved[key]=dict(path=value,sha256=sha(value))
            updates[key]=str(d/Path(value).name);updates[key+'_sha256']=sha(updates[key])
    raw=(u/'src/sound/sound.c').read_bytes();original=function(raw,'qsound_place');body=original
    before=b'    side = (way.x + 1.0F) * 15.5F;\n    place = (int)side * 4;'
    after=b'    side = jp_qsound_position_side(way.x);\n    place = jp_qsound_position_channel(side);'
    assert body.count(before)==1;body=body.replace(before,after)
    source=d/'external_game_sound.c';source.write_bytes(source.read_bytes()+b'\nfloat jp_qsound_position_side(float);\nint32_t jp_qsound_position_channel(float);\n'+body+b'\n'+(p/'jp_port_tools/qsound_spatial_observation.inc').read_bytes())
    sdk=list(next(row['command'] for row in b['commands'] if '-c' in row['command'] and Path(row['command'][row['command'].index('-c')+1])==base/'external_game_sound.c'))
    owner=list(next(row['command'] for row in b['commands'] if '-c' in row['command'] and Path(row['command'][row['command'].index('-c')+1])==base/'sound_owner_qsound.c'))
    link=list(next(row['command'] for row in b['commands'] if '-shared' in row['command'] and Path(row['command'][-1])==Path(b['game_sound_DLL'])))
    sdk=[v.replace(str(base),str(d)) for v in sdk];owner=[v.replace(str(base),str(d)) for v in owner];link=[v.replace(str(base),str(d)) for v in link]
    native=p/'native/jp_native_sound_spatial.c'
    spatial=[sdk[0],'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-Wall','-Wextra','-Werror','-c',str(native),'-o',str(d/'sound_spatial.obj')]
    link[4:4]=[str(d/'sound_spatial.obj'),str(d/Path(b['frames_DLL']).with_suffix('.lib').name)]
    commands=[]
    for i,cmd in enumerate((sdk,owner,spatial,link)):
        run=subprocess.run(cmd,capture_output=True);log=d/f'spatial_build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        commands.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size))
        (d/'spatial_compile_attempt.json').write_text(json.dumps(commands,indent=2)+'\n');assert not run.returncode and not log.stat().st_size,(log,log.read_text())
    for key in ('game_sound_DLL','sound_SDK_DLL'):updates[key+'_sha256']=sha(updates[key])
    authored=[native,p/'native/jp_native_sound_spatial.h',p/'jp_port_tools/qsound_spatial_observation.inc',Path(__file__).resolve()]
    updates.update(status='original_spatial_QSound_shared_matrix_and_RZ_values_linked_unverified',commands=commands+b['commands'],
        authored_source_sha256=dict(b['authored_source_sha256'],**{path.relative_to(r).as_posix():sha(path) for path in authored}),
        spatial_baseline_sha256=sha(base/'build_verification.json'),spatial_preserved_prior_DLLs=preserved,
        spatial_original_source_sha256=originals,spatial_original_qsound_place_sha256=hashlib.sha256(original).hexdigest(),
        spatial_adapted_qsound_place_sha256=hashlib.sha256(body).hexdigest(),
        spatial_generated_source_sha256=sha(source),game_sound_generated_source_sha256=sha(source),qsound_generated_source_sha256=sha(source),
        spatial_numeric_policy='toward_zero_DN1_exceptions_disabled_value_only',
        spatial_frame_import_lib_sha256=sha(d/Path(b['frames_DLL']).with_suffix('.lib').name),
        spatial_shared_frame_DLL_sha256=b['frames_DLL_sha256'],full_game_queue_or_spatial_acoustics_verified=False,
        public_artifact_qualified=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print(updates['status'],flush=True)

if __name__=='__main__':main()
