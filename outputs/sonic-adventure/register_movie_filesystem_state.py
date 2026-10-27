"""Register only completely qualified read bodies; preserve all old registry rows."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    qp=P/'analysis/movie_filesystem_state_qualification.json';q=json.loads(qp.read_text())
    assert (q['newly_registered_entries'],q['newly_qualified_rows'],q['qualified_conditional_outcomes'])==(2,102,14)
    for pin,h in q['sha256'].items():assert digest(pin)==h,pin
    rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text());previous=registry['routines'][:1513];assert len(previous)==1513
    for routine in q['routines']:
        row={k:routine[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')}
        row.update(verification=qp.relative_to(P).as_posix(),status='Original root-read gateway/retry reader qualified with real task-created requests andraw software device inputs. Rootvalidation/tableloop/hardware acquisition remain unfinished.',limitations=q['limitations'])
        old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==row['address']),None)
        if old is None:registry['routines'].append(row)
        else:assert old==row
    assert len(registry['routines'])==1515 and registry['routines'][:1513]==previous
    assert len({(r.get('module','1ST_READ.BIN'),r['address']) for r in registry['routines']})==1512
    rp.write_text(json.dumps(registry,indent=2)+'\n')
    sp=P/'project_status.json';status=json.loads(sp.read_text());data={k:value for k,value in q.items() if k not in ('routines','sha256','coverage_by_routine','partial_table_coverage')}
    data.update(verification=qp.relative_to(P).as_posix(),verification_sha256=digest(qp),guide='MOVIE_FILESYSTEM_STATE.txt')
    status['verified_progress']['movie_filesystem_state_integration']=data
    status['verified_progress']['recovered_buildable_routines']=status['recovered_buildable_routines']=1515
    sp.write_text(json.dumps(status,indent=2)+'\n');print('REGISTERED1515/unique1512;goalactive10%',flush=True)
if __name__=='__main__':main()
