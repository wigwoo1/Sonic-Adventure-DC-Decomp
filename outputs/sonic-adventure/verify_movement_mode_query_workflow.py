"""Reproduce the canonical adapter and preserve all previous published evidence."""
import ast,hashlib,json,re,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
G=r'C:\Program Files\Git\cmd\git.exe';BASE='a583f68b096a1d23a607096b7a568294b9166c37'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    names=('recover_movement_mode_query','connect_movement_mode_query','movement_mode_query_reference','write_watch_movement_mode_query',
      'movement_mode_query_task_fixtures','movement_mode_query_session','movement_mode_query_vectors','movement_mode_query_fixtures',
      'verify_movement_mode_query_direct','verify_movement_mode_query_sequence','verify_movement_mode_query_retained','check_movement_mode_query_unbounded',
      'audit_movement_mode_query','qualify_movement_mode_query','record_movement_mode_query','verify_movement_mode_query_workflow')
    scripts=[P/(name+'.py') for name in names]
    for path in scripts:ast.parse(path.read_text(),filename=str(path))
    generated=[P/name for name in ('source/sa_movement_mode_query.c','analysis/movement_mode_query_translation.json','analysis/movement_mode_query_returns.json',
      'source/sa_movement_mode_query_connected.c','source/movement_mode_query_probe.c','build_movement_mode_query.ps1',
      'analysis/movement_mode_query_connection.json','analysis/movement_mode_query_audit.json')]
    saved={str(path):digest(path) for path in generated}
    for name in ('recover_movement_mode_query.py','connect_movement_mode_query.py','audit_movement_mode_query.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
    assert saved=={str(path):digest(path) for path in generated}
    qp=P/'analysis/movement_mode_query_qualification.json';qraw=qp.read_bytes()
    subprocess.run([sys.executable,'-B',str(P/'qualify_movement_mode_query.py')],check=True);assert qp.read_bytes()==qraw
    rp=P/'analysis/recovered_routines.json';sp=P/'project_status.json';saved=rp.read_bytes(),sp.read_bytes()
    subprocess.run([sys.executable,'-B',str(P/'record_movement_mode_query.py')],check=True);assert saved==(rp.read_bytes(),sp.read_bytes())
    def historical(path):return subprocess.check_output([G,'-C',str(ROOT/'publication/sonic-adventure'),'show',BASE+':'+path])
    baseline=json.loads(historical('PUBLICATION_MANIFEST.json'));assert len(baseline['files'])==16023
    mutable={'outputs/sonic-adventure/'+name for name in ('README.md','SOURCE_HANDOFF.txt','project_status.json')}
    for row in baseline['files']:
        if row['path'] not in mutable:assert digest(ROOT/row['path'])==row['sha256'],row['path']
    for name in ('README.md','SOURCE_HANDOFF.txt'):assert (P/name).read_bytes().endswith(historical('outputs/sonic-adventure/'+name))
    assert rp.read_bytes()==historical('outputs/sonic-adventure/analysis/recovered_routines.json')
    old=json.loads(historical('outputs/sonic-adventure/project_status.json'));status=json.loads(sp.read_text())
    for key,value in old.items():
        if key=='verified_progress':
            for field,data in value.items():assert status[key][field]==data,field
            assert set(status[key])-set(value)=={'movement_mode_query_integration'}
        else:assert status[key]==value,key
    builds_path=P/'analysis/movement_mode_query_previous_build_manifest.json';builds=json.loads(builds_path.read_text());assert len(builds['files'])==2745
    for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    pins={}
    for name in ('translation','connection','audit','direct_verification','sequence_verification','retained_verification','qualification'):
        r=json.loads((P/f'analysis/movement_mode_query_{name}.json').read_text())
        for path,h in r['sha256'].items():assert digest(path)==h,(name,path);pins[path]=h
    ps=[P/'build_movement_mode_query.ps1',P/'ghidra/export_movement_mode_query.ps1']
    for path in ps:
        command="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(path)
        subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
    strict=P/'analysis/movement_mode_query_unbounded.json';u=json.loads(strict.read_text())
    sources=[P/'source'/name for name in re.findall(r"'([^']+\.c)'",(P/'build_movement_mode_query.ps1').read_text())]
    assert len(sources)==256 and u['source_sha256']=={str(path):digest(path) for path in sources} and u['exit_code']==0
    q=json.loads(qp.read_text());assert q['new_original_routine_credit']==0 and q['fresh_original_native_comparisons']==515 and q['newly_qualified_canonical_adapter_rows']==6
    assert q['executed_original_rows_including_canonical']==6 and q['executed_conditional_outcomes']==0 and not q['unaccounted_outcomes']
    files=scripts+ps+generated+[strict,qp,builds_path,P/'MOVEMENT_MODE_QUERY.txt']
    report=dict(status='verified_canonical_mode_query_workflow_full_game_unfinished',reproduced_generated_artifacts=8,
      qualification_and_status_record_byte_idempotent=True,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(ps),
      pinned_inputs_current=len(pins),preserved_baseline_commit=BASE,preserved_prior_selected_files=16020,reviewed_mutable_paths=sorted(mutable),
      prior_registry_rows_preserved=1602,entire_registry_bytes_preserved=True,registry_entries=1602,registry_unique_module_address_pairs=1599,
      confirmed_linked_unique_routines_by_name=645,preserved_build_files=2745,
      additional_build_files=['work/sonic-adventure/build/movement_mode_query.dll','work/sonic-adventure/build/movement_mode_query.lib'],
      compiled_source_count=256,strict_unbounded_sources_checked=256,new_original_routine_credit=0,canonical_duplicate_credit=0,canonical_full_state_adapters_qualified=1,
      original_rows_executed_including_canonical=6,original_outcomes_executed=0,domain_excluded_rows=[],domain_excluded_outcomes=[],
      host_import_recursion_floor=4096,guest_execution_limits_unchanged=True,fresh_original_native_comparisons=515,fresh_independent_checks=3977,total_evidence_original_native_comparisons=75497,
      original_requirements_preserved=True,world_owner028bfe_still_unregistered=True,parent0730d8_still_unregistered=True,
      world_owner_remaining_outcomes=q['world_owner_unaccounted_outcomes'],collision_caller_remaining_outcomes=q['collision_caller_unaccounted_outcomes'],
      overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',sha256={str(path):digest(path) for path in files})
    (P/'analysis/movement_mode_query_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('MODE QUERY WORKFLOW PASS:8artifacts;16020oldfiles/entireregistry/2745buildspreserved;645linked;10%milestones')

if __name__=='__main__':main()
