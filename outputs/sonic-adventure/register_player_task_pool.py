"""Append the verified task-pool entry; retain all1626 previous registry rows."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    qp=P/'analysis/player_task_pool_qualification.json';q=json.loads(qp.read_text())
    assert q['new_original_routine_credit']==1 and q['newly_qualified_original_rows']==114
    for path,expected in q['sha256'].items():assert sha(path)==expected,path
    rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text());prior=registry['routines'][:1626];assert len(prior)==1626
    for body in q['routines']:
        row={k:body[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')}
        row.update(verification=qp.relative_to(P).as_posix(),status='Complete original task-pool initializer software-qualified:111rows/12outcomes execute;3rows/4outcomes proven infeasible for normal immutable synchronous original-entry execution. Fullgame,physicalhardware and arbitrary nonlocal resumes remain unqualified.',limitations=q['limitations'])
        old=next((x for x in registry['routines'] if x.get('module','1ST_READ.BIN')==row['module'] and x['address']==row['address']),None)
        if old is None:registry['routines'].append(row)
        else:assert old==row
    assert len(registry['routines'])==1627 and registry['routines'][:1626]==prior
    assert len({(x.get('module','1ST_READ.BIN'),x['address']) for x in registry['routines']})==1624
    rp.write_text(json.dumps(registry,indent=2)+'\n')
    sp=P/'project_status.json';status=json.loads(sp.read_text())
    data={k:v for k,v in q.items() if k not in ('sha256','routines')};data.update(verification=qp.relative_to(P).as_posix(),verification_sha256=sha(qp),guide='PLAYER_TASK_POOL.txt')
    status['verified_progress']['player_task_pool_integration']=data
    status['verified_progress']['recovered_buildable_routines']=status['recovered_buildable_routines']=1627
    sp.write_text(json.dumps(status,indent=2)+'\n')
    print('REGISTERED1627entries/1624unique module-addresspairs/671confirmedlinks;10%milestones;goalACTIVE',flush=True)
if __name__=='__main__':main()
