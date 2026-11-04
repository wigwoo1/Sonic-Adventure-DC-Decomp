"""Audit real background providers in both render and expanded original boot graphs."""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','baseline-audit','boot-frontier','build','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();build=a.build.resolve();assert d.is_relative_to(r/'work/sonic-adventure') and not d.exists()
    prior=read(a.baseline_audit);boot=read(a.boot_frontier);b=read(build/'build_verification.json');objects={Path(n).resolve():h for n,h in prior['linked_objects_sha256'].items()};command=list(prior['command']);replaced={}
    for n,h in objects.items():assert sha(n)==h,n
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for old_name,new_name in [('frames.obj','frames_background.obj'),('gpu_window.obj','gpu_background.obj')]:
        found=[p for p in objects if p.name==old_name];assert len(found)==1;old=found[0];new=build/new_name;assert sha(new)==b['production_objects_sha256'][new_name]
        positions=[i for i,v in enumerate(command) if v.endswith('.obj') and (r/v).resolve()==old];assert len(positions)==1;command[positions[0]]=str(new);replaced[str(old)]=str(new);del objects[old];objects[new]=sha(new)
    for name in ['background_sdk.obj','background_decode.obj']:
        path=build/name;assert sha(path)==b['production_objects_sha256'][name];objects[path]=sha(path);command.insert(command.index('-Wl,kernel32.lib'),str(path))
    assert all(not any(s in str(n).lower() for s in ['diagnostic','probe.obj','fixture']) for n in objects)
    d.mkdir();results={}
    boot_obj=next(Path(n) for n in boot['production_link_command'] if n.endswith('original_logo_boot.obj'));assert sha(boot_obj)==boot['object_sha256']
    for name,extra,expected in [('render',[],prior['combined_remaining_imports']),('expanded_boot',[str(boot_obj)],sorted(set(boot['remaining_services'])-{'njSetBackColor'}))]:
        cmd=list(command);at=cmd.index('-Wl,kernel32.lib');cmd[at:at]=extra;cmd[-1]=str(d/(name+'_unfinished.dll'));run=subprocess.run(cmd,cwd=r,capture_output=True);log=d/(name+'_link.txt');log.write_bytes(run.stdout+run.stderr);text=log.read_text();missing=sorted(set(re.findall(r'undefined symbol: (\S+)',text)))
        assert run.returncode!=0 and not Path(cmd[-1]).exists() and missing==expected,text[:7000]
        assert len([line for line in text.splitlines() if 'error:' in line])==len(missing)+1,text[:7000]
        results[name]=dict(command=cmd,exit_code=run.returncode,log_sha256=sha(log),remaining_services=missing,remaining_count=len(missing))
    report=dict(status='real_background_frame_and_GPU_providers_production_and_expanded_boot_link_audited',linked_objects_sha256={str(n):h for n,h in objects.items()},command=results['render']['command'],combined_remaining_imports=results['render']['remaining_services'],combined_remaining_import_count=results['render']['remaining_count'],expanded_boot_remaining_import_count=results['expanded_boot']['remaining_count'],expanded_boot_services_now_resolved=['njSetBackColor'],graphs=results,replaced_objects=replaced,new_background_services=['njSetBackColor','kmSetBackGroundPlane','kmiInitializeBackGroundPlane','jp_background_prepend'],satisfied_presentation_import_count=prior['satisfied_presentation_import_count'],test_fixture_or_dummy_services_linked=False,original_boot_object_sha256=sha(boot_obj),baseline_audit_sha256=sha(a.baseline_audit),boot_frontier_sha256=sha(a.boot_frontier),build_report_sha256=sha(build/'build_verification.json'),verifier_sha256=sha(Path(__file__)),full_game_boot_executed=False,playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Background providers audited:',report['combined_remaining_import_count'],'render services and',report['expanded_boot_remaining_import_count'],'expanded boot services remain; njSetBackColor resolved.')
if __name__=='__main__':main()
