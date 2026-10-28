"""Reproducibility, original-file preservation and current proof pin checks."""
import ast,hashlib,json,re,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];BASELINE='d54a1fb449e9d5eaa918342bba5c7846d7802413';G=r'C:\Program Files\Git\cmd\git.exe'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    generated=[P/n for n in ('source/sa_movie_filesystem_command16.c','analysis/movie_filesystem_command16_translation.json','analysis/movie_filesystem_command16_returns.json','source/sa_movie_filesystem_command16_connected.c','source/movie_filesystem_command16_probe.c','build_movie_filesystem_command16.ps1','analysis/movie_filesystem_command16_connection.json','analysis/movie_filesystem_command16_audit.json')]
    before={str(p):digest(p) for p in generated}
    for name in ('recover_movie_filesystem_command16.py','connect_movie_filesystem_command16.py','audit_movie_filesystem_command16.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
    assert all(digest(p)==h for p,h in before.items())
    rp=P/'analysis/recovered_routines.json';sp=P/'project_status.json';saved=rp.read_bytes(),sp.read_bytes()
    subprocess.run([sys.executable,'-B',str(P/'register_movie_filesystem_command16.py')],check=True);assert saved==(rp.read_bytes(),sp.read_bytes())
    names=('recover_movie_filesystem_command16','connect_movie_filesystem_command16','movie_filesystem_command16_reference','filesystem_write_reference_cpu','write_watch_filesystem_command16','movie_filesystem_command16_task_fixtures','movie_filesystem_command16_session','movie_filesystem_command16_vectors','check_movie_filesystem_command16_unbounded','verify_movie_filesystem_command16_tasks','merge_movie_filesystem_command16_tasks','verify_movie_filesystem_command16_retained','verify_movie_filesystem_command16_frontier','audit_movie_filesystem_command16','audit_movie_linked_routines','qualify_movie_filesystem_command16','register_movie_filesystem_command16','verify_movie_filesystem_command16_workflow')
    scripts=[P/(n+'.py') for n in names+('assess_sarecomp_gameplay_fingerprints',)]
    for path in scripts:ast.parse(path.read_text())
    pins={}
    for name in ('translation','connection','audit','tasks_verification','retained_verification','frontier_verification','qualification'):
        report=json.loads((P/f'analysis/movie_filesystem_command16_{name}.json').read_text())
        for path,h in report['sha256'].items():assert digest(path)==h,(name,path);pins[path]=h
    def historical(path):return subprocess.check_output([G,'-C',str(ROOT/'publication/sonic-adventure'),'show',BASELINE+':'+path])
    baseline=json.loads(historical('PUBLICATION_MANIFEST.json'));assert len(baseline['files'])==15282
    mutable_names={'README.md','SOURCE_HANDOFF.txt','project_status.json','recovered_routines.json','case_dependency_map.json','main_dispatch_case_map_verification.json','project_completion_audit.json','recovery_coverage_audit.json','recovery_registry_evidence.json','unrecovered_function_candidates.tsv'}
    mutable=[r['path'] for r in baseline['files'] if Path(r['path']).name in mutable_names];assert len(mutable)==10
    for row in baseline['files']:
        if row['path'] not in mutable:assert digest(ROOT/row['path'])==row['sha256'],row['path']
    registry_baseline=historical('outputs/sonic-adventure/analysis/recovered_routines.json')
    old_registry=json.loads(registry_baseline)['routines'];now=json.loads(rp.read_text())['routines'];assert len(old_registry)==1580 and now[:1580]==old_registry and len(now)==1581
    reuse_path=P/'analysis/sarecomp_gameplay_compatibility_probe.json';reuse=json.loads(reuse_path.read_text())
    registry_snapshot_hash=next(r['sha256'] for r in baseline['files'] if r['path']=='outputs/sonic-adventure/analysis/recovered_routines.json')
    assert reuse['registry_entries_at_assessment']==1580 and reuse['registry_baseline_commit']==BASELINE and reuse['registry_snapshot_sha256']==registry_snapshot_hash
    assert reuse['new_routine_credit']==0 and not reuse['runtime_code_imported']
    for path,h in reuse['sha256'].items():assert digest(path)==h; pins[path]=h
    candidates={r['name']:r for r in reuse['results']}
    for name,address in (('cross','0x8c027360'),('length','0x8c63a75c')):
        assert candidates[name]['exact_matches']==[dict(usa_address=address,registered=[])]
    assert candidates['normalize']['exact_matches'][0]['usa_address']=='0x8c63a94c' and candidates['normalize']['exact_matches'][0]['registered']
    old_status=json.loads(historical('outputs/sonic-adventure/project_status.json'));status=json.loads(sp.read_text());assert status['requirements']==old_status['requirements']
    for key,value in old_status['verified_progress'].items():assert status['verified_progress'][key]==(1581 if key=='recovered_buildable_routines' else value),key
    builds=json.loads((P/'analysis/movie_filesystem_command16_previous_build_manifest.json').read_text());assert len(builds['files'])==2715
    for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    ps=P/'build_movie_filesystem_command16.ps1';sources=re.findall(r"'([^']+\.c)'",ps.read_text());assert len(sources)==len(set(sources))==242
    powershell=[ps,P/'ghidra/export_movie_filesystem_command16.ps1']
    for path in powershell:
        command="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(path)
        subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
    strict=P/'analysis/movie_filesystem_command16_unbounded.json';r=json.loads(strict.read_text());assert r['source_sha256']=={str(P/'source'/name):digest(P/'source'/name) for name in sources} and r['exit_code']==0
    coverage=json.loads((P/'analysis/recovery_coverage_audit.json').read_text());assert coverage['registered_routines']==1581 and not coverage['registry_evidence_errors'] and not coverage['whole_source_hash_refresh_needed']
    q=json.loads((P/'analysis/movie_filesystem_command16_qualification.json').read_text());assert (q['newly_registered_entries'],q['newly_qualified_rows'],q['qualified_conditional_outcomes'])==(1,54,4)
    files=scripts+powershell+[strict,P/'analysis/movie_filesystem_command16_qualification.json',reuse_path,P/'SA_RECOMP_GAMEPLAY_REUSE_FOLLOWUP.txt']
    report=dict(reproduced_generated_artifacts=8,registration_byte_idempotent=True,prior_registry_rows_identical=1580,registry_entries=1581,new_routine_credit=1,qualified_new_rows=54,qualified_new_outcomes=4,newly_translated_unregistered_rows=0,canonical_adapter_rows=0,partial_unregistered_gateway_rows=0,canonical_duplicate_credit=0,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(powershell),pinned_inputs_current=len(pins),compiled_source_count=242,strict_unbounded_sources_checked=242,preserved_baseline_commit=BASELINE,preserved_prior_selected_files=15272,reviewed_mutable_paths=mutable,original_requirements_preserved=True,preserved_build_files=2715,additional_build_files=['work/sonic-adventure/build/movie_filesystem_command16.dll','work/sonic-adventure/build/movie_filesystem_command16.lib'],fresh_original_native_comparisons=q['fresh_original_native_comparisons'],fresh_independent_checks=q['fresh_independent_checks'],total_evidence_original_native_comparisons=q['total_evidence_original_native_comparisons'],physical_maple_dma_complete=False,full_filesystem_qualified=False,overall_completion_percent=10,estimate_method='weighted milestones, not measured code coverage',sha256={str(p):digest(p) for p in files},scope='Eightartifacts reproduce after1entry whole-device-write qualification against frozen1580baseline.15272prior source/proofs preserved except10catalog/status/docpaths;2715oldbuilds byteidentical.242DLL/libadditional.54qualifiedrows/4outcomes all execute;normal512-blockACKimage anderrorprefixes verified via originalrawtask. Requirementsunchanged;10%weightedmilestones,notcoverage;researchDLLnotplayable/physicalproof.')
    (P/'analysis/movie_filesystem_command16_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('COMMAND16 WORKFLOW PASS15272preserved source/proofs;',len(pins),'pins',flush=True)
if __name__=='__main__':main()
