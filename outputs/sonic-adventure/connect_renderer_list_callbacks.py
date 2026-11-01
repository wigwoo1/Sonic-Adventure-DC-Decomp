"""Link the original list-completion proposal into the preserved event runtime."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent
mp=P/'analysis/renderer_list_callbacks_translation.json';manifest=json.loads(mp.read_text())
old=P/'source/sa_renderer_frame_signal_connected.c';text=old.read_text();marker='#include "sh4_exception_state.h"';assert text.count(marker)==1
text=text.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in manifest['routines'])+marker)
marker=' return -2;';assert text.count(marker)==1
text=text.replace(marker,''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n' for r in manifest['routines'])+marker)
text=text.replace('sa_renderer_frame_signal_','sa_renderer_list_callbacks_')
marker='static const uint32_t allowed[]={';begin=text.index(marker)+len(marker);end=text.index('};',begin);allowed={int(x,16) for x in re.findall(r'0x[0-9a-f]+',text[begin:end])};allowed.add(0x8c604562);text=text[:begin]+','.join(hex(x)+'u' for x in sorted(allowed))+text[end:]
bridge=P/'source/sa_renderer_list_callbacks_connected.c';assert not bridge.exists();bridge.write_text(text)
old_probe=P/'source/renderer_frame_signal_probe.c';pp=P/'source/renderer_list_callbacks_probe.c';assert not pp.exists();pp.write_text(old_probe.read_text().replace('sa_renderer_frame_signal_','sa_renderer_list_callbacks_'))
old_build=P/'build_renderer_frame_events_eol.ps1';build=old_build.read_text().replace("'sa_renderer_frame_signal_connected.c'","'sa_renderer_list_callbacks_connected.c','sa_renderer_list_callbacks.c'").replace("'renderer_frame_signal_probe.c'","'renderer_list_callbacks_probe.c'").replace('renderer_frame_events_eol.dll','renderer_list_callbacks.dll')
bp=P/'build_renderer_list_callbacks.ps1';assert not bp.exists();bp.write_text(build)
sources=re.findall(r"'([^']+\.c)'",build);assert len(sources)==len(set(sources))==286
files=[Path(__file__),mp,old,bridge,old_probe,pp,old_build,bp]+[P/'source'/n for n in sources]
report=dict(status='original_list_completion_proposal_connected_pending_qualification',compiled_sources=286,preserved_prior_compilation_inputs=283,original_prior_game_bodies_changed=0,proposed_original_entries=3,canonical_reused_entries=1,new_original_routine_credit=0,wait_guards_changed=False,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
target=P/'analysis/renderer_list_callbacks_connection.json';assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n');print('Generated286-source original list-completion integration',flush=True)
