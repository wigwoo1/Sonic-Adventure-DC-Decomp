"""Original sound getters with complete CPU, interrupt and persistent device state."""
import collections,csv,ctypes,hashlib,itertools,json,sys,types
from pathlib import Path
import movie_transfer_runtime_reference as old
import console_service_reference as console_model
import sound_ram_reference as sound_model
import verify_boot_device as seed
P=old.P;ROOT=old.ROOT;load=old.load;store=old.store
ENTRY,LOAD,POOL,ALLOC,ERROR,SET,CLEAR=(0x8c092fa0,0x8c093e60,0x8c65aa30,0x8c643ac0,0x8c65acac,0x8c5ff2cc,0x8c5ff2de)
CTX,OUT=0x8cd80000,0x8cda0000
MP=P/'analysis/movie_sound_read_translation.json';manifest=json.loads(MP.read_text());NEW={int(r['address'],16) for r in manifest['routines']}
ROWS=dict(old.ROWS);PER_BODY=dict(old.PER_BODY)
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
namespace=dict(old.namespace,ROWS=ROWS,RETURNS=old.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=old.namespace['LEGACY']-NEW)
from verify_sh4_exception_state import ExceptionState
import transfer_runtime_reference_cpu as context_cpu
import transfer_runtime_reference as runtime_model
EXCEPTION_KEYS=old.EXCEPTION_KEYS
namespace['reference']=old.namespace['reference']
native_exception=ExceptionState();native_runtime=runtime_model.State();runtime_attached=False
code=old.code
marker='  if pc==0x8c65a6ec and not sr&1:';assert code.count(marker)==1
# The native function owns a fresh research counter on each actual entry.
# Earlier startup now reaches two sequential REQUEST invocations in one run.
code=code.replace(marker,'  if pc==0x8c65a5dc:completion_wait_counts[0x8c65a6ec]=0\n'+marker)
exec(compile(code,'sound_read_original_model','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')

effects=types.SimpleNamespace(**old.effects.__dict__);effects.CONNECTED=set(old.effects.CONNECTED)|NEW;effects.namespace=namespace;effects.reference_run=reference_run
effects.DLL=ROOT/'work/sonic-adventure/build/movie_sound_read.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));console_model.declare(effects.LIB)
ordinary=effects.LIB.sa_movie_sound_read_exception_observed;ordinary.argtypes=old.ordinary.argtypes;ordinary.restype=ctypes.c_int
running=effects.LIB.sa_movie_sound_read_running_observed;running.argtypes=ordinary.argtypes+[ctypes.POINTER(runtime_model.State)];running.restype=ctypes.c_int
def observed(*args):return running(*args,ctypes.byref(native_runtime)) if runtime_attached else ordinary(*args)
for name in ('sa_driver_signal_bus_init','sa_driver_signal_set_samples','sa_hardware_transfer_set_samples','sa_display_timing_set_samples','sa_disc_queue_init','sa_rtc_sample_bus_init','sa_rtc_set_samples'):
 getattr(effects.LIB,name).argtypes=getattr(old.effects.LIB,name).argtypes;getattr(effects.LIB,name).restype=getattr(old.effects.LIB,name).restype
effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32;native_stats=old.native_stats.__class__()
def invoke(*args):return observed(*args,ctypes.byref(effects.console_state),ctypes.byref(native_stats),0xffffffff,ctypes.byref(native_exception))
effects.fn=invoke
compare_code=old.compare_code
exec(compile(compare_code,'init_records_comparison','exec'),dict(old._legacy_template_compare.__globals__,effects=effects,namespace=namespace,native_stats=native_stats),local:={});_legacy_template_compare=local['compare']

Input=old.Input;seed_ram=old.seed_ram;sample=old.sample
VECTORS=(0x8c65e7a4,0x8c65e7d8);NORMAL,NESTED,EXIT=0x8c65e80c,0x8c65e958,0x8c65eaec
COUNTER=old.COUNTER;ASIC=old.ASIC;HANDLER=old.HANDLER;DESCRIPTOR=old.DESCRIPTOR;RECORD=old.RECORD;ROOT_CONTEXT=old.ROOT_CONTEXT

FIELD,BYTE,WORD=0x8c667150,0x8c667188,0x8c66910c
OUT=0x8cda0000
def bind_vectors(vbr):
 """Bind verification metadata only; original startup must copy all guest bytes."""
 for source,offsets in ((VECTORS[0],(0x100,0x400)),(VECTORS[1],(0x600,))):
  for offset in offsets:
   origin=vbr+offset;aliases={}
   for pc,text in PER_BODY[source].items():
    if text.startswith(('mov.l 0x','mov.w 0x','mova 0x')):text=__import__('re').sub(r'0x([0-9a-f]+)',lambda hit:hex(origin+int(hit[1],16)-source),text)
    aliases[origin+pc-source]=text
   ROWS.update(aliases);namespace['CONTEXT_ROWS'].update(aliases);effects.CONNECTED.add(origin)
def prepare(ram,c):
 if c['entry'] not in NEW:
  result=old.prepare(ram,c);effects.CONNECTED.update(old.effects.CONNECTED);return result
 context=Input(c|dict(entry=old.old.old.GENERIC,depth=0));m,s,f=old.prepare(ram,context);context['entry']=c['entry'];context.pop('context_return_pc',None);c.update(context);s.pc=c['entry']
 s.r[4]=c.get('address',32);s.r[5]=c.get('output',OUT)
 m[OUT&0xffffff:(OUT&0xffffff)+16]=bytes([0x66])*16
 effects.CONNECTED.update(old.effects.CONNECTED);return m,s,f
