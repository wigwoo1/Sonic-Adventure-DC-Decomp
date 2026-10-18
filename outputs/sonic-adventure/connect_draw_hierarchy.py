"""Extend the native drawing chain with original hierarchy matrix preparation."""
import hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
manifest=json.loads((PROJECT/'analysis/draw_hierarchy_translation.json').read_text())
old=PROJECT/'source/sa_draw_motion_mesh_connected.c';source=old.read_text()
declarations=''.join(f'DECL({r["name"]});\n' for r in manifest['routines'])+'uint32_t sa_model_channel_difference_8c055d4e(uint32_t*);\n'
marker='struct context {';assert source.count(marker)==1;source=source.replace(marker,declarations+marker)
routes=''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);\n' for r in manifest['routines'])
routes+=' if(target==0x8c055d4eu){(void)sa_model_channel_difference_8c055d4e(r);return 0;}\n'
marker=' return -2;';assert source.count(marker)==1;source=source.replace(marker,routes+marker).replace('sa_draw_motion_mesh_connected','sa_draw_hierarchy_connected')
bridge=PROJECT/'source/sa_draw_hierarchy_connected.c';bridge.write_text(source)
old_build=PROJECT/'build_draw_motion_mesh.ps1';build=old_build.read_text().replace("'sa_draw_motion_mesh_connected.c'","'sa_draw_hierarchy_connected.c','sa_draw_hierarchy.c','model_channel_difference.c'").replace('draw_motion_mesh.dll','draw_hierarchy.dll').replace('-shared -O2','-shared -O2 -DSA_BOUND_RETAIL_STALLS=1')
build_path=PROJECT/'build_draw_hierarchy.ps1';build_path.write_text(build);names=re.findall(r"'([^']+\.c)'",build);assert len(names)==len(set(names))
files=[Path(__file__),old,old_build,bridge,build_path,PROJECT/'analysis/draw_hierarchy_translation.json',PROJECT/'analysis/draw_motion_mesh_connection.json',PROJECT/'analysis/model_channel_difference_verification.json',PROJECT/'analysis/model_matrix_bank_save_verification.json',PROJECT/'analysis/draw-hierarchy/8c055d4e_instructions.tsv']+[PROJECT/'source'/n for n in names]
report=dict(source=bridge.relative_to(PROJECT).as_posix(),compiled_source_count=len(names),research_define='SA_BOUND_RETAIL_STALLS=1',newly_recovered_entries=0,reused_routine_credit=0,candidate_recovery_credit=0,external_game_service_fixtures=0,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Original hierarchy preparation connected to native matrix, angle-difference and existing material/primitive/queue chain. Real vector numerical backend, no injected FSCA/FTRV results. Research-only bound on the original unsupported-layout stall; source production default retains it. Prior artifacts preserved.')
(PROJECT/'analysis/draw_hierarchy_connection.json').write_text(json.dumps(report,indent=2)+'\n')
print('Native hierarchy connection generated:',len(names),'sources.')
