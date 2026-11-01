"""Append only the four qualified shape originals;retain larger proposals unregistered."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    qp=P/'analysis/player_movement_path_qualification.json';q=json.loads(qp.read_text());assert q['new_original_routine_credit']==4 and q['newly_qualified_original_rows']==425
    for path,expected in q['sha256'].items():assert sha(path)==expected,path
    rp=P/'analysis/recovered_routines.json';reg=json.loads(rp.read_text());prior=reg['routines'][:1627];assert len(prior)==1627
    for body in q['routines']:
        row={k:body[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')}
        row.update(verification=qp.relative_to(P).as_posix(),status='Complete original point/direction helper software-qualified;all425group rows/56outcomes execute. Full-state original/native comparisons and148exact semantic cases. Broader movement owners,physicalhardware andfullgame remain unqualified.',limitations=q['limitations'])
        old=next((x for x in reg['routines'] if x.get('module','1ST_READ.BIN')==row['module'] and x['address']==row['address']),None)
        if old is None:reg['routines'].append(row)
        else:assert old==row
    assert len(reg['routines'])==1631 and reg['routines'][:1627]==prior
    assert len({(x.get('module','1ST_READ.BIN'),x['address']) for x in reg['routines']})==1628
    rp.write_text(json.dumps(reg,indent=2)+'\n')
    sp=P/'project_status.json';status=json.loads(sp.read_text());data={k:v for k,v in q.items() if k not in ('sha256','routines')}
    data.update(verification=qp.relative_to(P).as_posix(),verification_sha256=sha(qp),guide='PLAYER_MOVEMENT_PATH.txt')
    status['verified_progress']['player_movement_path_integration']=data
    status['verified_progress']['recovered_buildable_routines']=status['recovered_buildable_routines']=1631
    sp.write_text(json.dumps(status,indent=2)+'\n')
    print('REGISTERED1631entries/1628unique/675confirmedlinks;owner androute remainunregistered;goalACTIVE',flush=True)
if __name__=='__main__':main()
