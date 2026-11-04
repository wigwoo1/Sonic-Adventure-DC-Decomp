"""Replace exactly the frozen queue object in the real incomplete frame link."""
import argparse,hashlib,json,re,subprocess
from pathlib import Path

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','baseline-audit','component','output']:
        ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();component=a.component.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    prior=json.loads(a.baseline_audit.read_text());b=json.loads((component/'build_verification.json').read_text());proof=component/'intensity_verification.json'
    contract=json.loads(proof.read_text());assert contract['status']=='all_intensity_headers_and_culled_batch_contracts_verified' and contract['checks']==36879
    assert b['exit_code']==b['diagnostic_bytes']==0
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    objects={Path(n).resolve():h for n,h in prior['linked_objects_sha256'].items()}
    for obj,h in objects.items():assert sha(obj)==h,obj
    old=[o for o,h in objects.items() if h=='f4911492c710bfc59e2246ffef5202cf8daa313b03025a7fecd56fe2b59adc93'];assert len(old)==1
    old=old[0];new=component/'queue.obj';assert sha(new)==b['artifacts_sha256']['queue.obj'] and new not in objects
    cmd=list(prior['command']);assert sha(Path(cmd[0]))==b['compiler_sha256']
    locations=[i for i,v in enumerate(cmd) if v.endswith('.obj') and (r/v).resolve()==old];assert len(locations)==1
    cmd[locations[0]]=str(new);cmd[-1]=str(d/'production_unfinished.dll');d.mkdir(parents=True)
    run=subprocess.run(cmd,capture_output=True,cwd=r);log=d/'link.txt';log.write_bytes(run.stdout+run.stderr)
    remaining=sorted(set(re.findall(r'undefined symbol: (\S+)',log.read_text())))
    assert run.returncode!=0 and not (d/'production_unfinished.dll').exists() and remaining==prior['combined_remaining_imports']
    errors=[line for line in log.read_text().splitlines() if 'error:' in line];assert len(errors)==len(remaining)+1 and errors[-1].startswith('clang: error: linker command failed')
    del objects[old];objects[new]=sha(new)
    report=dict(prior);report.update(status='real_production_queue_intensity_replacement_verified',command=cmd,link_exit_code=run.returncode,log_sha256=sha(log),
        linked_objects_sha256={str(o):h for o,h in objects.items()},queue_replacement=dict(previous_object=str(old),previous_sha256='f4911492c710bfc59e2246ffef5202cf8daa313b03025a7fecd56fe2b59adc93',current_object=str(new),current_sha256=sha(new)),
        queue_intensity_contract_sha256=sha(proof),queue_build_report_sha256=sha(component/'build_verification.json'),queue_checks=contract['checks'],queue_original_header_calls=contract['original_header_calls'],queue_culled_path_cancellations=contract['culled_path_cancellations'],
        baseline_audit_sha256=sha(a.baseline_audit),verifier_sha256=sha(Path(__file__)),one_queue_owner=True,queue_safety_guard_and_discard_services_real=True,
        complete_sprite_producers_verified=False,full_frame_executed=False,overall_completion_percent=10,goal_paused=False,playable=False)
    (d/'verification.json').write_text(json.dumps(report,indent=2)+'\n');print('One real queue owner replaced; original 36 remaining services preserved; no diagnostic probes or extra owner linked.')

if __name__=='__main__':main()
