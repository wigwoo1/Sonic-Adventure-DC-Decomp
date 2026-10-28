"""Reproducibility, original-file preservation and current proof pin checks."""
import ast,hashlib,json,re,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];BASELINE='3896239c57b89462f64515a9d6845ad2649a6fce';G=r'C:\Program Files\Git\cmd\git.exe'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    generated=[P/n for n in ('source/sa_movie_filesystem_command17.c','analysis/movie_filesystem_command17_translation.json','analysis/movie_filesystem_command17_returns.json','source/sa_movie_filesystem_command17_connected.c','source/movie_filesystem_command17_probe.c','build_movie_filesystem_command17.ps1','analysis/movie_filesystem_command17_connection.json','analysis/movie_filesystem_command17_audit.json','analysis/movie_filesystem_command17_zero_preflight_proof.json')]
    before={str(p):digest(p) for p in generated}
    for name in ('recover_movie_filesystem_command17.py','connect_movie_filesystem_command17.py','audit_movie_filesystem_command17.py','prove_movie_filesystem_command17_zero_preflight.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
    assert all(digest(p)==h for p,h in before.items())
    rp=P/'analysis/recovered_routines.json';sp=P/'project_status.json';saved=rp.read_bytes(),sp.read_bytes()
    subprocess.run([sys.executable,'-B',str(P/'register_movie_filesystem_command17.py')],check=True);assert saved==(rp.read_bytes(),sp.read_bytes())
    names=('recover_movie_filesystem_command17','connect_movie_filesystem_command17','movie_filesystem_command17_reference','filesystem_write_reference_cpu','write_watch_filesystem_command17','movie_filesystem_command17_task_fixtures','movie_filesystem_command17_session','movie_filesystem_command17_vectors','check_movie_filesystem_command17_unbounded','verify_movie_filesystem_command17_tasks','merge_movie_filesystem_command17_tasks','verify_movie_filesystem_command17_retained','verify_movie_filesystem_command17_guards','prove_movie_filesystem_command17_zero_preflight','audit_movie_filesystem_command17','audit_movie_linked_routines','qualify_movie_filesystem_command17','register_movie_filesystem_command17','verify_movie_filesystem_command17_workflow')
    scripts=[P/(n+'.py') for n in names+('assess_sarecomp_gameplay_fingerprints',)]
    for path in scripts:ast.parse(path.read_text())
    pins={}
    for name in ('translation','connection','audit','tasks_verification','retained_verification','guards_verification','zero_preflight_proof','qualification'):
        report=json.loads((P/f'analysis/movie_filesystem_command17_{name}.json').read_text())
        for path,h in report['sha256'].items():assert digest(path)==h,(name,path);pins[path]=h
    def historical(path):return subprocess.check_output([G,'-C',str(ROOT/'publication/sonic-adventure'),'show',BASELINE+':'+path])
    baseline=json.loads(historical('PUBLICATION_MANIFEST.json'));assert len(baseline['files'])==15330
    mutable_names={'README.md','SOURCE_HANDOFF.txt','project_status.json','recovered_routines.json','case_dependency_map.json','main_dispatch_case_map_verification.json','project_completion_audit.json','recovery_coverage_audit.json','recovery_registry_evidence.json','unrecovered_function_candidates.tsv'}
    mutable=[r['path'] for r in baseline['files'] if Path(r['path']).name in mutable_names];assert len(mutable)==10
    for row in baseline['files']:
        if row['path'] not in mutable:assert digest(ROOT/row['path'])==row['sha256'],row['path']
    registry_baseline=historical('outputs/sonic-adventure/analysis/recovered_routines.json')
    old_registry=json.loads(registry_baseline)['routines'];now=json.loads(rp.read_text())['routines'];assert len(old_registry)==1581 and now[:1581]==old_registry and len(now)==1582
    old_status=json.loads(historical('outputs/sonic-adventure/project_status.json'));status=json.loads(sp.read_text());assert status['requirements']==old_status['requirements']
    for key,value in old_status['verified_progress'].items():assert status['verified_progress'][key]==(1582 if key=='recovered_buildable_routines' else value),key
    builds=json.loads((P/'analysis/movie_filesystem_command17_previous_build_manifest.json').read_text());assert len(builds['files'])==2717
    for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    ps=P/'build_movie_filesystem_command17.ps1';sources=re.findall(r"'([^']+\.c)'",ps.read_text());assert len(sources)==len(set(sources))==243
    powershell=[ps,P/'ghidra/export_movie_filesystem_command17.ps1']
    for path in powershell:
        command="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(path)
        subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
    strict=P/'analysis/movie_filesystem_command17_unbounded.json';r=json.loads(strict.read_text());assert r['source_sha256']=={str(P/'source'/name):digest(P/'source'/name) for name in sources} and r['exit_code']==0
    coverage=json.loads((P/'analysis/recovery_coverage_audit.json').read_text());assert coverage['registered_routines']==1582 and not coverage['registry_evidence_errors'] and not coverage['whole_source_hash_refresh_needed']
    q=json.loads((P/'analysis/movie_filesystem_command17_qualification.json').read_text());assert (q['newly_registered_entries'],q['newly_qualified_rows'],q['qualified_conditional_outcomes'])==(1,123,28)
    files=scripts+powershell+[strict,P/'analysis/movie_filesystem_command17_qualification.json']
    report=dict(reproduced_generated_artifacts=9,registration_byte_idempotent=True,prior_registry_rows_identical=1581,registry_entries=1582,new_routine_credit=1,qualified_new_rows=123,qualified_new_outcomes=28,executed_new_outcomes=27,structurally_impossible_outcomes=1,newly_translated_unregistered_rows=0,canonical_adapter_rows=0,partial_unregistered_gateway_rows=0,canonical_duplicate_credit=0,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(powershell),pinned_inputs_current=len(pins),compiled_source_count=243,strict_unbounded_sources_checked=243,preserved_baseline_commit=BASELINE,preserved_prior_selected_files=15320,reviewed_mutable_paths=mutable,original_requirements_preserved=True,preserved_build_files=2717,additional_build_files=['work/sonic-adventure/build/movie_filesystem_command17.dll','work/sonic-adventure/build/movie_filesystem_command17.lib'],fresh_original_native_comparisons=q['fresh_original_native_comparisons'],fresh_independent_checks=q['fresh_independent_checks'],total_evidence_original_native_comparisons=q['total_evidence_original_native_comparisons'],physical_maple_dma_complete=False,full_filesystem_qualified=False,overall_completion_percent=10,estimate_method='weighted milestones, not measured code coverage',sha256={str(p):digest(p) for p in files},scope='Nineartifacts reproduce after1entry batch-delete qualification against frozen1581baseline.15320prior source/proofs preserved except10catalog/status/docpaths;2717oldbuilds byteidentical.243DLL/libadditional.123rows/27outcomes execute,1outcome proved impossible;batchmetadata/ACKwrites/missing/duplicate/errorprecedence/guards verified via originalrawtask. Requirementsunchanged;10%weightedmilestones,notcoverage;researchDLLnotplayable/physicalproof.')
    (P/'analysis/movie_filesystem_command17_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('COMMAND17 WORKFLOW PASS15320preserved source/proofs;',len(pins),'pins',flush=True)
if __name__=='__main__':main()
