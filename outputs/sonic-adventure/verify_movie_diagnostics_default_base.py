"""Exercise the original integer formatter's incoming-R8 default radix path."""
import collections,hashlib,itertools,json
from pathlib import Path
import verify_movie_diagnostics as v
P=v.P

def main():
 ram=v.seed_ram();base=next(v.old.old.cases());records=[];coverage=set();edges=set();checks=0
 for value,radix,mode in itertools.product((0,1,42,0x80000000,0xffffffff),(8,10,16),(0,1,4)):
  c=base|dict(entry=v.INTEGER,value=value,specifier=63,incoming_radix=radix,mode=mode);m,s,f=v.prepare(ram,c);s.r[8]=radix;before=v.effects.State.from_buffer_copy(s)
  r,path,a,s,f=v.compare(m,s,f,c);assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match'];checks+=3
  text=format(value,{8:'o',10:'d',16:'X'}[radix]).encode();assert v.sample(a,v.DEST,len(text)+1)==text+b'\0';checks+=1
  assert s.r[8]==radix and s.r[15]==before.r[15] and s.pr==before.pr;checks+=1
  r['guest_r0_at_stop']=hex(s.r[0]);records.append(r);coverage.update(path)
  for i,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+distance<len(path):edges.add((pc,path[i+distance]))
 routine=next(r for r in v.manifest['routines'] if r['address']==hex(v.INTEGER));body=set(v.PER_BODY[v.INTEGER]);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
 row=dict(address=hex(v.INTEGER),static_rows=len(body),covered_rows=len(body&coverage),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)])
 files=[Path(__file__),Path(v.__file__),v.MP,P/'analysis/movie_diagnostics_connection.json']
 report=dict(cases_passed=len(records),independent_checks=checks,status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=[row],cases=records,dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Explicit unsupported-specifier input with original incoming R8 radix 8/10/16. Tests preserve the original default branch and every reached callee; they do not claim the normal formatter supplies that specifier. Zero additional canonical routine credit. Output, saved R8 and return/stack behavior checked independently.')
 (P/'analysis/movie_diagnostics_default_base_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(len(records),'default-radix comparisons;',checks,'checks')
if __name__=='__main__':main()
