"""Relink the actual unfinished production graph with the direct GPU provider."""
import argparse,hashlib,json,re,subprocess
from pathlib import Path

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','baseline-audit','build','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();assert d.is_relative_to(r/'work/sonic-adventure') and not d.exists()
    prior=json.loads(a.baseline_audit.read_text());b=json.loads((a.build/'build_verification.json').read_text());assert not prior['test_fixture_or_dummy_services_linked']
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    objects={Path(n).resolve():h for n,h in prior['linked_objects_sha256'].items()}
    for path,h in objects.items():assert sha(path)==h,path
    old=[n for n in objects if n.name=='gpu_window.obj'];assert len(old)==1;old=old[0];new=a.build.resolve()/'gpu_window.obj'
    assert sha(new)==b['production_object_sha256'];cmd=list(prior['command']);replaced=0
    for i,arg in enumerate(cmd):
        if arg.endswith('.obj') and (r/arg).resolve()==old:cmd[i]=str(new);replaced+=1
    assert replaced==1;del objects[old];objects[new]=sha(new);d.mkdir();cmd[-1]=str(d/'unfinished_production.dll')
    run=subprocess.run(cmd,cwd=r,capture_output=True);log=d/'link.txt';log.write_bytes(run.stdout+run.stderr);text=log.read_text()
    remaining=sorted(set(re.findall(r'undefined symbol: (\S+)',text)))
    assert run.returncode!=0 and not (d/'unfinished_production.dll').exists() and remaining==prior['combined_remaining_imports'],text[:6000]
    assert len([v for v in text.splitlines() if 'error:' in v])==len(remaining)+1
    result=dict(prior,status='actual_direct_backbuffer_production_link_audited',command=cmd,link_exit_code=run.returncode,log_sha256=sha(log),
        linked_objects_sha256={str(n):h for n,h in objects.items()},baseline_audit_sha256=sha(a.baseline_audit),verifier_sha256=sha(Path(__file__)),
        direct_GPU_output_provider_linked=True,newly_resolved_services=[],combined_remaining_imports=remaining,combined_remaining_import_count=len(remaining),
        Kamui_render_flip_statistics_implemented=False,full_frame_executed=False,test_fixture_or_dummy_services_linked=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print('Actual direct provider audited;',len(remaining),'services remain.')
if __name__=='__main__':main()
