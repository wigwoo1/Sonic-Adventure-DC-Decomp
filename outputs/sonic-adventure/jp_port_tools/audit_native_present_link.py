"""Link real native render/flip/statistics with the expanded game graph."""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','baseline-audit','build','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();assert d.is_relative_to(r/'work/sonic-adventure') and not d.exists()
    prior=json.loads(a.baseline_audit.read_text());b=json.loads((a.build/'build_verification.json').read_text())
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    objects={Path(n).resolve():h for n,h in prior['linked_objects_sha256'].items()}
    for n,h in objects.items():assert sha(n)==h,n
    cmd=list(prior['command']);replacements={}
    for oldname,newname in [('present.obj','game_present.obj'),('frame_pacing.obj','frame_pacing.obj'),('wait.obj','wait_service.obj')]:
        old=[n for n in objects if n.name==oldname];assert len(old)==1;old=old[0];new=a.build.resolve()/newname;changed=0
        for i,arg in enumerate(cmd):
            if arg.endswith('.obj') and (r/arg).resolve()==old:cmd[i]=str(new);changed+=1
        assert changed==1;replacements[str(old)]=str(new);del objects[old];objects[new]=sha(new)
    additions=[a.build.resolve()/n for n in ['kamui_present.obj','present_sdk.obj']]
    for n in additions:objects[n]=sha(n)
    at=cmd.index('-Wl,kernel32.lib');cmd[at:at]=list(map(str,additions));d.mkdir();cmd[-1]=str(d/'unfinished_production.dll')
    run=subprocess.run(cmd,cwd=r,capture_output=True);log=d/'link.txt';log.write_bytes(run.stdout+run.stderr);text=log.read_text()
    missing=sorted(set(re.findall(r'undefined symbol: (\S+)',text)));resolved=sorted(set(prior['combined_remaining_imports'])-set(missing))
    expected=['kmFlipFrameBuffer','kmRender','kmResetPointer','kmiIntStatisticData']
    assert run.returncode!=0 and not (d/'unfinished_production.dll').exists() and resolved==expected and set(missing)<=set(prior['combined_remaining_imports']),(resolved,text[:6000])
    assert len([v for v in text.splitlines() if 'error:' in v])==len(missing)+1 and len(missing)==39
    raw={v['name'] for v in coff_symbols(r/'work/sonic-adventure/jp-native-presentation-object-v3/present.obj') if v['section']==0 and v['value']==0};assert len(raw)==79
    satisfied=sorted(set(prior['satisfied_presentation_imports'])|(set(resolved)&raw));assert len(satisfied)==63
    assert all('diagnostic' not in str(n).lower() for n in objects)
    result=dict(prior,status='real_Kamui_native_render_flip_and_measured_statistics_production_link_audited',command=cmd,link_exit_code=run.returncode,log_sha256=sha(log),
        linked_objects_sha256={str(n):h for n,h in objects.items()},combined_remaining_imports=missing,combined_remaining_import_count=len(missing),
        satisfied_presentation_imports=satisfied,satisfied_presentation_import_count=len(satisfied),newly_resolved_services=resolved,replaced_native_objects=replacements,
        real_single_typed_device_owner=True,Kamui_render_flip_statistics_implemented=True,render_flip_implemented=True,measured_native_statistics=True,
        trace_fixture_services_linked=False,test_fixture_or_dummy_services_linked=False,full_frame_executed=False,
        baseline_audit_sha256=sha(a.baseline_audit),verifier_sha256=sha(Path(__file__)),overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print('Real native render/flip audit:',len(missing),'remaining services;',len(satisfied),'/79 original presentation imports supplied.')
if __name__=='__main__':main()
