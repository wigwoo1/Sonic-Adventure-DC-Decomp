"""Extend214-source integration with original countdown/input/audio routes."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;mp=P/'analysis/movie_task_yield_translation.json';manifest=json.loads(mp.read_text());assert len(manifest['routines'])==3
old=P/'source/sa_movie_active_input_connected.c';text=old.read_text()
text=text.replace('#include \"sh4_exception_state.h\"','VDECL(sa_movie_task_yield_resume_8c668e48);\n#include \"sh4_exception_state.h\"')
marker='#include "sh4_exception_state.h"';assert text.count(marker)==1;text=text.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in manifest['routines'])+marker)
marker=' return -2;';assert text.count(marker)==1;text=text.replace(marker,''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n' for r in manifest['routines'])+marker)
for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):text=text.replace('sa_movie_active_input_'+suffix,'sa_movie_task_yield_'+suffix)
marker=' if(c->observe)c->observe(c->user,target,m,f,sr,r,pr);';assert text.count(marker)==1
text=text.replace(marker,' if(target==0x8c668e48u)return sa_movie_task_yield_resume_8c668e48(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'+marker)
bridge=P/'source/sa_movie_task_yield_connected.c';bridge.write_text(text)
old_probe=P/'source/movie_active_input_probe.c';probe=P/'source/movie_task_yield_probe.c';probe.write_text(old_probe.read_text().replace('sa_movie_active_input_','sa_movie_task_yield_'))
old_build=P/'build_movie_active_input.ps1';build=old_build.read_text().replace("'sa_movie_active_input_connected.c'","'sa_movie_task_yield_connected.c','sa_movie_task_yield.c'").replace('movie_active_input_probe.c','movie_task_yield_probe.c').replace('movie_active_input.dll','movie_task_yield.dll');bp=P/'build_movie_task_yield.ps1';bp.write_text(build)
sources=re.findall(r"'([^']+\.c)'",build);assert len(sources)==len(set(sources))==217
files=[Path(__file__),old,old_probe,old_build,bridge,probe,bp,mp]+[P/'source'/n for n in sources]
report=dict(compiled_source_count=217,proposed_integrated_entries=3,newly_registered_entries=0,source=bridge.relative_to(P).as_posix(),scope='Exact old216 bridge plus3real routes/export renames. Runtime/console/IRQ/context/ABI/storage policies preserved. New source is proposals until qualification;canonical routines no extra credit;no result substitutes.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(P/'analysis/movie_task_yield_connection.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated217-source countdown integration')
