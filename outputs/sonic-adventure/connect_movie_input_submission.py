"""Extend preserved223 integration with six submission bodies and MMIO preflight."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;mp=P/'analysis/movie_input_submission_translation.json';manifest=json.loads(mp.read_text());assert len(manifest['routines'])==7
old=P/'source/sa_movie_input_pump_connected.c';text=old.read_text();marker='#include "sh4_exception_state.h"';assert text.count(marker)==1
text=text.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in manifest['routines'])+marker)
preflight=' if(target==0x8c65f1aau)return sa_movie_input_pump_8c65f1aa(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
assert text.count(preflight)==1;text=text.replace(preflight,'')
marker=' return -2;';assert text.count(marker)==1
text=text.replace(marker,''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n' for r in manifest['routines'])+marker)
for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):text=text.replace('sa_movie_input_pump_'+suffix,'sa_movie_input_submission_'+suffix)
# Retain the captured root PR and every old route/ABI; extend membership by real calls.
allowed=json.loads((P/manifest['return_manifest']).read_text())['allowed_call_returns'];marker='static const uint32_t allowed[]={'
start=text.index(marker)+len(marker);end=text.index('};',start);text=text[:start]+','.join(x+'u' for x in allowed)+text[end:]
bridge=P/'source/sa_movie_input_submission_connected.c';bridge.write_text(text)
old_probe=P/'source/movie_input_pump_probe.c';probe=P/'source/movie_input_submission_probe.c';probe.write_text(old_probe.read_text().replace('sa_movie_input_pump_','sa_movie_input_submission_'))
old_build=P/'build_movie_input_pump.ps1';build=old_build.read_text().replace("'sa_movie_input_pump_connected.c'","'sa_movie_input_submission_connected.c','sa_movie_input_submission.c'").replace('movie_input_pump_probe.c','movie_input_submission_probe.c').replace('movie_input_pump.dll','movie_input_submission.dll');bp=P/'build_movie_input_submission.ps1';bp.write_text(build)
sources=re.findall(r"'([^']+\.c)'",build);assert len(sources)==len(set(sources))==224
files=[Path(__file__),old,old_probe,old_build,bridge,probe,bp,mp]+[P/'source'/n for n in sources]
report=dict(compiled_source_count=224,proposed_integrated_entries=6,canonical_replaced_routes=['0x8c65f1aa'],canonical_duplicate_credit=0,newly_registered_entries=0,source=bridge.relative_to(P).as_posix(),scope='Previous223bridge/ABI/runtime plus six original submission routes, one replacement of canonical preflight with original33rows and four-address MMIO guard, exportprefix and returnmembership extensions only. Original decoder/worker continuation sources retained byte-identical; no new nonlocal graphs claimed.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(P/'analysis/movie_input_submission_connection.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated224-source input pump integration')
