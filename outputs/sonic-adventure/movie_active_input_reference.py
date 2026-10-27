"""Original record initialization/status family with retained complete platform state."""
from movie_countdown_reference import *
import movie_countdown_reference as old
import buffer_transfer_reference_cpu as buffer_cpu

MP=P/'analysis/movie_active_input_translation.json';manifest=json.loads(MP.read_text())
NEW={int(r['address'],16) for r in manifest['routines']}
ROWS=dict(old.ROWS);PER_BODY=dict(old.PER_BODY)
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')}
 ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
namespace=dict(old.namespace,ROWS=ROWS,RETURNS=old.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=old.namespace['LEGACY']-NEW)
guards=dict(old.namespace['GUARDS']);guards[0x8c65100c]=(False,2)
entries=dict(old.namespace['GUARD_ENTRY']);entries[0x8c651000]=[0x8c65100c]
namespace.update(reference=buffer_cpu.reference,GUARDS=guards,GUARD_ENTRY=entries)
code=old.code
marker='if w==0xb and pr not in RETURNS|{entry_pr}:';assert code.count(marker)==1
code=code.replace(marker,'if w==0xb and pc!=0x8c11094c and pr not in RETURNS|{entry_pr}:')
marker='  if pc in GUARD_ENTRY:';assert code.count(marker)==1
code=code.replace(marker,"  if pc in AI_FPU_ROWS and (w>>12==0xf or w&0xf0ff in (0x006a,0x406a,0x405a)) and sr&0x8000:stop(pc,r,sr,-4)\n"+marker)
namespace['AI_FPU_ROWS']=set(PER_BODY[0x8c668e60])|set(PER_BODY[0x8c1108e0])
exec(compile(code,'render_buffer_original_model','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')
effects=types.SimpleNamespace(**old.effects.__dict__)
effects.CONNECTED=set(old.effects.CONNECTED)|NEW;effects.namespace=namespace;effects.reference_run=reference_run
effects.DLL=ROOT/'work/sonic-adventure/build/movie_active_input.dll';effects.LIB=ctypes.CDLL(str(effects.DLL))
console_model.declare(effects.LIB);flash_model.declare(effects.LIB)
native_exception=ExceptionState();native_runtime=runtime_model.State();runtime_attached=False
native_flash=flash_model.State();native_flash_bytes=None;storage_attached=False
ordinary=effects.LIB.sa_movie_active_input_exception_observed
ordinary.argtypes=old.ordinary.argtypes;ordinary.restype=ctypes.c_int
running=effects.LIB.sa_movie_active_input_running_observed
running.argtypes=old.running.argtypes;running.restype=ctypes.c_int
storage=effects.LIB.sa_movie_active_input_storage_observed
storage.argtypes=old.storage.argtypes;storage.restype=ctypes.c_int

def observed(*args):
 if storage_attached:
  assert runtime_attached
  return storage(*args,ctypes.byref(native_runtime),ctypes.byref(native_flash))
 return running(*args,ctypes.byref(native_runtime)) if runtime_attached else ordinary(*args)

for name in ('sa_driver_signal_bus_init','sa_driver_signal_set_samples','sa_hardware_transfer_set_samples','sa_display_timing_set_samples','sa_disc_queue_init','sa_rtc_sample_bus_init','sa_rtc_set_samples'):
 getattr(effects.LIB,name).argtypes=getattr(old.effects.LIB,name).argtypes
 getattr(effects.LIB,name).restype=getattr(old.effects.LIB,name).restype
effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32
native_stats=old.native_stats.__class__()

def invoke(*args):return observed(*args,ctypes.byref(effects.console_state),ctypes.byref(native_stats),0xffffffff,ctypes.byref(native_exception))
effects.fn=invoke;compare_code=old.compare_code
exec(compile(compare_code,'render_buffer_legacy_comparison','exec'),dict(old._legacy_template_compare.__globals__,effects=effects,namespace=namespace,native_stats=native_stats,console_model=console_model),local:={})
_legacy_template_compare=local['compare']
setup_flash=types.FunctionType(old.setup_flash.__code__,globals(),'setup_flash')
native_flash_snapshot=types.FunctionType(old.native_flash_snapshot.__code__,globals(),'native_flash_snapshot')

def bind_vectors(vbr):
 old.bind_vectors(vbr);ROWS.update(old.ROWS);effects.CONNECTED.update(old.effects.CONNECTED)

BUFFER_INITIALIZE,BUFFER_DMA,BUFFER_WAIT=0x8c65393a,0x8c6534e0,0x8c651000

def prepare(ram,c):
 if c['entry'] not in NEW:return old.prepare(ram,c)
 entry=c['entry'];requested=dict(c);fixture=Input(c|dict(entry=PACKET,arg4=0,arg5=BUFFER))
 m,s,f=old.prepare(ram,fixture);c.update(fixture);c.update(requested);c['entry']=entry;s.pc=entry
 for i,value in enumerate(c.get('active_input_arguments',[0,0,0,0])):s.r[4+i]=value&0xffffffff
 return m,s,f

INHERITED_INTEGER_CONSTANTS={name:value for name,value in vars(old).items() if name.isupper() and type(value) is int}
for name,value in INHERITED_INTEGER_CONSTANTS.items():assert globals().get(name)==value,(name,value,globals().get(name))

FLOAT_INITIALIZE=0x8c60edb0

SYSTEM_QUERY,SYSTEM_BYTE,SYSTEM_INFO,SYSTEM_SUBTRACT,SYSTEM_PREDICATE=0x8c5ff39c,0x8c5ff3fa,0x8c5ff494,0x8c5ff396,0x8c04fe40

SLOT_ROOT,SLOT_BATCH,SLOT_ONE,SLOT_STATUS,SLOT_TABLE,SLOT_CLEAR,SLOT_DECODE=0x8c109cfe,0x8c108808,0x8c1087d2,0x8c602ab4,0x8c602998,0x8c107ec0,0x8c646c8a

CD_TICK,CD_VARIANT,CD_AUDIO,CD_INPUT,CD_PUMP,CD_WORD1,CD_WORD2,CD_THUNK,CD_MASK,CD_ON,CD_OFF,CD_RECORD_CALLBACK,CD_NOP,CD_DEVICE_TAIL,CD_CHANNEL_STATUS,CD_STATUS_SET,CD_RECORD_INIT,CD_FPU_WRAPPER=0x8c051810,0x8c04f7e0,0x8c643f98,0x8c602cb8,0x8c65a378,0x8c10d428,0x8c10d444,0x8c647908,0x8c648234,0x8c602df8,0x8c602e34,0x8c647bb8,0x8c05180c,0x8c66085a,0x8c660dac,0x8c647cb4,0x8c647cc4,0x8c652554

AI_SCHEDULE,AI_CLEANUP,AI_RESTORE=0x8c668e60,0x8c668cca,0x8c1108e0
