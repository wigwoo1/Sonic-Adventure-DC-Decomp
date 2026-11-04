"""Expand actual production graph with frame pacing; never link trace fixtures."""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','baseline-audit','build','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();assert d.is_relative_to(r/'work/sonic-adventure') and not d.exists()
    prior=json.loads(a.baseline_audit.read_text());b=json.loads((a.build/'build_verification.json').read_text())
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    objects={Path(n).resolve():h for n,h in prior['linked_objects_sha256'].items()}
    for n,h in objects.items():assert sha(n)==h,n
    cmd=list(prior['command']);old=[n for n in objects if n.name=='present.obj'];assert len(old)==1;old=old[0];new=a.build.resolve()/'present.obj';changed=0
    for i,arg in enumerate(cmd):
        if arg.endswith('.obj') and (r/arg).resolve()==old:cmd[i]=str(new);changed+=1
    assert changed==1;del objects[old];objects[new]=sha(new)
    additions=[a.build.resolve()/n for n in ['frame_pacing.obj','wait.obj','windows_idle.obj']]
    for n in additions:objects[n]=sha(n)
    at=cmd.index('-Wl,kernel32.lib');cmd[at:at]=list(map(str,additions));d.mkdir();cmd[-1]=str(d/'unfinished_production.dll')
    run=subprocess.run(cmd,cwd=r,capture_output=True);log=d/'link.txt';log.write_bytes(run.stdout+run.stderr);text=log.read_text()
    missing=sorted(set(re.findall(r'undefined symbol: (\S+)',text)));resolved=sorted(set(prior['combined_remaining_imports'])-set(missing))
    assert run.returncode!=0 and not (d/'unfinished_production.dll').exists() and resolved==['frame_countdown','frame_interval','frame_step','frame_step_count'],(resolved,text[:5000])
    assert len([v for v in text.splitlines() if 'error:' in v])==len(missing)+1
    introduced=sorted(set(missing)-set(prior['combined_remaining_imports']));assert set(introduced)<=set(b['frame_pacing_undefined_symbols'])
    assert all('diagnostic' not in str(n).lower() for n in objects)
    result=dict(prior,status='real_native_frame_wait_expanded_production_graph_audited',command=cmd,link_exit_code=run.returncode,log_sha256=sha(log),
        linked_objects_sha256={str(n):h for n,h in objects.items()},combined_remaining_imports=missing,combined_remaining_import_count=len(missing),
        newly_resolved_services=resolved,newly_exposed_original_frame_dependencies=introduced,prior_graph_remaining_services=prior['combined_remaining_import_count'],
        satisfied_presentation_imports=sorted(set(prior['satisfied_presentation_imports'])|set(resolved)),satisfied_presentation_import_count=prior['satisfied_presentation_import_count']+len(resolved),
        native_wait_service_and_original_frame_object_linked=True,trace_fixture_services_linked=False,test_fixture_or_dummy_services_linked=False,
        Kamui_render_flip_statistics_implemented=False,full_frame_executed=False,baseline_audit_sha256=sha(a.baseline_audit),verifier_sha256=sha(Path(__file__)),overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print('Actual frame wait graph:',len(missing),'remaining services; four original frame globals resolved;',len(introduced),'dependencies exposed.')
if __name__=='__main__':main()
