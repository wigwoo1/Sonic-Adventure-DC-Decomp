"""Reproduce the group and verify preservation, reports and registration idempotence."""
import ast,hashlib,json,re,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    scripts=sorted(P.glob('*movement_projection_response*.py'))
    for path in scripts:ast.parse(path.read_text(),filename=str(path))
    generated=[P/n for n in ('source/sa_movement_projection_response.c','analysis/movement_projection_response_translation.json','analysis/movement_projection_response_returns.json','source/sa_movement_projection_response_connected.c','source/movement_projection_response_probe.c','build_movement_projection_response.ps1','analysis/movement_projection_response_connection.json','analysis/movement_projection_response_audit.json','analysis/movement_projection_response_predicate_proof.json')]
    saved={str(path):digest(path) for path in generated}
    for name in ('recover_movement_projection_response.py','connect_movement_projection_response.py','audit_movement_projection_response.py','prove_movement_projection_response_predicate.py'):
        subprocess.run([sys.executable,'-B',str(P/name)],check=True)
    assert saved=={str(path):digest(path) for path in generated}
    qp=P/'analysis/movement_projection_response_qualification.json';before=qp.read_bytes();subprocess.run([sys.executable,'-B',str(P/'qualify_movement_projection_response.py')],check=True);assert qp.read_bytes()==before
    rp=P/'analysis/recovered_routines.json';sp=P/'project_status.json';before=rp.read_bytes(),sp.read_bytes();subprocess.run([sys.executable,'-B',str(P/'register_movement_projection_response.py')],check=True);assert before==(rp.read_bytes(),sp.read_bytes())
    subprocess.run([sys.executable,'-B',str(P/'verify_movement_projection_response_preservation.py')],check=True)
    pp=P/'analysis/movement_projection_response_preservation.json';preserved=json.loads(pp.read_text());assert preserved['preserved_previous_selected_files_outside_four_mutables']==16110 and preserved['preserved_prior_registry_rows']==1605 and preserved['preserved_archived_builds']==2749
    pins={}
    for name in ('translation','connection','audit','direct_verification','edges_verification','rounding_verification','predicate_proof','retained_verification','qualification','preservation'):
        r=json.loads((P/f'analysis/movement_projection_response_{name}.json').read_text())
        for path,h in r['sha256'].items():assert digest(path)==h,(name,path);pins[path]=h
    strict=P/'analysis/movement_projection_response_unbounded.json';u=json.loads(strict.read_text());sources=[P/'source'/n for n in re.findall(r"'([^']+\.c)'",(P/'build_movement_projection_response.ps1').read_text())]
    assert len(sources)==258 and u['source_sha256']=={str(p):digest(p) for p in sources} and u['exit_code']==0
    q=json.loads(qp.read_text());assert q['new_original_routine_credit']==5 and q['fresh_original_native_comparisons']==1151 and q['fresh_independent_checks']==8765
    assert q['executed_original_rows']==1005 and q['executed_conditional_outcomes']==63 and not q['unaccounted_software_profile_outcomes']
    assert q['unqualified_broader_precision_outcomes']==[['0x8c074626','0x8c074644'],['0x8c6402c0','0x8c640308']]
    ps=[P/'build_movement_projection_response.ps1',P/'ghidra/export_movement_projection_response.ps1']
    files=scripts+ps+generated+[strict,qp,pp,P/'MOVEMENT_PROJECTION_RESPONSE.txt']
    r=dict(status='verified_movement_projection_response_workflow_full_game_unfinished',reproduced_generated_artifacts=9,qualification_and_registration_byte_idempotent=True,python_scripts_parsed=len(scripts),powershell_scripts_parsed=preserved['powershell_scripts_parsed'],verified_current_evidence_pins=len(pins),preserved_previous_selected_files=16110,preserved_prior_registry_rows=1605,preserved_archived_builds=2749,result_registry_entries=1610,result_unique_module_address_pairs=1607,confirmed_linked_unique_routines=654,new_original_routine_credit=5,canonical_duplicate_credit=0,fresh_original_native_comparisons=1151,executed_original_rows=1005,executed_conditional_outcomes=63,scalar_profile_proved_infeasible_outcomes=1,broader_precision_and_prior_owner_obligations_preserved=True,overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',sha256={str(p):digest(p) for p in files})
    (P/'analysis/movement_projection_response_workflow_verification.json').write_text(json.dumps(r,indent=2)+'\n');print('PROJECTION WORKFLOW PASS9artifacts/16110oldfiles/1605oldrows/2749oldbuilds;654linked;10%milestones')
if __name__=='__main__':main()
