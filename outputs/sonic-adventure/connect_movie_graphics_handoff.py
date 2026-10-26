"""Preserve the201-source integration and add original memory accounting."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;mp=P/'analysis/movie_graphics_handoff_translation.json';manifest=json.loads(mp.read_text())
old=P/'source/sa_movie_graphics_init_connected.c';text=old.read_text()
marker='#include "sh4_exception_state.h"';assert text.count(marker)==1
text=text.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in manifest['routines'])+marker)
marker=' return -2;';assert text.count(marker)==1
routes=''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n' for r in manifest['routines'])
text=text.replace(marker,routes+marker)
for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):
 text=text.replace('sa_movie_graphics_init_'+suffix,'sa_movie_graphics_handoff_'+suffix)
bridge=P/'source/sa_movie_graphics_handoff_connected.c';bridge.write_text(text)
old_probe=P/'source/movie_graphics_init_probe.c';probe=old_probe.read_text().replace('sa_movie_graphics_init_','sa_movie_graphics_handoff_');pp=P/'source/movie_graphics_handoff_probe.c';pp.write_text(probe)
old_build=P/'build_movie_graphics_init.ps1'
build=old_build.read_text().replace("'sa_movie_graphics_init_connected.c'","'sa_movie_graphics_handoff_connected.c','sa_movie_graphics_handoff.c'").replace('movie_graphics_init_probe.c','movie_graphics_handoff_probe.c').replace('movie_graphics_init.dll','movie_graphics_handoff.dll')
bp=P/'build_movie_graphics_handoff.ps1';bp.write_text(build)
sources=re.findall(r"'([^']+\.c)'",build);assert len(sources)==len(set(sources))==202
files=[Path(__file__),old,old_probe,old_build,bridge,pp,bp,mp]+[P/'source'/n for n in sources]
report=dict(compiled_source_count=202,newly_registered_entries=0,source=bridge.relative_to(P).as_posix(),scope='Original memory accounting queries use the actual native providers and explicit complete state; partial callers remain unqualified. Persistent transfer/interrupt,flash storage and17/18/19/20argument APIs retained. Accounting results come from original list traversal,not substituted totals.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(P/'analysis/movie_graphics_handoff_connection.json').write_text(json.dumps(report,indent=2)+'\n')
print('Generated202-source graphics accounting pipeline')
