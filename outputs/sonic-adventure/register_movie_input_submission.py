"""Register six freshly qualified bodies; preserve canonical and historical rows."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    qp=P/'analysis/movie_input_submission_qualification.json';q=json.loads(qp.read_text())
    assert (q['newly_registered_entries'],q['newly_qualified_rows'],q['qualified_conditional_outcomes'])==(6,351,46)
    for pin,h in q['sha256'].items():assert digest(pin)==h,pin
    rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text());previous=registry['routines'][:1502];assert len(previous)==1502
    for routine in q['routines']:
        row={k:routine[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')}
        row.update(verification=qp.relative_to(P).as_posix(),status='Originalsubmission/preparation body qualified in declared coherent contexts with real callees and modeled bus. PhysicalDMA/activegameparser remain unfinished.',limitations=q['limitations'])
        old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==row['address']),None)
        if old is None:registry['routines'].append(row)
        else:assert old==row
    assert len(registry['routines'])==1508 and registry['routines'][:1502]==previous
    assert len({(r.get('module','1ST_READ.BIN'),r['address']) for r in registry['routines']})==1505
    rp.write_text(json.dumps(registry,indent=2)+'\n')
    sp=P/'project_status.json';status=json.loads(sp.read_text());data={k:value for k,value in q.items() if k not in ('routines','canonical_reuse','sha256','coverage_by_routine')}
    data.update(verification=qp.relative_to(P).as_posix(),verification_sha256=digest(qp),guide='MOVIE_INPUT_SUBMISSION.txt')
    status['verified_progress']['movie_input_submission_integration']=data
    status['verified_progress']['recovered_buildable_routines']=status['recovered_buildable_routines']=1508
    sp.write_text(json.dumps(status,indent=2)+'\n');print('REGISTERED1508/unique1505;goalactive10%',flush=True)
if __name__=='__main__':main()
