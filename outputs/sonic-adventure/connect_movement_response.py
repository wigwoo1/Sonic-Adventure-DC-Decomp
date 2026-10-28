"""Extend the preserved252 bridge with original contact-response proposals."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;mp=P/'analysis/movement_response_translation.json';manifest=json.loads(mp.read_text());assert len(manifest['routines'])==5
old=P/'source/sa_movement_world_connected.c';text=old.read_text();marker='#include "sh4_exception_state.h"';assert text.count(marker)==1
text=text.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in manifest['routines'])+marker)
text=text.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in manifest['resume_routes'])+marker)
marker=' return -2;';assert text.count(marker)==1
text=text.replace(marker,''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n' for r in manifest['routines'])+marker)
marker=' if(c->observe)c->observe(c->user,target,m,f,sr,r,pr);';assert text.count(marker)==1
text=text.replace(marker,''.join(f' if(target=={r["pc"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n' for r in manifest['resume_routes'])+marker)
for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):text=text.replace('sa_movement_world_'+suffix,'sa_movement_response_'+suffix)
# Retain the captured root PR and every old route/ABI; extend membership by real calls.
allowed=json.loads((P/manifest['return_manifest']).read_text())['allowed_call_returns'];marker='static const uint32_t allowed[]={'
start=text.index(marker)+len(marker);end=text.index('};',start);text=text[:start]+','.join(x+'u' for x in allowed)+text[end:]
bridge=P/'source/sa_movement_response_connected.c';bridge.write_text(text)
old_probe=P/'source/movement_world_probe.c';probe=P/'source/movement_response_probe.c';probe.write_text(old_probe.read_text().replace('sa_movement_world_','sa_movement_response_'))
old_build=P/'build_movement_world.ps1';build=old_build.read_text().replace("'sa_movement_world_connected.c'","'sa_movement_response_connected.c','sa_movement_response.c'").replace('movement_world_probe.c','movement_response_probe.c').replace('movement_world.dll','movement_response.dll');bp=P/'build_movement_response.ps1';bp.write_text(build)
sources=re.findall(r"'([^']+\.c)'",build);assert len(sources)==len(set(sources))==253
files=[Path(__file__),old,old_probe,old_build,bridge,probe,bp,mp]+[P/'source'/n for n in sources]
report=dict(resume_routes=manifest['resume_routes'],compiled_source_count=253,proposed_integrated_entries=5,canonical_replaced_routes=[],canonical_duplicate_credit=0,canonical_reused_entries=manifest['canonical_reused_entries'],newly_registered_entries=0,source=bridge.relative_to(P).as_posix(),scope='Previous252bridge/runtime/ABI plus original contact-response owner,signed-angle,cross/length,vector-add andcanonicalcosineadapter withactualcontinuations/JSRreturns. Previoussources/frames/guards/runtime unchanged;allproposalsgetzerocreditpendingqualification.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(P/'analysis/movement_response_connection.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated253-source contact-response integration')
