"""Accepted object draw prefixes through reused native state/matrix services."""
import collections,csv,ctypes,hashlib,itertools,json,sys
from pathlib import Path
import verify_overlay_draw_gate as gate
import verify_sh4_vector_numeric as vectors
from verify_startup import ROOT,State
from verify_fpu_cleanup_transfer import Fpu
from verify_main_dispatch_active_query import OBSERVE
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent
DRAW=gate.DRAW;STACK=0x8ce00000
CPU=(0x8c056990,0x8c056986,0x8c056926,0x8c056938,0x8c05694a,0x8c608ccc)
LEGACY={0x8c639c70,0x8c639b98,0x8c63a804,0x8c639ec8,0x8c639f5c,0x8c63a1cc}
MATRICES=LEGACY|{0x8c63a69c,0x8c63a878}
CONNECTED=set(CPU)|MATRICES|{DRAW,gate.SPRING,gate.PRED,gate.support.WRAPPER,gate.support.COPY}
ROWS=dict(gate.ROWS)
LISTINGS=list((PROJECT/'analysis/overlay-draw-state-services').glob('*_instructions.tsv'))+[PROJECT/f'analysis/main-dispatch-model-traversal/{a:08x}_instructions.tsv' for a in (0x8c63a69c,0x8c63a878)]
for listing in LISTINGS:
 for row in csv.DictReader(listing.open(),delimiter='\t'):ROWS[int(row['address'],16)]=row['instruction'].lstrip('_')
candidate_manifest=json.loads((PROJECT/'analysis/overlay_draw_candidate_translation.json').read_text())
CALLS=set(int(a,16) for a in candidate_manifest['routines'][0]['external_call_sites'])
RETURNS={a+4 for a in CALLS}|{0x8c043a38}
flows=list(csv.DictReader((PROJECT/'analysis/overlay-object-update-callers/0c9058a0_instructions.tsv').open(),delimiter='\t'))
MISSING_TARGETS={int(r['flows'].strip(),16) for r in flows if int(r['address'],16) in CALLS}-CONNECTED
VECTOR={s[3]:s[:3] for s in vectors.SPECS}
def primitive(word,f):
 if f.fpscr&3>1:return -1
 kind,n,m=VECTOR[word];return vectors.primitive(kind,n,m,ctypes.byref(f),f.fpul)
def reference_vector(word,f):assert primitive(word,f)==0
namespace=dict(gate.namespace,ROWS=ROWS,RETURNS=RETURNS,MISSING_TARGETS=MISSING_TARGETS,LEGACY=LEGACY,VECTOR_WORDS=set(VECTOR),vector_primitive=primitive,reference_vector=reference_vector)
code=gate.code.replace('pc==MISSING','pc in MISSING_TARGETS').replace('pr not in (entry_pr,0x8c043a38,0xc905906,0xc90591a)','pr not in RETURNS|{entry_pr}')
code=code.replace('  access=dynamic_access(text,r,f.fpscr)','  if pc in LEGACY and f.fpscr&0x180000:stop(pc,r,sr,-1)\n  access=dynamic_access(text,r,f.fpscr)').replace('if w in (0xf5fd,0xf1ed):','if w in VECTOR_WORDS:')
exec(compile(code,'overlay_draw_state_reference','exec'),namespace)
reference_run=namespace['reference_run']
DLL=ROOT/'work/sonic-adventure/build/overlay_draw_state.dll';LIB=ctypes.CDLL(str(DLL));fn=LIB.sa_overlay_draw_state_connected
fn.argtypes=gate.fn.argtypes;fn.restype=ctypes.c_int
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def cases():
 yield from gate.cases()
 for t,bank,flag,angles,nonnull in itertools.product((0,7),range(2),(0,4,0x10,0x14),(False,True),(False,True)):
  yield dict(type=t,bank=bank,draw_flags=flag,angles=angles,nonnull=nonnull,semantic=True)
 for flag,bank,limit in itertools.product((0,4,0x10,0x14),range(2),(1,16)):
  yield dict(draw_flags=flag,bank=bank,limit=limit,semantic=True)
 for entry,bank,mask in itertools.product(CPU,range(2),(0,1,0x200,0xffffffff)):
  yield dict(entry=entry,bank=bank,mask=mask,direct=True)
 for entry,bank in itertools.product(sorted(MATRICES),range(2)):
  yield dict(entry=entry,bank=bank,mask=0,direct=True,matrix=True)

def prepare(ram,c):
 m,s,f=gate.constructed(ram,c)
 if 'draw_flags' in c:store(m,gate.ENTITY+0x60,c['draw_flags'])
 if c.get('angles'):
  for a,v in ((gate.WORK+0x14,0x1000),(gate.WORK+0x18,0x2000),(gate.WORK+0x1c,0x3000),(gate.ENTITY+0x7c,0x0800),(gate.ENTITY+0x80,0xf800)):store(m,a,v)
 store(m,gate.ENTITY+0x7c4,0x8cc60000 if c.get('nonnull') else 0)
 store(m,0x8c88f698,c.get('limit',16));store(m,0x8c88f69c,1);store(m,0x8c88f5f8,STACK)
 for i,literal in enumerate((0x8c0569ac,0x8c0569b4,0x8c0569b0,0x8c0569b8,0x8c0569bc)):
  store(m,load(m,literal),(0xa5a50000+i*0x1111)|0x200)
 if c.get('direct'):
  s.pc=c['entry'];s.r[4]=c['mask'];s.r[5]=c['mask']^0x12345678
  if c['entry']==0x8c63a804:s.r[5]=gate.WORK+0x20
 return m,s,f

def compare(m,s,f,c):
 entry=s.pc;other=State.from_buffer_copy(s);ef=Fpu.from_buffer_copy(f);actual=bytearray(m);expected=bytearray(m)
 status,path,wanted,expected_macl=reference_run(expected,other,ef,c,tracked=tuple(CONNECTED))
 ml=ctypes.c_uint32(0x12345678);got=[];errors=[]
 A,B,SA,SB,W=(load(m,a) for a in (0x8c0569ac,0x8c0569b4,0x8c0569b0,0x8c0569b8,0x8c0569bc));PTR=0x8c673480
 independent={a:load(m,a) for a in (A,B,SA,SB,W,PTR)};float_words=None;depth=1;pointer=STACK;stack_saved=None
 @OBSERVE
 def observe(ctx,target,mem,fp,sr,r,pr):
  nonlocal float_words,depth,pointer,stack_saved
  try:
   got.append((target,tuple(r[:16]),sr[0],pr[0],bytes(fp.contents),ml.value,hashlib.sha256(ctypes.string_at(ctypes.addressof(mem.contents),16<<20)).digest()))
   if target==0x8c056990:independent[W]&=~r[4]&0xffffffff
   elif target==0x8c056986:independent[W]|=r[4]
   elif target==0x8c056926:independent[SA]=independent[A];independent[SB]=independent[B]
   elif target==0x8c056938:independent[A]=independent[SA];independent[B]=independent[SB]
   elif target==0x8c05694a:independent[A]|=r[4];independent[B]|=r[5]
   elif target==0x8c608ccc and r[4]:independent[PTR]=r[4]
   if target==gate.support.WRAPPER and not fp.contents.fpscr&0x180000:
    bank=fp.contents.fpscr>>21&1;float_words=list(fp.contents.banks[bank][4:8])
   if target==0x8c639c70 and depth<c.get('limit',16):
    depth+=1;pointer+=64
    if stack_saved is None:stack_saved=list(fp.contents.banks[(fp.contents.fpscr>>21&1)^1])
   elif target==0x8c639b98 and (depth-r[4])&0xffffffff>=1:depth=(depth-r[4])&0xffffffff;pointer=(pointer-r[4]*64)&0xffffffff
  except Exception as exc:errors.append(repr(exc))
 rc=fn((ctypes.c_uint8*len(actual)).from_buffer(actual),ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,Fpu.fpul.offset),gate.support.morph.P),ctypes.byref(ml),entry,None,observe)
 assert not errors and rc==status and got==wanted and actual==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef) and ml.value==expected_macl,(c,'full original state/boundaries',rc,status,hex(s.pc),hex(other.pc),errors,len(got),len(wanted))
 checked=bool(c.get('direct') or c.get('semantic'))
 if checked:
  assert all(load(actual,a)==v for a,v in independent.items()),(c,'independent global fields')
  if float_words is not None:assert [load(actual,gate.support.DEST+i*4) for i in range(4)]==float_words,(c,'independent raw state')
  assert load(actual,0x8c88f69c)==depth and load(actual,0x8c88f5f8)==pointer,(c,'independent matrix stack')
  if stack_saved is not None:assert [load(actual,STACK+i*4) for i in range(16)]==stack_saved,(c,'independent saved XMTRX')
  if not c.get('direct'):
   oracle=gate.update.spring_oracle(m,gate.ENTITY,gate.WORK)
   for a,n in ((gate.ENTITY+0xd4,4),(gate.WORK+0x2c,8)):assert gate.support.morph.bytes_at(actual,a,n)==gate.support.morph.bytes_at(oracle,a,n)
 return dict(input=c,status=rc,full_state_match=True,ordered_boundaries_match=True,independent_state_fields=checked,stop_pc=hex(s.pc),steps=len(path)),path,len(got)

def main():
 proof_names=('overlay_draw_state_connection.json','overlay_draw_gate_verification.json','draw_matrix_registers_verification.json')
 proofs=[PROJECT/'analysis'/n for n in proof_names]
 for proof in proofs:
  for p,h in json.loads(proof.read_text())['sha256'].items():assert digest(p)==h,p
 mainbytes=gate.support.morph.MAIN.read_bytes();binary=gate.support.morph.MODULE.read_bytes();ram=bytearray(16<<20);ram[0x10000:0x10000+len(mainbytes)]=mainbytes;ram[0x900000:0x900000+len(binary)]=binary
 records=[];coverage=set();boundaries=0;targets=collections.Counter();selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(type=0,semantic=True),dict(type=7,bank=1,draw_flags=0x10,semantic=True),dict(draw_flags=0x14,angles=True,semantic=True),dict(disabled=1,semantic=True),dict(entry=CPU[1],mask=0x200,direct=True)]
 for n,c in enumerate(selected):
  m,s,f=prepare(ram,c);record,path,count=compare(m,s,f,c);records.append(record);coverage.update(path);boundaries+=count
  targets.update(a for a in path if a in CONNECTED)
  if (n+1)%64==0:print(n+1,'native accepted-draw/state comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Native accepted-draw/state smoke passed:',len(records));return
 candidate=set(gate.CANDIDATE);stops=collections.Counter(r['stop_pc'] for r in records if r['status']==-2)
 assert '0x8c056990' not in stops
 assert all(targets[a] for a in set(CPU)|MATRICES)
 files=[Path(__file__),PROJECT/'verify_overlay_draw_gate.py',PROJECT/'verify_sh4_vector_numeric.py']+proofs+LISTINGS
 for proof in proofs:files.extend(Path(p) for p in json.loads(proof.read_text())['sha256'])
 report=dict(cases_passed=len(records),direct_service_cases=sum(c.get('direct',False) for c in selected),status_counts=dict(collections.Counter(r['status'] for r in records)),candidate_rows_covered=len(candidate&coverage),candidate_unexecuted_rows=len(candidate-coverage),candidate_static_rows=911,boundary_comparisons=boundaries,independent_cases=sum(r['independent_state_fields'] for r in records),target_invocations={hex(a):n for a,n in sorted(targets.items())},first_unconnected_targets=dict(stops),external_game_service_fixtures=0,newly_recovered_entries=0,reused_routine_credit=0,recovered_caller_credit=0,dll_sha256=digest(DLL),sha256={str(p):digest(p) for p in files},scope='Separate partial object-draw bridge connects existing global-state, stack, translation, rotation, scale and four-float services. Original full reached state/boundaries, independent global fields/raw state/saved matrix/stack/spring checks. Native initialized shipped objects with supplied state flags and angles; direct CPU routing checks.',limitations=['Candidate remains unregistered: not all 911 rows/50 outcomes or remaining services are executed. Missing services are explicit -2 stops, without game-service substitutes.','Legacy reused matrix services retain PR=SZ=0 and valid original RAM mappings; no universal partial-error/exception-delivery guarantee. Scalar/vector numerical and supplied camera/record limits remain.','No GPU pixels, full render/frame scheduling, high-FPS behavior, gameplay or playable-port proof.'],cases=records)
 (PROJECT/'analysis/overlay_draw_state_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','direct_service_cases','status_counts','candidate_rows_covered','candidate_unexecuted_rows','boundary_comparisons','independent_cases','first_unconnected_targets')},indent=2))
if __name__=='__main__':main()
