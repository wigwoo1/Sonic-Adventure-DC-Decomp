"""A guest list-init discards partial DMA state without inventing an EOL event."""
import hashlib,json
from pathlib import Path
from verify_ta_list_lifecycle import Fixture,PACKETS
import renderer_frame_ta_dma_reference as ta
P=Path(__file__).resolve().parent
def main():
 target=P/'analysis/ta_list_partial_reset_verification.json';assert not target.exists();f=Fixture();word=PACKETS[0][0]
 assert f.outer.write(f.outer.context,0x10000000,word,4)==0
 f.ref_runtime.ta_packet=(0x10000000,[word]);assert f.frame.packet_count==0 and f.frame.packets[0].queue==0x80000001
 f.write(0xa05f8144,0x80000000)
 assert f.ref_runtime.ta_packet is None and f.frame.packet_count==0 and f.frame.packets[0].queue==0 and list(f.frame.packets[0].words)==[0]*8
 assert f.pending.value==0 and f.event_state.ends==0
 assert f.outer.write(f.outer.context,0x10000004,0,4)==-3
 # Discard this negative fixture; the complete DMA case starts afresh.
 g=Fixture();g.ref_runtime.packets=g.packets;g.ref_runtime.packet_capacity=64;g.ref_runtime.events=[]
 for lane,value in enumerate(PACKETS[0]):
  assert g.outer.write(g.outer.context,0x10000000+4*lane,value,4)==ta.command_word(g.ref_runtime,0x10000000+4*lane,value)==0
  assert g.ref_events.consume(g.packets,g.ref_runtime)==0;g.check('DMA lane')
 assert g.frame.packet_count==1 and g.frame.packets[0].queue==2
 g.write(0xa05f8144,0x80000000);assert g.state.discarded==1 and g.pending.value==0 and g.event_state.active==0
 files=[Path(__file__),P/'verify_ta_list_lifecycle.py',P/'source/ta_list_lifecycle.c',P/'source/ta_list_lifecycle.h',P/'ta_list_lifecycle_reference.py',P/'source/frame_ta_command_bus.c']
 result=dict(status='partial_DMA_discard_and_complete_DMA_retirement_verified',partial_packet_cleared=True,orphaned_DMA_continuation_rejected=True,complete_DMA_lanes_compared=8,list_completion_not_fabricated=True,negative_fixture_discarded=True,overall_completion_percent=10,sha256={str(f):hashlib.sha256(f.read_bytes()).hexdigest() for f in files})
 target.write_text(json.dumps(result,indent=2)+'\n');print('VERIFIED TA list-init partial and complete DMA reset',flush=True)
if __name__=='__main__':main()
