"""Append three verified originals without changing earlier registry entries."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    qp=P/'analysis/player_work_lifecycle_qualification.json';q=json.loads(qp.read_text())
    assert q['new_original_routine_credit']==3 and q['newly_qualified_original_rows']==319
    for path,expected in q['sha256'].items():assert sha(path)==expected,path
    rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text());prior=registry['routines'][:1623]
    assert len(prior)==1623
    for body in q['routines']:
        row={k:body[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')}
        row.update(verification=qp.relative_to(P).as_posix(),status='Complete original player-work constructor/helper software-qualified;all319group rows/42outcomes execute. Original initialized heap/task lifecycles and declared bank/rounding inputs;physicalhardware/fullgame and universal caller stack-alias contract remain unqualified.',limitations=q['limitations'])
        old=next((x for x in registry['routines'] if x.get('module','1ST_READ.BIN')==row['module'] and x['address']==row['address']),None)
        if old is None:registry['routines'].append(row)
        else:assert old==row
    assert len(registry['routines'])==1626 and registry['routines'][:1623]==prior
    assert len({(x.get('module','1ST_READ.BIN'),x['address']) for x in registry['routines']})==1623
    rp.write_text(json.dumps(registry,indent=2)+'\n')
    sp=P/'project_status.json';status=json.loads(sp.read_text())
    data={k:v for k,v in q.items() if k not in ('routines','sha256')}
    data.update(verification=qp.relative_to(P).as_posix(),verification_sha256=sha(qp),guide='PLAYER_WORK_LIFECYCLE.txt')
    status['verified_progress']['player_work_lifecycle_integration']=data
    status['verified_progress']['recovered_buildable_routines']=status['recovered_buildable_routines']=1626
    sp.write_text(json.dumps(status,indent=2)+'\n')
    print('REGISTERED1626entries/1623unique/670confirmedlinks;10%milestones;goalACTIVE',flush=True)
if __name__=='__main__':main()
