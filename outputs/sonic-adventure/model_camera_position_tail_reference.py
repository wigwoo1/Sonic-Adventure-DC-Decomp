"""Original I/O/readiness with unchanged flat instruction execution and guards."""
import sys as _host_sys
# Retained stages form a finite import chain. Allow its host-side startup;
# this does not alter guest execution, recursion, step limits or bus guards.
HOST_IMPORT_RECURSION_FLOOR=4096
_host_sys.setrecursionlimit(max(_host_sys.getrecursionlimit(),HOST_IMPORT_RECURSION_FLOOR))
from renderer_filesystem_irq_reference import *
import renderer_filesystem_irq_reference as old
import frame_clock_reference_cpu as buffer_cpu

MP=P/'analysis/renderer_input_tick_translation.json';manifest=json.loads(MP.read_text())
NEW={int(r['address'],16) for r in manifest['routines']}
ROWS=dict(old.ROWS);PER_BODY=dict(old.PER_BODY)
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')}
 ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
EXTRA_MANIFEST=P/'analysis/model_view_producer_translation.json'
extra=json.loads(EXTRA_MANIFEST.read_text())
NEW|={int(r['address'],16) for r in extra['routines']}
for routine in extra['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')}
 ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
SCHEDULER_LISTING=P/'analysis/main-dispatch-context-callback/8c09878c_instructions.tsv'
body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(SCHEDULER_LISTING.open(),delimiter='\t')}
ROWS.update(body);PER_BODY[0x8c09878c]=body;NEW.add(0x8c09878c)
CAMERA_MANIFEST=P/'analysis/model_camera_owner_v3_translation.json'
for routine in json.loads(CAMERA_MANIFEST.read_text())['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')}
 ROWS.update(body);PER_BODY[int(routine['address'],16)]=body;NEW.add(int(routine['address'],16))
POSITION_MANIFEST=P/'analysis/model_camera_position_v2_translation.json'
for routine in json.loads(POSITION_MANIFEST.read_text())['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')}
 ROWS.update(body);PER_BODY[int(routine['address'],16)]=body;NEW.add(int(routine['address'],16))
UPDATE_MANIFEST=P/'analysis/model_camera_update_v2_translation.json'
for routine in json.loads(UPDATE_MANIFEST.read_text())['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')}
 ROWS.update(body);PER_BODY[int(routine['address'],16)]=body;NEW.add(int(routine['address'],16))
FINAL_MANIFEST=P/'analysis/model_camera_final_translation.json'
for routine in json.loads(FINAL_MANIFEST.read_text())['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')}
 ROWS.update(body);PER_BODY[int(routine['address'],16)]=body;NEW.add(int(routine['address'],16))
COPY_MANIFEST=P/'analysis/model_camera_copy_v2_translation.json'
copy_manifest=json.loads(COPY_MANIFEST.read_text())
for routine in copy_manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')}
 ROWS.update(body)
 for entry in copy_manifest['public_entries']:
  PER_BODY[int(entry['address'],16)]=body;NEW.add(int(entry['address'],16))
copy_internal={k:set(v) for k,v in old.namespace['PACKET_INTERNAL_ENTRY_ROWS'].items()}
for entry,predecessors in copy_manifest['internal_entry_predecessors'].items():
 copy_internal.setdefault(int(entry,16),set()).update(int(a,16) for a in predecessors)
SCENE_MANIFEST=P/'analysis/model_camera_scene_translation.json'
for routine in json.loads(SCENE_MANIFEST.read_text())['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')}
 ROWS.update(body);PER_BODY[int(routine['address'],16)]=body;NEW.add(int(routine['address'],16))
LIGHTING_MANIFEST=P/'analysis/model_camera_lighting_load_translation.json'
for routine in json.loads(LIGHTING_MANIFEST.read_text())['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')}
 ROWS.update(body);PER_BODY[int(routine['address'],16)]=body;NEW.add(int(routine['address'],16))
DRAW_MANIFEST=P/'analysis/model_camera_draw_translation.json'
for routine in json.loads(DRAW_MANIFEST.read_text())['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')}
 ROWS.update(body);PER_BODY[int(routine['address'],16)]=body;NEW.add(int(routine['address'],16))
DRAW_UPDATE_MANIFEST=P/'analysis/model_camera_draw_update_translation.json'
for routine in json.loads(DRAW_UPDATE_MANIFEST.read_text())['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')}
 ROWS.update(body);PER_BODY[int(routine['address'],16)]=body;NEW.add(int(routine['address'],16))
HISTORY_MANIFEST=P/'analysis/model_camera_history_vectors_translation.json'
for routine in json.loads(HISTORY_MANIFEST.read_text())['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')}
 ROWS.update(body);PER_BODY[int(routine['address'],16)]=body;NEW.add(int(routine['address'],16))
STATE2_MANIFEST=P/'analysis/model_camera_state2_translation.json'
for routine in json.loads(STATE2_MANIFEST.read_text())['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')}
 ROWS.update(body);PER_BODY[int(routine['address'],16)]=body;NEW.add(int(routine['address'],16))
POSITION_TAIL_MANIFEST=P/'analysis/model_camera_position_tail_translation.json'
for routine in json.loads(POSITION_TAIL_MANIFEST.read_text())['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')}
 ROWS.update(body);PER_BODY[int(routine['address'],16)]=body;NEW.add(int(routine['address'],16))
namespace=dict(old.namespace,ROWS=ROWS,RETURNS=old.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=old.namespace['LEGACY']-NEW,PACKET_INTERNAL_ENTRY_ROWS=copy_internal)


guards=dict(old.namespace['GUARDS']);guards[0x8c65100c]=(False,2)
entries=dict(old.namespace['GUARD_ENTRY']);entries[0x8c651000]=[0x8c65100c]
namespace.update(reference=buffer_cpu.reference,GUARDS=guards,GUARD_ENTRY=entries)
from renderer_ta_list_reuse_reference_code import extend
from renderer_pixel_jobs_state import observation as pixel_job_observation
namespace['pixel_job_observation']=pixel_job_observation
code=extend(old.code)
exec(compile(code,'render_buffer_original_model','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')
effects=types.SimpleNamespace(**old.effects.__dict__)
effects.CONNECTED=set(old.effects.CONNECTED)|NEW;effects.namespace=namespace;effects.reference_run=reference_run
effects.DLL=ROOT/'work/sonic-adventure/build/model_camera_position_tail.dll';effects.LIB=ctypes.CDLL(str(effects.DLL))
console_model.declare(effects.LIB);flash_model.declare(effects.LIB)
native_exception=ExceptionState();native_runtime=runtime_model.State();runtime_attached=False
native_flash=flash_model.State();native_flash_bytes=None;storage_attached=False
ordinary=effects.LIB.sa_renderer_input_tick_exception_observed
ordinary.argtypes=old.ordinary.argtypes;ordinary.restype=ctypes.c_int
running=effects.LIB.sa_renderer_input_tick_running_observed
running.argtypes=old.running.argtypes;running.restype=ctypes.c_int
storage=effects.LIB.sa_renderer_input_tick_storage_observed
storage.argtypes=old.storage.argtypes;storage.restype=ctypes.c_int
import frame_clock_native as frame_model
frame_model.declare(effects.LIB)
native_frame_clock=None
import render_completion_native as render_model
render_model.declare(effects.LIB)
native_render_completion=None
native_backend_completed_job=0
import stream_pixel_jobs_native as job_model
native_pixel_job=None
native_pixel_digests=None
native_pixel_vram=None
native_pixel_frame=None
native_pixel_pixels=None
native_pixel_depth=None
native_pixel_options=None
native_pixel_scratch=None

scheduled=effects.LIB.sa_renderer_input_tick_scheduled_observed
scheduled.argtypes=running.argtypes+[ctypes.POINTER(frame_model.State)];scheduled.restype=ctypes.c_int
render_scheduled=effects.LIB.sa_renderer_input_tick_render_scheduled_observed
render_scheduled.argtypes=scheduled.argtypes+[ctypes.POINTER(render_model.State),ctypes.c_uint64];render_scheduled.restype=ctypes.c_int



def pixel_bus_pointer(bus):
 target=render_scheduled.argtypes[8]
 left=target._type_;right=job_model.Bus
 assert ctypes.sizeof(left)==ctypes.sizeof(right)==32
 assert [name for name,_ in left._fields_]==[name for name,_ in right._fields_]
 for (name,a),(_,b) in zip(left._fields_,right._fields_):
  assert getattr(left,name).offset==getattr(right,name).offset and ctypes.sizeof(a)==ctypes.sizeof(b)
  assert getattr(a,'_argtypes_',None)==getattr(b,'_argtypes_',None)
  assert getattr(a,'_restype_',None)==getattr(b,'_restype_',None)
 return ctypes.cast(ctypes.byref(bus),target)

def observed(*args):
 global native_pixel_digests
 if native_pixel_job is not None:
  assert runtime_attached and native_frame_clock is not None and native_render_completion is not None and not storage_attached
  assert native_backend_completed_job==0,'no externally supplied completion token'
  rc=job_model.LIB.sa_stream_banked_pixel_job_execute(ctypes.byref(native_pixel_job),ctypes.byref(native_render_completion),ctypes.cast(native_pixel_vram,ctypes.POINTER(job_model.B)),ctypes.sizeof(native_pixel_vram),native_pixel_scratch,len(native_pixel_scratch),native_pixel_pixels,native_pixel_depth,len(native_pixel_pixels),ctypes.byref(native_pixel_options))
  if rc:return rc
  native_pixel_digests=job_model.image_digests(bytes(native_pixel_pixels),bytes(native_pixel_depth))
  binding=job_model.Binding();capture=job_model.Bus();parent=ctypes.cast(args[8],ctypes.POINTER(job_model.Bus)).contents
  assert job_model.LIB.sa_stream_pixel_job_bind(ctypes.byref(binding),ctypes.byref(native_pixel_job),ctypes.byref(native_render_completion),native_pixel_frame,ctypes.byref(parent),ctypes.byref(capture))==0
  args=list(args);args[8]=pixel_bus_pointer(capture)
  return render_scheduled(*args,ctypes.byref(native_runtime),ctypes.byref(native_frame_clock),ctypes.byref(native_render_completion),0)

 if native_render_completion is not None:
  assert runtime_attached and native_frame_clock is not None and not storage_attached
  return render_scheduled(*args,ctypes.byref(native_runtime),ctypes.byref(native_frame_clock),ctypes.byref(native_render_completion),native_backend_completed_job)

 if native_frame_clock is not None:
  assert runtime_attached and not storage_attached
  return scheduled(*args,ctypes.byref(native_runtime),ctypes.byref(native_frame_clock))

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
 for i,value in enumerate(c.get('renderer_context_owner_arguments',[0,0,0,0])):s.r[4+i]=value&0xffffffff
 return m,s,f

INHERITED_INTEGER_CONSTANTS={name:value for name,value in vars(old).items() if name.isupper() and type(value) is int}
for name,value in INHERITED_INTEGER_CONSTANTS.items():assert globals().get(name)==value,(name,value,globals().get(name))

FLOAT_INITIALIZE=0x8c60edb0

SYSTEM_QUERY,SYSTEM_BYTE,SYSTEM_INFO,SYSTEM_SUBTRACT,SYSTEM_PREDICATE=0x8c5ff39c,0x8c5ff3fa,0x8c5ff494,0x8c5ff396,0x8c04fe40

SLOT_ROOT,SLOT_BATCH,SLOT_ONE,SLOT_STATUS,SLOT_TABLE,SLOT_CLEAR,SLOT_DECODE=0x8c109cfe,0x8c108808,0x8c1087d2,0x8c602ab4,0x8c602998,0x8c107ec0,0x8c646c8a

CD_TICK,CD_VARIANT,CD_AUDIO,CD_INPUT,CD_PUMP,CD_WORD1,CD_WORD2,CD_THUNK,CD_MASK,CD_ON,CD_OFF,CD_RECORD_CALLBACK,CD_NOP,CD_DEVICE_TAIL,CD_CHANNEL_STATUS,CD_STATUS_SET,CD_RECORD_INIT,CD_FPU_WRAPPER=0x8c051810,0x8c04f7e0,0x8c643f98,0x8c602cb8,0x8c65a378,0x8c10d428,0x8c10d444,0x8c647908,0x8c648234,0x8c602df8,0x8c602e34,0x8c647bb8,0x8c05180c,0x8c66085a,0x8c660dac,0x8c647cb4,0x8c647cc4,0x8c652554

AI_SCHEDULE,AI_CLEANUP,AI_RESTORE=0x8c668e60,0x8c668cca,0x8c1108e0

OWNER_ALL,OWNER_ONE=0x8c668cec,0x8c668d18


ANALYSIS_ROUTINES=manifest['routines']
