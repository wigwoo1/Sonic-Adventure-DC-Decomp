"""Original driver-control word update with full interrupt and device state."""
import collections,csv,ctypes,hashlib,itertools,json,sys,types
from pathlib import Path
import movie_transfer_start_reference as old
import console_service_reference as console_model
import sound_ram_reference as sound_model
import verify_boot_device as seed
P=old.P;ROOT=old.ROOT;load=old.load;store=old.store
ENTRY,LOAD,POOL,ALLOC,ERROR,SET,CLEAR=(0x8c092fa0,0x8c093e60,0x8c65aa30,0x8c643ac0,0x8c65acac,0x8c5ff2cc,0x8c5ff2de)
CTX,OUT=0x8cd80000,0x8cda0000
MP=P/'analysis/movie_cpu_transfer_translation.json';manifest=json.loads(MP.read_text());NEW={int(r['address'],16) for r in manifest['routines']}
ROWS=dict(old.ROWS);PER_BODY=dict(old.PER_BODY)
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
namespace=dict(old.namespace,ROWS=ROWS,RETURNS=old.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=old.namespace['LEGACY']-NEW)
code=old.code
exec(compile(code,'init_records_original_model','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')
effects=types.SimpleNamespace(**old.effects.__dict__);effects.CONNECTED=old.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=reference_run
effects.DLL=ROOT/'work/sonic-adventure/build/movie_cpu_transfer.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));console_model.declare(effects.LIB)
observed=effects.LIB.sa_movie_cpu_transfer_observed;observed.argtypes=old.observed.argtypes;observed.restype=ctypes.c_int
for name in ('sa_driver_signal_bus_init','sa_driver_signal_set_samples','sa_hardware_transfer_set_samples','sa_display_timing_set_samples','sa_disc_queue_init','sa_rtc_sample_bus_init','sa_rtc_set_samples'):
 getattr(effects.LIB,name).argtypes=getattr(old.effects.LIB,name).argtypes;getattr(effects.LIB,name).restype=getattr(old.effects.LIB,name).restype
effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32;native_stats=old.native_stats.__class__()
def invoke(*args):return observed(*args,ctypes.byref(effects.console_state),ctypes.byref(native_stats),0xffffffff)
effects.fn=invoke
compare_code=old.compare_code
exec(compile(compare_code,'init_records_comparison','exec'),dict(old._legacy_template_compare.__globals__,effects=effects,namespace=namespace,native_stats=native_stats),local:={});_legacy_template_compare=local['compare']

Input=old.Input;seed_ram=old.seed_ram;sample=old.sample
COPY,COUNTER,SUBMIT,REQUEST,CLIENT_START=(0x8c669088,0x8c666e06,0x8c666c2c,0x8c65a5dc,old.CLIENT_START)
SRC,RECORD,ROOT_CONTEXT,OUT1=(0x8c900000,old.RECORD,old.ROOT_CONTEXT,old.OUT1)
DRIVER=ROOT/'work/sonic-adventure/extracted/files/SONICADV/MASARU.DRV'

def prepare(ram,c):
 entry=c['entry'];length=c.get('length',64)&0xffffffff;source=c.get('source',SRC);client=entry in (SUBMIT,REQUEST,CLIENT_START)
 context=Input(c|dict(entry=CLIENT_START if client else old.START,destination=source,length=length,sdk_mode=c.get('sdk_mode',0),current_record=c.get('current_record',0)))
 m,s,f=old.prepare(ram,context);context['entry']=entry;c.update(context);s.pc=entry
 store(m,ROOT_CONTEXT+old.half(m,0x8c668fe0),c.get('sdk_mode',0))
 store(m,load(m,0x8c666e4c),c.get('error_counter',0));store(m,load(m,0x8c669100),0x13579bdf);store(m,0x8c67340c,0x11223344)
 if c.get('source_pattern')=='masaru':
  payload=DRIVER.read_bytes()[32:];assert length==len(payload);m[SRC&0xffffff:(SRC&0xffffff)+length]=payload
 else:
  n=c.get('pattern_bytes',max(64,min(length if length<0x80000000 else 0,0x200000)))
  assert 0<=n<=0x200000
  m[SRC&0xffffff:(SRC&0xffffff)+n]=bytes((i*37+(i>>8)*13+(i>>16)*7+11)&255 for i in range(n))
 if source==0x8cfffffc:store(m,source,0x76543210)
 if entry==COPY:
  s.r[4]=source;s.r[5]=c.get('offset',0);s.r[6]=length;s.r[7]=c.get('callback',0xffffffff);store(m,s.r[15],c.get('callback_argument',0x2468ace0))
 if client:s.r[5]=c.get('offset',0)
 if entry==COUNTER:s.r[0]=c.get('r0',0x89abcdef)
 return m,s,f

