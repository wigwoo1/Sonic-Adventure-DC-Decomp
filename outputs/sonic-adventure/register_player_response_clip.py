"""Append only the qualified line clipping original; preserve all prior rows."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    qp=P/'analysis/player_response_clip_qualification.json';q=json.loads(qp.read_text())
    assert q['new_original_routine_credit']==1 and q['newly_qualified_original_rows']==296
    for p,value in q['sha256'].items():assert sha(p)==value,p
    rp=P/'analysis/recovered_routines.json';reg=json.loads(rp.read_text());prior=reg['routines'][:1631];assert len(prior)==1631
    for body in q['routines']:
        row={k:body[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')}
        row.update(verification=qp.relative_to(P).as_posix(),status='Complete original line clipping helper software-qualified;all296rows/16outcomes execute;136normal full-state comparisons and544independent exact oracles. Larger response/renderer and physical hardware remain unqualified.',limitations=q['limitations'])
        old=next((x for x in reg['routines'] if x.get('module','1ST_READ.BIN')==row['module'] and x['address']==row['address']),None)
        if old is None:reg['routines'].append(row)
        else:assert old==row
    assert len(reg['routines'])==1632 and reg['routines'][:1631]==prior
    assert len({(x.get('module','1ST_READ.BIN'),x['address']) for x in reg['routines']})==1629
    rp.write_text(json.dumps(reg,indent=2)+'\n')
    sp=P/'project_status.json';status=json.loads(sp.read_text());data={k:v for k,v in q.items() if k not in ('sha256','routines')}
    data.update(verification=qp.relative_to(P).as_posix(),verification_sha256=sha(qp),guide='PLAYER_RESPONSE_CLIP.txt')
    status['verified_progress']['player_response_clip_integration']=data
    status['verified_progress']['recovered_buildable_routines']=status['recovered_buildable_routines']=1632
    sp.write_text(json.dumps(status,indent=2)+'\n')
    print('REGISTERED1632entries/1629unique/676confirmedlinks;broaderresponse/renderer unregistered;goal active',flush=True)
if __name__=='__main__':main()
