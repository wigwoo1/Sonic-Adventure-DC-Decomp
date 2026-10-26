"""Actual first-fit and render-dispatch calls with independent pool bookkeeping."""
import hashlib,itertools,json
from pathlib import Path
import write_watch_render_buffer as fast
import verify_movie_render_buffer_format as formatting
v=fast.old;P=v.P;ROOT=v.ROOT
META,POOL,POOL_BYTES=0x8c67c2e0,0x8c8a83e4,98304
CACHE,CACHE_BYTES,CACHE_HEAD=0x8c8c03e4,0x13000,0x8c67c308
DESC,OUT=0x8cdb0000,0x8cdb0100
ALLOC,BANK,DISPATCH=0x8c66452a,0x8c663540,0x8c6536ec
MAXIMA=0x8c663c86

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def patch(m,a,raw):m[a&0xffffff:(a&0xffffff)+len(raw)]=raw
def flags(m,a,x):patch(m,a,(x&65535).to_bytes(2,'little'))
def signed(x):return x-(1<<32) if x&0x80000000 else x
def case(entry=ALLOC,bank=1,request=32,**extra):
 args=[bank,request,DESC] if entry==ALLOC else [request,DESC] if entry==BANK else [OUT,OUT+4] if entry==MAXIMA else [DESC,extra.get('width',8),extra.get('height',8),extra.get('tag',0x100)]
 return dict(entry=entry,mode=extra.pop('mode',0),render_arguments=args,alloc_bank=bank,alloc_request=request,alloc_free=extra.pop('free',[[512],[1024]]),alloc_spares=extra.pop('spares',2),alloc_busy=extra.pop('busy',False),alloc_options=extra.pop('options',0))|extra

def prepare(ram,c):
 m,s,f=v.prepare(ram,c);patch(m,META,bytes(40));patch(m,POOL,(b'\x01\x00'+bytes([0xa5])*22)*(POOL_BYTES//24));patch(m,CACHE,(b'\x01\x00'+bytes([0x5a])*74)*(CACHE_BYTES//76));v.store(m,CACHE_HEAD,0)
 for index in range(c['alloc_spares']):flags(m,POOL+(8+index)*24,0)
 for bank,blocks in enumerate(c['alloc_free']):
  nodes=[POOL+(bank*4+i)*24 for i in range(len(blocks))];assert len(nodes)<=3;base=META+20*bank
  for i,(node,size) in enumerate(zip(nodes,blocks)):
   flags(m,node,1)
   for off,x in ((4,nodes[i-1] if i else 0),(8,nodes[i+1] if i+1<len(nodes) else 0),(12,bank*0x400000+i*0x10000),(16,size),(20,0)):v.store(m,node+off,x)
  for off,x in ((8,nodes[0] if nodes else 0),(12,nodes[-1] if nodes else 0),(16,sum(blocks)&0xffffffff)):v.store(m,base+off,x)
  if c['alloc_busy']:
   node=POOL+(12+bank)*24;flags(m,node,5)
   for off,x in ((4,0),(8,0),(12,0x700000+bank*1024),(16,64),(20,OUT)):v.store(m,node+off,x)
   v.store(m,base,node);v.store(m,base+4,node)
 patch(m,DESC,bytes([0x77])*32);patch(m,OUT,bytes([0x99])*32);v.store(m,0x8c8a2d5c,c['alloc_options'])
 # A declared real retail empty callback, not an injected helper return.
 v.store(m,0x8c8a2f84+3*8,0x8c653440);v.store(m,0x8c8a2f84+3*8+4,0x76543210)
 v.store(m,0x8c67c004,0x8cda0000);v.store(m,0x8cda009c,1)
 return m,s,f

def list_maxima(m):
 result=[]
 for bank in range(2):
  best=0
  for off,filtered in ((8,False),(0,True)):
   node=v.load(m,META+20*bank+off);seen=set()
   while node:
    assert node not in seen;seen.add(node);size=v.load(m,node+16)
    if (not filtered or not v.load(m,node)&4) and signed(size)>signed(best):best=size
    node=v.load(m,node+8)
  result.append(best)
 return result

def expected_first_fit(m,bank,request,output=DESC+28):
 e=bytearray(m);base=META+20*bank;request&=0xffffffff;remainder=(abs(signed(request))&31)*(1 if signed(request)>=0 else -1)
 if remainder:request=(request+32-remainder)&0xffffffff
 node=v.load(e,base+8);seen=set()
 while node and signed(v.load(e,node+16))<signed(request):assert node not in seen;seen.add(node);node=v.load(e,node+8)
 if not node:return e,3
 size=v.load(e,node+16);address=v.load(e,node+12)
 if size==request:
  previous,next_node=v.load(e,node+4),v.load(e,node+8)
  v.store(e,previous+8 if previous else base+8,next_node);v.store(e,next_node+4 if next_node else base+12,previous);selected=node
 else:
  selected=next((a for a in range(POOL,POOL+POOL_BYTES,24) if not v.load(e,a)&1),0)
  if not selected:return e,3
  flags(e,selected,1);v.store(e,selected+12,address);v.store(e,selected+16,request);v.store(e,node+16,(size-request)&0xffffffff);v.store(e,node+12,(address+request)&0xffffffff)
 old_head=v.load(e,base)
 v.store(e,selected+4,v.load(e,old_head+4) if old_head else 0);v.store(e,selected+8,old_head)
 if old_head:v.store(e,old_head+4,selected)
 else:v.store(e,base+4,selected)
 v.store(e,base,selected);v.store(e,output,address);v.store(e,selected+20,DESC);flags(e,selected,1|(32 if bank==0 else 64)|4);v.store(e,base+16,(v.load(e,base+16)-request)&0xffffffff)
 return e,0

def cases():
 for bank,request,blocks,spares in itertools.product((0,1),(0,1,31,32,33,64,128),((),(32,),(16,64),(512,)),(0,2)):
  free=[[],[]];free[bank]=list(blocks);yield case(bank=bank,request=request,free=free,spares=spares,busy=True)
 for options,(a,b),request in itertools.product((0,4),((32,128),(128,32),(64,64),(0,0)),(32,64)):
  yield case(BANK,options=options,free=[[a] if a else [],[b] if b else []],request=request)
 for blocks in (([],[]),([32],[64]),([128,16],[64]),([0xffffffff,0x80000000],[0x7fffffff])):yield case(MAXIMA,free=blocks)
 for tag,blocks in itertools.product((0x100,0x900),((),(32,),(128,),(512,))):yield case(DISPATCH,tag=tag,free=[[],list(blocks)])
 for mode in (1,4):yield case(mode=mode,free=[[256],[512]],request=33,busy=True)

def main():
 ram=v.seed_ram();records=[];seen=set();edges=set();checks=0
 for i,data in enumerate(cases(),1):
  c=v.Input(data);m,s,f=prepare(ram,c);before=v.effects.State.from_buffer_copy(s);fp=bytes(f)
  if c['entry']==MAXIMA:
   expected=bytearray(m);values=list_maxima(m)
   for j,x in enumerate(values):v.store(expected,OUT+4*j,x)
   result=0
  else:
   initial=bytearray(m);request=c['alloc_request'];bank=c['alloc_bank']
   if c['entry']==DISPATCH:
    fc=dict(render_tag=c.get('tag',0x100),render_low=0,render_width=c.get('width',8),render_height=c.get('height',8),render_context=1)
    fields,error=formatting.format_oracle(fc,[v.load(m,DESC+4*j) for j in range(8)]);assert error==0
    for j,x in enumerate(fields):v.store(initial,DESC+4*j,x)
    request=fields[5]
   if c['entry'] in (BANK,DISPATCH):
    maxima=list_maxima(m);bank=int(signed(maxima[0])<signed(maxima[1])) if c['alloc_options']&4 else 1
   expected,result=expected_first_fit(initial,bank,request)
  r,path,a,s,f=fast.compare(m,s,f,c)
  assert r['full_state_match'] and r['ordered_boundaries_match'] and r['status']==0 and s.r[0]==result,(i,c,r['status'],hex(s.r[0]),result);n=4
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
 files=[Path(__file__),Path(formatting.__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_render_buffer_connection.json',v.effects.DLL]
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Real first-fit allocation/exact/split/no-space/no-descriptor branches; complete independent 98304-byte pool,77824-byte cache pool,bank metadata,descriptor andoutput oracles. Real bank selection,maxima andnon-type3 renderdispatch includeoriginalerrorcallback toexplicitretail emptybody. Explicitcoherent finite aligned lists anddisjointstack. No helperreturns injected; coverage measured,not blanket qualification orplayability.')
 (P/'analysis/movie_render_buffer_allocation_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Allocation',len(records),checks,[(x['address'],x['covered_rows'],x['static_rows'],x['covered_outcomes'],x['conditional_outcomes']) for x in coverage])
if __name__=='__main__':main()
