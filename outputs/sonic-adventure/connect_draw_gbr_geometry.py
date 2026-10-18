"""Extend the native renderer bridge with explicit caller-owned GBR state."""
import hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;manifest=json.loads((PROJECT/'analysis/draw_gbr_geometry_translation.json').read_text())
old=PROJECT/'source/sa_draw_motion_sampling_connected.c';source=old.read_text()
marker='struct context {';assert source.count(marker)==1
decl='#define GDECL(name) int name(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,void*,service,uint32_t*,uint32_t*)\n'
source=source.replace(marker,decl+''.join(f'GDECL({r["name"]});\n' for r in manifest['routines'])+marker)
marker='const struct sa_vertex_bus *bus;};';assert source.count(marker)==1
source=source.replace(marker,'const struct sa_vertex_bus *bus;uint32_t *gbr;};')
marker=' return -2;';assert source.count(marker)==1
routes=''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr);\n' for r in manifest['routines'])
source=source.replace(marker,routes+marker).replace('sa_draw_motion_sampling_connected','sa_draw_gbr_geometry_connected')
marker='observer observe,const struct sa_vertex_bus *bus){';assert source.count(marker)==1
source=source.replace(marker,'observer observe,const struct sa_vertex_bus *bus,uint32_t *gbr){')
marker='struct context c={&c.stop,fpul,macl,entry,user,observe,bus};';assert source.count(marker)==1
source=source.replace(marker,'struct context c={&c.stop,fpul,macl,entry,user,observe,bus,gbr};')
bridge=PROJECT/'source/sa_draw_gbr_geometry_connected.c';bridge.write_text(source)
old_build=PROJECT/'build_draw_motion_sampling.ps1'
build=old_build.read_text().replace("'sa_draw_motion_sampling_connected.c'","'sa_draw_gbr_geometry_connected.c','sa_draw_gbr_geometry.c'").replace('draw_motion_sampling.dll','draw_gbr_geometry.dll').replace('Motion sampling native pipeline build failed','GBR geometry native pipeline build failed')
build_path=PROJECT/'build_draw_gbr_geometry.ps1';build_path.write_text(build);names=re.findall(r"'([^']+\.c)'",build);assert len(names)==len(set(names))==112
files=[Path(__file__),old,old_build,bridge,build_path,PROJECT/'analysis/draw_gbr_geometry_translation.json',PROJECT/'analysis/draw_motion_sampling_connection.json']+[PROJECT/'source'/n for n in names]
report=dict(source=bridge.relative_to(PROJECT).as_posix(),compiled_source_count=len(names),research_define='SA_BOUND_RETAIL_STALLS=1',explicit_special_register='GBR',new_public_argument='Final uint32_t* gbr appended after the existing bus parameter; older source/API unchanged.',newly_recovered_entries=0,reused_routine_credit=0,candidate_recovery_credit=0,external_game_service_fixtures=0,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Original renderer setup/geometry with caller-owned GBR state through the existing native callback and queue pipeline. Unknown reached lower services remain explicit. No fixed or implicit synthesized GBR value; prior source, APIs, DLLs and proofs preserved.')
(PROJECT/'analysis/draw_gbr_geometry_connection.json').write_text(json.dumps(report,indent=2)+'\n')
print('Native explicit-GBR connection generated:',len(names),'sources.')
