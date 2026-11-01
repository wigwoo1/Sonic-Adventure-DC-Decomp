"""Add explicit host/idle scheduling without altering recovered game bodies."""
import ast,hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent
def replace(text,old,new,count=1):
 assert text.count(old)==count,(old,text.count(old),count)
 return text.replace(old,new)
def write(name,text):
 p=P/name;assert not p.exists();p.write_text(text)
 if p.suffix=='.py':ast.parse(text,filename=str(p))
 return p
created=[]
text=(P/'source/sa_renderer_input_tick_connected.c').read_text()
text=replace(text,'#include "flash_storage.h"','#include "frame_clock.h"\n#include "flash_storage.h"')
text=replace(text,'uint32_t root_pr;};','uint32_t root_pr;struct sa_frame_clock_binding *clock;};')
text=replace(text,'s->pr};int status=dispatch','s->pr,0};int status=dispatch',4)
start=text.index('static int runtime_poll_frame(');end=text.index('static int dispatch(void *opaque',start)
helper=text[start:end]
helper=replace(helper,'runtime_poll_frame(','scheduled_poll_frame(')
helper=replace(helper,'uint32_t *pr){','uint32_t *pr,uint32_t resume,int idle){')
helper=replace(helper,'||(*pr!=0x8c642060u&&*pr!=0x8c6420a0u)','')
helper=helper.replace('c->runtime->parent.read(c->runtime->parent.context','c->bus->read(c->bus->context')
helper=replace(helper,'if(!c->runtime->parent.read)','if(!c->bus||!c->bus->read)')
marker=' if(!(mask&(normal|c->runtime->state->pending)))return 0;'
helper=replace(helper,marker,'''
 if(idle&&!(mask&(normal|c->runtime->state->pending))){
  status=sa_frame_clock_next_in(c->clock->state,1u);if(status)return status;
  status=c->bus->read(c->bus->context,0xa05f6930u,4u,&mask);if(status)return status;
  status=c->bus->read(c->bus->context,0xa05f6900u,4u,&normal);if(status)return status;
 }
'''+marker)
helper=replace(helper,'uint32_t resume=*pr,pc=resume,old_stop=c->stop;','uint32_t pc=resume,old_stop=c->stop;')
text=text[:end]+helper+text[end:]
marker=' if(c->observe)c->observe(c->user,target,m,f,sr,r,pr);'
assert text.count(marker)==1
text=replace(text,marker,'''
 if(c->clock&&target==0x8c6050b0u&&(!c->runtime||!c->runtime->state->in_irq)){
  int scheduled=sa_frame_clock_next_in(c->clock->state,0u);if(scheduled)return scheduled;
  scheduled=scheduled_poll_frame(c,m,f,sr,r,pr,target,0);if(scheduled)return scheduled;
 }
'''+marker)
marker='return status?status:runtime_poll_frame(c,m,f,sr,r,pr);'
text=replace(text,marker,'return status?status:(c->clock&&(*pr==0x8c642060u||*pr==0x8c6420a0u)?scheduled_poll_frame(c,m,f,sr,r,pr,*pr,1):runtime_poll_frame(c,m,f,sr,r,pr));')
start=text.index('SA_STAGE_EXPORT int sa_renderer_input_tick_running_connected(')
end=text.index('\n/* Explicit mutable host storage;',start)
scheduled=text[start:end]
scheduled=replace(scheduled,'sa_renderer_input_tick_running_connected','sa_renderer_input_tick_scheduled_connected')
scheduled=replace(scheduled,'struct sa_transfer_runtime *runtime){','struct sa_transfer_runtime *runtime,struct sa_frame_clock_state *clock){')
marker=' exception->returned=0;'
scheduled=replace(scheduled,marker,'''
 if(!clock){s->pc=entry;return -6;}
 struct sa_frame_clock_binding clock_memory;struct sa_vertex_bus clock_bus;
 int clock_status=sa_frame_clock_bind(&clock_memory,clock,&runtime_bus,&clock_bus);if(clock_status){s->pc=entry;return clock_status;}
 clock_status=sa_frame_clock_begin_entry(clock);if(clock_status){s->pc=entry;return clock_status;}
'''+marker)
scheduled=replace(scheduled,'observe,&runtime_bus,gbr','observe,&clock_bus,gbr')
scheduled=replace(scheduled,'&runtime_memory,0,s->pr,0};','&runtime_memory,0,s->pr,&clock_memory};')
text+='\n/* Declared scheduler domain; no instruction-cost or physical board claim. */\n'+scheduled
created.append(write('source/sa_renderer_frame_clock_connected.c',text))
text=(P/'source/renderer_input_tick_probe.c').read_text()
text='#include "frame_clock.h"\n'+text
prototype=next(line for line in text.splitlines() if line.startswith('int sa_renderer_input_tick_running_connected('))
new=prototype.replace('running_connected','scheduled_connected').replace('struct sa_transfer_runtime*);','struct sa_transfer_runtime*,struct sa_frame_clock_state*);')
text=replace(text,prototype,prototype+'\n'+new)
start=text.index('SA_STAGE_EXPORT int sa_renderer_input_tick_running_observed(');end=text.index('\nSA_STAGE_EXPORT int sa_renderer_input_tick_storage_observed',start)
scheduled=text[start:end].replace('running_observed','scheduled_observed').replace('running_connected','scheduled_connected')
scheduled=replace(scheduled,'struct sa_transfer_runtime *runtime){','struct sa_transfer_runtime *runtime,struct sa_frame_clock_state *clock){')
scheduled=replace(scheduled,'exception,runtime);','exception,runtime,clock);')
text+='\n'+scheduled
created.append(write('source/renderer_frame_clock_probe.c',text))
text=(P/'build_renderer_input_tick_mmio.ps1').read_text().replace("'sa_renderer_input_tick_connected.c'","'sa_renderer_frame_clock_connected.c','tmu0_clock.c','frame_clock.c'").replace("'renderer_input_tick_probe.c'","'renderer_frame_clock_probe.c'").replace('renderer_input_tick_mmio.dll','renderer_frame_clock.dll')
sources=re.findall(r"'([^']+\.c)'",text);assert len(sources)==len(set(sources))==290
created.append(write('build_renderer_frame_clock.ps1',text))
text=(P/'frame_signal_reference_cpu.py').read_text()
marker='namespace=dict(old.namespace);';assert text.count(marker)==1
text=replace(text,marker,"code=code.replace(\"if runtime_boundary is not None and (\",\"if runtime_boundary is not None and ((pc==0x8c6050b0 and getattr(mmio,'frame_clock',None) is not None) or \" )\n"+marker)
created.append(write('frame_clock_reference_cpu.py',text))
text=(P/'renderer_input_tick_mmio_reference.py').read_text().replace('import frame_signal_reference_cpu as buffer_cpu','import frame_clock_reference_cpu as buffer_cpu').replace('build/renderer_input_tick_mmio.dll','build/renderer_frame_clock.dll')
text=replace(text,'code=old.code',"from renderer_frame_clock_reference_code import extend\ncode=extend(old.code)")
marker='storage.argtypes=old.storage.argtypes;storage.restype=ctypes.c_int'
text=replace(text,marker,marker+'''
import frame_clock_native as frame_model
frame_model.declare(effects.LIB)
native_frame_clock=None
scheduled=effects.LIB.sa_renderer_input_tick_scheduled_observed
scheduled.argtypes=running.argtypes+[ctypes.POINTER(frame_model.State)];scheduled.restype=ctypes.c_int
''')
marker='def observed(*args):'
text=replace(text,marker,marker+'''
 if native_frame_clock is not None:
  assert runtime_attached and not storage_attached
  return scheduled(*args,ctypes.byref(native_runtime),ctypes.byref(native_frame_clock))
''')
created.append(write('renderer_frame_clock_reference.py',text))
for suffix in ('_base','_state','_capture',''):
 text=(P/('write_watch_renderer_input_tick_mmio'+suffix+'.py')).read_text().replace('write_watch_renderer_input_tick_mmio','write_watch_renderer_frame_clock').replace('import renderer_input_tick_mmio_reference as old','import renderer_frame_clock_reference as old')
 if not suffix:
  text=text.replace("code=previous.code","from renderer_frame_clock_comparison import extend,watched\nwatched_reference=watched(watched_reference)\ncode=extend(previous.code)")
  text=replace(text,'EventsState=EventsState)','EventsState=EventsState,frame_model=__import__(\'frame_clock_native\'))')
 created.append(write('write_watch_renderer_frame_clock'+suffix+'.py',text))
report=dict(status='frame_clock_connection_generated_pending_compilation_and_scene_qualification',compiled_sources=len(sources),prior_shared_c_inputs=286,new_original_routine_credit=0,wait_guards_changed=False,source_hashes={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in created})
target=P/'analysis/renderer_frame_clock_connection.json';assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n');print('Generated frame-clock variant: 290 sources; qualification pending',flush=True)
