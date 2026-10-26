"""Original flash-packet bodies with full machine state and reached platform stops."""
import collections,csv,ctypes,hashlib,itertools,json,sys,types
from pathlib import Path
import movie_sound_read_reference as old
import console_service_reference as console_model
import sound_ram_reference as sound_model
import verify_boot_device as seed
P=old.P;ROOT=old.ROOT;load=old.load;store=old.store
ENTRY,LOAD,POOL,ALLOC,ERROR,SET,CLEAR=(0x8c092fa0,0x8c093e60,0x8c65aa30,0x8c643ac0,0x8c65acac,0x8c5ff2cc,0x8c5ff2de)
CTX,OUT=0x8cd80000,0x8cda0000
MP=P/'analysis/movie_packet_startup_translation.json';manifest=json.loads(MP.read_text());NEW={int(r['address'],16) for r in manifest['routines']}
ROWS=dict(old.ROWS);PER_BODY=dict(old.PER_BODY)
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
namespace=dict(old.namespace,ROWS=ROWS,RETURNS=old.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=old.namespace['LEGACY']-NEW)
namespace['PACKET_INTERNAL_ENTRY_ROWS']={int(r['address'],16):set(PER_BODY[int(r['address'],16)]) for r in manifest['routines'] if r['discontiguous_before_entry_rows']}
from verify_sh4_exception_state import ExceptionState
import transfer_runtime_reference_cpu as context_cpu
import transfer_runtime_reference as runtime_model
EXCEPTION_KEYS=old.EXCEPTION_KEYS
namespace['reference']=old.namespace['reference']
native_exception=ExceptionState();native_runtime=runtime_model.State();runtime_attached=False
code=old.code
marker='  if pc==0x8c65a5dc:';assert code.count(marker)==1
addition='  if pc==0x8c64bddc:\n   completion_wait_counts[0x8c64be22]=completion_wait_counts[0x8c64bea0]=0\n  if pc in (0x8c64be22,0x8c64bea0) and sr&1:\n   completion_wait_counts[pc]=completion_wait_counts.get(pc,0)+1\n   if completion_wait_counts[pc]>=(3 if pc==0x8c64bea0 else 2):stop(pc,r,sr,-4)\n'
code=code.replace(marker,addition+marker)
marker='  if pc in tracked and not (';assert code.count(marker)==1
# Bottom-tested byte helpers enter at a test shared with an internal backedge.
# Native observers run at actual dispatch, not every revisit of that address.
code=code.replace(marker,'  if pc in tracked and not (pc in PACKET_INTERNAL_ENTRY_ROWS and len(path)>1 and path[-2] in PACKET_INTERNAL_ENTRY_ROWS[pc]) and not (')
exec(compile(code,'packet_startup_original_model','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')

effects=types.SimpleNamespace(**old.effects.__dict__);effects.CONNECTED=set(old.effects.CONNECTED)|NEW;effects.namespace=namespace;effects.reference_run=reference_run
effects.DLL=ROOT/'work/sonic-adventure/build/movie_packet_startup.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));console_model.declare(effects.LIB)
ordinary=effects.LIB.sa_movie_packet_startup_exception_observed;ordinary.argtypes=old.ordinary.argtypes;ordinary.restype=ctypes.c_int
running=effects.LIB.sa_movie_packet_startup_running_observed;running.argtypes=ordinary.argtypes+[ctypes.POINTER(runtime_model.State)];running.restype=ctypes.c_int
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
 old.bind_vectors(vbr);ROWS.update(old.ROWS);effects.CONNECTED.update(old.effects.CONNECTED)

PACKET,OPEN,FORMAT,READ,WRITE,POLL=(0x8c64bddc,0x8c604044,0x8c604148,0x8c6041d4,0x8c604210,0x8c604250)
PKTCTX,BUFFER,DATA,DEST=0x8cd40000,0x8cd42000,0x8cd48000,0x8cd4c000
GLOBAL=0x8c673450

def prepare(ram,c):
 if c['entry'] not in NEW:
  result=old.prepare(ram,c);effects.CONNECTED.update(old.effects.CONNECTED);return result
 context=Input(c|dict(entry=old.old.old.old.GENERIC,depth=0))
 m,s,f=old.prepare(ram,context);context['entry']=c['entry'];context.pop('context_return_pc',None);c.update(context);s.pc=c['entry']
 # Direct packet fixtures declare firmware vectors through the same authored
 # binding API as the earlier console tests; no game helper result is supplied.
 if c.get('console_info') or c.get('console_flash'):
  console,info,flash=console_model.configure(effects.LIB,c)
  assert effects.LIB.sa_console_bind_vectors(ctypes.byref(console),effects.RAM_TYPE.from_buffer(m))==0
 store(m,GLOBAL,PKTCTX if c.get('packet_context',True) else 0)
 fields={4:0x18000,8:0x2000,12:BUFFER,16:c.get('used',0),20:c.get('capacity',4),24:c.get('bitmap_blocks',1),28:c.get('operation',0),32:c.get('packet_status',0),36:c.get('retry',0),40:c.get('packet_index',0),44:0,48:c.get('callback',DEST)}
 for offset,value in fields.items():store(m,PKTCTX+offset,value)
 store(m,PKTCTX,c.get('partition',2)|(c.get('version',0)<<16))
 m[BUFFER&0xffffff:(BUFFER&0xffffff)+0x4000]=bytes([0xff])*0x4000
 m[DATA&0xffffff:(DATA&0xffffff)+0x4000]=bytes((i*29+17)&255 for i in range(0x4000))
 m[DEST&0xffffff:(DEST&0xffffff)+0x4000]=bytes([0x66])*0x4000
 for a,data in c.get('packet_patches',[]):m[a&0xffffff:(a&0xffffff)+len(data)]=bytes(data)
 s.r[4]=c.get('arg4',PKTCTX);s.r[5]=c.get('arg5',DEST);s.r[6]=c.get('arg6',64)
 for key,value in c.get('registers',{}).items():s.r[int(key)]=value
 effects.CONNECTED.update(old.effects.CONNECTED);return m,s,f
