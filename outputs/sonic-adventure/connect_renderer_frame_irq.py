"""Add a frame-idle interrupt boundary; retain all recovered game bodies."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent


def once(text,before,after):
 assert text.count(before)==1,before
 return text.replace(before,after)


old=P/'source/sa_renderer_context_owner_connected.c';text=old.read_text()
start=text.index('static int runtime_poll_buffer(')
end=text.index('\nstatic int dispatch(void *opaque,',start)
frame=text[start:end].replace('runtime_poll_buffer','runtime_poll_frame',1)
frame=once(frame,'*pr!=0x8c65100au||!(c->runtime->state->pending&(1u<<19))','(*pr!=0x8c642060u&&*pr!=0x8c6420a0u)||!c->runtime->state->pending')
text=text[:end]+'\n/* Authored untimed delivery of already-latched external device events.\n * No event is generated and no guest work/completion flag is written here. */\n'+frame+text[end:]
route=' if(target==0x8c6416ecu)return sa_player_texture_driver_defaults_8c6416ec(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);'
text=once(text,route,' if(target==0x8c6416ecu){int status=sa_player_texture_driver_defaults_8c6416ec(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);return status?status:runtime_poll_frame(c,m,f,sr,r,pr);}')
pattern=r'sa_renderer_context_owner_(connected|initialized_connected|exception_connected|running_connected|storage_connected)\b'
text=re.sub(pattern,r'sa_renderer_frame_irq_\1',text)
bridge=P/'source/sa_renderer_frame_irq_connected.c';assert not bridge.exists();bridge.write_text(text)
old_probe=P/'source/renderer_context_owner_probe.c'
probe=re.sub(r'sa_renderer_context_owner_(?=(?:connected|initialized_connected|exception_connected|running_connected|storage_connected|observed|exception_observed|running_observed|storage_observed)\b)','sa_renderer_frame_irq_',old_probe.read_text())
pp=P/'source/renderer_frame_irq_probe.c';assert not pp.exists();pp.write_text(probe)
old_build=P/'build_renderer_frame_ta.ps1';build=old_build.read_text()
build=once(build,"'sa_renderer_context_owner_connected.c'","'sa_renderer_frame_irq_connected.c'")
build=once(build,"'renderer_context_owner_probe.c'","'renderer_frame_irq_probe.c'")
build=once(build,'renderer_frame_ta.dll','renderer_frame_irq.dll')
bp=P/'build_renderer_frame_irq.ps1';assert not bp.exists();bp.write_text(build)
sources=re.findall(r"'([^']+\.c)'",build);assert len(sources)==len(set(sources))==284
files=[Path(__file__),old,bridge,old_probe,pp,old_build,bp]+[P/'source'/n for n in sources]
report=dict(status='additive_frame_idle_IRQ_boundary_pending_qualification',compiled_source_count=284,original_game_C_changes=0,new_original_routine_credit=0,ABI_changed=False,service_boundaries=['6416ec actualRTS/delay return to642060','6416ec actualRTS/delay return to6420a0'],event='already-latched normal device status enabled by6930;INTEVT320;priority6',automatic_VBlank_or_TA_events=False,scope='Deliver existing external device input through actual copied vector/context/registered graphics handler/RTE. Original wait guards,game bodies,old backends andall prior artifacts remain unchanged. This doesnot generate events orclaim physical timing/pixels.',primary_contracts=['https://kos-docs.dreamcast.wiki/group__asic__regs.html','https://kos-docs.dreamcast.wiki/kernel_2arch_2dreamcast_2include_2arch_2irq_8h.html','https://www.renesas.com/en/document/mah/sh7750-sh7750s-sh7750r-group-users-manual-hardware'],sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
target=P/'analysis/renderer_frame_irq_connection.json';assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n')
print('Generated additive284-source frameIRQ bridge;no recovered game body changed',flush=True)
