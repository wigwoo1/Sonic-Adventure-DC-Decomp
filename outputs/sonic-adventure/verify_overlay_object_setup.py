"""Compare object preparation with original SH-4 execution and real heap code."""
import collections,ctypes,hashlib,itertools,json,random,sys
from pathlib import Path
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call
from verify_main_dispatch_active_query import OBSERVE
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent
MAIN=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN'
MODULE=ROOT/'work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN'
DLL=ROOT/'work/sonic-adventure/build/overlay_object_setup.dll'
ENTRIES=(0xc9044a0,0xc9044e2,0xc90451c,0xc904d22)
SERVICES=(0xc904aea,0xc904bbc,0xc904c80)
REUSED=(0x8c098a80,0x8c098b42,0x8c604500)
WORK,ENTITY,APP,SRC,MODEL,TABLE,HEAP=0x8cc00000,0x8cc10000,0x8cc20000,0x8cc30000,0x8cc31000,0x8cc40000,0x8cd00000
LIB=ctypes.CDLL(str(DLL));fn=LIB.sa_overlay_object_setup_connected
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(State),ctypes.POINTER(Fpu),ctypes.c_uint32,ctypes.c_void_p,Call,OBSERVE];fn.restype=ctypes.c_int
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def span(m,a,n):return bytes(m[(a&0xffffff):(a&0xffffff)+n])
def preorder(m,root):
 out=[]
 def visit(a):
  assert a not in out and len(out)<100;out.append(a)
  for off in (44,48):
   p=load(m,a+off)
   if p:visit(p)
 visit(root);return out
def fixture(m,c):
 # Distinct raw words also exercise signed bytes and raw floating transfers.
 for i in range(0x600):m[(SRC&0xffffff)+i]=(i*73+31)&255
 for j in range(4):
  a=SRC+j*64
  for i in range(13):store(m,a+i*4,0xabcd0000+j*0x100+i)
  store(m,a+4,MODEL if c.get('model',True) else 0);store(m,a+44,0);store(m,a+48,0)
 if c.get('tree',False):
  store(m,SRC+44,SRC+64);store(m,SRC+48,SRC+192);store(m,SRC+64+48,SRC+128)
 mask=c.get('mask',31)
 for i in range(40):m[(MODEL&0xffffff)+i]=i*3&255
 for off,bit,p in ((0,1,SRC+0x400),(4,2,SRC+0x440),(12,8,SRC+0x480),(16,4,SRC+0x4c0)):store(m,MODEL+off,p if mask&bit else 0)
 store(m,MODEL+8,3);store(m,MODEL+20,2,2);store(m,MODEL+22,2,2)
 seg=SRC+0x480
 for j in range(2):
  store(m,seg+j*24+2,2,2);store(m,seg+j*24+12,SRC+0x520 if mask&16 else 0)
 return SRC
def clone_check(before,after,source,clone):
 assert clone and clone!=source
 a=source;b=clone
 for off in range(0,52,4):
  if off not in (4,44,48):assert load(after,b+off)==load(before,a+off)
 model=load(before,a+4);copied=load(after,b+4)
 if not model:assert not copied
 else:
  assert copied and copied!=model
  for off in range(0,40,4):
   if off not in (0,4,12,16):assert load(after,copied+off)==load(before,model+off)
  nv=load(before,model+8);ns=load(before,model+20,2);nm=load(before,model+22,2)
  for off,n in ((0,nv*12),(4,nv*12),(16,nm*20)):
   p=load(before,model+off);q=load(after,copied+off)
   if p:assert q and q!=p and span(after,q,n)==span(before,p,n)
   else:assert q==0
  p=load(before,model+12);q=load(after,copied+12)
  if p:
   assert q and q!=p
   for off in range(0,ns*24,4):
    if off!=12:assert load(after,q+off)==load(before,p+off)
   normal=load(before,p+12);copy=load(after,q+12)
   if normal:assert copy and copy!=normal and span(after,copy,load(before,p+2,2)*12)==span(before,normal,load(before,p+2,2)*12)
   else:assert copy==0
  else:assert q==0
 for off in (44,48):
  p=load(before,a+off);q=load(after,b+off)
  if p:clone_check(before,after,p,q)
  else:assert q==0
def cases():
 for mask,mode in itertools.product(range(32),range(8)):
  yield dict(kind='clone',mask=mask,mode=mode,tree=True,heap=4096)
 for mask,size in itertools.product(range(32),range(0,400,16)):
  yield dict(kind='clone',mask=mask,mode=0,heap=size)
 for size in range(0,1600,16):yield dict(kind='clone',mask=31,mode=0,heap=size,tree=True)
 for kind,tree,mode,alias in itertools.product(('walk','root'),(False,True),range(8),(0,0x80000000,0xa0000000)):
  yield dict(kind=kind,tree=tree,mode=mode,alias=alias,heap=0)
 for kind,mode in itertools.product(('clone','walk','root'),range(8)):
  yield dict(kind=kind,mode=mode,model=False,tree=True,heap=1024)
 for t,flags,mode in itertools.product(range(8),(0,4,8,12,64,68,72,76),(0,2,4,6)):
  yield dict(kind='setup',type=t,flags=flags,mode=mode,heap=0x10000)
 for mode in (1,3,5,7):yield dict(kind='setup',type=0,flags=0,mode=mode,heap=0x10000)
 for kind in ('clone','walk','root'):yield dict(kind=kind,mode=0,null=True,heap=1024)
 yield dict(kind='setup',type=0,flags=0,mode=0,heap=0)
 for target,error,mode in itertools.product(SERVICES,('missing','error'),(0,4)):
  yield dict(kind='setup',type=0,flags={SERVICES[0]:64,SERVICES[1]:4,SERVICES[2]:8}[target],mode=mode,heap=0x10000,**{error+'_target':target})
class Stopped(Exception):pass
def main():
 manifest_path=PROJECT/'analysis/overlay_object_setup_translation.json';manifest=json.loads(manifest_path.read_text())
 for p,h in manifest['sha256'].items():assert digest(p)==h,p
 # Independently check the seven copy opcodes before applying the existing C copy.
 mainbytes=MAIN.read_bytes();assert mainbytes[0x5f4500:0x5f450e]==bytes.fromhex('546010460024fb8f01740b000900')
 ram=bytearray(16<<20);ram[0x10000:0x10000+len(mainbytes)]=mainbytes
 binary=MODULE.read_bytes();ram[0x900000:0x900000+len(binary)]=binary
 own={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']}
 wanted_edges={tuple(int(a,16) for a in e) for r in manifest['routines'] for e in r['conditional_edges']}
 wanted_calls={int(a,16) for r in manifest['routines'] for a in r['external_call_sites']}
 rows=set();edges=set();calls=set();counts=collections.Counter();boundaries=0;records=[];allocation_failures=set();rng=random.Random(0xc90451c)
 selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(kind='clone',mask=31,mode=0,heap=4096,tree=True),dict(kind='clone',mask=31,mode=0,heap=192),dict(kind='setup',type=0,flags=76,mode=2,heap=0x10000),dict(kind='setup',type=7,flags=0,mode=1,heap=1024),dict(kind='root',mode=0,null=True,heap=0)]
 for n,c in enumerate(selected):
  entry={'clone':ENTRIES[2],'walk':ENTRIES[0],'root':ENTRIES[1],'setup':ENTRIES[3]}[c['kind']]
  m=bytearray(ram);source=fixture(m,c);alias=c.get('alias',0)
  source=(source&0x1fffffff)|alias if alias else source
  if c.get('null'):source=0
  store(m,ENTITY+0xec,APP);store(m,APP,c.get('type',0),1);store(m,APP+0x36,c.get('flags',0),2)
  size=c['heap'];store(m,load(m,0x8c098b60),HEAP if size else 0)
  if size:
   for i,v in enumerate((HEAP+size,0,HEAP+size,0)):store(m,HEAP+i*4,v)
   for i,v in enumerate((0,HEAP,0,HEAP)):store(m,HEAP+size+i*4,v)
  state=State();state.r[:]=[rng.getrandbits(32) for _ in range(16)];state.r[4]=WORK if c['kind']=='setup' else source;state.r[5]=ENTITY if c['kind']=='setup' else TABLE;state.r[6]=3;state.r[15]=0x8cf00000;state.pr=0x8cfed000;state.pc=entry;state.sr=rng.getrandbits(32)
  initial=State.from_buffer_copy(state);other=State.from_buffer_copy(state);f=Fpu();f.fpscr=0x40000|(c['mode']<<19)
  for bank in range(2):
   for i in range(16):f.banks[bank][i]=rng.getrandbits(32)
  ef=Fpu.from_buffer_copy(f);actual=bytearray(m);expected=bytearray(m);want=[];got=[];path=[];errors=[];allocations=[];expected_status=0
  def event(t,mem,r,sr,pr,fp):return t,tuple(r),sr,pr,bytes(fp)[:132],load(mem,load(mem,0x8c098b60)),span(mem,ENTITY,0x900),span(mem,WORK,32)
  def stop(pc,r,sr,status):
   nonlocal expected_status
   expected_status=status;other.r[:]=r;other.sr=sr;other.pc=pc;raise Stopped()
  def effect(target,mem,r,sr,fp):
   assert r[4]==WORK and r[5]==ENTITY
   store(mem,WORK+(SERVICES.index(target)*4),target)
   for i in range(8):r[i]=0xbeef0000+i*0x101
   fp.banks[(fp.fpscr>>21)&1][0]=0x3f800000
   return sr^1
  def hook(target):
   def invoke(mem,r,sr):
    want.append(event(target,mem,r,sr,other.pr,ef))
    if c.get('missing_target')==target:stop(target,r,sr,-2)
    sr=effect(target,mem,r,sr,ef)
    if c.get('error_target')==target:stop(target,r,sr,-17)
    return sr
   return invoke
  def trace(pc,w,r,sr,pr,fp):
   path.append(pc)
   if pc in ENTRIES+REUSED and not (pc==0x8c604500 and len(path)>1 and path[-2]==0x8c604508):want.append(event(pc,expected,r,sr,pr,ef))
   if pc==0x8c098a80:allocations.append(r[4])
   if pc in (0xc904540,0xc904562,0xc90458c,0xc9045c0,0xc904602,0xc90468c,0xc9046f6) and r[0]==0:allocation_failures.add(pc)
   if pc==0xc90471c:
    # Freeing the material allocation retains the separate model pointer.
    assert load(expected,r[14]+16)!=0
   if pc==0xc904720:assert r[3]!=0
   if pc==0xc904d5e and ef.fpscr&0x80000:stop(pc,r,sr,-1)
   if pc in (0xc9044b4,0xc9044f2,0xc904534):
    reg=10 if pc==0xc904534 else 13
    if r[reg]&0x1f000000!=0xc000000:stop(pc,r,sr,-3)
  try:reference(expected,other,fpu=ef,hooks={t:hook(t) for t in SERVICES},trace=trace,instruction_budget=3000000)
  except Stopped:pass
  @Call
  def external(ctx,target,mem,fp,sr,r,pr):
   try:
    assert target in SERVICES
    if c.get('missing_target')==target:return -2
    sr[0]=effect(target,mem,r,sr[0],fp.contents)
    return -17 if c.get('error_target')==target else 0
   except Exception as exc:errors.append(repr(exc));return -99
  @OBSERVE
  def observe(ctx,target,mem,fp,sr,r,pr):
   try:got.append(event(target,mem,r[:16],sr[0],pr[0],fp.contents))
   except Exception as exc:errors.append(repr(exc))
  status=fn((ctypes.c_uint8*len(actual)).from_buffer(actual),ctypes.byref(state),ctypes.byref(f),entry,None,Call() if c.get('missing_target') else external,observe)
  assert not errors and status==expected_status,(c,status,expected_status,errors)
  assert got==want,(c,'boundaries',len(got),len(want),next((i for i,(a,b) in enumerate(zip(got,want)) if a!=b),None))
  assert actual==expected and bytes(state)==bytes(other) and bytes(f)==bytes(ef),(c,'full RAM/CPU/FPU',next((i for i,(a,b) in enumerate(zip(actual,expected)) if a!=b),None),list(state.r),list(other.r),hex(state.pc),hex(other.pc))
  assert span(actual,0xc900000,len(binary))==binary and span(actual,SRC,0x600)==span(m,SRC,0x600) and span(actual,MODEL,40)==span(m,MODEL,40)
  independent=False
  if status==0 and c['kind'] in ('walk','root'):
   nodes=preorder(m,source);start=3 if c['kind']=='walk' else 0
   assert [load(actual,TABLE+4*(start+i)) for i in range(len(nodes))]==nodes
   if c['kind']=='walk':assert state.r[0]==start+len(nodes)
   independent=True
  if status==0 and c['kind']=='clone' and c['heap']>=4096:
   clone_check(m,actual,source,state.r[0]);independent=True
  if status==0 and c['kind']=='setup':
   t=0 if c['type']==7 else c['type'];roots=[load(m,0xc91c3d0+t*24+i*4) for i in range(6)];copy=load(actual,ENTITY+0x248);clone_check(m,actual,roots[0],copy)
   for root,offset in zip([copy]+roots,(0x24c,0x314,0x3dc,0x6fc,0x4a4,0x56c,0x634)):
    nodes=preorder(actual,root);assert len(nodes)==39
    assert [load(actual,ENTITY+offset+4*i) for i in range(len(nodes))]==nodes
   assert load(actual,ENTITY+0x7c4)==load(m,0xc904e68)
   bank=(c['mode']>>2)&1
   assert load(actual,ENTITY+0xd4)==(ef.banks[bank^1][2] if c['mode']&2 else 0)
   variant=([SERVICES[0]] if c['flags']&64 else [])+([SERVICES[1]] if c['flags']&4 else [SERVICES[2]] if c['flags']&8 else [])
   assert [e[0] for e in got if e[0] in SERVICES]==variant
   independent=True
  rows.update(set(path)&own);calls.update(set(path)&wanted_calls)
  for i,a in enumerate(path[:-1]):
   if a not in own:continue
   w=int.from_bytes(binary[a-0xc900000:a-0xc900000+2],'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f):
    j=i+(2 if w>>8 in (0x8d,0x8f) else 1)
    if j<len(path):edges.add((a,path[j]))
  counts[status]+=1;boundaries+=len(got)
  records.append(dict(input=c,status=status,steps=len(path),boundary_count=len(got),allocations=allocations,full_state_match=True,ordered_boundaries_match=True,source_preserved=True,independent_checks=independent))
  if (n+1)%128==0:print(n+1,'object setup comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Object setup smoke passed:',len(records));return
 print('Missing rows/edges/calls:',[hex(a) for a in sorted(own-rows)],[[hex(a),hex(b)] for a,b in sorted(wanted_edges-edges)],[hex(a) for a in sorted(wanted_calls-calls)],flush=True)
 excluded_rows={0xc904536,0xc904538}
 excluded_edges={(0xc904532,0xc904536),(0xc904720,0xc904726)}
 assert own-rows==excluded_rows and wanted_edges-edges==excluded_edges and edges<=wanted_edges and calls==wanted_calls
 assert allocation_failures=={0xc904540,0xc904562,0xc90458c,0xc9045c0,0xc904602,0xc90468c,0xc9046f6}
 files=[Path(__file__),manifest_path,PROJECT/'source/sa_overlay_object_setup_connected.c',PROJECT/'source/sa_heap_allocate.c',PROJECT/'source/sa_heap_release.c',PROJECT/'source/byte_copy_forward.c',PROJECT/'source/byte_copy_forward.h',PROJECT/'source/fpu_cleanup_transfer.h',PROJECT/'source/stage00_task.h',PROJECT/'build_overlay_object_setup.ps1',PROJECT/'verify_startup.py',PROJECT/'verify_fpu_cleanup_transfer.py',PROJECT/'verify_overlay_selector_initializer.py',PROJECT/'verify_tikal_controller.py',PROJECT/'verify_main_dispatch_active_query.py',MAIN]+[Path(p) for p in manifest['sha256']]
 report=dict(module='AL_MAIN.BIN',load_address='0x0c900000',source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),rows_covered=len(rows),instruction_rows_total=len(own),excluded_rows=[hex(a) for a in sorted(excluded_rows)],coverage_exclusions={'null_branch':'0c904536/538 and outcome 0c904532->0c904536 require null source, but delay-slot 0c904534 first reads source+4 outside modeled RAM. Guarded before that read.', 'retained_material_pointer':'Outcome 0c904720->0c904726 requires zero material pointer after a preceding nonzero test and freeing a distinct allocation. The separate model field remains nonzero for the valid bounded heap contract; checked at 0c90471c/720.'},conditional_edges_covered=len(edges),conditional_edges_total=len(wanted_edges),uncovered_edges=[[hex(a),hex(b)] for a,b in sorted(wanted_edges-edges)],call_sites_covered=len(calls),status_counts=dict(counts),boundary_comparisons=boundaries,allocation_failure_sites_covered=[hex(a) for a in sorted(allocation_failures)],independent_cases=sum(r['independent_checks'] for r in records),remaining_service_targets=[hex(t) for t in SERVICES],newly_recovered_entries=4,reused_heap_copy_new_credit=0,dll_sha256=digest(DLL),sha256={str(p):digest(p) for p in files},scope='Four native object preparation bodies with native recursive clone/table walks and reused original-state heap/free/copy. Full RAM/CPU/FPU and ordered entry comparisons, unchanged original asset trees, partial allocation failures, raw transfer modes and independent traversal/copy/variant-priority assertions.',limitations=['Two null-source branch rows are guarded before an invalid read. Two CFG outcomes are excluded: guarded null branch and the repeated material-null test under valid separate allocations. No coverage is claimed for these paths.','Three appearance variants remain explicit state-mutating fixtures; no full scene, draw call, gameplay or hardware pixels proven.','Bounded valid acyclic object/model graphs and positive copy sizes only; guarded invalid node reads and PR=1 FLDI0 stop before effects rather than delivering hardware exceptions.','Original first-segment-only auxiliary copy, other shallow streams, partial descendant failure and asymmetric failure cleanup are preserved.'],cases=records)
 (PROJECT/'analysis/overlay_object_setup_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','rows_covered','conditional_edges_covered','conditional_edges_total','call_sites_covered','status_counts','boundary_comparisons','independent_cases','uncovered_edges')},indent=2))
if __name__=='__main__':main()
