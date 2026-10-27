"""Reproduce current artifacts and preserve the published source/proof baseline."""
import ast,hashlib,json,re,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];BASELINE='776209cad5514628962ae96ad1066e0187a2433f'
G=r'C:\Program Files\Git\cmd\git.exe'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    generated=[P/n for n in ('source/sa_movie_input_submission.c','analysis/movie_input_submission_translation.json','analysis/movie_input_submission_returns.json','source/sa_movie_input_submission_connected.c','source/movie_input_submission_probe.c','build_movie_input_submission.ps1','analysis/movie_input_submission_connection.json','analysis/movie_input_submission_audit.json')]
    before={str(p):digest(p) for p in generated}
    for name in ('recover_movie_input_submission.py','connect_movie_input_submission.py','audit_movie_input_submission.py'):
        subprocess.run([sys.executable,'-B',str(P/name)],check=True)
    assert all(digest(p)==h for p,h in before.items())
    rp=P/'analysis/recovered_routines.json';sp=P/'project_status.json';saved=rp.read_bytes(),sp.read_bytes()
    subprocess.run([sys.executable,'-B',str(P/'register_movie_input_submission.py')],check=True)
    assert saved==(rp.read_bytes(),sp.read_bytes())
    names=('recover_movie_input_submission','connect_movie_input_submission','input_submission_reference_cpu','movie_input_submission_reference','write_watch_input_submission','movie_input_submission_fixtures','movie_input_submission_task_fixtures','check_movie_input_submission_unbounded','verify_movie_input_submission','verify_movie_input_submission_lifecycle','verify_movie_input_submission_faults','verify_movie_input_submission_retained','audit_movie_input_submission','qualify_movie_input_submission','register_movie_input_submission','verify_movie_input_submission_workflow')
    scripts=[P/(n+'.py') for n in names]
    for path in scripts:ast.parse(path.read_text())
    pins={}
    for name in ('translation','connection','audit','verification','lifecycle_verification','faults_verification','retained_verification','qualification'):
        report=json.loads((P/f'analysis/movie_input_submission_{name}.json').read_text())
        for path,h in report['sha256'].items():assert digest(path)==h,(name,path);pins[path]=h
    def historical(path):return subprocess.check_output([G,'-C',str(ROOT/'publication/sonic-adventure'),'show',BASELINE+':'+path])
    baseline=json.loads(historical('PUBLICATION_MANIFEST.json'));assert len(baseline['files'])==14270
    mutable_names={'README.md','SOURCE_HANDOFF.txt','project_status.json','recovered_routines.json','case_dependency_map.json','main_dispatch_case_map_verification.json','project_completion_audit.json','recovery_coverage_audit.json','recovery_registry_evidence.json','unrecovered_function_candidates.tsv'}
    mutable=[r['path'] for r in baseline['files'] if Path(r['path']).name in mutable_names];assert len(mutable)==10
    for row in baseline['files']:
        if row['path'] not in mutable:assert digest(ROOT/row['path'])==row['sha256'],row['path']
    old_registry=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];now=json.loads(rp.read_text())['routines']
    assert len(old_registry)==1502 and now[:1502]==old_registry and len(now)==1508
    old_status=json.loads(historical('outputs/sonic-adventure/project_status.json'));status=json.loads(sp.read_text());assert status['requirements']==old_status['requirements']
    for key,value in old_status['verified_progress'].items():assert status['verified_progress'][key]==(1508 if key=='recovered_buildable_routines' else value),key
    builds=json.loads((P/'analysis/movie_input_submission_previous_build_manifest.json').read_text());assert len(builds['files'])==2679
    for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    ps=P/'build_movie_input_submission.ps1';sources=re.findall(r"'([^']+\.c)'",ps.read_text());assert len(sources)==len(set(sources))==224
    powershell=[ps,P/'ghidra/export_movie_input_submission.ps1']
    for path in powershell:
        command="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(path)
        subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
    strict=P/'analysis/movie_input_submission_unbounded.json';r=json.loads(strict.read_text())
    assert r['source_sha256']=={str(P/'source'/name):digest(P/'source'/name) for name in sources} and r['exit_code']==0
    coverage=json.loads((P/'analysis/recovery_coverage_audit.json').read_text())
    assert coverage['registered_routines']==1508 and not coverage['registry_evidence_errors'] and not coverage['whole_source_hash_refresh_needed']
    q=json.loads((P/'analysis/movie_input_submission_qualification.json').read_text())
    assert (q['newly_registered_entries'],q['newly_qualified_rows'],q['qualified_conditional_outcomes'],q['fresh_original_native_comparisons'],q['fresh_independent_checks'])==(6,351,46,1247,18295)
    files=scripts+powershell+[strict,P/'analysis/movie_input_submission_qualification.json']
    report=dict(reproduced_generated_artifacts=8,registration_byte_idempotent=True,prior_registry_rows_identical=1502,registry_entries=1508,new_routine_credit=6,qualified_new_rows=351,qualified_new_outcomes=46,canonical_duplicate_credit=0,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(powershell),pinned_inputs_current=len(pins),compiled_source_count=224,strict_unbounded_sources_checked=224,preserved_baseline_commit=BASELINE,preserved_prior_selected_files=14260,reviewed_mutable_paths=mutable,original_requirements_preserved=True,preserved_build_files=2679,additional_build_files=['work/sonic-adventure/build/movie_input_submission.dll','work/sonic-adventure/build/movie_input_submission.lib'],fresh_original_native_comparisons=1247,fresh_independent_checks=18295,total_evidence_original_native_comparisons=18474,physical_maple_dma_complete=False,natural_response_to_task_parser_complete=False,overall_completion_percent=10,estimate_method='weighted milestones, not measured code coverage',sha256={str(p):digest(p) for p in files},scope='Current generation is reproducible after six-entry registration using frozen1502 baseline. Existing14260selected source/proofs and2679builds preserved except10reviewed catalog/status/doc paths. New224 strict sources, original request/submission/response and retained/failure cases scoped to explicit software inputs. Physical transport, active task parser and complete game/ports unfinished. Goal active10%.')
    (P/'analysis/movie_input_submission_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('SUBMISSION WORKFLOW PASS14260preserved source/proofs;',len(pins),'pins',flush=True)
if __name__=='__main__':main()
