"""Audit the real game graph with actual bitmap queue/memory upload providers."""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','baseline-audit','build','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();assert d.is_relative_to(r/'work/sonic-adventure') and not d.exists()
    prior=json.loads(a.baseline_audit.read_text());b=json.loads((a.build/'build_verification.json').read_text());objects={Path(n).resolve():h for n,h in prior['linked_objects_sha256'].items()}
    for n,h in objects.items():assert sha(n)==h,n
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    cmd=list(prior['command']);replacements={}
    for oldname,newname in [('textures_retained.obj','textures_upload.obj'),('frame_pacing.obj','frame_pacing.obj'),('game_present.obj','game_present.obj')]:
        old=[n for n in objects if n.name==oldname];assert len(old)==1;old=old[0];new=a.build.resolve()/newname
        indexes=[i for i,arg in enumerate(cmd) if arg.endswith('.obj') and (r/arg).resolve()==old];assert len(indexes)==1
        cmd[indexes[0]]=str(new);replacements[str(old)]=str(new);del objects[old];objects[new]=sha(new)
    additions=[a.build.resolve()/n for n in ['memory_upload.obj','bitmap_source.obj','bitmap_queue.obj','memory_list.obj']]
    for n in additions:objects[n]=sha(n)
    at=cmd.index('-Wl,kernel32.lib');cmd[at:at]=list(map(str,additions));d.mkdir();cmd[-1]=str(d/'unfinished_production.dll')
    run=subprocess.run(cmd,cwd=r,capture_output=True);log=d/'link.txt';log.write_bytes(run.stdout+run.stderr);text=log.read_text()
    missing=sorted(set(re.findall(r'undefined symbol: (\S+)',text)));resolved=sorted(set(prior['combined_remaining_imports'])-set(missing))
    assert run.returncode!=0 and not (d/'unfinished_production.dll').exists() and resolved==['bitmap_clear_queue','bitmap_flush_queue','texture_queue_count'],(resolved,text[:6000])
    assert len(missing)==36 and set(missing)<set(prior['combined_remaining_imports']) and len([line for line in text.splitlines() if 'error:' in line])==len(missing)+1,text[:6000]
    raw={v['name'] for v in coff_symbols(r/'work/sonic-adventure/jp-native-presentation-object-v3/present.obj') if v['section']==0 and v['value']==0}
    satisfied=sorted(set(prior['satisfied_presentation_imports'])|(set(resolved)&raw));assert all('diagnostic' not in str(n).lower() for n in objects)
    result=dict(prior,status='real_bitmap_queue_and_bounded_memory_loader_production_graph_audited',command=cmd,link_exit_code=run.returncode,log_sha256=sha(log),
        linked_objects_sha256={str(n):h for n,h in objects.items()},combined_remaining_imports=missing,combined_remaining_import_count=len(missing),
        satisfied_presentation_imports=satisfied,satisfied_presentation_import_count=len(satisfied),newly_resolved_services=resolved,replaced_native_objects=replacements,
        original_bitmap_queue_GCC_pointer_repair_retained=True,real_memory_upload_and_single_retained_texture_owner=True,bitmap_trace_providers_linked=False,
        test_fixture_or_dummy_services_linked=False,full_game_loop_executed=False,baseline_audit_sha256=sha(a.baseline_audit),verifier_sha256=sha(Path(__file__)),overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print('Real bitmap/memory production graph:',len(missing),'remaining services;',len(satisfied),'/79 original presentation imports supplied.')
if __name__=='__main__':main()
