"""Reproducible current evidence and preservation against the actual publication baseline."""
import ast,hashlib,json,re,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
G=r'C:\Program Files\Git\cmd\git.exe';BASE='06371602cceb288b74d14fb585cb9044019cb793'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    names=('recover_movement_world','connect_movement_world','movement_world_reference','write_watch_movement_world',
      'movement_world_task_fixtures','movement_world_session','movement_world_vectors','movement_world_fixtures',
      'verify_movement_world_direct','verify_movement_world_aliases','verify_movement_world_sequence',
      'verify_movement_world_retained','check_movement_world_unbounded','audit_movement_world',
      'prove_movement_world_predicates','qualify_movement_world','register_movement_world','verify_movement_world_workflow')
    scripts=[P/(name+'.py') for name in names]
    for path in scripts:ast.parse(path.read_text(),filename=str(path))
    generated=[P/name for name in ('source/sa_movement_world.c','analysis/movement_world_translation.json',
      'analysis/movement_world_returns.json','source/sa_movement_world_connected.c','source/movement_world_probe.c',
      'build_movement_world.ps1','analysis/movement_world_connection.json','analysis/movement_world_audit.json',
      'analysis/movement_world_predicate_proof.json')]
    saved={str(path):digest(path) for path in generated}
    for name in ('recover_movement_world.py','connect_movement_world.py','audit_movement_world.py','prove_movement_world_predicates.py'):
        subprocess.run([sys.executable,'-B',str(P/name)],check=True)
    assert saved=={str(path):digest(path) for path in generated}
    qp=P/'analysis/movement_world_qualification.json';qraw=qp.read_bytes()
    subprocess.run([sys.executable,'-B',str(P/'qualify_movement_world.py')],check=True);assert qp.read_bytes()==qraw
    rp=P/'analysis/recovered_routines.json';sp=P/'project_status.json';saved=rp.read_bytes(),sp.read_bytes()
    subprocess.run([sys.executable,'-B',str(P/'register_movement_world.py')],check=True);assert saved==(rp.read_bytes(),sp.read_bytes())
    def historical(path):return subprocess.check_output([G,'-C',str(ROOT/'publication/sonic-adventure'),'show',BASE+':'+path])
    baseline=json.loads(historical('PUBLICATION_MANIFEST.json'));assert len(baseline['files'])==15829
    mutable={'outputs/sonic-adventure/'+name for name in ('README.md','SOURCE_HANDOFF.txt','project_status.json','analysis/recovered_routines.json')}
    for row in baseline['files']:
        if row['path'] not in mutable:assert digest(ROOT/row['path'])==row['sha256'],row['path']
    for name in ('README.md','SOURCE_HANDOFF.txt'):assert (P/name).read_bytes().endswith(historical('outputs/sonic-adventure/'+name))
    oldreg=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'));current=json.loads(rp.read_text())
    assert current['routines'][:1594]==oldreg['routines'] and len(current['routines'])==1596
    for key,value in oldreg.items():
        if key!='routines':assert current[key]==value,key
    old=json.loads(historical('outputs/sonic-adventure/project_status.json'));status=json.loads(sp.read_text())
    for key,value in old.items():
        if key=='verified_progress':
            for field,data in value.items():assert status[key][field]==(1596 if field=='recovered_buildable_routines' else data),field
            assert set(status[key])-set(value)=={'movement_world_integration'}
        else:assert status[key]==(1596 if key=='recovered_buildable_routines' else value),key
    builds_path=P/'analysis/movement_world_previous_build_manifest.json';builds=json.loads(builds_path.read_text());assert len(builds['files'])==2737
    for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    pins={}
    for name in ('translation','connection','audit','predicate_proof','direct_verification','aliases_verification','sequence_verification','retained_verification','qualification'):
        report=json.loads((P/f'analysis/movement_world_{name}.json').read_text())
        for path,sha in report['sha256'].items():assert digest(path)==sha,(name,path);pins[path]=sha
    ps=[P/'build_movement_world.ps1']+[P/'ghidra'/('export_movement_world_'+suffix+'.ps1') for suffix in ('owner','grid','math')]
    for path in ps:
        command="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(path)
        subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
    strict=P/'analysis/movement_world_unbounded.json';u=json.loads(strict.read_text())
    sources=[P/'source'/name for name in re.findall(r"'([^']+\.c)'",(P/'build_movement_world.ps1').read_text())]
    assert len(sources)==252 and u['source_sha256']=={str(path):digest(path) for path in sources} and u['exit_code']==0
    q=json.loads(qp.read_text());assert q['new_original_routine_credit']==2 and q['fresh_original_native_comparisons']==517
    assert q['executed_original_rows']==540 and q['proven_infeasible_conditional_outcomes']==1 and len(q['unaccounted_outcomes'])==2
    files=scripts+ps+generated+[strict,qp,builds_path,P/'MOVEMENT_WORLD.txt']
    report=dict(status='verified_nearby_and_identity_workflow_world_owner_partial_full_game_unfinished',reproduced_generated_artifacts=9,
      qualification_and_registration_byte_idempotent=True,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(ps),
      pinned_inputs_current=len(pins),preserved_baseline_commit=BASE,preserved_prior_selected_files=15825,
      reviewed_mutable_paths=sorted(mutable),prior_registry_rows_preserved=1594,registry_entries=1596,
      registry_unique_module_address_pairs=1593,confirmed_linked_unique_routines_by_name=638,preserved_build_files=2737,
      additional_build_files=['work/sonic-adventure/build/movement_world.dll','work/sonic-adventure/build/movement_world.lib'],
      compiled_source_count=252,strict_unbounded_sources_checked=252,new_original_routine_credit=2,canonical_duplicate_credit=0,
      original_rows_executed=540,qualified_original_rows=295,original_outcomes_executed=83,original_outcomes_proved_infeasible=1,
      partial_owner_unexecuted_rows=q['unexecuted_rows'],partial_owner_unaccounted_outcomes=q['unaccounted_outcomes'],
      fresh_original_native_comparisons=517,fresh_independent_checks=3073,total_evidence_original_native_comparisons=72940,
      original_requirements_preserved=True,world_owner028bfe_still_unregistered=True,parent0730d8_still_unregistered=True,
      collision_caller_remaining_outcomes=[['0x8c02a48c','0x8c02a4a8']],overall_completion_percent=10,
      estimate_method='weighted milestones;not measured code coverage',sha256={str(path):digest(path) for path in files})
    (P/'analysis/movement_world_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print('WORLD WORKFLOW PASS:9artifacts;15825oldfiles/1594registryrows/2737buildspreserved;638linked;10%milestones')

if __name__=='__main__':main()
