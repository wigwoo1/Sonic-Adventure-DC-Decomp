"""Original driver-control word update with full interrupt and device state."""
import collections,csv,ctypes,hashlib,itertools,json,sys,types
from pathlib import Path
import movie_cpu_transfer_reference as old
import console_service_reference as console_model
import sound_ram_reference as sound_model
import verify_boot_device as seed
P=old.P;ROOT=old.ROOT;load=old.load;store=old.store
ENTRY,LOAD,POOL,ALLOC,ERROR,SET,CLEAR=(0x8c092fa0,0x8c093e60,0x8c65aa30,0x8c643ac0,0x8c65acac,0x8c5ff2cc,0x8c5ff2de)
CTX,OUT=0x8cd80000,0x8cda0000
MP=P/'analysis/movie_transfer_interrupt_translation.json';manifest=json.loads(MP.read_text());NEW={int(r['address'],16) for r in manifest['routines']}
ROWS=dict(old.ROWS);PER_BODY=dict(old.PER_BODY)
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
namespace=dict(old.namespace,ROWS=ROWS,RETURNS=old.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=old.namespace['LEGACY']-NEW)
code=old.code
marker=' hooked={BIOS:firmware_hook}';assert code.count(marker)==1
code=code.replace(marker,marker+"\n # Unsupported P4 callback: stop at the reached service boundary before instruction fetch.\n def missing_p4_callback(m,regs,sr):\n  events.append(event(0xffffffff,regs,sr,state.pr));stop(0xffffffff,regs,sr,-2)\n hooked[0xffffffff]=missing_p4_callback")
exec(compile(code,'init_records_original_model','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')
effects=types.SimpleNamespace(**old.effects.__dict__);effects.CONNECTED=old.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=reference_run
effects.DLL=ROOT/'work/sonic-adventure/build/movie_transfer_interrupt.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));console_model.declare(effects.LIB)
observed=effects.LIB.sa_movie_transfer_interrupt_observed;observed.argtypes=old.observed.argtypes;observed.restype=ctypes.c_int
for name in ('sa_driver_signal_bus_init','sa_driver_signal_set_samples','sa_hardware_transfer_set_samples','sa_display_timing_set_samples','sa_disc_queue_init','sa_rtc_sample_bus_init','sa_rtc_set_samples'):
 getattr(effects.LIB,name).argtypes=getattr(old.effects.LIB,name).argtypes;getattr(effects.LIB,name).restype=getattr(old.effects.LIB,name).restype
effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32;native_stats=old.native_stats.__class__()
def invoke(*args):return observed(*args,ctypes.byref(effects.console_state),ctypes.byref(native_stats),0xffffffff)
effects.fn=invoke
compare_code=old.compare_code
exec(compile(compare_code,'init_records_comparison','exec'),dict(old._legacy_template_compare.__globals__,effects=effects,namespace=namespace,native_stats=native_stats),local:={});_legacy_template_compare=local['compare']

Input=old.Input;seed_ram=old.seed_ram;sample=old.sample
HANDLER=0x8c10ebe8
START=old.old.START;DESCRIPTOR=old.old.DESCRIPTOR;TABLE=old.old.TABLE;RECORD=old.RECORD;ROOT_CONTEXT=old.ROOT_CONTEXT

def prepare(ram,c):
 entry=c['entry'];context=Input(c|dict(entry=old.old.CALLBACK if c.get('handler_callback',0)==old.old.CALLBACK else START))
 m,s,f=old.old.prepare(ram,context);context['entry']=entry;c.update(context);s.pc=entry
 descriptor=c.get('descriptor',DESCRIPTOR)
 for off,value in ((0,c.get('channel',0)),(28,c.get('handler_callback',0)),(32,c.get('handler_argument',RECORD)),(44,c.get('active',1)),(48,c.get('completed',0))):store(m,DESCRIPTOR+off,value)
 store(m,ROOT_CONTEXT+0xec+c.get('channel',0)*4,c.get('completed_counter',0xffffffff)) if c.get('channel',0)<32 else None
 store(m,old.load(m,0x8c666e4c),c.get('error_counter',0))
 if c.get('ack_pointer') is not None:store(m,old.load(m,0x8c10ec3c),c['ack_pointer'])
 if entry==HANDLER:s.r[4]=descriptor
 return m,s,f
