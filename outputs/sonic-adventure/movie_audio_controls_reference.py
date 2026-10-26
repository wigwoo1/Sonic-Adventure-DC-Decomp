"""Original audio-mode/SDK instructions with retained full platform state."""
from movie_audio_mode_reference import *
import movie_audio_mode_reference as old

MP=P/'analysis/movie_audio_controls_translation.json';manifest=json.loads(MP.read_text())
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
effects.DLL=ROOT/'work/sonic-adventure/build/movie_audio_controls.dll';effects.LIB=ctypes.CDLL(str(effects.DLL))
console_model.declare(effects.LIB);flash_model.declare(effects.LIB)
native_exception=ExceptionState();native_runtime=runtime_model.State();runtime_attached=False
native_flash=flash_model.State();native_flash_bytes=None;storage_attached=False
ordinary=effects.LIB.sa_movie_audio_controls_exception_observed
ordinary.argtypes=old.ordinary.argtypes;ordinary.restype=ctypes.c_int
running=effects.LIB.sa_movie_audio_controls_running_observed
running.argtypes=old.running.argtypes;running.restype=ctypes.c_int
storage=effects.LIB.sa_movie_audio_controls_storage_observed
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

MODE,SDK,COPY,LOGGER=0x8c64387c,0x8c65a464,0x8c10cd44,0x8c65acac
MONO,STEREO=0x4f4e4f4d,0x4f525453
LIBRARY,QUEUE,INPUT,COMMAND=0x8cd60000,0x8cd62000,0x8cd66000,0x8c890600
ROOT_WORD=0x8c67c1d0

BLOCK,TRIPLE,PAIR=0x8c6436e8,0x8c643754,0x8c6437f0
BUFFERS={BLOCK:0x8c8905c4,TRIPLE:0x8c8905d8,PAIR:0x8c8905ec}
def prepare(ram,c):
 if c['entry'] not in NEW:return old.prepare(ram,c)
 entry=c['entry'];fixture=Input(c|dict(entry=SDK))
 m,s,f=old.prepare(ram,fixture);c.update(fixture);c['entry']=entry;s.pc=entry
 m[0x8905b0:0x890600]=bytes((i*23+11)&255 for i in range(0x50))
 m[(LIBRARY+0x1d2)&0xffffff]=c.get('version',39)&255
 s.r[4]=c.get('arg4',INPUT if entry==BLOCK else 1)&0xffffffff
 s.r[5]=c.get('arg5',2)&0xffffffff;s.r[6]=c.get('arg6',3)&0xffffffff
 for address,data in c.get('control_patches',[]):m[address&0xffffff:(address&0xffffff)+len(data)]=bytes(data)
 return m,s,f
