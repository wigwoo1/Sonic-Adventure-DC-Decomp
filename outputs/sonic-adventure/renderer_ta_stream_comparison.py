"""Select the paged host device while retaining complete original CPU comparisons.

The existing 64-packet device remains available and unchanged. Only the new
host capture profile uses owned pages, global cursors and separate retired lists.
Pixel-job integration is deliberately rejected until it has its own verification.
"""
import ctypes
from dataclasses import replace
import ta_stream_native as stream
import ta_stream_events_reference as events_model

def install(code,namespace):
 base_watched=namespace['watched_reference'];base_restore=namespace['restore_native'];base_state=namespace['native_state'];Bus=namespace['effects'].Bus
 def watched(*args):
  ref=base_watched(*args)
  # Counter range, not a newly enlarged fixed array. The native backend allocates
  # 64-packet pages as required and fails explicitly on allocation exhaustion.
  ref.capacity=ref.packet_capacity=0xffffffff
  return ref
 def restore_native(memory,pvr,sound,c):
  assert c.get('pixel_job_seed') is None,'paged pixel-job execution is not yet integrated'
  temporary=dict(c);state=c.get('graphics_in')
  if state is not None:temporary['graphics_in']=replace(state,packets=())
  base_restore(memory,pvr,sound,temporary)
 def native_state(memory,pvr,sound):return replace(base_state(memory,pvr,sound),packets=stream.packets())
 prior_snapshot=events_model.snapshot
 def event_snapshot(state):return stream.events() if getattr(state,'owned_stream',False) else prior_snapshot(state)
 events_model.snapshot=event_snapshot
 def change(a,b):
  nonlocal code
  assert code.count(a)==1,(a,code.count(a));code=code.replace(a,b)
 marker=' pvr_state=PvrState();pvr_parent=bus;bus=effects.Bus();pvr_seed='
 change(marker,''' stream.current=stream.LIB.sa_ta_stream_create_win32();assert stream.current,'native page owner allocation'
 stream_parent=effects.Bus.from_buffer_copy(bus);stream_output=effects.Bus()
 assert stream.LIB.sa_ta_stream_bind(stream.current,ctypes.byref(memory.vertex),ctypes.byref(stream_parent),ctypes.byref(stream_output),None,None,None)==0
 bus=stream_output
'''+marker)
 change(" assert effects.LIB.sa_frame_events_bus_init(ctypes.byref(event_state),ctypes.byref(memory.frame),ctypes.byref(event_parent),ctypes.byref(bus),pvr_pending,event_seed,int(c.get('frame_events_enabled',True)))==0",''' event_values=list(c.get('frame_events_seed',(0,)*9));event_values[8]=int(c.get('frame_events_enabled',True));event_seed=(ctypes.c_uint32*9)(*event_values)
 assert not ta_list_state.last,'restore of retired page generations is a later integration step'
 for stream_address,stream_queue,stream_words in c['graphics_in'].packets if c.get('graphics_in') is not None else ():
  packet=stream.Packet(stream_address,stream_queue,(stream.U*8)(*stream_words));assert stream.LIB.sa_ta_stream_append(stream.current,ctypes.byref(packet))==0
 lifecycle_seed=(stream.Q*8)(*ta_list_state.values);unused_stream_bus=effects.Bus()
 assert stream.LIB.sa_ta_stream_bind(stream.current,ctypes.byref(memory.vertex),ctypes.byref(stream_parent),ctypes.byref(unused_stream_bus),pvr_pending,event_seed,lifecycle_seed)==0
 event_state.owned_stream=True''')
 change(' ta_list_parent=effects.Bus.from_buffer_copy(bus);ta_list_binding=TaListBinding()\n assert ta_list_model.LIB.sa_ta_list_lifecycle_bind(ctypes.byref(ta_list_binding),ctypes.byref(ta_list_state),ctypes.byref(memory.frame),ctypes.byref(event_state),ctypes.byref(old.native_pixel_job) if old.native_pixel_job is not None else None,ctypes.byref(ta_list_parent),ctypes.byref(bus))==0',' # Owned stream handles the original list-init transition below DMA.')
 change('ta_current_packets=memory.frame.packet_count','ta_stream_pages=stream.LIB.sa_ta_stream_pages(stream.current,0),ta_retired_pages=stream.LIB.sa_ta_stream_pages(stream.current,1),ta_current_packets=stream.LIB.sa_ta_stream_count(stream.current,0)')
 namespace=dict(namespace,stream=stream,watched_reference=watched,restore_native=restore_native,native_state=native_state,packet_snapshot=lambda memory:stream.packets())
 return code,namespace
