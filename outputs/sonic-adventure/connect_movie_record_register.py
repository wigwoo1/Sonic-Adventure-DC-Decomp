"""Extend verified210-source integration with original record registration routes."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;mp=P/'analysis/movie_record_register_translation.json';manifest=json.loads(mp.read_text());assert len(manifest['routines'])>=20
old=P/'source/sa_movie_slot_initialize_connected.c';text=old.read_text()
marker='#include "sh4_exception_state.h"';assert text.count(marker)==1;text=text.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in manifest['routines'])+marker)
marker=' return -2;';assert text.count(marker)==1
text=text.replace(marker,''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n' for r in manifest['routines'])+marker)
for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):text=text.replace('sa_movie_slot_initialize_'+suffix,'sa_movie_record_register_'+suffix)
bridge=P/'source/sa_movie_record_register_connected.c';bridge.write_text(text)
old_probe=P/'source/movie_slot_initialize_probe.c';probe=P/'source/movie_record_register_probe.c';probe.write_text(old_probe.read_text().replace('sa_movie_slot_initialize_','sa_movie_record_register_'))
old_build=P/'build_movie_slot_initialize.ps1';build=old_build.read_text().replace("'sa_movie_slot_initialize_connected.c'","'sa_movie_record_register_connected.c','sa_movie_record_register.c'").replace('movie_slot_initialize_probe.c','movie_record_register_probe.c').replace('movie_slot_initialize.dll','movie_record_register.dll');bp=P/'build_movie_record_register.ps1';bp.write_text(build)
sources=re.findall(r"'([^']+\.c)'",build);assert len(sources)==len(set(sources))==211
files=[Path(__file__),old,old_probe,old_build,bridge,probe,bp,mp]+[P/'source'/n for n in sources]
report=dict(compiled_source_count=211,proposed_integrated_entries=len(manifest['routines']),newly_registered_entries=0,source=bridge.relative_to(P).as_posix(),scope='Only original guest dispatch routes and export names extend210-source integration. Existing firmware/platform/IRQ/exception/storage policies and complete APIs remain unchanged. Proposals, not qualification or duplicate canonical credit; no substitute helper returns.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(P/'analysis/movie_record_register_connection.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated211-source record registration integration')
