"""Connect original effect bodies without substituting missing lower services."""
import hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;manifest=json.loads((PROJECT/'analysis/draw_object_effects_translation.json').read_text())
old=PROJECT/'source/sa_draw_point_connected.c';source=old.read_text();marker='struct context {';assert source.count(marker)==1
marker='target==0x8c6215d6u';assert source.count(marker)==1
source=source.replace(marker,marker+'||target==0x8c10d334u')
marker='struct context {'
source=source.replace(marker,''.join(f'DECL({r["name"]});\n' for r in manifest['routines'])+marker)
marker=' return -2;';assert source.count(marker)==1
routes=''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);\n' for r in manifest['routines'])
source=source.replace(marker,routes+marker).replace('sa_draw_point_connected','sa_draw_object_effects_connected')
bridge=PROJECT/'source/sa_draw_object_effects_connected.c';bridge.write_text(source)
old_build=PROJECT/'build_draw_point.ps1';build=old_build.read_text().replace("'sa_draw_point_connected.c'","'sa_draw_object_effects_connected.c','sa_draw_object_effects.c'").replace('draw_point.dll','draw_object_effects.dll').replace('Point-transform native pipeline build failed','Object-effect native pipeline build failed')
build_path=PROJECT/'build_draw_object_effects.ps1';build_path.write_text(build);names=re.findall(r"'([^']+\.c)'",build);assert len(names)==len(set(names))==106
files=[Path(__file__),old,old_build,bridge,build_path,PROJECT/'analysis/draw_object_effects_translation.json',PROJECT/'analysis/draw_point_connection.json',PROJECT/'analysis/random_u15_8c10d334_verification.json',PROJECT/'analysis/draw-effect-rng/8c10d334_instructions.tsv',PROJECT/'ghidra/export_draw_effect_rng.ps1']+[PROJECT/'source'/n for n in names]
report=dict(source=bridge.relative_to(PROJECT).as_posix(),compiled_source_count=len(names),research_define='SA_BOUND_RETAIL_STALLS=1',newly_recovered_entries=0,reused_routine_credit=0,candidate_recovery_credit=0,external_game_service_fixtures=0,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Twelve original effect/dispatcher/render-service bodies connected to the existing native point/hierarchy/motion/material/primitive chain. Missing lower services stay reached -2 boundaries; no fabricated game or vector outputs. Per-body verification/registration separate.')
(PROJECT/'analysis/draw_object_effects_connection.json').write_text(json.dumps(report,indent=2)+'\n')
print('Native object-effect connection generated:',len(names),'sources.')
