"""Replace the real model object in the retained production link, never probes.
Require the same unresolved frame services; numerical migration is not a new
service count or a successful frame link.
"""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','baseline-audit','model','runtime','compiler','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();m=a.model.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    prior=json.loads(a.baseline_audit.read_text());b=json.loads((m/'build_verification.json').read_text());qpath=a.runtime/'qualified_contract_verification.json';q=json.loads(qpath.read_text())
    assert b['exit_code']==b['diagnostic_bytes']==0 and sha(a.compiler)==b['compiler_sha256']
    assert q['status']=='sampled_complete_actual_model_RZ_contracts_verified' and q['checks']==200669 and not q['original_dependency_hooks_used']
    assert q['verifier_sha256']==sha(r/'outputs/sonic-adventure/jp_port_tools/verify_native_model_rz.py')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    old_objects={Path(n).resolve():h for n,h in prior['linked_objects_sha256'].items()}
    for n,h in old_objects.items():assert sha(n)==h,n
    old=[n for n in old_objects if n.name=='model.obj'];assert len(old)==1
    new=m/'model.obj';assert sha(new)==b['production_objects_sha256']['model.obj']
    cmd=list(prior['command']);matches=[i for i,v in enumerate(cmd) if v.endswith('.obj') and (r/v).resolve()==old[0]];assert len(matches)==1
    cmd[matches[0]]=str(new);cmd[-1]=str(d/'production_unfinished.dll')
    d.mkdir(parents=True);run=subprocess.run(cmd,cwd=r,capture_output=True);log=d/'link.txt';log.write_bytes(run.stdout+run.stderr)
    text=log.read_text();remaining=sorted(set(re.findall(r'undefined symbol: (\S+)',text)))
    assert run.returncode!=0 and not (d/'production_unfinished.dll').exists()
    assert remaining==prior['combined_remaining_imports']
    errors=[line for line in text.splitlines() if 'error:' in line]
    assert len(errors)==len(remaining)+1 and errors[-1].startswith('clang: error: linker command failed')
    objects=dict(old_objects);del objects[old[0]];objects[new]=sha(new)
    report=dict(prior);report.update(status='real_complete_model_RZ_replacement_link_audited',command=cmd,link_exit_code=run.returncode,log_sha256=sha(log),
        linked_objects_sha256={str(n):h for n,h in objects.items()},model_replacement={'previous_object':str(old[0]),'previous_sha256':old_objects[old[0]],'current_object':str(new),'current_sha256':sha(new)},
        model_build_report_sha256=sha(m/'build_verification.json'),model_contract_report_sha256=sha(qpath),baseline_audit_sha256=sha(a.baseline_audit),verifier_sha256=sha(Path(__file__)),
        numerical_mode='Ninja_camera_sprites_primitives_and_complete_model_RZ_DN1_value_services',renderer_inline_arithmetic_migrated=True,
        full_frame_executed=False,complete_retained_replay_executed=False,model_checks=200669,original_model_entry_calls=3485,newly_resolved_services=[],test_fixture_or_dummy_services_linked=False)
    (d/'verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Complete actual model numerical migration audited;',len(remaining),'real frame services remain; no diagnostic objects linked',flush=True)
if __name__=='__main__':main()
