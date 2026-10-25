"""Original initialization caller and cable/region dependencies, retaining BIOS stops."""
import collections,csv,ctypes,hashlib,itertools,json,types
from pathlib import Path
import verify_movie_disc_paths as old
P=old.P;ROOT=old.ROOT;load=old.load;store=old.store
CALLER,SET,QUERY,SIGNATURE,COUNTDOWN,REGION,CLEAR,SETUP,INFO,INFO_INIT,FLASH=(0x8c0546c0,0x8c604510,0x8c10c07e,0x8c10ba1e,0x8c051760,0x8c10c044,0x8c055c44,0x8c603b84,0x8c10c0f8,0x8c10c0d0,0x8c10da50)
MP=P/'analysis/movie_initialize_translation.json';manifest=json.loads(MP.read_text());NEW={int(r['address'],16) for r in manifest['routines']}
ROWS=dict(old.ROWS);PER_BODY=dict(old.PER_BODY)
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
namespace=dict(old.namespace,ROWS=ROWS,RETURNS=old.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=old.namespace['LEGACY']-NEW,unknown_callback_word=load)
code=old.old.old.code;marker=' hooked[0x8c66598c]=clear_hook';assert code.count(marker)==1
# The decoder cannot fetch instructions at an unmapped unresolved callback.
# Match the existing native dispatch frontier: record its entry, then stop.
# This hook never returns a service result or continues original execution.
code=code.replace(marker,marker+"\n for vector in (0x8c0000b0,0x8c0000b8):\n  target=unknown_callback_word(memory,vector)\n  if target not in ROWS and target!=BIOS:\n   def missing_service(m,regs,sr,target=target):\n    events.append(event(target,regs,sr,state.pr));stop(target,regs,sr,-2)\n   hooked[target]=missing_service")
exec(compile(code,'initialize_caller_original_model','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')
effects=types.SimpleNamespace(**old.effects.__dict__);effects.CONNECTED=old.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=reference_run
effects.DLL=ROOT/'work/sonic-adventure/build/movie_initialize.dll';effects.LIB=ctypes.CDLL(str(effects.DLL))
observed=effects.LIB.sa_movie_initialize_observed;observed.argtypes=old.observed.argtypes;observed.restype=ctypes.c_int
for name in ('sa_driver_signal_bus_init','sa_driver_signal_set_samples','sa_hardware_transfer_set_samples','sa_display_timing_set_samples','sa_disc_queue_init','sa_rtc_sample_bus_init','sa_rtc_set_samples'):
 getattr(effects.LIB,name).argtypes=getattr(old.effects.LIB,name).argtypes;getattr(effects.LIB,name).restype=getattr(old.effects.LIB,name).restype
effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32
native_stats=old.native_stats.__class__()
def invoke(*args):return observed(*args,ctypes.byref(native_stats),0xffffffff)
effects.fn=invoke
compare=types.FunctionType(old.compare.__code__,dict(old.compare.__globals__,effects=effects,namespace=namespace,native_stats=native_stats),'compare')

def prepare(ram,c):
 m,s,f=old.old.old.prepare(ram,c)
 if c['entry']==SET:s.r[4]=c.get('value',0)
 if c['entry']==SIGNATURE:
  signature=load(m,0x8c10ba38);address=load(m,0x8c10ba44);value_address=load(m,0x8c10ba40)
  store(m,address,signature if c.get('matches',True) else signature^0x80000000)
  at=value_address&0xffffff;m[at:at+2]=(c.get('value',0)&0xffff).to_bytes(2,'little')
 if c['entry']==COUNTDOWN:s.r[4]=c.get('value',2);s.r[5]=c.get('limit',2)
 if c['entry']==CALLER:
  store(m,load(m,0x8c0547e0),0x12345678);store(m,load(m,0x8c0547e4),0x87654321)
 if c.get('bad_stack'):s.r[15]=0x8c000000
 return m,s,f

def cases(smoke=False):
 base=dict(mode=0,rtc_samples=[],fifo_samples=[0]*128,scan_samples=[0,1]*32)
 if smoke:
  for a in sorted(NEW):yield base|dict(entry=a)
  return
 for a in sorted(NEW):
  for mode in (0,1,4):yield base|dict(entry=a,mode=mode)
 for a,value in itertools.product((SET,SIGNATURE),(1,0x7fff,0x8000,0xffff,0xffffffff)):yield base|dict(entry=a,value=value)
 for matches,value in itertools.product((False,True),(0,1,2,0x8000,0xffff)):yield base|dict(entry=SIGNATURE,matches=matches,value=value)
 for a,port in itertools.product((CALLER,QUERY),(0x100,0x200,0x300,0xa35a)):yield base|dict(entry=a,port_data=port)
 for a in (CALLER,QUERY,REGION,SETUP,COUNTDOWN):yield base|dict(entry=a,bad_stack=True)
 for a in (CALLER,QUERY):yield base|dict(entry=a,null_bus=True)

def main():
 import sys
 ram=old.driver.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.seed_ram();records=[];coverage=set();edges=set();checks=0
 for c in cases('--smoke' in sys.argv):
  m,s,f=prepare(ram,c);before=effects.State.from_buffer_copy(s);record,path,a,s,f=compare(m,s,f,c)
  assert record['full_state_match'] and record['ordered_boundaries_match'];checks+=2
  entry=c['entry']
  if entry in (SET,CLEAR,SIGNATURE):
   assert record['status']==0 and s.r[15]==before.r[15] and s.pr==before.pr;checks+=3
   if entry==SET:
    expected=bytearray(m);store(expected,load(m,0x8c60461c),before.r[4]);assert a==expected;checks+=1
   if entry==CLEAR:
    expected=bytearray(m);store(expected,load(m,0x8c055d08),0);assert a==expected;checks+=1
   if entry==SIGNATURE:
    value=int.from_bytes(old.sample(m,load(m,0x8c10ba40),2),'little',signed=True) if c.get('matches',True) else 0
    assert s.r[0]==value&0xffffffff and a==m;checks+=2
  if entry==CALLER and not c.get('bad_stack'):
   assert load(a,load(m,0x8c0547e0))==load(a,load(m,0x8c0547e4))==0;checks+=2
   assert load(a,load(m,0x8c60461c))==load(m,0x8c0547e8);checks+=1
  record['guest_r0_at_stop']=hex(s.r[0]);record['system_info_vector']=hex(load(m,0x8c0000b0));record['flash_vector']=hex(load(m,0x8c0000b8));records.append(record);coverage.update(path)
  for i,pc in enumerate(path):
   text=ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+distance<len(path):edges.add((pc,path[i+distance]))
  print(hex(entry),record['status'],record['stop_pc'],flush=True)
 if '--smoke' in sys.argv:return
 rows=[]
 for routine in manifest['routines']:
  body=set(PER_BODY[int(routine['address'],16)]);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  rows.append(dict(address=routine['address'],static_rows=len(body),covered_rows=len(body&coverage),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)]))
 files=[Path(__file__),Path(old.__file__),MP,P/'analysis/movie_initialize_connection.json',P/'analysis/movie_disc_paths_qualification.json',P/'source/sa_movie_initialize.c',P/'source/sa_movie_initialize_connected.c']
 report=dict(cases_passed=len(records),independent_checks=checks,status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=rows,next_dependencies=sorted({r['stop_pc'] for r in records if r['status']}),compiled_source_count=172,new_registry_credit=0,dll_sha256=hashlib.sha256(effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},cases=records,scope='Actual initializer caller, cable/region decisions and canonical adapters. Original console-info/flash callback slots and unrelated missing game services remain explicit stops. No BIOS result, callback return, region result or early-initializer completion is supplied. Full machine, RAM, FPU, software devices and ordered boundaries match reached original execution; parent bodies remain partial and earn no routine credit.')
 (P/'analysis/movie_initialize_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Initializer caller',len(records),'cases',checks,'checks',report['status_counts']);print([(r['address'],r['covered_rows'],r['static_rows']) for r in rows])
if __name__=='__main__':main()
