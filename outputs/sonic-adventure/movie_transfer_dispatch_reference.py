"""Original driver-control word update with full interrupt and device state."""
import collections,csv,ctypes,hashlib,itertools,json,sys,types
from pathlib import Path
import movie_transfer_interrupt_reference as old
import console_service_reference as console_model
import sound_ram_reference as sound_model
import verify_boot_device as seed
P=old.P;ROOT=old.ROOT;load=old.load;store=old.store
ENTRY,LOAD,POOL,ALLOC,ERROR,SET,CLEAR=(0x8c092fa0,0x8c093e60,0x8c65aa30,0x8c643ac0,0x8c65acac,0x8c5ff2cc,0x8c5ff2de)
CTX,OUT=0x8cd80000,0x8cda0000
MP=P/'analysis/movie_transfer_dispatch_translation.json';manifest=json.loads(MP.read_text());NEW={int(r['address'],16) for r in manifest['routines']}
ROWS=dict(old.ROWS);PER_BODY=dict(old.PER_BODY)
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
namespace=dict(old.namespace,ROWS=ROWS,RETURNS=old.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=old.namespace['LEGACY']-NEW)
code=old.code
exec(compile(code,'init_records_original_model','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')
effects=types.SimpleNamespace(**old.effects.__dict__);effects.CONNECTED=old.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=reference_run
effects.DLL=ROOT/'work/sonic-adventure/build/movie_transfer_dispatch.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));console_model.declare(effects.LIB)
observed=effects.LIB.sa_movie_transfer_dispatch_observed;observed.argtypes=old.observed.argtypes;observed.restype=ctypes.c_int
for name in ('sa_driver_signal_bus_init','sa_driver_signal_set_samples','sa_hardware_transfer_set_samples','sa_display_timing_set_samples','sa_disc_queue_init','sa_rtc_sample_bus_init','sa_rtc_set_samples'):
 getattr(effects.LIB,name).argtypes=getattr(old.effects.LIB,name).argtypes;getattr(effects.LIB,name).restype=getattr(old.effects.LIB,name).restype
effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32;native_stats=old.native_stats.__class__()
def invoke(*args):return observed(*args,ctypes.byref(effects.console_state),ctypes.byref(native_stats),0xffffffff)
effects.fn=invoke
compare_code=old.compare_code
exec(compile(compare_code,'init_records_comparison','exec'),dict(old._legacy_template_compare.__globals__,effects=effects,namespace=namespace,native_stats=native_stats),local:={});_legacy_template_compare=local['compare']

Input=old.Input;seed_ram=old.seed_ram;sample=old.sample
GENERIC,PRIMARY,SECONDARY,ASIC=0x8c645afa,0x8c645b1e,0x8c645b68,0x8c645b8e
COUNTER=old.old.COUNTER;HANDLER=old.HANDLER;DESCRIPTOR=old.DESCRIPTOR;RECORD=old.RECORD;ROOT_CONTEXT=old.ROOT_CONTEXT;TABLE=old.TABLE
NODES=0x8ce00000

def prepare(ram,c):
 entry=c['entry'];context=Input(c|dict(entry=HANDLER));m,s,f=old.prepare(ram,context);context['entry']=entry;c.update(context);s.pc=entry
 m[0x892840:0x893c00]=bytes(0x13c0)
 events=c.get('events',[c.get('event',0xa60)])
 nodes=[]
 for event_index,event in enumerate(events):
  depth=c.get('depth',1);head=NODES+event_index*0x100
  signed=ctypes.c_int16(event&0xffff).value;head_slot=(0x8c893840+((signed>>4)*4))&0xffffffff
  store(m,head_slot,head if depth else 0)
  for i in range(depth):
   callback=c.get('dispatch_callback',COUNTER);arg=c.get('dispatch_argument',0x13579bdf)
   node=head+i*16
   if c.get('bad_next') and i==depth-1:next_node=0x8d000000
   else:next_node=node+16 if i+1<depth else 0
   for off,value in ((0,callback),(4,next_node),(8,arg),(12,0x80000060+i)):store(m,node+off,value)
   nodes.append(node)
 if entry in (PRIMARY,SECONDARY):store(m,0x8c893a80 if entry==PRIMARY else 0x8c893a88,NODES if c.get('depth',1) else 0)
 s.r[4]=c.get('event',0x360 if entry==ASIC else 0xa60)
 return m,s,f
