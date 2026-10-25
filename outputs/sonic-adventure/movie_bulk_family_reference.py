"""Original driver-control word update with full interrupt and device state."""
import collections,csv,ctypes,hashlib,itertools,json,sys,types
from pathlib import Path
import verify_movie_driver_control as old
import console_service_reference as console_model
import sound_ram_reference as sound_model
import verify_boot_device as seed
P=old.P;ROOT=old.ROOT;load=old.load;store=old.store
ENTRY,LOAD,POOL,ALLOC,ERROR,SET,CLEAR=(0x8c092fa0,0x8c093e60,0x8c65aa30,0x8c643ac0,0x8c65acac,0x8c5ff2cc,0x8c5ff2de)
CTX,OUT=0x8cd80000,0x8cda0000
MP=P/'analysis/movie_bulk_family_translation.json';manifest=json.loads(MP.read_text());NEW={int(r['address'],16) for r in manifest['routines']}
ROWS=dict(old.ROWS);PER_BODY=dict(old.PER_BODY)
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
namespace=dict(old.namespace,ROWS=ROWS,RETURNS=old.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=old.namespace['LEGACY']-NEW)
code=old.code.replace('instruction_budget=2000000',"instruction_budget=c.get('instruction_budget',2000000)")
marker='   a,n=access';assert code.count(marker)==1
code=code.replace(marker,marker+"\n   if sound_model.inside(a):\n    if not sound_model.valid(a,n):stop(pc,r,sr,-3)\n    if c.get('null_bus') or c.get('sound_unconfigured'):stop(pc,r,sr,-2)")
namespace['sound_model']=sound_model
exec(compile(code,'init_records_original_model','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')
effects=types.SimpleNamespace(**old.effects.__dict__);effects.CONNECTED=old.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=reference_run
effects.DLL=ROOT/'work/sonic-adventure/build/movie_bulk_family.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));console_model.declare(effects.LIB)
observed=effects.LIB.sa_movie_bulk_family_observed;observed.argtypes=old.observed.argtypes;observed.restype=ctypes.c_int
for name in ('sa_driver_signal_bus_init','sa_driver_signal_set_samples','sa_hardware_transfer_set_samples','sa_display_timing_set_samples','sa_disc_queue_init','sa_rtc_sample_bus_init','sa_rtc_set_samples'):
 getattr(effects.LIB,name).argtypes=getattr(old.effects.LIB,name).argtypes;getattr(effects.LIB,name).restype=getattr(old.effects.LIB,name).restype
effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32;native_stats=old.native_stats.__class__()
def invoke(*args):return observed(*args,ctypes.byref(effects.console_state),ctypes.byref(native_stats),0xffffffff)
effects.fn=invoke
compare_code=old.compare_code
exec(compile(compare_code,'init_records_comparison','exec'),dict(old.compare.__globals__,effects=effects,namespace=namespace,native_stats=native_stats),local:={});_legacy_template_compare=local['compare']

import verify_movie_hardware as hardware
RESET,RELEASE,REQUEST,INFO,CALLBACK=(0x8c65a8c8,0x8c65a27c,0x8c65a5dc,0x8c65aaf4,0x8c65a518)
RECORD,CONTEXT=0x8cda0000,0x8cdb0000

class Input(dict):
 def get(self,key,default=None):
  if key=='fifo_samples' and key not in self and 'fifo_ready_count' in self:
   count=self['fifo_ready_count'];assert type(count) is int and 0<=count<=0x200000
   if getattr(self,'_fifo_count',None)!=count:self._fifo_words=[0]*count;self._fifo_count=count
   return self._fifo_words
  return super().get(key,default)

def seed_ram():return seed.seed_ram()
def sample(m,a,n):return bytes(m[a&0xffffff:(a&0xffffff)+n])

def prepare(ram,c):
 entry=c['entry'];context=Input(c);context['entry']=hardware.READ
 m,s,f=hardware.prepare(ram,context);context['entry']=entry;c.update(context);s.pc=entry;s.sr=c.get('initial_sr',0x40000000)
 if 'fill_word' in c:store(m,0x8c67c1c4,c['fill_word'])
 if entry in (CALLBACK,REQUEST):
  s.r[4]=c.get('record',RECORD);s.r[5]=c.get('argument',0)
  store(m,0x8c67c1d0,CONTEXT if c.get('root_enabled',True) else 0)
  for offset,value in ((4,c.get('state',0)),(16,c.get('callback',0xffffffff)),(20,c.get('callback_argument',0x2468ace0)),(24,0xdeadbeef)):store(m,RECORD+offset,value)
 if entry==INFO:store(m,0x8c67c1d0,CONTEXT if c.get('root_enabled',False) else 0)
 if 'stack' in c:s.r[15]=c['stack']
 return m,s,f
