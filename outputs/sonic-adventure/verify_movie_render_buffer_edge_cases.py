"""Reach remaining pool, size-error and metadata boundaries with explicit inputs."""
import itertools,json
from pathlib import Path
import verify_movie_render_buffer_cache as cache
fixed=cache.fixed;pool=cache.pool;v=pool.v;fast=pool.fast;P=pool.P;ROOT=pool.ROOT
META,POOL,POOL_BYTES,CACHE,CACHE_BYTES,CACHE_HEAD,DESC,OUT=(getattr(pool,n) for n in ('META','POOL','POOL_BYTES','CACHE','CACHE_BYTES','CACHE_HEAD','DESC','OUT'))

def cases():
 for bank,request in itertools.product((0,1),(0xfffffff0,0xffffffe0)):
  yield pool.case(bank=bank,request=request,busy=True)|dict(edge='pool')
  yield fixed.case(fixed.ALIGNED,bank=bank,request=request,busy=True)|dict(edge='aligned')
 for bank,(offset,request),free in itertools.product((0,1),((0,33),(32,0xfffffff0),(32,0xffffffc0),(0,256)),(False,True)):
  banks=[[],[]];banks[bank]=[128] if free else []
  yield fixed.case(bank=bank,start=bank*0x400000+offset,request=request,free=banks,spares=2)|dict(edge='fixed')
 for bank,blocks,shift,request in itertools.product((0,1),((32,),(2080,),(2080,4096)),(0,32),(32,2080)):
  banks=[[],[]];banks[bank]=list(blocks)
  yield fixed.case(fixed.ALIGNED,bank=bank,request=request,free=banks,base_shift=shift,spares=2,busy=True)|dict(edge='aligned')
 for flags in (0,1,4,5):yield pool.case(pool.MAXIMA,free=[[64,32],[128,32]],busy=True)|dict(edge='maxima',busy_flags=flags)
 yield pool.case(pool.DISPATCH,tag=0x105)|dict(edge='format_error')
 yield cache.case(options=4,pattern='empty',free=[[4096],[8192]])|dict(edge='cache')
 yield pool.case(0x8c6652c0)|dict(edge='metadata',render_arguments=[OUT,DESC])
 yield pool.case(0x8c663712,spares=0)|dict(edge='zero_slots',render_arguments=[DESC])
 yield pool.case(0x8c664b36)|dict(edge='link',render_arguments=[META+12,POOL+8*24,POOL])

def prepare(ram,c):
 edge=c['edge']
 if edge in ('aligned','fixed'):return fixed.prepare(ram,c)
 if edge=='cache':return cache.prepare(ram,c)
 m,s,f=pool.prepare(ram,c)
 if edge=='maxima':
  for bank in range(2):
   head=POOL+(12+bank)*24;tail=POOL+(14+bank)*24;pool.flags(m,head,c['busy_flags']);v.store(m,head+16,256);v.store(m,head+8,tail);pool.flags(m,tail,c['busy_flags']);v.store(m,tail+4,head);v.store(m,tail+8,0);v.store(m,tail+16,16);v.store(m,META+bank*20+4,tail)
 elif edge in ('metadata','zero_slots'):
  v.store(m,DESC+12,128)
  if edge=='zero_slots':
   # The ordinary direct-entry RAM fixture already has zero local words.
   # No guest checkpoint or callee result is written to force this branch.
   assert v.load(m,s.r[15]-24)==0 and v.load(m,s.r[15]-40)==0
   v.store(m,CACHE_HEAD,CACHE);pool.flags(m,CACHE,1);v.store(m,CACHE+4,0)
 elif edge=='link':
  v.store(m,POOL+8,POOL+24);v.store(m,POOL+24+4,POOL);v.store(m,POOL+24+8,0)
 return m,s,f

def oracle(m,c):
 edge=c['edge']
 if edge=='pool':return pool.expected_first_fit(m,c['alloc_bank'],c['alloc_request'])
 if edge=='aligned':
  raw=c['alloc_request'];rem=(abs(pool.signed(raw))&31)*(1 if pool.signed(raw)>=0 else -1);rounded=(raw+32-rem)&0xffffffff if rem else raw
  return fixed.aligned_oracle(m,c['alloc_bank'],rounded)
 if edge=='fixed':return fixed.fixed_oracle(m,c['alloc_bank'],c['fixed_start'],c['alloc_request'],c['fixed_active'],c['fixed_lock'])
 if edge=='cache':return cache.oracle(m,c)
 e=bytearray(m)
 if edge=='maxima':
  for j,x in enumerate(pool.list_maxima(m)):v.store(e,OUT+j*4,x)
  return e,0
 if edge=='format_error':
  fields,result=pool.formatting.format_oracle(dict(render_tag=0x100,render_low=5,render_width=8,render_height=8,render_context=1),[v.load(m,DESC+4*j) for j in range(8)])
  for j,x in enumerate(fields):v.store(e,DESC+4*j,x)
  assert result==4;return e,result
 if edge=='metadata':return e,0
 if edge=='zero_slots':return e,3
 if edge=='link':
  new=POOL+8*24;next_node=POOL+24;v.store(e,next_node+4,new);v.store(e,new+4,POOL);v.store(e,new+8,next_node);v.store(e,POOL+8,new)
  return e,None
 raise AssertionError(edge)

def main():
 ram=v.seed_ram();records=[];seen=set();edges=set();checks=0
 for i,data in enumerate(cases(),1):
  c=v.Input(data);m,s,f=prepare(ram,c);before=v.effects.State.from_buffer_copy(s);fp=bytes(f);expected,result=oracle(m,c)
  r,path,a,s,f=fast.compare(m,s,f,c);assert r['full_state_match'] and r['ordered_boundaries_match'] and r['status']==0;assert result is None or s.r[0]==result,(i,c,hex(s.r[0]),result);n=4
  assert s.pc==s.pr==before.pr and list(s.r[8:])==list(before.r[8:]) and bytes(f)==fp;n+=3
  for address,size in ((META,40),(POOL,POOL_BYTES),(CACHE,CACHE_BYTES),(CACHE_HEAD,4),(DESC,32),(OUT,32)):
   assert v.sample(a,address,size)==v.sample(expected,address,size),(i,c,hex(address));n+=1
  assert not r['vram_writes'] and not any(r['sound_state']) and r['runtime_state'] is None;n+=3;checks+=n;seen.update(path)
  for j,pc in enumerate(path):
   ins=v.ROWS.get(pc,'');distance=2 if ins.startswith(('bt/s ','bf/s ')) else 1
   if ins.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
  records.append(dict(input=dict(c),status=r['status'],r0=s.r[0],steps=len(path),independent_checks=n));print(i,c['edge'],hex(s.r[0]),flush=True)
 coverage=[]
 for body in v.manifest['routines']:
  wanted=set(body['reachable_addresses']);wanted_edges=set(map(tuple,body['conditional_edges']));actual={hex(a) for a in seen};actual_edges={(hex(a),hex(b)) for a,b in edges}
  coverage.append(dict(address=body['address'],static_rows=len(wanted),covered_rows=len(wanted&actual),conditional_outcomes=len(wanted_edges),covered_outcomes=len(wanted_edges&actual_edges),missing_rows=sorted(wanted-actual),missing_outcomes=[list(e) for e in sorted(wanted_edges-actual_edges)]))
 files=[Path(__file__),Path(cache.__file__),Path(fixed.__file__),Path(pool.__file__),Path(pool.formatting.__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_render_buffer_connection.json',v.effects.DLL]
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=pool.digest(v.effects.DLL),sha256={str(p):pool.digest(p) for p in files},scope='Measured remaining descriptor error,negative size/wrap,alignment skip/fallback/exact,occupied-list maximum,metadata unsupportedwidth,zero untouchedlocals andmiddle listlink cases. No callee result orguestcheckpoint injection; originalnormal direct-entry initialRAM is declared. Fullindependent pools/metadata/descriptor oracles supplementcomplete state andorderedboundaries. No gameplay or blanket qualification claim.')
 (P/'analysis/movie_render_buffer_edges_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Edges',len(records),checks,[(x['address'],x['covered_rows'],x['static_rows'],x['covered_outcomes'],x['conditional_outcomes']) for x in coverage])
if __name__=='__main__':main()
