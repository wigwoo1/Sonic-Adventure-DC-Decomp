"""Verify original list/header and display builders with independent data oracles."""
import hashlib,itertools,json
from pathlib import Path
import write_watch_graphics_init as fast
v=fast.old;P=v.P;ROOT=v.ROOT
HEADER,POINTERS,DISPLAY=0x8c657240,0x8c6574b8,0x8c665cc0
FORCE,FMT,MASK=0x8c8a3090,0x8c8a316c,0x8c8a31b4
TABLE,SIZES,BYTES,BASE=0x8c8a3398,0x8c8a33f4,0x8c8a3210,0x8c8a318c
DEST=0x8cda0000
NEW=(HEADER,POINTERS,DISPLAY)
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def words(values):return list(b''.join((x&0xffffffff).to_bytes(4,'little') for x in values))
def header_case(mask=31,count=2,force=0,fmt=31,kind='header',**extra):
 pointers=[DEST+i*0x100+j*0x40 for i in range(4) for j in range(5)]
 alias=extra.pop('alias',False)
 if alias:pointers=[DEST]*20
 if extra.pop('vram',False):pointers=([0xa5100000]*20 if alias else [0xa5100000+i*0x100+j*0x40 for i in range(4) for j in range(5)])
 return dict(entry=HEADER,kind=kind,gfx_arguments=[count],gfx_global_words=[(MASK,mask),(FORCE,force),(FMT,fmt)],gfx_memory_patches=[(TABLE,words(pointers)),(SIZES,words([0x11110000+i for i in range(5)])),(DEST,[0xa5]*0x500)],**extra)
def pointer_case(args,count=2,base=0x12340003,force=0,fmt=31,kind='pointers',**extra):
 # Original caller pushes three stack words. SP+4 is unused here; count is SP+8.
 unused=extra.pop('unused_stack_argument',0)
 return dict(entry=POINTERS,kind=kind,gfx_arguments=list(args)+[unused,count],gfx_global_words=[(BASE,base),(FORCE,force),(FMT,fmt)],gfx_memory_patches=[(TABLE,[0x5a]*0x50)],**extra)
def display_case(video_mode=0,flags=0,index=0,custom=0,height=1,default=0,kind='display',**extra):
 return dict(entry=DISPLAY,kind=kind,gfx_arguments=[index,custom,height],gfx_global_words=[(0x8c8a2d74,video_mode),(0x8c8a2d78,flags),(0x8c8a2d7c,default),(0x8c8a3014,0xa53f1234)],**extra)
def cases():
 for mask,count in itertools.product(range(32),(0,1,2)):yield header_case(mask,count)
 for force,fmt,count in itertools.product((1,2),range(31,38),(0,2)):yield header_case(force=force,fmt=fmt,count=count,kind='header_override')
 for alias,vram,mode in itertools.product((False,True),(False,True),(0,1,4)):yield header_case(alias=alias,vram=vram,mode=mode,kind='header_storage')
 yield header_case(count=4,kind='header_four_buffers')
 for bits,count in itertools.product(range(32),(0,1,2)):
  yield pointer_case([9 if bits>>i&1 else 0 for i in range(5)],count=count)
 for force,fmt,args in itertools.product((0,1,2),range(31,38),([0]*5,[1,7,8,23,32])):yield pointer_case(args,force=force,fmt=fmt,kind='pointer_override')
 for base,args in itertools.product((0,1,31,32,0xffffffe0,0xffffffff),([1,7,8,9,31],[0xffffffff,0x80000000,0x7fffffff,0,0x10000000])):yield pointer_case(args,base=base,kind='pointer_wrap')
 for mode in (1,4):yield pointer_case([9]*5,mode=mode,kind='pointer_fpu')
 for unused in (1,0xdeadbeef,0xffffffff):yield pointer_case([9]*5,unused_stack_argument=unused,kind='pointer_unused_stack_word')
 for mode,flags,index in itertools.product((0,1,2),(0,1,4,8,0x10,0x20,0x21,0x24,0x28,0x30,0xffffffff),range(6)):yield display_case(mode,flags,index)
 for index,default in itertools.product((0xff,),range(6)):yield display_case(index=index,default=default,kind='display_default_format')
 for height in (0,1,31,32,33,1023,1024,0x7fffffff,0xfffffff0,0xffffffff):yield display_case(video_mode=1,custom=1,height=height,kind='display_custom_height')
 for mode in (1,4):yield display_case(flags=0x30,index=0xff,default=5,custom=1,height=33,mode=mode,kind='display_fpu')

def semantic(m,a,before,fp,s,f,r,c):
 assert r['full_state_match'] and r['ordered_boundaries_match'];n=2
 assert r['status']==0 and s.pc==s.pr==before.pr and list(s.r[8:])==list(before.r[8:]) and bytes(f)==fp;n+=4
 entry=c['entry'];args=[x&0xffffffff for x in c['gfx_arguments']]
 if entry==HEADER:
  count=args[0];mask=v.load(m,MASK);force=v.load(m,FORCE);fmt=v.load(m,FMT);template=v.sample(m,v.load(m,0x8c657440),32);expected=bytearray(m);sizes=[v.load(m,SIZES+4*i) for i in range(5)];events=[];calls=[]
  for j in range(5):
   if not mask>>j&1 or (force!=0 and fmt==32+j):continue
   data=(template if j==0 else (0x80000000+j*0x1000000).to_bytes(4,'little')+template[4:])
   for i in range(count):
    dest=v.load(m,TABLE+i*20+j*4);calls.append((dest,32))
    if dest&0x1f000000==0x0c000000:expected[dest&0xffffff:(dest&0xffffff)+32]=data
    else:events.extend([[14,dest+k,int.from_bytes(data[k:k+4],'little'),4] for k in range(0,32,4)])
   sizes[j]=8
  assert v.sample(a,DEST,0x500)==v.sample(expected,DEST,0x500);n+=1
  assert [v.load(a,SIZES+4*j) for j in range(5)]==sizes;n+=1
  assert [(b['arguments'][0],b['arguments'][2]) for b in r['boundaries'] if b['target']=='0x8c65253e']==calls;n+=1
  assert r['vram_writes']==events;n+=1
 elif entry==POINTERS:
  sizes=args[:5];count=args[6];force=v.load(m,FORCE);fmt=v.load(m,FMT);raw_base=v.load(m,BASE);expected=bytearray(m)
  for j,size in enumerate(sizes):v.store(expected,BYTES+4*j,(size*4)&0xffffffff)
  base=raw_base
  for i in range(count):
   base=((base+31)&0xffffffe0) if base&31 else base;used=0
   for j,size in enumerate(sizes):
    if force==1 and fmt==32+j:value=0x10000000
    elif size==0:value=0
    else:value=(base+used*4)&0xffffffff;used=((used+size+23)&0xffffffff)&0xfffffff8
    v.store(expected,TABLE+i*20+j*4,value)
   base=(base+used*4)&0xffffffff
  assert s.r[0]==0 and v.sample(a,BYTES,20)==v.sample(expected,BYTES,20) and v.sample(a,TABLE,0x50)==v.sample(expected,TABLE,0x50);n+=3
 elif entry==DISPLAY:
  mode=v.load(m,0x8c8a2d74);flags=v.load(m,0x8c8a2d78);index,custom,raw_height=args;width=640 if mode==1 or flags&0x10 else 320
  if custom==1:height=((raw_height+31)&0xffffffff)&0xffffffe0
  elif mode!=1 and not flags&1 and flags&0x2c==0x20:height=240
  elif mode!=1 and not flags&0x20:height=240
  else:height=480
  index=v.load(m,0x8c8a2d7c) if index==0xff else index;stride=v.load(m,v.load(m,0x8c665d68)+index*4)>>3;context=v.load(m,0x8c665dec);total=(width*height*stride)&0xffffffff
  assert [v.load(a,context+offset) for offset in (0x48,0x4c,0x50,0x60)]==[width,height,stride,total];n+=4
  register=v.load(m,0x8c8a3014)
  if custom==1:register=(register&0xffa0ffff)|0x400000|((height>>5)&31)<<16
  assert v.load(a,0x8c8a3014)==register and r['macl']==total;n+=2
 else:raise AssertionError(hex(entry))
 assert not any(r['sound_state']) and r['runtime_state'] is None and r['flash_storage_snapshot'] is None;n+=3
 return n

def main():
 ram=v.seed_ram();seen=set();edges=set();records=[];checks=0
 for i,case in enumerate(cases(),1):
  c=v.Input(dict(mode=0)|case);m,s,f=v.prepare(ram,c);before=v.effects.State.from_buffer_copy(s);fp=bytes(f)
  r,path,a,s,f=fast.compare(m,s,f,c);n=semantic(m,a,before,fp,s,f,r,c);checks+=n;seen.update(path)
  for j,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
  records.append(dict(input=dict(c),status=r['status'],stop_pc=r['stop_pc'],independent_checks=n,boundaries=len(r['boundaries'])));print(i,hex(c['entry']),c['kind'],flush=True)
 coverage=[]
 for body in v.manifest['routines']:
  if int(body['address'],16) not in NEW:continue
  wanted=set(body['reachable_addresses']);actual={hex(pc) for pc in seen};want_edges=set(map(tuple,body['conditional_edges']));actual_edges={(hex(x),hex(y)) for x,y in edges}
  coverage.append(dict(address=body['address'],static_rows=len(wanted),covered_rows=len(wanted&actual),conditional_outcomes=len(want_edges),covered_outcomes=len(want_edges&actual_edges),missing_rows=sorted(wanted-actual),missing_outcomes=[list(e) for e in sorted(want_edges-actual_edges)]))
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),v.MP,v.GRAPHICS_CONFIG_FIXTURE_PATH,P/'analysis/movie_graphics_init_connection.json',P/'verify_movie_clear.py']
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Three original graphics builders with independent header bytes, exact copy calls/VRAM word writes, mask/count/override metadata, aligned pointer/size/wrap arithmetic, format/default/display dimensions/register-shadow/total-size oracles. Whole CPU/FPU/RAM/VRAM/Sound/device/ordered boundaries compare. Declared direct input fixtures,not guest checkpoints/helper results. Full conditional/instruction coverage remains measured,not assumed. No visible rendering/physical timing/playability claim.')
 (P/'analysis/movie_graphics_buffers_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Graphics buffer cases',len(records),'checks',checks);print(coverage)
if __name__=='__main__':main()
