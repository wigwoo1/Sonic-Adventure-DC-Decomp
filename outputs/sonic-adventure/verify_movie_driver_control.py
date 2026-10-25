"""Original driver-control word update with full interrupt and device state."""
import collections,csv,ctypes,hashlib,itertools,json,sys,types
from pathlib import Path
import verify_movie_request_fields as old
import console_service_reference as console_model
import verify_boot_device as seed
P=old.P;ROOT=old.ROOT;load=old.load;store=old.store
ENTRY,LOAD,POOL,ALLOC,ERROR,SET,CLEAR=(0x8c092fa0,0x8c093e60,0x8c65aa30,0x8c643ac0,0x8c65acac,0x8c5ff2cc,0x8c5ff2de)
CTX,OUT=0x8cd80000,0x8cda0000
MP=P/'analysis/movie_driver_control_translation.json';manifest=json.loads(MP.read_text());NEW={int(r['address'],16) for r in manifest['routines']}
ROWS=dict(old.ROWS);PER_BODY=dict(old.PER_BODY)
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
namespace=dict(old.namespace,ROWS=ROWS,RETURNS=old.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=old.namespace['LEGACY']-NEW)
code=old.code
exec(compile(code,'init_records_original_model','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')
effects=types.SimpleNamespace(**old.effects.__dict__);effects.CONNECTED=old.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=reference_run
effects.DLL=ROOT/'work/sonic-adventure/build/movie_driver_control.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));console_model.declare(effects.LIB)
observed=effects.LIB.sa_movie_driver_control_observed;observed.argtypes=old.observed.argtypes;observed.restype=ctypes.c_int
for name in ('sa_driver_signal_bus_init','sa_driver_signal_set_samples','sa_hardware_transfer_set_samples','sa_display_timing_set_samples','sa_disc_queue_init','sa_rtc_sample_bus_init','sa_rtc_set_samples'):
 getattr(effects.LIB,name).argtypes=getattr(old.effects.LIB,name).argtypes;getattr(effects.LIB,name).restype=getattr(old.effects.LIB,name).restype
effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32;native_stats=old.native_stats.__class__()
def invoke(*args):return observed(*args,ctypes.byref(effects.console_state),ctypes.byref(native_stats),0xffffffff)
effects.fn=invoke
compare_code=old.compare_code
exec(compile(compare_code,'init_records_comparison','exec'),dict(old.compare.__globals__,effects=effects,namespace=namespace,native_stats=native_stats),local:={});compare=local['compare']

import verify_movie_hardware as hardware
CONTROL,SHADOW,PORT=0x8c65a200,0x8c8a3408,0xa0702c00

def seed_ram():return seed.seed_ram()
def sample(m,a,n):return bytes(m[a&0xffffff:(a&0xffffff)+n])

def prepare(ram,c):
 entry=c['entry'];context=c|dict(entry=hardware.READ)
 m,s,f=hardware.prepare(ram,context);context['entry']=entry;c.update(context)
 s.pc=c['entry'];s.sr=c.get('initial_sr',0x40000000)
 store(m,SHADOW,c.get('initial_shadow',0xa5a5f000))
 if 'stack' in c:s.r[15]=c['stack']
 return m,s,f

def cases(smoke=False):
 base=dict(entry=CONTROL,mode=0,control_word=0x2468ace0,fifo_samples=[0]*32,mask_interrupts=1)
 if smoke:
  yield base
  yield base|dict(initial_sr=0x600083f3,mode=4,control_word=0xffffffff)
  yield base|dict(initial_sr=0)
  yield base|dict(fifo_samples=[])
  return
 for mode,mask,bank in itertools.product((0,1,4),range(16),(0,1)):
  yield base|dict(mode=mode,initial_sr=0x40008302|(bank<<29)|(mask<<4),control_word=(0x9e3779b9*(mask+1)+bank)&0xffffffff)
 for word in (0,1,2,0x7ffffffe,0x80000000,0xfffffffe,0xffffffff,0x12345678):
  yield base|dict(mode=4,control_word=word,initial_sr=0x60000071)
 for word in (0,0xffffffff):yield base|dict(mask_interrupts=0,control_word=word)
 for counter in (0xfffffffe,0xffffffff):yield base|dict(read_counter=counter,write_counter=counter)
 for samples in ([],[0x20],[0x21,0x20,0],[0,0]):yield base|dict(fifo_samples=samples)
 for address,status in itertools.product((hardware.FIFO,PORT),(-2,-3)):
  yield base|dict(reject_bus=address,bus_status=status)
 yield base|dict(null_bus=True)
 for sp in (0x8c000000,0x8c000004):yield base|dict(stack=sp)
 for sr in (0,0x20000001):yield base|dict(initial_sr=sr)

def semantics(m,a,before,s,record,path,c):
 checks=2;assert record['full_state_match'] and record['ordered_boundaries_match']
 if record['status']:
  if not before.sr&0x40000000:
   assert record['status']==-4 and record['stop_pc']=='0x8c65a206' and s.sr==before.sr and s.r[15]==before.r[15]-12;checks+=4
  return checks
 value=c.get('control_word',0)|1
 assert load(a,SHADOW)==record['control_word']==value;checks+=2
 events=[e for e in record['hardware_events'] if e[0] in (23,24)]
 assert events==[[23,PORT,c['control_word'],4],[24,PORT,value,4]];checks+=1
 assert s.r[0]==0 and s.r[4]==0 and s.r[15]==before.r[15] and s.pr==before.pr and s.r[14]==before.r[14];checks+=5
 assert s.sr==(before.sr|1) and s.sr&0xf0==before.sr&0xf0;checks+=2
 assert path.count(0x8c65a25a)==256 and path.count(0x8c65a25c)==256;checks+=2
 expected=bytearray(m);store(expected,SHADOW,value);sp=before.r[15]&0xffffff
 # Existing original channel install/restore and wait helpers produce these
 # effects for the declared [0,1,0,1] channel fixture. Counters wrap at 32 bits.
 for offset,increment in ((0x10c,3),(0x110,1)):
  address=hardware.CTX+offset;store(expected,address,(load(m,address)+increment)&0xffffffff)
 for channel in (1,3):
  store(expected,hardware.CTX+4+channel*52+0x18,2)
  store(expected,hardware.CHANNELS+channel*32+0x1c,0)
 expected[sp-128:sp]=a[sp-128:sp]
 assert a==expected;checks+=1
 calls=[b for b in record['boundaries'] if b['target'] in ('0x8c10d460','0x8c10d770')]
 assert len(calls)==2 and calls[0]['arguments']==[PORT,SHADOW,4,1] and calls[1]['arguments']==[SHADOW,PORT,4,1];checks+=3
 return checks

def main():
 ram=seed_ram();records=[];coverage=set();edges=set();checks=0
 for c in cases('--smoke' in sys.argv):
  m,s,f=prepare(ram,c);before=effects.State.from_buffer_copy(s);r,path,a,s,f=compare(m,s,f,c)
  count=semantics(m,a,before,s,r,path,c);checks+=count;r['independent_checks']=count;r['guest_r0_at_stop']=hex(s.r[0]);r['guest_sr_at_stop']=hex(s.sr);records.append(r);coverage.update(path)
  for i,pc in enumerate(path):
   text=ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+distance<len(path):edges.add((pc,path[i+distance]))
  print(hex(c['initial_sr']) if 'initial_sr' in c else 'default SR',r['status'],r['stop_pc'],flush=True)
 if '--smoke' in sys.argv:print('Driver-control smoke passed',len(records));return
 rows=[]
 for routine in manifest['routines']:
  body=set(PER_BODY[int(routine['address'],16)]);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  rows.append(dict(address=routine['address'],static_rows=len(body),covered_rows=len(body&coverage),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)]))
 files=[Path(__file__),Path(old.__file__),Path(hardware.__file__),MP,P/'analysis/movie_driver_control_connection.json',P/'analysis/movie_request_fields_qualification.json']
 report=dict(cases_passed=len(records),independent_checks=checks,status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=rows,dll_sha256=hashlib.sha256(effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},cases=records,scope='Actual original driver-control read/set-bit/write through both existing transfer services. All ordinary/banked/special CPU, FPU, RAM, device and boundary state compares. Additional semantic checks cover both transfer arguments, device events, exact control value, interrupt-mask restoration, result/link/stack registers, 256 actual delay iterations and exact known channel/status/counter effects and unchanged RAM elsewhere, excluding a 128-byte callee stack window. The primary oracle compares every stack byte. Initialized hardware-context fixtures and finite FIFO/control inputs are explicit software policy; no game service result is supplied. Host/privilege faults do not claim physical exception delivery; natural startup is tested separately.')
 (P/'analysis/movie_driver_control_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Driver-control comparisons',len(records),checks,report['status_counts']);print(rows)
if __name__=='__main__':main()
