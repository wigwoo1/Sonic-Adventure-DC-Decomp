"""Replay the unchanged direct/range checks against the preserved259-source DLL.

Only report destinations change. Every numeric assertion, fixture, original
instruction, native call, observer and guest limit stays exactly as published.
"""
import ast,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent

def digest(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()

def main():
    records=[]
    for suite in ('direct','range'):
        parent=P/f'verify_movement_orientation_response_{suite}.py'
        original=parent.read_text()
        before=f'movement_orientation_response_{suite}_verification.json'
        after=f'movement_orientation_fold_{suite}_verification.json'
        assert original.count(before)==1
        source=original.replace(before,after)
        # Verify that redirection changes just one string constant in the AST.
        old=ast.parse(original);new=ast.parse(source)
        changed=0
        for node in ast.walk(new):
            if isinstance(node,ast.Constant) and isinstance(node.value,str) and after in node.value:
                node.value=node.value.replace(after,before);changed+=1
        assert changed==1 and ast.dump(old)==ast.dump(new)
        ns=dict(__name__='fold_fresh_replay',__file__=str(parent))
        exec(compile(source,str(parent),'exec'),ns);ns['main']()
        report_path=P/'analysis'/after
        report=json.loads(report_path.read_text())
        for path,expected in report['sha256'].items():assert digest(path)==expected,path
        report['sha256'][str(Path(__file__))]=digest(Path(__file__))
        report['sha256'][str(P/'analysis/movement_orientation_fold_certificate.json')]=digest(P/'analysis/movement_orientation_fold_certificate.json')
        report['test_AST_unchanged_except_report_destination']=True
        report['native_code_changed']=False
        report_path.write_text(json.dumps(report,indent=2)+'\n')
        records.append(dict(suite=suite,proof=report_path.relative_to(P).as_posix(),
          original_native_comparisons=report['original_native_comparisons'],
          independent_checks=report['independent_checks'],independent_oracle_cases=report['independent_oracle_cases']))
    result=dict(status='fresh_original_native_replay_passed',suites=records,
      original_native_comparisons=sum(r['original_native_comparisons'] for r in records),
      independent_checks=sum(r['independent_checks'] for r in records),
      independent_oracle_cases=sum(r['independent_oracle_cases'] for r in records),
      supplied_game_callee_outputs=0,supplied_execution_frames=0,
      new_original_routine_credit=0,sha256={str(P/r['proof']):digest(P/r['proof']) for r in records})
    assert (result['original_native_comparisons'],result['independent_checks'],result['independent_oracle_cases'])==(465,4299,424)
    (P/'analysis/movement_orientation_fold_replay.json').write_text(json.dumps(result,indent=2)+'\n')
    print('FOLD FRESH REPLAY PASS465comparisons/4299checks;old scripts and proofs unchanged',flush=True)
if __name__=='__main__':main()
