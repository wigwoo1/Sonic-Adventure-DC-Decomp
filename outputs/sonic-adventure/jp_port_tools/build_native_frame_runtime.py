"""Replace the real queue/texture owners with retained frame versions.
All complete model/sprite/primitive producers remain real and unchanged.
Diagnostic frame/replay observers do not implement render or flip.
"""
import argparse,hashlib,json,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','baseline','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';u=a.upstream.resolve();d=a.output.resolve();base=a.baseline.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    b=json.loads((base/'build_verification.json').read_text());assert b['exit_code']==b['diagnostic_bytes']==0 and sha(a.compiler)==b['compiler_sha256']
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    link=list(next(c['command'] for c in b['commands'] if '-shared' in c['command']));objects={Path(v):sha(Path(v)) for v in link if v.endswith('.obj')}
    for n,h in b['existing_provider_objects_sha256'].items():assert objects[Path(n)]==h,n
    replaced={}
    for basename in ['queue.obj','jp_native_textures.obj']:
        old=[v for v in objects if v.name==basename];assert len(old)==1;replaced[str(old[0])]=objects.pop(old[0])
    old_probe=[v for v in objects if v.name=='primitive_probe.obj'];assert len(old_probe)==1
    replaced_probe={str(old_probe[0]):objects.pop(old_probe[0])}
    pin=json.loads((p/'jp_port_tools/jp_task_external_inputs.json').read_text())
    for n,h in pin['files_sha256'].items():assert sha(u/n)==h,n
    d.mkdir(parents=True)
    flags=['--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-ffreestanding','-fno-builtin','-Wall','-Wextra','-Werror','-Wno-unknown-pragmas',
        '-include',str(p/'native/jp_host_types.h'),'-I'+str(p/'native'),'-I'+str(u/'sdk/include'),'-I'+str(u)]
    names={'queue_frames':'jp_native_queue_frames.c','textures_retained':'jp_native_textures_retained.c','frames':'jp_native_frames.c'}
    commands=[[str(a.compiler),*flags,'-c',str(p/'native'/src),'-o',str(d/(name+'.obj'))] for name,src in names.items()]
    probe=p/'jp_port_tools/frame_diagnostic_probe.c';commands.append([str(a.compiler),*flags,'-c',str(probe),'-o',str(d/'frame_probe.obj')])
    link=[v for v in link if not v.endswith('.obj')];link[-1]=str(d/'jp_native_frames.dll');at=link.index('-o');link[at:at]=[*map(str,objects),*[str(d/(n+'.obj')) for n in names],str(d/'frame_probe.obj')]
    exports=['njStartVertexNonTexOpaqueIntens','njStartVertexNonTexTransIntens','njStartVertexTexOpaqueIntens','njStartVertexTexTransIntens','njStartVertexTexOpaqueIntensH','njStartVertexTexTransIntensH','njStartVertexTexPunch','njStartVertexTexPunchIntens','njStartVertexTexPunchIntensH']
    existing={v.split('/export:',1)[1].split(',')[0] for v in link if '/export:' in v}
    link[1:1]=['-Wl,/export:'+n for n in exports if n not in existing]
    commands.append(link);results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6000]
    authored=dict(b['authored_source_sha256']);paths=[p/'native'/v for v in names.values()]+[p/'native/jp_native_frames.h',p/'native/jp_native_frames_internal.h',probe,Path(__file__)]
    for path in paths:authored[path.resolve().relative_to(r).as_posix()]=sha(path)
    dll=d/'jp_native_frames.dll'
    report=dict(status='actual_producers_and_owned_frame_runtime_linked_behavior_unqualified',exit_code=0,diagnostic_bytes=0,commands=results,compiler_sha256=sha(a.compiler),authored_source_sha256=authored,
        DLL=str(dll),DLL_sha256=sha(dll),existing_provider_objects_sha256={str(o):h for o,h in objects.items()},existing_diagnostic_objects_sha256={str(o):h for o,h in objects.items()},
        production_objects_sha256={n+'.obj':sha(d/(n+'.obj')) for n in names},replaced_owner_objects_sha256=replaced,replaced_diagnostic_objects_sha256=replaced_probe,diagnostic_texture_fixture_uses_actual_SDK_parameter_builder=True,baseline_report_sha256=sha(base/'build_verification.json'),
        preserved_owner_sources_sha256={n:sha(p/'native'/n) for n in ['jp_native_queue.c','jp_native_queue_intensity.c','jp_native_textures.c']},
        one_queue_matrix_texture_and_render_state_owner=True,complete_model_source_linked=True,game_source_linked=True,artifact_publishable=False,diagnostic_frame_replay_services=True,
        owned_submission_verified=False,render_flip_implemented=False,full_frame_executed=False,rendered_pixels_verified=False,playable=False,overall_completion_percent=10,goal_paused=False)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(report['status'],'zero diagnostics',flush=True)
if __name__=='__main__':main()
