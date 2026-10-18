"""Real draw entry: native spring/culling and original rejected returns."""
import collections,csv,ctypes,hashlib,itertools,json,sys
from pathlib import Path
import verify_draw_predicate_support as support
import verify_overlay_initializer_complete as initial
import verify_overlay_parameter_update as update
from verify_startup import ROOT,State
from verify_fpu_cleanup_transfer import Fpu
from verify_main_dispatch_active_query import OBSERVE
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent
DRAW,SPRING,PRED=0xc9058a0,update.SPRING,support.PRED
MISSING=0x8c056990
TASK,WORK,ENTITY,APP,HEAP=initial.TASK,initial.WORK,initial.ENTITY,initial.APP,initial.HEAP
DLL=ROOT/'work/sonic-adventure/build/overlay_draw_gate.dll';LIB=ctypes.CDLL(str(DLL));fn=LIB.sa_overlay_draw_gate_connected
fn.argtypes=support.fn.argtypes;fn.restype=ctypes.c_int
ROWS=dict(support.ROWS);CANDIDATE={}
for row in csv.DictReader((PROJECT/'analysis/overlay-object-update-callers/0c9058a0_instructions.tsv').open(),delimiter='\t'):
 a=int(row['address'],16);ROWS[a]=row['instruction'].lstrip('_');CANDIDATE[a]=ROWS[a]
for row in csv.DictReader((PROJECT/'analysis/overlay-selector-initializer/0c905618_instructions.tsv').open(),delimiter='\t'):
 ROWS[int(row['address'],16)]=row['instruction'].lstrip('_')
namespace=dict(support.namespace,ROWS=ROWS,MISSING=MISSING)
code=support.code.replace('pr not in (entry_pr,0x8c043a38)','pr not in (entry_pr,0x8c043a38,0xc905906,0xc90591a)')
marker="  assert pc in ROWS,hex(pc);"
assert code.count(marker)==1
code=code.replace(marker,"  if pc==MISSING:events.append(event(pc,r,sr,pr));stop(pc,r,sr,-2)\n"+marker)
exec(compile(code,'overlay_draw_gate_reference','exec'),namespace)
reference_run=namespace['reference_run']
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def constructed(ram,c):
 m=bytearray(ram)
 for a,v in ((TASK+32,WORK),(TASK+36,ENTITY),(ENTITY+0xec,APP)):store(m,a,v)
 store(m,APP,c.get('type',0),1);store(m,APP+0x34,16,1)
 for i in range(7):store(m,APP+0x4c+i,255,1)
 for i,v in enumerate((-.25,.75,.5)):store(m,APP+0x20+i*4,support.morph.bits(v))
 store(m,initial.finish.SEED,0x12345678);size=0x10000;store(m,load(m,0x8c098b60),HEAP)
 for i,v in enumerate((HEAP+size,0,HEAP+size,0)):store(m,HEAP+i*4,v)
 for i,v in enumerate((0,HEAP,0,HEAP)):store(m,HEAP+size+i*4,v)
 s=State();s.r[4]=TASK;s.r[15]=0x8cf00000;s.pr=0x8cfed000;s.pc=initial.ENTRY
 f=Fpu();f.fpscr=0x40000|(c.get('bank',0)<<21);macl=ctypes.c_uint32(0x12345678)
 assert initial.fn((ctypes.c_uint8*len(m)).from_buffer(m),ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,Fpu.fpul.offset),support.morph.P),ctypes.byref(macl),None,OBSERVE())==0
 # Preserve the initialized shipped object. Supply camera/viewport globals,
 # world position and a scale/velocity state for this bounded caller test.
 seed,_,_=support.prepare(ram,{k:v for k,v in c.items() if k!='bad'})
 for a,n in ((0x8c754ec8,4),(0x8c88f610,16),(0x8c88f5e4,4),(0x8c88f5f0,8),(0x8c88f600,16)):
  m[a&0xffffff:(a&0xffffff)+n]=seed[a&0xffffff:(a&0xffffff)+n]
 for i,v in enumerate(c.get('point',(0,0,4))):store(m,WORK+0x20+i*4,support.morph.bits(v))
 store(m,WORK+0x30,support.morph.bits(1.5));store(m,WORK+0x2c,support.morph.bits(.5));store(m,ENTITY+0xd4,support.morph.bits(.125))
 s.r[4]=WORK;s.r[5]=ENTITY;s.r[15]=0x8cf00000;s.pr=0x8cfed000;s.pc=DRAW;s.sr=0x700003f2
 f=Fpu();f.fpscr=0x40000|(c.get('mode',c.get('bank',0)*4)<<19)|c.get('enable',0)|c.get('rm',0);f.fpul=0xabcdef12
 for b in range(2):
  for i in range(16):f.banks[b][i]=support.morph.bits((i+1)/8)
 active=f.fpscr>>21&1;f.banks[active^1][:]=[support.morph.bits(1 if i%5==0 else 0) for i in range(16)]
 if c.get('bad')=='stack':s.r[15]=0
 if c.get('bad')=='work':s.r[4]=0
 return m,s,f

def cases():
 for t,bank,point in itertools.product(range(8),range(2),((0,0,4),(100,0,4),(0,100,4),(0,0,10000))):
  yield dict(type=t,bank=bank,point=point,semantic=True)
 for t,bank in itertools.product(range(8),range(2)):yield dict(type=t,bank=bank,disabled=1,semantic=True)
 for mode in range(8):yield dict(mode=mode)
 for bank,bad in itertools.product(range(2),('stack','work')):yield dict(bank=bank,bad=bad)
 for bank,enable,rm in itertools.product(range(2),(0x80,0x800),(0,2)):yield dict(bank=bank,enable=enable,rm=rm,probe=True)

def main():
 proofs=[PROJECT/'analysis'/n for n in ('draw_predicate_support_verification.json','overlay_draw_candidate_translation.json','overlay_parameter_update_verification.json','overlay_initializer_complete_verification.json')]
 for proof in proofs:
  for p,h in json.loads(proof.read_text())['sha256'].items():assert digest(p)==h,p
 assert digest(initial.DLL)==json.loads(proofs[-1].read_text())['dll_sha256']
 mainbytes=support.morph.MAIN.read_bytes();binary=support.morph.MODULE.read_bytes();ram=bytearray(16<<20);ram[0x10000:0x10000+len(mainbytes)]=mainbytes;ram[0x900000:0x900000+len(binary)]=binary
 records=[];coverage=set();boundaries=0;counts=collections.Counter();semantic_count=0;selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(type=0,bank=0,disabled=1,semantic=True),dict(type=7,bank=1,point=(0,0,4),semantic=True),dict(type=0,bank=0,point=(100,0,4),semantic=True),dict(mode=1)]
 for n,c in enumerate(selected):
  m,s,f=constructed(ram,c);other=State.from_buffer_copy(s);ef=Fpu.from_buffer_copy(f);actual=bytearray(m);expected=bytearray(m)
  status,path,wanted,expected_macl=reference_run(expected,other,ef,c,tracked=(DRAW,SPRING,PRED))
  ml=ctypes.c_uint32(0x12345678);got=[]
  @OBSERVE
  def observe(ctx,target,mem,fp,sr,r,pr):got.append((target,tuple(r[:16]),sr[0],pr[0],bytes(fp.contents),ml.value,hashlib.sha256(ctypes.string_at(ctypes.addressof(mem.contents),16<<20)).digest()))
  rc=fn((ctypes.c_uint8*len(actual)).from_buffer(actual),ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,Fpu.fpul.offset),support.morph.P),ctypes.byref(ml),DRAW,None,observe)
  assert rc==status and got==wanted and actual==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef) and ml.value==expected_macl,(c,'full original draw state',rc,status,hex(s.pc),hex(other.pc))
  checked=False;decision=None
  if c.get('semantic'):
   oracle=update.spring_oracle(m,ENTITY,WORK)
   assert support.morph.bytes_at(actual,ENTITY+0xd4,4)==support.morph.bytes_at(oracle,ENTITY+0xd4,4)
   assert support.morph.bytes_at(actual,WORK+0x2c,8)==support.morph.bytes_at(oracle,WORK+0x2c,8)
   event=next(e for e in got if e[0]==PRED);inputs=Fpu.from_buffer_copy(event[4]);bank=inputs.fpscr>>21&1
   pc=dict(c,point=tuple(support.morph.value(inputs.banks[bank][4+i]) for i in range(3)),radius=support.morph.value(inputs.banks[bank][7]))
   decision=support.predicate_oracle(pc)
   assert rc==(-2 if decision else 0),(c,'independent original gate decision',decision,rc)
   checked=True;semantic_count+=1
  coverage.update(path);boundaries+=len(got);counts[rc]+=1
  records.append(dict(input=c,status=rc,full_state_match=True,ordered_boundaries_match=True,independent_scale_and_gate=checked,predicate_decision=decision,stop_pc=hex(s.pc),steps=len(path)))
  if (n+1)%32==0:print(n+1,'original object-draw gate comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Original object-draw gate smoke passed:',len(records));return
 assert counts[0]>0 and counts[-2]>0
 files=[Path(__file__),PROJECT/'source/sa_overlay_draw_gate_connected.c',PROJECT/'build_overlay_draw_gate.ps1',PROJECT/'verify_draw_predicate_support.py',PROJECT/'verify_overlay_parameter_update.py',initial.DLL]+proofs
 for proof in proofs:files.extend(Path(p) for p in json.loads(proof.read_text())['sha256'])
 report=dict(cases_passed=len(records),status_counts=dict(counts),candidate_rows_covered=len(set(CANDIDATE)&coverage),candidate_static_rows=911,candidate_unexecuted_rows=len(set(CANDIDATE)-coverage),boundary_comparisons=boundaries,independent_cases=semantic_count,newly_recovered_entries=0,recovered_caller_credit=0,external_game_service_fixtures=0,first_unconnected_target=hex(MISSING),sha256={str(p):digest(p) for p in files},dll_sha256=digest(DLL),scope='Original AL_MAIN object draw entry on constructed runtime records with natively initialized shipped objects. Spring and culling run natively; rejected paths return through the original epilogue. Accepted paths stop at the first unconnected render-state service. Full reached RAM/CPU/FPU/FPUL/MACL and ordered entries match; independent scale/gate checks.',limitations=['Complete candidate draw source compiles and is byte-checked, but only entry/culling/rejection/first-continuation paths are behavior-verified. No draw-callback registry credit.','8c056990 already has recovered source; it is not yet wired into this new bridge. Its -2 stop denotes an integration boundary, not a successfully rendered frame or an unrecovered helper.','Supplied camera, position, scale and record inputs; no natural creation/frame scheduling, GPU pixels, full gameplay or playable-port proof.'],cases=records)
 (PROJECT/'analysis/overlay_draw_gate_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','candidate_rows_covered','candidate_unexecuted_rows','boundary_comparisons','independent_cases')},indent=2))
if __name__=='__main__':main()
