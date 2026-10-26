"""Original framebuffer/context paths, actual providers and independent state oracles."""
import hashlib,itertools,json
from pathlib import Path
import verify_movie_graphics_allocations as allocation
fast=allocation.fast;v=allocation.v;P=v.P;ROOT=v.ROOT
FRAME,CONTEXT,SWITCH=0x8c665ae0,0x8c641c16,0x8c657760
CTX,STATE,SLOT,FORMAT=0x8c8a330c,0x8c67c154,0x8c67c31c,0x8c8a2d7c
OUT,OUT2=allocation.OUT,allocation.OUT2
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def case(entry=FRAME,**fixture):
 mode=fixture.pop('fpu_mode',0)
 args=fixture.pop('arguments',[OUT,0x12345678,33,0,0] if entry==FRAME else [OUT,OUT2,0,0x13579bdf])
 return dict(entry=entry,mode=mode,gfx_arguments=args,allocation_fixture=dict(free_sizes=fixture.pop('free_sizes',[0x400000]),bank1_sizes=[0x400000],busy=2,exhausted=fixture.pop('exhausted',False),cache_size=fixture.pop('cache_size',0x76543210),cache_address=fixture.pop('cache_address',0x120000)),frame_fixture=fixture)
def prepare(ram,c):
 m,s,f=allocation.prepare(ram,c);x=c['frame_fixture'];allocation.patch(m,CTX,bytes(0x70))
 for offset,value in ((0x18,0),(0x1c,OUT),(0x20,0x12345678),(0x24,33),(0x28,0),(0x2c,OUT2),(0x30,0x87654321),(0x34,33),(0x38,0),(0x3c,OUT),(0x40,OUT2),(0x44,0x13579bdf),(0x48,8),(0x4c,8),(0x50,1),(0x54,x.get('recalculate',0)),(0x60,x.get('bytes',64))):v.store(m,CTX+offset,value)
 for address,value in ((STATE,x.get('state',0x80000000)),(SLOT,x.get('slot',0)),(FORMAT,x.get('format',0)),(0x8c8a2d74,x.get('video_mode',0)),(0x8c8a2d78,x.get('video_flags',0)),(0x8c8a3014,0xa53f1234),(0x8c8a301c,0x13200),(0x8c8a3020,0x45600),(0x8c8a3080,0x11110000),(0x8c8a3084,0x22220000),(0x8c8a3088,0x33330000),(0x8c8a308c,0x44440000),(0x8c8a2f6c,17),(0x8c8a2f70,19),(0x8c8a2ff4,0),(0x8c8a2f3c,0xabcddcba),(0x8c8a3090,0),(0x8c67c158,0)):v.store(m,address,value)
 if c['entry']==CONTEXT:
  for pointer,size,flag,base in ((OUT,64,x.get('record_mode',0),0x123000),(OUT2,x.get('second_size',64),x.get('second_mode',x.get('record_mode',0)),0x456000)):
   v.store(m,pointer+20,size);v.store(m,pointer+24,flag);v.store(m,pointer+28,base)
 return m,s,f
def cases():
 for index,fmt,recalculate,mode in itertools.product((0,1),range(6),(0,1),(0,1)):
  yield case(format=fmt,slot=index,recalculate=recalculate,arguments=[OUT,0x12345678,33,mode,0])
 for size,clear,index in itertools.product((0,64,2560,2561,5120),(0,1),(0,1)):yield case(bytes=size,slot=index,arguments=[OUT,0x87654321,0,0,clear])
 for state in (0,0x200,0xb00,0x80000001,0xffffffff):yield case(state=state,arguments=[OUT,7,33,1,1])
 for exhausted,free in itertools.product((False,True),([0],[32],[0x400000])):yield case(exhausted=exhausted,free_sizes=free)
 for pointer in (0,4,0x7ffffc,0x800000):yield case(cache_size=64,cache_address=pointer)
 for mode in (1,4):yield case(fpu_mode=mode,bytes=2560,arguments=[OUT,0x1234,0,1,1])
 for first,second in ((0,OUT2),(OUT,0),(0,0)):yield case(CONTEXT,arguments=[first,second,0,0x13579bdf])
 for record_mode,second_mode,second_size,video in itertools.product((0,1),(0,1),(64,128),(0,1,2)):
  yield case(CONTEXT,record_mode=record_mode,second_mode=second_mode,second_size=second_size,arguments=[OUT,OUT2,video,0x13579bdf])
 for state in (0,0x200,0x80000001,0xffffffff):yield case(CONTEXT,state=state)
 yield case(CONTEXT,state=0x300)
 for mode in (1,4):yield case(CONTEXT,fpu_mode=mode,arguments=[OUT,OUT2,1,0xdeadbeef])

def host_display(m,custom,height):
 flags=v.load(m,0x8c8a2d78);video=v.load(m,0x8c8a2d74);width=640 if video==1 or flags&0x10 else 320
 if custom==1:
  height=((height+31)&0xffffffff)&0xffffffe0;v.store(m,0x8c8a3014,(v.load(m,0x8c8a3014)&0xffa0ffff)|0x400000|((height>>5)&31)<<16)
 elif video!=1 and not flags&1 and flags&0x2c==0x20:height=240
 elif video!=1 and not flags&0x20:height=240
 else:height=480
 stride=v.load(m,0x8c67256c+4*v.load(m,FORMAT))>>3
 for offset,value in ((0x48,width),(0x4c,height),(0x50,stride),(0x60,(width*height*stride)&0xffffffff)):v.store(m,CTX+offset,value)
def host_cache(m,size,out):
 size=allocation.rounded(size)
 if v.load(m,allocation.CACHE_SIZE)==size:
  v.store(m,out+20,size);v.store(m,out+28,allocation.address_map(v.load(m,allocation.CACHE_ADDRESS)+4));v.store(m,allocation.CACHE_SIZE,0);v.store(m,allocation.CACHE_ADDRESS,0);return 0
 bank=int(v.load(m,allocation.META+16)<v.load(m,allocation.META+36)) if v.load(m,allocation.CONTROL)&4 else 0
 result=allocation.host_allocate(m,bank,(size*2)&0xffffffff,out,8)
 if not result:
  pointer=v.load(m,out+28);v.store(m,allocation.CACHE_SIZE,size);v.store(m,allocation.CACHE_ADDRESS,pointer);v.store(m,out+20,size);v.store(m,out+28,allocation.address_map(pointer))
 return result
def host_frame(m,args,events,copies):
 out,tag,height,mode,clear=[x&0xffffffff for x in args];slot=v.load(m,SLOT)
 for offset,value in ((0,out),(4,tag),(8,height),(12,clear)):v.store(m,CTX+0x1c+slot*16+offset,value)
 v.store(m,CTX+0x18,mode);v.store(m,SLOT,(slot+1)&1);v.store(m,0x8c8a2d88,mode)
 if v.load(m,CTX+0x54):host_display(m,mode,height)
 if v.load(m,STATE)!=0x80000000:v.store(m,STATE,v.load(m,STATE)|0x200);return 0
 result=host_cache(m,v.load(m,CTX+0x60),out)
 if result:return result
 fmt=v.load(m,FORMAT)
 for offset,value in ((0,1),(4,v.load(m,0x8c672554+4*fmt)),(8,fmt),(12,v.load(m,CTX+0x48)),(16,v.load(m,CTX+0x4c)),(24,int(mode==1))):v.store(m,out+offset,value)
 if clear==1:
  for offset in range(0,v.load(m,out+20),0xa00):
   dest=(v.load(m,out+28)+offset)&0xffffffff;copies.append((dest,0xa00))
   events.extend([[14,(0xa5000000+dest+i)&0xffffffff,0,4] for i in range(0,0xa00,4)])
 return 0
def host_context(m,args,registers,events,copies):
 first,second,mode,tag=[x&0xffffffff for x in args]
 if not first or not second:return 14
 for offset,value in ((0x3c,first),(0x40,second),(0x44,tag),(0x18,mode)):v.store(m,CTX+offset,value)
 v.store(m,0x8c8a2d88,mode)
 if v.load(m,STATE)!=0x80000000:
  stage=v.load(m,STATE)|0x800;v.store(m,STATE,stage)
  if stage!=0xb00:return 0
  v.store(m,STATE,0x80000000)
  assert v.load(m,CTX+0x54)==0,'This oracle deliberately stages the actual mode0 path'
  for i in range(1 if v.load(m,CTX+0x1c)==v.load(m,CTX+0x2c) else 2):
   offset=CTX+0x1c+i*16;values=[v.load(m,offset+j*4) for j in range(4)];result=host_frame(m,values[:3]+[v.load(m,CTX+0x18),values[3]],events,copies)
   if result:return result
  result=host_context(m,[v.load(m,CTX+0x3c),v.load(m,CTX+0x40),v.load(m,CTX+0x18),v.load(m,CTX+0x44)],registers,events,copies)
  assert v.load(m,0x8c8a3090)!=1 and v.load(m,0x8c67c158)!=1
  return result
 if v.load(m,first+24)!=v.load(m,second+24):return 15
 if v.load(m,first+20)!=v.load(m,second+20):return 16
 one,two=v.load(m,first+28),v.load(m,second+28);old_front=v.load(m,0x8c8a301c)
 for address,value in ((0x8c8a3080,one),(0x8c8a3088,0),(0x8c8a3084,two),(0x8c8a308c,0)):v.store(m,address,value)
 registers.extend([[7,0xa05f8000+offset,value,4] for offset,value in ((0x50,old_front),(0x54,old_front),(0x60,one),(0x64,two))])
 v.store(m,0x8c8a301c,one);v.store(m,0x8c8a3020,two)
 if mode==1:v.store(m,0x8c8a2f6c,0);v.store(m,0x8c8a2f70,0)
 registers.extend([[7,0xa05f8050,one,4],[7,0xa05f8054,two if mode==1 else one,4]])
 v.store(m,0x8c8a2ff4,1);v.store(m,0x8c8a2f3c,tag);return 0
def semantic(m,a,before,fp,s,f,r,c):
 assert r['full_state_match'] and r['ordered_boundaries_match'];checks=2
 assert r['status']==0 and s.pc==s.pr==before.pr and list(s.r[8:])==list(before.r[8:]) and bytes(f)==fp;checks+=4
 expected=bytearray(m);events=[];copies=[];registers=[]
 result=host_frame(expected,c['gfx_arguments'],events,copies) if c['entry']==FRAME else host_context(expected,c['gfx_arguments'],registers,events,copies)
 assert s.r[0]==result;checks+=1
 for address,size in ((allocation.META,0x40),(allocation.POOL,allocation.POOL_BYTES),(CTX,0x70),(OUT,128),(OUT2,128),(STATE,4),(SLOT,4),(0x8c8a2d74,0x18),(0x8c8a3014,16),(0x8c8a3080,16),(0x8c8a2f6c,8),(0x8c8a2ff4,4),(0x8c8a2f3c,4)):
  assert v.sample(a,address,size)==v.sample(expected,address,size),(hex(c['entry']),c,'state oracle',hex(address));checks+=1
 assert r['vram_writes']==events and r['register_writes']==registers;checks+=2
 assert [(b['arguments'][0],b['arguments'][2]) for b in r['boundaries'] if b['target']=='0x8c65347c']==copies;checks+=1
 assert not any(r['sound_state']) and r['runtime_state'] is None and r['flash_storage_snapshot'] is None;checks+=3
 return checks
def main():
 ram=v.seed_ram();seen=set();edges=set();records=[];checks=0
 for i,data in enumerate(cases(),1):
  c=v.Input(data);m,s,f=prepare(ram,c);before=v.effects.State.from_buffer_copy(s);fp=bytes(f)
  r,path,a,s,f=fast.compare(m,s,f,c);n=semantic(m,a,before,fp,s,f,r,c);checks+=n;seen.update(path)
  for j,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
  records.append(dict(input=dict(c),status=r['status'],stop_pc=r['stop_pc'],r0=s.r[0],independent_checks=n,boundaries=len(r['boundaries']),vram_word_writes=len(r['vram_writes']),register_writes=r['register_writes']));print(i,hex(c['entry']),s.r[0],flush=True)
 coverage=[]
 for body in v.manifest['routines']:
  wanted=set(body['reachable_addresses']);actual={hex(pc) for pc in seen};want_edges=set(map(tuple,body['conditional_edges']));actual_edges={(hex(x),hex(y)) for x,y in edges}
  coverage.append(dict(address=body['address'],static_rows=len(wanted),covered_rows=len(wanted&actual),conditional_outcomes=len(want_edges),covered_outcomes=len(want_edges&actual_edges),missing_rows=sorted(wanted-actual),missing_outcomes=[list(e) for e in sorted(want_edges-actual_edges)]))
 files=[Path(__file__),Path(allocation.__file__),Path(v.__file__),Path(fast.__file__),v.MP,v.GRAPHICS_CONFIG_FIXTURE_PATH,P/'analysis/movie_graphics_init_connection.json']
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Original framebuffer creation and paired-context installation with actual cached/pool/list/address/display/copy/MMIO providers;pending-state path invokes real mode0 code rather than a substituted result. Independent initialmetadata/record/pool/arithmetic/orderedVRAMzero-word/copy-call/MMIOregister oracles supplement fullCPU/FPU/RAM/VRAM/Sound/device/ordered boundaries. Directfixtures are declared validmappedalignedrecords/pools,notguestcheckpoints;broadercallbacks/modes/root/fullstartup/hardware/pixels/playability remain unqualified.')
 (P/'analysis/movie_graphics_framebuffers_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Framebuffer/context cases',len(records),'checks',checks);print([r for r in coverage if r['address'] in ('0x8c665ae0','0x8c641c16','0x8c657760')])
if __name__=='__main__':main()
