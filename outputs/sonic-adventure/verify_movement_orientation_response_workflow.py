"""Reproduce helper qualification without promoting the unverified owner branch."""
import ast,hashlib,json,re,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    scripts=sorted(P.glob('*movement_orientation*.py'))
    for path in scripts:ast.parse(path.read_text(),filename=str(path))
    generated=[P/n for n in ('source/sa_movement_orientation_response.c','analysis/movement_orientation_response_translation.json','analysis/movement_orientation_response_returns.json','source/sa_movement_orientation_response_connected.c','source/movement_orientation_response_probe.c','build_movement_orientation_response.ps1','analysis/movement_orientation_response_connection.json','analysis/movement_orientation_response_audit.json','analysis/movement_orientation_response_angle_fold_obligation.json')]
    saved={str(path):digest(path) for path in generated}
    for name in ('recover_movement_orientation_response.py','connect_movement_orientation_response.py','audit_movement_orientation_response.py','audit_movement_orientation_angle_fold.py'):
        subprocess.run([sys.executable,'-B',str(P/name)],check=True)
    assert saved=={str(path):digest(path) for path in generated}
    qp=P/'analysis/movement_orientation_response_qualification.json';before=qp.read_bytes();subprocess.run([sys.executable,'-B',str(P/'qualify_movement_orientation_response.py')],check=True);assert qp.read_bytes()==before
    rp=P/'analysis/recovered_routines.json';sp=P/'project_status.json';before=rp.read_bytes(),sp.read_bytes();subprocess.run([sys.executable,'-B',str(P/'register_movement_orientation_response.py')],check=True);assert before==(rp.read_bytes(),sp.read_bytes())
    subprocess.run([sys.executable,'-B',str(P/'verify_movement_orientation_response_preservation.py')],check=True)
    pp=P/'analysis/movement_orientation_response_preservation.json';preserved=json.loads(pp.read_text());assert preserved['preserved_previous_selected_files_outside_four_mutables']==16167 and preserved['preserved_prior_registry_rows']==1610 and preserved['preserved_archived_builds']==2751
    pins={}
    for name in ('translation','connection','audit','direct_verification','range_verification','angle_fold_obligation','retained_verification','qualification','preservation'):
        r=json.loads((P/f'analysis/movement_orientation_response_{name}.json').read_text())
        for path,h in r['sha256'].items():assert digest(path)==h,(name,path);pins[path]=h
    strict=P/'analysis/movement_orientation_response_unbounded.json';u=json.loads(strict.read_text());sources=[P/'source'/n for n in re.findall(r"'([^']+\.c)'",(P/'build_movement_orientation_response.ps1').read_text())]
    assert len(sources)==259 and u['source_sha256']=={str(p):digest(p) for p in sources} and u['exit_code']==0
    q=json.loads(qp.read_text());assert q['new_original_routine_credit']==2 and q['fresh_original_native_comparisons']==708 and q['fresh_independent_checks']==5982
    assert q['executed_original_rows']==233 and q['executed_conditional_outcomes']==35 and q['unregistered_owner']=='0x8c074ee4' and q['proposed_owner_new_routine_credit']==0
    assert not q['angle_fold_range_proved'] and q['unqualified_original_outcomes']==[['0x8c074fd4','0x8c074fd6']]
    assert q['unqualified_broader_precision_outcomes']==[['0x8c074626','0x8c074644'],['0x8c6402c0','0x8c640308']]
    registry=json.loads(rp.read_text())['routines'];assert not any(x['address']=='0x8c074ee4' and x.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for x in registry)
    ps=[P/'build_movement_orientation_response.ps1',P/'ghidra/export_movement_orientation_response.ps1'];files=scripts+ps+generated+[strict,qp,pp,P/'MOVEMENT_ORIENTATION_RESPONSE.txt']
    r=dict(status='verified_helper_workflow_orientation_owner_gate_still_open',reproduced_generated_artifacts=9,qualification_and_registration_byte_idempotent=True,python_scripts_parsed=len(scripts),powershell_scripts_parsed=preserved['powershell_scripts_parsed'],verified_current_evidence_pins=len(pins),preserved_previous_selected_files=16167,preserved_prior_registry_rows=1610,preserved_archived_builds=2751,result_registry_entries=1612,result_unique_module_address_pairs=1609,confirmed_linked_unique_routines=656,new_original_routine_credit=2,owner_new_original_routine_credit=0,canonical_duplicate_credit=0,fresh_original_native_comparisons=708,executed_original_rows=233,executed_conditional_outcomes=35,unregistered_owner='0x8c074ee4',unqualified_owner_outcomes=q['unqualified_original_outcomes'],prior_obligations_and_goal_requirements_preserved=True,overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',sha256={str(p):digest(p) for p in files})
    (P/'analysis/movement_orientation_response_workflow_verification.json').write_text(json.dumps(r,indent=2)+'\n');print('ORIENTATION WORKFLOW PASS9artifacts/16167oldfiles/1610oldrows/2751oldbuilds;2helpers;OWNERgateOPEN;10%milestones')
if __name__=='__main__':main()
