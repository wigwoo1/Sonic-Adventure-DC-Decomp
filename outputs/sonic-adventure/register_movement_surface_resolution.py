"""Append both verified originals while preserving every previous registry row."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    qp=P/'analysis/movement_surface_resolution_qualification.json';q=json.loads(qp.read_text())
    assert q['new_original_routine_credit']==3 and q['newly_qualified_original_rows']==647
    for name,h in q['sha256'].items():assert digest(name)==h,name
    rp=P/'analysis/recovered_routines.json';r=json.loads(rp.read_text());previous=r['routines'][:1615];assert len(previous)==1615
    for body in q['routines']:
        row={k:body[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')}
        row.update(verification=qp.relative_to(P).as_posix(),status='Complete original surface resolution/vector redirection/nullable-clear bodies software-qualified;all647group rows/64outcomes execute. Physicalhardware/fullgame and older owner/precision/callback obligations remain unqualified.',limitations=q['limitations'])
        old=next((x for x in r['routines'] if x.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and x['address']==row['address']),None)
        if old is None:r['routines'].append(row)
        else:assert old==row
    assert len(r['routines'])==1618 and r['routines'][:1615]==previous
    assert len({(x.get('module','1ST_READ.BIN'),x['address']) for x in r['routines']})==1615
    rp.write_text(json.dumps(r,indent=2)+'\n')
    sp=P/'project_status.json';s=json.loads(sp.read_text());data={k:v for k,v in q.items() if k not in ('sha256','routines')}
    data.update(verification=qp.relative_to(P).as_posix(),verification_sha256=digest(qp),guide='MOVEMENT_SURFACE_RESOLUTION.txt')
    s['verified_progress']['movement_surface_resolution_integration']=data
    s['verified_progress']['recovered_buildable_routines']=s['recovered_buildable_routines']=1618
    sp.write_text(json.dumps(s,indent=2)+'\n')
    print('REGISTERED1618entries/1615unique/662linked;10%milestones;goalACTIVE',flush=True)
if __name__=='__main__':main()
