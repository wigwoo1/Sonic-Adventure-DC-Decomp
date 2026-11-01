"""Exact old comparison with additive native/reference TA DMA endpoints."""
import renderer_frame_ta_dma_reference as ta
import write_watch_renderer_frame_signal_capture as previous
from write_watch_renderer_frame_submit import with_extend
parent_watched=previous.namespace['watched_reference']


def watched_reference(*args):
 ref=parent_watched(*args);ref.events=with_extend(ref.events)
 ref.packet_capacity=ref.capacity
 replacement=ta.Reference.__new__(ta.Reference);replacement.__dict__.update(ref.pvr_dma.__dict__);ref.pvr_dma=replacement
 old_write=ref.pvr_device_write
 def device_write(address,value):
  if 0x10000000<=address<0x10800000:return ta.command_word(ref,address,value)
  return old_write(address,value)
 ref.pvr_device_write=device_write
 return ref


namespace=dict(previous.namespace,watched_reference=watched_reference)
exec(compile(previous.code,'TA_DMA_complete_state_comparison','exec'),namespace,local:={})
compare=local['compare']
