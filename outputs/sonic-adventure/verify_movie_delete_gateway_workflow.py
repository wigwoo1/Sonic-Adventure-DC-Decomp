"""Qualification idempotence and immutable original-file/build preservation."""
import ast,hashlib,json,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];BASELINE='bcbe43e88135697a2092abbee3e9fef06207e9b8';G=r'C:\Program Files\Git\cmd\git.exe'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    qp=P/'analysis/movie_delete_gateway_qualification.json';before=digest(qp)
    subprocess.run([sys.executable,'-B',str(P/'qualify_movie_delete_gateway.py')],check=True);assert digest(qp)==before
    rp=P/'analysis/recovered_routines.json';sp=P/'project_status.json';saved=rp.read_bytes(),sp.read_bytes()
    subprocess.run([sys.executable,'-B',str(P/'register_movie_delete_gateway.py')],check=True);assert saved==(rp.read_bytes(),sp.read_bytes())
    scripts=[P/(name+'.py') for name in ('verify_movie_delete_gateway_direct','verify_movie_delete_gateway_async','verify_movie_delete_gateway_tasks','qualify_movie_delete_gateway','register_movie_delete_gateway','verify_movie_delete_gateway_workflow')]
    for p in scripts:ast.parse(p.read_text())
    q=json.loads(qp.read_text());assert (q['fresh_original_native_comparisons'],q['fresh_independent_checks'],q['total_evidence_original_native_comparisons'])==(586,3138,35108)
    for pin,h in q['sha256'].items():assert digest(pin)==h,pin
    def historical(path):return subprocess.check_output([G,'-C',str(ROOT/'publication/sonic-adventure'),'show',BASELINE+':'+path])
    baseline=json.loads(historical('PUBLICATION_MANIFEST.json'));assert len(baseline['files'])==14798
    mutable_names={'README.md','SOURCE_HANDOFF.txt','project_status.json','recovered_routines.json','case_dependency_map.json','main_dispatch_case_map_verification.json','project_completion_audit.json','recovery_coverage_audit.json','recovery_registry_evidence.json','unrecovered_function_candidates.tsv'}
    mutable=[r['path'] for r in baseline['files'] if Path(r['path']).name in mutable_names];assert len(mutable)==10
    for row in baseline['files']:
        if row['path'] not in mutable:assert digest(ROOT/row['path'])==row['sha256'],row['path']
    old=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];now=json.loads(rp.read_text())['routines'];assert len(old)==1545 and now[:1545]==old and len(now)==1546
    previous=json.loads(historical('outputs/sonic-adventure/project_status.json'));current=json.loads(sp.read_text());assert current['requirements']==previous['requirements']
    for k,v in previous['verified_progress'].items():assert current['verified_progress'][k]==(1546 if k=='recovered_buildable_routines' else v),k
    bm=P/'analysis/movie_delete_gateway_previous_build_manifest.json';build=json.loads(bm.read_text());assert len(build['files'])==2699
    for row in build['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    audit=json.loads((P/'analysis/recovery_coverage_audit.json').read_text());assert audit['registered_routines']==1546 and not audit['registry_evidence_errors'] and not audit['whole_source_hash_refresh_needed']
    report=dict(qualification_byte_reproducible=True,registration_byte_idempotent=True,python_scripts_parsed=6,prior_registry_rows_identical=1545,registry_entries=1546,unique_module_address_pairs=1543,new_routine_credit=1,reused_previously_unregistered_rows=92,executed_rows=89,statically_proven_nonexecuted_rows=3,executed_outcomes=15,statically_proven_impossible_outcomes=1,canonical_duplicate_credit=0,native_sources_changed=False,fresh_native_rebuild=False,compiled_source_count=233,previous_strict_build_source_pins_current=True,preserved_prior_selected_files=14788,preserved_baseline_commit=BASELINE,reviewed_mutable_paths=mutable,preserved_build_files=2699,additional_build_files=[],original_requirements_preserved=True,fresh_original_native_comparisons=586,fresh_independent_checks=3138,total_evidence_original_native_comparisons=35108,full_filesystem_qualified=False,physical_maple_dma_complete=False,next_original_dependency='0x8c649f1e',actual_parent_return='0x8c647a86',overall_completion_percent=10,estimate_method='weighted milestones, not measured code coverage',sha256={str(p):digest(p) for p in scripts+[qp,bm]},scope='Previously compiled deletegateway qualified in mapped helper/rawACK domains;89executed+3constantproof rows.233nativeC/DLL/all2699builds and14788earlier source/proofs unchanged exceptten reviewed catalog/status/docs. No freshnative rebuild. Two nextcommand8frontiers remainpending withoutinventedcompletion. Goalactive10%;fullgame/portsunfinished,researchDLLsnotplayable.')
    (P/'analysis/movie_delete_gateway_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('DELETE GATEWAY WORKFLOW PASS14788preserved source/proofs2699builds',flush=True)
if __name__=='__main__':main()
