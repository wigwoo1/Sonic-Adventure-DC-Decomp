"""Reproducibility, original-file preservation and current proof pin checks."""
import ast,hashlib,json,re,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];BASELINE='72b4b61ac53a8d0321efc93458991c03e872e45d';G=r'C:\Program Files\Git\cmd\git.exe'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    generated=[P/n for n in ('source/sa_movie_filesystem_command8.c','analysis/movie_filesystem_command8_translation.json','analysis/movie_filesystem_command8_returns.json','source/sa_movie_filesystem_command8_connected.c','source/movie_filesystem_command8_probe.c','build_movie_filesystem_command8.ps1','analysis/movie_filesystem_command8_connection.json','analysis/movie_filesystem_command8_audit.json')]
    before={str(p):digest(p) for p in generated}
    for name in ('recover_movie_filesystem_command8.py','connect_movie_filesystem_command8.py','audit_movie_filesystem_command8.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
    assert all(digest(p)==h for p,h in before.items())
    rp=P/'analysis/recovered_routines.json';sp=P/'project_status.json';saved=rp.read_bytes(),sp.read_bytes()
    subprocess.run([sys.executable,'-B',str(P/'register_movie_filesystem_command8.py')],check=True);assert saved==(rp.read_bytes(),sp.read_bytes())
    names=('recover_movie_filesystem_command8','connect_movie_filesystem_command8','movie_filesystem_command8_reference','filesystem_write_reference_cpu','write_watch_filesystem_command8','movie_filesystem_command8_task_fixtures','movie_filesystem_command8_session','movie_filesystem_command8_vectors','check_movie_filesystem_command8_unbounded','verify_movie_filesystem_command8_direct','verify_movie_filesystem_command8_tasks','verify_movie_filesystem_command8_retained','verify_movie_filesystem_command8_frontier','audit_movie_filesystem_command8','qualify_movie_filesystem_command8','register_movie_filesystem_command8','verify_movie_filesystem_command8_workflow')
    scripts=[P/(n+'.py') for n in names]
    for path in scripts:ast.parse(path.read_text())
    pins={}
    for name in ('translation','connection','audit','direct_verification','tasks_verification','retained_verification','frontier_verification','qualification'):
        report=json.loads((P/f'analysis/movie_filesystem_command8_{name}.json').read_text())
        for path,h in report['sha256'].items():assert digest(path)==h,(name,path);pins[path]=h
    def historical(path):return subprocess.check_output([G,'-C',str(ROOT/'publication/sonic-adventure'),'show',BASELINE+':'+path])
    baseline=json.loads(historical('PUBLICATION_MANIFEST.json'));assert len(baseline['files'])==14813
    mutable_names={'README.md','SOURCE_HANDOFF.txt','project_status.json','recovered_routines.json','case_dependency_map.json','main_dispatch_case_map_verification.json','project_completion_audit.json','recovery_coverage_audit.json','recovery_registry_evidence.json','unrecovered_function_candidates.tsv'}
    mutable=[r['path'] for r in baseline['files'] if Path(r['path']).name in mutable_names];assert len(mutable)==10
    for row in baseline['files']:
        if row['path'] not in mutable:assert digest(ROOT/row['path'])==row['sha256'],row['path']
    old_registry=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];now=json.loads(rp.read_text())['routines'];assert len(old_registry)==1546 and now[:1546]==old_registry and len(now)==1547
    old_status=json.loads(historical('outputs/sonic-adventure/project_status.json'));status=json.loads(sp.read_text());assert status['requirements']==old_status['requirements']
    for key,value in old_status['verified_progress'].items():assert status['verified_progress'][key]==(1547 if key=='recovered_buildable_routines' else value),key
    builds=json.loads((P/'analysis/movie_filesystem_command8_previous_build_manifest.json').read_text());assert len(builds['files'])==2699
    for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    ps=P/'build_movie_filesystem_command8.ps1';sources=re.findall(r"'([^']+\.c)'",ps.read_text());assert len(sources)==len(set(sources))==234
    powershell=[ps,P/'ghidra/export_movie_filesystem_command8.ps1']
    for path in powershell:
        command="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(path)
        subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
    strict=P/'analysis/movie_filesystem_command8_unbounded.json';r=json.loads(strict.read_text());assert r['source_sha256']=={str(P/'source'/name):digest(P/'source'/name) for name in sources} and r['exit_code']==0
    coverage=json.loads((P/'analysis/recovery_coverage_audit.json').read_text());assert coverage['registered_routines']==1547 and not coverage['registry_evidence_errors'] and not coverage['whole_source_hash_refresh_needed']
    q=json.loads((P/'analysis/movie_filesystem_command8_qualification.json').read_text());assert (q['newly_registered_entries'],q['newly_qualified_rows'],q['qualified_conditional_outcomes'])==(1,116,18)
    files=scripts+powershell+[strict,P/'analysis/movie_filesystem_command8_qualification.json']
    report=dict(reproduced_generated_artifacts=8,registration_byte_idempotent=True,prior_registry_rows_identical=1546,registry_entries=1547,new_routine_credit=1,qualified_new_rows=116,qualified_new_outcomes=18,newly_translated_unregistered_rows=116,canonical_adapter_rows=0,partial_unregistered_gateway_rows=0,canonical_duplicate_credit=0,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(powershell),pinned_inputs_current=len(pins),compiled_source_count=234,strict_unbounded_sources_checked=234,preserved_baseline_commit=BASELINE,preserved_prior_selected_files=14803,reviewed_mutable_paths=mutable,original_requirements_preserved=True,preserved_build_files=2699,additional_build_files=['work/sonic-adventure/build/movie_filesystem_command8.dll','work/sonic-adventure/build/movie_filesystem_command8.lib'],fresh_original_native_comparisons=q['fresh_original_native_comparisons'],fresh_independent_checks=q['fresh_independent_checks'],total_evidence_original_native_comparisons=q['total_evidence_original_native_comparisons'],physical_maple_dma_complete=False,full_filesystem_qualified=False,overall_completion_percent=10,estimate_method='weighted milestones, not measured code coverage',sha256={str(p):digest(p) for p in files},scope='Eight artifacts reproduce after one-entry rename registration against frozen1546 baseline. Existing14803 source/proofs and2699builds preserved exceptten reviewed catalog/status/docpaths. New234DLL/lib areadditional.116rename rows qualify(113executed+3constantproof),18outcomes(17executed+1impossibleproof). Originalrequirements preserved;physicalhardware/fullfilesystem/game/portsunfinished;goalactive10%,researchDLLsnotplayable.')
    (P/'analysis/movie_filesystem_command8_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('COMMAND8 WORKFLOW PASS14803preserved source/proofs;',len(pins),'pins',flush=True)
if __name__=='__main__':main()
