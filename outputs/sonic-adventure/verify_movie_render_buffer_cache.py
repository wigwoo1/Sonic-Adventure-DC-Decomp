"""Real cache-slot allocation with independent metadata, slot and bank oracles."""
import itertools,json,types
from pathlib import Path
import verify_movie_render_buffer_fixed_allocations as fixed
pool=fixed.pool;v=pool.v;fast=pool.fast;P=pool.P;ROOT=pool.ROOT
META,POOL,POOL_BYTES,CACHE,CACHE_BYTES,CACHE_HEAD,DESC,OUT=(getattr(pool,n) for n in ('META','POOL','POOL_BYTES','CACHE','CACHE_BYTES','CACHE_HEAD','DESC','OUT'))
ENTRY,DISPATCH=0x8c663712,0x8c6536ec
# Values independently checked against the earlier canonical metadata recovery.
PARAMETERS={(8,0):(0x1000,128,32,3984,16),(8,1):(0x2000,128,32,4000,16),(16,0):(0x3000,128,64,4032,16),(16,1):(0x4000,128,96,4064,16),(32,0):(0x5000,256,256,4096,8),(32,1):(0x6000,512,352,3936,4),(64,0):(0x7000,1024,1024,4096,2),(64,1):(0x8000,2048,1376,3424,1)}

def case(entry=ENTRY,w=8,flags=0,pattern='empty',**extra):
 c=pool.case(entry,free=extra.pop('free',[[8192],[8192]]),**extra);c.update(cache_width=w,cache_flags=flags,cache_pattern=pattern,cache_free=extra.get('cache_free',True),cache_slot=extra.get('slot',0),cache_prefix=extra.get('prefix',False))
 c['render_arguments']=[DESC,w,w,0x1100 if flags&1 else 0x1000] if entry==DISPATCH else [DESC]
 return c

def prepare(ram,c):
 m,s,f=pool.prepare(ram,c)
 if c['cache_free']:pool.flags(m,CACHE+4*76,0)
 w,flag=c['cache_width'],c['cache_flags']&1;tag,stride,block,total,count=PARAMETERS[w,flag]
 if c['entry']==ENTRY:
  for j,x in enumerate((3,1,0,w,w,0x77777777,c['cache_flags'],0x77777777)):v.store(m,DESC+4*j,x)
 if c['cache_pattern']!='empty':
  nodes=[CACHE,CACHE+76] if c['cache_prefix'] else [CACHE];v.store(m,CACHE_HEAD,nodes[0])
  for i,node in enumerate(nodes):
   pool.flags(m,node,(0x9000 if i<len(nodes)-1 else tag)|3);v.store(m,node+4,nodes[i+1] if i+1<len(nodes) else 0);v.store(m,node+8,0x10000+i*0x4000)
   for slot in range(16):v.store(m,node+12+slot*4,0x8cdd0000+slot*32)
  if c['cache_pattern']=='available':v.store(m,nodes[-1]+12+c['cache_slot']*4,0)
 return m,s,f

def with_owner(m,bank,total,owner):
 namespace=dict(fixed.__dict__,DESC=owner)
 namespace['fixed_oracle']=types.FunctionType(fixed.fixed_oracle.__code__,namespace)
 run=types.FunctionType(fixed.aligned_oracle.__code__,namespace)
 return run(m,bank,total)

def oracle(m,c):
 e=bytearray(m)
 if c['entry']==DISPATCH:
  fields,error=pool.formatting.format_oracle(dict(render_tag=0x1100 if c['cache_flags']&1 else 0x1000,render_low=0,render_width=c['cache_width'],render_height=c['cache_width'],render_context=1),[v.load(m,DESC+4*j) for j in range(8)])
  for j,x in enumerate(fields):v.store(e,DESC+j*4,x)
  if error:return e,error
 tag,stride,block,total,count=PARAMETERS[c['cache_width'],c['cache_flags']&1]
 node=v.load(e,CACHE_HEAD);selected=0;slot=0
 while node:
  if v.load(e,node)&0xf000==tag:
   candidate=next((j for j in range(count) if v.load(e,node+12+4*j)==0),None)
   if candidate is not None:selected=node;slot=candidate;break
  node=v.load(e,node+4)
 if not selected:
  selected=next((a for a in range(CACHE,CACHE+CACHE_BYTES,76) if not v.load(e,a)&1),0)
  if not selected:return e,3
  pool.flags(e,selected,1)
  for j in range(16):v.store(e,selected+12+j*4,0)
  v.store(e,selected+4,v.load(e,CACHE_HEAD));v.store(e,CACHE_HEAD,selected);pool.flags(e,selected,v.load(e,selected)|tag)
  maxima=pool.list_maxima(e);bank=int(pool.signed(maxima[0])<pool.signed(maxima[1])) if c['alloc_options']&4 else 1
  e,result=with_owner(e,bank,total,selected)
  if result:pool.flags(e,selected,v.load(e,selected)&0xfffe);return e,3
 v.store(e,DESC+28,(v.load(e,selected+8)+slot*(stride>>2)*4)&0xffffffff);v.store(e,selected+12+slot*4,DESC);pool.flags(e,selected,v.load(e,selected)|2)
 return e,0

def cases():
 for w,flags,pattern,space in itertools.product((8,16,32,64),(0,1),('empty','available','full'),(False,True)):
  count=PARAMETERS[w,flags][4]
  yield case(w=w,flags=flags,pattern=pattern,free=[[8192],[8192]] if space else [[],[]],slot=count-1,prefix=pattern!='empty')
 for w,flags in itertools.product((8,16,32,64),(0,1)):
  yield case(w=w,flags=flags,cache_free=False,pattern='full')
  yield case(DISPATCH,w=w,flags=flags,pattern='available',slot=0)
  yield case(DISPATCH,w=w,flags=flags,pattern='empty',free=[[],[]])
 for options,spares in itertools.product((0,4),(0,1,2)):
  yield case(options=options,spares=spares,pattern='empty',free=[[8192],[4096]])
 for mode in (1,4):yield case(pattern='available',mode=mode,prefix=True)

def main():
 ram=v.seed_ram();records=[];seen=set();edges=set();checks=0
 for i,data in enumerate(cases(),1):
  c=v.Input(data);m,s,f=prepare(ram,c);before=v.effects.State.from_buffer_copy(s);fp=bytes(f);expected,result=oracle(m,c)
  r,path,a,s,f=fast.compare(m,s,f,c);assert r['full_state_match'] and r['ordered_boundaries_match'] and r['status']==0 and s.r[0]==result,(i,c,r['status'],hex(s.r[0]),result);n=4
  assert s.pc==s.pr==before.pr and list(s.r[8:])==list(before.r[8:]) and bytes(f)==fp;n+=3
  for address,size in ((META,40),(POOL,POOL_BYTES),(CACHE,CACHE_BYTES),(CACHE_HEAD,4),(DESC,32),(OUT,32)):
   assert v.sample(a,address,size)==v.sample(expected,address,size),(i,c,hex(address));n+=1
  if c['entry']==DISPATCH:assert sum(b['target']=='0x8c6524d2' for b in r['boundaries'])==int(result==3);n+=1
  assert not r['vram_writes'] and not any(r['sound_state']) and r['runtime_state'] is None;n+=3;checks+=n;seen.update(path)
  for j,pc in enumerate(path):
   ins=v.ROWS.get(pc,'');distance=2 if ins.startswith(('bt/s ','bf/s ')) else 1
   if ins.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
  records.append(dict(input=dict(c),status=r['status'],r0=s.r[0],steps=len(path),independent_checks=n));print(i,hex(c['entry']),result,flush=True)
 coverage=[]
 for body in v.manifest['routines']:
  wanted=set(body['reachable_addresses']);wanted_edges=set(map(tuple,body['conditional_edges']));actual={hex(a) for a in seen};actual_edges={(hex(a),hex(b)) for a,b in edges}
  coverage.append(dict(address=body['address'],static_rows=len(wanted),covered_rows=len(wanted&actual),conditional_outcomes=len(wanted_edges),covered_outcomes=len(wanted_edges&actual_edges),missing_rows=sorted(wanted-actual),missing_outcomes=[list(e) for e in sorted(wanted_edges-actual_edges)]))
 files=[Path(__file__),Path(fixed.__file__),Path(pool.__file__),Path(pool.formatting.__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_render_buffer_connection.json',v.effects.DLL]
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=pool.digest(v.effects.DLL),sha256={str(p):pool.digest(p) for p in files},scope='Actual type3 cacheallocation andoriginal renderdispatch with independently enumerated8metadata classes,slots,strides,cache-list links,allocationowner,poolspares/banks andfailureflagcleanup. Wholepool/cache/metadata/descriptor oracles supplementfullmachineandorderedboundaries. Explicitfinitecoherent mappedmetadata/disjointstack; no helperreturn/checkpoint. Draftcoverageonly pendingseparatequalification.')
 (P/'analysis/movie_render_buffer_cache_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Cache',len(records),checks,[(x['address'],x['covered_rows'],x['static_rows'],x['covered_outcomes'],x['conditional_outcomes']) for x in coverage])
if __name__=='__main__':main()
