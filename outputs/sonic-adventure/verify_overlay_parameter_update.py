"""Full original-state and separate data-algorithm checks of two update bodies."""
import collections,csv,ctypes,hashlib,inspect,itertools,json,random,sys
from pathlib import Path
import sh4_binary32 as binary32
import verify_overlay_model_blend as morph
from verify_startup import ROOT,State
from verify_fpu_cleanup_transfer import Fpu
from verify_main_dispatch_active_query import OBSERVE
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent
UPDATE,SPRING,BLEND,COPY=0xc905666,0xc905618,morph.ENTRY,morph.COPY
ENTITY,APP,WORK=morph.ENTITY,morph.APP,0x8cc90000
DLL=ROOT/'work/sonic-adventure/build/overlay_parameter_update.dll'
LIB=ctypes.CDLL(str(DLL));fn=LIB.sa_overlay_parameter_update_connected
fn.argtypes=morph.fn.argtypes;fn.restype=ctypes.c_int
MANIFEST=PROJECT/'analysis/overlay_parameter_update_translation.json'
OWN={}
for entry in (UPDATE,SPRING):
 for row in csv.DictReader((PROJECT/f'analysis/overlay-selector-initializer/{entry:08x}_instructions.tsv').open(),delimiter='\t'):
  OWN[int(row['address'],16)]=row['instruction'].lstrip('_')
ROWS=dict(morph.ROWS,**{});ROWS.update(OWN)
namespace=dict(morph.__dict__,ROWS=ROWS)
code=inspect.getsource(morph.reference_run).replace('if pc in tracked:events.append(event(pc,r,sr,pr))','if pc in tracked:events.append(event(pc,r,sr,pr))\n  if pc==ENTRY and prefix_check is not None:prefix_check(memory)')
code=code.replace("c.get('probe') or f.fpscr&0x100000", "c.get('probe') or f.fpscr&0x180000")
namespace['prefix_check']=None
exec(compile(code,'parameter_update_reference','exec'),namespace)
reference_run=namespace['reference_run']

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def add(a,b,rm=0,sub=False):return binary32.add_sub(a,b,sub,rm)[0]
def mul(a,b,rm=0):
 x,p=binary32.decode(a);y,q=binary32.decode(b)
 return binary32.round_dyadic(x*y,p+q,rm,bool((a^b)&0x80000000))[0]

def parameter_oracle(before,entity=ENTITY,app=APP,rm=0):
 """Data algorithm, independent of instruction/register translation."""
 m=bytearray(before);changed=False;step=0x3b449ba5
 for i in range(2):
  a=app+0x20+i*4;v=morph.value(load(m,a))
  if v>1.5:store(m,a,morph.bits(1.5))
  if v<-1.5:store(m,a,morph.bits(-1.5))
  target=load(m,entity+0xdc+i*4)
  if morph.value(target)>morph.value(load(m,a)):
   store(m,a,add(load(m,a),step,rm));changed=True
  if morph.value(load(m,a))>morph.value(target):
   store(m,a,add(load(m,a),step,rm,True));changed=True
 if load(m,app+0x36,2)&1:
  store(m,app+0x36,load(m,app+0x36,2)&0xfffe,2)
  store(m,entity+0xe4,add(load(m,entity+0xe4),0x3c03126e if load(m,app,1)==0 else 0x3b83126e,rm))
  for index,(axis,negative) in enumerate(((1,True),(1,False),(0,False),(0,True))):
   current,peak=app+0xc+index*2,app+0x54+index*2
   if load(m,current,2)<load(m,peak,2):
    store(m,current,(load(m,current,2)+10)&65535,2)
    target=entity+0xdc+axis*4;store(m,target,add(load(m,target),0x3ac49ba5,rm,negative))
  for index in range(4):
   current,peak=app+0xc+index*2,app+0x54+index*2
   store(m,peak,max(load(m,current,2),load(m,peak,2)),2)
 z=entity+0xe4;stored=app+0x28
 if morph.value(load(m,z))>1.5:store(m,z,morph.bits(1.5))
 if morph.value(load(m,z))>morph.value(load(m,stored)):
  store(m,stored,add(load(m,stored),step,rm));changed=True
 elif morph.value(load(m,stored))>morph.value(load(m,z)):store(m,stored,load(m,z))
 return m,changed

def check_parameter_data(actual,oracle,entity=ENTITY,app=APP):
 assert morph.bytes_at(actual,app,0x60)==morph.bytes_at(oracle,app,0x60),'independent appearance parameters/counters'
 assert morph.bytes_at(actual,entity+0xdc,12)==morph.bytes_at(oracle,entity+0xdc,12),'independent targets'

def spring_oracle(before,entity=ENTITY,work=WORK,rm=0):
 m=bytearray(before);s=load(m,work+0x30);v=load(m,entity+0xd4)
 displacement=add(s,morph.bits(-1),rm)
 damping=mul(v,0x3e199999,rm);restoring=mul(displacement,0x3e8f5c28,rm)
 delta=add(damping^0x80000000,restoring,rm,True)
 next_v=add(v,delta,rm);next_s=add(s,next_v,rm)
 store(m,entity+0xd4,next_v);store(m,work+0x30,next_s)
 store(m,work+0x2c,add(next_s^0x80000000,morph.bits(2),rm))
 return m

def cases():
 for axis,v,target,bank,rm in itertools.product(range(3),(-2,-1.5,-.25,0,.25,1.5,2),(-2,-.25,0,.251,1.5,2),range(2),range(2)):
  xyz=[.25,.75,.5];targets=xyz[:];xyz[axis]=v;targets[axis]=target
  yield dict(xyz=xyz,targets=targets,mode=bank*4,rm=rm,semantic=True)
 for pattern,t,bank,rm in itertools.product(range(16),(0,1,6,255),range(2),range(2)):
  counters=[(100,105) if pattern>>i&1 else (120,105) for i in range(4)]
  yield dict(flags=0xa003,type=t,counters=counters,mode=bank*4,rm=rm,semantic=True)
 for current,peak,bank in itertools.product((0,1,32767,32768,65526,65530,65535),(0,5,32767,32768,65535),range(2)):
  yield dict(flags=1,counters=[(current,peak)]*4,mode=bank*4,semantic=True)
 for mode in range(8):yield dict(mode=mode)
 for entry,mode,rm,enable in itertools.product((UPDATE,SPRING),(0,2,4,6),range(4),(0x80,0x800)):
  yield dict(entry=entry,mode=mode,rm=rm,enable=enable,probe=True,targets=(.4,.8,.75))
 for entry,kind,mode in itertools.product((UPDATE,SPRING),('entity','work','stack'),(0,4)):
  yield dict(entry=entry,bad=kind,mode=mode)
 for raw,bank in itertools.product((1,0x7f61b1e6,0x7fc00001),range(2)):
  yield dict(xyz_raw=(raw,morph.bits(.75),morph.bits(.5)),mode=bank*4,dn=0,probe=True)
 for s,v,bank,rm in itertools.product((-.5,0,.75,1,1.5,2),(-.25,0,.25),range(2),range(2)):
  yield dict(entry=SPRING,scale=s,velocity=v,mode=bank*4,rm=rm,semantic=True)
 for mode in range(8):yield dict(entry=SPRING,mode=mode)

def compare(m,s,f,c,semantic=False,entity=ENTITY,app=APP,work=WORK):
 entry=s.pc;oracle=None;should_blend=None;prefix_checks=[]
 if semantic:
  if entry==UPDATE:
   oracle,should_blend=parameter_oracle(m,entity,app,c.get('rm',0))
  else:oracle=spring_oracle(m,entity,work,c.get('rm',0))
 def check(mem):
  if semantic and entry==UPDATE:check_parameter_data(mem,oracle,entity,app);prefix_checks.append(True)
 namespace['prefix_check']=check if semantic and entry==UPDATE else None
 other=State.from_buffer_copy(s);ef=Fpu.from_buffer_copy(f);actual=bytearray(m);expected=bytearray(m)
 status,path,want,expected_macl=reference_run(expected,other,ef,c,tracked=(UPDATE,SPRING,BLEND,COPY))
 ml=ctypes.c_uint32(c.get('macl',0x12345678));got=[];errors=[]
 @OBSERVE
 def observe(ctx,target,mem,fp,sr,r,pr):
  try:
   got.append((target,tuple(r[:16]),sr[0],pr[0],bytes(fp.contents),ml.value,hashlib.sha256(ctypes.string_at(ctypes.addressof(mem.contents),16<<20)).digest()))
   if target==BLEND:check(mem)
  except Exception as exc:errors.append(repr(exc))
 rc=fn((ctypes.c_uint8*len(actual)).from_buffer(actual),ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,Fpu.fpul.offset),morph.P),ctypes.byref(ml),entry,None,observe)
 assert not errors and rc==status and got==want,(c,'status/boundaries',rc,status,errors,len(got),len(want))
 assert actual==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef) and ml.value==expected_macl,(c,'full RAM/CPU/FPU/FPUL/MACL',hex(s.pc),hex(other.pc))
 if semantic and rc==0:
  if entry==UPDATE:
   assert bool(prefix_checks)==should_blend,(c,'independent morph request',should_blend,prefix_checks)
   if not should_blend:check_parameter_data(actual,oracle,entity,app)
  else:
   assert morph.bytes_at(actual,entity+0xd4,4)==morph.bytes_at(oracle,entity+0xd4,4)
   assert morph.bytes_at(actual,work+0x2c,8)==morph.bytes_at(oracle,work+0x2c,8)
 return actual,dict(input=c,status=rc,steps=len(path),stop_pc=hex(s.pc),full_state_match=True,ordered_boundaries_match=True,independent_checks=bool(semantic and rc==0)),path,len(got)

def main():
 manifest=json.loads(MANIFEST.read_text());baseline_path=PROJECT/'analysis/overlay_model_blend_verification.json';baseline=json.loads(baseline_path.read_text())
 for item in (manifest,baseline):
  for p,h in item['sha256'].items():assert digest(p)==h,p
 ram=bytearray(16<<20);mainbytes=morph.MAIN.read_bytes();binary=morph.MODULE.read_bytes()
 ram[0x10000:0x10000+len(mainbytes)]=mainbytes;ram[0x900000:0x900000+len(binary)]=binary
 rng=random.Random(UPDATE);records=[];coverage=set();edges=set();boundaries=0;selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(semantic=True),dict(semantic=True,targets=(.4,.6,.8)),dict(semantic=True,flags=1,counters=[(100,105)]*4),dict(entry=SPRING,semantic=True,scale=1.5,velocity=.25),dict(mode=1),dict(mode=2)]
 for number,c in enumerate(selected):
  entry=c.get('entry',UPDATE);m=morph.fixture(ram,c)
  xyz=c.get('xyz',(.25,.75,.5))
  for i,v in enumerate(c.get('targets',xyz)):store(m,ENTITY+0xdc+i*4,morph.bits(v))
  for i,(current,peak) in enumerate(c.get('counters',[(100,100)]*4)):
   store(m,APP+0xc+i*2,current,2);store(m,APP+0x54+i*2,peak,2)
  store(m,WORK+0x30,morph.bits(c.get('scale',1.5)));store(m,ENTITY+0xd4,morph.bits(c.get('velocity',.125)))
  s=State();s.r[:]=[rng.getrandbits(32) for _ in range(16)];s.r[4]=WORK;s.r[5]=ENTITY;s.r[15]=0x8cf00000;s.pr=0x8cfed000;s.pc=entry;s.sr=rng.getrandbits(32)
  if c.get('bad')=='entity':s.r[5]=0
  if c.get('bad')=='work':s.r[4]=0
  if c.get('bad')=='stack':s.r[15]=0
  f=Fpu();f.fpscr=c.get('dn',0x40000)|(c.get('mode',0)<<19)|c.get('rm',0)|c.get('enable',0);f.fpul=rng.getrandbits(32)
  for bank in range(2):
   for i in range(16):f.banks[bank][i]=morph.bits((i+1)/8)
  _,record,path,n=compare(m,s,f,c,c.get('semantic',False));records.append(record);coverage.update(path);boundaries+=n
  for i,a in enumerate(path[:-1]):
   if a not in OWN:continue
   w=int.from_bytes(binary[a-0xc900000:a-0xc900000+2],'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f):
    j=i+(2 if w>>8 in (0x8d,0x8f) else 1)
    if j<len(path):edges.add((a,path[j]))
  if (number+1)%256==0:print(number+1,'parameter/scale comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Parameter/scale smoke passed:',len(records));return
 wanted={tuple(int(a,16) for a in e) for r in manifest['routines'] for e in r['conditional_edges']}
 assert set(OWN)<=coverage and wanted==edges,(list(map(hex,set(OWN)-coverage)),wanted-edges)
 files=[Path(__file__),MANIFEST,baseline_path,PROJECT/'source/sa_overlay_parameter_update_connected.c',PROJECT/'build_overlay_parameter_update.ps1',PROJECT/'verify_overlay_model_blend.py',PROJECT/'sh4_binary32.py']+[Path(p) for item in (manifest,baseline) for p in item['sha256']]
 report=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),rows_covered=len(set(OWN)&coverage),conditional_edges_covered=len(edges),boundary_comparisons=boundaries,independent_cases=sum(r['independent_checks'] for r in records),external_game_service_fixtures=0,newly_recovered_entries=2,reused_morph_copy_credit=0,dll_sha256=digest(DLL),sha256={str(p):digest(p) for p in files},scope='Two original guarded update/scale bodies connected natively to existing morph/copy: full 16MiB RAM, registers/SR/PR/PC, both FPU banks/FPSCR/FPUL/MACL and ordered entries. Independent data algorithms validate narrow finite updates and scale smoothing.',limitations=['Constructed records; natural scheduling and drawing are separate unfinished dependencies.','Wide transfers/PR modes, enabled/reserved arithmetic and invalid RAM are checked as reached partial-state guards, not hardware exception delivery.','Independent update checks stop at the morph boundary; morph output is covered by full original-state comparisons and its earlier independent proof.'],cases=records)
 (PROJECT/'analysis/overlay_parameter_update_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','rows_covered','conditional_edges_covered','boundary_comparisons','independent_cases')},indent=2))
if __name__=='__main__':main()
