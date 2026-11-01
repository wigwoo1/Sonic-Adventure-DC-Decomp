"""Add authored TA command DMA capture while preserving historical backends."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent


def once(s,a,b):
 assert s.count(a)==1,a
 return s.replace(a,b)


frame_old=P/'source/frame_command_bus.c';dma_old=P/'source/pvr_dma_bus.c'
frame=frame_old.read_text();marker=' return m->graphics_bus.write(m->graphics_bus.context,a,value,n);'
frame=once(frame,marker,''' /* An authored FIFO endpoint for contiguous channel-2 DMA words.
  * queue=2 identifies DMA capture;CPU SQ state/counters are unchanged.
  * The high bit is a partial-packet cursor,never a completed packet tag. */
 if(a>=0x10000000u && a<0x10800000u){
  if(n!=4u || (a&3u))return -3;
  if(m->packet_count>=m->packet_capacity || m->packet_count>=SA_FRAME_COMMAND_CAPACITY)return -4;
  struct sa_frame_command_packet *p=&m->packets[m->packet_count];
  uint32_t lane=(a>>2)&7u,base=a&~31u;
  if(!lane){p->destination=base;p->queue=0x80000000u;}
  if(p->destination!=base || p->queue!=(0x80000000u|lane))return -3;
  p->words[lane]=value;
  if(lane==7u){p->queue=2u;m->packet_count++;event(&m->graphics.vertex,9u,base,2u,32u);}
  else p->queue=0x80000000u|(lane+1u);
  return 0;
 }
'''+marker)
dma=dma_old.read_text();dma=once(dma,' unsigned mode_index=(raw&0x02000000u)?9u:8u;', ' int ta=raw>=0x10000000u && raw<0x10800000u;\n unsigned mode_index=(raw&0x02000000u)?9u:8u;')
dma=once(dma,' s->destination=(mode?0xa5000000u:0xa4000000u)+offset;', ' s->destination=ta?(0x10000000u+offset):((mode?0xa5000000u:0xa4000000u)+offset);')
dma=once(dma,'if((s->registers[4]&0xffff8201u)!=0x8201u || mode>1u ||', 'if((s->registers[4]&0xffff8201u)!=0x8201u || (!ta && mode>1u) ||')
dma=once(dma,'(raw&0xfc000000u)!=0x10000000u || !(raw&0x01000000u) ||', '(raw&0xfc000000u)!=0x10000000u || (!ta && !(raw&0x01000000u)) ||')
f=P/'source/frame_ta_command_bus.c';d=P/'source/pvr_ta_dma_bus.c';f.write_text(frame);d.write_text(dma)
build_old=P/'build_renderer_context_owner.ps1';build=build_old.read_text().replace("'frame_command_bus.c'","'frame_ta_command_bus.c'").replace("'pvr_dma_bus.c'","'pvr_ta_dma_bus.c'").replace('renderer_context_owner.dll','renderer_frame_ta.dll');bp=P/'build_renderer_frame_ta.ps1';bp.write_text(build)
sources=re.findall(r"'([^']+\.c)'",build);assert len(sources)==len(set(sources))==284
files=[Path(__file__),frame_old,dma_old,f,d,build_old,bp,P/'source/frame_command_bus.h',P/'source/pvr_dma_bus.h']
report=dict(status='additive_authored_TA_DMA_capture_backend_pending_qualification',source_count=284,original_game_C_changes=0,new_original_routine_credit=0,ABI_changed=False,packet_route_tag_DMA=2,CPU_store_queue_counters_changed_by_DMA=False,TA_pixels_or_list_interrupts_emulated=False,scope='Contiguous32-byte TA command packet capture and existing untimed channel-2 completion19. Historical texture-transfer path unchanged. Capture doesnot implementTA list interpretation,rasterization,physical timing orend-of-list IRQs.',primary_contracts=['https://kos-docs.dreamcast.wiki/group__pvr__addresses.html','https://kos-docs.dreamcast.wiki/group__pvr__dma.html'],sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(P/'analysis/renderer_frame_ta_connection.json').write_text(json.dumps(report,indent=2)+'\n')
print('Generated284-source additiveTA DMA backend;zero original-routine credit',flush=True)
