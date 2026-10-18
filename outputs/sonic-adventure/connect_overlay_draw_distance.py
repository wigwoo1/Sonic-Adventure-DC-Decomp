"""Extend accepted-draw integration with native distance/signed-square-root."""
import hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
old_bridge=PROJECT/'source/sa_overlay_draw_state_connected.c';source=old_bridge.read_text()
marker='struct context {';assert source.count(marker)==1
source=source.replace(marker,'DECL(sa_draw_distance_8c640128);\nDECL(sa_draw_signed_sqrt_8c63a9c4);\n'+marker)
marker=' return -2;';assert source.count(marker)==1
source=source.replace(marker,' if(target==0x8c640128u)return sa_draw_distance_8c640128(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);\n if(target==0x8c63a9c4u)return sa_draw_signed_sqrt_8c63a9c4(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);\n'+marker)
source=source.replace('sa_overlay_draw_state_connected','sa_overlay_draw_distance_connected')
bridge=PROJECT/'source/sa_overlay_draw_distance_connected.c';bridge.write_text(source)
old_build=PROJECT/'build_overlay_draw_state.ps1';build=old_build.read_text().replace("'sa_overlay_draw_state_connected.c'","'sa_overlay_draw_distance_connected.c','sa_draw_distance_helpers.c','sh4_sqrt.c'").replace('overlay_draw_state.dll','overlay_draw_distance.dll')
build_path=PROJECT/'build_overlay_draw_distance.ps1';build_path.write_text(build)
names=re.findall(r"'([^']+\.c)'",build)
files=[Path(__file__),old_bridge,old_build,bridge,build_path,PROJECT/'analysis/overlay_draw_state_connection.json',PROJECT/'analysis/draw_distance_helpers_translation.json']+[PROJECT/'source'/n for n in names]
report=dict(source=bridge.relative_to(PROJECT).as_posix(),compiled_source_count=len(names),newly_recovered_entries=0,reused_routine_credit=0,candidate_recovery_credit=0,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Separate partial AL_MAIN draw build adds complete native distance/signed-sqrt routes. Prior bridge/build/proofs remain preserved; all remaining services are explicit stops. Behavioral proof separate.')
(PROJECT/'analysis/overlay_draw_distance_connection.json').write_text(json.dumps(report,indent=2)+'\n')
print('Native distance draw integration generated:',len(names),'sources; zero reused/candidate recovery credit.')
