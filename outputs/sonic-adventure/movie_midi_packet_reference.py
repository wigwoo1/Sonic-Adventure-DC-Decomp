"""Original audio-mode/SDK instructions with retained full platform state."""
from movie_audio_controls_reference import *
import movie_audio_controls_reference as old

MP=P/'analysis/movie_midi_packet_translation.json';manifest=json.loads(MP.read_text())
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
effects.DLL=ROOT/'work/sonic-adventure/build/movie_midi_packet.dll';effects.LIB=ctypes.CDLL(str(effects.DLL))
console_model.declare(effects.LIB);flash_model.declare(effects.LIB)
native_exception=ExceptionState();native_runtime=runtime_model.State();runtime_attached=False
native_flash=flash_model.State();native_flash_bytes=None;storage_attached=False
ordinary=effects.LIB.sa_movie_midi_packet_exception_observed
ordinary.argtypes=old.ordinary.argtypes;ordinary.restype=ctypes.c_int
running=effects.LIB.sa_movie_midi_packet_running_observed
running.argtypes=old.running.argtypes;running.restype=ctypes.c_int
storage=effects.LIB.sa_movie_midi_packet_storage_observed
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

MIDI=0x8c643c18
DEST=0x8cd67000

def prepare(ram,c):
 if c['entry'] not in NEW:return old.prepare(ram,c)
 entry=c['entry'];fixture=Input(c|dict(entry=SDK))
 m,s,f=old.prepare(ram,fixture);c.update(fixture);c['entry']=entry;s.pc=entry
 s.r[15]=c.get('stack_address',s.r[15])
 if c.get('bad_stack'):s.r[15]=0x8c000000
 s.r[4]=c.get('object_pointer',INPUT if not c.get('null_object') else 0)
 s.r[5]=c.get('destination',DEST)
 m[(INPUT+1)&0xffffff]=c.get('object_byte',3)&255
 m[DEST&0xffffff:(DEST&0xffffff)+32]=bytes((i*19+7)&255 for i in range(32))
 sp=s.r[15]
 for address,value in ((sp,c.get('message',0x90)),(sp+4,c.get('data1',0x123456ab)),(sp+8,c.get('data2',0xffffff80))):
  if 0x8c000020<=address<=0x8cfffffc:store(m,address,value)
 store(m,0x8c67c1d8,c.get('trace_enabled',0));store(m,0x8c67c1dc,c.get('trace_callback',0x8c5ff2cc))
 for address,data in c.get('midi_patches',[]):m[address&0xffffff:(address&0xffffff)+len(data)]=bytes(data)
 return m,s,f
