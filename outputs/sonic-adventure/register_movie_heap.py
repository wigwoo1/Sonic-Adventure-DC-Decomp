"""Register original heap bodies and preserve the canonical thunk recovery."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 qp=P/'analysis/movie_heap_qualification.json';q=json.loads(qp.read_text());assert q['newly_registered_entries']==3 and q['qualified_rows']==178
 for path,h in q['sha256'].items():assert digest(path)==h,path
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text())
 for routine in q['routines']:
  a,b=(int(routine[k],16) for k in ('address','end_exclusive'));assert hashlib.sha256(binary[a-0x8c010000:b-0x8c010000]).hexdigest()==routine['retail_span_sha256']
  row=dict(module='1ST_READ.BIN',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],source=routine['source'],retail_span_sha256=routine['retail_span_sha256'],verification=qp.relative_to(P).as_posix(),
   status='Qualified own original body in 365 complete-state allocation/init/free comparisons, including repeated allocations and coalescing; all 180 integrated rows / 22 outcomes execute. Startup retains its actual null-heap fault.',limitations=q['limitations'])
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==row['address']),None)
  if old is None:registry['routines'].append(row)
  else:assert old==row
 integration={k:v for k,v in q.items() if k not in ('routines','sha256')};integration.update(verification=qp.relative_to(P).as_posix(),verification_sha256=digest(qp),guide='MOVIE_HEAP.txt',adapter_source='source/sa_movie_heap_connected.c',adapter_source_sha256=digest(P/'source/sa_movie_heap_connected.c'))
 thunk=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']=='0x8c6021d2');thunk['movie_heap_integration']=integration
 rp.write_text(json.dumps(registry,indent=2)+'\n')
 sp=P/'project_status.json';status=json.loads(sp.read_text());status.setdefault('verified_progress',{})['movie_heap_integration']=integration
 status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);status['recovered_buildable_routines']=len(registry['routines']);sp.write_text(json.dumps(status,indent=2)+'\n');print('Registered three heap routines; total entries',len(registry['routines']))
if __name__=='__main__':main()
