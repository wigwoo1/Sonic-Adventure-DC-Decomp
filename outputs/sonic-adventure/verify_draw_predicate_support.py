"""Original predicate/state-copy comparisons and independent field algorithms."""
import collections,csv,ctypes,hashlib,itertools,json,random,sys
from pathlib import Path
import sh4_binary32 as arithmetic
import verify_draw_matrix_registers as matrix
import verify_overlay_model_blend as morph
from verify_startup import ROOT,State
from verify_fpu_cleanup_transfer import Fpu
from verify_main_dispatch_active_query import OBSERVE
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent
PRED,WRAPPER,COPY=0x8c038d00,0x8c043a20,0x8c60edf0
DEST=0x8c88f668
DLL=ROOT/'work/sonic-adventure/build/draw_predicate_support.dll';LIB=ctypes.CDLL(str(DLL));fn=LIB.sa_draw_predicate_support_connected
fn.argtypes=morph.fn.argtypes;fn.restype=ctypes.c_int
ROWS={}
for entry in (PRED,WRAPPER,COPY):
 for row in csv.DictReader((PROJECT/f'analysis/draw-predicate-support/{entry:08x}_instructions.tsv').open(),delimiter='\t'):
  ROWS[int(row['address'],16)]=row['instruction'].lstrip('_')
namespace=dict(matrix.namespace,ROWS=ROWS)
code=matrix.code.replace('pr!=entry_pr','pr not in (entry_pr,0x8c043a38)')
exec(compile(code,'draw_predicate_reference','exec'),namespace)
reference_run=namespace['reference_run']
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def predicate_oracle(c):
 if c.get('disabled',0):return 0
 rm=c.get('rm',0);add=lambda a,b,sub=False:arithmetic.add_sub(a,b,sub,rm)[0]
 mul=lambda a,b:arithmetic.multiply(a,b,rm)[0]
 raw=lambda v:morph.bits(v);val=morph.value
 x,y,z=map(raw,c.get('point',(0,0,4)));r=raw(c.get('radius',.25));near,far=map(raw,c.get('depth',(1,10)))
 # The focused semantic domain uses identity XMTRX; ordinary binary32 scalar
 # operations below are rounded separately and are independent of C dataflow.
 if not val(z)>val(add(near,r,True)):return 0
 if val(z)>val(add(far,r)):return 0
 inv=arithmetic.divide(raw(1),add(z,r),rm)[0]
 p,ox,oy=map(raw,c.get('projection',(2,3,4)));sx,sy=map(raw,c.get('scale',(2,3)))
 b0,b1,b2,b3=map(raw,c.get('bounds',(-10,-10,10,10)))
 gx=mul(mul(sx,p),inv)
 if not val(add(mul(add(x,r),gx),ox))>val(b0):return 0
 if val(add(mul(add(x,r,True),gx),ox))>val(b2):return 0
 gy=mul(mul(sy,ox),inv)
 if not val(add(mul(add(y,r),gy),oy))>val(b1):return 0
 # Original final test reuses horizontal gain and B2. B3 was loaded but unused.
 if val(add(mul(add(y,r,True),gx),oy))>val(b2):return 0
 return 1

def cases():
 for axis,v,radius,bank,rm in itertools.product(range(3),(-100,-10,-2,0,2,10,100),(0,.25,1),range(2),range(2)):
  point=[0,0,4];point[axis]=v;yield dict(point=point,radius=radius,bank=bank,rm=rm,semantic=True)
 for x,y,z,bank in itertools.product((-4,-1,0,1,4),(-4,-1,0,1,4),(.5,1,4,12),range(2)):
  yield dict(point=(x,y,z),bank=bank,projection=(1,2,3),scale=(1,4),bounds=(-2,-3,4,999),semantic=True)
 for mode,value in itertools.product(range(8),(1,0xffffffff)):yield dict(mode=mode,disabled=value,semantic=True)
 for mode,rm,enable in itertools.product(range(8),range(4),(0x80,0x800)):
  yield dict(mode=mode,rm=rm,enable=enable,probe=True)
 for entry,mode,offset in itertools.product((WRAPPER,COPY),range(8),(-8,0,4,8,32,0x100)):
  yield dict(entry=entry,mode=mode,offset=offset,copy_semantic=not mode&2)
 for entry,mode,address in itertools.product((WRAPPER,COPY),range(8),(0,0x8cfffffc,0x8cfffff8)):
  yield dict(entry=entry,mode=mode,bad=address)

def prepare(ram,c):
 rng=random.Random(c.get('seed',123));m=bytearray(ram);s=State();s.r[:]=[rng.getrandbits(32) for _ in range(16)];s.r[4]=DEST+c.get('offset',0x100);s.r[15]=0x8cf00000;s.pr=0x8cfed000;s.pc=c.get('entry',PRED);s.sr=rng.getrandbits(32)
 f=Fpu();f.fpscr=0x40000|(c.get('mode',c.get('bank',0)*4)<<19)|c.get('rm',0)|c.get('enable',0);f.fpul=rng.getrandbits(32)
 for bank in range(2):
  for i in range(16):f.banks[bank][i]=morph.bits((i+1)/8)
 active=f.fpscr>>21&1;f.banks[active^1][:]=[morph.bits(1 if i%5==0 else 0) for i in range(16)]
 for i,v in enumerate((*c.get('point',(0,0,4)),c.get('radius',.25))):f.banks[active][4+i]=morph.bits(v)
 store(m,0x8c754ec8,c.get('disabled',0));store(m,0x8c88f610,morph.bits(c.get('depth',(1,10))[0]));store(m,0x8c88f614,morph.bits(c.get('depth',(1,10))[1]))
 projection=c.get('projection',(2,3,4))
 for off,v in ((0,projection[0]),(12,projection[1]),(16,projection[2])):store(m,0x8c88f5e4+off,morph.bits(v))
 for i,v in enumerate(c.get('scale',(2,3))):store(m,0x8c88f618+i*4,morph.bits(v))
 for i,v in enumerate(c.get('bounds',(-10,-10,10,10))):store(m,0x8c88f600+i*4,morph.bits(v))
 for a in range(DEST-16,DEST+0x140,4):store(m,a,rng.getrandbits(32))
 if 'bad' in c:
  if s.pc==WRAPPER:s.r[15]=c['bad']
  else:s.r[4]=c['bad']
 return m,s,f

def compare(m,s,f,c):
 entry=s.pc;original=State.from_buffer_copy(s);before_f=Fpu.from_buffer_copy(f);other=State.from_buffer_copy(s);ef=Fpu.from_buffer_copy(f);actual=bytearray(m);expected=bytearray(m)
 status,path,wanted,expected_macl=reference_run(expected,other,ef,c,tracked=(PRED,WRAPPER,COPY));got=[];ml=ctypes.c_uint32(0x12345678)
 @OBSERVE
 def observe(ctx,target,mem,fp,sr,r,pr):got.append((target,tuple(r[:16]),sr[0],pr[0],bytes(fp.contents),ml.value,hashlib.sha256(ctypes.string_at(ctypes.addressof(mem.contents),16<<20)).digest()))
 rc=fn((ctypes.c_uint8*len(actual)).from_buffer(actual),ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,Fpu.fpul.offset),morph.P),ctypes.byref(ml),entry,None,observe)
 assert rc==status and got==wanted and actual==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef) and ml.value==expected_macl,(c,'full state/boundaries',rc,status,hex(s.pc),hex(other.pc))
 semantic=False
 if rc==0 and entry==PRED and c.get('semantic'):
  assert s.r[0]==predicate_oracle(c),(c,'independent predicate',s.r[0],predicate_oracle(c));assert actual==m;semantic=True
 if rc==0 and c.get('copy_semantic'):
  oracle=bytearray(m)
  if entry==WRAPPER:
   sp=original.r[15]-20;store(oracle,original.r[15]-4,original.pr)
   for i in range(4):store(oracle,sp+i*4,before_f.banks[before_f.fpscr>>21&1][4+i])
   src=sp
  else:src=original.r[4]
  for i in range(4):store(oracle,DEST+i*4,load(oracle,src+i*4))
  assert actual==oracle,(c,'independent sequential field copy');semantic=True
 return dict(input=c,status=rc,full_state_match=True,ordered_boundaries_match=True,independent_checks=semantic,steps=len(path),stop_pc=hex(s.pc)),path,len(got)

def main():
 manifest_path=PROJECT/'analysis/draw_predicate_support_translation.json';manifest=json.loads(manifest_path.read_text());proof=PROJECT/'analysis/draw_matrix_registers_verification.json';old=json.loads(proof.read_text())
 for item in (manifest,old):
  for p,h in item['sha256'].items():assert digest(p)==h,p
 binary=morph.MAIN.read_bytes();ram=bytearray(16<<20);ram[0x10000:0x10000+len(binary)]=binary
 assert load(ram,0x8c60ee40)==DEST
 records=[];coverage=set();edges=set();boundaries=0;selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(semantic=True),dict(point=(100,0,4),semantic=True),dict(disabled=1,semantic=True),dict(entry=WRAPPER,copy_semantic=True),dict(entry=COPY,offset=4,copy_semantic=True),dict(entry=WRAPPER,mode=2)]
 for n,c in enumerate(selected):
  m,s,f=prepare(ram,c);record,path,count=compare(m,s,f,c);records.append(record);coverage.update(path);boundaries+=count
  for i,a in enumerate(path[:-1]):
   w=int.from_bytes(binary[a-0x8c010000:a-0x8c010000+2],'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f):
    j=i+(2 if w>>8 in (0x8d,0x8f) else 1)
    if j<len(path):edges.add((a,path[j]))
  if (n+1)%256==0:print(n+1,'draw predicate/float-state comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Draw predicate/float support smoke passed:',len(records));return
 wanted={tuple(int(a,16) for a in e) for r in manifest['routines'] for e in r['conditional_edges']}
 assert coverage==set(ROWS) and edges==wanted,(set(ROWS)-coverage,wanted-edges)
 files=[Path(__file__),manifest_path,proof,PROJECT/'source/sa_draw_predicate_support_connected.c',PROJECT/'build_draw_predicate_support.ps1',PROJECT/'verify_draw_matrix_registers.py']+[Path(p) for item in (manifest,old) for p in item['sha256']]
 report=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),rows_covered=len(coverage),conditional_edges_covered=len(edges),independent_cases=sum(r['independent_checks'] for r in records),boundary_comparisons=boundaries,newly_recovered_entries=3,external_game_service_fixtures=0,dll_sha256=digest(DLL),sha256={str(p):digest(p) for p in files},scope='Three complete guarded native bodies, full original RAM/CPU/FPU/FPUL/MACL/stop state and ordered calls. Independent identity-transform predicate/scalar evaluation and sequential raw-float state/overlap writes.',limitations=['Predicate independent checks use identity XMTRX and finite masked narrow arithmetic; vector arithmetic differential checks share the pinned backend core. No physical hardware numerical or pixel proof.','PR/SZ/RM/trap/RAM guards publish reached state rather than delivering CPU exceptions. Wide stack stores can overwrite saved PR and produce an expected return guard.','Actual AL_MAIN draw caller integration is separately scoped. Names, clipping purposes and field meanings remain inferred.'],cases=records)
 (PROJECT/'analysis/draw_predicate_support_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','rows_covered','conditional_edges_covered','independent_cases','boundary_comparisons')},indent=2))
if __name__=='__main__':main()
