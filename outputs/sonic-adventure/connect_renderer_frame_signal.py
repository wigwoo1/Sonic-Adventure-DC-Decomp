"""Observe the existing external normal-status source at frame idle boundaries."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent


def once(s,a,b):
 assert s.count(a)==1,a
 return s.replace(a,b)


old=P/'source/sa_renderer_frame_irq_connected.c';text=old.read_text()
start=text.index('static int runtime_poll_frame(');end=text.index('\nstatic int dispatch(void *opaque,',start)
frame=text[start:end]
frame=once(frame,'(*pr!=0x8c642060u&&*pr!=0x8c6420a0u)||!c->runtime->state->pending','(*pr!=0x8c642060u&&*pr!=0x8c6420a0u)')
before=' uint32_t ready;if(!c->runtime->parent.read)return -2;status=c->runtime->parent.read(c->runtime->parent.context,0xa05f6930u,4u,&ready);if(status||!(ready&c->runtime->state->pending))return status;'
after=''' uint32_t mask,normal;if(!c->runtime->parent.read)return -2;
 status=c->runtime->parent.read(c->runtime->parent.context,0xa05f6930u,4u,&mask);if(status)return status;
 status=c->runtime->parent.read(c->runtime->parent.context,0xa05f6900u,4u,&normal);if(status)return status;
 if(!(mask&(normal|c->runtime->state->pending)))return 0;'''
frame=once(frame,before,after)
text=text[:start]+frame+text[end:]
text=text.replace('sa_renderer_frame_irq_','sa_renderer_frame_signal_')
bridge=P/'source/sa_renderer_frame_signal_connected.c';assert not bridge.exists();bridge.write_text(text)
old_probe=P/'source/renderer_frame_irq_probe.c';probe=old_probe.read_text().replace('sa_renderer_frame_irq_','sa_renderer_frame_signal_')
pp=P/'source/renderer_frame_signal_probe.c';assert not pp.exists();pp.write_text(probe)
old_build=P/'build_renderer_frame_irq.ps1';build=old_build.read_text().replace("'sa_renderer_frame_irq_connected.c'","'sa_renderer_frame_signal_connected.c'").replace("'renderer_frame_irq_probe.c'","'renderer_frame_signal_probe.c'").replace('renderer_frame_irq.dll','renderer_frame_signal.dll')
bp=P/'build_renderer_frame_signal.ps1';assert not bp.exists();bp.write_text(build)
sources=re.findall(r"'([^']+\.c)'",build);assert len(sources)==len(set(sources))==284
files=[Path(__file__),old,bridge,old_probe,pp,old_build,bp]+[P/'source'/n for n in sources]
report=dict(status='external_status_frame_idle_delivery_pending_qualification',compiled_source_count=284,original_game_C_changes=0,new_original_routine_credit=0,ABI_changed=False,service_boundaries=['original6416ecRTS/delay to642060','original6416ecRTS/delay to6420a0'],automatic_VBlank_or_TA_events=False,scope='At actual idle return,authored platform reads original enabled mask6930 andexisting declared external normalstatus6900;also respects device-latchedDMA/G2pending. Original vectors/registered handlers/RTE execute. No gameflag orevent isgenerated;external observations andtiming remain explicit software inputs.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
target=P/'analysis/renderer_frame_signal_connection.json';assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n')
print('Generated284-source external-status frame delivery bridge',flush=True)
