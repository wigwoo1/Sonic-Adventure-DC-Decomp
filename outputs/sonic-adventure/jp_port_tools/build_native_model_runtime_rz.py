"""Link complete actual model producer with the qualified primitive runtime.
Model bodies and all native state/texture/queue/math providers are real.
Frame/replay observers and static-state accessors are diagnostic, not a game.
"""
import argparse,hashlib,json,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','baseline','model','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();m=a.model.resolve();bdir=a.baseline.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    b=json.loads((bdir/'build_verification.json').read_text());mb=json.loads((m/'build_verification.json').read_text())
    assert b['exit_code']==b['diagnostic_bytes']==mb['exit_code']==mb['diagnostic_bytes']==0
    assert b['compiler_sha256']==mb['compiler_sha256']==sha(a.compiler)
    for x in [b,mb]:
        for n,h in x['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(m/'external_model_native_rz.c')==mb['private_generated_source_sha256']
    link=list(next(v['command'] for v in b['commands'] if '-shared' in v['command']))
    objects={Path(v):sha(Path(v)) for v in link if v.endswith('.obj')}
    for n,h in b['existing_provider_objects_sha256'].items():assert objects[Path(n)]==h,n
    d.mkdir(parents=True);probe=p/'jp_port_tools/model_diagnostic_probe.c'
    flags=mb['commands'][0]['command'][1:];at=flags.index('-c');flags=flags[:at]
    cmd=[str(a.compiler),*flags,'-DJP_MODEL_SOURCE="'+(m/'external_model_native_rz.c').as_posix()+'"','-c',str(probe),'-o',str(d/'model_diagnostic.obj')]
    pf=p/'native/jp_model_preflight.c';pf_flags=list(flags)
    compat=str(p/'native/jp_renderer_host_compat_rz.h');at=pf_flags.index(compat);assert pf_flags[at-1]=='-include';del pf_flags[at-1:at+1]
    pf_cmd=[str(a.compiler),*pf_flags,'-c',str(pf),'-o',str(d/'preflight.obj')]
    link=[v for v in link if not v.endswith('.obj')];link[-1]=str(d/'jp_native_model_rz.dll');at=link.index('-o');link[at:at]=[*map(str,objects),str(d/'model_diagnostic.obj'),str(d/'preflight.obj')]
    exports=mb['defined_functions'];data_exports=['event_skipping','port_view_aspect','_nj_constant_attr_and_','_nj_constant_attr_or_']
    link[1:1]=['-Wl,/export:'+n for n in exports]+[v for n in data_exports if '/export:'+n+',DATA' not in link for v in ['-Xlinker','/export:'+n+',DATA']]
    results=[]
    for i,c in enumerate([cmd,pf_cmd,link]):
        run=subprocess.run(c,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=c,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6000]
    authored=dict(b['authored_source_sha256'],**mb['authored_source_sha256'])
    for path in [probe,pf,p/'native/jp_model_preflight.h',Path(__file__)]:authored[path.resolve().relative_to(r).as_posix()]=sha(path)
    dll=d/'jp_native_model_rz.dll'
    report=dict(status='complete_actual_model_RZ_diagnostic_runtime_linked_behavior_unqualified',exit_code=0,diagnostic_bytes=0,commands=results,compiler_sha256=sha(a.compiler),
        DLL=str(dll),DLL_sha256=sha(dll),authored_source_sha256=authored,existing_provider_objects_sha256={str(o):h for o,h in objects.items()},existing_diagnostic_objects_sha256={str(o):h for o,h in objects.items()},
        model_build_report_sha256=sha(m/'build_verification.json'),baseline_report_sha256=sha(bdir/'build_verification.json'),model_generated_source_sha256=sha(m/'external_model_native_rz.c'),
        model_diagnostic_object_sha256=sha(d/'model_diagnostic.obj'),preflight_object_sha256=sha(d/'preflight.obj'),complete_model_source_linked=True,diagnostic_frame_replay_services=True,
        one_queue_matrix_texture_and_render_state_owner=True,game_source_linked=True,artifact_publishable=False,complete_model_behavior_verified=False,full_frame_executed=False,playable=False,overall_completion_percent=10,goal_paused=False)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(report['status'],'zero diagnostics; complete producer and existing real native owners',flush=True)
if __name__=='__main__':main()
