"""Original initializer records, request pools and actual driver-file loader."""
import collections,csv,ctypes,hashlib,itertools,json,sys,types
from pathlib import Path
import verify_movie_console as old
P=old.P;ROOT=old.ROOT;load=old.load;store=old.store
ENTRY,LOAD,POOL,ALLOC,ERROR,SET,CLEAR=(0x8c092fa0,0x8c093e60,0x8c65aa30,0x8c643ac0,0x8c65acac,0x8c5ff2cc,0x8c5ff2de)
CTX,OUT=0x8cd80000,0x8cda0000
MP=P/'analysis/movie_init_records_translation.json';manifest=json.loads(MP.read_text());NEW={int(r['address'],16) for r in manifest['routines']}
ROWS=dict(old.ROWS);PER_BODY=dict(old.PER_BODY)
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
namespace=dict(old.namespace,ROWS=ROWS,RETURNS=old.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=old.namespace['LEGACY']-NEW)
code=old.code;marker=' def event(target,r,sr,pr):return ';assert code.count(marker)==1
code=code.replace(marker," def event(target,r,sr,pr):\n  if c.get('progress') and len(events)%4096==0:print('reference boundary',len(events),hex(target),flush=True)\n  return ")
exec(compile(code,'init_records_original_model','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')
effects=types.SimpleNamespace(**old.effects.__dict__);effects.CONNECTED=old.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=reference_run
effects.DLL=ROOT/'work/sonic-adventure/build/movie_init_records.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));old.console_model.declare(effects.LIB)
observed=effects.LIB.sa_movie_init_records_observed;observed.argtypes=old.observed.argtypes;observed.restype=ctypes.c_int
for name in ('sa_driver_signal_bus_init','sa_driver_signal_set_samples','sa_hardware_transfer_set_samples','sa_display_timing_set_samples','sa_disc_queue_init','sa_rtc_sample_bus_init','sa_rtc_set_samples'):
 getattr(effects.LIB,name).argtypes=getattr(old.effects.LIB,name).argtypes;getattr(effects.LIB,name).restype=getattr(old.effects.LIB,name).restype
effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32;native_stats=old.native_stats.__class__()
def invoke(*args):return observed(*args,ctypes.byref(effects.console_state),ctypes.byref(native_stats),0xffffffff)
effects.fn=invoke
compare_code=old.compare_code;marker="  boundaries.append(dict(target=hex(target),pr=hex(pr[0]),stack=hex(r[15]),arguments=list(r[4:8])))";assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker,marker+"\n  if c.get('progress') and len(events)%4096==0:print('native boundary',len(events),hex(target),flush=True)")
exec(compile(compare_code,'init_records_comparison','exec'),dict(old.compare.__globals__,effects=effects,namespace=namespace,native_stats=native_stats),local:={});compare=local['compare']

def sample(m,a,n):return bytes(m[a&0xffffff:(a&0xffffff)+n])
def prepare(ram,c):
 m,s,f=old.prepare(ram,c);entry=c['entry']
 if c.get('cold_filesystem'):store(m,0x8c673414,0);store(m,0x8c673420,0)
 if entry in (POOL,ALLOC):
  root=load(m,0x8c65aae0);store(m,root,CTX if c.get('enabled',True) else 0)
  m[(CTX-16)&0xffffff:(CTX+272)&0xffffff]=bytes([0xa5])*288
  for offset,count in ((8,8),(0x48,8),(0x88,4)):
   for i in range(count):store(m,CTX+offset+i*8,0x11220000+i);store(m,CTX+offset+i*8+4,c.get('free_flag',0))
  kind=1 if entry==ALLOC else c.get('kind',1);offset,count={1:(8,8),2:(0x48,8),3:(0x88,4)}.get(kind,(8,0))
  for i in range(min(c.get('occupied',0),count)):store(m,CTX+offset+i*8+4,1)
  pointer=CTX+offset+c.get('occupied',0)*8+4 if c.get('alias_output') else c.get('output',OUT)
  store(m,OUT,0xabcdef01);s.r[4]=pointer;s.r[5]=c.get('kind',1)&0xffffffff
  for literal,value in ((0x8c65acb8,0x12345678),(0x8c65acbc,0x87654321)):store(m,load(m,literal),value)
 if entry==ERROR:s.r[4]=c.get('message',0x12345678);s.r[5]=c.get('value',0x87654321)
 if entry in (SET,CLEAR):s.r[4]=c.get('value',OUT)
 if c.get('bad_stack'):s.r[15]=0x8c000000
 return m,s,f

def cases(smoke=False):
 base=dict(mode=0,console_info=True,console_flash=True,rtc_samples=[0x1234,0xabcd]*32,render_seed={0x5f8000:0x17fd11db,0x5f8004:0x11},startup_overrides={0x5f689c:0xb},irq_samples=[0,8],fifo_samples=[0]*256,scan_samples=[0,1]*64)
 if smoke:
  for entry in (POOL,ALLOC,ERROR,SET,CLEAR,ENTRY,LOAD):yield base|dict(entry=entry)
  return
 for entry,mode in itertools.product((POOL,ALLOC,ERROR,SET,CLEAR),(0,1,4)):yield base|dict(entry=entry,mode=mode)
 for kind in (1,2,3):
  count=4 if kind==3 else 8
  for occupied in (0,1,count-1,count):yield base|dict(entry=POOL,kind=kind,occupied=occupied)
 for kind in (0,4,0xffffffff,0x80000000):yield base|dict(entry=POOL,kind=kind)
 for entry in (POOL,ALLOC):
  for extra in (dict(occupied=8),dict(occupied=3,free_flag=2),dict(free_flag=0xffffffff),dict(alias_output=True),dict(output=0x8d000000),dict(output=0),dict(enabled=False),dict(bad_stack=True)):yield base|dict(entry=entry)|extra
 for entry in (SET,CLEAR):
  for value in (0,1,0xffffffff,0x80000000):yield base|dict(entry=entry,value=value)
 for value in (0,1,0xffffffff,0x80000000):yield base|dict(entry=ERROR,message=value,value=value^0xffffffff)

def semantics(m,a,before,s,r,c):
 checks=2;assert r['full_state_match'] and r['ordered_boundaries_match']
 if r['status']:return checks
 entry=c['entry']
 if entry==ERROR:
  expected=bytearray(m);store(expected,load(m,0x8c65acb8),before.r[4]);store(expected,load(m,0x8c65acbc),before.r[5]);assert a==expected and s.r[0]==before.r[0] and s.r[15]==before.r[15];checks+=3
 elif entry==SET:
  expected=bytearray(m)
  if before.r[4]:store(expected,0x8c67340c,before.r[4])
  assert a==expected and s.r[0]==(0 if before.r[4] else 0xffffffff) and bool(s.sr&1)==(not before.r[4]);checks+=3
 elif entry==CLEAR:
  expected=bytearray(m);store(expected,load(m,0x8c5ff4b4),0)
  assert a==expected and s.r[0]==0 and s.sr==before.sr;checks+=3
 elif entry in (POOL,ALLOC):
  expected=bytearray(m);stack=before.r[15]
  saved=((4,before.r[14]),(8,before.r[13]),(12,before.pr)) if entry==POOL else ((4,before.r[14]),(8,before.pr))
  for offset,value in saved:store(expected,stack-offset,value)
  kind=c.get('kind',1) if entry==POOL else 1;enabled=c.get('enabled',True);out=before.r[4];code=0;chosen=None
  if entry==ALLOC and not enabled:code=load(m,0x8c643af8)
  elif entry==ALLOC and not out:pass
  elif kind not in (1,2,3):code=load(m,0x8c65aae4)
  else:
   start,count={1:(8,8),2:(0x48,8),3:(0x88,4)}[kind]
   chosen=next((CTX+start+i*8 for i in range(count) if load(m,CTX+start+i*8+4)!=1),0)
   store(expected,out,chosen)
   if chosen:store(expected,chosen+4,1)
   else:code=load(m,0x8c65aae8)
   if entry==ALLOC:
    store(expected,stack-12,0);store(expected,stack-16,before.r[13]);store(expected,stack-20,0x8c643ade)
  if code:
   message=load(m,0x8c643b00) if entry==ALLOC else load(m,0x8c65aaec)
   store(expected,load(m,0x8c65acb8),message);store(expected,load(m,0x8c65acbc),code)
  assert s.r[0]==code and s.r[15]==before.r[15] and s.pr==before.pr;checks+=3
  assert a==expected;checks+=1
 return checks

def main():
 ram=old.old.old.driver.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.seed_ram();records=[];coverage=set();edges=set();checks=0
 for c in cases('--smoke' in sys.argv):
  m,s,f=prepare(ram,c);before=effects.State.from_buffer_copy(s);r,path,a,s,f=compare(m,s,f,c);checks+=semantics(m,a,before,s,r,c);r['guest_r0_at_stop']=hex(s.r[0]);records.append(r);coverage.update(path)
  for i,pc in enumerate(path):
   text=ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+distance<len(path):edges.add((pc,path[i+distance]))
  print(hex(c['entry']),r['status'],r['stop_pc'],flush=True)
 if '--smoke' in sys.argv:return
 rows=[]
 for routine in manifest['routines']:
  body=set(PER_BODY[int(routine['address'],16)]);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  rows.append(dict(address=routine['address'],static_rows=len(body),covered_rows=len(body&coverage),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)]))
 files=[Path(__file__),Path(old.__file__),MP,P/'analysis/movie_init_records_connection.json',P/'analysis/movie_console_qualification.json']
 report=dict(cases_passed=len(records),independent_checks=checks,status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=rows,dll_sha256=hashlib.sha256(effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},cases=records,scope='Original request-pool/record adapters with explicit direct-call root/slot/output fixtures. Every reached service executes or retains its actual missing/fault boundary. Actual startup/file loading is verified separately; no original helper/driver result is substituted.')
 (P/'analysis/movie_init_records_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Record comparisons',len(records),checks,report['status_counts']);print([(r['address'],r['covered_rows'],r['static_rows'],len(r['missing_edges'])) for r in rows])
if __name__=='__main__':main()
