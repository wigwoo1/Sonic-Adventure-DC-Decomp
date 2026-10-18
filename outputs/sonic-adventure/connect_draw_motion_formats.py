"""Extend native motion controls with original format/keyframe bodies."""
import hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
manifest=json.loads((PROJECT/'analysis/draw_motion_formats_translation.json').read_text())
old_bridge=PROJECT/'source/sa_draw_motion_setup_connected.c';source=old_bridge.read_text()
declarations=''.join(f'DECL({r["name"]});\n' for r in manifest['routines'])
helpers=((0x8c10cc80,'sa_unsigned_remainder_8c10cc80'),(0x8c10cd38,'sa_engine_record_copy_dispatch'),(0x8c10cd8e,'sa_engine_record_copy8_tail'))
declarations+=''.join(f'int {name}(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,void*,service);\n' for a,name in helpers)
marker='struct context {';assert source.count(marker)==1;source=source.replace(marker,declarations+marker)
routes=''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);\n' for r in manifest['routines'])
routes+=''.join(f' if(target==0x{a:x}u)return {name}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);\n' for a,name in helpers)
marker=' return -2;';assert source.count(marker)==1;source=source.replace(marker,routes+marker).replace('sa_draw_motion_setup_connected','sa_draw_motion_formats_connected')
bridge=PROJECT/'source/sa_draw_motion_formats_connected.c';bridge.write_text(source)
old_build=PROJECT/'build_draw_motion_setup.ps1';build=old_build.read_text().replace("'sa_draw_motion_setup_connected.c'","'sa_draw_motion_formats_connected.c','sa_draw_motion_formats.c','sa_unsigned_remainder.c','sa_engine_record_copy_dispatch.c','sa_engine_record_copy8_tail.c'").replace('draw_motion_setup.dll','draw_motion_formats.dll')
build_path=PROJECT/'build_draw_motion_formats.ps1';build_path.write_text(build)
names=re.findall(r"'([^']+\.c)'",build);assert len(names)==len(set(names))
files=[Path(__file__),old_bridge,old_build,bridge,build_path,PROJECT/'analysis/draw_motion_formats_translation.json',PROJECT/'analysis/draw_motion_setup_connection.json',PROJECT/'analysis/unsigned_remainder_verification.json',PROJECT/'analysis/engine_frame_pool_queue_initialize_verification.json']+[PROJECT/'source'/n for n in names]
files.extend(PROJECT/f'analysis/draw-motion-formats/{a:08x}_instructions.tsv' for a,name in helpers)
report=dict(source=bridge.relative_to(PROJECT).as_posix(),compiled_source_count=len(names),newly_recovered_entries=0,reused_routine_credit=0,candidate_recovery_credit=0,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Original motion format routing, four samplers and four interpolators with native unsigned remainder and original eight-byte copy dispatcher/tail. No substituted game-service results; motion mesh output remains explicit. Existing artifacts preserved.')
(PROJECT/'analysis/draw_motion_formats_connection.json').write_text(json.dumps(report,indent=2)+'\n')
print('Native motion format connection generated:',len(names),'sources.')
