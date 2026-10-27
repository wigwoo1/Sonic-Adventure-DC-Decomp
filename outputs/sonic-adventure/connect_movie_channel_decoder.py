"""Extend the 219-source worker stage with original descriptor/decoder routes."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;mp=P/'analysis/movie_channel_decoder_translation.json';manifest=json.loads(mp.read_text());assert len(manifest['routines'])==9
old=P/'source/sa_movie_channel_worker_connected.c';text=old.read_text()
marker='#include "sh4_exception_state.h"';assert text.count(marker)==1;text=text.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in manifest['routines'])+marker)
marker=' return -2;';assert text.count(marker)==1;text=text.replace(marker,''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n' for r in manifest['routines'])+marker)
for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):text=text.replace('sa_movie_channel_worker_'+suffix,'sa_movie_channel_decoder_'+suffix)
bridge=P/'source/sa_movie_channel_decoder_connected.c';bridge.write_text(text)
old_probe=P/'source/movie_channel_worker_probe.c';probe=P/'source/movie_channel_decoder_probe.c';probe.write_text(old_probe.read_text().replace('sa_movie_channel_worker_','sa_movie_channel_decoder_'))
old_build=P/'build_movie_channel_worker.ps1';build=old_build.read_text().replace("'sa_movie_channel_worker_connected.c'","'sa_movie_channel_decoder_connected.c','sa_movie_channel_decoder.c'").replace('movie_channel_worker_probe.c','movie_channel_decoder_probe.c').replace('movie_channel_worker.dll','movie_channel_decoder.dll');bp=P/'build_movie_channel_decoder.ps1';bp.write_text(build)
sources=re.findall(r"'([^']+\.c)'",build);assert len(sources)==len(set(sources))==220
files=[Path(__file__),old,old_probe,old_build,bridge,probe,bp,mp]+[P/'source'/n for n in sources]
report=dict(compiled_source_count=220,proposed_integrated_entries=9,newly_registered_entries=0,source=bridge.relative_to(P).as_posix(),scope='Exact old219 bridge plus9real routes/export renames. Runtime/console/IRQ/context/ABI/storage policies preserved. New source is proposals until qualification;canonical routines no extra credit;no result substitutes.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(P/'analysis/movie_channel_decoder_connection.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated220-source channel-decoder integration')
