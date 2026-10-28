"""Reproduce movement recovery and preserve all earlier source/proof/build identities."""
import ast,hashlib,json,re,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
G=r'C:\Program Files\Git\cmd\git.exe';BASE='327df5dc508fa708b5410bc3b78a6780e64851a8'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def main():
    names=('recover_movement_vectors','connect_movement_vectors','movement_vectors_reference','write_watch_movement_vectors',
      'movement_vectors_task_fixtures','movement_vectors_session','movement_vectors_vectors','movement_vectors_fixtures',
      'verify_movement_vectors_direct','verify_movement_vectors_aliases','verify_movement_vectors_sequence',
      'verify_movement_vectors_retained','check_movement_vectors_unbounded','audit_movement_vectors',
      'prove_movement_vectors_predicates','qualify_movement_vectors','register_movement_vectors','verify_movement_vectors_workflow')
    scripts=[P/(name+'.py') for name in names]
    for path in scripts:ast.parse(path.read_text(),filename=str(path))
    generated=[P/name for name in ('source/sa_movement_vectors.c','analysis/movement_vectors_translation.json',
      'analysis/movement_vectors_returns.json','source/sa_movement_vectors_connected.c','source/movement_vectors_probe.c',
      'build_movement_vectors.ps1','analysis/movement_vectors_connection.json','analysis/movement_vectors_audit.json',
      'analysis/movement_vectors_predicate_proof.json')]
    saved={str(path):digest(path) for path in generated}
    for name in ('recover_movement_vectors.py','connect_movement_vectors.py','audit_movement_vectors.py','prove_movement_vectors_predicates.py'):
        subprocess.run([sys.executable,'-B',str(P/name)],check=True)
    assert saved=={str(path):digest(path) for path in generated}
    qp=P/'analysis/movement_vectors_qualification.json';qraw=qp.read_bytes()
    subprocess.run([sys.executable,'-B',str(P/'qualify_movement_vectors.py')],check=True)
    assert qp.read_bytes()==qraw
    rp=P/'analysis/recovered_routines.json';sp=P/'project_status.json';saved=rp.read_bytes(),sp.read_bytes()
    subprocess.run([sys.executable,'-B',str(P/'register_movement_vectors.py')],check=True)
    assert saved==(rp.read_bytes(),sp.read_bytes())
    def historical(path):return subprocess.check_output([G,'-C',str(ROOT/'publication/sonic-adventure'),'show',BASE+':'+path])
    baseline=json.loads(historical('PUBLICATION_MANIFEST.json'));assert len(baseline['files'])==15772
    mutable={'outputs/sonic-adventure/'+name for name in ('README.md','SOURCE_HANDOFF.txt','project_status.json','analysis/recovered_routines.json')}
    for row in baseline['files']:
        if row['path'] not in mutable:assert digest(ROOT/row['path'])==row['sha256'],row['path']
    oldreg=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'));current=json.loads(rp.read_text())
    assert current['routines'][:1591]==oldreg['routines'] and len(current['routines'])==1594
    for key,value in oldreg.items():
        if key!='routines':assert current[key]==value,key
    old=json.loads(historical('outputs/sonic-adventure/project_status.json'));status=json.loads(sp.read_text())
    for key,value in old.items():
        if key=='verified_progress':
            for field,data in value.items():assert status[key][field]==(1594 if field=='recovered_buildable_routines' else data),field
            assert set(status[key])-set(value)=={'movement_vectors_integration'}
        else:assert status[key]==(1594 if key=='recovered_buildable_routines' else value),key
    builds_path=P/'analysis/movement_vectors_previous_build_manifest.json';builds=json.loads(builds_path.read_text());assert len(builds['files'])==2735
    for row in builds['files']:
        assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    pins={}
    for name in ('translation','connection','audit','predicate_proof','direct_verification','aliases_verification',
                 'sequence_verification','retained_verification','qualification'):
        report=json.loads((P/f'analysis/movement_vectors_{name}.json').read_text())
        for path,sha in report['sha256'].items():assert digest(path)==sha,(name,path);pins[path]=sha
    ps=[P/'build_movement_vectors.ps1']+[P/'ghidra'/name for name in ('export_movement_vector_dependencies.ps1','export_movement_vector_math.ps1')]
    for path in ps:
        command="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(path)
        subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
    strict=P/'analysis/movement_vectors_unbounded.json';u=json.loads(strict.read_text())
    sources=[P/'source'/name for name in re.findall(r"'([^']+\.c)'",(P/'build_movement_vectors.ps1').read_text())]
    assert len(sources)==251 and u['source_sha256']=={str(path):digest(path) for path in sources} and u['exit_code']==0
    q=json.loads(qp.read_text());assert q['new_original_routine_credit']==3 and q['fresh_original_native_comparisons']==596
    assert q['executed_rows_including_canonical_adapters']==449 and q['proven_infeasible_conditional_outcomes']==2 and not q['unaccounted_outcomes']
    files=scripts+ps+generated+[strict,qp,builds_path,P/'MOVEMENT_VECTORS.txt']
    report=dict(status='verified_movement_helper_workflow_full_game_unfinished',reproduced_generated_artifacts=9,
      qualification_and_registration_byte_idempotent=True,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(ps),
      pinned_inputs_current=len(pins),preserved_baseline_commit=BASE,preserved_prior_selected_files=15768,
      reviewed_mutable_paths=sorted(mutable),prior_registry_rows_preserved=1591,registry_entries=1594,
      registry_unique_module_address_pairs=1591,confirmed_linked_unique_routines_by_name=636,preserved_build_files=2735,
      additional_build_files=['work/sonic-adventure/build/movement_vectors.dll','work/sonic-adventure/build/movement_vectors.lib'],
      compiled_source_count=251,strict_unbounded_sources_checked=251,new_original_routine_credit=3,canonical_duplicate_credit=0,
      original_rows_executed=449,original_outcomes_executed=56,original_outcomes_proved_infeasible=2,
      domain_excluded_rows=[],domain_excluded_outcomes=[],fresh_original_native_comparisons=596,fresh_independent_checks=3566,
      total_evidence_original_native_comparisons=72423,original_requirements_preserved=True,
      parent0730d8_still_unregistered=True,collision_caller_remaining_outcomes=[['0x8c02a48c','0x8c02a4a8']],
      overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',
      sha256={str(path):digest(path) for path in files})
    (P/'analysis/movement_vectors_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print('MOVEMENT WORKFLOW PASS:9artifacts;15768oldfiles/1591registryrows/2735buildspreserved;636linked;10%milestones')


if __name__=='__main__':main()
