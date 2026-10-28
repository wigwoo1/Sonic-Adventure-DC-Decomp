"""Append current partial evidence while retaining historical progress and registry."""
import hashlib
import json
from pathlib import Path

P=Path(__file__).resolve().parent


def main():
    path=P/'analysis/collision_contact_counter_alias_qualification.json'
    q=json.loads(path.read_text())
    assert q['executed_conditional_outcomes']==73 and q['new_original_routine_credit']==0
    for name,sha in q['sha256'].items():
        assert hashlib.sha256(Path(name).read_bytes()).hexdigest()==sha,name
    status=P/'project_status.json'
    report=json.loads(status.read_text())
    data={key:value for key,value in q.items() if key!='sha256'}
    data.update(verification=path.relative_to(P).as_posix(),
                verification_sha256=hashlib.sha256(path.read_bytes()).hexdigest(),
                guide='COLLISION_CONTACT_COUNTER_ALIAS.txt')
    report['verified_progress']['collision_contact_counter_alias']=data
    status.write_text(json.dumps(report,indent=2)+'\n')
    print('Partial collision alias evidence recorded;historical progress and registry retained;10%milestones')


if __name__=='__main__':
    main()
