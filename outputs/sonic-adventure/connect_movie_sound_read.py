"""Retain the193-source runtime and connect three existing original sound getters."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;mp=P/'analysis/movie_sound_read_translation.json';manifest=json.loads(mp.read_text())
old=P/'source/sa_movie_transfer_runtime_connected.c';text=old.read_text();declarations=''.join(f'VDECL({r["name"]});\n' for r in manifest['routines'])
marker='#include "sh4_exception_state.h"';assert text.count(marker)==1;text=text.replace(marker,declarations+marker)
routes=''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n' for r in manifest['routines'])
marker=' return -2;';assert text.count(marker)==1;text=text.replace(marker,routes+marker)
text=text.replace('sa_movie_transfer_runtime_connected','sa_movie_sound_read_connected').replace('sa_movie_transfer_runtime_initialized_connected','sa_movie_sound_read_initialized_connected').replace('sa_movie_transfer_runtime_exception_connected','sa_movie_sound_read_exception_connected').replace('sa_movie_transfer_runtime_running_connected','sa_movie_sound_read_running_connected')
bridge=P/'source/sa_movie_sound_read_connected.c';bridge.write_text(text)
old_probe=P/'source/movie_transfer_runtime_probe.c';probe=old_probe.read_text().replace('sa_movie_transfer_runtime','sa_movie_sound_read');pp=P/'source/movie_sound_read_probe.c';pp.write_text(probe)
old_build=P/'build_movie_transfer_runtime.ps1';build=old_build.read_text().replace("'sa_movie_transfer_runtime_connected.c'","'sa_movie_sound_read_connected.c','sa_movie_sound_read.c'").replace('movie_transfer_runtime_probe.c','movie_sound_read_probe.c').replace('movie_transfer_runtime.dll','movie_sound_read.dll');bp=P/'build_movie_sound_read.ps1';bp.write_text(build)
sources=re.findall(r"'([^']+\.c)'",build);assert len(sources)==len(set(sources))==194
files=[Path(__file__),old,old_probe,old_build,bridge,pp,bp,mp]+[P/'source'/n for n in sources]
report=dict(compiled_source_count=194,newly_registered_entries=0,source=bridge.relative_to(P).as_posix(),scope='Existingfield667150,byte667188andword66910cgetter bodies call actual connected10d460andmappedSoundRAM. Previouspersistent transfer/IRQ paths retained; no lower result is supplied. Canonical existing getters receive zero additional recovery credit.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(P/'analysis/movie_sound_read_connection.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated194-source sound-read/runtime pipeline')
