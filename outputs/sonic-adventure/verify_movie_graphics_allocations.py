"""Real original pool/cache/texture allocation, with independent linked-state oracle."""
import hashlib,itertools,json
from pathlib import Path
import write_watch_graphics_init as fast
v=fast.old;P=v.P;ROOT=v.ROOT
ALLOC,CACHE,TEXTURE=0x8c664352,0x8c663320,0x8c6634a0
META,POOL,POOL_BYTES=0x8c67c2e0,0x8c8a83e4,0x18000
OUT,OUT2=0x8cdb0000,0x8cdb0200
CACHE_SIZE,CACHE_ADDRESS,CONTROL=0x8c67c310,0x8c67c30c,0x8c8a2d5c
NEW=(ALLOC,CACHE,TEXTURE)
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def signed(x):return x-0x100000000 if x&0x80000000 else x
def rounded(x):
 x&=0xffffffff;rem=(x&31) if not x&0x80000000 else -((-signed(x))&31)
 return (x+(32-rem if rem else 0))&0xffffffff
def address_map(x):
 x&=0xffffffff;return (((x&0xfffffff8)>>1)+(x&3)+(0x400000 if x&4 else 0)+(0x400000 if x>=0x800000 else 0))&0xffffffff
def patch(m,address,data):m[address&0xffffff:(address&0xffffff)+len(data)]=data
def flags(m,address,value):patch(m,address,(value&0xffff).to_bytes(2,'little'))
def load_flags(m,address):return int.from_bytes(v.sample(m,address,2),'little')
def prepare(ram,c):
 m,s,f=v.prepare(ram,c);fixture=c['allocation_fixture']
 patch(m,POOL,(b'\x01\x00\xa5\xa5'+bytes([0xa5])*20)*(POOL_BYTES//24))
 for bank,sizes in enumerate((fixture.get('free_sizes',[128]),fixture.get('bank1_sizes',[256]))):
  nodes=[POOL+(bank*3+i)*24 for i in range(len(sizes))];base=META+bank*20
  for i,(node,size) in enumerate(zip(nodes,sizes)):
   flags(m,node,1);v.store(m,node+4,nodes[i-1] if i else 0);v.store(m,node+8,nodes[i+1] if i+1<len(nodes) else 0);v.store(m,node+12,(fixture.get('start',0x120000)+bank*0x400000+i*0x1000)&0xffffffff);v.store(m,node+16,size&0xffffffff);v.store(m,node+20,0)
  busy=[POOL+(6+bank*2+i)*24 for i in range(fixture.get('busy',0))]
  for i,node in enumerate(busy):
   flags(m,node,13);v.store(m,node+4,busy[i-1] if i else 0);v.store(m,node+8,busy[i+1] if i+1<len(busy) else 0);v.store(m,node+12,0x600000+i*0x1000);v.store(m,node+16,32);v.store(m,node+20,0x8cdb1000+i*32)
  for offset,value in ((0,busy[0] if busy else 0),(4,busy[-1] if busy else 0),(8,nodes[0] if nodes else 0),(12,nodes[-1] if nodes else 0),(16,sum(sizes)&0xffffffff)):v.store(m,base+offset,value)
 if not fixture.get('exhausted',False):flags(m,POOL+12*24,0)
 patch(m,OUT,bytes([0x66])*128);patch(m,OUT2,bytes([0x88])*128)
 for address,value in ((CACHE_SIZE,fixture.get('cache_size',0x76543210)),(CACHE_ADDRESS,fixture.get('cache_address',0x654321)),(CONTROL,fixture.get('control',0))):v.store(m,address,value)
 return m,s,f
def case(entry=ALLOC,size=64,kind=8,bank=0,**fixture):
 args=[bank,size,OUT,kind] if entry==ALLOC else [size,OUT] if entry==CACHE else [size,OUT,OUT2]
 return dict(entry=entry,mode=fixture.pop('fpu_mode',0),gfx_arguments=args,allocation_fixture=fixture)
def cases():
 for free,requested,busy,kind in itertools.product(([],[32],[64],[64,128],[32,64],[32,64,128],[128]),(64,),(0,2),(8,0x2000,7)):
  yield case(size=requested,kind=kind,free_sizes=free,busy=busy)
 for free,size,kind in itertools.product(([0],[128]),(0,1,31,32,33,0x7fffffff,0x80000000,0xffffffc1,0xffffffe0,0xffffffff),(8,0x2000)):
  yield case(size=size,kind=kind,free_sizes=free)
 for bank,kind,exhausted in itertools.product((0,1,0x10000,0x10001),(8,0x2000,0x10008,0x12000),(False,True)):
  yield case(bank=bank,kind=kind,exhausted=exhausted,free_sizes=[128],bank1_sizes=[32,128],busy=2)
 for mode in (1,4):yield case(fpu_mode=mode,free_sizes=[32,64,128],busy=2)
 for size,control,free0,free1 in itertools.product((32,33),(0,4),([32],[128]),([64],[256])):
  yield case(CACHE,size=size,control=control,free_sizes=free0,bank1_sizes=free1,busy=2)
 for size,cached in itertools.product((0,1,32,0xffffffff),(0,4,0x7ffffc,0x800000,0xffffffff)):
  yield case(CACHE,size=size,cache_size=rounded(size),cache_address=cached)
 for size,exhausted in itertools.product((0,1,32,0x80000000,0xffffffc1,0xffffffff),(False,True)):
  yield case(TEXTURE,size=size,free_sizes=[128],busy=2,exhausted=exhausted)
 for entry,mode in itertools.product((CACHE,TEXTURE),(1,4)):yield case(entry,size=33,fpu_mode=mode)

def host_allocate(m,bank,size,out,kind):
 # Original signed remainder adjustment, signed first fit and low16 selectors.
 bank=bank&0xffff;bank=bank-0x10000 if bank&0x8000 else bank;base=META+bank*20;size=rounded(size);kind&=0xffff
 node=v.load(m,base+8)
 while node and signed(v.load(m,node+16))<signed(size):node=v.load(m,node+8)
 if not node:return 3
 def insert(node):
  head=v.load(m,base)
  if head:v.store(m,node+4,v.load(m,head+4));v.store(m,node+8,head);v.store(m,head+4,node);v.store(m,base,node)
  else:
   for address,value in ((node+4,0),(node+8,0),(base,node),(base+4,node)):v.store(m,address,value)
 if v.load(m,node+16)==size:
  prev=v.load(m,node+4);next_=v.load(m,node+8)
  v.store(m,prev+8 if prev else base+8,next_);v.store(m,next_+4 if next_ else base+12,prev);insert(node);flags(m,node,1)
 else:
  spare=next((p for p in range(POOL,POOL+POOL_BYTES,24) if not load_flags(m,p)&1),0)
  if not spare:return 3
  flags(m,spare,1);start=v.load(m,node+12);v.store(m,spare+12,start);v.store(m,spare+16,size);v.store(m,node+16,(v.load(m,node+16)-size)&0xffffffff);v.store(m,node+12,(start+size)&0xffffffff);insert(spare);node=spare
 if kind not in (8,0x2000):return 3
 v.store(m,out+(28 if kind==8 else 0),v.load(m,node+12));v.store(m,node+20,out);flags(m,node,load_flags(m,node)|kind|4)
 v.store(m,base+16,(v.load(m,base+16)-v.load(m,node+16))&0xffffffff);return 0

def semantic(m,a,before,fp,s,f,r,c):
 assert r['full_state_match'] and r['ordered_boundaries_match'];checks=2
 assert r['status']==0 and s.pc==s.pr==before.pr and list(s.r[8:])==list(before.r[8:]) and bytes(f)==fp;checks+=4
 expected=bytearray(m);args=[x&0xffffffff for x in c['gfx_arguments']];entry=c['entry']
 if entry==ALLOC:result=host_allocate(expected,*args)
 elif entry==CACHE:
  size=rounded(args[0]);out=args[1]
  if v.load(expected,CACHE_SIZE)==size:
   v.store(expected,out+20,size);v.store(expected,out+28,address_map(v.load(expected,CACHE_ADDRESS)+4));v.store(expected,CACHE_SIZE,0);v.store(expected,CACHE_ADDRESS,0);result=0
  else:
   bank=int(v.load(expected,META+16)<v.load(expected,META+36)) if v.load(expected,CONTROL)&4 else 0
   result=host_allocate(expected,bank,(size*2)&0xffffffff,out,8)
   if result==0:
    pointer=v.load(expected,out+28);v.store(expected,CACHE_SIZE,size);v.store(expected,CACHE_ADDRESS,pointer);v.store(expected,out+20,size);v.store(expected,out+28,address_map(pointer))
 elif entry==TEXTURE:
  size=rounded(args[0]);out,out2=args[1:];result=host_allocate(expected,0,(size*2)&0xffffffff,out,0x2000)
  if result==0:
   pointer=v.load(expected,out);v.store(expected,out,address_map(pointer));v.store(expected,out2,address_map(pointer+4))
 else:raise AssertionError(hex(entry))
 assert s.r[0]==result;checks+=1
 for address,size in ((META,0x40),(POOL,POOL_BYTES),(OUT,128),(OUT2,128)):
  assert v.sample(a,address,size)==v.sample(expected,address,size),(hex(entry),c,'allocation metadata',hex(address));checks+=1
 assert not r['vram_writes'] and not any(r['sound_state']) and r['runtime_state'] is None and r['flash_storage_snapshot'] is None;checks+=4
 return checks

def main():
 ram=v.seed_ram();records=[];checks=0;seen=set();edges=set()
 for i,data in enumerate(cases(),1):
  c=v.Input(data);m,s,f=prepare(ram,c);before=v.effects.State.from_buffer_copy(s);fp=bytes(f)
  r,path,a,s,f=fast.compare(m,s,f,c);n=semantic(m,a,before,fp,s,f,r,c);checks+=n;seen.update(path)
  for j,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
  records.append(dict(input=dict(c),status=r['status'],stop_pc=r['stop_pc'],r0=s.r[0],independent_checks=n,boundaries=len(r['boundaries'])));print(i,hex(c['entry']),s.r[0],flush=True)
 coverage=[]
 for body in v.manifest['routines']:
  if int(body['address'],16) not in NEW:continue
  wanted=set(body['reachable_addresses']);actual={hex(pc) for pc in seen};want_edges=set(map(tuple,body['conditional_edges']));actual_edges={(hex(x),hex(y)) for x,y in edges}
  coverage.append(dict(address=body['address'],static_rows=len(wanted),covered_rows=len(wanted&actual),conditional_outcomes=len(want_edges),covered_outcomes=len(want_edges&actual_edges),missing_rows=sorted(wanted-actual),missing_outcomes=[list(e) for e in sorted(want_edges-actual_edges)]))
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),v.MP,v.GRAPHICS_CONFIG_FIXTURE_PATH,P/'analysis/movie_graphics_init_connection.json']
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Independent signed-rounding/first-fit/list-split/descriptor-exhaustion/error-prefix/cache/address-map oracle covers all pool bytes, both bank descriptors and output records. Original real descriptor/list/address providers execute; no allocation result or completed state is supplied. Aligned mapped distinct caller records and explicit coherent list/pool input metadata are declared; odd size wrap/error behavior is retained. Whole CPU/FPU/RAM/VRAM/Sound/device/ordered boundaries compare. Broader alias/corruption/concurrency/hardware/playability remains unqualified.')
 (P/'analysis/movie_graphics_allocations_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Allocation cases',len(records),'checks',checks);print(coverage)
if __name__=='__main__':main()
