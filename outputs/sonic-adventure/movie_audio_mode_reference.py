"""Original audio-mode/SDK instructions with retained full platform state."""
from movie_packet_flash_reference import *
import movie_packet_flash_reference as old

MP=P/'analysis/movie_audio_mode_translation.json';manifest=json.loads(MP.read_text())
NEW={int(r['address'],16) for r in manifest['routines']}
ROWS=dict(old.ROWS);PER_BODY=dict(old.PER_BODY)
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')}
 ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
namespace=dict(old.namespace,ROWS=ROWS,RETURNS=old.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=old.namespace['LEGACY']-NEW)
code=old.code
marker='pc in (0x8c10cd7a,0x8c10cd8e)';assert code.count(marker)==1
code=code.replace(marker,'pc in (0x8c10cd7a,0x8c10cd82,0x8c10cd8e)')
exec(compile(code,'audio_mode_original_model','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')
effects=types.SimpleNamespace(**old.effects.__dict__)
effects.CONNECTED=set(old.effects.CONNECTED)|NEW;effects.namespace=namespace;effects.reference_run=reference_run
effects.DLL=ROOT/'work/sonic-adventure/build/movie_audio_mode.dll';effects.LIB=ctypes.CDLL(str(effects.DLL))
console_model.declare(effects.LIB);flash_model.declare(effects.LIB)
native_exception=ExceptionState();native_runtime=runtime_model.State();runtime_attached=False
native_flash=flash_model.State();native_flash_bytes=None;storage_attached=False
ordinary=effects.LIB.sa_movie_audio_mode_exception_observed
ordinary.argtypes=old.ordinary.argtypes;ordinary.restype=ctypes.c_int
running=effects.LIB.sa_movie_audio_mode_running_observed
running.argtypes=old.running.argtypes;running.restype=ctypes.c_int
storage=effects.LIB.sa_movie_audio_mode_storage_observed
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

def prepare(ram,c):
 if c['entry'] not in NEW:return old.prepare(ram,c)
 context=Input(c|dict(entry=0x8c645afa,depth=0))
 m,s,f=old.prepare(ram,context);context['entry']=c['entry'];context.pop('context_return_pc',None);c.update(context);s.pc=c['entry']
 m[LIBRARY&0xffffff:(LIBRARY&0xffffff)+0x1000]=bytes(0x1000)
 m[(QUEUE-16)&0xffffff:(QUEUE&0xffffff)+0x1c*256+16]=bytes([0x5a])*(0x1c*256+32)
 m[INPUT&0xffffff:(INPUT&0xffffff)+64]=bytes((i*17+9)&255 for i in range(64))
 m[COMMAND&0xffffff:(COMMAND&0xffffff)+32]=bytes((i*13+7)&255 for i in range(32))
 store(m,ROOT_WORD,c.get('library_pointer',LIBRARY if c.get('enabled',True) else 0))
 for offset,value in ((12,c.get('queue_pointer',QUEUE)),(16,c.get('ring_capacity',256)),(20,c.get('queued',0)),(24,c.get('tail',0))):store(m,LIBRARY+0x158+offset,value)
 for literal,value in ((0x8c65acb8,0x12345678),(0x8c65acbc,0x87654321)):store(m,load(m,literal),value)
 s.r[4]=c.get('audio_mode',STEREO) if c['entry']==MODE else c.get('source',INPUT)
 s.sr=(s.sr&~0xf3)|((c.get('imask',0)&15)<<4)|(c.get('ts',0)&3)
 if c.get('unprivileged'):s.sr&=~0x40000000
 if c.get('bad_stack'):s.r[15]=0x8c000000
 for address,data in c.get('audio_patches',[]):m[address&0xffffff:(address&0xffffff)+len(data)]=bytes(data)
 return m,s,f
