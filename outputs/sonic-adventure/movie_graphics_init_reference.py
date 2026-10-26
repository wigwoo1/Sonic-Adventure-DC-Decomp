"""Original graphics initializer instructions with retained complete platform state."""
from movie_midi_commands_reference import *
import movie_midi_commands_reference as old

MP=P/'analysis/movie_graphics_init_translation.json';manifest=json.loads(MP.read_text())
NEW={int(r['address'],16) for r in manifest['routines']}
ROWS=dict(old.ROWS);PER_BODY=dict(old.PER_BODY)
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')}
 ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
namespace=dict(old.namespace,ROWS=ROWS,RETURNS=old.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=old.namespace['LEGACY']-NEW)
code=old.code
exec(compile(code,'audio_controls_original_model','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')
effects=types.SimpleNamespace(**old.effects.__dict__)
effects.CONNECTED=set(old.effects.CONNECTED)|NEW;effects.namespace=namespace;effects.reference_run=reference_run
effects.DLL=ROOT/'work/sonic-adventure/build/movie_graphics_init.dll';effects.LIB=ctypes.CDLL(str(effects.DLL))
console_model.declare(effects.LIB);flash_model.declare(effects.LIB)
native_exception=ExceptionState();native_runtime=runtime_model.State();runtime_attached=False
native_flash=flash_model.State();native_flash_bytes=None;storage_attached=False
ordinary=effects.LIB.sa_movie_graphics_init_exception_observed
ordinary.argtypes=old.ordinary.argtypes;ordinary.restype=ctypes.c_int
running=effects.LIB.sa_movie_graphics_init_running_observed
running.argtypes=old.running.argtypes;running.restype=ctypes.c_int
storage=effects.LIB.sa_movie_graphics_init_storage_observed
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
exec(compile(compare_code,'audio_mode_legacy_comparison','exec'),dict(old._legacy_template_compare.__globals__,effects=effects,namespace=namespace,native_stats=native_stats),local:={})
_legacy_template_compare=local['compare']
setup_flash=types.FunctionType(old.setup_flash.__code__,globals(),'setup_flash')
native_flash_snapshot=types.FunctionType(old.native_flash_snapshot.__code__,globals(),'native_flash_snapshot')

def bind_vectors(vbr):
 old.bind_vectors(vbr);ROWS.update(old.ROWS);effects.CONNECTED.update(old.effects.CONNECTED)

GRAPHICS_INIT_ENTRY=0x8c656940
GRAPHICS_CONFIG_BASE=0x8cd88000
GRAPHICS_CONFIG_FIXTURE_PATH=P/'analysis/graphics_init_input_fixture.json'
GRAPHICS_CONFIG_FIXTURE=json.loads(GRAPHICS_CONFIG_FIXTURE_PATH.read_text())

def prepare(ram,c):
 if c['entry'] not in NEW:return old.prepare(ram,c)
 entry=c['entry'];fixture=Input(c|dict(entry=SDK))
 m,s,f=old.prepare(ram,fixture);c.update(fixture);c['entry']=entry;s.pc=entry
 address=c.get('gfx_config_address',GRAPHICS_CONFIG_FIXTURE['observed_caller_arguments'][0])
 record=GRAPHICS_CONFIG_FIXTURE['descriptor_bytes']
 if address&0x1f000000==0x0c000000 and address&0xffffff<=0x1000000-len(record):m[address&0xffffff:(address&0xffffff)+len(record)]=bytes(record)
 for indirect in GRAPHICS_CONFIG_FIXTURE['indirect_records']:
  target=int(indirect['address'],16);data=indirect['bytes'];m[target&0xffffff:(target&0xffffff)+len(data)]=bytes(data)
 for offset,value in c.get('gfx_config_words',[]):
  target=(address+offset)&0xffffffff
  assert target&0x1f000000==0x0c000000 and target&0xffffff<=0xfffffc
  store(m,target,value&0xffffffff)
 arguments=c.get('gfx_arguments',[address,0x1ff,0,0] if entry==GRAPHICS_INIT_ENTRY else [0,0,0,0])
 for i,value in enumerate(arguments):
  if i<4:s.r[4+i]=value&0xffffffff
  else:store(m,s.r[15]+(i-4)*4,value&0xffffffff)
 for target,value in c.get('gfx_global_words',[]):store(m,target,value&0xffffffff)
 for target,data in c.get('gfx_memory_patches',[]):m[target&0xffffff:(target&0xffffff)+len(data)]=bytes(data)
 if c.get('gfx_observed_fpscr'):f.fpscr=GRAPHICS_CONFIG_FIXTURE['observed_fpscr']
 return m,s,f
