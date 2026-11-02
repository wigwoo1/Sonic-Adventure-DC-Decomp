"""Native guest-strobed reuse through the real frame/event/pixel bus chain."""
import ctypes,hashlib,json,struct,types
from pathlib import Path
import ta_list_lifecycle_native as life
from ta_list_lifecycle_reference import Lifecycle
import frame_events_eol_reference as events
import pixel_banked_native as jobs
import pvr_pixels_native as pixels
import render_completion_native as render
from render_completion_reference import Completion
from pixel_jobs_reference import Jobs
import write_watch_renderer_banked_pixel_jobs as abi
P=Path(__file__).resolve().parent;R=P.parents[1];Bus=render.Bus;Binding=life.declare(Bus);Events=events.state_type(Bus);game=abi.namespace['effects'].LIB
events.declare(game,Bus,Events)
game.sa_frame_command_bus_init.argtypes=[ctypes.c_void_p,ctypes.POINTER(jobs.B),ctypes.POINTER(Bus)];game.sa_frame_command_bus_init.restype=None
def float_word(value):return int.from_bytes(struct.pack('<f',value),'little')
QUAD=[0x82800028,7<<29,(1<<29)|(2<<22)|(1<<20),0xffffffff,*([float_word(1)]*4)]
for index,(x,y) in enumerate(((1,1),(1,7),(7,1),(7,7))):QUAD += [0xf0000000 if index==3 else 0xe0000000,float_word(x),float_word(y),float_word(1),0,0,float_word(1),0]
PACKETS=[tuple(QUAD[k:k+8]) for k in range(0,len(QUAD),8)]+[(0,)*8]

class Fixture:
 def __init__(self,enabled=True):
  self.memory=abi.namespace['RtcMemory']();self.frame=self.memory.frame;self.ram=(jobs.B*0x1000000)();self.base=Bus();game.sa_frame_command_bus_init(ctypes.byref(self.frame),self.ram,ctypes.byref(self.base));self.frame.graphics.vertex.qacr[0]=16
  @render.READ
  def hardware_read(ctx,a,n,out):
   if a&0x1fffffff==0x5f6900:
    if n!=4:return -3
    out[0]=0;return 0
   return self.base.read(self.base.context,a,n,out)
  @render.WRITE
  def hardware_write(ctx,a,value,n):
   if a&0x1fffffff==0x5f6900:return 0 if n==4 else -3
   return self.base.write(self.base.context,a,value,n)
  @render.CACHE
  def hardware_cache(ctx,a,op):return self.base.cache(self.base.context,a,op)
  self.hardware=Bus(None,hardware_read,hardware_write,hardware_cache);self.hardware_callbacks=(hardware_read,hardware_write,hardware_cache)
  self.pending=jobs.U();self.event_state=Events();self.event_bus=Bus();assert game.sa_frame_events_bus_init(ctypes.byref(self.event_state),ctypes.byref(self.frame),ctypes.byref(self.hardware),ctypes.byref(self.event_bus),ctypes.byref(self.pending),None,1)==0
  self.fail=0;self.parent_calls=0
  @render.READ
  def read(ctx,a,n,out):return self.event_bus.read(self.event_bus.context,a,n,out)
  @render.WRITE
  def write(ctx,a,value,n):
   self.parent_calls+=1
   return self.fail or self.event_bus.write(self.event_bus.context,a,value,n)
  @render.CACHE
  def cache(ctx,a,op):return self.fail or self.event_bus.cache(self.event_bus.context,a,op)
  self.proxy=Bus(None,read,write,cache);self.callbacks=(read,write,cache);self.state=life.restore(enabled=enabled);self.job=jobs.State();jobs.LIB.sa_pixel_job_init(ctypes.byref(self.job))
  params=[0]*16;params[7]=params[8]=7<<16;self.render=render.initialized(params,lib=jobs.LIB)
  self.binding=Binding();self.lifecycle_bus=Bus();assert life.LIB.sa_ta_list_lifecycle_bind(ctypes.byref(self.binding),ctypes.byref(self.state),ctypes.byref(self.frame),ctypes.byref(self.event_state),ctypes.byref(self.job),ctypes.byref(self.proxy),ctypes.byref(self.lifecycle_bus))==0
  self.capture=Bus();self.job_binding=jobs.Binding();assert jobs.LIB.sa_pixel_job_bind(ctypes.byref(self.job_binding),ctypes.byref(self.job),ctypes.byref(self.render),ctypes.byref(self.frame),ctypes.byref(self.lifecycle_bus),ctypes.byref(self.capture))==0
  self.outer=Bus();self.render_binding=render.Binding();assert jobs.LIB.sa_render_completion_bind(ctypes.byref(self.render_binding),ctypes.byref(self.render),ctypes.byref(self.capture),ctypes.byref(self.outer))==0
  self.ref=Lifecycle(enabled=enabled);self.ref_events=events.Reference();self.ref_runtime=types.SimpleNamespace(pending=0,ta_packet=None);self.ref_render=Completion.restore(render.serialize(self.render))
  seed=jobs.initial(8,8,False);self.ref_job=Jobs(jobs.Seed(seed.state,seed.pixels,seed.depth,(8,8,8,8,0,0,3,128)));self.packets=[];self.checks=0
  self.colour=(jobs.U*64).from_buffer_copy(seed.pixels);self.depth=(jobs.U*64)();self.options=pixels.Options(8,8,8,8,0,0,3,128)
 def check(self,label):
  assert life.snapshot(self.state)==self.ref.snapshot(),('lifecycle',label)
  assert life.observation(self.state)==self.ref.observation(),('lifecycle observation',label)
  assert events.snapshot(self.event_state)==self.ref_events.snapshot(),('events',label)
  assert jobs.snapshot(self.job)==self.ref_job.snapshot(),('immutable job',label)
  assert render.snapshot(self.render)==self.ref_render.snapshot(),('render',label)
  assert self.pending.value==self.ref_runtime.pending,('pending',label)
  native=[(p.destination,p.queue,tuple(p.words)) for p in self.frame.packets[:self.frame.packet_count]];assert native==self.packets,('current collector',label)
  self.checks+=1
 def write(self,a,value,n=4):
  expected=self.ref_render.check(a,value,n)
  if expected==0:expected=self.ref.check(a,value,n,self.packets,self.frame.packet_capacity,self.ref_events,self.ref_job)
  if expected==0:expected=self.fail
  actual=self.outer.write(self.outer.context,a,value,n);assert actual==expected,('write result',hex(a),actual,expected)
  if expected==0:
   if a&0x1fffffff==0x5f6900:self.ref_events.acknowledge(value,self.ref_runtime)
   assert self.ref_events.consume(self.packets,self.ref_runtime)==0
   self.ref.committed(a,value,n,self.packets,self.ref_events,self.ref_job,self.ref_runtime)
   self.ref_job.committed(a,value,n,self.ref_render,self.packets);self.ref_render.committed(a,value,n)
  self.check(hex(a));return actual
 def packet(self,words):
  self.frame.graphics.vertex.queue[:8]=words
  expected=self.fail or (-4 if len(self.packets)>=min(self.frame.packet_capacity,64) else 0)
  actual=self.outer.cache(self.outer.context,0xe0000000,1);assert actual==expected,('packet result',actual,expected)
  if expected==0:self.packets.append((0x10000000,0,tuple(words)));assert self.ref_events.consume(self.packets,self.ref_runtime)==0
  self.check('packet');return actual
 def execute(self):
  if self.job.ready and self.render.enabled:self.colour[:]=[0xff101018]*64;self.depth[:]=[0]*64
  actual=jobs.LIB.sa_banked_pixel_job_execute(ctypes.byref(self.job),ctypes.byref(self.render),None,0,None,0,self.colour,self.depth,64,ctypes.byref(self.options))
  expected=self.ref_job.execute(self.ref_render,None);assert actual==expected==0
  assert bytes(self.colour)==self.ref_job.pixels and bytes(self.depth)==self.ref_job.depth
  self.check('pixel work')

def main():
 target=P/'analysis/ta_list_lifecycle_verification.json';assert not target.exists();f=Fixture()
 # A submitted job survives reset/list-init and writes into the next collector.
 for words in PACKETS:f.packet(words)
 f.write(0xa05f8014,0xffffffff);immutable=tuple(f.job.words);job_id=f.job.job_id
 f.write(0xa05f8008,1);assert f.job.ready and tuple(f.job.words)==immutable
 f.write(0xa05f8124,0x155780);f.write(0xa05f8128,0);f.write(0xa05f8144,0x80000000)
 assert f.frame.packet_count==f.event_state.cursor==f.job.cursor==0 and f.pending.value==512 and f.render.pending==0 and f.job.job_id==job_id and tuple(f.job.words)==immutable
 f.packet(PACKETS[0]);f.packet(PACKETS[-1]);f.write(0xa05f8160,0x80000000);assert f.frame.packet_count==2
 f.execute();assert f.job.stats.pixels==36 and f.render.pending==7 and f.frame.packet_count==2
 f.write(0xa05f6900,0xffffffff);f.write(0xa05f8014,0xffffffff);f.write(0xa05f8144,0x80000000);f.execute();f.write(0xa05f6900,0xffffffff)
 # Reuse the same fixed-size collector for 1,000 actual native jobs.
 for generation in range(1000):
  for words in PACKETS:f.packet(words)
  f.write(0xa05f8014,0xffffffff);f.write(0xa05f8008,1);f.write(0xa05f8128,(generation%2)*0x400000);f.write(0xa05f8144,0x80000000)
  assert f.frame.packet_capacity==64 and f.frame.packet_count==0 and f.job.ready
  f.execute();f.write(0xa05f6900,0xffffffff)
 assert f.render.completed==1002 and f.state.retired==6008 and f.state.discarded==0
 f.packet(PACKETS[0]);before=f.frame.packet_count;f.write(0xa05f8144,0);assert f.frame.packet_count==before
 saved=bytes(f.state),bytes(f.job),events.snapshot(f.event_state),f.frame.packet_count;f.fail=-8;assert f.write(0xa05f8144,0x80000000)==-8 and saved==(bytes(f.state),bytes(f.job),events.snapshot(f.event_state),f.frame.packet_count);f.fail=0
 calls=f.parent_calls;assert f.write(0xa05f8144,0x80000000,2)==-3 and f.parent_calls==calls
 f.write(0xa05f8144,0x80000000);assert f.state.discarded==1 and f.event_state.active==0
 disabled=Fixture(False)
 for _ in range(64):disabled.packet(PACKETS[0])
 assert disabled.packet(PACKETS[0])==-4;disabled.write(0xa05f8144,0x80000000);assert disabled.frame.packet_count==64 and disabled.packet(PACKETS[0])==-4 and disabled.state.inits==0
 # Each invalid fixture is discarded after its one failing operation.
 overflow_checks=0
 for name in ('inits','retired','discarded','soft_resets'):
  g=Fixture();g.packet(PACKETS[0]);setattr(g.state,name,2**64-1);setattr(g.ref,name,2**64-1);g.check('explicit overflow input')
  calls=g.parent_calls;assert g.write(0xa05f8008 if name=='soft_resets' else 0xa05f8144,1 if name=='soft_resets' else 0x80000000)==-6 and g.parent_calls==calls;overflow_checks+=1
 files=[Path(__file__),P/'source/ta_list_lifecycle.c',P/'source/ta_list_lifecycle.h',P/'build_ta_list_lifecycle.ps1',P/'ta_list_lifecycle_native.py',P/'ta_list_lifecycle_reference.py',P/'source/frame_ta_command_bus.c',P/'source/frame_events_bus_eol.c',P/'source/pixel_jobs.c',life.DLL,jobs.DLL,abi.namespace['effects'].DLL]
 result=dict(status='guest_list_init_reuses_fixed_collector_and_preserves_immutable_jobs',checks=f.checks+disabled.checks,normal_native_render_jobs_completed=1002,normal_contexts_initialized=f.state.inits,accepted_packets_retired=f.state.retired,unsubmitted_packets_discarded=f.state.discarded,per_context_capacity=64,disabled_profile_retains_capacity_failure=True,pending_list_events_preserved=True,TA_continue_does_not_discard=True,parent_failure_and_width_controls=True,overflow_controls=overflow_checks,full_colour_depth_job_and_lifecycle_state_compared=True,physical_TA_parameter_RAM_layout_qualified=False,overall_completion_percent=10,new_original_routine_credit=0,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
 target.write_text(json.dumps(result,indent=2)+'\n');print('VERIFIED TA list lifecycle:',result['checks'],'comparisons,',f.render.completed,'native jobs; unchanged capacity64',flush=True)
if __name__=='__main__':main()
