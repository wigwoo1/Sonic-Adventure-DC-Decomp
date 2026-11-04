"""Link real file/PRS/game texture providers into the unfinished production graph."""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def definitions(p):return {v['name'] for v in coff_symbols(p) if v['section']>0}
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','baseline-audit','build','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();build=a.build.resolve();assert d.is_relative_to(r/'work/sonic-adventure') and not d.exists()
    prior=json.loads(a.baseline_audit.read_text());b=json.loads((build/'build_verification.json').read_text());objects={Path(n).resolve():h for n,h in prior['linked_objects_sha256'].items()}
    for n,h in objects.items():assert sha(n)==h,n
    for n,h in dict(b['authored_source_sha256'],**b['vendored_MIT_source_sha256']).items():assert sha(r/n)==h,n
    command=list(prior['command']);old=[n for n in objects if n.name=='textures_upload.obj'];assert len(old)==1;old=old[0];new=build/'textures_move.obj'
    positions=[i for i,arg in enumerate(command) if arg.endswith('.obj') and (r/arg).resolve()==old];assert len(positions)==1
    command[positions[0]]=str(new);del objects[old];objects[new]=sha(new)
    for name in ['assets.obj','prskit.obj','game_texture.obj']:
        path=build/name;assert sha(path)==b['production_objects_sha256'][name];objects[path]=sha(path);command.insert(command.index('-Wl,kernel32.lib'),str(path))
    assert definitions(old)<=definitions(new)
    provided=set().union(*(definitions(build/name) for name in ['textures_move.obj','assets.obj','prskit.obj','game_texture.obj']))
    required={'jp_texture_move_names','jp_texture_validate_move','jp_assets_read_prs','jp_assets_load_game_file','jp_assets_commit_game','texture_load_pvm','texture_load_prs_file','prskit_decompress'};assert required<=provided
    assert all(not any(s in str(n).lower() for s in ['diagnostic','probe.obj','fixture']) for n in objects)
    d.mkdir();command[-1]=str(d/'unfinished_production.dll');run=subprocess.run(command,cwd=r,capture_output=True);log=d/'link.txt';log.write_bytes(run.stdout+run.stderr);text=log.read_text();missing=sorted(set(re.findall(r'undefined symbol: (\S+)',text)))
    assert run.returncode!=0 and not (d/'unfinished_production.dll').exists() and missing==prior['combined_remaining_imports'],text[:7000]
    assert len([line for line in text.splitlines() if 'error:' in line])==len(missing)+1,text[:7000]
    result=dict(prior,status='native_file_PRS_original_texture_wrapper_and_binding_moves_production_link_audited',command=command,link_exit_code=run.returncode,log_sha256=sha(log),linked_objects_sha256={str(n):h for n,h in objects.items()},combined_remaining_imports=missing,combined_remaining_import_count=len(missing),newly_resolved_services=[],newly_provided_asset_services=sorted(required),all_previous_texture_services_preserved=True,replaced_texture_owner={str(old):str(new)},test_fixture_or_dummy_services_linked=False,real_native_file_PRS_provider_available=True,real_original_texture_wrapper_available=True,existing_boot_graph_calls_asset_provider_yet=False,full_game_loop_executed=False,baseline_audit_sha256=sha(a.baseline_audit),asset_build_sha256=sha(build/'build_verification.json'),verifier_sha256=sha(Path(__file__)),overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print('Real file/PRS/game texture providers linked without new unresolved imports;',len(missing),'existing game services remain.')
if __name__=='__main__':main()
