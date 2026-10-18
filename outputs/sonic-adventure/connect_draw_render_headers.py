"""Connect real render-header dependencies to existing original effect bodies."""
import hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;manifest=json.loads((PROJECT/'analysis/draw_render_headers_translation.json').read_text())
old=PROJECT/'source/sa_draw_object_effects_connected.c';source=old.read_text();marker='struct context {';assert source.count(marker)==1
source=source.replace(marker,''.join(f'DECL({r["name"]});\n' for r in manifest['routines'])+marker)
marker=' return -2;';assert source.count(marker)==1
routes=''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);\n' for r in manifest['routines'])
source=source.replace(marker,routes+marker).replace('sa_draw_object_effects_connected','sa_draw_render_headers_connected');bridge=PROJECT/'source/sa_draw_render_headers_connected.c';bridge.write_text(source)
old_build=PROJECT/'build_draw_object_effects.ps1';build=old_build.read_text().replace("'sa_draw_object_effects_connected.c'","'sa_draw_render_headers_connected.c','sa_draw_render_headers.c'").replace('draw_object_effects.dll','draw_render_headers.dll').replace('Object-effect native pipeline build failed','Render-header native pipeline build failed')
build_path=PROJECT/'build_draw_render_headers.ps1';build_path.write_text(build);names=re.findall(r"'([^']+\.c)'",build);assert len(names)==len(set(names))==107
files=[Path(__file__),old,old_build,bridge,build_path,PROJECT/'analysis/draw_render_headers_translation.json',PROJECT/'analysis/draw_object_effects_connection.json']+[PROJECT/'source'/n for n in names]
report=dict(source=bridge.relative_to(PROJECT).as_posix(),compiled_source_count=len(names),research_define='SA_BOUND_RETAIL_STALLS=1',newly_recovered_entries=0,reused_routine_credit=0,candidate_recovery_credit=0,external_game_service_fixtures=0,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Expanded original state/header/visibility/converter bodies connected to the existing native effect, point, hierarchy and material/primitive software queues. Missing lower services stay explicit reached boundaries. Prior DLLs/source/proofs preserved.')
(PROJECT/'analysis/draw_render_headers_connection.json').write_text(json.dumps(report,indent=2)+'\n')
print('Native render-header connection generated:',len(names),'sources.')
