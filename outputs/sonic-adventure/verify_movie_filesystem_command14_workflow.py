"""Reproducibility, original-file preservation and current proof pin checks."""
import ast,hashlib,json,re,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];BASELINE='02bdecc88efee4b41a583243f1068fce24d98c3e';G=r'C:\Program Files\Git\cmd\git.exe'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    generated=[P/n for n in ('source/sa_movie_filesystem_command14.c','analysis/movie_filesystem_command14_translation.json','analysis/movie_filesystem_command14_returns.json','source/sa_movie_filesystem_command14_connected.c','source/movie_filesystem_command14_probe.c','build_movie_filesystem_command14.ps1','analysis/movie_filesystem_command14_connection.json','analysis/movie_filesystem_command14_audit.json')]
    before={str(p):digest(p) for p in generated}
    for name in ('recover_movie_filesystem_command14.py','connect_movie_filesystem_command14.py','audit_movie_filesystem_command14.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
    assert all(digest(p)==h for p,h in before.items())
    rp=P/'analysis/recovered_routines.json';sp=P/'project_status.json';saved=rp.read_bytes(),sp.read_bytes()
    subprocess.run([sys.executable,'-B',str(P/'register_movie_filesystem_command14.py')],check=True);assert saved==(rp.read_bytes(),sp.read_bytes())
    names=('recover_movie_filesystem_command14','connect_movie_filesystem_command14','movie_filesystem_command14_reference','filesystem_write_reference_cpu','write_watch_filesystem_command14','movie_filesystem_command14_task_fixtures','movie_filesystem_command14_session','movie_filesystem_command14_vectors','check_movie_filesystem_command14_unbounded','verify_movie_filesystem_command14_seek','verify_movie_filesystem_command14_guards','verify_movie_filesystem_command14_tasks','merge_movie_filesystem_command14_tasks','verify_movie_filesystem_command14_retained','verify_movie_filesystem_command14_frontier','audit_movie_filesystem_command14','qualify_movie_filesystem_command14','register_movie_filesystem_command14','verify_movie_filesystem_command14_workflow')
    scripts=[P/(n+'.py') for n in names]
    for path in scripts:ast.parse(path.read_text())
    pins={}
    for name in ('translation','connection','audit','seek_verification','guards_verification','tasks_verification','retained_verification','frontier_verification','qualification'):
        report=json.loads((P/f'analysis/movie_filesystem_command14_{name}.json').read_text())
        for path,h in report['sha256'].items():assert digest(path)==h,(name,path);pins[path]=h
    def historical(path):return subprocess.check_output([G,'-C',str(ROOT/'publication/sonic-adventure'),'show',BASELINE+':'+path])
    baseline=json.loads(historical('PUBLICATION_MANIFEST.json'));assert len(baseline['files'])==15183
    mutable_names={'README.md','SOURCE_HANDOFF.txt','project_status.json','recovered_routines.json','case_dependency_map.json','main_dispatch_case_map_verification.json','project_completion_audit.json','recovery_coverage_audit.json','recovery_registry_evidence.json','unrecovered_function_candidates.tsv'}
    mutable=[r['path'] for r in baseline['files'] if Path(r['path']).name in mutable_names];assert len(mutable)==10
    for row in baseline['files']:
        if row['path'] not in mutable:assert digest(ROOT/row['path'])==row['sha256'],row['path']
    old_registry=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];now=json.loads(rp.read_text())['routines'];assert len(old_registry)==1577 and now[:1577]==old_registry and len(now)==1579
    old_status=json.loads(historical('outputs/sonic-adventure/project_status.json'));status=json.loads(sp.read_text());assert status['requirements']==old_status['requirements']
    for key,value in old_status['verified_progress'].items():assert status['verified_progress'][key]==(1579 if key=='recovered_buildable_routines' else value),key
    builds=json.loads((P/'analysis/movie_filesystem_command14_previous_build_manifest.json').read_text());assert len(builds['files'])==2711
    for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    ps=P/'build_movie_filesystem_command14.ps1';sources=re.findall(r"'([^']+\.c)'",ps.read_text());assert len(sources)==len(set(sources))==240
    powershell=[ps,P/'ghidra/export_movie_filesystem_command14.ps1',P/'ghidra/export_movie_filesystem_command14_dependencies.ps1']
    for path in powershell:
        command="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(path)
        subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
    strict=P/'analysis/movie_filesystem_command14_unbounded.json';r=json.loads(strict.read_text());assert r['source_sha256']=={str(P/'source'/name):digest(P/'source'/name) for name in sources} and r['exit_code']==0
    coverage=json.loads((P/'analysis/recovery_coverage_audit.json').read_text());assert coverage['registered_routines']==1579 and not coverage['registry_evidence_errors'] and not coverage['whole_source_hash_refresh_needed']
    q=json.loads((P/'analysis/movie_filesystem_command14_qualification.json').read_text());assert (q['newly_registered_entries'],q['newly_qualified_rows'],q['qualified_conditional_outcomes'])==(2,128,22)
    files=scripts+powershell+[strict,P/'analysis/movie_filesystem_command14_qualification.json']
    report=dict(reproduced_generated_artifacts=8,registration_byte_idempotent=True,prior_registry_rows_identical=1577,registry_entries=1579,new_routine_credit=2,qualified_new_rows=128,qualified_new_outcomes=22,newly_translated_unregistered_rows=0,canonical_adapter_rows=0,partial_unregistered_gateway_rows=0,canonical_duplicate_credit=0,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(powershell),pinned_inputs_current=len(pins),compiled_source_count=240,strict_unbounded_sources_checked=240,preserved_baseline_commit=BASELINE,preserved_prior_selected_files=15173,reviewed_mutable_paths=mutable,original_requirements_preserved=True,preserved_build_files=2711,additional_build_files=['work/sonic-adventure/build/movie_filesystem_command14.dll','work/sonic-adventure/build/movie_filesystem_command14.lib'],fresh_original_native_comparisons=q['fresh_original_native_comparisons'],fresh_independent_checks=q['fresh_independent_checks'],total_evidence_original_native_comparisons=q['total_evidence_original_native_comparisons'],physical_maple_dma_complete=False,full_filesystem_qualified=False,overall_completion_percent=10,estimate_method='weighted milestones, not measured code coverage',sha256={str(p):digest(p) for p in files},scope='Eightartifacts reproduce after2entry range-read/seek qualification against frozen1577baseline.15173prior source/proofs preserved except10catalog/status/docpaths;2711oldbuilds byteidentical.240DLL/libadditional.128qualifiedrows/22outcomes all execute in declared raw/mappedABI domains. Completedauto-countunderflowtasks notclaimed;originalarithmetic retained. Requirementsunchanged;10%weightedmilestones,notcoverage;researchDLLnotplayable/physicalproof.')
    (P/'analysis/movie_filesystem_command14_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('COMMAND14 WORKFLOW PASS15173preserved source/proofs;',len(pins),'pins',flush=True)
if __name__=='__main__':main()
