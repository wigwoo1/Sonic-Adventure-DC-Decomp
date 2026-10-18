"""Original dataflow, reached guards and independent matrix algebra checks."""
import collections,csv,ctypes,hashlib,inspect,itertools,json,random,sys
from fractions import Fraction
from pathlib import Path
import verify_overlay_model_blend as morph
import verify_sh4_vector_numeric as vectors
from verify_startup import ROOT,State
from verify_fpu_cleanup_transfer import Fpu
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent
SCALE,TRANSLATE=0x8c63a69c,0x8c63a878
MATRIX=0x8cc10000
DLL=ROOT/'work/sonic-adventure/build/draw_matrix_registers.dll';LIB=ctypes.CDLL(str(DLL))
fn=LIB.sa_draw_matrix_registers_connected
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(State),ctypes.POINTER(Fpu),morph.P,morph.P,ctypes.c_uint32];fn.restype=ctypes.c_int
ROWS={}
for entry in (SCALE,TRANSLATE):
 for row in csv.DictReader((PROJECT/f'analysis/main-dispatch-model-traversal/{entry:08x}_instructions.tsv').open(),delimiter='\t'):
  ROWS[int(row['address'],16)]=row['instruction'].lstrip('_')

def vector_primitive(word,f):
 if f.fpscr&3>1:return -1
 if word==0xf5fd:return vectors.primitive(1,4,0,ctypes.byref(f),f.fpul)
 assert word==0xf1ed,hex(word)
 return vectors.primitive(2,0,4,ctypes.byref(f),f.fpul)

def reference_vector(word,f):assert vector_primitive(word,f)==0
namespace=dict(morph.__dict__,ROWS=ROWS,reference_vector=reference_vector,vector_primitive=vector_primitive)
code=inspect.getsource(morph.reference_run)
code=code.replace("c.get('probe') or f.fpscr&0x100000", "c.get('probe') or f.fpscr&0x180000")
code=code.replace('if w==0xb and pr not in (entry_pr,0xc90545a)', 'if w==0xb and pr!=entry_pr')
marker="  scalar=w>>12==15"
assert code.count(marker)==1
code=code.replace(marker,"""  if w==0xf3fd and f.fpscr&0x80000:stop(pc,r,sr,-1)
  if w in (0xf5fd,0xf1ed):
   probe=Fpu.from_buffer_copy(f);rc=vector_primitive(w,probe)
   if rc:stop(pc,r,sr,rc,probe)
  scalar=w>>12==15""")
code=code.replace('hooks=hooked,instruction_budget=400000','hooks=hooked,vector_instruction=reference_vector,instruction_budget=400000')
exec(compile(code,'draw_matrix_reference','exec'),namespace)
reference_run=namespace['reference_run']

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def algebra(before,before_f,s,after,after_f,entry):
 bank=before_f.fpscr>>21&1;explicit=s.r[4]!=0
 matrix=[load(before,s.r[4]+i*4) for i in range(16)] if explicit else list(before_f.banks[bank^1])
 vector=[Fraction(morph.value(before_f.banks[bank][i])) for i in range(4,7)]+[Fraction(1)]
 expected=[Fraction(morph.value(x)) for x in matrix]
 if entry==SCALE:
  for col in range(3):
   for row in range(4):expected[col*4+row]*=vector[col]
 else:
  for row in range(4):expected[12+row]=sum((Fraction(morph.value(matrix[col*4+row]))*vector[col] for col in range(4)),Fraction())
 got=[load(after,s.r[4]+i*4) for i in range(16)] if explicit else list(after_f.banks[bank^1])
 assert [Fraction(morph.value(x)) for x in got]==expected,(hex(entry),explicit,'independent full matrix',list(map(hex,got)),list(map(str,expected)))
 # Implicit operations never write RAM. Explicit operations write only matrix
 # cells: entire untouched RAM, including surrounding bytes, is checked here.
 oracle=bytearray(before)
 if explicit:
  for i,value in enumerate(expected):store(oracle,s.r[4]+i*4,morph.bits(float(value)))
  # Mathematical zero has no sign; retain observed signed-zero encoding only.
  for i,value in enumerate(expected):
   if not value:store(oracle,s.r[4]+i*4,got[i])
 assert after==oracle,'independent write footprint'

def cases():
 for entry,explicit,bank,rm,sz,seed in itertools.product((SCALE,TRANSLATE),range(2),range(2),range(2),range(2),range(16)):
  yield dict(entry=entry,explicit=explicit,bank=bank,rm=rm,sz=sz,seed=seed,semantic=not sz)
 for entry,explicit,mode,rm,enable in itertools.product((SCALE,TRANSLATE),range(2),range(8),range(4),(0x80,0x800)):
  yield dict(entry=entry,explicit=explicit,bank=mode>>2,pr=mode&1,sz=mode>>1&1,rm=rm,enable=enable,probe=True)
 for entry,raw,bank,dn in itertools.product((SCALE,TRANSLATE),(1,0x7f61b1e6,0x7fc00001),range(2),(0,0x40000)):
  yield dict(entry=entry,explicit=1,bank=bank,raw=raw,dn=dn,probe=True)
 for entry,offset,sz,bank in itertools.product((SCALE,TRANSLATE),(-4,0,4,20,48,56,60),range(2),range(2)):
  yield dict(entry=entry,explicit=1,bank=bank,address=0x8d000000+offset,sz=sz)
 for entry,bank,sz in itertools.product((SCALE,TRANSLATE),range(2),range(2)):
  yield dict(entry=entry,explicit=1,bank=bank,address=0,sz=sz,bad=True)

def prepare(ram,c):
 rng=random.Random(c.get('seed',3));m=bytearray(ram);s=State();s.r[:]=[rng.getrandbits(32) for _ in range(16)]
 s.r[4]=c.get('address',MATRIX) if c['explicit'] else 0;s.r[5]=0xa5a5a5a5;s.r[15]=0x8cf00000;s.sr=rng.getrandbits(32);s.pr=0x8cfed000;s.pc=c['entry']
 if c.get('bad'):s.r[4]=0x80000000
 f=Fpu();f.fpscr=c.get('dn',0x40000)|(c.get('bank',0)<<21)|(c.get('sz',0)<<20)|(c.get('pr',0)<<19)|c.get('rm',0)|c.get('enable',0);f.fpul=rng.getrandbits(32)
 for bank in range(2):
  for i in range(16):f.banks[bank][i]=morph.bits(rng.randrange(-16,17)/8)
 for i in range(16):store(m,MATRIX+i*4,morph.bits(rng.randrange(-16,17)/8))
 if 'raw' in c:f.banks[c.get('bank',0)][4]=c['raw']
 return m,s,f

def compare(m,s,f,c):
 original=State.from_buffer_copy(s);before_f=Fpu.from_buffer_copy(f);other=State.from_buffer_copy(s);ef=Fpu.from_buffer_copy(f);actual=bytearray(m);expected=bytearray(m)
 status,path,_,expected_macl=reference_run(expected,other,ef,c,tracked=())
 ml=ctypes.c_uint32(0x12345678)
 rc=fn((ctypes.c_uint8*len(actual)).from_buffer(actual),ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,Fpu.fpul.offset),morph.P),ctypes.byref(ml),original.pc)
 assert rc==status and actual==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef) and ml.value==expected_macl,(c,'full original state',rc,status,hex(s.pc),hex(other.pc))
 semantic=not rc and c.get('semantic',False)
 if semantic:algebra(m,before_f,original,actual,f,original.pc)
 return actual,dict(input=c,status=rc,steps=len(path),stop_pc=hex(s.pc),full_state_match=True,independent_matrix_and_writes=bool(semantic)),path

def main():
 manifest_path=PROJECT/'analysis/draw_matrix_registers_translation.json';manifest=json.loads(manifest_path.read_text());vector_proof=PROJECT/'analysis/sh4_vector_numeric_verification.json';vector_report=json.loads(vector_proof.read_text())
 for item in (manifest,vector_report):
  for p,h in item['sha256'].items():assert digest(p)==h,p
 assert digest(vectors.BUILD/'sh4_vector_numeric_reference.dll')==vector_report['primitive_reference_dll_sha256']
 ram=bytearray(16<<20);binary=morph.MAIN.read_bytes();ram[0x10000:0x10000+len(binary)]=binary
 records=[];coverage=set();edges=set();selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(entry=e,explicit=x,bank=b,semantic=True) for e,x,b in itertools.product((SCALE,TRANSLATE),range(2),range(2))]+[dict(entry=SCALE,explicit=0,pr=1),dict(entry=TRANSLATE,explicit=1,sz=1)]
 for n,c in enumerate(selected):
  m,s,f=prepare(ram,c);_,record,path=compare(m,s,f,c);records.append(record);coverage.update(path)
  for i,a in enumerate(path[:-1]):
   w=int.from_bytes(binary[a-0x8c010000:a-0x8c010000+2],'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f):
    j=i+(2 if w>>8 in (0x8d,0x8f) else 1)
    if j<len(path):edges.add((a,path[j]))
  if (n+1)%256==0:print(n+1,'matrix-register comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Matrix-register smoke passed:',len(records));return
 wanted={tuple(int(a,16) for a in e) for r in manifest['routines'] for e in r['conditional_edges']}
 assert coverage==set(ROWS) and wanted==edges,(set(ROWS)-coverage,wanted-edges)
 files=[Path(__file__),manifest_path,vector_proof,PROJECT/'verify_overlay_model_blend.py',PROJECT/'verify_sh4_vector_numeric.py',PROJECT/'source/sa_draw_matrix_registers_connected.c',PROJECT/'build_draw_matrix_registers.ps1',vectors.BUILD/'sh4_vector_numeric_reference.dll']
 for item in (manifest,vector_report):files.extend(Path(p) for p in item['sha256'])
 old=json.loads((PROJECT/'analysis/overlay_model_blend_verification.json').read_text());files.extend(Path(p) for p in old['sha256']);files.append(PROJECT/'analysis/overlay_model_blend_verification.json')
 report=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),rows_covered=len(coverage),conditional_edges_covered=len(edges),independent_cases=sum(r['independent_matrix_and_writes'] for r in records),external_game_service_fixtures=0,newly_registered_entries=2,reused_prior_implementation_credit=0,dll_sha256=digest(DLL),sha256={str(p):digest(p) for p in files},scope='Full original dataflow and complete RAM/CPU/FPU/FPUL/MACL/stop state for guarded scale and translation. Independent exact rational matrix/write-footprint checks for narrow finite dyadic cases. Native scalar/vector numerical backends have earlier separately scoped proofs.',limitations=['Vector differential arithmetic uses a separate adapter to the same pinned upstream core; this validates wiring/dataflow, not independent hardware arithmetic. Rational checks are separate and use exactly representable dyadic matrices.','Reached PR/SZ/RM/trap/RAM boundary tests do not implement CPU exception delivery or prove universal hardware behavior. No scene/pixel/frame-rate claim.','Earlier limited matrix implementations and evidence are preserved; no duplicate credit for their source.'],cases=records)
 (PROJECT/'analysis/draw_matrix_registers_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','rows_covered','conditional_edges_covered','independent_cases')},indent=2))
if __name__=='__main__':main()
