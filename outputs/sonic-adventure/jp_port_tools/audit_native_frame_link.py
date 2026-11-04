"""Replace real queue/texture owners and add owned frames to production.
No diagnostic bodies or successful placeholder render/flip functions link.
"""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','baseline-audit','runtime','compiler','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();s=a.runtime.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    prior=json.loads(a.baseline_audit.read_text());b=json.loads((s/'build_verification.json').read_text());qpath=s/'final_frame_verification.json';q=json.loads(qpath.read_text());mpath=s/'qualified_model_retention.json';model=json.loads(mpath.read_text())
    assert b['exit_code']==b['diagnostic_bytes']==0 and b['compiler_sha256']==sha(a.compiler)
    assert q['checks']==8572 and model['original_model_checks']==200669 and model['checks']==7
    for path,report in [(r/'outputs/sonic-adventure/jp_port_tools/verify_native_frames.py',q),(r/'outputs/sonic-adventure/jp_port_tools/verify_native_frame_model.py',model)]:assert sha(path)==report['verifier_sha256']
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    objects={Path(n).resolve():h for n,h in prior['linked_objects_sha256'].items()}
    for n,h in objects.items():assert sha(n)==h,n
    cmd=list(prior['command']);replacements={}
    for old_name,new_name in [('queue.obj','queue_frames.obj'),('jp_native_textures.obj','textures_retained.obj')]:
        old=[n for n in objects if n.name==old_name];assert len(old)==1;old=old[0]
        new=s/new_name;assert sha(new)==b['production_objects_sha256'][new_name]
        positions=[i for i,v in enumerate(cmd) if v.endswith('.obj') and (r/v).resolve()==old];assert len(positions)==1
        cmd[positions[0]]=str(new);replacements[str(old)]=dict(previous_sha256=objects.pop(old),current_object=str(new),current_sha256=sha(new));objects[new]=sha(new)
    extra=s/'frames.obj';assert sha(extra)==b['production_objects_sha256']['frames.obj'];objects[extra]=sha(extra)
    at=cmd.index('-Wl,kernel32.lib');cmd[at:at]=[str(extra)];cmd[-1]=str(d/'production_unfinished.dll')
    d.mkdir(parents=True);run=subprocess.run(cmd,cwd=r,capture_output=True);log=d/'link.txt';log.write_bytes(run.stdout+run.stderr)
    text=log.read_text();remaining=sorted(set(re.findall(r'undefined symbol: (\S+)',text)))
    assert run.returncode!=0 and not (d/'production_unfinished.dll').exists() and remaining==prior['combined_remaining_imports']
    errors=[line for line in text.splitlines() if 'error:' in line];assert len(errors)==len(remaining)+1 and errors[-1].startswith('clang: error: linker command failed')
    report=dict(prior);report.update(status='real_owned_frame_and_texture_owner_replacement_link_audited',command=cmd,link_exit_code=run.returncode,log_sha256=sha(log),
        linked_objects_sha256={str(n):h for n,h in objects.items()},frame_owner_replacements=replacements,frame_runtime_report_sha256=sha(s/'build_verification.json'),frame_contract_sha256=sha(qpath),model_retention_contract_sha256=sha(mpath),
        baseline_audit_sha256=sha(a.baseline_audit),verifier_sha256=sha(Path(__file__)),owned_submission_verified=True,render_flip_implemented=False,
        full_frame_executed=False,rendered_pixels_verified=False,newly_resolved_services=[],test_fixture_or_dummy_services_linked=False)
    (d/'verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Real retained owners/frame storage linked;',len(remaining),'frame services still unresolved; no diagnostic objects',flush=True)
if __name__=='__main__':main()
