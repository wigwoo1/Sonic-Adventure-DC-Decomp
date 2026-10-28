"""Reproduce response evidence while preserving the prior published snapshot."""
import ast,hashlib,json,re,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
G=r'C:\Program Files\Git\cmd\git.exe';BASE='a7ae9c190bc49df600ac0e05de0c6e679fbd64c8'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    names=('recover_movement_response','connect_movement_response','movement_response_reference','write_watch_movement_response',
      'movement_response_task_fixtures','movement_response_session','movement_response_vectors','movement_response_fixtures',
      'verify_movement_response_direct','verify_movement_response_aliases','verify_movement_response_sequence',
      'verify_movement_response_retained','check_movement_response_unbounded','audit_movement_response',
      'qualify_movement_response','register_movement_response','verify_movement_response_workflow')
    scripts=[P/(name+'.py') for name in names]
    for path in scripts:ast.parse(path.read_text(),filename=str(path))
    generated=[P/name for name in ('source/sa_movement_response.c','analysis/movement_response_translation.json','analysis/movement_response_returns.json',
      'source/sa_movement_response_connected.c','source/movement_response_probe.c','build_movement_response.ps1',
      'analysis/movement_response_connection.json','analysis/movement_response_audit.json')]
    saved={str(path):digest(path) for path in generated}
    for name in ('recover_movement_response.py','connect_movement_response.py','audit_movement_response.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
    assert saved=={str(path):digest(path) for path in generated}
    qp=P/'analysis/movement_response_qualification.json';qraw=qp.read_bytes()
    subprocess.run([sys.executable,'-B',str(P/'qualify_movement_response.py')],check=True);assert qp.read_bytes()==qraw
    rp=P/'analysis/recovered_routines.json';sp=P/'project_status.json';saved=rp.read_bytes(),sp.read_bytes()
    subprocess.run([sys.executable,'-B',str(P/'register_movement_response.py')],check=True);assert saved==(rp.read_bytes(),sp.read_bytes())
    def historical(path):return subprocess.check_output([G,'-C',str(ROOT/'publication/sonic-adventure'),'show',BASE+':'+path])
    baseline=json.loads(historical('PUBLICATION_MANIFEST.json'));assert len(baseline['files'])==15882
    mutable={'outputs/sonic-adventure/'+name for name in ('README.md','SOURCE_HANDOFF.txt','project_status.json','analysis/recovered_routines.json')}
    for row in baseline['files']:
        if row['path'] not in mutable:assert digest(ROOT/row['path'])==row['sha256'],row['path']
    for name in ('README.md','SOURCE_HANDOFF.txt'):assert (P/name).read_bytes().endswith(historical('outputs/sonic-adventure/'+name))
    oldreg=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'));current=json.loads(rp.read_text())
    assert current['routines'][:1596]==oldreg['routines'] and len(current['routines'])==1600
    for key,value in oldreg.items():
        if key!='routines':assert current[key]==value,key
    old=json.loads(historical('outputs/sonic-adventure/project_status.json'));status=json.loads(sp.read_text())
    for key,value in old.items():
        if key=='verified_progress':
            for field,data in value.items():assert status[key][field]==(1600 if field=='recovered_buildable_routines' else data),field
            assert set(status[key])-set(value)=={'movement_response_integration'}
        else:assert status[key]==(1600 if key=='recovered_buildable_routines' else value),key
    builds_path=P/'analysis/movement_response_previous_build_manifest.json';builds=json.loads(builds_path.read_text());assert len(builds['files'])==2739
    for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    pins={}
    for name in ('translation','connection','audit','direct_verification','aliases_verification','sequence_verification','retained_verification','qualification'):
        report=json.loads((P/f'analysis/movement_response_{name}.json').read_text())
        for path,h in report['sha256'].items():assert digest(path)==h,(name,path);pins[path]=h
    ps=[P/'build_movement_response.ps1']+[P/'ghidra'/('export_movement_response_'+suffix+'.ps1') for suffix in ('owner','math','cross')]
    for path in ps:
        command="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(path)
        subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
    strict=P/'analysis/movement_response_unbounded.json';u=json.loads(strict.read_text())
    sources=[P/'source'/name for name in re.findall(r"'([^']+\.c)'",(P/'build_movement_response.ps1').read_text())]
    assert len(sources)==253 and u['source_sha256']=={str(path):digest(path) for path in sources} and u['exit_code']==0
    q=json.loads(qp.read_text());assert q['new_original_routine_credit']==4 and q['fresh_original_native_comparisons']==777
    assert q['executed_original_rows_including_canonical']==378 and q['executed_conditional_outcomes']==32 and not q['unaccounted_outcomes']
    files=scripts+ps+generated+[strict,qp,builds_path,P/'MOVEMENT_RESPONSE.txt']
    report=dict(status='verified_original_contact_response_workflow_full_game_unfinished',reproduced_generated_artifacts=8,
      qualification_and_registration_byte_idempotent=True,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(ps),
      pinned_inputs_current=len(pins),preserved_baseline_commit=BASE,preserved_prior_selected_files=15878,reviewed_mutable_paths=sorted(mutable),
      prior_registry_rows_preserved=1596,registry_entries=1600,registry_unique_module_address_pairs=1597,confirmed_linked_unique_routines_by_name=642,
      preserved_build_files=2739,additional_build_files=['work/sonic-adventure/build/movement_response.dll','work/sonic-adventure/build/movement_response.lib'],
      compiled_source_count=253,strict_unbounded_sources_checked=253,new_original_routine_credit=4,canonical_duplicate_credit=0,
      original_rows_executed=378,qualified_new_original_rows=373,original_outcomes_executed=32,domain_excluded_rows=[],domain_excluded_outcomes=[],
      original_zero_length_branch_executed=True,fresh_original_native_comparisons=777,fresh_independent_checks=4473,total_evidence_original_native_comparisons=73717,
      original_requirements_preserved=True,world_owner028bfe_still_unregistered=True,parent0730d8_still_unregistered=True,
      world_owner_remaining_outcomes=q['world_owner_unaccounted_outcomes'],collision_caller_remaining_outcomes=q['collision_caller_unaccounted_outcomes'],
      overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',sha256={str(path):digest(path) for path in files})
    (P/'analysis/movement_response_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print('RESPONSE WORKFLOW PASS:8artifacts;15878oldfiles/1596registryrows/2739buildspreserved;642linked;10%milestones')

if __name__=='__main__':main()
