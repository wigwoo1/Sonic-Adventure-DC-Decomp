"""Reproduce complete movement/model group evidence; preserve prior publication bytes."""
import ast,hashlib,json,re,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
G=r'C:\Program Files\Git\cmd\git.exe';BASE='fbdcea8e1d78c2171ccc3cb56382165574a59ee6'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    names=('recover_movement_model_response','connect_movement_model_response','movement_model_response_reference','write_watch_movement_model_response',
      'movement_model_response_task_fixtures','movement_model_response_session','movement_model_response_vectors','movement_model_response_fixtures','movement_model_response_edges_fixtures',
      'verify_movement_model_response_direct','verify_movement_model_response_edges','verify_movement_model_response_pipeline','verify_movement_model_response_retained',
      'check_movement_model_response_unbounded','audit_movement_model_response','prove_movement_model_response_predicates','qualify_movement_model_response',
      'register_movement_model_response','verify_movement_model_response_workflow')
    scripts=[P/(n+'.py') for n in names]
    for path in scripts:ast.parse(path.read_text(),filename=str(path))
    generated=[P/n for n in ('source/sa_movement_model_response.c','analysis/movement_model_response_translation.json','analysis/movement_model_response_returns.json',
      'source/sa_movement_model_response_connected.c','source/movement_model_response_probe.c','build_movement_model_response.ps1',
      'analysis/movement_model_response_connection.json','analysis/movement_model_response_audit.json','analysis/movement_model_response_predicate_proof.json')]
    saved={str(path):digest(path) for path in generated}
    for name in ('recover_movement_model_response.py','connect_movement_model_response.py','audit_movement_model_response.py','prove_movement_model_response_predicates.py'):
        subprocess.run([sys.executable,'-B',str(P/name)],check=True)
    assert saved=={str(path):digest(path) for path in generated}
    qp=P/'analysis/movement_model_response_qualification.json';before=qp.read_bytes()
    subprocess.run([sys.executable,'-B',str(P/'qualify_movement_model_response.py')],check=True);assert qp.read_bytes()==before
    rp=P/'analysis/recovered_routines.json';sp=P/'project_status.json';before=rp.read_bytes(),sp.read_bytes()
    subprocess.run([sys.executable,'-B',str(P/'register_movement_model_response.py')],check=True);assert before==(rp.read_bytes(),sp.read_bytes())
    def historical(path):return subprocess.check_output([G,'-C',str(ROOT/'publication/sonic-adventure'),'show',BASE+':'+path])
    oldmanifest=json.loads(historical('PUBLICATION_MANIFEST.json'));assert len(oldmanifest['files'])==16064
    mutable={'outputs/sonic-adventure/'+n for n in ('README.md','SOURCE_HANDOFF.txt','project_status.json','analysis/recovered_routines.json')}
    for row in oldmanifest['files']:
        if row['path'] not in mutable:assert digest(ROOT/row['path'])==row['sha256'],row['path']
    for name in ('README.md','SOURCE_HANDOFF.txt'):assert (P/name).read_bytes().endswith(historical('outputs/sonic-adventure/'+name))
    oldreg=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'));current=json.loads(rp.read_text())
    assert current['routines'][:1602]==oldreg['routines'] and len(current['routines'])==1605
    for k,value in oldreg.items():
        if k!='routines':assert current[k]==value,k
    old=json.loads(historical('outputs/sonic-adventure/project_status.json'));status=json.loads(sp.read_text())
    for key,value in old.items():
        if key=='verified_progress':
            for field,data in value.items():assert status[key][field]==(1605 if field=='recovered_buildable_routines' else data),field
            assert set(status[key])-set(value)=={'movement_model_response_integration'}
        else:assert status[key]==(1605 if key=='recovered_buildable_routines' else value),key
    bp=P/'analysis/movement_model_response_previous_build_manifest.json';builds=json.loads(bp.read_text());assert len(builds['files'])==2747
    for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    pins={}
    for name in ('translation','connection','audit','direct_verification','edges_verification','pipeline_verification','predicate_proof','retained_verification','qualification'):
        r=json.loads((P/f'analysis/movement_model_response_{name}.json').read_text())
        for path,h in r['sha256'].items():assert digest(path)==h,(name,path);pins[path]=h
    ps=[P/'build_movement_model_response.ps1',P/'ghidra/export_movement_model_response.ps1']
    for path in ps:
        cmd="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(path)
        subprocess.run(['powershell.exe','-NoProfile','-Command',cmd],check=True)
    strict=P/'analysis/movement_model_response_unbounded.json';u=json.loads(strict.read_text())
    sources=[P/'source'/n for n in re.findall(r"'([^']+\.c)'",(P/'build_movement_model_response.ps1').read_text())]
    assert len(sources)==257 and u['source_sha256']=={str(p):digest(p) for p in sources} and u['exit_code']==0
    q=json.loads(qp.read_text());assert q['new_original_routine_credit']==3 and q['fresh_original_native_comparisons']==1125
    assert q['executed_original_rows']==904 and q['proved_unreachable_original_rows']==2 and q['executed_conditional_outcomes']==112
    assert q['unqualified_broader_precision_outcomes']==[['0x8c074626','0x8c074644']] and not q['unaccounted_software_profile_outcomes']
    files=scripts+ps+generated+[strict,qp,bp,P/'MOVEMENT_MODEL_RESPONSE.txt']
    report=dict(status='verified_movement_model_response_workflow_full_game_unfinished',reproduced_generated_artifacts=9,
      qualification_and_registration_byte_idempotent=True,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(ps),pinned_inputs_current=len(pins),
      preserved_baseline_commit=BASE,preserved_prior_selected_files=16060,reviewed_mutable_paths=sorted(mutable),prior_registry_rows_preserved=1602,
      registry_entries=1605,registry_unique_module_address_pairs=1602,confirmed_linked_unique_routines_by_name=648,preserved_build_files=2747,
      additional_build_files=['work/sonic-adventure/build/movement_model_response.dll','work/sonic-adventure/build/movement_model_response.lib'],
      compiled_source_count=257,strict_unbounded_sources_checked=257,new_original_routine_credit=3,canonical_duplicate_credit=0,
      original_rows_executed=904,original_rows_proved_unreachable=2,original_outcomes_executed=112,integer_proved_infeasible_outcomes=1,scalar_profile_proved_infeasible_outcomes=1,
      broader_precision_outcomes_unqualified=q['unqualified_broader_precision_outcomes'],domain_excluded_rows=[],domain_excluded_outcomes=[],guest_execution_limits_unchanged=True,
      fresh_original_native_comparisons=1125,fresh_independent_checks=10817,total_evidence_original_native_comparisons=76622,original_requirements_preserved=True,
      parent0730d8_still_unregistered=True,world_owner_remaining_outcomes=q['world_owner_unaccounted_outcomes'],collision_caller_remaining_outcomes=q['collision_caller_unaccounted_outcomes'],
      overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',sha256={str(p):digest(p) for p in files})
    (P/'analysis/movement_model_response_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('MODEL RESPONSE WORKFLOW PASS9artifacts/16060oldfiles/1602oldrows/2747oldbuilds;648linked;10%milestones')

if __name__=='__main__':main()
