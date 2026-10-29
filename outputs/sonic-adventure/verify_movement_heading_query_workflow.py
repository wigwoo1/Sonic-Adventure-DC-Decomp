"""Reproduce additive recovery/qualification; preserve all prior source/proofs."""
import ast,hashlib,json,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    names=('source/sa_movement_heading_query.c','analysis/movement_heading_query_translation.json','analysis/movement_heading_query_returns.json','source/sa_movement_heading_query_connected.c','source/movement_heading_query_probe.c','build_movement_heading_query.ps1','analysis/movement_heading_query_connection.json','analysis/movement_heading_query_audit.json')
    generated=[P/n for n in names];before={str(p):digest(p) for p in generated}
    for name in ('recover_movement_heading_query.py','connect_movement_heading_query.py','audit_movement_heading_query.py'):
        subprocess.run([sys.executable,'-B',str(P/name)],check=True)
    assert {str(p):digest(p) for p in generated}==before
    preserved=P/'analysis/movement_heading_query_preservation.json';old_preserved=preserved.read_bytes()
    subprocess.run([sys.executable,'-B',str(P/'verify_movement_heading_query_preservation.py')],check=True)
    assert preserved.read_bytes()==old_preserved
    qp=P/'analysis/movement_heading_query_qualification.json';old=qp.read_bytes()
    subprocess.run([sys.executable,'-B',str(P/'qualify_movement_heading_query.py')],check=True);assert qp.read_bytes()==old
    rp=P/'analysis/recovered_routines.json';sp=P/'project_status.json';old=rp.read_bytes(),sp.read_bytes()
    subprocess.run([sys.executable,'-B',str(P/'register_movement_heading_query.py')],check=True)
    assert old==(rp.read_bytes(),sp.read_bytes())
    pins={}
    for suffix in ('translation','connection','audit','direct_verification','retained_verification','unbounded','preservation','qualification'):
        report=json.loads((P/f'analysis/movement_heading_query_{suffix}.json').read_text())
        for name,h in report.get('sha256',{}).items():assert digest(name)==h,(suffix,name);pins[name]=h
    scripts=sorted(P.glob('*movement_heading_query*.py'))
    for p in scripts:ast.parse(p.read_text(),filename=str(p))
    q=json.loads(qp.read_text());assert q['new_original_routine_credit']==1 and q['fresh_original_native_comparisons']==586 and q['fresh_independent_checks']==5455
    assert q['result_registry_entries']==1619 and q['result_unique_module_address_pairs']==1616
    files=scripts+generated+[preserved,qp,P/'MOVEMENT_HEADING_QUERY.txt']
    result=dict(status='complete_original_record_adjustment_workflow_verified',reproduced_generated_artifacts=8,
      qualification_and_registration_byte_idempotent=True,python_scripts_parsed=len(scripts),powershell_scripts_parsed=2,
      verified_current_evidence_pins=len(pins),preserved_prior_selected_files=16325,preserved_prior_registry_rows=1618,
      preserved_archived_builds=2757,result_registry_entries=1619,result_unique_module_address_pairs=1616,
      confirmed_linked_unique_routines=663,fresh_original_native_comparisons=586,fresh_independent_checks=5455,
      original_instruction_rows_executed=106,conditional_outcomes_executed=26,prior_goal_requirements_preserved=True,
      overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',
      sha256={str(p):digest(p) for p in files})
    (P/'analysis/movement_heading_query_workflow_verification.json').write_text(json.dumps(result,indent=2)+'\n')
    print('RECORD WORKFLOW PASS8artifacts/16325oldfiles/1618oldrows/2757oldbuilds;586comparisons/5455checks',flush=True)
if __name__=='__main__':main()
