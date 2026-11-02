"""Compare guest-strobed collector generations in addition to all earlier state."""
from ta_stream_lifecycle_reference import Lifecycle
def watched(parent):
 def build(*args):
  ref=parent(*args);c=args[-1];ref.ta_lists=Lifecycle(c.get('ta_list_seed'),c.get('ta_list_enabled',True))
  event_class=next(k for k in type(ref).__mro__ if k.__name__=='RenderBelowClock').__bases__[0]
  class ListLifecycle(event_class):
   def __setitem__(self,a,value):
    job=getattr(self,'pixel_jobs',None);width=self.access_width
    assert self.ta_lists.check(a,value,width,self.packets,self.packet_capacity,self.frame_events,job)==0,('TA list lifecycle write',hex(a),hex(value))
    result=super().__setitem__(a,value)
    self.ta_lists.committed(a,value,width,self.packets,self.frame_events,job,self)
    return result
  class ReusingReference(type(ref),ListLifecycle):pass
  ref.__class__=ReusingReference;return ref
 return build
def extend(code):
 def change(a,b):
  nonlocal code
  assert code.count(a)==1,(a,code.count(a));code=code.replace(a,b)
 change('def compare(m,s,f,c):',"def compare(m,s,f,c):\n ta_list_state=ta_list_model.restore(c.get('ta_list_seed'),c.get('ta_list_enabled',True))")
 marker=" assert effects.LIB.sa_frame_events_bus_init(ctypes.byref(event_state),ctypes.byref(memory.frame),ctypes.byref(event_parent),ctypes.byref(bus),pvr_pending,event_seed,int(c.get('frame_events_enabled',True)))==0"
 change(marker,marker+'''
 ta_list_parent=effects.Bus.from_buffer_copy(bus);ta_list_binding=TaListBinding()
 assert ta_list_model.LIB.sa_ta_list_lifecycle_bind(ctypes.byref(ta_list_binding),ctypes.byref(ta_list_state),ctypes.byref(memory.frame),ctypes.byref(event_state),ctypes.byref(old.native_pixel_job) if old.native_pixel_job is not None else None,ctypes.byref(ta_list_parent),ctypes.byref(bus))==0
''')
 change('old.native_pixel_digests))','old.native_pixel_digests,ta_list_model.observation(ta_list_state)))')
 change(' journal.check()'," assert ta_list_model.snapshot(ta_list_state)==ref.ta_lists.snapshot(),'complete TA collector lifecycle and retired packets'\n journal.check()")
 change(' return dict('," return dict(ta_list_out=ta_list_model.serialize(ta_list_state),ta_current_packets=memory.frame.packet_count,ta_packet_capacity=memory.frame.packet_capacity,")
 return code
