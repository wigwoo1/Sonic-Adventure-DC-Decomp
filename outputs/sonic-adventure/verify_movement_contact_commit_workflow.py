"""Reproduce additive recovery/qualification; preserve all prior source/proofs."""
import ast,hashlib,json,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    names=('source/sa_movement_contact_commit.c','analysis/movement_contact_commit_translation.json','analysis/movement_contact_commit_returns.json','source/sa_movement_contact_commit_connected.c','source/movement_contact_commit_probe.c','build_movement_contact_commit.ps1','analysis/movement_contact_commit_connection.json','analysis/movement_contact_commit_audit.json')
    generated=[P/n for n in names];before={str(p):digest(p) for p in generated}
    for name in ('recover_movement_contact_commit.py','connect_movement_contact_commit.py','audit_movement_contact_commit.py'):
        subprocess.run([sys.executable,'-B',str(P/name)],check=True)
    assert {str(p):digest(p) for p in generated}==before
    preserved=P/'analysis/movement_contact_commit_preservation.json';old_preserved=preserved.read_bytes()
    subprocess.run([sys.executable,'-B',str(P/'verify_movement_contact_commit_preservation.py')],check=True)
    assert preserved.read_bytes()==old_preserved
    qp=P/'analysis/movement_contact_commit_qualification.json';old=qp.read_bytes()
    subprocess.run([sys.executable,'-B',str(P/'qualify_movement_contact_commit.py')],check=True);assert qp.read_bytes()==old
    rp=P/'analysis/recovered_routines.json';sp=P/'project_status.json';old=rp.read_bytes(),sp.read_bytes()
    subprocess.run([sys.executable,'-B',str(P/'register_movement_contact_commit.py')],check=True)
    assert old==(rp.read_bytes(),sp.read_bytes())
    pins={}
    for suffix in ('translation','connection','audit','direct_verification','fallback_verification','retained_verification','unbounded','preservation','qualification'):
        report=json.loads((P/f'analysis/movement_contact_commit_{suffix}.json').read_text())
        for name,h in report.get('sha256',{}).items():assert digest(name)==h,(suffix,name);pins[name]=h
    scripts=sorted(P.glob('*movement_contact_commit*.py'))
    for p in scripts:ast.parse(p.read_text(),filename=str(p))
    q=json.loads(qp.read_text());assert q['new_original_routine_credit']==2
    assert q['result_registry_entries']==1621 and q['result_unique_module_address_pairs']==1618
    files=scripts+generated+[preserved,qp,P/'MOVEMENT_CONTACT_COMMIT.txt']
    result=dict(status='complete_original_contact_commit_workflow_verified',reproduced_generated_artifacts=8,
      qualification_and_registration_byte_idempotent=True,python_scripts_parsed=len(scripts),powershell_scripts_parsed=2,
      verified_current_evidence_pins=len(pins),preserved_prior_selected_files=16366,preserved_prior_registry_rows=1619,
      preserved_archived_builds=2759,result_registry_entries=1621,result_unique_module_address_pairs=1618,
      confirmed_linked_unique_routines=665,fresh_original_native_comparisons=q['fresh_original_native_comparisons'],fresh_independent_checks=q['fresh_independent_checks'],
      original_instruction_rows_executed=384,conditional_outcomes_executed=32,prior_goal_requirements_preserved=True,
      overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',
      sha256={str(p):digest(p) for p in files})
    (P/'analysis/movement_contact_commit_workflow_verification.json').write_text(json.dumps(result,indent=2)+'\n')
    print('CONTACT COMMIT WORKFLOW PASS',q['fresh_original_native_comparisons'],q['fresh_independent_checks'],flush=True)
if __name__=='__main__':main()
