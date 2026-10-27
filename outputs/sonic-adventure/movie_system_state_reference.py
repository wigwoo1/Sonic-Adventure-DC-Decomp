"""Original console settings/info queries with corrected SYSINFO status and complete platform state."""
from movie_float_initialize_reference import *
import movie_float_initialize_reference as old
import buffer_transfer_reference_cpu as buffer_cpu

# Authored firmware ABI correction. All old service data/state/counters and
# error-prefix behavior remain in the existing implementation.
console_model=types.SimpleNamespace(**old.console_model.__dict__)
class StatusConsoleReference(old.console_model.Reference):
 def service(self,m,channel,selector,first,second,count):
  status,result=super().service(m,channel,selector,first,second,count)
  return (0,0) if status==0 and channel==0 and selector==0 else (status,result)
console_model.Reference=StatusConsoleReference

MP=P/'analysis/movie_system_state_translation.json';manifest=json.loads(MP.read_text())
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
exec(compile(code,'render_buffer_original_model','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')
effects=types.SimpleNamespace(**old.effects.__dict__)
effects.CONNECTED=set(old.effects.CONNECTED)|NEW;effects.namespace=namespace;effects.reference_run=reference_run
effects.DLL=ROOT/'work/sonic-adventure/build/movie_system_state.dll';effects.LIB=ctypes.CDLL(str(effects.DLL))
console_model.declare(effects.LIB);flash_model.declare(effects.LIB)
native_exception=ExceptionState();native_runtime=runtime_model.State();runtime_attached=False
native_flash=flash_model.State();native_flash_bytes=None;storage_attached=False
ordinary=effects.LIB.sa_movie_system_state_exception_observed
ordinary.argtypes=old.ordinary.argtypes;ordinary.restype=ctypes.c_int
running=effects.LIB.sa_movie_system_state_running_observed
running.argtypes=old.running.argtypes;running.restype=ctypes.c_int
storage=effects.LIB.sa_movie_system_state_storage_observed
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
 entry=c['entry'];fixture=Input(c|dict(entry=PACKET,arg4=0,arg5=BUFFER))
 m,s,f=old.prepare(ram,fixture);c.update(fixture);c['entry']=entry;s.pc=entry
 for i,value in enumerate(c.get('system_arguments',[OUT,0,0,0])):s.r[4+i]=value&0xffffffff
 store(m,0x8c67340c,c.get('system_buffer',0x8cd42000))
 if c.get('sysinfo_guest_predicate') is not None:
  # Initial guest vector points to an unchanged, byte-checked game predicate.
  # Its original code executes on both sides; no host callback returns a value.
  store(m,0x8c0000b0,0x8c04fe40)
  destination=load(m,0x8c04fe8c);m[destination&0xffffff:(destination&0xffffff)+2]=(c['sysinfo_guest_predicate']&0xffff).to_bytes(2,'little')
 return m,s,f

INHERITED_INTEGER_CONSTANTS={name:value for name,value in vars(old).items() if name.isupper() and type(value) is int}
for name,value in INHERITED_INTEGER_CONSTANTS.items():assert globals().get(name)==value,(name,value,globals().get(name))

FLOAT_INITIALIZE=0x8c60edb0

SYSTEM_QUERY,SYSTEM_BYTE,SYSTEM_INFO,SYSTEM_SUBTRACT,SYSTEM_PREDICATE=0x8c5ff39c,0x8c5ff3fa,0x8c5ff494,0x8c5ff396,0x8c04fe40
