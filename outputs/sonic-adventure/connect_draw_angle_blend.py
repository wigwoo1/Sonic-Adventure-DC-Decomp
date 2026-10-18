"""Connect the original integer-angle helper to the preserved explicit-GBR pipeline."""
import hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;manifest=json.loads((PROJECT/'analysis/draw_angle_blend_translation.json').read_text())
old=PROJECT/'source/sa_draw_vector_blend_connected.c';source=old.read_text();marker='struct context {';assert source.count(marker)==1
macro='#define BDECL(name) int name(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,void*,service,uint32_t*,uint32_t*,const struct sa_vertex_bus*)\n'
source=source.replace(marker,''.join(f'BDECL({r["name"]});\n' for r in manifest['routines'])+marker)
marker=' return -2;';assert source.count(marker)==1
routes=''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus);\n' for r in manifest['routines'])
source=source.replace(marker,routes+marker).replace('sa_draw_vector_blend_connected','sa_draw_angle_blend_connected')
bridge=PROJECT/'source/sa_draw_angle_blend_connected.c';bridge.write_text(source)
old_build=PROJECT/'build_draw_vector_blend.ps1'
build=old_build.read_text().replace("'sa_draw_vector_blend_connected.c'","'sa_draw_angle_blend_connected.c','sa_draw_angle_blend.c'").replace('draw_vector_blend.dll','draw_angle_blend.dll').replace('GBR geometry native pipeline build failed','Geometry stream native pipeline build failed')
build_path=PROJECT/'build_draw_angle_blend.ps1';build_path.write_text(build);names=re.findall(r"'([^']+\.c)'",build);assert len(names)==len(set(names))==119
files=[Path(__file__),old,old_build,bridge,build_path,PROJECT/'analysis/draw_angle_blend_translation.json',PROJECT/'analysis/draw_vector_blend_connection.json']+[PROJECT/'source'/n for n in names]
report=dict(source=bridge.relative_to(PROJECT).as_posix(),compiled_source_count=len(names),research_define='SA_BOUND_RETAIL_STALLS=1',explicit_special_register='GBR',newly_recovered_entries=0,reused_routine_credit=0,candidate_recovery_credit=0,external_game_service_fixtures=0,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='One original integer-angle blend helper uses the preserved native dispatch, explicit GBR and software queue backend. No fake lower-service results; older source/ABIs/DLLs/proofs retained.')
(PROJECT/'analysis/draw_angle_blend_connection.json').write_text(json.dumps(report,indent=2)+'\n');print('Native geometry stream connection generated:',len(names),'sources.')
