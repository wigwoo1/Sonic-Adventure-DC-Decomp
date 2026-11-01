"""Preserve complete comparisons and append accepted-packet device state."""
import ctypes
import frame_events_eol_reference as events_model
import renderer_frame_ta_dma_reference as ta
import write_watch_renderer_render_completion_capture as previous
from write_watch_renderer_frame_submit import with_extend
parent_watched=previous.namespace['watched_reference']
effects=previous.namespace['effects']
EventsState=events_model.state_type(effects.Bus)
events_model.declare(effects.LIB,effects.Bus,EventsState)


def watched_reference(*args):
 ref=parent_watched(*args);ref.events=with_extend(ref.events);ref.packet_capacity=ref.capacity
 replacement=ta.Reference.__new__(ta.Reference);replacement.__dict__.update(ref.pvr_dma.__dict__);ref.pvr_dma=replacement
 old_write=ref.pvr_device_write
 def device_write(address,value):
  if 0x10000000<=address<0x10800000:return ta.command_word(ref,address,value)
  return old_write(address,value)
 ref.pvr_device_write=device_write
 class WithEvents(type(ref)):
  def __getitem__(self,address):
   value=super().__getitem__(address)
   return value|self.frame_events.pending if self.access_width==4 and address&0x1fffffff==0x5f6900 else value
  def __setitem__(self,address,value):
   result=super().__setitem__(address,value)
   if self.access_width==4 and address&0x1fffffff==0x5f6900:self.frame_events.acknowledge(value,self.runtime)
   assert self.frame_events.consume(self.packets,self.runtime)==0
   return result
  def prefetch(self,address):
   result=super().prefetch(address)
   assert self.frame_events.consume(self.packets,self.runtime)==0
   return result
 ref.__class__=WithEvents;c=args[-1]
 ref.frame_events=events_model.Reference(c.get('frame_events_seed'),c.get('frame_events_enabled',True))
 if ref.runtime:ref.runtime.pending|=ref.frame_events.pending
 assert ref.frame_events.consume(ref.packets,ref.runtime)==0
 return ref


from renderer_frame_clock_comparison import extend,watched
watched_reference=watched(watched_reference)
code=extend(previous.code)
marker=' restore_native(memory,pvr_state,sound_view,c)';assert code.count(marker)==1
code=code.replace(marker,marker+'''
 event_state=EventsState();event_parent=effects.Bus.from_buffer_copy(bus)
 event_seed=(ctypes.c_uint32*9)(*c['frame_events_seed']) if c.get('frame_events_seed') is not None else None
 assert effects.LIB.sa_frame_events_bus_init(ctypes.byref(event_state),ctypes.byref(memory.frame),ctypes.byref(event_parent),ctypes.byref(bus),pvr_pending,event_seed,int(c.get('frame_events_enabled',True)))==0
''')
marker='pvr_model.snapshot(pvr_state),console_model.snapshot';assert code.count(marker)==1
code=code.replace(marker,'pvr_model.snapshot(pvr_state),events_model.snapshot(event_state),console_model.snapshot')
marker=' metrics=journal.finish()';assert code.count(marker)==1
code=code.replace(marker," assert events_model.snapshot(event_state)==ref.frame_events.snapshot(),'complete frame-event device state'\n"+marker)
marker=' return dict(';assert code.count(marker)==1
code=code.replace(marker,' return dict(frame_events_state=events_model.snapshot(event_state),')
from renderer_render_completion_comparison import extend as render_extend,watched as render_watched,attach_pacing,render_fields
watched_reference=render_watched(watched_reference)
code=render_extend(code)
namespace=dict(previous.namespace,watched_reference=watched_reference,events_model=events_model,EventsState=EventsState,frame_model=__import__('frame_clock_native'),render_model=__import__('render_completion_native'),pacing_fields=render_fields)
namespace['pacing_stores']=attach_pacing(namespace['namespace'])
exec(compile(code,'TA_list_event_complete_state_comparison','exec'),namespace,local:={})
compare=local['compare']
