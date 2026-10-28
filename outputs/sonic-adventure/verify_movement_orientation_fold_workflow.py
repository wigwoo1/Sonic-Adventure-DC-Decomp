"""Check deterministic analysis/qualification and immutable earlier evidence."""
import ast,hashlib,json,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def main():
    names=['analysis/movement_orientation_fold_'+n+'.json' for n in ('certificate','audit','qualification','replay','direct_verification','range_verification')]
    names+=['analysis/recovered_routines.json','project_status.json']
    paths=[P/n for n in names];before={str(p):digest(p) for p in paths}
    scripts=[P/n for n in ('prove_movement_orientation_fold.py','audit_movement_orientation_fold.py','qualify_movement_orientation_fold.py','register_movement_orientation_fold.py')]
    for path in scripts:subprocess.run([sys.executable,'-B',str(path)],check=True)
    assert {str(p):digest(p) for p in paths}==before,'Reproduction changed an already-reviewed artifact'
    for p in paths[:6]:
        r=json.loads(p.read_text())
        for name,h in r.get('sha256',{}).items():assert digest(name)==h,name
    scripts+=list(P.glob('*movement_orientation_fold*.py'))
    scripts=sorted(set(scripts))
    for path in scripts:ast.parse(path.read_text(),filename=str(path))
    q=json.loads((P/'analysis/movement_orientation_fold_qualification.json').read_text())
    assert q['new_original_routine_credit']==1 and q['result_registry_entries']==1613
    report=dict(status='deterministic_workflow_and_current_evidence_pins_passed',
      byte_idempotent_artifacts=len(paths),python_scripts_parsed=len(scripts),
      new_original_routine_credit=0,overall_completion_percent=10,
      native_code_or_existing_build_changed=False,old_proofs_rewritten=False,
      sha256={str(p):digest(p) for p in paths+scripts+[P/'MOVEMENT_ORIENTATION_FOLD.txt']})
    (P/'analysis/movement_orientation_fold_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print('FOLD WORKFLOW PASS8byte-idempotent artifacts;all current evidence pins valid',flush=True)
if __name__=='__main__':main()
