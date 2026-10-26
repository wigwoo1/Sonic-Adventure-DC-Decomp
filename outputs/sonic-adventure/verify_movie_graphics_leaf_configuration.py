"""Original graphics bitfields,address mapping and reused list adapters with oracles."""
import hashlib,itertools,json
from pathlib import Path
import write_watch_graphics_init as fast
v=fast.old;P=v.P;ROOT=v.ROOT
FORCE,FMT,MASK,CONTROL,OPAQUE=0x8c8a3090,0x8c8a316c,0x8c8a31b4,0x8c8a2d5c,0x8c8a31b0
REGION=0x8cd90000
def cases():
 for bits in range(32):yield dict(entry=0x8c656f50,kind='list_mask',gfx_arguments=[(bits>>i)&1 for i in range(5)],gfx_global_words=[(FORCE,0)])
 for fmt,args in itertools.product(range(32,37),([0]*5,[1]*5,[-1]*5)):yield dict(entry=0x8c656f50,kind='list_mask_force',gfx_arguments=args,gfx_global_words=[(FORCE,1),(FMT,fmt)])
 for mode in (1,4):yield dict(entry=0x8c656f50,kind='list_mask_fpu',mode=mode,gfx_arguments=[1]*5)
 for bits,compact in itertools.product(range(32),(0,1)):
  args=[(bits>>i)&1 for i in range(5)]+[compact]
  yield dict(entry=0x8c657096,kind='list_control',gfx_arguments=args,gfx_global_words=[(FORCE,0),(CONTROL,0x20),(OPAQUE,0xdeadbeef)])
 for fmt,compact,args in itertools.product(range(31,38),(0,1),([0]*5,[1]*5)):
  yield dict(entry=0x8c657096,kind='list_control_force',gfx_arguments=args+[compact],gfx_global_words=[(FORCE,1),(FMT,fmt),(CONTROL,0x20),(OPAQUE,0xdeadbeef)])
 for args in ([1,1,2,1,1,0],[2,1,1,1,1,0],[0xffffffff,0,1,0,0,0]):yield dict(entry=0x8c657096,kind='list_control_order',gfx_arguments=args)
 for mode in (1,4):yield dict(entry=0x8c657096,kind='list_control_fpu',mode=mode,gfx_arguments=[1]*6)
 for fields,flags,opaque in itertools.product((0,0x07fe0000,0x05540000),(0,0x20),(0,1)):
  yield dict(entry=0x8c6573c4,kind='list_space',gfx_arguments=[fields],gfx_global_words=[(0x8c8a31ac,7),(0x8c8a31a8,9),(CONTROL,flags),(OPAQUE,opaque)])
 for value in tuple(range(16))+(0x7ffffc,0x7fffff,0x800000,0x800004,0xffffff,0x80000000,0xffffffff):yield dict(entry=0x8c66563e,kind='address_map',gfx_arguments=[value])
 for entry,value,mode in itertools.product((0x8c6578e0,0x8c657960,0x8c657968),(0,0x12345678,0xffffffff),(0,1,4)):
  yield dict(entry=entry,kind='reused_global',mode=mode,gfx_arguments=[value])
 for nonempty,mode in itertools.product((False,True),(0,1,4)):
  b=bytearray([0x5a])*0x200
  def put(off,value):b[off:off+4]=value.to_bytes(4,'little')
  put(0,REGION+0x40 if nonempty else 0);put(4,REGION+0x40 if nonempty else 0);put(0x44,0);put(0x48,0)
  yield dict(entry=0x8c664b12,kind='reused_insert',mode=mode,gfx_arguments=[REGION,REGION+4,REGION+0x80],gfx_memory_patches=[(REGION,list(b))])
 for prev,next_,mode in itertools.product((0,REGION+0x40),(0,REGION+0xc0),(0,1,4)):
  b=bytearray([0x5a])*0x200
  def put(off,value):b[off:off+4]=value.to_bytes(4,'little')
  put(0,prev or REGION+0x80);put(4,next_ or REGION+0x80);put(0x84,prev);put(0x88,next_)
  if prev:put(0x44,0);put(0x48,REGION+0x80)
  if next_:put(0xc4,REGION+0x80);put(0xc8,0)
  yield dict(entry=0x8c664b52,kind='reused_remove',mode=mode,gfx_arguments=[REGION,REGION+4,REGION+0x80],gfx_memory_patches=[(REGION,list(b))])
def semantics(m,a,before,fp,s,f,r,c):
 assert r['full_state_match'] and r['ordered_boundaries_match'];checks=2
 assert r['status']==0 and s.pc==s.pr==before.pr and list(s.r[8:])==list(before.r[8:]) and bytes(f)==fp;checks+=4
 entry=c['entry'];args=[x&0xffffffff for x in c['gfx_arguments']]
 if entry==0x8c656f50:
  force=v.load(m,FORCE);fmt=v.load(m,FMT);values=[int(((x+int(force==1 and fmt==32+i))&0xffffffff)!=0) for i,x in enumerate(args)]
  assert s.r[0]==sum(values) and v.load(a,MASK)==sum(x<<i for i,x in enumerate(values));checks+=2
 elif entry==0x8c657096:
  p0,p1,p2,p3,p4,compact=args;types=[(2 if compact==1 else 3) if p0 else 0,int(p1!=0),(2 if compact==1 else 3) if p2 else 0,int(p3!=0),2 if p4 else 0]
  if compact!=1 and p0 and p2:types[0 if p0<p2 else 2]=2
  if v.load(m,FORCE)==1 and 32<=v.load(m,FMT)<=36:
   index=v.load(m,FMT)-32;types[index]=(3 if index in (0,2,4) else 2)
  result=v.load(m,0x8c8a2dbc)&255
  for value,shift,mask in zip(types,(17,19,21,23,25),(0x60000,0x180000,0x600000,0x1800000,0x6000000)):result|=value<<shift&mask
  result|=0x10000;assert s.r[0]==result;checks+=1
  wanted=int(all(types[i]!=0 for i in (1,2,4))) if v.load(m,CONTROL)&0x20 else v.load(m,OPAQUE)
  assert v.load(a,OPAQUE)==wanted;checks+=1
 elif entry==0x8c6573c4:
  table=v.load(m,0x8c65746c);count=sum(v.load(m,table+4*((args[0]>>shift)&3)) for shift in (17,19,21,23,25))
  multiplier=(2 if not v.load(m,OPAQUE) else 4) if v.load(m,CONTROL)&0x20 else 1
  wanted=((count*4+24)*v.load(m,0x8c8a31ac)*v.load(m,0x8c8a31a8)*multiplier)&0xffffffff
  assert v.load(a,0x8c8a3370)==wanted;checks+=1
 elif entry==0x8c66563e:
  value=args[0];wanted=(((value&0xfffffff8)>>1)+(value&3)+(0x400000 if value&4 else 0)+(0x400000 if value>=0x800000 else 0))&0xffffffff
  assert s.r[0]==wanted;checks+=1
 elif entry in (0x8c6578e0,0x8c657960,0x8c657968):
  location=v.load(m,0x8c657950 if entry==0x8c6578e0 else 0x8c657a00)
  wanted=(v.load(m,location)|args[0]) if entry==0x8c6578e0 else args[0] if entry==0x8c657960 else v.load(m,location)
  assert s.r[0]==wanted and v.load(a,location)==wanted;checks+=2
 elif entry==0x8c664b12:
  expected=bytearray(m);head=v.load(m,REGION);node=REGION+0x80
  if head:
   v.store(expected,node+4,v.load(m,head+4));v.store(expected,node+8,head);v.store(expected,head+4,node);v.store(expected,REGION,node)
  else:
   v.store(expected,node+4,0);v.store(expected,node+8,0);v.store(expected,REGION,node);v.store(expected,REGION+4,node)
  assert v.sample(a,REGION,0x200)==v.sample(expected,REGION,0x200);checks+=1
 elif entry==0x8c664b52:
  expected=bytearray(m);prev=v.load(m,REGION+0x84);next_=v.load(m,REGION+0x88)
  v.store(expected,prev+8 if prev else REGION,next_);v.store(expected,next_+4 if next_ else REGION+4,prev)
  assert v.sample(a,REGION,0x200)==v.sample(expected,REGION,0x200);checks+=1
 else:raise AssertionError(hex(entry))
 assert not any(r['sound_state']) and r['runtime_state'] is None and r['flash_storage_snapshot'] is None;checks+=3
 return checks
def main():
 ram=v.seed_ram();coverage=set();edges=set();records=[];checks=0
 for i,case in enumerate(cases(),1):
  c=v.Input(dict(mode=0)|case);m,s,f=v.prepare(ram,c);before=v.effects.State.from_buffer_copy(s);fp=bytes(f)
  r,path,a,s,f=fast.compare(m,s,f,c);n=semantics(m,a,before,fp,s,f,r,c);checks+=n;coverage.update(path)
  for j,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
  records.append(dict(input=dict(c),status=r['status'],stop_pc=r['stop_pc'],r0=hex(s.r[0]),independent_checks=n));print(i,hex(c['entry']),c['kind'],flush=True)
 cov=[]
 for body in v.manifest['routines']:
  if not any(int(body['address'],16)==c['input']['entry'] for c in records):continue
  own=set(body['reachable_addresses']);want=set(map(tuple,body['conditional_edges']));seen={hex(a) for a in coverage};actual={(hex(a),hex(b)) for a,b in edges}
  cov.append(dict(address=body['address'],static_rows=len(own),covered_rows=len(own&seen),conditional_outcomes=len(want),covered_outcomes=len(want&actual),missing_rows=sorted(own-seen),missing_outcomes=[list(e) for e in sorted(want-actual)]))
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),v.MP,v.GRAPHICS_CONFIG_FIXTURE_PATH,P/'analysis/movie_graphics_init_connection.json']
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,coverage_by_routine=cov,dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Direct original/native list-mask/control/space/address mapping and reused global/list adapters. Independent mathematical bitfield/size/address and linked-list graph oracles supplement whole machine/memory/device/boundary state. Declared configuration data fixtures,not guest checkpoints. Coverage measured per body;incomplete bodies remain unqualified. Reused canonical routines receive zero duplicate credit;no physical rendering/playability claim.')
 (P/'analysis/movie_graphics_leaf_configuration_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Graphics leaf cases',len(records),'checks',checks);print([(x['address'],x['covered_rows'],x['static_rows'],x['covered_outcomes'],x['conditional_outcomes']) for x in cov])
if __name__=='__main__':main()
