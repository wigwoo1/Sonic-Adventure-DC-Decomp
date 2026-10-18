"""Original initializer plus native selector, with explicit remaining services."""
import collections,ctypes,hashlib,itertools,json,random,struct,sys
from pathlib import Path
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call
from verify_main_dispatch_active_query import OBSERVE
PROJECT=Path(__file__).resolve().parent
MODULE=ROOT/'work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN';MAIN=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN'
ENTRY,SELECTOR=0x0c90fde4,0x0c905548
TASK,WORK,ENTITY,APPEARANCE,NODE,SCRATCH=0x8cc00000,0x8cc10000,0x8cc20000,0x8cc30000,0x8cc40000,0x8cc50000
SETUP=(0x0c90bd40,0x0c90d210,0x0c909d40,0x0c90853e,0x0c904d22,0x0c904e80,0x0c9048ec)
STUB,MARKER=0x0c90d210,0x0c909d40
TAILS=(0x0c90fb82,0x0c90fca0,0x0c90fd42);SERVICES=set(SETUP+TAILS)-{STUB,MARKER}
DLL=ROOT/'work/sonic-adventure/build/overlay_selector_initializer.dll';LIB=ctypes.CDLL(str(DLL))
fn=LIB.sa_overlay_selector_initializer_connected;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(State),ctypes.POINTER(Fpu),ctypes.c_void_p,Call,OBSERVE];fn.restype=ctypes.c_int

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def load(m,a,n=4):return int.from_bytes(bytes(m[(a&0xffffff)+i] for i in range(n)),'little')
def store(m,a,value,n=4):
 for i in range(n):m[(a&0xffffff)+i]=(value>>(8*i))&255
class Stopped(Exception):pass

def cases():
 for flags,mode in itertools.product(range(256),range(8)):yield dict(flags=flags,tail_flags=0,mode=mode)
 for flags,tail,mode in itertools.product((0,1,2,4,8,16,31,255),(4,8,12,64,68,72,76),range(8)):yield dict(flags=flags,tail_flags=tail,mode=mode)
 for target,error,bank in itertools.product(sorted(SERVICES),('missing','error'),range(2)):
  tail={TAILS[0]:64,TAILS[1]:4,TAILS[2]:8}.get(target,0)
  yield dict(flags=16,tail_flags=tail,mode=bank*4,**{error+'_target':target})

def main():
 manifest_path=PROJECT/'analysis/overlay_selector_initializer_translation.json';manifest=json.loads(manifest_path.read_text());selector_report=PROJECT/'analysis/overlay_face_model_selector_verification.json';old=json.loads(selector_report.read_text())
 for item in (manifest,old):
  for p,h in item['sha256'].items():assert digest(p)==h,p
 routine=manifest['routines'][0];caller={int(a,16) for a in routine['reachable_addresses']};marker={int(a,16) for a in manifest['routines'][1]['reachable_addresses']};own=caller|marker;stub={STUB,STUB+2};child=set()
 import csv
 listing=PROJECT/'analysis/overlay-face-model-selector/0c905548_instructions.tsv'
 for row in csv.DictReader(listing.open(),delimiter='\t'):child.add(int(row['address'],16))
 binary=MODULE.read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+MAIN.stat().st_size]=MAIN.read_bytes();ram[0x900000:0x900000+len(binary)]=binary
 roots=[load(ram,a) for a in (0xc9055ec,0xc9055f0,0xc9055f4,0xc9055f8,0xc905740)];appearance_mask=load(ram,0xc90feec,2)
 records=[];rows=set();child_rows=set();stub_rows=set();edges=set();calls=set();totals=dict(boundary_comparisons=0,fixture_invocations=0,selector_invocations=0,stub_invocations=0,marker_invocations=0,complete_cases=0,partial_cases=0);counts=collections.Counter();rng=random.Random(ENTRY)
 selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(flags=flags,tail_flags=tail,mode=mode) for flags,tail,mode in itertools.product((0,1,16,31),(0,4,8,64),(0,2,4,6))]
 for n,c in enumerate(selected):
  c=dict(c,appearance_flags=c['tail_flags']|0xa001)
  m=bytearray(ram);store(m,TASK+32,WORK);store(m,TASK+36,ENTITY);store(m,ENTITY+0xec,APPEARANCE);store(m,ENTITY+0x2d4,NODE)
  for i in range(13):store(m,NODE+i*4,0xaabb0000+i*0x101)
  for i in range(16):store(m,WORK+i*4,0x12340000+i*0x101)
  for i in range(4):store(m,APPEARANCE+0x20+i*4,0x3e000000+i*0x10000)
  store(m,APPEARANCE+0x34,c['flags'],1);store(m,APPEARANCE+0x36,c['appearance_flags'],2)
  store(m,ENTITY+0x812,0xaabbccdd);store(m,ENTITY+0x816,0x12345678);store(m,ENTITY+0x60,0x89abcdef);store(m,ENTITY+0x74,0x87654321);store(m,ENTITY+0x78,0xdeadbeef)
  initial=State();initial.r[:]=[rng.getrandbits(32) for _ in range(16)];initial.r[4]=TASK;initial.r[15]=0x8cf00000;initial.pr=0x8cfed000;initial.pc=ENTRY;initial.sr=rng.getrandbits(32)
  actual=bytearray(m);expected=bytearray(m);s=State.from_buffer_copy(initial);other=State.from_buffer_copy(initial)
  f=Fpu();f.fpscr=0x40000|(c['mode']<<19)
  for bank in range(2):
   for i in range(16):f.banks[bank][i]=rng.getrandbits(32)
  ef=Fpu.from_buffer_copy(f);want=[];got=[];path=[];errors=[]
  def event(t,mem,r,sr,pr,fp):return t,tuple(r),sr,pr,bytes(fp)[:132],tuple(load(mem,ENTITY+o) for o in (0x60,0x74,0x78,0xd8,0xdc,0xe0,0xe4,0xe8,0xec,0x2d4)),tuple(load(mem,WORK+i*4) for i in range(8)),tuple(load(mem,NODE+i*4) for i in range(13))
  def effect(target,mem,r,sr,fp):
   assert target in SERVICES and r[4]==WORK and r[5]==ENTITY,(c,hex(target),hex(r[4]),hex(r[5]))
   slot=(SETUP+TAILS).index(target);store(mem,SCRATCH+slot*4,load(mem,SCRATCH+slot*4)+1)
   for i in range(8):r[i]=0xbad00000+i*0x101+slot
   bank=(fp.fpscr>>21)&1
   for i in range(8):fp.banks[bank][i]=0x3f000000+i*0x10000+slot
   return sr^1
  def stop(target,r,sr):
   other.r[:]=r;other.sr=sr;other.pc=target;raise Stopped()
  def hook(target):
   def call(mem,r,sr):
    want.append(event(target,mem,r,sr,other.pr,ef))
    if c.get('missing_target')==target:stop(target,r,sr)
    sr=effect(target,mem,r,sr,ef)
    if c.get('error_target')==target:stop(target,r,sr)
    return sr
   return call
  def trace(pc,w,r,sr,pr,fp):
   assert pc in own|child|stub,hex(pc);path.append(pc)
   if pc in (SELECTOR,STUB,MARKER):want.append(event(pc,expected,r,sr,pr,ef))
  expected_status=0
  try:reference(expected,other,fpu=ef,hooks={t:hook(t) for t in SERVICES},trace=trace,instruction_budget=1000)
  except Stopped:expected_status=-2 if 'missing_target' in c else -17
  @Call
  def external(ctx,target,mem,fp,sr,r,pr):
   try:
    if c.get('missing_target')==target:return -2
    sr[0]=effect(target,mem,r,sr[0],fp.contents)
    return -17 if c.get('error_target')==target else 0
   except Exception as exc:errors.append(repr(exc));return -99
  @OBSERVE
  def observe(ctx,target,mem,fp,sr,r,pr):
   try:got.append(event(target,mem,r[:16],sr[0],pr[0],fp.contents))
   except Exception as exc:errors.append(repr(exc))
  status=fn((ctypes.c_uint8*len(actual)).from_buffer(actual),ctypes.byref(s),ctypes.byref(f),None,external,observe)
  assert not errors and status==expected_status and got==want,(c,'status/boundaries',status,expected_status,errors)
  assert actual==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef),(c,'original RAM/CPU/FPU')
  rows.update(set(path)&own);child_rows.update(set(path)&child);stub_rows.update(set(path)&stub)
  for i,a in enumerate(path[:-1]):
   if a not in own:continue
   w=int.from_bytes(binary[a-0xc900000:a-0xc900000+2],'little')
   if w>>8 in (0x89,0x8b):edges.add((a,path[i+1]))
  calls.update(set(path)&{int(a,16) for a in routine['external_call_sites']})
  if status==0:
   for off,width in ((0,1),(1,1),(6,2),(0x14,4),(0x1c,4)):assert load(actual,WORK+off,width)==0
   for off in (0x7c,0x80,0x84,0x74):assert load(actual,ENTITY+off)==0
   assert load(actual,ENTITY+0x78)==0xdead0000 and load(actual,ENTITY+0x60)==0x89abcde1
   for dest,source in ((0xe0,0x24),(0xe4,0x28)):assert load(actual,ENTITY+dest)==load(m,APPEARANCE+source)
   assert load(actual,ENTITY+0xdc)==(load(ram,0xc90fef4) if c['mode']&2 else load(m,APPEARANCE+0x20))
   assert load(actual,ENTITY+0xd8)==load(ram,0xc90fef0)
   assert load(actual,APPEARANCE+0x36,2)==c['appearance_flags']&appearance_mask
   first=next((i for i,bit in enumerate((16,8,4,2,1)) if c['flags']&bit),None)
   assert load(actual,NODE+4)==(load(ram,roots[first]+4) if first is not None else 0xaabb0101)
   assert all(load(actual,NODE+off)==0x3e999999 for off in (0x20,0x24,0x28))
   tail=next((t for bit,t in zip((64,4,8),TAILS) if (c['tail_flags']&appearance_mask)&bit),None)
   assert [t for t,*_ in want]==list(SETUP)+[SELECTOR]+([tail] if tail else [])
   assert load(actual,ENTITY+0x812)==0xaabb0000 and load(actual,ENTITY+0x816)==(0x12340000|load(ram,0xc909d6c,2))
   assert s.r[15]==initial.r[15] and s.pr==initial.pr and list(s.r)[8:15]==list(initial.r)[8:15]
  totals['boundary_comparisons']+=len(got);totals['fixture_invocations']+=sum(t in SERVICES for t,*_ in got);totals['selector_invocations']+=sum(t==SELECTOR for t,*_ in got);totals['stub_invocations']+=sum(t==STUB for t,*_ in got);totals['marker_invocations']+=sum(t==MARKER for t,*_ in got);totals['complete_cases']+=status==0;totals['partial_cases']+=status!=0;counts[status]+=1
  records.append(dict(input=c,status=status,steps=len(path),full_state_match=True,ordered_boundaries_match=True,independent_initializer_selection_checks=status==0))
  if (n+1)%512==0:print(n+1,'initializer/native-selector comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Initializer/native-selector smoke passed:',len(records));return
 assert rows==own and child_rows==child and stub_rows==stub
 assert edges=={tuple(int(a,16) for a in e) for e in routine['conditional_edges']}
 assert calls=={int(a,16) for a in routine['external_call_sites']}
 files=[Path(__file__),manifest_path,selector_report,PROJECT/manifest['source'],PROJECT/'source/sa_overlay_selector_initializer_connected.c',PROJECT/'source/sa_overlay_face_model_selector_0c905548.c',PROJECT/'build_overlay_selector_initializer.ps1',PROJECT/'analysis/overlay-selector-callers/references.tsv',PROJECT/'ghidra/ExportOverlaySelectorReferences.java',PROJECT/'ghidra/export_overlay_selector_callers.ps1']
 files.append(PROJECT/'source/retail_return_delay_stubs.c');files.extend(Path(p) for p in manifest['sha256']);files.extend(Path(p) for p in old['sha256'])
 report=dict(module='AL_MAIN.BIN',load_address='0x0c900000',source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),rows_covered=len(rows),caller_rows_covered=len(rows&caller),marker_rows_covered=len(rows&marker),reused_stub_rows_covered=len(stub_rows),reused_selector_rows_covered=len(child_rows),conditional_edges_covered=len(edges),call_sites_covered=len(calls),status_counts=dict(counts),**totals,remaining_service_targets=[hex(a) for a in sorted(SERVICES)],sha256={str(p):digest(p) for p in files},dll_sha256=digest(DLL),newly_recovered_entries=2,reused_selector_new_credit=0,reused_stub_new_credit=0,scope='Complete native initialization caller and marker reset, with previously recovered native model selector and empty stub, original full RAM/CPU/FPU and ordered call-boundary comparisons, independent reset/copy/mask/model/scale/tail-priority assertions. All selector flag bytes and transfer modes plus explicit dependency-failure partial states.',limitations=['Five other initialization helpers and three tail-state helpers remain explicit state-mutating fixtures. This verifies caller behavior and selector integration, not a complete initialized scene or gameplay.', 'Constructed valid task/work/entity/appearance/object pointers. No hardware MMIO/alignment/fault or scene scheduling/pixels proof. Raw wide transfers preserve original overlap effects; they do not establish normal gameplay use.'],cases=records)
 (PROJECT/'analysis/overlay_selector_initializer_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','rows_covered','reused_selector_rows_covered','conditional_edges_covered','call_sites_covered','status_counts',*totals)},indent=2))
if __name__=='__main__':main()
