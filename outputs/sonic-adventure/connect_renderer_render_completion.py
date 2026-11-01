"""Connect explicit backend job acknowledgements without changing game C."""
import ast,hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;created=[]
def change(text,a,b):
 assert text.count(a)==1,(a,text.count(a));return text.replace(a,b)
def write(name,text):
 p=P/name;assert not p.exists();p.write_text(text)
 if p.suffix=='.py':ast.parse(text,filename=str(p))
 created.append(p)
text=(P/'source/renderer_frame_clock_probe.c').read_text();text='#include "render_completion.h"\n'+text
start=text.index('SA_STAGE_EXPORT int sa_renderer_input_tick_scheduled_observed(');extra=text[start:]
extra=change(extra,'sa_renderer_input_tick_scheduled_observed','sa_renderer_input_tick_render_scheduled_observed')
extra=change(extra,'struct sa_frame_clock_state *clock){','struct sa_frame_clock_state *clock,struct sa_render_completion *render,uint64_t backend_completed_job){')
marker=' return sa_renderer_input_tick_scheduled_connected('
extra=change(extra,marker,'''
 if(!render||!bus){s->pc=entry;return -6;}
 if(backend_completed_job){int rc=sa_render_completion_finish(render,backend_completed_job);if(rc){s->pc=entry;return rc;}}
 struct sa_render_completion_binding render_binding;struct sa_vertex_bus render_bus;
 int rc=sa_render_completion_bind(&render_binding,render,&decorated,&render_bus);if(rc){s->pc=entry;return rc;}
'''+marker)
extra=change(extra,'bus?&decorated:0,gbr','&render_bus,gbr')
write('source/renderer_render_completion_probe.c',text+'\n'+extra)
text=(P/'build_renderer_frame_clock.ps1').read_text().replace("'renderer_frame_clock_probe.c'","'renderer_render_completion_probe.c','render_completion.c'").replace('renderer_frame_clock.dll','renderer_render_completion.dll')
sources=re.findall(r"'([^']+\.c)'",text);assert len(sources)==len(set(sources))==291
write('build_renderer_render_completion.ps1',text)
text=(P/'renderer_frame_clock_reference.py').read_text().replace('renderer_frame_clock_reference_code import extend','renderer_render_completion_reference_code import extend').replace('build/renderer_frame_clock.dll','build/renderer_render_completion.dll')
marker='native_frame_clock=None'
text=change(text,marker,marker+'''
import render_completion_native as render_model
render_model.declare(effects.LIB)
native_render_completion=None
native_backend_completed_job=0
''')
marker='scheduled.argtypes=running.argtypes+[ctypes.POINTER(frame_model.State)];scheduled.restype=ctypes.c_int'
text=change(text,marker,marker+'''
render_scheduled=effects.LIB.sa_renderer_input_tick_render_scheduled_observed
render_scheduled.argtypes=scheduled.argtypes+[ctypes.POINTER(render_model.State),ctypes.c_uint64];render_scheduled.restype=ctypes.c_int
''')
text=change(text,'def observed(*args):','''def observed(*args):
 if native_render_completion is not None:
  assert runtime_attached and native_frame_clock is not None and not storage_attached
  return render_scheduled(*args,ctypes.byref(native_runtime),ctypes.byref(native_frame_clock),ctypes.byref(native_render_completion),native_backend_completed_job)
''')
write('renderer_render_completion_reference.py',text)
for suffix in ('_base','_state','_capture',''):
 text=(P/('write_watch_renderer_frame_clock'+suffix+'.py')).read_text().replace('write_watch_renderer_frame_clock','write_watch_renderer_render_completion').replace('import renderer_frame_clock_reference as old','import renderer_render_completion_reference as old')
 if not suffix:
  marker='namespace=dict(previous.namespace,'
  text=change(text,marker,'from renderer_render_completion_comparison import extend as render_extend,watched as render_watched,attach_pacing,render_fields\nwatched_reference=render_watched(watched_reference)\ncode=render_extend(code)\n'+marker)
  marker="frame_model=__import__('frame_clock_native'))"
  text=change(text,marker,"frame_model=__import__('frame_clock_native'),render_model=__import__('render_completion_native'),pacing_fields=render_fields)")
  marker="exec(compile(code,'TA_list_event_complete_state_comparison','exec'),namespace,local:={})"
  text=change(text,marker,"namespace['pacing_stores']=attach_pacing(namespace['namespace'])\n"+marker)
 write('write_watch_renderer_render_completion'+suffix+'.py',text)
report=dict(status='explicit_backend_render_completion_connected_pending_qualification',compiled_sources=291,old_C_inputs_preserved=289,new_original_routine_credit=0,backend_acknowledgement_is_explicit=True,pixel_rendering_implemented=False,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in created})
target=P/'analysis/renderer_render_completion_connection.json';assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n');print('Connected render completion variant,291 sources; verification pending')
