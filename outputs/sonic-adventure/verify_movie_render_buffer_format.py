"""Compare original descriptor/size bodies with independent field and size oracles.

These direct fixtures are not a startup checkpoint or qualification of allocators.
"""
import hashlib,itertools,json
from pathlib import Path
import write_watch_render_buffer as fast
v=fast.old;P=v.P;ROOT=v.ROOT
DESC,CONTEXT=0x8cdb0000,0x8cda0000
FORMAT,SIZE=0x8c653f08,0x8c654286
SIZERS=(0x8c6542e0,0x8c654334,0x8c654392,0x8c65441c)
MIP_TABLE=(0,0,0,128,128,512,2048,6,8,16,48)
PLAIN_TABLE=(0,0,0,128,128,256,1024,0,0,0,128)
VALID={8,16,32,64,128,256,512,1024}

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def u32(x):return x&0xffffffff
def case(entry=FORMAT,w=128,h=128,tag=0x100,low=0,**extra):
 return dict(entry=entry,mode=extra.pop('mode',0),render_arguments=[DESC,w,h,tag|low] if entry==FORMAT else [DESC],render_width=w,render_height=h,render_tag=tag,render_low=low,render_context=extra.pop('context',1),render_kind=extra.pop('kind',2),render_flags=extra.pop('flags',0),render_pixel=extra.pop('pixel',0),render_bpp=extra.pop('bpp',1))|extra

def prepare(ram,c):
 m,s,f=v.prepare(ram,c);v.store(m,0x8c67c004,CONTEXT);v.store(m,CONTEXT+0x9c,c['render_context'])
 for i in range(8):v.store(m,DESC+4*i,0xa5a50000+i)
 if c['entry']!=FORMAT:
  for i,x in enumerate((c['render_kind'],c['render_bpp'],c['render_pixel'],c['render_width'],c['render_height'],0xa5a50005,c['render_flags'])):v.store(m,DESC+i*4,x)
 return m,s,f

def size_oracle(fields,entry=SIZE):
 kind,bpp,pixel,w,h,_,flags=fields[:7];w=u32(w);h=u32(h)
 if entry==SIZE:entry=0x8c65441c if kind&0xffff==3 else 0x8c654334 if flags&8 else 0x8c654392 if flags&32 else 0x8c6542e0
 if entry in (0x8c654334,0x8c65441c):a,b=w>>1,h>>1
 else:a,b=w,h
 result=u32(a*b)
 if flags&1:
  a>>=1;b>>=1;n=min(a,b)
  while n>1:result=u32(result+a*b);a>>=1;b>>=1;n>>=1
  if entry==0x8c6542e0:result=u32(result+(4 if a==b else a*b))
  elif entry==0x8c654392:result=u32(result+(1 if a==b else a*b)+(43 if pixel==0x28000000 else 11))
  else:result=u32(result+(2 if a==b else a*b)+10)
 if entry==0x8c6542e0:result=u32(result<<(1 if bpp==1 else 2))
 elif entry==0x8c654334:result=u32(result+2048+(16 if not flags&1 and w==h==8 else 0))
 elif entry==0x8c654392 and pixel==0x28000000:result=1 if result==1 else result>>1
 elif entry==0x8c65441c:
  minimum=min(w,h);index=minimum.bit_length()-1 if minimum in {1<<i for i in range(11)} else 3
  result=u32(result+(MIP_TABLE if flags&1 else PLAIN_TABLE)[index])
 return result

def format_oracle(c,before):
 tag=c['render_tag']&0xff00;low=c['render_low']&255;w,h=c['render_width'],c['render_height'];fields=list(before);error=0
 fields[0]=3 if tag in (0x1000,0x1100) else 2
 fields[1]=3 if tag in (0x500,0x600,0x700,0x800) and c['render_context']==3 else 1
 if tag in (0x500,0x600):fields[2]=0x28000000
 elif tag in (0x700,0x800):fields[2]=0x30000000
 elif low<=4:fields[2]=low*0x08000000
 else:error=4
 fields[3:5]=[w,h]
 if w not in VALID or h not in VALID or w!=h and tag not in (0x900,0xb00,0xd00,0x500,0x700):error=4
 flags=0
 if tag in (0xf00,0x200,0x400,0x1100,0x600,0x800):flags|=1
 if tag in (0xe00,0xf00,0x100,0x200,0xd00):flags|=4
 if tag in (0x300,0x400,0x1000,0x1100):flags|=12
 if tag==0xb00:flags|=16
 if tag in (0x500,0x600,0x700,0x800):flags|=36
 fields[6]=flags;fields[5]=size_oracle(fields)
 return fields,error

def cases():
 for tag,low in itertools.product(range(0,0x1200,0x100),range(6)):yield case(tag=tag,low=low)
 for tag,context in itertools.product((0x500,0x600,0x700,0x800),(0,3,4)):yield case(tag=tag,context=context)
 for w in (8,16,32,64,256,512,1024,7,0xffffffff):yield case(w=w,h=w)
 for tag in (0x100,0x500,0x700,0x900,0xb00,0xd00,0x1000):
  for w,h in ((8,16),(256,128),(7,16)):yield case(w=w,h=h,tag=tag)
 for entry,flags,(w,h) in itertools.product(SIZERS,(0,1),((8,8),(16,32),(64,64),(1,1),(0,0))):yield case(entry,w=w,h=h,flags=flags,pixel=0x28000000)
 for kind,flags,bpp in itertools.product((2,3),(0,1,8,9,32,33),(1,3)):yield case(SIZE,w=32,h=16,kind=kind,flags=flags,bpp=bpp,pixel=0x30000000)
 for mode in (1,4):yield case(tag=0x1100,w=64,h=64,mode=mode)

def main():
 ram=v.seed_ram();records=[];seen=set();edges=set();checks=0
 for base,values in ((0x8c671b94,MIP_TABLE),(0x8c671b78,PLAIN_TABLE)):
  assert tuple(v.load(ram,base+4*i) for i in range(11))==values
 for i,data in enumerate(cases(),1):
  c=v.Input(data);m,s,f=prepare(ram,c);before=v.effects.State.from_buffer_copy(s);fp=bytes(f);fields=[v.load(m,DESC+4*j) for j in range(8)];r,path,a,s,f=fast.compare(m,s,f,c)
  assert r['full_state_match'] and r['ordered_boundaries_match'] and r['status']==0;n=3
  if c['entry']==FORMAT:
   expected,result=format_oracle(c,fields);assert [v.load(a,DESC+4*j) for j in range(8)]==expected,(i,c,expected,[v.load(a,DESC+4*j) for j in range(8)]);n+=8
  else:
   result=size_oracle(fields,c['entry']);expected=list(fields)
   if c['entry']==SIZE:expected[5]=result
   assert [v.load(a,DESC+4*j) for j in range(8)]==expected;n+=8
  assert s.r[0]==result and s.pc==s.pr==before.pr and list(s.r[8:])==list(before.r[8:]) and bytes(f)==fp;n+=5
  assert not r['vram_writes'] and not any(r['sound_state']) and r['runtime_state'] is None;n+=3;checks+=n;seen.update(path)
  for j,pc in enumerate(path):
   ins=v.ROWS.get(pc,'');distance=2 if ins.startswith(('bt/s ','bf/s ')) else 1
   if ins.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
  records.append(dict(input=dict(c),status=r['status'],stop_pc=r['stop_pc'],r0=s.r[0],steps=len(path),independent_checks=n));print(i,hex(c['entry']),result,flush=True)
 coverage=[]
 for body in v.manifest['routines']:
  wanted=set(body['reachable_addresses']);wanted_edges=set(map(tuple,body['conditional_edges']));actual={hex(a) for a in seen};actual_edges={(hex(a),hex(b)) for a,b in edges}
  coverage.append(dict(address=body['address'],static_rows=len(wanted),covered_rows=len(wanted&actual),conditional_outcomes=len(wanted_edges),covered_outcomes=len(wanted_edges&actual_edges),missing_rows=sorted(wanted-actual),missing_outcomes=[list(e) for e in sorted(wanted_edges-actual_edges)]))
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_render_buffer_connection.json',v.effects.DLL]
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Direct descriptor/size comparisons with independent field validation, format flags, dimensions, mip totals, original table bytes and result oracles. Complete CPU/FPU/RAM/VRAM/Sound/device/ordered boundaries compare. Explicit mapped aligned descriptor and context; private stack disjoint. No allocator qualification, original-startup or gameplay claim; no credit before separate qualification.')
 (P/'analysis/movie_render_buffer_format_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Descriptor/size',len(records),checks,[(x['address'],x['covered_rows'],x['static_rows'],x['covered_outcomes'],x['conditional_outcomes']) for x in coverage])
if __name__=='__main__':main()
