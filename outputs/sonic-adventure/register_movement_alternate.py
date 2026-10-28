"""Append one qualified original owner without rewriting earlier registry entries."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    qp=P/'analysis/movement_alternate_qualification.json';q=json.loads(qp.read_text());assert q['new_original_routine_credit']==1 and q['newly_qualified_original_rows']==202
    for name,h in q['sha256'].items():assert digest(name)==h,name
    rp=P/'analysis/recovered_routines.json';r=json.loads(rp.read_text());previous=r['routines'][:1600];assert len(previous)==1600
    routine=q['routines'][0];assert routine['address']=='0x8c075860'
    row={k:routine[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')}
    row.update(verification=qp.relative_to(P).as_posix(),status='Original alternate contact-response body software-qualified through current native ABI;all202instructions/20outcomes execute,including actual producer/history zero-length path. Physical FPU/exception/MMU/full game startup/gameplay/nonlocal resumes remain unqualified.',limitations=q['limitations'])
    old=next((x for x in r['routines'] if x.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and x['address']==row['address']),None)
    if old is None:r['routines'].append(row)
    else:assert old==row
    assert len(r['routines'])==1601 and r['routines'][:1600]==previous
    assert len({(x.get('module','1ST_READ.BIN'),x['address']) for x in r['routines']})==1598
    rp.write_text(json.dumps(r,indent=2)+'\n')
    sp=P/'project_status.json';status=json.loads(sp.read_text());data={k:v for k,v in q.items() if k not in ('sha256','routines')}
    data.update(verification=qp.relative_to(P).as_posix(),verification_sha256=digest(qp),guide='MOVEMENT_ALTERNATE.txt')
    status['verified_progress']['movement_alternate_integration']=data;status['verified_progress']['recovered_buildable_routines']=status['recovered_buildable_routines']=1601
    sp.write_text(json.dumps(status,indent=2)+'\n');print('REGISTERED1601entries/1598unique/643linked;10%milestones;goalactive')

if __name__=='__main__':main()
