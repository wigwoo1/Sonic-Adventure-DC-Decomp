"""Register four verified original routines; retain canonical identity and all earlier rows."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    qp=P/'analysis/movement_response_qualification.json';q=json.loads(qp.read_text());assert q['new_original_routine_credit']==4 and q['newly_qualified_original_rows']==373
    for name,h in q['sha256'].items():assert digest(name)==h,name
    rp=P/'analysis/recovered_routines.json';r=json.loads(rp.read_text());previous=r['routines'][:1596];assert len(previous)==1596
    for routine in q['routines']:
        assert routine['address'] in ('0x8c075416','0x8c055dac','0x8c638eac','0x8c639b50')
        row={k:routine[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')}
        row.update(verification=qp.relative_to(P).as_posix(),status='Original contact-response/math body software-qualified through current native ABI;all original rows and conditional outcomes execute,including actual alias/history zero-length path. Physical FPU/exception/MMU delivery/full game startup/gameplay/nonlocal resumes remain unqualified.',limitations=q['limitations'])
        old=next((x for x in r['routines'] if x.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and x['address']==row['address']),None)
        if old is None:r['routines'].append(row)
        else:assert old==row
    assert len(r['routines'])==1600 and r['routines'][:1596]==previous
    assert len({(x.get('module','1ST_READ.BIN'),x['address']) for x in r['routines']})==1597
    rp.write_text(json.dumps(r,indent=2)+'\n')
    sp=P/'project_status.json';status=json.loads(sp.read_text());data={k:v for k,v in q.items() if k not in ('sha256','routines')}
    data.update(verification=qp.relative_to(P).as_posix(),verification_sha256=digest(qp),guide='MOVEMENT_RESPONSE.txt')
    status['verified_progress']['movement_response_integration']=data;status['verified_progress']['recovered_buildable_routines']=status['recovered_buildable_routines']=1600
    sp.write_text(json.dumps(status,indent=2)+'\n');print('REGISTERED1600entries/1597unique/642linked;10%milestones;goalactive')

if __name__=='__main__':main()
