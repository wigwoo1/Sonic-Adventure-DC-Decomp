"""Append the qualified movement/model group without editing previous rows."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    qp=P/'analysis/movement_model_response_qualification.json';q=json.loads(qp.read_text());assert q['new_original_routine_credit']==3 and q['newly_qualified_original_rows']==906
    for name,h in q['sha256'].items():assert digest(name)==h,name
    rp=P/'analysis/recovered_routines.json';r=json.loads(rp.read_text());previous=r['routines'][:1602];assert len(previous)==1602
    for routine in q['routines']:
        row={k:routine[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')}
        row.update(verification=qp.relative_to(P).as_posix(),status='Original movement/model group software-qualified under explicit current scalar PR0/SZ0 contract;904of906group rows execute,2integerselector rows provedunreachable;one further numerical outcome proved onlywithinthatprofile. Broaderprecision/physicalhardware/fullgame remainunqualified.',limitations=q['limitations'])
        old=next((x for x in r['routines'] if x.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and x['address']==row['address']),None)
        if old is None:r['routines'].append(row)
        else:assert old==row
    assert len(r['routines'])==1605 and r['routines'][:1602]==previous
    assert len({(x.get('module','1ST_READ.BIN'),x['address']) for x in r['routines']})==1602
    rp.write_text(json.dumps(r,indent=2)+'\n')
    sp=P/'project_status.json';status=json.loads(sp.read_text());data={k:v for k,v in q.items() if k not in ('sha256','routines')}
    data.update(verification=qp.relative_to(P).as_posix(),verification_sha256=digest(qp),guide='MOVEMENT_MODEL_RESPONSE.txt')
    status['verified_progress']['movement_model_response_integration']=data;status['verified_progress']['recovered_buildable_routines']=status['recovered_buildable_routines']=1605
    sp.write_text(json.dumps(status,indent=2)+'\n');print('REGISTERED1605entries/1602unique/648linked;10%milestones;goalactive')

if __name__=='__main__':main()
