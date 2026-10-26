"""Original specified-range allocator with independent split/list oracles."""
import hashlib,itertools,json
from pathlib import Path
import verify_movie_render_buffer_allocations as pool
v=pool.v;fast=pool.fast;P=pool.P;ROOT=pool.ROOT
META,POOL,POOL_BYTES,CACHE,CACHE_BYTES,CACHE_HEAD,DESC,OUT=(getattr(pool,n) for n in ('META','POOL','POOL_BYTES','CACHE','CACHE_BYTES','CACHE_HEAD','DESC','OUT'))
FIXED,ALIGNED=0x8c664f72,0x8c66466c

def case(entry=FIXED,bank=1,start=0x400020,request=32,**extra):
 c=pool.case(entry,bank=bank,request=request,**extra);c.update(fixed_start=start,fixed_active=extra.get('active',0),fixed_lock=extra.get('lock',1))
 c['render_arguments']=[bank,start,request,DESC] if entry==FIXED else [bank,request,DESC]
 return c

def prepare(ram,c):
 m,s,f=pool.prepare(ram,c)
 if c['entry']==FIXED:v.store(m,s.r[15],c['fixed_active']);v.store(m,s.r[15]+4,c['fixed_lock'])
 if 'base_shift' in c:
  for bank in range(2):
   node=v.load(m,META+20*bank+8)
   while node:v.store(m,node+12,(v.load(m,node+12)+c['base_shift'])&0xffffffff);node=v.load(m,node+8)
 return m,s,f

def get_spare(e):
 node=next((a for a in range(POOL,POOL+POOL_BYTES,24) if not v.load(e,a)&1),0)
 if node:pool.flags(e,node,1)
 return node

def insert(e,base,node):
 head=v.load(e,base);v.store(e,node+4,v.load(e,head+4) if head else 0);v.store(e,node+8,head)
 if head:v.store(e,head+4,node)
 else:v.store(e,base+4,node)
 v.store(e,base,node)

def remove(e,base,node):
 previous,next_node=v.load(e,node+4),v.load(e,node+8);v.store(e,previous+8 if previous else base,next_node);v.store(e,next_node+4 if next_node else base+4,previous)

def fixed_oracle(m,bank,start,raw,active,lock):
 e=bytearray(m);base=META+20*bank;end=(start+raw)&0xffffffff;remainder=(abs(pool.signed(raw))&31)*(1 if pool.signed(raw)>=0 else -1);size=(raw+32-remainder)&0xffffffff if remainder else raw
 node=v.load(e,base+8)
 while node:
  address,total=v.load(e,node+12),v.load(e,node+16);limit=(address+total)&0xffffffff
  if pool.signed(start)>=pool.signed(address) and pool.signed(start)<pool.signed(limit) and pool.signed(end)>=pool.signed(address) and pool.signed(end)<=pool.signed(limit):break
  node=v.load(e,node+8)
 if not node:return e,3
 address,total=v.load(e,node+12),v.load(e,node+16)
 if total==size:remove(e,base+8,node);selected=node;pool.flags(e,selected,1)
 else:
  selected=get_spare(e)
  if not selected:return e,3
  if address==start:
   v.store(e,selected+12,address);v.store(e,selected+16,size);v.store(e,node+12,(address+size)&0xffffffff);v.store(e,node+16,(total-size)&0xffffffff)
  elif (address+total)&0xffffffff==end:
   v.store(e,node+16,(total-size)&0xffffffff);v.store(e,selected+12,start);v.store(e,selected+16,size)
  else:
   tail=get_spare(e)
   if not tail:pool.flags(e,selected,v.load(e,selected)&0xfffe);return e,3
   v.store(e,node+16,(start-address)&0xffffffff);v.store(e,selected+12,start);v.store(e,selected+16,size)
   v.store(e,tail+12,(start+size)&0xffffffff);v.store(e,tail+16,(total-size-(start-address))&0xffffffff)
   old_tail=v.load(e,base+12);next_node=v.load(e,old_tail+8)
   if next_node:v.store(e,next_node+4,tail)
   else:v.store(e,base+12,tail)
   v.store(e,tail+4,old_tail);v.store(e,tail+8,next_node);v.store(e,old_tail+8,tail)
 insert(e,base,selected);v.store(e,DESC+8,v.load(e,selected+12));v.store(e,selected+20,DESC)
 pool.flags(e,selected,v.load(e,selected)|(0x800 if bank==0 else 0x1000)|(2 if active else 0)|(4 if lock else 0));v.store(e,base+16,(v.load(e,base+16)-v.load(e,selected+16))&0xffffffff)
 return e,0

def aligned_oracle(m,bank,raw):
 size=(raw+31)&~31;base=META+bank*20;first=0;node=v.load(m,base+8);choice=0;start=0
 while node:
  total,address=v.load(m,node+16),v.load(m,node+12)
  if pool.signed(total)>=pool.signed(size):
   if not first:first=node
   if address%2048==0:choice=node;break
   aligned=(address+2048)&0xfffff800;limit=(address+total)&0xffffffff
   if pool.signed(limit)<=pool.signed(aligned) or pool.signed(size)<=pool.signed((aligned-address)&0xffffffff):choice=node;break
   if pool.signed((limit-aligned)&0xffffffff)>=pool.signed(size):return fixed_oracle(m,bank,aligned,size,0,1)
  node=v.load(m,node+8)
 if not choice:choice=first
 if not choice:return bytearray(m),3
 # Earlier candidates may be skipped for alignment; first_fit must start here.
 # Rebuild the expected transformation directly, keeping the original list links.
 e=bytearray(m);node=choice;total,address=v.load(e,node+16),v.load(e,node+12)
 if total==size:remove(e,base+8,node);selected=node;pool.flags(e,selected,1)
 else:
  selected=get_spare(e)
  if not selected:return e,3
  v.store(e,selected+12,address);v.store(e,selected+16,size);v.store(e,node+16,(total-size)&0xffffffff);v.store(e,node+12,(address+size)&0xffffffff)
 insert(e,base,selected);v.store(e,DESC+8,address);v.store(e,selected+20,DESC);pool.flags(e,selected,v.load(e,selected)|(0x200 if bank==0 else 0x400)|4);v.store(e,base+16,(v.load(e,base+16)-size)&0xffffffff)
 return e,0

def cases():
 for bank,offset,spares,active,lock in itertools.product((0,1),(-32,0,32,96,128),(0,1,2),(0,1),(0,1)):
  start=bank*0x400000+offset;free=[[],[]];free[bank]=[128,64]
  yield case(bank=bank,start=start&0xffffffff,request=32,free=free,spares=spares,busy=True,active=active,lock=lock)
 for bank,active,lock in itertools.product((0,1),(0,1),(0,1)):
  free=[[],[]];free[bank]=[32];yield case(bank=bank,start=bank*0x400000,request=32,free=free,active=active,lock=lock)
 for bank,shift,request,blocks,spares in itertools.product((0,1),(0,32),(32,2048,2080),((),(4096,),(1024,4096)),(0,2)):
  free=[[],[]];free[bank]=list(blocks);yield case(ALIGNED,bank=bank,request=request,free=free,base_shift=shift,spares=spares,busy=True)

def main():
 ram=v.seed_ram();records=[];seen=set();edges=set();checks=0
 for i,data in enumerate(cases(),1):
  c=v.Input(data);m,s,f=prepare(ram,c);before=v.effects.State.from_buffer_copy(s);fp=bytes(f)
  expected,result=fixed_oracle(m,c['alloc_bank'],c['fixed_start'],c['alloc_request'],c['fixed_active'],c['fixed_lock']) if c['entry']==FIXED else aligned_oracle(m,c['alloc_bank'],c['alloc_request'])
  r,path,a,s,f=fast.compare(m,s,f,c);assert r['full_state_match'] and r['ordered_boundaries_match'] and r['status']==0 and s.r[0]==result,(i,c,r['status'],hex(s.r[0]),result);n=4
  assert s.pc==s.pr==before.pr and list(s.r[8:])==list(before.r[8:]) and bytes(f)==fp;n+=3
  for address,size in ((META,40),(POOL,POOL_BYTES),(CACHE,CACHE_BYTES),(CACHE_HEAD,4),(DESC,32),(OUT,32)):
   assert v.sample(a,address,size)==v.sample(expected,address,size),(i,c,hex(address));n+=1
  assert not r['vram_writes'] and not any(r['sound_state']) and r['runtime_state'] is None;n+=3;checks+=n;seen.update(path)
  for j,pc in enumerate(path):
   ins=v.ROWS.get(pc,'');distance=2 if ins.startswith(('bt/s ','bf/s ')) else 1
   if ins.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
  records.append(dict(input=dict(c),status=r['status'],r0=s.r[0],steps=len(path),independent_checks=n));print(i,hex(c['entry']),result,flush=True)
 coverage=[]
 for body in v.manifest['routines']:
  wanted=set(body['reachable_addresses']);wanted_edges=set(map(tuple,body['conditional_edges']));actual={hex(a) for a in seen};actual_edges={(hex(a),hex(b)) for a,b in edges}
  coverage.append(dict(address=body['address'],static_rows=len(wanted),covered_rows=len(wanted&actual),conditional_outcomes=len(wanted_edges),covered_outcomes=len(wanted_edges&actual_edges),missing_rows=sorted(wanted-actual),missing_outcomes=[list(e) for e in sorted(wanted_edges-actual_edges)]))
 files=[Path(__file__),Path(pool.__file__),Path(pool.formatting.__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_render_buffer_connection.json',v.effects.DLL]
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=pool.digest(v.effects.DLL),sha256={str(p):pool.digest(p) for p in files},scope='Original specified-range andalignment-sensitive allocation,independent entire pool/cache/bank/descriptor oracles. Exact/head/tail/middle split,outside-range,descriptor exhaustion androllback,bank/active/lock options; no checkpoint orhelperreturn. Finitecoherent mapped aligned metadata/disjointstack. Coverage explicit; no blanket qualification.')
 (P/'analysis/movie_render_buffer_fixed_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Fixed/aligned',len(records),checks,[(x['address'],x['covered_rows'],x['static_rows'],x['covered_outcomes'],x['conditional_outcomes']) for x in coverage])
if __name__=='__main__':main()
