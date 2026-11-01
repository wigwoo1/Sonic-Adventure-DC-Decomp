"""Add original gate to the existing full filesystem/IRQ runtime."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;mp=P/'analysis/renderer_filesystem_callback_translation.json';manifest=json.loads(mp.read_text())
old=P/'source/sa_renderer_list_callbacks_connected.c';text=old.read_text()
marker='#include "sh4_exception_state.h"';assert text.count(marker)==1;text=text.replace(marker,'VDECL(sa_renderer_filesystem_callback_8c5ffcf0);\n'+marker)
marker=' return -2;';assert text.count(marker)==1;text=text.replace(marker,' if(target==0x8c5ffcf0u)return sa_renderer_filesystem_callback_8c5ffcf0(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'+marker)
for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):text=text.replace('sa_renderer_list_callbacks_'+suffix,'sa_renderer_filesystem_callback_'+suffix)
bridge=P/'source/sa_renderer_filesystem_callback_connected.c';assert not bridge.exists();bridge.write_text(text)
old_probe=P/'source/renderer_list_callbacks_probe.c';probe=P/'source/renderer_filesystem_callback_probe.c';assert not probe.exists();probe.write_text(old_probe.read_text().replace('sa_renderer_list_callbacks_','sa_renderer_filesystem_callback_'))
old_build=P/'build_renderer_list_callbacks.ps1';build=old_build.read_text().replace("'sa_renderer_list_callbacks_connected.c'","'sa_renderer_filesystem_callback_connected.c','sa_renderer_filesystem_callback.c'").replace("'renderer_list_callbacks_probe.c'","'renderer_filesystem_callback_probe.c'").replace('renderer_list_callbacks.dll','renderer_filesystem_callback.dll')
bp=P/'build_renderer_filesystem_callback.ps1';assert not bp.exists();bp.write_text(build)
sources=re.findall(r"'([^']+\.c)'",build);assert len(sources)==len(set(sources))==287
files=[Path(__file__),mp,old,bridge,old_probe,probe,old_build,bp]+[P/'source'/n for n in sources]
report=dict(status='complete_gate_and_real_shared_worker_pending_qualification',compiled_sources=287,proposed_original_entries=1,original_prior_game_bodies_changed=0,new_original_routine_credit=0,shared_tail_new_credit=0,wait_guards_changed=False,service_outputs_supplied=False,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
target=P/'analysis/renderer_filesystem_callback_connection.json';assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n');print('Generated287-source original gate integration',flush=True)
