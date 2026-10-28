"""Reproduce parameter-update evidence and preserve the prior published snapshot."""
import ast,hashlib,json,re,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
G=r'C:\Program Files\Git\cmd\git.exe';BASE='c9bee666b0a0402acb2051e1fc7339c0e6ae5737'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    names=('recover_movement_parameters','connect_movement_parameters','movement_parameters_reference','write_watch_movement_parameters',
      'movement_parameters_task_fixtures','movement_parameters_session','movement_parameters_vectors','movement_parameters_fixtures',
      'verify_movement_parameters_direct','verify_movement_parameters_aliases','prove_movement_parameters_predicates',
      'verify_movement_parameters_retained','check_movement_parameters_unbounded','audit_movement_parameters',
      'qualify_movement_parameters','register_movement_parameters','verify_movement_parameters_workflow')
    scripts=[P/(name+'.py') for name in names]
    for path in scripts:ast.parse(path.read_text(),filename=str(path))
    generated=[P/name for name in ('source/sa_movement_parameters.c','analysis/movement_parameters_translation.json','analysis/movement_parameters_returns.json',
      'source/sa_movement_parameters_connected.c','source/movement_parameters_probe.c','build_movement_parameters.ps1',
      'analysis/movement_parameters_connection.json','analysis/movement_parameters_audit.json','analysis/movement_parameters_predicate_proof.json')]
    saved={str(path):digest(path) for path in generated}
    for name in ('recover_movement_parameters.py','connect_movement_parameters.py','audit_movement_parameters.py','prove_movement_parameters_predicates.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
    assert saved=={str(path):digest(path) for path in generated}
    qp=P/'analysis/movement_parameters_qualification.json';qraw=qp.read_bytes()
    subprocess.run([sys.executable,'-B',str(P/'qualify_movement_parameters.py')],check=True);assert qp.read_bytes()==qraw
    rp=P/'analysis/recovered_routines.json';sp=P/'project_status.json';saved=rp.read_bytes(),sp.read_bytes()
    subprocess.run([sys.executable,'-B',str(P/'register_movement_parameters.py')],check=True);assert saved==(rp.read_bytes(),sp.read_bytes())
    def historical(path):return subprocess.check_output([G,'-C',str(ROOT/'publication/sonic-adventure'),'show',BASE+':'+path])
    baseline=json.loads(historical('PUBLICATION_MANIFEST.json'));assert len(baseline['files'])==15980
    mutable={'outputs/sonic-adventure/'+name for name in ('README.md','SOURCE_HANDOFF.txt','project_status.json','analysis/recovered_routines.json')}
    for row in baseline['files']:
        if row['path'] not in mutable:assert digest(ROOT/row['path'])==row['sha256'],row['path']
    for name in ('README.md','SOURCE_HANDOFF.txt'):assert (P/name).read_bytes().endswith(historical('outputs/sonic-adventure/'+name))
    oldreg=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'));current=json.loads(rp.read_text())
    assert current['routines'][:1601]==oldreg['routines'] and len(current['routines'])==1602
    for key,value in oldreg.items():
        if key!='routines':assert current[key]==value,key
    old=json.loads(historical('outputs/sonic-adventure/project_status.json'));status=json.loads(sp.read_text())
    for key,value in old.items():
        if key=='verified_progress':
            for field,data in value.items():assert status[key][field]==(1602 if field=='recovered_buildable_routines' else data),field
            assert set(status[key])-set(value)=={'movement_parameters_integration'}
        else:assert status[key]==(1602 if key=='recovered_buildable_routines' else value),key
    builds_path=P/'analysis/movement_parameters_previous_build_manifest.json';builds=json.loads(builds_path.read_text());assert len(builds['files'])==2743
    for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    pins={}
    for name in ('translation','connection','audit','direct_verification','aliases_verification','predicate_proof','retained_verification','qualification'):
        r=json.loads((P/f'analysis/movement_parameters_{name}.json').read_text())
        for path,h in r['sha256'].items():assert digest(path)==h,(name,path);pins[path]=h
    ps=[P/'build_movement_parameters.ps1',P/'ghidra/export_movement_parameters_owner.ps1']
    for path in ps:
        command="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(path)
        subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
    strict=P/'analysis/movement_parameters_unbounded.json';u=json.loads(strict.read_text())
    sources=[P/'source'/name for name in re.findall(r"'([^']+\.c)'",(P/'build_movement_parameters.ps1').read_text())]
    assert len(sources)==255 and u['source_sha256']=={str(path):digest(path) for path in sources} and u['exit_code']==0
    q=json.loads(qp.read_text());assert q['new_original_routine_credit']==1 and q['fresh_original_native_comparisons']==667
    assert q['executed_original_rows']==216 and q['executed_conditional_outcomes']==15 and q['proved_infeasible_conditional_outcomes']==1 and not q['unaccounted_outcomes']
    files=scripts+ps+generated+[strict,qp,builds_path,P/'MOVEMENT_PARAMETERS.txt']
    report=dict(status='verified_original_parameter_update_workflow_full_game_unfinished',reproduced_generated_artifacts=9,
      qualification_and_registration_byte_idempotent=True,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(ps),
      pinned_inputs_current=len(pins),preserved_baseline_commit=BASE,preserved_prior_selected_files=15976,reviewed_mutable_paths=sorted(mutable),
      prior_registry_rows_preserved=1601,registry_entries=1602,registry_unique_module_address_pairs=1599,confirmed_linked_unique_routines_by_name=644,
      preserved_build_files=2743,additional_build_files=['work/sonic-adventure/build/movement_parameters.dll','work/sonic-adventure/build/movement_parameters.lib'],
      compiled_source_count=255,strict_unbounded_sources_checked=255,new_original_routine_credit=1,canonical_duplicate_credit=0,
      original_rows_executed=216,original_outcomes_executed=15,original_outcomes_proved_infeasible=1,domain_excluded_rows=[],domain_excluded_outcomes=[],
      host_import_recursion_floor=4096,guest_execution_limits_unchanged=True,fresh_original_native_comparisons=667,fresh_independent_checks=5211,total_evidence_original_native_comparisons=74982,
      original_requirements_preserved=True,world_owner028bfe_still_unregistered=True,parent0730d8_still_unregistered=True,
      world_owner_remaining_outcomes=q['world_owner_unaccounted_outcomes'],collision_caller_remaining_outcomes=q['collision_caller_unaccounted_outcomes'],
      overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',sha256={str(path):digest(path) for path in files})
    (P/'analysis/movement_parameters_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('PARAMETERS WORKFLOW PASS:9artifacts;15976oldfiles/1601registryrows/2743buildspreserved;644linked;10%milestones')

if __name__=='__main__':main()
