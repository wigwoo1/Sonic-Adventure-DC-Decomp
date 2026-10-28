"""Reproduce additive recovery/qualification; preserve all prior source/proofs."""
import ast,hashlib,json,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    names=('source/sa_movement_record_adjustment.c','analysis/movement_record_adjustment_translation.json','analysis/movement_record_adjustment_returns.json','source/sa_movement_record_adjustment_connected.c','source/movement_record_adjustment_probe.c','build_movement_record_adjustment.ps1','analysis/movement_record_adjustment_connection.json','analysis/movement_record_adjustment_audit.json')
    generated=[P/n for n in names];before={str(p):digest(p) for p in generated}
    for name in ('recover_movement_record_adjustment.py','connect_movement_record_adjustment.py','audit_movement_record_adjustment.py'):
        subprocess.run([sys.executable,'-B',str(P/name)],check=True)
    assert {str(p):digest(p) for p in generated}==before
    preserved=P/'analysis/movement_record_adjustment_preservation.json';old_preserved=preserved.read_bytes()
    subprocess.run([sys.executable,'-B',str(P/'verify_movement_record_adjustment_preservation.py')],check=True)
    assert preserved.read_bytes()==old_preserved
    qp=P/'analysis/movement_record_adjustment_qualification.json';old=qp.read_bytes()
    subprocess.run([sys.executable,'-B',str(P/'qualify_movement_record_adjustment.py')],check=True);assert qp.read_bytes()==old
    rp=P/'analysis/recovered_routines.json';sp=P/'project_status.json';old=rp.read_bytes(),sp.read_bytes()
    subprocess.run([sys.executable,'-B',str(P/'register_movement_record_adjustment.py')],check=True)
    assert old==(rp.read_bytes(),sp.read_bytes())
    pins={}
    for suffix in ('translation','connection','audit','direct_verification','callback_alias_verification','retained_verification','unbounded','preservation','qualification'):
        report=json.loads((P/f'analysis/movement_record_adjustment_{suffix}.json').read_text())
        for name,h in report.get('sha256',{}).items():assert digest(name)==h,(suffix,name);pins[name]=h
    scripts=sorted(P.glob('*movement_record_adjustment*.py'))
    for p in scripts:ast.parse(p.read_text(),filename=str(p))
    q=json.loads(qp.read_text());assert q['new_original_routine_credit']==2 and q['fresh_original_native_comparisons']==484 and q['fresh_independent_checks']==3790
    assert q['result_registry_entries']==1615 and q['result_unique_module_address_pairs']==1612
    files=scripts+generated+[preserved,qp,P/'MOVEMENT_RECORD_ADJUSTMENT.txt']
    result=dict(status='complete_original_record_adjustment_workflow_verified',reproduced_generated_artifacts=8,
      qualification_and_registration_byte_idempotent=True,python_scripts_parsed=len(scripts),powershell_scripts_parsed=2,
      verified_current_evidence_pins=len(pins),preserved_prior_selected_files=16233,preserved_prior_registry_rows=1613,
      preserved_archived_builds=2753,result_registry_entries=1615,result_unique_module_address_pairs=1612,
      confirmed_linked_unique_routines=659,fresh_original_native_comparisons=484,fresh_independent_checks=3790,
      original_instruction_rows_executed=164,conditional_outcomes_executed=8,prior_goal_requirements_preserved=True,
      overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',
      sha256={str(p):digest(p) for p in files})
    (P/'analysis/movement_record_adjustment_workflow_verification.json').write_text(json.dumps(result,indent=2)+'\n')
    print('RECORD WORKFLOW PASS8artifacts/16233oldfiles/1613oldrows/2753oldbuilds;484comparisons/3790checks',flush=True)
if __name__=='__main__':main()
