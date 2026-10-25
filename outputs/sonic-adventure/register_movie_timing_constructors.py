"""Register three newly closed constructors without changing prior entries."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    qp=P/'analysis/movie_timing_constructors_qualification.json';q=json.loads(qp.read_text())
    assert q['newly_registered_entries']==3 and q['qualified_rows']==879
    for path,h in q['sha256'].items():assert digest(path)==h,path
    rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text());binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    for routine in q['routines']:
        start=int(routine['address'],16);end=int(routine['end_exclusive'],16)
        assert hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest()==routine['retail_span_sha256']
        row=dict(module='1ST_READ.BIN',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],
                 retail_span_sha256=routine['retail_span_sha256'],source=routine['source'],verification=qp.relative_to(P).as_posix(),
                 status='Qualified reachable original body with retained renderer comparisons plus 134 new complete-state cases; 12 rows and 26 outcomes across three bodies are proven unreachable by conservative original-code dataflow.',
                 limitations=q['limitations'],preconditions=q['preconditions'])
        existing=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==row['address']),None)
        if existing is None:registry['routines'].append(row)
        else:assert existing==row
    rp.write_text(json.dumps(registry,indent=2)+'\n')
    sp=P/'project_status.json';status=json.loads(sp.read_text());integration={k:v for k,v in q.items() if k not in ('routines','sha256')}
    integration.update(verification=qp.relative_to(P).as_posix(),verification_sha256=digest(qp),guide='MOVIE_TIMING_CONSTRUCTORS.txt')
    status.setdefault('verified_progress',{})['movie_timing_constructors']=integration
    status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);status['recovered_buildable_routines']=len(registry['routines'])
    sp.write_text(json.dumps(status,indent=2)+'\n');print('Registered three constructors; entries',len(registry['routines']))
if __name__=='__main__':main()
