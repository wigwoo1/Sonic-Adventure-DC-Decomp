"""Unsigned range wrap and actual no-completion wait, without fabricated progress."""
import hashlib,itertools,json
from pathlib import Path
import verify_movie_transfer_start as direct
fast=direct.fast;v=direct.v;P=v.P

def main():
 ram=v.seed_ram();records=[];rows=set();edges=set();checks=0
 inputs=[dict(entry=v.CACHE,mode=0,address=0x8cd70000,length=0x13291020,ccr=0x80)]
 inputs.extend((dict(entry=v.CACHE,mode=0,address=0x8b000000,length=65536,tag_seed=0x0b000000,ccr=0,expected_stop='0xac5fc698'),dict(entry=v.CACHE,mode=0,address=0x8b000000,length=65536,tag_seed=0,ccr=0x80,expected_stop='0xac5fc728'),dict(entry=v.CACHE,mode=0,address=0x8b002000,length=65536,tag_seed=0x0b000000,ccr=0,expected_stop='0x8c5fc7a8')))
 inputs.extend(dict(entry=v.CLIENT_START,mode=mode,sdk_mode=1,current_record=0,callback=callback,initial_sr=0x60008302|(mask<<4)) for mode,callback,mask in itertools.product((0,1,4),(0xffffffff,0),(0,3,7,15)))
 for spec in inputs:
  c=v.Input(spec);m,s,f=v.prepare(ram,c)
  if c['entry']!=v.CACHE:c['entry']=0x8c65a5dc;s.pc=c['entry']
  before=v.effects.State.from_buffer_copy(s);r,path,a,s,f=fast.compare(m,s,f,c);rows.update(path)
  for i,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+distance<len(path):edges.add((pc,path[i+distance]))
  assert r['full_state_match'] and r['ordered_boundaries_match'];checks+=2
  if c['entry']==v.CACHE:
   count=direct.semantics(m,a,before,s,r,path,c);checks+=count
   if c.get('expected_stop'):
    assert r['status']==-3 and r['stop_pc']==c['expected_stop'] and not r['cache_writeback_sequence'];checks+=3
    expected=direct.cache_ram(m,before,c)
    if c['expected_stop']=='0xac5fc698':
     for offset,reg in ((24,10),(28,9),(32,8)):v.store(expected,before.r[15]-offset,before.r[reg])
     start=(before.r[15]-36)&0xffffff;expected[start:start+4]=m[start:start+4]
    assert a==expected;checks+=1
   else:assert r['status']==0 and not r['cache_writebacks'] and (0xac5fc720,0xac5fc72a) in edges;checks+=3
  else:
   record=before.r[4];waiting=c['callback']==0xffffffff;calls=[b for b in r['boundaries'] if b['target']=='0x8c10faa4'];empty=[b for b in r['boundaries'] if b['target']=='0x8c666e02']
   assert r['status']==(-4 if waiting else 0) and r['stop_pc']==('0x8c65a6ec' if waiting else hex(before.pr));checks+=2
   assert path.count(0x8c65a6ec)==(2 if waiting else 0) and len(empty)==(1 if waiting else 0);checks+=2
   assert len(calls)==1 and calls[0]['arguments'][0]==v.DESCRIPTOR;checks+=2
   assert v.load(a,record+4)==2 and v.load(a,record+24)==0;checks+=2
   assert v.load(a,v.ROOT_CONTEXT+0x1a4)==0 and v.load(a,v.ROOT_CONTEXT+0x190)==record;checks+=2
   assert not any(r['sound_state']) and len(r['cache_writebacks'])==2;checks+=2
   if not waiting:assert s.r[0]==0 and s.r[15]==before.r[15] and s.pr==before.pr;checks+=3
  keep=('input','status','stop_pc','steps','boundary_comparisons','full_state_match','ordered_boundaries_match','cache_writebacks','ccr_state','sound_state','write_watch')
  records.append({k:r[k] for k in keep});print(hex(c['entry']),r['status'],r['stop_pc'],flush=True)
 files=[Path(__file__),Path(direct.__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_transfer_wait_translation.json',P/'analysis/movie_transfer_start_connection.json']
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,executed_rows=[hex(a) for a in sorted(rows)],executed_edges=[[hex(a),hex(b)] for a,b in sorted(edges)],dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='One real unsigned-wrap range executes the otherwise unavailable below-start branch of the second cache-tag mode. Three unsupported-writeback address controls prove exact cached/uncached stop PCs and all partial stack bytes. Twenty-four explicit initialized request calls cover three FPU modes/four interrupt masks, genuine bounded synchronous completion waits versus actual asynchronous API returns. Completion stays zero, the channel stays submitted, and no IRQ/callback/helper return is supplied. Guarded waits preserve the real partial machine/stack state after two continued tests; production without the research macro retains the original wait.')
 (P/'analysis/movie_transfer_start_boundaries_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Transfer boundaries',len(records),'comparisons;',checks,'checks')
if __name__=='__main__':main()
