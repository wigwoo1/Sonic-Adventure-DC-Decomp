"""Original-code, numerical and independent model-morph comparisons."""
import collections,csv,ctypes,hashlib,itertools,json,random,re,struct,sys
from fractions import Fraction
import sh4_multiply_extended_reference as extended_multiply
from pathlib import Path
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_main_dispatch_active_query import OBSERVE
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent
MAIN=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';MODULE=ROOT/'work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN'
ENTRY,COPY=0xc904e80,0x8c10cddc
ENTITY,APP,OBJECTS=0x8cc00000,0x8cc02000,0x8cc10000
OFFSETS=(0x24c,0x314,0x3dc,0x6fc,0x4a4,0x56c,0x634)
DLL=ROOT/'work/sonic-adventure/build/overlay_model_blend.dll';LIB=ctypes.CDLL(str(DLL));P=ctypes.POINTER(ctypes.c_uint32)
fn=LIB.sa_overlay_model_blend_connected
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(State),ctypes.POINTER(Fpu),P,P,ctypes.c_uint32,ctypes.c_void_p,OBSERVE];fn.restype=ctypes.c_int
for name,args in (('sa_sh4_add_sub',[P,P,P,P,ctypes.c_int]),('sa_sh4_multiply_extended',[P,P,P,P]),('sa_sh4_divide',[P,P,P,P]),('sa_sh4_fmac',[P,P,P,P,P]),('sa_sh4_float_i32',[ctypes.c_uint32,P,P]),('sa_sh4_truncate_i32',[ctypes.c_uint32,P,P]),('sa_sh4_compare',[ctypes.c_uint32,ctypes.c_uint32,P,P,ctypes.c_int])):
 method=getattr(LIB,name);method.argtypes=args;method.restype=ctypes.c_int
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def bits(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def value(x):return struct.unpack('<f',struct.pack('<I',x))[0]
def fraction(m,a):return Fraction(value(load(m,a)))
def bytes_at(m,a,n):
 off=a&0xffffff
 return bytes(m[off:off+n]) if isinstance(m,bytearray) else ctypes.string_at(ctypes.addressof(m.contents)+off,n)
def get_rows():
 rows={}
 for listing in ('overlay-initializer-dependencies/0c904e80_instructions.tsv','overlay-model-blend-copy/8c10cddc_instructions.tsv'):
  for row in csv.DictReader((PROJECT/'analysis'/listing).open(),delimiter='\t'):rows[int(row['address'],16)]=row['instruction'].lstrip('_')
 return rows
ROWS=get_rows();COPY_TARGETS={0x8c10ce32-i for i in range(0,68,4)}
def dynamic_access(text,r,fpscr):
 # Decode disassembly operands independently of the C generator's raw opcodes.
 if text=='sts.l PR,@-r15':return (r[15]-4)&0xffffffff,4
 if text=='lds.l @r15+,PR':return r[15],4
 if match:=re.fullmatch(r'(?:mov\.(b|w|l)|fmov(?:\.s)?) (@r(\d+)(\+)?),(?:r|fr)\d+',text):
  return r[int(match[3])],{'b':1,'w':2,'l':4}.get(match[1],8 if fpscr&0x100000 else 4)
 if match:=re.fullmatch(r'(?:mov\.(b|w|l)|fmov(?:\.s)?) (?:r|fr)\d+,@(-)?r(\d+)',text):
  width={'b':1,'w':2,'l':4}.get(match[1],8 if fpscr&0x100000 else 4)
  return (r[int(match[3])]-(width if match[2] else 0))&0xffffffff,width
 if match:=re.fullmatch(r'(?:mov\.(b|w|l)|fmov(?:\.s)?) @\((r0|0x[0-9a-f]+),r(\d+)\),(?:r|fr)\d+',text):
  width={'b':1,'w':2,'l':4}.get(match[1],8 if fpscr&0x100000 else 4);offset=r[0] if match[2]=='r0' else int(match[2],16)
  return (r[int(match[3])]+offset)&0xffffffff,width
 if match:=re.fullmatch(r'(?:mov\.(b|w|l)|fmov(?:\.s)?) (?:r|fr)\d+,@\((r0|0x[0-9a-f]+),r(\d+)\)',text):
  width={'b':1,'w':2,'l':4}.get(match[1],8 if fpscr&0x100000 else 4);offset=r[0] if match[2]=='r0' else int(match[2],16)
  return (r[int(match[3])]+offset)&0xffffffff,width
 return None
def numeric_probe(w,f,sr):
 p=Fpu.from_buffer_copy(f);bank=p.fpscr>>21&1;n=w>>8&15;m=w>>4&15
 fps=ctypes.c_uint32(p.fpscr);fpul=ctypes.c_uint32(p.fpul);ssr=ctypes.c_uint32(sr)
 ptr=lambda i:ctypes.cast(ctypes.byref(p.banks[bank],i*4),P)
 rc=0
 if w&0xf0ff==0xf02d:rc=LIB.sa_sh4_float_i32(p.fpul,ctypes.byref(fps),ptr(n))
 elif w&0xf0ff==0xf03d:rc=LIB.sa_sh4_truncate_i32(p.banks[bank][n],ctypes.byref(fps),ctypes.byref(fpul))
 elif w&15==0xe:rc=LIB.sa_sh4_fmac(ptr(0),ptr(m),ptr(n),ctypes.byref(fps),ptr(n))
 elif w&15==2:rc=LIB.sa_sh4_multiply_extended(ptr(n),ptr(m),ctypes.byref(fps),ptr(n))
 elif w&15==3:rc=LIB.sa_sh4_divide(ptr(n),ptr(m),ctypes.byref(fps),ptr(n))
 elif w&15 in (0,1):rc=LIB.sa_sh4_add_sub(ptr(n),ptr(m),ctypes.byref(fps),ptr(n),w&1)
 elif w&15 in (4,5):rc=LIB.sa_sh4_compare(p.banks[bank][n],p.banks[bank][m],ctypes.byref(fps),ctypes.byref(ssr),int(w&15==5))
 p.fpscr=fps.value;p.fpul=fpul.value;return rc,p,ssr.value
class Stopped(Exception):pass
def reference_run(memory,state,f,c,tracked=(ENTRY,COPY),hooked=None):
 path=[];events=[];status=0;entry_pr=state.pr;probing=bool(c.get('probe') or f.fpscr&0x100000);special={'macl':c.get('macl',0x12345678)}
 def event(target,r,sr,pr):return target,tuple(r),sr,pr,bytes(f),special['macl'],hashlib.sha256(memory).digest()
 def stop(pc,r,sr,rc,probe=None):
  nonlocal status
  if probe is not None:ctypes.memmove(ctypes.byref(f),ctypes.byref(probe),ctypes.sizeof(Fpu))
  state.r[:]=r;state.sr=sr;state.pc=pc;status=rc;raise Stopped()
 def trace(pc,w,r,sr,pr,fp):
  assert pc in ROWS,hex(pc);path.append(pc);text=ROWS[pc]
  if pc in tracked:events.append(event(pc,r,sr,pr))
  access=dynamic_access(text,r,f.fpscr)
  if access:
   a,n=access
   if a&0x1f000000!=0xc000000 or (a&0xffffff)+n>0x1000000:stop(pc,r,sr,-3)
  if w==0xb and pr not in (entry_pr,0xc90545a):stop(pc,r,sr,-3)
  if pc==0x8c10cde8 and r[3] not in COPY_TARGETS:stop(pc,r,sr,-3)
  if w&0xf0ff in (0xf08d,0xf09d,0xf04d) and f.fpscr&0x80000:stop(pc,r,sr,-1)
  scalar=w>>12==15 and (w&15 in (0,1,2,3,4,5,14) or w&0xf0ff in (0xf02d,0xf03d))
  if scalar and probing:
   rc,probe,new_sr=numeric_probe(w,f,sr)
   if rc:stop(pc,r,new_sr,rc,probe)
 # The independent finite reference supports masked exceptions. Probe mode
 # checks backend status before an instruction, then masks enables only while
 # the independently implemented non-trapping instruction executes.
 import sh4_binary32
 names=('execute_add_sub','execute_multiply','execute_divide','execute_fmac','execute_float','execute_truncate_i32')
 saved={name:getattr(sh4_binary32,name) for name in names}
 def masked(method):
  def run(fp,*args):
   enabled=fp.fpscr&0xf80;fp.fpscr&=~0xf80;method(fp,*args);fp.fpscr|=enabled
  return run
 for name in names:
  method=extended_multiply.execute if name=='execute_multiply' else saved[name]
  setattr(sh4_binary32,name,masked(method) if probing else method)
 try:
  reference(memory,state,fpu=f,special=special,trace=trace,hooks=hooked,instruction_budget=400000)
 except Stopped:pass
 except AssertionError as exc:
  raise AssertionError((str(exc),hex(path[-1]),ROWS[path[-1]],list(map(hex,f.banks[(f.fpscr>>21)&1])))) from exc
 finally:
  for name in names:setattr(sh4_binary32,name,saved[name])
 return status,path,events,special['macl']
def fixture(ram,c):
 m=bytearray(ram);store(m,ENTITY+0xec,APP);store(m,APP,c.get('type',1),1);store(m,APP+0x36,c.get('flags',0),2)
 for i,raw in enumerate(c.get('xyz_raw',tuple(bits(v) for v in c.get('xyz',(.25,.75,.5))))):store(m,APP+0x20+i*4,raw)
 for table,off in enumerate(OFFSETS):
  for i in range(39):
   node=OBJECTS+table*0x10000+i*64;model=node+0x1000;pos=node+0x2000;normal=node+0x3000;mat=node+0x4000
   store(m,ENTITY+off+i*4,node);store(m,node+4,0 if table==0 and c.get('missing_model')==i else model)
   for axis in range(3):store(m,node+8+axis*4,bits((table*3+i+axis-16)/8))
   store(m,model,pos);store(m,model+4,normal);store(m,model+8,c.get('vertices',2));store(m,model+16,mat);store(m,model+22,c.get('materials',2),2)
   for j in range(12):
    store(m,pos+j*4,bits((table*11+i*3+j-32)/8));store(m,normal+j*4,bits((table*5-i+j-16)/16))
   for j in range(64):m[(mat&0xffffff)+j]=(table*37+i*13+j*29)&255
 if c.get('bad_table'):store(m,ENTITY+0x24c+4,0)
 return m
def independent(c,before,after):
 x,y,z=map(Fraction,c.get('xyz',(.25,.75,.5)));x=abs(x);y=abs(y);assert x and y and x+y in (Fraction(1),Fraction(2),Fraction(1,2))
 xt=5 if c['xyz'][0]>0 else 6;yt=4 if c['xyz'][1]>0 else 3
 def node(table,i):return load(before,ENTITY+OFFSETS[table]+i*4)
 def blend(vals):return (x*(x*vals[xt]+(1-x)*vals[2])+y*(y*vals[yt]+(1-y)*vals[2]))/(x+y)
 expected=bytearray(before);mesh_count=12 if c.get('type',1)==0 else 14;mesh=load(before,0xc905220);indices=[load(before,mesh+i*4) for i in range(mesh_count)]
 for i in indices:
  models=[load(before,node(t,i)+4) for t in range(7)];dest=models[0]
  for field in (0,4):
   pointers=[load(before,a+field) for a in models]
   for j in range(c.get('vertices',2)*3):
    vals=[fraction(before,a+j*4) for a in pointers];out=z*blend(vals)+(1-z)*vals[1];store(expected,pointers[0]+j*4,bits(float(out)))
  if i==27:continue
  materials=[load(before,a+16) for a in models]
  for j in range(c.get('materials',2)):
   for channel in range(3):
    if c.get('flags',0)&2:out=85
    else:
     vals=[Fraction(load(before,a+j*20+channel,1)) for a in materials];mixed=max(Fraction(0),min(Fraction(255),blend(vals)));weight=min(z,Fraction(1));out=int(weight*mixed+(1-weight)*vals[1])&255
    store(expected,materials[0]+j*20+channel,out,1)
  if i==indices[0] and c.get('materials',2):
   for channel in range(3):store(expected,ENTITY+0x8c4+channel,load(expected,materials[0]+channel,1),1)
   store(expected,ENTITY+0x8c7,255,1)
 position_list=load(before,0xc9055e0)
 for k in range(22):
  i=load(before,position_list+k*4)
  for axis in range(3):
   vals=[fraction(before,node(t,i)+8+axis*4) for t in range(7)];out=z*blend(vals)+(1-z)*vals[1];store(expected,node(0,i)+8+axis*4,bits(float(out)))
 for address,size in ((ENTITY,0x900),(APP,0x100),(OBJECTS,0x70000)):
  assert bytes_at(after,address,size)==bytes_at(expected,address,size),(c,'independent morph',hex(address),next((i for i,(a,b) in enumerate(zip(bytes_at(after,address,size),bytes_at(expected,address,size))) if a!=b),None))
def cases():
 for sx,sy,z,t,bank,rm in itertools.product((-1,1),(-1,1),(-1,0,.5,1,2),(0,1,7),range(2),range(2)):
  yield dict(xyz=(sx*.25,sy*.75,z),type=t,mode=bank*4,rm=rm,independent=True)
 for shape,sx,sy,z,t,bank,rm in itertools.product(((1.5,.5),(.5,1.5)),(-1,1),(-1,1),(-1,0,.5,1,2),(0,1),range(2),range(2)):
  yield dict(xyz=(sx*shape[0],sy*shape[1],z),type=t,mode=bank*4,rm=rm,independent=True)
 for sx,t,bank,dn,rm in itertools.product((-1,1),(0,1),range(2),(0,0x40000),range(2)):
  yield dict(xyz_raw=(0x7f61b1e6|(0x80000000 if sx<0 else 0),bits(.5),bits(.5)),type=t,mode=bank*4,dn=dn,rm=rm,probe=True)
 for sx,sy,z,t,bank in itertools.product((-1,1),(-1,1),(-1,.5,2),(0,1),range(2)):
  yield dict(xyz=(sx*.25,sy*.75,z),type=t,mode=bank*4,flags=2,independent=True)
 for vertices,materials,t,bank in itertools.product((0,1,3),(0,1,3),(0,1),range(2)):
  yield dict(xyz=(.25,.75,.5),vertices=vertices,materials=materials,type=t,mode=bank*4,independent=True)
 for x,y,z,t,bank in itertools.product((0,.5,-.5),(0,.5,-.5),(0,.5,1),(0,1),range(2)):
  if not x or not y:yield dict(xyz=(x,y,z),type=t,mode=bank*4)
 for mode in range(8):yield dict(xyz=(.25,.75,.5),mode=mode)
 for mode,rm,enable in itertools.product((0,2,4,6),(0,1,2,3),(0,0x80,0x800)):
  yield dict(xyz=(.25,.75,.5),mode=mode,rm=rm,enable=enable,probe=True)
 for raw,bank,dn in itertools.product((1,0x7f800000,0x7fffffff,0x7fc00001,0x7f61b1e6),range(2),(0,0x40000)):
  yield dict(xyz_raw=(raw,raw,bits(.5)),mode=bank*4,dn=dn,probe=True)
 for missing,bank in itertools.product((1,6,27),range(2)):yield dict(xyz=(.25,.75,.5),missing_model=missing,mode=bank*4)
 for bank in range(2):yield dict(mode=bank*4,bad_table=True)
 # Direct copy cases include every short dispatch entry, nonmultiple bulk
 # lengths, and both overlapping directions (whose ordering differs by size).
 for length,delta,mode in itertools.product(tuple(range(0,65,4))+(65,66,67,68,72,88,92,128),(-8,0,4,128),range(8)):
  yield dict(entry=COPY,length=length,delta=delta,mode=mode)
 for length in (1,2,3,5,7,63):yield dict(entry=COPY,length=length,delta=128,mode=0)
def main():
 manifest_path=PROJECT/'analysis/overlay_model_blend_translation.json';manifest=json.loads(manifest_path.read_text())
 for p,h in manifest['sha256'].items():assert digest(p)==h,p
 ram=bytearray(16<<20);main=MAIN.read_bytes();binary=MODULE.read_bytes();ram[0x10000:0x10000+len(main)]=main;ram[0x900000:0x900000+len(binary)]=binary
 records=[];coverage=set();edges=set();counts=collections.Counter();boundaries=0;independent_count=0;rng=random.Random(ENTRY);selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(xyz=(.25,.75,.5),type=1,mode=0,independent=True),dict(xyz=(-.25,-.75,2),type=0,mode=4,flags=2,independent=True),dict(xyz=(.25,.75,.5),mode=2),dict(mode=1),dict(entry=COPY,length=88,delta=128,mode=7),dict(entry=COPY,length=64,delta=4,mode=0)]
 for number,c in enumerate(selected):
  entry=c.get('entry',ENTRY);m=fixture(ram,c) if entry==ENTRY else bytearray(ram)
  s=State();s.r[:]=[rng.getrandbits(32) for _ in range(16)];s.r[4]=0x8cbff000;s.r[5]=ENTITY;s.r[15]=0x8cf00000;s.pc=entry;s.pr=0x8cfed000;s.sr=rng.getrandbits(32)
  if entry==COPY:
   src=0x8cc80020;dest=src+c['delta'];s.r[0]=c['length'];s.r[1]=dest;s.r[2]=src
   for i in range(-16,400):m[(src&0xffffff)+i]=(i*73+31)&255
  f=Fpu();f.fpscr=c.get('dn',0x40000)|(c.get('mode',0)<<19)|c.get('rm',0)|c.get('enable',0);f.fpul=rng.getrandbits(32)
  for bank in range(2):
   for i in range(16):f.banks[bank][i]=bits((i+1)/8)
  original=State.from_buffer_copy(s);other=State.from_buffer_copy(s);ef=Fpu.from_buffer_copy(f);actual=bytearray(m);expected=bytearray(m)
  status,path,want,expected_macl=reference_run(expected,other,ef,c)
  ml=ctypes.c_uint32(c.get('macl',0x12345678));got=[];errors=[]
  @OBSERVE
  def observe(ctx,target,mem,fp,sr,r,pr):
   try:got.append((target,tuple(r[:16]),sr[0],pr[0],bytes(fp.contents),ml.value,hashlib.sha256(ctypes.string_at(ctypes.addressof(mem.contents),16<<20)).digest()))
   except Exception as exc:errors.append(repr(exc))
  rc=fn((ctypes.c_uint8*len(actual)).from_buffer(actual),ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,Fpu.fpul.offset),P),ctypes.byref(ml),entry,None,observe)
  assert not errors and rc==status and got==want,(c,'status/boundaries',rc,status,errors,len(got),len(want))
  assert actual==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef) and ml.value==expected_macl,(c,'original RAM/CPU/FPU/FPUL/MACL',hex(s.pc),hex(other.pc),next((i for i,(a,b) in enumerate(zip(actual,expected)) if a!=b),None))
  semantic=False
  if not rc:
   assert s.r[15]==original.r[15] and s.pr==original.pr and list(s.r)[8:15]==list(original.r)[8:15]
   if entry==ENTRY and c.get('independent'):independent(c,m,actual);independent_count+=1;semantic=True
   if entry==COPY:
    oracle=bytearray(m);length=c['length'];offsets=range(length-4,-1,-4) if length<=64 else range(0,length//4*4,4)
    for off in offsets:store(oracle,dest+off,load(oracle,src+off))
    assert bytes_at(actual,src-16,416)==bytes_at(oracle,src-16,416);semantic=True;independent_count+=1
  coverage.update(path)
  for i,a in enumerate(path[:-1]):
   w=int.from_bytes((binary if a<0x10000000 else main)[a-(0xc900000 if a<0x10000000 else 0x8c010000):a-(0xc900000 if a<0x10000000 else 0x8c010000)+2],'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f):
    j=i+(2 if w>>8 in (0x8d,0x8f) else 1)
    if j<len(path):edges.add((a,path[j]))
  counts[rc]+=1;boundaries+=len(got);records.append(dict(input=c,status=rc,full_state_match=True,ordered_boundaries_match=True,independent_checks=semantic,stop_pc=hex(s.pc),steps=len(path)))
  if (number+1)%128==0:print(number+1,'morph/copy comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Morph/copy smoke passed:',len(records));return
 own={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']};wanted={tuple(int(a,16) for a in e) for r in manifest['routines'] for e in r['conditional_edges']}
 assert coverage<=own and edges<=wanted
 missing=sorted(own-coverage);missing_edges=sorted(wanted-edges)
 print('Uncovered:',len(missing),'rows;',len(missing_edges),'outcomes',flush=True)
 files=[Path(__file__),manifest_path,PROJECT/'source/sa_overlay_model_blend_connected.c',PROJECT/'build_overlay_model_blend.ps1',PROJECT/'verify_startup.py',PROJECT/'verify_fpu_cleanup_transfer.py',PROJECT/'verify_overlay_selector_initializer.py',PROJECT/'verify_main_dispatch_active_query.py',PROJECT/'sh4_binary32.py',PROJECT/'sh4_binary64.py',PROJECT/'sh4_multiply_extended_reference.py']+[Path(p) for p in manifest['sha256']]
 files.extend(PROJECT/'source'/name for name in ('sh4_add_sub.c','sh4_add_sub.h','sh4_multiply_extended.c','sh4_multiply_extended.h','sh4_multiply.c','sh4_fmac.c','sh4_fmac.h','sh4_divide.c','sh4_divide.h','sh4_float_i32.c','sh4_float_i32.h','sh4_truncate_i32.c','sh4_compare.c','sh4_compare_bits.h','fpu_cleanup_transfer.h','stage00_task.h'))
 report=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(counts),rows_covered=len(coverage),blend_rows_covered=sum(a<0x10000000 for a in coverage),copy_rows_covered=sum(a>0x10000000 for a in coverage),conditional_edges_covered=len(edges),conditional_edges_total=len(wanted),uncovered_rows=[hex(a) for a in missing],uncovered_edges=[[hex(a),hex(b)] for a,b in missing_edges],boundary_comparisons=boundaries,independent_cases=independent_count,native_external_game_services=0,newly_recovered_entries=2,dll_sha256=digest(DLL),sha256={str(p):digest(p) for p in files},scope='Original 813-row morph plus complete 58-row word-copy dispatcher, full RAM/CPU/FPU/FPUL/MACL and ordered entries. Independent dyadic geometry/material/position model and copy-order checks; all short targets, bulk/overlap ordering, reached numerical and memory guard states.',limitations=['Constructed valid model graphs; shipped object integration is verified separately. Exact numerical tests use the existing SH-4 software reference/backend, not hardware pixels or CPU exception delivery.','Unsupported precision/rounding/numeric states and unmapped transfers stop at reached instructions; numerical probes use the existing backend only to classify stop/trap states. Completed ordinary arithmetic uses the independent Python integer-dyadic reference.','Dynamic coverage exclusions are recorded explicitly, without claiming every CFG outcome or hardware state is reached.'],cases=records)
 (PROJECT/'analysis/overlay_model_blend_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','rows_covered','blend_rows_covered','copy_rows_covered','conditional_edges_covered','conditional_edges_total','boundary_comparisons','independent_cases','uncovered_rows','uncovered_edges')},indent=2))
if __name__=='__main__':main()
