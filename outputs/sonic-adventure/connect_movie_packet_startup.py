"""Retain the194-source integration and add native original flash-packet bodies."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent
mp=P/'analysis/movie_packet_startup_translation.json';manifest=json.loads(mp.read_text())
old=P/'source/sa_movie_sound_read_connected.c';text=old.read_text()
marker='#include "sh4_exception_state.h"';assert text.count(marker)==1
text=text.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in manifest['routines'])+marker)
marker=' return -2;';assert text.count(marker)==1
routes=''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n' for r in manifest['routines'])
text=text.replace(marker,routes+marker)
for suffix in ('connected','initialized_connected','exception_connected','running_connected'):
 text=text.replace('sa_movie_sound_read_'+suffix,'sa_movie_packet_startup_'+suffix)
bridge=P/'source/sa_movie_packet_startup_connected.c';bridge.write_text(text)
old_probe=P/'source/movie_sound_read_probe.c';probe=old_probe.read_text().replace('sa_movie_sound_read','sa_movie_packet_startup')
pp=P/'source/movie_packet_startup_probe.c';pp.write_text(probe)
old_build=P/'build_movie_sound_read.ps1'
build=old_build.read_text().replace("'sa_movie_sound_read_connected.c'","'sa_movie_packet_startup_connected.c','sa_movie_packet_startup.c'").replace('movie_sound_read_probe.c','movie_packet_startup_probe.c').replace('movie_sound_read.dll','movie_packet_startup.dll')
bp=P/'build_movie_packet_startup.ps1';bp.write_text(build)
sources=re.findall(r"'([^']+\.c)'",build);assert len(sources)==len(set(sources))==195
files=[Path(__file__),old,old_probe,old_build,bridge,pp,bp,mp]+[P/'source'/n for n in sources]
report=dict(compiled_source_count=195,newly_registered_entries=0,source=bridge.relative_to(P).as_posix(),scope='Adds24original flash-packet bodies to the previous194-source integration. Reused canonical files/proofs unchanged; source proposals receive no duplicate credit. Unsupported flash selectors remain actual platform boundaries until separately implemented and tested.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(P/'analysis/movie_packet_startup_connection.json').write_text(json.dumps(report,indent=2)+'\n')
print('Generated195-source packet startup pipeline')
