"""Extend214-source integration with original countdown/input/audio routes."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;mp=P/'analysis/movie_channel_worker_translation.json';manifest=json.loads(mp.read_text());assert len(manifest['routines'])==9
old=P/'source/sa_movie_task_owner_connected.c';text=old.read_text()
text=text.replace('#include "sh4_exception_state.h"','VDECL(sa_movie_channel_worker_resume_8c647982);\nVDECL(sa_movie_channel_worker_resume_8c647b74);\n#include "sh4_exception_state.h"')
marker='#include "sh4_exception_state.h"';assert text.count(marker)==1;text=text.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in manifest['routines'])+marker)
marker=' return -2;';assert text.count(marker)==1;text=text.replace(marker,''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n' for r in manifest['routines'])+marker)
for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):text=text.replace('sa_movie_task_owner_'+suffix,'sa_movie_channel_worker_'+suffix)
marker=' if(c->observe)c->observe(c->user,target,m,f,sr,r,pr);';assert text.count(marker)==1
text=text.replace(marker,''.join(f' if(target==0x{pc:x}u)return sa_movie_channel_worker_resume_{pc:08x}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n' for pc in (0x8c647982,0x8c647b74))+marker)
bridge=P/'source/sa_movie_channel_worker_connected.c';bridge.write_text(text)
old_probe=P/'source/movie_task_owner_probe.c';probe=P/'source/movie_channel_worker_probe.c';probe.write_text(old_probe.read_text().replace('sa_movie_task_owner_','sa_movie_channel_worker_'))
old_build=P/'build_movie_task_owner.ps1';build=old_build.read_text().replace("'sa_movie_task_owner_connected.c'","'sa_movie_channel_worker_connected.c','sa_movie_channel_worker.c'").replace('movie_task_owner_probe.c','movie_channel_worker_probe.c').replace('movie_task_owner.dll','movie_channel_worker.dll');bp=P/'build_movie_channel_worker.ps1';bp.write_text(build)
sources=re.findall(r"'([^']+\.c)'",build);assert len(sources)==len(set(sources))==219
files=[Path(__file__),old,old_probe,old_build,bridge,probe,bp,mp]+[P/'source'/n for n in sources]
report=dict(compiled_source_count=219,proposed_integrated_entries=9,newly_registered_entries=0,source=bridge.relative_to(P).as_posix(),scope='Exact old218 bridge plus9real routes/export renames. Runtime/console/IRQ/context/ABI/storage policies preserved. New source is proposals until qualification;canonical routines no extra credit;no result substitutes.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(P/'analysis/movie_channel_worker_connection.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated219-source countdown integration')
