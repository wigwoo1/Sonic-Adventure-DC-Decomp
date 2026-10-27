"""Add only new original guest routes to the verified209-source integration."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;mp=P/'analysis/movie_slot_initialize_translation.json';manifest=json.loads(mp.read_text());assert len(manifest['routines'])==7
old=P/'source/sa_movie_system_state_connected.c';text=old.read_text()
marker='#include "sh4_exception_state.h"';assert text.count(marker)==1;text=text.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in manifest['routines'])+marker)
marker=' return -2;';assert text.count(marker)==1
text=text.replace(marker,''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n' for r in manifest['routines'])+marker)
for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):text=text.replace('sa_movie_system_state_'+suffix,'sa_movie_slot_initialize_'+suffix)
bridge=P/'source/sa_movie_slot_initialize_connected.c';bridge.write_text(text)
old_probe=P/'source/movie_system_state_probe.c';probe=P/'source/movie_slot_initialize_probe.c';probe.write_text(old_probe.read_text().replace('sa_movie_system_state_','sa_movie_slot_initialize_'))
old_build=P/'build_movie_system_state.ps1';build=old_build.read_text().replace("'sa_movie_system_state_connected.c'","'sa_movie_slot_initialize_connected.c','sa_movie_slot_initialize.c'").replace('movie_system_state_probe.c','movie_slot_initialize_probe.c').replace('movie_system_state.dll','movie_slot_initialize.dll');bp=P/'build_movie_slot_initialize.ps1';bp.write_text(build)
sources=re.findall(r"'([^']+\.c)'",build);assert len(sources)==len(set(sources))==210
files=[Path(__file__),old,old_probe,old_build,bridge,probe,bp,mp]+[P/'source'/n for n in sources]
report=dict(compiled_source_count=210,newly_registered_entries=0,source=bridge.relative_to(P).as_posix(),scope='Only seven original guest dispatch routes and export names extend the209-source integration. Original clear helper and metadata decoder have earlier canonical recoveries; no additional credit. Firmware/platform/IRQ/exception/storage policies and complete APIs retained. Unqualified proposals, no substitute helper returns.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(P/'analysis/movie_slot_initialize_connection.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated210-source record initialization integration')
