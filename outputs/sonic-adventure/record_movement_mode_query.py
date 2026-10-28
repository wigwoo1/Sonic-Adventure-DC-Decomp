"""Record canonical integration without rewriting the existing routine registry."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    qp=P/'analysis/movement_mode_query_qualification.json';q=json.loads(qp.read_text())
    assert q['new_original_routine_credit']==q['newly_registered_entries']==0 and q['newly_qualified_canonical_adapter_rows']==6
    for name,h in q['sha256'].items():assert digest(name)==h,name
    rp=P/'analysis/recovered_routines.json';before=rp.read_bytes();r=json.loads(before)
    assert len(r['routines'])==1602 and len({(x.get('module','1ST_READ.BIN'),x['address']) for x in r['routines']})==1599
    sp=P/'project_status.json';status=json.loads(sp.read_text());assert status['recovered_buildable_routines']==status['verified_progress']['recovered_buildable_routines']==1602
    data={k:v for k,v in q.items() if k not in ('sha256','routines')};data.update(verification=qp.relative_to(P).as_posix(),verification_sha256=digest(qp),guide='MOVEMENT_MODE_QUERY.txt')
    status['verified_progress']['movement_mode_query_integration']=data;sp.write_text(json.dumps(status,indent=2)+'\n')
    assert rp.read_bytes()==before;print('RECORDED existingcanonical049a4e integration;registry1602/1599unchanged;645linked;10%milestones')

if __name__=='__main__':main()
