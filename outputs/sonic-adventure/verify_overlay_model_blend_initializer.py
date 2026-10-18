"""Combined initializer with native model morph and copy dispatch."""
import collections,csv,ctypes,hashlib,itertools,json,random,sys
import sh4_binary32
import sh4_multiply_extended_reference as extended_multiply
import verify_overlay_model_blend as morph
from pathlib import Path
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call
from verify_main_dispatch_active_query import OBSERVE
from verify_overlay_selector_initializer import load,store,SETUP,TAILS,STUB,MARKER,SELECTOR,ENTRY
from verify_overlay_object_setup import clone_check,preorder
from verify_overlay_object_variants import apply_materials,ENTRIES as VARIANTS
PROJECT=Path(__file__).resolve().parent
MAIN=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';MODULE=ROOT/'work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN'
DLL=ROOT/'work/sonic-adventure/build/overlay_model_blend_initializer.dll'
TASK,WORK,ENTITY,APP,SCRATCH,HEAP=0x8cc00000,0x8cc10000,0x8cc20000,0x8cc30000,0x8cc50000,0x8cd00000
BLEND,COPY=morph.ENTRY,morph.COPY
OBJECT=0xc904d22;OBJECTS=(0xc9044a0,0xc9044e2,0xc90451c,OBJECT);REUSED=(0x8c098a80,0x8c098b42,0x8c604500)
SERVICES=set(SETUP+TAILS)-{STUB,MARKER,OBJECT,BLEND}
NATIVE=(SELECTOR,STUB,MARKER,BLEND,COPY)+OBJECTS+VARIANTS+REUSED
LIB=ctypes.CDLL(str(DLL));fn=LIB.sa_overlay_model_blend_initializer_connected
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(State),ctypes.POINTER(Fpu),morph.P,morph.P,ctypes.c_void_p,Call,OBSERVE];fn.restype=ctypes.c_int
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def heap_bytes(mem):
 offset=HEAP&0xffffff;size=0x10010
 return bytes(mem[offset:offset+size]) if isinstance(mem,bytearray) else ctypes.string_at(ctypes.addressof(mem.contents)+offset,size)
def cases():
 for t,sx,sy,bank in itertools.product(range(8),(-1,1),(-1,1),range(2)):
  yield dict(type=t,flags=16,mode=bank*4,tail=0,colour=0,heap=0x10000,xyz=(sx*.25,sy*.75,.5))
 for t,tail,bank in itertools.product((0,7),(0,2,4,6,8,10,64,66,68,70,72,74,76,78),range(2)):
  yield dict(type=t,flags=16,mode=bank*4,tail=tail,colour=3,heap=0x10000)
 for t,colour,bank in itertools.product((0,7),(0,1,2,3,4,255),range(2)):
  yield dict(type=t,flags=16,mode=bank*4,tail=76,colour=colour,heap=0x10000)
 for t,flags,bank in itertools.product((0,7),(0,1,2,4,8,16,31,255),range(2)):
  yield dict(type=t,flags=flags,mode=bank*4,tail=0,colour=0,heap=0x10000)
 for target,error,bank in itertools.product(sorted(SERVICES),('missing','error'),range(2)):
  yield dict(type=0,flags=16,tail={TAILS[0]:64,TAILS[1]:4,TAILS[2]:8}.get(target,0),colour=1,mode=bank*4,heap=0x10000,**{error+'_target':target})
 for mode in (1,3,5,7):yield dict(type=0,flags=16,mode=mode,tail=0,colour=0,heap=0x10000)
 for mode,size in itertools.product((0,4),(0,32,80,160)):yield dict(type=0,flags=16,mode=mode,tail=0,colour=0,heap=size)
 for t,mode in itertools.product(range(8),(2,6)):yield dict(type=t,flags=16,mode=mode,tail=0,colour=0,heap=0x10000)
class Stopped(Exception):pass
def main():
 baseline_paths=[PROJECT/'analysis'/n for n in ('overlay_selector_initializer_verification.json','overlay_object_setup_verification.json','overlay_object_variants_verification.json','overlay_selector_guard_verification.json','overlay_model_blend_verification.json')]
 baselines=[json.loads(p.read_text()) for p in baseline_paths]
 for item in baselines:
  for p,h in item['sha256'].items():assert digest(p)==h,p
 mainbytes=MAIN.read_bytes();binary=MODULE.read_bytes();ram=bytearray(16<<20);ram[0x10000:0x10000+MAIN.stat().st_size]=MAIN.read_bytes();ram[0x900000:0x900000+len(binary)]=binary
 manifests=[json.loads((PROJECT/'analysis'/n).read_text()) for n in ('overlay_selector_initializer_translation.json','overlay_object_setup_translation.json','overlay_object_variants_translation.json','overlay_model_blend_translation.json')]
 routines=[r for m in manifests for r in m['routines']];own={int(a,16) for r in routines for a in r['reachable_addresses']}
 selector_rows={int(r['address'],16) for r in csv.DictReader((PROJECT/'analysis/overlay-face-model-selector/0c905548_instructions.tsv').open(),delimiter='\t')};stub_rows={STUB,STUB+2}
 selected=list(cases());rng=random.Random(ENTRY^OBJECT);rows=set();edges=set();calls=set();counts=collections.Counter();targets=collections.Counter();records=[];boundaries=0;independent=0
 if '--smoke' in sys.argv:selected=[dict(type=t,flags=flag,tail=tail,colour=colour,mode=mode,heap=size) for t,flag,tail,colour,mode,size in ((0,0,0,0,0,0x10000),(7,16,76,3,6,0x10000),(1,255,8,255,2,0x10000),(0,16,0,0,1,0x10000),(0,16,0,0,0,0))]
 for n,c in enumerate(selected):
  m=bytearray(ram);store(m,TASK+32,WORK);store(m,TASK+36,ENTITY);store(m,ENTITY+0xec,APP);store(m,APP,c['type'],1);store(m,APP+0x34,c['flags'],1);store(m,APP+0x36,c['tail']|0xa001,2);store(m,APP+0x192,c['colour'],1)
  for i in range(16):store(m,WORK+i*4,0x12340000+i*0x101)
  for i in range(3):store(m,APP+0x20+i*4,morph.bits(c.get('xyz',(.25,.75,.5))[i]))
  store(m,ENTITY+0x812,0xaabbccdd);store(m,ENTITY+0x816,0x12345678);store(m,ENTITY+0x60,0x89abcdef);store(m,ENTITY+0x78,0xdeadbeef)
  size=c['heap'];store(m,load(m,0x8c098b60),HEAP if size else 0)
  if size:
   for i,v in enumerate((HEAP+size,0,HEAP+size,0)):store(m,HEAP+i*4,v)
   for i,v in enumerate((0,HEAP,0,HEAP)):store(m,HEAP+size+i*4,v)
  initial=State();initial.r[:]=[rng.getrandbits(32) for _ in range(16)];initial.r[4]=TASK;initial.r[15]=0x8cf00000;initial.pr=0x8cfed000;initial.pc=ENTRY;initial.sr=rng.getrandbits(32)
  s=State.from_buffer_copy(initial);other=State.from_buffer_copy(initial);f=Fpu();f.fpscr=0x40000|(c['mode']<<19);f.fpul=rng.getrandbits(32)
  for bank in range(2):
   for i in range(16):f.banks[bank][i]=rng.getrandbits(32)
  ef=Fpu.from_buffer_copy(f);actual=bytearray(m);expected=bytearray(m);path=[];want=[];got=[];errors=[];expected_status=0;blend_pr=None;copy_pr=None;oracle=None;old_mesh=None;node=None;checked=False
  def event(target,mem,r,sr,pr,fp):
   # Hashing checks all heap bytes at every boundary without huge event records.
   heap=hashlib.sha256(heap_bytes(mem)).digest()
   return target,tuple(r),sr,pr,bytes(fp),heap,tuple(load(mem,ENTITY+off) for off in (0x248,0x2d4,0xd4,0xd8,0xdc,0xe0,0xe4)),tuple(load(mem,WORK+i*4) for i in range(8))
  def validate_object(mem):
   nonlocal oracle,checked
   t=0 if c['type']==7 else c['type'];roots=[load(m,0xc91c3d0+t*24+i*4) for i in range(6)];copy=load(mem,ENTITY+0x248);clone_check(m,mem,roots[0],copy)
   for root,off in zip([copy]+roots,(0x24c,0x314,0x3dc,0x6fc,0x4a4,0x56c,0x634)):
    nodes=preorder(mem,root);assert len(nodes)==39 and [load(mem,ENTITY+off+i*4) for i in range(39)]==nodes
   assert load(mem,ENTITY+0x2d4)==preorder(mem,copy)[34]
   oracle=bytearray(mem)
   variants=([VARIANTS[0]] if c['tail']&64 else [])+([VARIANTS[1]] if c['tail']&4 else [VARIANTS[2]] if c['tail']&8 else [])
   for target in variants:apply_materials(oracle,ENTITY,target,c['colour'])
   checked=True
  def stop(pc,r,sr,status):
   nonlocal expected_status
   expected_status=status;other.r[:]=r;other.sr=sr;other.pc=pc;raise Stopped()
  def effect(target,mem,r,sr,fp):
   assert target in SERVICES and r[4]==WORK and r[5]==ENTITY
   slot=(SETUP+TAILS).index(target);store(mem,SCRATCH+slot*4,load(mem,SCRATCH+slot*4)+1)
   for i in range(8):r[i]=0xbad00000+i*0x101+slot;fp.banks[(fp.fpscr>>21)&1][i]=0x3f000000+i*0x10000+slot
   return sr^1
  def hook(target):
   def call(mem,r,sr):
    want.append(event(target,mem,r,sr,other.pr,ef))
    if c.get('missing_target')==target:stop(target,r,sr,-2)
    sr=effect(target,mem,r,sr,ef)
    if c.get('error_target')==target:stop(target,r,sr,-17)
    return sr
   return call
  def trace(pc,w,r,sr,pr,fp):
   nonlocal old_mesh,node,blend_pr,copy_pr
   path.append(pc)
   if pc==BLEND:blend_pr=pr
   if pc==COPY:copy_pr=pr
   if pc in NATIVE and not (pc==0x8c604500 and len(path)>1 and path[-2]==0x8c604508):want.append(event(pc,expected,r,sr,pr,ef))
   if pc in VARIANTS+(BLEND,) and not checked and c['heap']>=0x10000:validate_object(expected)
   if pc==SELECTOR:
    node=load(expected,ENTITY+0x2d4);old_mesh=load(expected,node+4)
   if pc==BLEND and oracle is not None:assert heap_bytes(expected)==heap_bytes(oracle)
   if pc in morph.ROWS:
    text=morph.ROWS[pc];access=morph.dynamic_access(text,r,ef.fpscr)
    if access:
     a,nbytes=access
     if a&0x1f000000!=0xc000000 or (a&0xffffff)+nbytes>0x1000000:stop(pc,r,sr,-3)
    if w==0xb and pr!=(blend_pr if pc<0x10000000 else copy_pr):stop(pc,r,sr,-3)
    if w&0xf0ff in (0xf08d,0xf09d,0xf04d) and ef.fpscr&0x80000:stop(pc,r,sr,-1)
    if w>>12==15 and (w&15 in (0,1,2,3,4,5,14) or w&0xf0ff in (0xf02d,0xf03d)) and ef.fpscr&0x100000:
     rc,probe,new_sr=morph.numeric_probe(w,ef,sr)
     if rc:
      ctypes.memmove(ctypes.byref(ef),ctypes.byref(probe),ctypes.sizeof(Fpu));stop(pc,r,new_sr,rc)
   if pc==0xc905572 and r[2]&0x1f000000!=0xc000000:stop(pc,r,sr,-3)
   if pc==0xc904d5e and ef.fpscr&0x80000:stop(pc,r,sr,-1)
   if pc in (0xc9044b4,0xc9044f2,0xc904534) and r[10 if pc==0xc904534 else 13]&0x1f000000!=0xc000000:stop(pc,r,sr,-3)
  saved_multiply=sh4_binary32.execute_multiply;sh4_binary32.execute_multiply=extended_multiply.execute
  try:reference(expected,other,fpu=ef,hooks={t:hook(t) for t in SERVICES},trace=trace,special={'macl':0xa55a1234},instruction_budget=3000000)
  except Stopped:pass
  finally:sh4_binary32.execute_multiply=saved_multiply
  @Call
  def external(ctx,target,mem,fp,sr,r,pr):
   try:
    if c.get('missing_target')==target:return -2
    sr[0]=effect(target,mem,r,sr[0],fp.contents);return -17 if c.get('error_target')==target else 0
   except Exception as exc:errors.append(repr(exc));return -99
  @OBSERVE
  def observe(ctx,target,mem,fp,sr,r,pr):
   try:
    got.append(event(target,mem,r[:16],sr[0],pr[0],fp.contents))
    if target==BLEND and oracle is not None:
     assert heap_bytes(mem)==oracle[HEAP&0xffffff:(HEAP&0xffffff)+0x10010]
   except Exception as exc:errors.append(repr(exc))
  macl=ctypes.c_uint32(0xa55a1234)
  status=fn((ctypes.c_uint8*len(actual)).from_buffer(actual),ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,Fpu.fpul.offset),morph.P),ctypes.byref(macl),None,external,observe)
  assert macl.value==0xa55a1234
  assert status==expected_status and not errors and got==want,(c,'status/boundaries',status,expected_status,errors,len(got),len(want),next((i for i,(a,b) in enumerate(zip(got,want)) if a!=b),None))
  assert actual==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef),(c,'full RAM/CPU/FPU',hex(s.pc),hex(other.pc))
  assert actual[0x900000:0x900000+len(binary)]==binary
  if status==0:
   assert checked and node
   first=next((i for i,bit in enumerate((16,8,4,2,1)) if c['flags']&bit),None)
   root_literals=(0xc9055ec,0xc9055f0,0xc9055f4,0xc9055f8,0xc905740)
   assert load(actual,node+4)==(load(m,load(m,root_literals[first])+4) if first is not None else old_mesh)
   assert all(load(actual,node+off)==0x3e999999 for off in (0x20,0x24,0x28))
   assert load(actual,ENTITY+0x60)==0x89abcde1 and load(actual,ENTITY+0x78)==0xdead0000
   assert load(actual,APP+0x36,2)==(c['tail']|0xa001)&0xfffe
   assert load(actual,ENTITY+0xd8)==load(m,0xc90fef0)
   assert load(actual,ENTITY+0xdc)==(load(m,0xc90fef4) if c['mode']&2 else load(m,APP+0x20))
   assert load(actual,ENTITY+0xe0)==load(m,APP+0x24) and load(actual,ENTITY+0xe4)==load(m,APP+0x28)
   assert load(actual,ENTITY+0x812)==0xaabb0000 and load(actual,ENTITY+0x816)==0x1234ffff
   assert s.r[15]==initial.r[15] and s.pr==initial.pr and list(s.r)[8:15]==list(initial.r)[8:15]
   variant_sequence=([VARIANTS[0]] if c['tail']&64 else [])+([VARIANTS[1]] if c['tail']&4 else [VARIANTS[2]] if c['tail']&8 else [])
   assert [t for t,*_ in got if t in VARIANTS]==variant_sequence
   tail=next((target for bit,target in zip((64,4,8),TAILS) if c['tail']&bit),None)
   assert [t for t,*_ in got if t in SETUP+(SELECTOR,)+TAILS]==list(SETUP)+[SELECTOR]+([tail] if tail else [])
   independent+=1
  rows.update(set(path)&(own|selector_rows|stub_rows));counts[status]+=1;boundaries+=len(got);targets.update(t for t,*_ in got)
  for i,a in enumerate(path[:-1]):
   if a not in own:continue
   image,base=(binary,0xc900000) if a<0x10000000 else (mainbytes,0x8c010000)
   w=int.from_bytes(image[a-base:a-base+2],'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f):
    j=i+(2 if w>>8 in (0x8d,0x8f) else 1)
    if j<len(path):edges.add((a,path[j]))
  call_set={int(a,16) for r in routines for a in r['external_call_sites']};calls.update(set(path)&call_set)
  records.append(dict(input=c,status=status,full_state_match=True,ordered_boundaries_match=True,independent_checks=status==0,steps=len(path),boundary_count=len(got)))
  if (n+1)%32==0:print(n+1,'combined morph initializer comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Combined morph initializer smoke passed:',len(records));return
 assert counts[0]>0 and counts[-1]>=4 and counts[-3]>=4 and counts[-2]==12 and counts[-17]==12
 assert NATIVE and all(targets[t] for t in NATIVE)
 files=[Path(__file__),PROJECT/'build_overlay_model_blend_initializer.ps1',PROJECT/'source/sa_overlay_model_blend_initializer_connected.c',PROJECT/'source/sa_overlay_object_variants_connected.c',PROJECT/'source/sa_overlay_selector_guard.c']+baseline_paths
 for b in baselines:files.extend(Path(p) for p in b['sha256'])
 report=dict(cases_passed=len(records),complete_cases=counts[0],partial_cases=len(records)-counts[0],status_counts=dict(counts),boundary_comparisons=boundaries,native_rows_covered=len(rows),conditional_edges_covered=len(edges),call_sites_covered=len(calls),target_invocations={hex(k):v for k,v in sorted(targets.items())},remaining_service_targets=[hex(a) for a in sorted(SERVICES)],independent_cases=independent,newly_recovered_entries=0,reused_routine_credit=0,sha256={str(p):digest(p) for p in files},dll_sha256=digest(DLL),scope='Original initializer with native morph, complete word-copy dispatcher, recursive clone/tables, material variants, marker/no-op and selector. Full RAM/CPU/FPU/FPUL/MACL plus ordered boundaries. Independent pre-morph tree/material checks and post-morph selector/reset/tail assertions; focused morph suite provides independent mathematical checks.',limitations=['Three other initialization helpers and three tail-state helpers remain state-mutating fixtures. Full initialized scenes, draw calls, pixels and gameplay are not proven.','Valid task/entity/appearance records and shipped object graphs. PR=1, invalid source/destination pointers, unsupported arithmetic and incomplete-copy writes stop at original unsupported instructions with exact partial state; no hardware fault delivery. Earlier bounded object proof records excluded null/repeated-pointer paths.'],cases=records)
 (PROJECT/'analysis/overlay_model_blend_initializer_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','complete_cases','partial_cases','status_counts','boundary_comparisons','native_rows_covered','conditional_edges_covered','call_sites_covered','independent_cases')},indent=2))
if __name__=='__main__':main()
