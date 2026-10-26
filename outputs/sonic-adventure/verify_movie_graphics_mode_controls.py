"""Original mode/list/texture controls with explicit metadata and independent contracts."""
import hashlib,itertools,json
from pathlib import Path
import verify_movie_graphics_framebuffers as frame
fast=frame.fast;v=frame.v;P=v.P;ROOT=v.ROOT
ONE,TWO,MODE=0x8c65614e,0x8c656660,0x8c657760
OBJ,LIST_BASE=0x8cdb0400,0x8cdc1003
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def case(entry=ONE,counts=(32,32,0,0),request=0x80000,selected=0,**fixture):
 args=[OBJ,*counts] if entry==ONE else [OBJ,request] if entry==TWO else [selected]
 c=frame.case(entry,arguments=args,**fixture);c['mode_fixture']=dict(counts=list(counts),request=request,selected=selected);return c
def prepare(ram,c):
 m,s,f=frame.prepare(ram,c);x=c['frame_fixture'];modes=c['mode_fixture'];frame.allocation.patch(m,OBJ,bytes([0x77])*64)
 v.store(m,OBJ+16,x.get('object_mask',15));v.store(m,frame.CTX,OBJ)
 for i,value in enumerate(modes['counts'],1):v.store(m,frame.CTX+i*4,value)
 v.store(m,frame.CTX+0x18,x.get('video_argument',0));v.store(m,frame.CTX+0x54,x.get('mode_setup',0));v.store(m,frame.CTX+0x58,modes['request']);v.store(m,frame.CTX+0x64,x.get('initial_list_space',0))
 if 'second_height' in x:v.store(m,frame.CTX+0x34,x['second_height'])
 for address,value in ((0x8c8a318c,LIST_BASE),(0x8c8a3090,x.get('force',0)),(0x8c8a316c,x.get('list_format',32)),(0x8c67c158,x.get('engine',0)),(frame.allocation.CONTROL,x.get('control_flags',0)),(0x8c8a30a8,x.get('slot',0)),(0x8c8a30b8,0x55),(0x8c8a30b4,0x1234),(0x8c8a31a4,0x5678)):
  v.store(m,address,value)
 for i,value in enumerate((128,256,512,1024)):v.store(m,0x8c8a2dc0+i*4,value)
 for i in (13,14,15):frame.allocation.flags(m,frame.allocation.POOL+i*24,1 if c['allocation_fixture'].get('exhausted') else 0)
 if 'r12_value' in x:s.r[12]=x['r12_value']
 return m,s,f
def cases():
 for bits,force in itertools.product(range(16),(0,1)):
  yield case(counts=[32 if bits>>i&1 else 0 for i in range(4)],force=force)
 for counts,flags,addon in itertools.product(((16,32,64,128),(65536,0,0,0),(65537,0,0,0),(0x80000,0,0,0)),(0,0x40),(0,0x200000)):
  yield case(counts=counts,control_flags=flags,initial_list_space=addon)
 for force,fmt in itertools.product((0,1,2),range(32,37)):yield case(force=force,list_format=fmt)
 for request,mask,video in itertools.product((0,1,0x80000,0x80000000,0xffffffff),(0,15),(0,1)):yield case(TWO,request=request,object_mask=mask,video_argument=video)
 for entry,state in itertools.product((ONE,TWO),(0,0x200,0xa00,0xb00)):yield case(entry,state=state)
 for entry,exhausted,free in itertools.product((ONE,TWO),(False,True),([0],[0x400000])):yield case(entry,exhausted=exhausted,free_sizes=free)
 for selected,force,engine,same in itertools.product((0,1,2,3),(0,1),(0,1),(False,True)):
  yield case(MODE,selected=selected,force=force,engine=engine,same_frame_pointer=same)
 for selected,state in itertools.product((0,1,2),(0,0x100,0x200,0xa00)):yield case(MODE,selected=selected,state=state)
 for selected,free,exhausted in itertools.product((0,1,2),([0],[128],[0x400000]),(False,True)):yield case(MODE,selected=selected,free_sizes=free,exhausted=exhausted)
 for entry,mode in itertools.product((ONE,TWO,MODE),(1,4)):yield case(entry,selected=1,fpu_mode=mode)
 for entry,value in itertools.product((TWO,MODE),(0,1,0x12345678,0xffffffff)):yield case(entry,selected=2,state=0,r12_value=value)
 for error in (14,15,16):yield case(MODE,selected=0,force=1,context_error=error)
 yield case(MODE,selected=0,mode_setup=1,video_argument=1,second_height=4096)
 yield case(MODE,selected=2,video_argument=1,second_height=4096)
def list_space(m,counts,compact):
 p0,p1,p2,p3,p4=counts;types=[(2 if compact else 3) if p0 else 0,int(bool(p1)),(2 if compact else 3) if p2 else 0,int(bool(p3)),2 if p4 else 0]
 if not compact and p0 and p2:types[0 if p0<p2 else 2]=2
 if v.load(m,0x8c8a3090)==1 and 32<=v.load(m,0x8c8a316c)<=36:
  index=v.load(m,0x8c8a316c)-32;types[index]=3 if index in (0,2,4) else 2
 flags=v.load(m,frame.allocation.CONTROL);opaque=int(all(types[i]!=0 for i in (1,2,4))) if flags&0x20 else v.load(m,0x8c8a31b0)
 multiplier=(4 if opaque else 2) if flags&0x20 else 1;table=v.load(m,0x8c65746c)
 return ((sum(v.load(m,table+4*t) for t in types)*4+24)*v.load(m,0x8c8a31ac)*v.load(m,0x8c8a31a8)*multiplier)&0xffffffff
def host_mode_zero(m):
 registers=[];events=[];copies=[];calls=[];count=1 if v.load(m,frame.CTX+0x1c)==v.load(m,frame.CTX+0x2c) else 2
 for i in range(count):
  base=frame.CTX+0x1c+i*16;values=[v.load(m,base+4*j) for j in range(4)];args=values[:3]+[v.load(m,frame.CTX+0x18),values[3]];calls.append(args[:4]);result=frame.host_frame(m,args,events,copies)
  if result:return result,registers,events,copies,calls
 result=frame.host_context(m,[v.load(m,frame.CTX+0x3c),v.load(m,frame.CTX+0x40),v.load(m,frame.CTX+0x18),v.load(m,frame.CTX+0x44)],registers,events,copies)
 if result==0 and (v.load(m,0x8c8a3090)==1 or v.load(m,0x8c67c158)==1):
  slot=v.load(m,0x8c8a30a8);v.store(m,0x8c8a30b0,slot);v.store(m,0x8c8a3098+slot*4,0);v.store(m,0x8c8a30c0,1)
  registers.extend([[7,0xa05f8000+offset,value,4] for offset,value in [(8,1),(8,0)]+[(offset,v.load(m,pointer+(slot&1)*32)) for offset,pointer in ((0x124,0x8c8a31c4),(0x12c,0x8c8a31c8),(0x128,0x8c8a31d0),(0x130,0x8c8a31d4),(0x164,0x8c8a31cc))]+[(0x144,0x80000000)]])
  v.store(m,0x8c8a30b8,0);v.store(m,0x8c8a30b4,v.load(m,0x8c8a31b4))
 return result,registers,events,copies,calls
def semantic(m,a,before,fp,s,f,r,c):
 assert r['full_state_match'] and r['ordered_boundaries_match'];checks=2
 if r['status']:
  # A nonzero list of exactly16 words becomes zero after the original header
  # reservation,while its enable mask remains set. Preserve the real null copy.
  assert r['status']==-3 and r['stop_pc']=='0x8c652544' and c['entry']==ONE
  counts=c['mode_fixture']['counts'];first=(counts[0]+(32 if v.load(m,frame.allocation.CONTROL)&0x40 else 0))&0xffffffff
  assert any(n==16 for n in [first]+counts[1:]) and r['boundaries'][-1]['target']=='0x8c65253e' and r['boundaries'][-1]['arguments'][0]==0 and r['boundaries'][-1]['arguments'][2]==32;checks+=4
  assert v.load(a,OBJ)==v.load(m,OBJ) and not any(r['sound_state']) and r['runtime_state'] is None;checks+=3
  return checks
 assert r['status']==0 and s.pc==s.pr==before.pr and list(s.r[8:])==list(before.r[8:]) and bytes(f)==fp;checks+=4
 entry=c['entry'];active=v.load(m,frame.STATE)==0x80000000;counts=c['mode_fixture']['counts'];requested=c['mode_fixture']['request'];x=c['frame_fixture']
 # Independent entry contract applies even on real allocation failure.
 if entry==ONE:
  first=(counts[0]+(32 if v.load(m,frame.allocation.CONTROL)&0x40 else 0))&0xffffffff
  assert v.load(a,frame.CTX)==OBJ and v.load(a,frame.CTX+0x54)==1;checks+=2
  if active:
   adjusted=[((n-16)&0xffffffff) if n else 0 for n in [first]+counts[1:]]
   assert [v.load(a,frame.CTX+i*4) for i in range(1,5)]==adjusted;checks+=1
   space=list_space(m,[first]+counts[1:]+[0],False);assert v.load(a,frame.CTX+0x64)==space;checks+=1
   total=(max((first+sum(counts[1:]))&0xffffffff,0x10000)*4+space)&0xffffffff
   capacity=(0x200000-v.load(a,frame.CTX+0x60))&0xffffffff;wanted=min(total,capacity)
   assert v.load(a,frame.CTX+0x58)==wanted;checks+=1
   texture=[b for b in r['boundaries'] if b['target']=='0x8c6634a0'];assert len(texture)==1 and texture[0]['arguments'][:3]==[wanted,0x8c8a33ec,0x8c8a33f0];checks+=1
   if s.r[0]==0:
    assert [v.load(a,OBJ+i*4) for i in range(4)]==[0x8c8a33c0,0x8c67c014,0x8c8a3374,0x8c8a3390];checks+=1
    assert v.load(a,0x8c8a31a4)==wanted;checks+=1
 elif entry==TWO:
  assert v.load(a,frame.CTX)==OBJ and v.load(a,frame.CTX+0x54)==2;checks+=2
  if active:
   capacity=(0x200000-v.load(a,frame.CTX+0x60))&0xffffffff;wanted=min(requested>>1,capacity)
   assert v.load(a,frame.CTX+0x58)==wanted;checks+=1
   texture=[b for b in r['boundaries'] if b['target']=='0x8c6634a0'];assert len(texture)==1 and texture[0]['arguments'][:3]==[wanted,0x8c8a33ec,0x8c8a33f0];checks+=1
   if s.r[0]==0:assert v.load(a,0x8c8a31a4)==wanted and v.load(a,0x8c8a2f60)==1;checks+=2
 elif entry!=MODE:raise AssertionError(hex(entry))
 if entry==MODE and active and c['mode_fixture']['selected'] not in (1,2):
  expected=bytearray(m);result,registers,events,copies,calls=host_mode_zero(expected);assert s.r[0]==result;checks+=1
  for address,size in ((frame.allocation.META,0x40),(frame.allocation.POOL,frame.allocation.POOL_BYTES),(frame.CTX,0x70),(frame.OUT,128),(frame.OUT2,128),(OBJ,64),(0x8c8a3014,16),(0x8c8a3080,0x44),(0x8c8a2f6c,8),(0x8c8a2ff4,4),(0x8c8a2f3c,4)):
   assert v.sample(a,address,size)==v.sample(expected,address,size),('mode0 independent state',c,hex(address));checks+=1
  assert r['register_writes']==registers and r['vram_writes']==events;checks+=2
  assert [b['arguments'] for b in r['boundaries'] if b['target']==hex(frame.FRAME)]==calls;checks+=1
 deferred_two=(not active and (v.load(m,frame.STATE)|0x100)!=0xb00 and (entry==TWO or entry==MODE and c['mode_fixture']['selected']==2))
 if deferred_two and (entry==TWO or before.r[12]!=0):assert s.r[0]==before.r[12];checks+=1
 else:assert s.r[0] in (0,3,14,15,16);checks+=1
 if entry in (ONE,TWO) and active:
  # Predict actual allocator success/error from independent first-fit/pool oracle.
  expected=bytearray(m);wanted=v.load(a,frame.CTX+0x58)
  result=frame.allocation.host_allocate(expected,0,(frame.allocation.rounded(wanted)*2)&0xffffffff,0x8c8a33ec,0x2000)
  assert s.r[0]==result;checks+=1
 assert not any(r['sound_state']) and r['runtime_state'] is None;checks+=2
 return checks
def main():
 ram=v.seed_ram();seen=set();edges=set();records=[];checks=0
 for i,data in enumerate(cases(),1):
  c=v.Input(data);m,s,f=prepare(ram,c)
  if c['frame_fixture'].get('same_frame_pointer'):v.store(m,frame.CTX+0x2c,frame.OUT);v.store(m,frame.CTX+0x40,frame.OUT)
  error=c['frame_fixture'].get('context_error')
  if error==14:v.store(m,frame.CTX+0x3c,0)
  elif error in (15,16):
   v.store(m,frame.CTX+0x2c,frame.OUT);v.store(m,frame.CTX+0x40,frame.OUT2);v.store(m,frame.OUT2+24,0 if error==16 else 1);v.store(m,frame.OUT2+20,0)
  before=v.effects.State.from_buffer_copy(s);fp=bytes(f);r,path,a,s,f=fast.compare(m,s,f,c);n=semantic(m,a,before,fp,s,f,r,c);checks+=n;seen.update(path)
  for j,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
  records.append(dict(input=dict(c),status=r['status'],stop_pc=r['stop_pc'],r0=s.r[0],independent_checks=n,boundaries=len(r['boundaries'])));print(i,hex(c['entry']),s.r[0],flush=True)
 coverage=[]
 for body in v.manifest['routines']:
  wanted=set(body['reachable_addresses']);actual={hex(pc) for pc in seen};want_edges=set(map(tuple,body['conditional_edges']));actual_edges={(hex(x),hex(y)) for x,y in edges}
  coverage.append(dict(address=body['address'],static_rows=len(wanted),covered_rows=len(wanted&actual),conditional_outcomes=len(want_edges),covered_outcomes=len(want_edges&actual_edges),missing_rows=sorted(wanted-actual),missing_outcomes=[list(e) for e in sorted(want_edges-actual_edges)]))
 files=[Path(__file__),Path(frame.__file__),Path(frame.allocation.__file__),Path(v.__file__),Path(fast.__file__),v.MP,v.GRAPHICS_CONFIG_FIXTURE_PATH,P/'analysis/movie_graphics_init_connection.json']
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Direct original mode1/mode2 list/texture setup andmode dispatcher with explicit coherent pool/list/object/framebuffer input metadata. Real providers execute;no function result/return/completion is supplied. Independent stored-count/minimum/capacity/texture-call/output-pointer/allocator-result contracts supplement fullCPU/FPU/RAM/VRAM/Sound/device/orderedboundaries. Pending stages invokeactual recursive dispatcher with originalmode-state transitions. Coverage is measured;incomplete bodies andunprovededges receive zero credit. Not a fullstartup/physicalGPU/pixels/gameplay proof.')
 (P/'analysis/movie_graphics_mode_controls_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Mode controls',len(records),'checks',checks);print([x for x in coverage if x['address'] in ('0x8c65614e','0x8c656660','0x8c657760')])
if __name__=='__main__':main()
