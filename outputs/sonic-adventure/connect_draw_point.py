"""Connect original point transformation to the native hierarchy/draw chain."""
import hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
old=PROJECT/'source/sa_draw_hierarchy_connected.c';source=old.read_text()
marker='struct context {';assert source.count(marker)==1
source=source.replace(marker,'DECL(sa_draw_point_transform_8c638ecc);\n'+marker)
marker=' return -2;';assert source.count(marker)==1
source=source.replace(marker,' if(target==0x8c638eccu)return sa_draw_point_transform_8c638ecc(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);\n'+marker).replace('sa_draw_hierarchy_connected','sa_draw_point_connected')
bridge=PROJECT/'source/sa_draw_point_connected.c';bridge.write_text(source)
old_build=PROJECT/'build_draw_hierarchy.ps1';build=old_build.read_text().replace("'sa_draw_hierarchy_connected.c'","'sa_draw_point_connected.c','sa_draw_point.c'").replace('draw_hierarchy.dll','draw_point.dll').replace('Motion mesh native pipeline build failed','Point-transform native pipeline build failed')
build_path=PROJECT/'build_draw_point.ps1';build_path.write_text(build);names=re.findall(r"'([^']+\.c)'",build);assert len(names)==len(set(names))==105
files=[Path(__file__),old,old_build,bridge,build_path,PROJECT/'analysis/draw_point_translation.json',PROJECT/'analysis/draw_hierarchy_connection.json']+[PROJECT/'source'/n for n in names]
report=dict(source=bridge.relative_to(PROJECT).as_posix(),compiled_source_count=len(names),research_define='SA_BOUND_RETAIL_STALLS=1',newly_recovered_entries=0,reused_routine_credit=0,candidate_recovery_credit=0,external_game_service_fixtures=0,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Original point-transform body connected to the native hierarchy, matrix/motion and complete existing material/primitive software queue chain. Real vector core with no injected vector or game-service results. Existing source/DLL/proofs remain unchanged.')
(PROJECT/'analysis/draw_point_connection.json').write_text(json.dumps(report,indent=2)+'\n')
print('Native point-transform connection generated:',len(names),'sources.')
