"""Original distance/signed-sqrt states with independent rational FSQRT."""
import ast,collections,csv,ctypes,hashlib,inspect,itertools,json,math,random,struct,sys
from fractions import Fraction
from pathlib import Path
import sh4_binary32 as arithmetic
import verify_overlay_model_blend as morph
from verify_startup import ROOT,State
from verify_fpu_cleanup_transfer import Fpu
from verify_main_dispatch_active_query import OBSERVE
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent
DIST,SQRT=0x8c640128,0x8c63a9c4
A,B=0x8cc80000,0x8cc90000
DLL=ROOT/'work/sonic-adventure/build/draw_distance_helpers.dll';LIB=ctypes.CDLL(str(DLL));fn=LIB.sa_draw_distance_helpers_connected
fn.argtypes=morph.fn.argtypes;fn.restype=ctypes.c_int
LIB.sa_sh4_sqrt.argtypes=[ctypes.c_uint32,morph.P,morph.P];LIB.sa_sh4_sqrt.restype=ctypes.c_int
# Extract only the old independent rational oracle functions. Importing that
# script would execute its test main and overwrite the preserved old report.
ORACLE_FILE=PROJECT/'verify_native_sh4_sqrt.py';oracle_namespace=dict(Fraction=Fraction,math=math,struct=struct)
for node in ast.parse(ORACLE_FILE.read_text()).body:
 if isinstance(node,ast.FunctionDef) and node.name in ('value','oracle'):
  exec(compile(ast.Module(body=[node],type_ignores=[]),str(ORACLE_FILE),'exec'),oracle_namespace)
sqrt_oracle=oracle_namespace['oracle']
ROWS={}
for entry in (DIST,SQRT):
 for row in csv.DictReader((PROJECT/f'analysis/overlay-draw-distance-helpers/{entry:08x}_instructions.tsv').open(),delimiter='\t'):
  ROWS[int(row['address'],16)]=row['instruction'].lstrip('_')
def sqrt_probe(word,f):
 p=Fpu.from_buffer_copy(f);bank=p.fpscr>>21&1;n=word>>8&15;raw=p.banks[bank][n];absolute=raw&0x7fffffff
 if p.fpscr&0x80000 or p.fpscr&3>1:return -1,p
 if absolute==0 or raw==0x7f800000:p.fpscr&=~0x3f000;return 0,p
 if raw&0x80000000 or absolute<0x800000 or absolute>=0x7f800000:return -1,p
 out,inexact=sqrt_oracle(raw,p.fpscr&1);p.fpscr&=~0x3f000
 if inexact:p.fpscr|=0x1004
 # Preserve the existing separately qualified normal-input EN.I trap policy.
 if p.fpscr&0x80:return 1,p
 p.banks[bank][n]=out;return 0,p
def reference_sqrt(word,f):
 rc,p=sqrt_probe(word,f);assert rc==0
 ctypes.memmove(ctypes.byref(f),ctypes.byref(p),ctypes.sizeof(Fpu))
namespace=dict(morph.__dict__,ROWS=ROWS,sqrt_probe=sqrt_probe,reference_sqrt=reference_sqrt)
code=inspect.getsource(morph.reference_run).replace("c.get('probe') or f.fpscr&0x100000", "c.get('probe') or f.fpscr&0x180000")
code=code.replace('pr not in (entry_pr,0xc90545a)','pr!=entry_pr').replace('(0xf08d,0xf09d,0xf04d)','(0xf08d,0xf09d,0xf04d,0xf05d)')
marker='  scalar=w>>12==15';assert code.count(marker)==1
code=code.replace(marker,"  if w&0xf0ff==0xf06d:\n   rc,probe=sqrt_probe(w,f)\n   if rc:stop(pc,r,sr,rc,probe)\n"+marker)
code=code.replace('hooks=hooked,instruction_budget=400000','hooks=hooked,vector_instruction=reference_sqrt,instruction_budget=400000')
exec(compile(code,'draw_distance_reference','exec'),namespace)
reference_run=namespace['reference_run']
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def independent_distance(before,initial,after_f,rm):
 delta=[arithmetic.add_sub(load(before,initial.r[4]+i*4),load(before,initial.r[5]+i*4),True,rm)[0] for i in range(3)]
 x,y,z=delta;acc=arithmetic.multiply(y,y,rm)[0]
 acc=arithmetic.multiply_add(x,x,acc,rm)[0];acc=arithmetic.multiply_add(z,z,acc,rm)[0]
 result=sqrt_oracle(acc,rm)[0];bank=after_f.fpscr>>21&1
 assert after_f.banks[bank][4]==acc and after_f.banks[bank][0]==result,'independent rounded distance and root'

def cases():
 points=(((0,0,0),(0,0,0)),((0,0,0),(3,4,0)),((1,2,3),(-1,-2,-3)),((.25,.5,.75),(1.25,-.5,2.75)),((1/7,2/11,3/13),(-4/17,5/19,-6/23)),((-16,8,-4),(2,-1,.5)))
 for (a,b),bank,rm,sz in itertools.product(points,range(2),range(2),range(2)):
  yield dict(a=a,b=b,mode=bank*4+sz*2,rm=rm,semantic=not sz)
 rng=random.Random(DIST)
 for i in range(96):
  yield dict(a=[rng.randrange(-128,129)/16 for _ in range(3)],b=[rng.randrange(-128,129)/16 for _ in range(3)],mode=(i%2)*4,rm=i//2%2,semantic=True)
 raw_values=(0,0x80000000,0x3f800000,0xbf800000,0x40000000,0xc0000000,0x40800000,0xc0800000,0x00800000,0x80800000,0x7f7fffff,0xff7fffff,0x7f800000,0xff800000,0x7fbfffff,0x7fc00001,1,0x80000001)
 for raw,mode,rm in itertools.product(raw_values,range(8),range(2)):
  yield dict(entry=SQRT,raw=raw,mode=mode,rm=rm,semantic=not mode&1 and raw&0x7fffffff<0x7f800000 and (raw&0x7fffffff==0 or raw&0x7fffffff>=0x800000))
 for entry,mode,rm,enable in itertools.product((DIST,SQRT),range(8),range(4),(0x80,0x800)):
  yield dict(entry=entry,raw=0xc0000000,mode=mode,rm=rm,enable=enable,probe=True)
 for kind,mode in itertools.product(('left','right','stack','alias'),(0,2,4,6)):
  yield dict(bad=kind,mode=mode)
 for bank in range(2):yield dict(a=(0,0,0),b=(1,0,0),shared=True,mode=bank*4,semantic=True)

def prepare(ram,c):
 m=bytearray(ram);rng=random.Random(0x1234);s=State();s.r[:]=[rng.getrandbits(32) for _ in range(16)];s.r[4]=A;s.r[5]=A if c.get('shared') else B;s.r[15]=0x8cf00000;s.pr=0x8cfed000;s.pc=c.get('entry',DIST);s.sr=rng.getrandbits(32)
 for pointer,values in ((A,c.get('a',(0,0,0))),(B,c.get('b',(1,2,3)))):
  for i,v in enumerate(values):store(m,pointer+i*4,morph.bits(v))
 if c.get('bad')=='left':s.r[4]=0
 if c.get('bad')=='right':s.r[5]=0x8cfffffc
 if c.get('bad')=='stack':s.r[15]=0
 if c.get('bad')=='alias':s.r[4]=s.r[15]-20
 f=Fpu();f.fpscr=c.get('dn',0x40000)|(c.get('mode',0)<<19)|c.get('rm',0)|c.get('enable',0);f.fpul=0xabcdef12
 for bank in range(2):
  for i in range(16):f.banks[bank][i]=morph.bits((i+1)/8)
 f.banks[f.fpscr>>21&1][4]=c.get('raw',morph.bits(2))
 return m,s,f

def compare(m,s,f,c):
 entry=s.pc;initial=State.from_buffer_copy(s);before_f=Fpu.from_buffer_copy(f);other=State.from_buffer_copy(s);ef=Fpu.from_buffer_copy(f);actual=bytearray(m);expected=bytearray(m)
 status,path,wanted,expected_macl=reference_run(expected,other,ef,c,tracked=(DIST,SQRT));got=[];ml=ctypes.c_uint32(0x12345678)
 @OBSERVE
 def observe(ctx,target,mem,fp,sr,r,pr):got.append((target,tuple(r[:16]),sr[0],pr[0],bytes(fp.contents),ml.value,hashlib.sha256(ctypes.string_at(ctypes.addressof(mem.contents),16<<20)).digest()))
 rc=fn((ctypes.c_uint8*len(actual)).from_buffer(actual),ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,Fpu.fpul.offset),morph.P),ctypes.byref(ml),entry,None,observe)
 assert rc==status and got==wanted and actual==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef) and ml.value==expected_macl,(c,'full original state/boundaries',rc,status,hex(s.pc),hex(other.pc))
 semantic=not rc and c.get('semantic',False)
 if semantic:
  if entry==DIST:independent_distance(m,initial,f,c.get('rm',0))
  else:
   raw=c['raw'];out=sqrt_oracle(raw&0x7fffffff,c.get('rm',0))[0]|(raw&0x80000000)
   assert f.banks[f.fpscr>>21&1][0]==out and actual==m,'independent signed sqrt and RAM footprint'
 return dict(input=c,status=rc,full_state_match=True,ordered_boundaries_match=True,independent_checks=bool(semantic),steps=len(path),stop_pc=hex(s.pc)),path,len(got)

def core_checks():
 rng=random.Random(SQRT);raws=[0,0x80000000,0x7f800000,0x00800000,0x3f800000,0x40000000,0x40800000,0x7f7fffff]+[rng.randrange(0x800000,0x7f800000) for _ in range(256)];count=0
 for raw,rm,enable,sticky in itertools.product(raws,range(2),(0,0x80),(0,0x7c)):
  f=Fpu();f.fpscr=0x200000|0x3f000|rm|enable|sticky;f.banks[1][0]=raw
  rc,want=sqrt_probe(0xf06d,f);fps=ctypes.c_uint32(f.fpscr);result=ctypes.c_uint32(raw)
  actual=LIB.sa_sh4_sqrt(raw,ctypes.byref(fps),ctypes.byref(result))
  assert actual==rc and fps.value==want.fpscr and result.value==want.banks[1][0],(hex(raw),'independent core result/status')
  count+=1
 return count

def main():
 manifest_path=PROJECT/'analysis/draw_distance_helpers_translation.json';manifest=json.loads(manifest_path.read_text())
 for p,h in manifest['sha256'].items():assert digest(p)==h,p
 binary=morph.MAIN.read_bytes();ram=bytearray(16<<20);ram[0x10000:0x10000+len(binary)]=binary
 selected=list(cases());records=[];coverage=set();edges=set();boundaries=0
 if '--smoke' in sys.argv:selected=[dict(semantic=True),dict(a=(0,0,0),b=(3,4,0),semantic=True),dict(entry=SQRT,raw=0xc0800000,semantic=True),dict(entry=SQRT,raw=0x80000000,semantic=True),dict(entry=SQRT,raw=0x7fc00001,probe=True),dict(mode=1)]
 for n,c in enumerate(selected):
  m,s,f=prepare(ram,c);record,path,b=compare(m,s,f,c);records.append(record);coverage.update(path);boundaries+=b
  for i,a in enumerate(path[:-1]):
   w=int.from_bytes(binary[a-0x8c010000:a-0x8c010000+2],'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f):
    j=i+(2 if w>>8 in (0x8d,0x8f) else 1)
    if j<len(path):edges.add((a,path[j]))
  if (n+1)%256==0:print(n+1,'distance/signed-sqrt original comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Distance/signed-sqrt smoke passed:',len(records));return
 wanted={tuple(int(a,16) for a in e) for r in manifest['routines'] for e in r['conditional_edges']};assert coverage==set(ROWS) and edges==wanted,(set(ROWS)-coverage,wanted-edges)
 core=core_checks();files=[Path(__file__),manifest_path,ORACLE_FILE,PROJECT/'source/sa_draw_distance_helpers_connected.c',PROJECT/'build_draw_distance_helpers.ps1',PROJECT/'verify_overlay_model_blend.py',PROJECT/'sh4_binary32.py']+[Path(p) for p in manifest['sha256']]
 baseline=PROJECT/'analysis/overlay_model_blend_verification.json';files.append(baseline);files.extend(Path(p) for p in json.loads(baseline.read_text())['sha256'])
 report=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),rows_covered=len(coverage),conditional_edges_covered=len(edges),boundary_comparisons=boundaries,independent_cases=sum(r['independent_checks'] for r in records),independent_sqrt_core_checks=core,external_game_service_fixtures=0,newly_registered_entries=2,new_distance_instruction_rows=32,completed_existing_sqrt_rows=10,prior_limited_sqrt_credit=0,dll_sha256=digest(DLL),sha256={str(p):digest(p) for p in files},scope='Two complete guarded native original bodies with unchanged native integer FSQRT: full RAM/CPU/FPU/FPUL/MACL/stop state and ordered entries. Original FSQRT numerical results use a separate rational squared-bracket oracle; independent rounded distance and signed-root checks.',limitations=['The unchanged FSQRT backend supports normal inputs, signed zero and positive infinity, with its previously qualified trap/precision/RM policy. Exact mathematical checks do not independently validate physical hardware exception policy. Unsupported operands/modes stop with reached state.','Finite independent distance checks preserve separately rounded subtractions, Y square, X fused square-add, Z fused square-add and final square-root rounding. No universal memory alignment/fault or hardware equivalence claim.','Existing limited signed-sqrt source/report preserved with zero duplicate credit; AL_MAIN caller integration is checked separately.'],cases=records)
 (PROJECT/'analysis/draw_distance_helpers_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','rows_covered','conditional_edges_covered','boundary_comparisons','independent_cases','independent_sqrt_core_checks')},indent=2))
if __name__=='__main__':main()
