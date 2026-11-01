"""Add two original input callbacks without changing prior bodies or guards."""
import ast,hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent
def write(name,text):
 p=P/name;assert not p.exists();p.write_text(text);return p
old=P/'source/sa_renderer_filesystem_callback_connected.c';text=old.read_text()
marker='#include "sh4_exception_state.h"';assert text.count(marker)==1
text=text.replace(marker,'VDECL(sa_renderer_input_tick_8c646024);\nVDECL(sa_renderer_input_tick_8c6463ac);\n'+marker)
marker=' return -2;';assert text.count(marker)==1
text=text.replace(marker,''.join(f' if(target==0x{a:08x}u)return sa_renderer_input_tick_{a:08x}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n' for a in (0x8c646024,0x8c6463ac))+marker)
for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):text=text.replace('sa_renderer_filesystem_callback_'+suffix,'sa_renderer_input_tick_'+suffix)
bridge=write('source/sa_renderer_input_tick_connected.c',text)
probe=write('source/renderer_input_tick_probe.c',(P/'source/renderer_filesystem_callback_probe.c').read_text().replace('sa_renderer_filesystem_callback_','sa_renderer_input_tick_'))
build=(P/'build_renderer_filesystem_callback.ps1').read_text().replace("'sa_renderer_filesystem_callback_connected.c'","'sa_renderer_input_tick_connected.c','sa_renderer_input_tick.c'").replace("'renderer_filesystem_callback_probe.c'","'renderer_input_tick_probe.c'").replace('renderer_filesystem_callback.dll','renderer_input_tick.dll')
recipe=write('build_renderer_input_tick.ps1',build);sources=re.findall(r"'([^']+\.c)'",build);assert len(sources)==len(set(sources))==288
reference=(P/'renderer_filesystem_irq_reference.py').read_text().replace('from renderer_filesystem_callback_reference import *','from renderer_filesystem_irq_reference import *').replace('import renderer_filesystem_callback_reference as old','import renderer_filesystem_irq_reference as old').replace('analysis/renderer_filesystem_callback_translation.json','analysis/renderer_input_tick_translation.json')
start=reference.index('# Hooks borrow');end=reference.index("exec(compile(code,'render_buffer_original_model'",start)
reference=reference[:start]+reference[end:]
reference=reference.replace('build/renderer_filesystem_callback.dll','build/renderer_input_tick.dll').replace('effects.LIB.sa_renderer_filesystem_callback_','effects.LIB.sa_renderer_input_tick_')
generated=[bridge,probe,recipe,write('renderer_input_tick_reference.py',reference)]
for suffix in ('_base','_state','_capture',''):
 name='write_watch_renderer_filesystem_irq'+suffix+'.py'
 generated.append(write('write_watch_renderer_input_tick'+suffix+'.py',(P/name).read_text().replace('write_watch_renderer_filesystem_irq','write_watch_renderer_input_tick').replace('renderer_filesystem_irq_reference','renderer_input_tick_reference')))
generated.append(write('renderer_input_tick_fixtures.py',(P/'renderer_filesystem_irq_fixtures.py').read_text().replace('renderer_filesystem_irq_reference','renderer_input_tick_reference').replace('write_watch_renderer_filesystem_irq','write_watch_renderer_input_tick')))
for path in generated:
 if path.suffix=='.py':ast.parse(path.read_text(),filename=str(path))
files=[Path(__file__),P/'analysis/renderer_input_tick_translation.json',old,P/'source/renderer_filesystem_callback_probe.c',P/'build_renderer_filesystem_callback.ps1',P/'renderer_filesystem_irq_reference.py',*generated,*[P/'source'/n for n in sources]]
report=dict(status='original_input_callbacks_connected_pending_qualification',compiled_sources=288,proposed_original_entries=2,new_original_routine_credit=0,original_prior_game_bodies_changed=0,wait_guards_changed=False,service_outputs_supplied=False,reference_observer_unchanged=True,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
target=P/'analysis/renderer_input_tick_connection.json';assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n');print('Connected 288 sources; no old game body or guard changed',flush=True)
