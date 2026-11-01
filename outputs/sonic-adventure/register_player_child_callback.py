"""Append two qualified child initializer/query originals; preserve canonical entries."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    qp=P/'analysis/player_child_callback_qualification.json';q=json.loads(qp.read_text());assert q['new_original_routine_credit']==2 and q['canonical_duplicate_credit']==0
    for p,value in q['sha256'].items():assert sha(p)==value,p
    rp=P/'analysis/recovered_routines.json';reg=json.loads(rp.read_text());prior=reg['routines'][:1634];assert len(prior)==1634
    for b in q['routines']:
        row={k:b[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')}
        row.update(verification=qp.relative_to(P).as_posix(),status='Complete original child initializer or character query software-qualified;all68group rows/7feasible outcomes execute;one original constant-return outcome proven infeasible. Exact state/argument/table checks and16real allocated-child lifecycles. Subsequent callbacks and retail startup remain unqualified.',limitations=q['limitations'])
        old=next((x for x in reg['routines'] if x.get('module','1ST_READ.BIN')==row['module'] and x['address']==row['address']),None)
        if old is None:reg['routines'].append(row)
        else:assert old==row
    assert len(reg['routines'])==1636 and reg['routines'][:1634]==prior
    assert len({(x.get('module','1ST_READ.BIN'),x['address']) for x in reg['routines']})==1633
    rp.write_text(json.dumps(reg,indent=2)+'\n')
    sp=P/'project_status.json';status=json.loads(sp.read_text());data={k:v for k,v in q.items() if k not in ('sha256','routines')}
    data.update(verification=qp.relative_to(P).as_posix(),verification_sha256=sha(qp),guide='PLAYER_CHILD_CALLBACK.txt')
    status['verified_progress']['player_child_callback_integration']=data
    status['verified_progress']['recovered_buildable_routines']=status['recovered_buildable_routines']=1636
    sp.write_text(json.dumps(status,indent=2)+'\n');print('REGISTERED1636entries/1633unique/680confirmedlinks;canonical reuse zero duplicate credit;goal active',flush=True)
if __name__=='__main__':main()
