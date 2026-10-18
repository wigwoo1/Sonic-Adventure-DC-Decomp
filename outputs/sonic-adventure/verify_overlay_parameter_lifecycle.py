"""Original shipped objects: native initialization followed by repeated updates."""
import collections,ctypes,hashlib,itertools,json,random,sys
from pathlib import Path
import sh4_binary32
import sh4_multiply_extended_reference as extended_multiply
import verify_overlay_initializer_complete as initial
import verify_overlay_parameter_update as update
from verify_startup import State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_main_dispatch_active_query import OBSERVE
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent
TASK,WORK,ENTITY,APP,HEAP=initial.TASK,initial.WORK,initial.ENTITY,initial.APP,initial.HEAP

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 proofs=[PROJECT/'analysis'/n for n in ('overlay_initializer_complete_verification.json','overlay_parameter_update_verification.json')]
 for proof in proofs:
  for p,h in json.loads(proof.read_text())['sha256'].items():assert digest(p)==h,p
 ram=bytearray(16<<20);mainbytes=update.morph.MAIN.read_bytes();binary=update.morph.MODULE.read_bytes()
 ram[0x10000:0x10000+len(mainbytes)]=mainbytes;ram[0x900000:0x900000+len(binary)]=binary
 selected=list(itertools.product(range(8),range(2),(False,True)))
 if '--smoke' in sys.argv:selected=[(0,0,False),(7,1,True)]
 rng=random.Random(update.UPDATE);records=[];boundaries=0;stages=0;counts=collections.Counter();morph_calls=0
 for t,bank,parts in selected:
  m=bytearray(ram)
  for a,v in ((TASK+32,WORK),(TASK+36,ENTITY),(ENTITY+0xec,APP)):store(m,a,v)
  store(m,APP,t,1);store(m,APP+0x34,16,1);store(m,APP+0x36,0xa04d,2);store(m,APP+0x192,3,1)
  for i in range(7):store(m,APP+0x4c+i,6 if parts else 255,1)
  for i,v in enumerate((-.25,.75,.5)):store(m,APP+0x20+i*4,update.morph.bits(v))
  for i,v in enumerate((-36,35,0)):store(m,APP+0x5c+i,v,1)
  store(m,initial.finish.SEED,0x12345678)
  size=0x10000;store(m,load(m,0x8c098b60),HEAP)
  for i,v in enumerate((HEAP+size,0,HEAP+size,0)):store(m,HEAP+i*4,v)
  for i,v in enumerate((0,HEAP,0,HEAP)):store(m,HEAP+size+i*4,v)
  s=State();s.r[:]=[rng.getrandbits(32) for _ in range(16)];s.r[4]=TASK;s.r[15]=0x8cf00000;s.pr=0x8cfed000;s.pc=initial.ENTRY;s.sr=rng.getrandbits(32)
  f=Fpu();f.fpscr=0x40000|bank*0x200000;f.fpul=rng.getrandbits(32)
  for b in range(2):
   for i in range(16):f.banks[b][i]=rng.getrandbits(32)
  other=State.from_buffer_copy(s);ef=Fpu.from_buffer_copy(f);expected=bytearray(m);special={'macl':0x12345678}
  saved=sh4_binary32.execute_multiply;sh4_binary32.execute_multiply=extended_multiply.execute
  try:reference(expected,other,fpu=ef,hooks={},special=special,instruction_budget=3000000)
  finally:sh4_binary32.execute_multiply=saved
  macl=ctypes.c_uint32(0x12345678)
  rc=initial.fn((ctypes.c_uint8*len(m)).from_buffer(m),ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,Fpu.fpul.offset),update.morph.P),ctypes.byref(macl),None,OBSERVE())
  assert rc==0 and m==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef) and macl.value==special['macl'],(t,bank,parts,'constructor original state')
  history=[]
  # These are supplied test frames, not an assertion about the game's scheduler.
  # Include no-change, both movement signs, event-counter wrap, clamp and Z-only
  # decrease paths. Retain initialized real meshes and shared material mutations.
  for frame in range(8):
   x,y,z=(load(m,APP+0x20+i*4) for i in range(3))
   tx=update.add(x,update.morph.bits(.125),sub=bool(frame&1)) if frame not in (0,7) else x
   ty=update.add(y,update.morph.bits(.0625),sub=not bool(frame&1)) if frame not in (0,7) else y
   tz=update.morph.bits(2 if frame==6 else -.5) if frame in (6,7) else z
   for i,v in enumerate((tx,ty,tz)):store(m,ENTITY+0xdc+i*4,v)
   if frame in (2,4):
    store(m,APP+0x36,load(m,APP+0x36,2)|1,2)
    for i in range(4):
     store(m,APP+0xc+i*2,65530 if frame==4 else 100+i*20,2)
     store(m,APP+0x54+i*2,65535 if frame==4 else 105+i*20,2)
   s.r[4]=WORK;s.r[5]=ENTITY;s.pc=update.UPDATE;s.pr=0x8cfed000
   m,record,path,n=update.compare(m,s,f,dict(frame=frame,macl=macl.value),True,ENTITY,APP,WORK)
   assert record['status']==0;boundaries+=n;stages+=1;morph_calls+=path.count(update.BLEND);counts[record['status']]+=1
   history.append(dict(stage='parameter',**record))
   if frame==0:
    store(m,WORK+0x30,update.morph.bits(1.5));store(m,ENTITY+0xd4,update.morph.bits(-.125))
   s.r[4]=WORK;s.r[5]=ENTITY;s.pc=update.SPRING;s.pr=0x8cfed000
   m,record,path,n=update.compare(m,s,f,dict(frame=frame,macl=macl.value),True,ENTITY,APP,WORK)
   assert record['status']==0;boundaries+=n;stages+=1;counts[record['status']]+=1;history.append(dict(stage='scale',**record))
  records.append(dict(type=t,bank=bank,accessories=parts,constructor_full_state_match=True,updates=history))
  print('Shipped-object lifecycle passed:',t,bank,parts,flush=True)
 if '--smoke' in sys.argv:print('Shipped-object lifecycle smoke passed:',len(records));return
 files=[Path(__file__),PROJECT/'verify_overlay_parameter_update.py',PROJECT/'verify_overlay_initializer_complete.py',initial.DLL,update.DLL]+proofs
 for proof in proofs:files.extend(Path(p) for p in json.loads(proof.read_text())['sha256'])
 report=dict(cases_passed=len(records),constructor_comparisons=len(records),update_stages=stages,independent_stages=stages,boundary_comparisons=boundaries,morph_calls=morph_calls,status_counts=dict(counts),external_game_service_fixtures=0,newly_recovered_entries=0,reused_routine_credit=0,dll_sha256=digest(update.DLL),sha256={str(p):digest(p) for p in files},scope='Native original-model initialization followed by eight supplied parameter-update/scale frames per instance. Original complete state matches at construction and after every stage; ordered update/morph/copy boundaries and independent parameter/event/scale data checks.',limitations=['All eight appearance types, both FPU banks, with and without source accessories; supplied task/entity/heap and frame inputs. Natural task creation and scheduling are not recovered here.','No draw callback, visible pixels, platform renderer, fixed/high-FPS timing or playable-game claim. Existing complete-initializer and morph/copy sources remain unchanged.'],cases=records)
 (PROJECT/'analysis/overlay_parameter_lifecycle_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','constructor_comparisons','update_stages','boundary_comparisons','morph_calls','status_counts')},indent=2))
if __name__=='__main__':main()
