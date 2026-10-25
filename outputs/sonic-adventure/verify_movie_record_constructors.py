"""Original initializer records, request pools and actual driver-file loader."""
import collections,csv,ctypes,hashlib,itertools,json,sys,types
from pathlib import Path
import verify_movie_init_records as old
import console_service_reference as console_model
import verify_boot_device as seed
P=old.P;ROOT=old.ROOT;load=old.load;store=old.store
ENTRY,LOAD,POOL,ALLOC,ERROR,SET,CLEAR=(0x8c092fa0,0x8c093e60,0x8c65aa30,0x8c643ac0,0x8c65acac,0x8c5ff2cc,0x8c5ff2de)
CTX,OUT=0x8cd80000,0x8cda0000
MP=P/'analysis/movie_record_constructors_translation.json';manifest=json.loads(MP.read_text());NEW={int(r['address'],16) for r in manifest['routines']}
ROWS=dict(old.ROWS);PER_BODY=dict(old.PER_BODY)
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
namespace=dict(old.namespace,ROWS=ROWS,RETURNS=old.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=old.namespace['LEGACY']-NEW)
code=old.code
marker='if pc in tracked and not (pc==0x8c6453cc and len(path)>1 and path[-2] in COMPARISON_ROWS):'
assert code.count(marker)==1
# Overlapping copy entry labels are not calls when execution falls through.
code=code.replace(marker,'if pc in tracked and not (len(path)>1 and ((pc==0x8c6453cc and path[-2] in COMPARISON_ROWS) or (pc in (0x8c10cd7a,0x8c10cd8e) and path[-2]==pc-2))):')
exec(compile(code,'init_records_original_model','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')
effects=types.SimpleNamespace(**old.effects.__dict__);effects.CONNECTED=old.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=reference_run
effects.DLL=ROOT/'work/sonic-adventure/build/movie_record_constructors.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));console_model.declare(effects.LIB)
observed=effects.LIB.sa_movie_record_constructors_observed;observed.argtypes=old.observed.argtypes;observed.restype=ctypes.c_int
for name in ('sa_driver_signal_bus_init','sa_driver_signal_set_samples','sa_hardware_transfer_set_samples','sa_display_timing_set_samples','sa_disc_queue_init','sa_rtc_sample_bus_init','sa_rtc_set_samples'):
 getattr(effects.LIB,name).argtypes=getattr(old.effects.LIB,name).argtypes;getattr(effects.LIB,name).restype=getattr(old.effects.LIB,name).restype
effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32;native_stats=old.native_stats.__class__()
def invoke(*args):return observed(*args,ctypes.byref(effects.console_state),ctypes.byref(native_stats),0xffffffff)
effects.fn=invoke
compare_code=old.compare_code
exec(compile(compare_code,'init_records_comparison','exec'),dict(old.compare.__globals__,effects=effects,namespace=namespace,native_stats=native_stats),local:={});compare=local['compare']

ROOT_WORD=0x8c67c1d0
A,B,C,D,E,F=(0x8c666e50,0x8c666ef0,0x8c666b34,0x8c6669c4,0x8c667240,0x8c10ec40)
DATA,DB=0x8cda0000,0x8cdb0000

def seed_ram():return seed.seed_ram()
def prepare(ram,c):
 m,s,f=old.prepare(ram,c);entry=c['entry'];s.r[4]=c.get('data',DATA);s.r[5]=c.get('count',2)&0xffffffff
 store(m,ROOT_WORD,CTX if c.get('enabled',True) else 0)
 m[CTX&0xffffff:(CTX&0xffffff)+0x240]=bytes([0xa5])*0x240
 m[DATA&0xffffff:(DATA&0xffffff)+0x1000]=bytes([0x6d])*0x1000
 for literal,value in ((0x8c65acb8,0x12345678),(0x8c65acbc,0x87654321)):store(m,load(m,literal),value)
 if entry==F:
  store(m,0x8c5fbb14,DB);m[DB&0xffffff:(DB&0xffffff)+0x400]=bytes([0x5a])*0x400
  for i in range(8):store(m,DB+4+i*52,i);store(m,DB+4+i*52+40,0)
  index=c.get('index',1);s.r[5]=index;store(m,DB+4+index*52,c.get('next',3));store(m,DB+4+index*52+40,c.get('busy',0))
 if c.get('bad_stack'):s.r[15]=0x8c000000
 return m,s,f

def cases():
 base=next(old.cases())
 for entry,mode in itertools.product((A,B,D,E,F),(0,1,4)):yield base|dict(entry=entry,mode=mode)
 for entry in (A,B,D):
  for count in (0,1,4,64):yield base|dict(entry=entry,count=count)
  yield base|dict(entry=entry,enabled=False)
  yield base|dict(entry=entry,bad_stack=True)
 for entry in (A,B):
  for count in (0x80000000,0xffffffff):yield base|dict(entry=entry,count=count)
 for busy,index,nxt in itertools.product((0,1,2),(0,1,3),(0,3)):
  yield base|dict(entry=F,busy=busy,index=index,next=nxt)
 for entry in (A,B,D,F):
  for data in (0x0cda0000,0xacda0000):yield base|dict(entry=entry,data=data)
 for entry in (C,E):yield base|dict(entry=entry,enabled=False)
 yield base|dict(entry=C)
 yield base|dict(entry=0x8c65acc0)

def semantics(m,a,before,s,r,c):
 assert r['full_state_match'] and r['ordered_boundaries_match'];checks=2
 if r['status']:return checks
 entry=c['entry'];expected=bytearray(m);count=before.r[5];enabled=c.get('enabled',True);result=0
 if entry in (A,B,D) and not enabled:
  result=0x0d000001;store(expected,load(m,0x8c65acb8),entry);store(expected,load(m,0x8c65acbc),result)
 elif entry in (A,B,D):
  offset={A:0x17c,B:0x194,D:0x164}[entry];store(expected,CTX+offset,before.r[4]);store(expected,CTX+offset+4,count)
  for n in ((8,12,16,20) if entry==A else (8,12,16) if entry==B else (12,16)):store(expected,CTX+offset+n,0)
  if entry==A:store(expected,0x8c8d33ec,CTX+offset)
  if entry==B:
   store(expected,0x8c8d33f8,CTX+offset);store(expected,0x8c8d33f0,before.r[4]);store(expected,0x8c8d33f4,count)
  if entry==D:store(expected,0x8c8d33e4,before.r[4])
  n={A:32,B:8,D:28}[entry];template={A:0x8c67c354,B:0x8c67c374,D:0x8c67c334}[entry]
  iterations=count if (entry==D or count<0x80000000) else 0
  for i in range(iterations):expected[(before.r[4]&0xffffff)+i*n:(before.r[4]&0xffffff)+(i+1)*n]=m[template&0xffffff:(template&0xffffff)+n]
 elif entry==E:
  for offset in range(0,0xa8,8):expected[(CTX+offset+1)&0xffffff]=0;store(expected,CTX+offset+4,0)
  for kind,start,count in ((1,8,8),(2,0x48,8),(3,0x88,4)):
   for i in range(count):expected[(CTX+start+i*8)&0xffffff]=kind;expected[(CTX+start+i*8+1)&0xffffff]=i
  expected[CTX&0xffffff]=9
 elif entry==F:
  requested=DB+4+count*52;other=DB+4+load(m,requested)*52
  if load(m,requested+40)==1:result=0x90101
  else:
   store(expected,other,load(m,requested))
   for offset in (4,8,12,16,20,28,32,40,44,48):store(expected,other+offset,0)
   store(expected,other+24,3);store(expected,requested+40,1);store(expected,before.r[4],requested)
 else:return checks
 # Stack scratch is compared byte-for-byte by the original/native oracle above.
 # This independent semantic oracle checks every byte outside the declared stack.
 sp=before.r[15]&0xffffff;expected[sp-64:sp]=a[sp-64:sp]
 assert a==expected,('semantic RAM',hex(entry),[(hex(0x8c000000+i),x,y) for i,(x,y) in enumerate(zip(a,expected)) if x!=y][:12]);checks+=1
 assert s.r[0]==result and s.r[15]==before.r[15] and s.pr==before.pr;checks+=3
 return checks

def main():
 ram=seed_ram();records=[];coverage=set();edges=set();checks=0
 for c in cases():
  m,s,f=prepare(ram,c);before=effects.State.from_buffer_copy(s);r,path,a,s,f=compare(m,s,f,c);checks+=semantics(m,a,before,s,r,c);r['guest_r0_at_stop']=hex(s.r[0]);records.append(r);coverage.update(path)
  for i,pc in enumerate(path):
   text=ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+distance<len(path):edges.add((pc,path[i+distance]))
  print(hex(c['entry']),r['status'],r['stop_pc'],flush=True)
 rows=[]
 for routine in manifest['routines']:
  body=set(PER_BODY[int(routine['address'],16)]);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  rows.append(dict(address=routine['address'],static_rows=len(body),covered_rows=len(body&coverage),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)]))
 files=[Path(__file__),Path(old.__file__),MP,P/'analysis/movie_record_constructors_connection.json',P/'analysis/movie_init_records_qualification.json']
 report=dict(cases_passed=len(records),independent_checks=checks,status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=rows,dll_sha256=hashlib.sha256(effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},cases=records,scope='Original constructor/copy adapters using explicit direct-call root/data/record preconditions. All machine/RAM/device state and ordered boundaries compare. Independent semantic oracle excludes only declared stack scratch, which remains covered by the full original comparison. Direct SDK constructor is a real original-call sequence, not a natural startup claim; unresolved callees remain stops. No driver result is supplied.')
 (P/'analysis/movie_record_constructors_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Constructor comparisons',len(records),checks,report['status_counts']);print([(r['address'],r['covered_rows'],r['static_rows'],len(r['missing_edges'])) for r in rows])
if __name__=='__main__':main()
