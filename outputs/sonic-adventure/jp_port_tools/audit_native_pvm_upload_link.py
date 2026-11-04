"""Link real PVM providers into the existing production graph, no fixtures."""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def definitions(p):return {v['name'] for v in coff_symbols(p) if v['section']>0}
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','baseline-audit','build','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();assert d.is_relative_to(r/'work/sonic-adventure') and not d.exists()
    prior=json.loads(a.baseline_audit.read_text());b=json.loads((a.build/'build_verification.json').read_text());objects={Path(n).resolve():h for n,h in prior['linked_objects_sha256'].items()}
    for n,h in objects.items():assert sha(n)==h,n
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    cmd=list(prior['command']);old=[n for n in objects if n.name=='memory_upload.obj'];assert len(old)==1;old=old[0];new=a.build.resolve()/'memory_pvm.obj'
    positions=[i for i,arg in enumerate(cmd) if arg.endswith('.obj') and (r/arg).resolve()==old];assert len(positions)==1
    cmd[positions[0]]=str(new);del objects[old];objects[new]=sha(new);helper=a.build.resolve()/'pvm_helpers.obj';objects[helper]=sha(helper)
    at=cmd.index('-Wl,kernel32.lib');cmd.insert(at,str(helper));d.mkdir();cmd[-1]=str(d/'unfinished_production.dll')
    run=subprocess.run(cmd,cwd=r,capture_output=True);log=d/'link.txt';log.write_bytes(run.stdout+run.stderr);text=log.read_text()
    missing=sorted(set(re.findall(r'undefined symbol: (\S+)',text)))
    assert run.returncode!=0 and not (d/'unfinished_production.dll').exists() and missing==prior['combined_remaining_imports'] and len(missing)==36,text[:6000]
    assert len([line for line in text.splitlines() if 'error:' in line])==len(missing)+1 and all('diagnostic' not in str(n).lower() for n in objects),text[:6000]
    previous=definitions(old);provided=definitions(new)|definitions(helper);assert previous<=provided
    added=sorted(provided-previous);expected=['njLoadTexturePvmMemory','njCalcPvmStatusSize','njSetTexListToPvmM','njSearchEntryCount','njSetPvmTextureList','njMemOpen','njMemRead','njMemFseek']
    assert set(expected)<=set(added)
    result=dict(prior,status='bounded_PVM_memory_provider_and_original_metadata_helpers_production_link_audited',command=cmd,link_exit_code=run.returncode,log_sha256=sha(log),
        linked_objects_sha256={str(n):h for n,h in objects.items()},combined_remaining_imports=missing,combined_remaining_import_count=36,
        newly_resolved_services=[],newly_provided_SDK_PVM_services=expected,new_defined_symbols=added,all_previous_memory_services_preserved=True,
        replaced_memory_object={str(old):str(new)},new_PVM_helper_object=str(helper),test_fixture_or_dummy_services_linked=False,real_PVM_archive_service_available=True,
        existing_boot_graph_uses_PVM_service_yet=False,full_game_loop_executed=False,baseline_audit_sha256=sha(a.baseline_audit),verifier_sha256=sha(Path(__file__)),overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print('Real PVM providers linked without new unresolved imports;',len(missing),'existing game services remain.')
if __name__=='__main__':main()
