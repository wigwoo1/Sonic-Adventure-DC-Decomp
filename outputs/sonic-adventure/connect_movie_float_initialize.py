"""Keep206-source integration intact and add only the reached original initializer."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;mp=P/'analysis/movie_float_initialize_translation.json';manifest=json.loads(mp.read_text());assert len(manifest['routines'])==2
old=P/'source/sa_movie_buffer_initialize_connected.c';text=old.read_text()
marker='#include "sh4_exception_state.h"';assert text.count(marker)==1;text=text.replace(marker,''.join(f'VDECL({routine["name"]});\n' for routine in manifest['routines'])+marker)
marker=' return -2;';assert text.count(marker)==1
text=text.replace(marker,''.join(f' if(target=={routine["address"]}u)return {routine["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n' for routine in manifest['routines'])+marker)
for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):text=text.replace('sa_movie_buffer_initialize_'+suffix,'sa_movie_float_initialize_'+suffix)
bridge=P/'source/sa_movie_float_initialize_connected.c';bridge.write_text(text)
old_probe=P/'source/movie_buffer_initialize_probe.c';pp=P/'source/movie_float_initialize_probe.c';pp.write_text(old_probe.read_text().replace('sa_movie_buffer_initialize_','sa_movie_float_initialize_'))
old_build=P/'build_movie_buffer_initialize.ps1';build=old_build.read_text().replace("'sa_movie_buffer_initialize_connected.c'","'sa_movie_float_initialize_connected.c','sa_movie_float_initialize.c'").replace('movie_buffer_initialize_probe.c','movie_float_initialize_probe.c').replace('movie_buffer_initialize.dll','movie_float_initialize.dll');bp=P/'build_movie_float_initialize.ps1';bp.write_text(build)
sources=re.findall(r"'([^']+\.c)'",build);assert len(sources)==len(set(sources))==207
files=[Path(__file__),old,old_probe,old_build,bridge,pp,bp,mp]+[P/'source'/n for n in sources]
report=dict(compiled_source_count=207,newly_registered_entries=0,source=bridge.relative_to(P).as_posix(),scope='Onlyoriginal60edb0route andexportprefixnames addedto206-sourceintegration. Platformpolicies/IRQ/context/RTE and17/18/19/20argument APIs retained. Naminginferred;separatequalificationrequired.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(P/'analysis/movie_float_initialize_connection.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated207-source drawing-state integration')
