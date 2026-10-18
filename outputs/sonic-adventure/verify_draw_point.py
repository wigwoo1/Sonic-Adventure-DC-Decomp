"""Original point transform, complete reached machine state and exact affine oracles."""
import collections,csv,ctypes,hashlib,itertools,json,sys
from fractions import Fraction
from pathlib import Path
import verify_draw_hierarchy as previous
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent;ENTRY=0x8c638ecc
MANIFEST=PROJECT/'analysis/draw_point_translation.json';manifest=json.loads(MANIFEST.read_text())
OWN={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(manifest['routines'][0]['listing']).open(),delimiter='\t')}
ROWS=dict(previous.ROWS,**{});ROWS.update(OWN);CONNECTED=previous.CONNECTED|{ENTRY};RETURNS=previous.RETURNS
namespace=dict(previous.namespace,ROWS=ROWS,RETURNS=RETURNS,LEGACY=previous.namespace['LEGACY']-{ENTRY})
code=previous.code;exec(compile(code,'draw_point_reference','exec'),namespace);reference_run=namespace['reference_run']
DLL=previous.seed.ROOT/'work/sonic-adventure/build/draw_point.dll';LIB=ctypes.CDLL(str(DLL));fn=LIB.sa_draw_point_connected;fn.argtypes=previous.fn.argtypes;fn.restype=ctypes.c_int
LIB.sa_vertex_bus_init.argtypes=previous.LIB.sa_vertex_bus_init.argtypes
State,Fpu,OBSERVE,Memory,Bus,QueueReference=previous.State,previous.Fpu,previous.OBSERVE,previous.Memory,previous.Bus,previous.QueueReference
RAM_TYPE,ram_digest,bits,value=previous.RAM_TYPE,previous.ram_digest,previous.bits,previous.value
MATRIX,POINT,OUTPUT=0x8cc50000,0x8cc60000,0x8cc70000

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def matrix(kind):
 if kind==0:return [Fraction(i%5==0) for i in range(16)]
 if kind==1:return list(map(Fraction,(1,.5,-.25,0,-.5,2,.25,0,.25,-.5,1,0,3,-2,1,1)))
 if kind==2:return list(map(Fraction,(-2,.25,.5,.5,.5,-1,.25,-.5,-.25,.5,2,.25,-1,2,-3,2)))
 return [Fraction(0)]*16

def prepare(ram,c):
 mode=c.get('mode',0);m,s,f=previous.seed.prepare(ram,dict(entry=ENTRY,mode=mode,rm=c.get('rm',0),enable=c.get('enable',0)))
 s.pc=ENTRY;alias=c.get('alias',0x8c000000);s.r[4]=(alias|(MATRIX&0xffffff)) if c.get('explicit') else 0;s.r[5]=alias|(POINT&0xffffff);s.r[6]=alias|(OUTPUT&0xffffff)
 mat=matrix(c.get('matrix',1));f.banks[(f.fpscr>>21&1)^1][:]=[bits(x) for x in mat]
 for i,x in enumerate(mat):store(m,MATRIX+i*4,bits(x))
 point=((0,0,0),(1,2,3),(-1,.5,2),(.25,-2,4))[c.get('point',1)]
 for i,x in enumerate(point):store(m,POINT+i*4,bits(Fraction(x)))
 for i in range(4):store(m,OUTPUT+i*4,0xa5000000+i)
 if 'overlap' in c:s.r[6]=alias|(((MATRIX if c.get('overlap_matrix') else POINT)+c['overlap'])&0xffffff)
 for reg,key in ((4,'bad_matrix'),(5,'bad_point'),(6,'bad_output')):
  if key in c:s.r[reg]=c[key]
 if 'special' in c:store(m,POINT,c['special'])
 if 'special_matrix' in c:store(m,MATRIX,c['special_matrix']);f.banks[(f.fpscr>>21&1)^1][0]=c['special_matrix']
 f.fpscr|=c.get('fpscr_bits',0)
 return m,s,f

def exact_oracle(m,s,f):
 result=bytearray(m);v=[Fraction(value(load(m,s.r[5]+i*4))) for i in range(3)]+[Fraction(1)]
 def dot(row):return bits(sum((Fraction(value(x))*y for x,y in zip(row,v)),Fraction(0)))
 def row(i):return [load(result,s.r[4]+j*16+i*4) for j in range(4)]
 if s.r[4]:
  x=dot(row(0));second=row(1);store(result,s.r[6],x);y=dot(second);third=row(2);store(result,s.r[6]+4,y);z=dot(third);store(result,s.r[6]+8,z)
  return result,[x,y,z]
 xf=f.banks[(f.fpscr>>21&1)^1];out=[dot([xf[j*4+i] for j in range(4)]) for i in range(4)]
 for i in (2,1,0):store(result,s.r[6]+i*4,out[i])
 return result,out

def compare(m,s,f,c):
 initial_s=State.from_buffer_copy(s);initial_f=Fpu.from_buffer_copy(f);other=State.from_buffer_copy(s);ef=Fpu.from_buffer_copy(f);actual=bytearray(m);expected=bytearray(m)
 ref=QueueReference();ref.ram=expected;namespace['queue_reference']=ref
 status,path,wanted,expected_macl=reference_run(expected,other,ef,c,tracked=tuple(CONNECTED));native=Memory();bus=Bus();view=RAM_TYPE.from_buffer(actual);LIB.sa_vertex_bus_init(ctypes.byref(native),view,ctypes.byref(bus));ml=ctypes.c_uint32(0x12345678);got=[]
 @OBSERVE
 def observe(ctx,target,mem,fp,sr,r,pr):got.append((target,tuple(r[:16]),sr[0],pr[0],bytes(fp.contents),ml.value,ram_digest(mem)))
 rc=fn(view,ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,Fpu.fpul.offset),previous.seed.state.gate.support.morph.P),ctypes.byref(ml),ENTRY,None,observe,ctypes.byref(bus))
 assert rc==status and got==wanted and actual==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef) and ml.value==expected_macl,(c,'full original point-transform state',rc,status,hex(s.pc),hex(other.pc))
 assert not native.trace_count and not ref.events,(c,'point transform must not fabricate packets')
 independent=False
 if c.get('semantic') and rc==0 and not initial_f.fpscr&0x180000:
  oracle,out=exact_oracle(m,initial_s,initial_f);assert actual==oracle,(c,'independent output/read-before-write overlap order')
  assert s.r[5]==initial_s.r[5]+12 and s.r[6]==initial_s.r[6]+(8 if c.get('explicit') else 0) and s.sr&1==int(not c.get('explicit')),(c,'independent pointer/T result')
  if not c.get('explicit'):assert list(f.banks[f.fpscr>>21&1][:4])==out,(c,'independent homogeneous vector, no perspective division')
  independent=True
 return dict(input=c,status=rc,full_state_match=True,ordered_boundaries_match=True,queue_match=True,independent_checks=independent,stop_pc=hex(s.pc),steps=len(path)),path,len(got)

def cases():
 for mode,explicit,kind,point in itertools.product(range(8),(False,True),range(4),range(4)):yield dict(mode=mode,explicit=explicit,matrix=kind,point=point,semantic=True)
 for bank,explicit,overlap_matrix,offset in itertools.product(range(2),(False,True),(False,True),(-4,0,4,8,12,16,32,48,60)):
  if not explicit and overlap_matrix:continue
  yield dict(mode=bank*4,explicit=explicit,overlap_matrix=overlap_matrix,overlap=offset,semantic=True)
 for explicit,alias in itertools.product((False,True),(0x0c000000,0x8c000000,0xac000000)):yield dict(explicit=explicit,alias=alias,semantic=True)
 for explicit,key,address in itertools.product((False,True),('bad_matrix','bad_point','bad_output'),(0,0x8cfffff4,0x8cfffff8,0x8cfffffc,0x8d000000)):
  if key=='bad_matrix' and not explicit:continue
  yield dict(explicit=explicit,**{key:address})
 for explicit,bank,word,dn in itertools.product((False,True),range(2),(0x80000000,1,0x80000001,0x007fffff,0x00800000,0x7f7fffff,0x7f800000,0x7fc12345),(0,0x40000)):
  yield dict(explicit=explicit,mode=bank*4,special=word,fpscr_bits=dn)
 for explicit,word in itertools.product((False,True),(0x7f800000,0xff800000,0x7fc12345)):yield dict(explicit=explicit,special_matrix=word)
 for explicit,rm,enable in itertools.product((False,True),range(4),(0,0x80,0x800)):yield dict(explicit=explicit,rm=rm,enable=enable,point=2,matrix=2)

def main():
 proofs=[MANIFEST,PROJECT/'analysis/draw_point_connection.json',PROJECT/'analysis/draw_hierarchy_verification.json']
 for proof in proofs:
  for p,h in json.loads(proof.read_text())['sha256'].items():assert digest(p)==h,p
 binary=previous.seed.state.gate.support.morph.MAIN.read_bytes();ram=bytearray(16<<20);ram[0x10000:0x10000+len(binary)]=binary
 selected=list(cases());records=[];coverage=set();edges=set();boundaries=0
 if '--smoke' in sys.argv:selected=[dict(explicit=False,semantic=True),dict(explicit=True,semantic=True),dict(explicit=True,overlap_matrix=True,overlap=8,semantic=True),dict(mode=2,explicit=True),dict(explicit=False,bad_output=0x8cfffffc)]
 for n,c in enumerate(selected):
  record,path,b=compare(*prepare(ram,c),c);records.append(record);coverage.update(path);boundaries+=b
  if ENTRY+8 in path:
   i=path.index(ENTRY+8)
   if i+2<len(path):edges.add((ENTRY+8,path[i+2]))
  if (n+1)%128==0:print(n+1,'original point-transform comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Point-transform smoke passed:',len(records));return
 assert set(OWN)<=coverage
 wanted={tuple(int(a,16) for a in e) for e in manifest['routines'][0]['conditional_edges']};assert edges==wanted,(edges,wanted)
 files=[Path(__file__),PROJECT/'verify_draw_hierarchy.py']+proofs
 for proof in proofs:files.extend(Path(p) for p in json.loads(proof.read_text())['sha256'])
 report=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),rows_covered=len(set(OWN)&coverage),conditional_edges_covered=len(edges),boundary_comparisons=boundaries,independent_cases=sum(r['independent_checks'] for r in records),newly_registered_entries=1,reused_routine_credit=0,external_game_service_fixtures=0,dll_sha256=digest(DLL),sha256={str(p):digest(p) for p in files},scope='Complete guarded original point transform. Original opcode reference, full reached CPU/FPU/FPUL/MACL/RAM and exact dyadic affine/homogeneous/read-before-write overlap checks. Real numerical core; no injected outputs.',limitations=['Vector opcode reference shares pinned separately qualified numerical support; exact dyadic matrix/point/overlap/pointer checks are independent. Numerical support and MMU-off valid-RAM guard limitations remain explicit.','No natural scene initialization, physical GPU pixels, whole frame/gameplay/high-FPS or playable-port proof.'],cases=records)
 (PROJECT/'analysis/draw_point_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','rows_covered','conditional_edges_covered','boundary_comparisons','independent_cases')},indent=2))
if __name__=='__main__':main()
