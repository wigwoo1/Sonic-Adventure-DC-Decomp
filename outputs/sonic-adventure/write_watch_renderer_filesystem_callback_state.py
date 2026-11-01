"""Carry matched graphics-device outputs across ordinary public calls.

The existing decoder,native DLL,wait guards and complete comparisons are retained.
Only test-device initialization and extraction are extended. No CPU frame or
game-callee return is supplied. Trace buffers remain observations per comparison.
"""
import ctypes,hashlib
from dataclasses import dataclass,field
import write_watch_renderer_filesystem_callback_base as old

ARRAYS=('registers','dma_registers','masks','transfer_registers','startup_registers','system_registers','cache_tags')
SCALARS=('normal_status','ccr','ic_requests','oc_requests','port_data','control_word','dma_operation')
REF_ARRAYS=('registers','dma','masks','transfer_registers','startup_registers','system_registers','cache_tags')
REF_SCALARS=('normal_status','ccr','ic','oc','port_data','control_word','dma_operation')
DMA=old.compare.__globals__['DMA']

@dataclass(frozen=True,repr=False)
class GraphicsState:
 vram:bytes=field(repr=False)
 sound:bytes=field(repr=False)
 arrays:tuple
 scalars:tuple
 qacr:tuple
 queue:tuple
 vertex_counts:tuple
 packets:tuple
 pvr:tuple
 pvr_error_events:tuple
 def summary(self):
  return dict(vram_sha256=hashlib.sha256(self.vram).hexdigest(),sound_sha256=hashlib.sha256(self.sound).hexdigest(),registers_sha256=hashlib.sha256(b''.join(x.to_bytes(4,'little') for x in self.arrays[0])).hexdigest(),qacr=list(self.qacr),queued_packets=len(self.packets),pvr_pending=self.pvr[7],pvr_active=self.pvr[11])
 def __repr__(self):return 'GraphicsState('+str(self.summary())+')'

def reference_state(ref):
 arrays=(tuple(ref.registers.get(0x5f8000+4*i,0) for i in range(2048)),tuple(ref.dma.get(a,0) for a in DMA),*(tuple(getattr(ref,n)) for n in REF_ARRAYS[2:]))
 return GraphicsState(bytes(ref.vram),bytes(ref.sound.ram),arrays,tuple(getattr(ref,n) for n in REF_SCALARS),tuple(ref.qacr),tuple(ref.queue),(ref.invalidates,ref.prefetches,ref.flushes),tuple(ref.packets),old.pvr_model.snapshot(ref.pvr_dma),tuple(ref.pvr_dma.error_events))

def native_state(memory,pvr,sound_view):
 return GraphicsState(bytes(memory.vram),bytes(sound_view),tuple(tuple(getattr(memory,n)) for n in ARRAYS),tuple(getattr(memory,n) for n in SCALARS),tuple(memory.vertex.qacr),tuple(memory.vertex.queue),(memory.vertex.invalidates,memory.vertex.prefetches,memory.vertex.flushes),old.compare.__globals__['packet_snapshot'](memory),old.pvr_model.snapshot(pvr),tuple(tuple(e) for e in pvr.error_trace[:pvr.error_count]))

def restore_reference(ref,c):
 state=c.get('graphics_in')
 if state is None:return
 assert isinstance(state,GraphicsState) and len(state.vram)==len(ref.vram) and len(state.sound)==len(ref.sound.ram)
 ref.vram[:]=state.vram;ref.sound.ram[:]=state.sound
 ref.registers={0x5f8000+4*i:x for i,x in enumerate(state.arrays[0])};ref.dma=dict(zip(DMA,state.arrays[1]))
 for name,values in zip(REF_ARRAYS[2:],state.arrays[2:]):getattr(ref,name)[:]=values
 for name,value in zip(REF_SCALARS,state.scalars):setattr(ref,name,value)
 ref.qacr[:]=state.qacr;ref.queue[:]=state.queue;ref.invalidates,ref.prefetches,ref.flushes=state.vertex_counts;ref.packets[:]=state.packets
 p=ref.pvr_dma;p.registers[:]=state.pvr[0]
 for name,value in zip(old.pvr_model.TAIL,state.pvr[1:-1]):setattr(p,name,value)
 for name,value in zip(old.pvr_model.ERROR,state.pvr[-1]):setattr(p,name,value)
 p.error_events[:]=state.pvr_error_events
 assert reference_state(ref)==state,'exact prior matched graphics input restored on reference side'

def restore_native(memory,pvr,sound_view,c):
 state=c.get('graphics_in')
 if state is None:return
 assert isinstance(state,GraphicsState) and len(state.vram)==ctypes.sizeof(memory.vram) and len(state.sound)==len(sound_view)
 ctypes.memmove(memory.vram,state.vram,len(state.vram));sound_view[:]=state.sound
 for name,values in zip(ARRAYS,state.arrays):getattr(memory,name)[:]=values
 for name,value in zip(SCALARS,state.scalars):setattr(memory,name,value)
 memory.vertex.qacr[:]=state.qacr;memory.vertex.queue[:]=state.queue;memory.vertex.invalidates,memory.vertex.prefetches,memory.vertex.flushes=state.vertex_counts
 memory.frame.packet_count=len(state.packets);assert len(state.packets)<=memory.packet_capacity
 for out,(destination,queue,words) in zip(memory.packets,state.packets):out.destination=destination;out.queue=queue;out.words[:]=words
 pvr.registers[:]=state.pvr[0]
 for name,value in zip(old.pvr_model.TAIL,state.pvr[1:-1]):setattr(pvr,name,value)
 for name,value in zip(old.pvr_model.ERROR,state.pvr[-1]):setattr(pvr,name,value)
 for out,values in zip(pvr.error_trace,state.pvr_error_events):out[:]=values
 assert native_state(memory,pvr,sound_view)==state,'exact prior matched graphics input restored on native side'

parent_watched=old.compare.__globals__['watched_reference']
def watched_reference(*args):
 ref=parent_watched(*args);restore_reference(ref,args[-1]);return ref

code=old.font_code
marker=' @effects.OBSERVE';assert code.count(marker)==1
code=code.replace(marker,' restore_native(memory,pvr_state,sound_view,c)\n'+marker)
marker=' metrics=journal.finish()';assert code.count(marker)==1
code=code.replace(marker," graphics_out=native_state(memory,pvr_state,sound_view)\n assert graphics_out==reference_state(ref),'complete graphics-device output equality'\n"+marker)
marker=' return dict(';assert code.count(marker)==1
code=code.replace(marker,' return dict(graphics_out=graphics_out,graphics_input_summary=c[\'graphics_in\'].summary() if c.get(\'graphics_in\') is not None else None,graphics_output_summary=graphics_out.summary(),')
namespace=dict(old.compare.__globals__,restore_native=restore_native,native_state=native_state,reference_state=reference_state,watched_reference=watched_reference)
exec(compile(code,'persistent_graphics_full_comparison','exec'),namespace,local:={})
compare=local['compare']
