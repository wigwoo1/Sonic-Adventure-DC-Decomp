"""Target missing original configuration, wait and packing paths."""
import collections,hashlib,itertools,json
from pathlib import Path
import verify_movie_render_config as v
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def cases():
 for mode,bits,pixel in itertools.product((0,4),(2,3,8,0xa,0x28,0x2e,0x2f),(0,1,3,4,5)):
  yield dict(entry=v.CONFIG,mode=mode,mode_bits=bits,pixel_format=pixel,scan_samples=[0,1,1,0]*16)
 for mode in (0,4):
  yield dict(entry=v.PACK,mode=mode,stack_words=[0x12345678+i*0x10203 for i in range(40)],line_bias=1)
  yield dict(entry=v.VIDEO,mode=mode,flags=2,stack_words=[0]*40,scan_samples=[0,1,1,0])
  yield dict(entry=v.TYPE2,mode=mode,timing_selector=6)
  for pending in ('render','transfer'):yield dict(entry=v.IDLE,mode=mode,renderer=0,render_busy=int(pending=='render'),transfer_busy=int(pending=='transfer'),clear_pending=pending)
def main():
 ram=v.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.seed_ram();records=[];coverage=set();edges=set()
 for c in cases():
  m,s,f=v.prepare(ram,c)
  if 'line_bias' in c:v.store(m,v.load(m,0x8c657d18),c['line_bias'])
  if 'timing_selector' in c:v.store(m,v.load(m,0x8c658bf4),c['timing_selector'])
  if c.get('clear_pending'):
   # A real already-connected original memory-fill body is a declared direct
   # callback fixture. It clears the selected pending word with its original
   # instructions; no callback result or engine timing is fabricated.
   target=0x8c8a2f5c if c['clear_pending']=='render' else 0x8c8a2f60
   v.store(m,0x8c8a2fbc,0x8c10d31c);v.store(m,0x8c8a2fc0,target);s.r[5]=0;s.r[6]=4
  before=v.effects.State.from_buffer_copy(s);fp=v.effects.Fpu.from_buffer_copy(f);record,path,a,s,f=v.compare(m,s,f,c);record['independent_checks']=v.semantics(m,a,before,fp,s,f,record,c)
  if c.get('clear_pending'):
   assert record['status']==0 and sum(b['target']=='0x8c10d31c' for b in record['boundaries'])==1;record['independent_checks']+=2
  records.append(record);coverage.update(path)
  for j,pc in enumerate(path):
   if pc not in v.ROWS:continue
   text=v.ROWS[pc];distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
 files=[Path(__file__),Path(v.__file__),v.MP,v.P/'analysis/movie_render_config_connection.json']
 report=dict(cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),independent_checks=sum(r['independent_checks'] for r in records),covered_addresses=[hex(x) for x in sorted(coverage)],covered_conditional_edges=[[hex(a),hex(b)] for a,b in sorted(edges)],coverage_by_routine=v.coverage_rows(coverage,edges),dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Explicit mode-bit/format inputs, a positive original line-bias global, timing selector six, raw scan observations that exercise both polling continuations, and pending completion through the actual original memory-fill callback. Full state is compared. These direct callback fixtures do not assert natural game callback installation or physical display timing.',cases=records)
 (v.P/'analysis/movie_render_edges_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:report[k] for k in ('cases_passed','status_counts','independent_checks')})
if __name__=='__main__':main()
