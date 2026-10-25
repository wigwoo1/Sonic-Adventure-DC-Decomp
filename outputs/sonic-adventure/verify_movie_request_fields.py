"""Original initializer records, request pools and actual driver-file loader."""
import collections,csv,ctypes,hashlib,itertools,json,sys,types
from pathlib import Path
import verify_movie_diagnostics as old
import console_service_reference as console_model
import verify_boot_device as seed
P=old.P;ROOT=old.ROOT;load=old.load;store=old.store
ENTRY,LOAD,POOL,ALLOC,ERROR,SET,CLEAR=(0x8c092fa0,0x8c093e60,0x8c65aa30,0x8c643ac0,0x8c65acac,0x8c5ff2cc,0x8c5ff2de)
CTX,OUT=0x8cd80000,0x8cda0000
MP=P/'analysis/movie_request_fields_translation.json';manifest=json.loads(MP.read_text());NEW={int(r['address'],16) for r in manifest['routines']}
ROWS=dict(old.ROWS);PER_BODY=dict(old.PER_BODY)
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
namespace=dict(old.namespace,ROWS=ROWS,RETURNS=old.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=old.namespace['LEGACY']-NEW)
code=old.code
exec(compile(code,'init_records_original_model','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')
effects=types.SimpleNamespace(**old.effects.__dict__);effects.CONNECTED=old.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=reference_run
effects.DLL=ROOT/'work/sonic-adventure/build/movie_request_fields.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));console_model.declare(effects.LIB)
observed=effects.LIB.sa_movie_request_fields_observed;observed.argtypes=old.observed.argtypes;observed.restype=ctypes.c_int
for name in ('sa_driver_signal_bus_init','sa_driver_signal_set_samples','sa_hardware_transfer_set_samples','sa_display_timing_set_samples','sa_disc_queue_init','sa_rtc_sample_bus_init','sa_rtc_set_samples'):
 getattr(effects.LIB,name).argtypes=getattr(old.effects.LIB,name).argtypes;getattr(effects.LIB,name).restype=getattr(old.effects.LIB,name).restype
effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32;native_stats=old.native_stats.__class__()
def invoke(*args):return observed(*args,ctypes.byref(effects.console_state),ctypes.byref(native_stats),0xffffffff)
effects.fn=invoke
compare_code=old.compare_code
exec(compile(compare_code,'init_records_comparison','exec'),dict(old.compare.__globals__,effects=effects,namespace=namespace,native_stats=native_stats),local:={});compare=local['compare']

GET,SET8,SET4,SETC,PAIR,BIND=(0x8c65a568,0x8c65a5ac,0x8c65a5cc,0x8c65a5d4,0x8c65a5b4,0x8c643624)
RECORD,OUTPUT,CONTEXT,DATA=0x8cda0000,0x8cdb0000,0x8cdc0000,0x8cdd0000
PARENT_MANIFEST=P/'analysis/movie_init_records_translation.json'
parent=next(r for r in json.loads(PARENT_MANIFEST.read_text())['routines'] if r['address']==hex(BIND))
TRACKED=list(manifest['routines'])+[parent]

def seed_ram():return seed.seed_ram()
def sample(m,a,n):return bytes(m[a&0xffffff:(a&0xffffff)+n])
def prepare(ram,c):
 m,s,f=old.old.old.prepare(ram,c);entry=c['entry'];alias=c.get('alias',0x8c000000);record=alias|(RECORD&0xffffff);out=alias|(OUTPUT&0xffffff)
 m[(RECORD-16)&0xffffff:(RECORD+128)&0xffffff]=bytes([0xa5])*144
 for offset,value in ((0,1),(4,c.get('state',0)),(8,0x11223344),(12,0x55667788),(16,0x12345678),(20,0x87654321)):store(m,RECORD+offset,value)
 store(m,OUTPUT,0xabcdef01);store(m,0x8c67c1d0,CONTEXT if c.get('enabled',True) else 0)
 for literal,value in ((0x8c65acb8,0x12345678),(0x8c65acbc,0x87654321)):store(m,load(m,literal),value)
 s.r[4]=c.get('record',record);s.r[5]=c.get('value',0x2468ace0);s.r[6]=c.get('second',0x13579bdf)
 if entry==GET:s.r[5]=c.get('output',s.r[4]+4 if c.get('alias_output') else out)
 if entry==BIND:
  s.r[5]=c.get('value',DATA);s.r[6]=c.get('second',0x13579bdf);s.r[7]=c.get('third',0x55aa00ff);store(m,s.r[15],c.get('fourth',0xffffffff))
 if c.get('bad_stack'):s.r[15]=0x8c000000
 return m,s,f

def cases(smoke=False):
 base=next(old.old.old.cases())
 if smoke:
  for entry in (GET,SET8,SET4,SETC,PAIR,BIND):yield base|dict(entry=entry)
  return
 for entry,mode in itertools.product((GET,SET8,SET4,SETC,PAIR,BIND),(0,1,4)):yield base|dict(entry=entry,mode=mode)
 for entry,alias in itertools.product((GET,SET8,SET4,SETC,PAIR,BIND),(0x0c000000,0xac000000)):yield base|dict(entry=entry,alias=alias)
 for state in (0,1,2,0xffffffff,0x80000000):yield base|dict(entry=GET,state=state);yield base|dict(entry=BIND,state=state)
 for extra in (dict(record=0),dict(output=0),dict(record=0,output=0),dict(alias_output=True),dict(record=0x8d000000),dict(output=0x8d000000),dict(bad_stack=True)):yield base|dict(entry=GET)|extra
 for entry in (SET8,SET4,SETC,PAIR):
  for value in (0,1,0xffffffff,0x80000000):yield base|dict(entry=entry,value=value,second=value^0xffffffff)
  for record in (0,0x8d000000):yield base|dict(entry=entry,record=record)
 yield base|dict(entry=PAIR,record=0x8cffffec)
 for extra in (dict(enabled=False),dict(record=0),dict(enabled=False,record=0),dict(record=0x8d000000),dict(bad_stack=True),dict(value=0,second=0,third=0,fourth=0),dict(value=0xffffffff,second=0xffffffff,third=0xffffffff,fourth=0xffffffff)):
  yield base|dict(entry=BIND)|extra

def semantics(m,a,before,s,r,c):
 checks=2;assert r['full_state_match'] and r['ordered_boundaries_match']
 entry=c['entry'];expected=bytearray(m);ptr=before.r[4];sp=before.r[15];status=0
 if r['status']:
  if entry==PAIR and ptr==0x8cffffec:
   store(expected,0x8cfffffc,before.r[5]);assert a==expected and r['status']==-3 and s.r[0]==before.r[0];checks+=3
  return checks
 if entry==GET:
  store(expected,sp-4,before.r[14]);store(expected,sp-8,before.pr)
  if not ptr:status=load(m,0x8c65a59c)
  elif not before.r[5]:status=load(m,0x8c65a5a0)
  else:store(expected,before.r[5],load(m,ptr+4))
  if status:
   store(expected,load(m,0x8c65acb8),load(m,0x8c65a5a4));store(expected,load(m,0x8c65acbc),status)
 elif entry in (SET8,SET4,SETC):store(expected,ptr+{SET8:8,SET4:4,SETC:12}[entry],before.r[5])
 elif entry==PAIR:
  if not ptr:status=load(m,0x8c65a5c8)
  else:store(expected,ptr+16,before.r[5]);store(expected,ptr+20,before.r[6])
 elif entry==BIND:
  for offset,value in ((4,before.r[14]),(8,before.r[13]),(12,before.pr),(16,before.r[5]),(24,before.r[6]),(20,before.r[7])):store(expected,sp-offset,value)
  if not c.get('enabled',True):status=load(m,0x8c6436a8)
  elif not ptr:status=load(m,0x8c6436ac)
  else:
   state=load(m,ptr+4);store(expected,sp-28,state);store(expected,sp-32,ptr);store(expected,sp-36,0x8c643656)
   if state==2:status=load(m,0x8c6436b4)
   else:
    for offset,value in ((4,0),(8,before.r[5]),(12,before.r[6]),(16,before.r[7]),(20,load(m,sp))):store(expected,ptr+offset,value)
  if status:
   store(expected,load(m,0x8c65acb8),load(m,0x8c6436c8));store(expected,load(m,0x8c65acbc),status)
 assert a==expected,('exact independent request RAM',hex(entry),c);checks+=1
 assert s.r[0]==status and s.r[15]==before.r[15] and s.pr==before.pr;checks+=3
 return checks

def main():
 ram=seed_ram();records=[];coverage=set();edges=set();checks=0
 for c in cases('--smoke' in sys.argv):
  m,s,f=prepare(ram,c);before=effects.State.from_buffer_copy(s);r,path,a,s,f=compare(m,s,f,c);checks+=semantics(m,a,before,s,r,c);r['guest_r0_at_stop']=hex(s.r[0]);records.append(r);coverage.update(path)
  for i,pc in enumerate(path):
   text=ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+distance<len(path):edges.add((pc,path[i+distance]))
  print(hex(c['entry']),r['status'],r['stop_pc'],flush=True)
 if '--smoke' in sys.argv:return
 rows=[]
 for routine in TRACKED:
  body=set(PER_BODY[int(routine['address'],16)]);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  rows.append(dict(address=routine['address'],static_rows=len(body),covered_rows=len(body&coverage),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)]))
 files=[Path(__file__),Path(old.__file__),MP,PARENT_MANIFEST,P/'analysis/movie_request_fields_connection.json',P/'analysis/movie_diagnostics_qualification.json']
 report=dict(cases_passed=len(records),independent_checks=checks,status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=rows,dll_sha256=hashlib.sha256(effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},cases=records,scope='Original request getter/setters and existing partial configuration caller. Explicit record/root/output/argument fixtures. Full native/original CPU/RAM/FPU/device/boundary state compares, plus independent exact entire RAM including stack/error state and a partial pair-store fault. No getter or setter result supplied; natural startup tested separately.')
 (P/'analysis/movie_request_fields_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Request-field comparisons',len(records),checks,report['status_counts']);print([(r['address'],r['covered_rows'],r['static_rows'],len(r['missing_edges'])) for r in rows])
if __name__=='__main__':main()
