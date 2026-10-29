"""Extend the preserved264 bridge with original contact-response proposals."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;mp=P/'analysis/movement_special_tail_translation.json';manifest=json.loads(mp.read_text());assert len(manifest['routines'])==1
old=P/'source/sa_movement_contact_candidates_connected.c';text=old.read_text();marker='#include "sh4_exception_state.h"';assert text.count(marker)==1
text=text.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in manifest['routines'])+marker)
text=text.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in manifest['resume_routes'])+marker)
marker=' return -2;';assert text.count(marker)==1
text=text.replace(marker,''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n' for r in manifest['routines'])+marker)
marker=' if(c->observe)c->observe(c->user,target,m,f,sr,r,pr);';assert text.count(marker)==1
text=text.replace(marker,''.join(f' if(target=={r["pc"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n' for r in manifest['resume_routes'])+marker)
for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):text=text.replace('sa_movement_contact_candidates_'+suffix,'sa_movement_special_tail_'+suffix)
# Retain the captured root PR and every old route/ABI; extend membership by real calls.
allowed=json.loads((P/manifest['return_manifest']).read_text())['allowed_call_returns'];marker='static const uint32_t allowed[]={'
start=text.index(marker)+len(marker);end=text.index('};',start);text=text[:start]+','.join(x+'u' for x in allowed)+text[end:]
bridge=P/'source/sa_movement_special_tail_connected.c';bridge.write_text(text)
old_probe=P/'source/movement_contact_candidates_probe.c';probe=P/'source/movement_special_tail_probe.c';probe.write_text(old_probe.read_text().replace('sa_movement_contact_candidates_','sa_movement_special_tail_'))
old_build=P/'build_movement_contact_candidates.ps1';build=old_build.read_text().replace("'sa_movement_contact_candidates_connected.c'","'sa_movement_special_tail_connected.c','sa_movement_special_tail.c'").replace('movement_contact_candidates_probe.c','movement_special_tail_probe.c').replace('movement_contact_candidates.dll','movement_special_tail.dll');bp=P/'build_movement_special_tail.ps1';bp.write_text(build)
sources=re.findall(r"'([^']+\.c)'",build);assert len(sources)==len(set(sources))==265
files=[Path(__file__),old,old_probe,old_build,bridge,probe,bp,mp]+[P/'source'/n for n in sources]
report=dict(resume_routes=manifest['resume_routes'],compiled_source_count=265,proposed_integrated_entries=1,canonical_replaced_routes=[],canonical_duplicate_credit=0,canonical_reused_entries=manifest['canonical_reused_entries'],newly_registered_entries=0,source=bridge.relative_to(P).as_posix(),scope='Previous264bridge/runtime/ABI plus original movement-parameter owner withactualcontinuations/JSRreturns. Previoussources/frames/guards/runtime unchanged;allproposalsgetzerocreditpendingqualification.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(P/'analysis/movement_special_tail_connection.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated265-source contact-response integration')
