"""Append only qualified collision child and line wrapper originals."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    qp=P/'analysis/collision_response_qualification.json';q=json.loads(qp.read_text());assert q['new_original_routine_credit']==2
    for p,value in q['sha256'].items():assert sha(p)==value,p
    rp=P/'analysis/recovered_routines.json';reg=json.loads(rp.read_text());prior=reg['routines'][:1632];assert len(prior)==1632
    for b in q['routines']:
        row={k:b[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')}
        row.update(verification=qp.relative_to(P).as_posix(),status='Complete original collision child or line wrapper software-qualified;all526group rows/52outcomes execute. Original/native full-state comparisons,independent output/argument checks and real renderer calls. Larger response caller validity invariant and physical hardware remain unqualified.',limitations=q['limitations'])
        old=next((x for x in reg['routines'] if x.get('module','1ST_READ.BIN')==row['module'] and x['address']==row['address']),None)
        if old is None:reg['routines'].append(row)
        else:assert old==row
    assert len(reg['routines'])==1634 and reg['routines'][:1632]==prior
    assert len({(x.get('module','1ST_READ.BIN'),x['address']) for x in reg['routines']})==1631
    rp.write_text(json.dumps(reg,indent=2)+'\n')
    sp=P/'project_status.json';status=json.loads(sp.read_text());data={k:v for k,v in q.items() if k not in ('sha256','routines')}
    data.update(verification=qp.relative_to(P).as_posix(),verification_sha256=sha(qp),guide='COLLISION_RESPONSE_QUALIFICATION.txt')
    status['verified_progress']['collision_response_qualification']=data
    status['verified_progress']['recovered_buildable_routines']=status['recovered_buildable_routines']=1634
    sp.write_text(json.dumps(status,indent=2)+'\n');print('REGISTERED1634entries/1631unique/678confirmedlinks;larger response unregistered;goal active',flush=True)
if __name__=='__main__':main()
