"""Original driver-control word update with full interrupt and device state."""
import collections,csv,ctypes,hashlib,itertools,json,sys,types
from pathlib import Path
import movie_bulk_family_reference as old
import console_service_reference as console_model
import sound_ram_reference as sound_model
import verify_boot_device as seed
P=old.P;ROOT=old.ROOT;load=old.load;store=old.store
ENTRY,LOAD,POOL,ALLOC,ERROR,SET,CLEAR=(0x8c092fa0,0x8c093e60,0x8c65aa30,0x8c643ac0,0x8c65acac,0x8c5ff2cc,0x8c5ff2de)
CTX,OUT=0x8cd80000,0x8cda0000
MP=P/'analysis/movie_queue_family_translation.json';manifest=json.loads(MP.read_text());NEW={int(r['address'],16) for r in manifest['routines']}
ROWS=dict(old.ROWS);PER_BODY=dict(old.PER_BODY)
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
namespace=dict(old.namespace,ROWS=ROWS,RETURNS=old.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=old.namespace['LEGACY']-NEW)
code=old.code
exec(compile(code,'init_records_original_model','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')
effects=types.SimpleNamespace(**old.effects.__dict__);effects.CONNECTED=old.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=reference_run
effects.DLL=ROOT/'work/sonic-adventure/build/movie_queue_family.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));console_model.declare(effects.LIB)
observed=effects.LIB.sa_movie_queue_family_observed;observed.argtypes=old.observed.argtypes;observed.restype=ctypes.c_int
for name in ('sa_driver_signal_bus_init','sa_driver_signal_set_samples','sa_hardware_transfer_set_samples','sa_display_timing_set_samples','sa_disc_queue_init','sa_rtc_sample_bus_init','sa_rtc_set_samples'):
 getattr(effects.LIB,name).argtypes=getattr(old.effects.LIB,name).argtypes;getattr(effects.LIB,name).restype=getattr(old.effects.LIB,name).restype
effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32;native_stats=old.native_stats.__class__()
def invoke(*args):return observed(*args,ctypes.byref(effects.console_state),ctypes.byref(native_stats),0xffffffff)
effects.fn=invoke
compare_code=old.compare_code
exec(compile(compare_code,'init_records_comparison','exec'),dict(old._legacy_template_compare.__globals__,effects=effects,namespace=namespace,native_stats=native_stats),local:={});_legacy_template_compare=local['compare']

Input=old.Input;seed_ram=old.seed_ram;sample=old.sample
ENQUEUE,DEQUEUE,START,EXECUTE,SUBMIT=(0x8c667080,0x8c666f98,0x8c666d10,0x8c666e02,0x8c666c2c)
CONFIG=0x8c10f9f4
ROOT_CONTEXT,QUEUE,OUT1,OUT2,RECORD=(0x8cdd0000,0x8cda8000,0x8cdf0000,0x8cdf0100,0x8cdb0000)

def prepare(ram,c):
 m,s,f=old.prepare(ram,c);alias=c.get('alias',0x8c000000);root=alias|(ROOT_CONTEXT&0xffffff);queue=alias|(QUEUE&0xffffff)
 store(m,0x8c67c1d0,root if c.get('enabled',True) else 0)
 for offset,value in ((0x194,c.get('queue_pointer',queue)),(0x198,c.get('capacity',4)),(0x19c,c.get('tail',0)),(0x1a0,c.get('head',0)),(0x1a4,c.get('count',1))):store(m,root+offset,value&0xffffffff)
 for i in range(-2,18):store(m,QUEUE+i*8,(0x11110000+i)&0xffffffff);store(m,QUEUE+i*8+4,(0xeeee0000+i)&0xffffffff)
 for address in (OUT1,OUT2):store(m,address,0xa5a5a5a5)
 if c['entry']==ENQUEUE:s.r[4]=c.get('first',RECORD);s.r[5]=c.get('second',0x2468ace0)
 if c['entry']==DEQUEUE:s.r[4]=c.get('output1',alias|(OUT1&0xffffff));s.r[5]=c.get('output2',alias|(OUT2&0xffffff))
 if c['entry'] in (START,SUBMIT):
  s.r[4]=c.get('record',RECORD);s.r[5]=c.get('argument',0)
  for offset,value in ((4,c.get('state',0)),(8,0x8cd70040),(12,64),(16,0xffffffff),(20,0),(24,0xdeadbeef)):store(m,RECORD+offset,value)
 if c['entry']==CONFIG:
  s.r[4]=c.get('record',RECORD)
  for reg,key,default in ((5,'source',0x8cd70040),(6,'destination',0xa0800000),(7,'transfer_size',64)):s.r[reg]=c.get(key,default)
  for i,(key,default) in enumerate((('direction',1),('flag_a',1),('flag_b',1),('callback',0x8c668f40),('callback_argument',RECORD))):store(m,s.r[15]+i*4,c.get(key,default))
  for offset in range(0,0x40,4):store(m,RECORD+offset,0xa5a50000+offset)
  store(m,RECORD+0x14,c.get('flags',0xdeadbeef))
 return m,s,f
