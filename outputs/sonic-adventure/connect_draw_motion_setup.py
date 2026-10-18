"""Connect new motion controls and existing matrix/state routines, no fixtures."""
import hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
old_bridge=PROJECT/'source/sa_overlay_draw_distance_connected.c';source=old_bridge.read_text()
specs=((0x8c60e5e4,'setup'),(0x8c60e31c,'dispatch'),(0x8c60cfc4,'base_position'),(0x8c60cfe2,'base_rotation'),(0x8c60cffc,'base_rotation_yxz'),(0x8c60d016,'base_scale'),(0x8c60d034,'format_zero'))
declarations=''.join(f'DECL(sa_motion_{label}_{a:08x});\n' for a,label in specs)
declarations+='''
int sa_particle_render_state_save_8c605dac(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,void*,service);
int sa_particle_render_state_restore_8c605e0a(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,void*,service);
int sa_model_matrix_rotate(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,void*,axis_op);
int sa_matrix_rotate_yxz(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,void*,axis_op);
void sa_empty_stub_8c611708(struct sa_cpu_state*);
'''
marker='struct context {';assert source.count(marker)==1;source=source.replace(marker,declarations+marker)
routes=''.join(f' if(target==0x{a:x}u)return sa_motion_{label}_{a:08x}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);\n' for a,label in specs)
for a,label in ((0x8c605dac,'save'),(0x8c605e0a,'restore')):routes+=f' if(target==0x{a:x}u)return sa_particle_render_state_{label}_{a:08x}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);\n'
routes+=''' if(target==0x8c639cf4u)return sa_model_matrix_rotate(m,f,sr,r,c->fpul,c,axis);
 if(target==0x8c639ff8u)return sa_matrix_rotate_yxz(m,f,sr,r,c->fpul,c,axis);
 if(target==0x8c611708u){struct sa_cpu_state s;for(unsigned i=0;i<16;i++)s.r[i]=r[i];s.sr=*sr;s.pr=*pr;s.pc=target;sa_empty_stub_8c611708(&s);return 0;}
'''
marker=' return -2;';assert source.count(marker)==1;source=source.replace(marker,routes+marker).replace('sa_overlay_draw_distance_connected','sa_draw_motion_setup_connected')
bridge=PROJECT/'source/sa_draw_motion_setup_connected.c';bridge.write_text(source)
old_build=PROJECT/'build_overlay_draw_distance.ps1';build=old_build.read_text().replace("'sa_overlay_draw_distance_connected.c'","'sa_draw_motion_setup_connected.c','sa_draw_motion_setup.c','sa_main_dispatch_particle_callback.c','model_matrix_rotate.c','model_matrix_rotate_implicit.c','sa_matrix_rotate_yxz.c','retail_empty_stubs.c'").replace('overlay_draw_distance.dll','draw_motion_setup.dll')
build_path=PROJECT/'build_draw_motion_setup.ps1';build_path.write_text(build)
names=re.findall(r"'([^']+\.c)'",build);assert len(names)==len(set(names))
files=[Path(__file__),old_bridge,old_build,bridge,build_path,PROJECT/'analysis/draw_motion_setup_translation.json',PROJECT/'analysis/overlay_draw_distance_connection.json',PROJECT/'analysis/main_dispatch_particle_callback_verification.json',PROJECT/'analysis/model_matrix_rotate_verification.json',PROJECT/'analysis/model_matrix_rotate_implicit_verification.json',PROJECT/'analysis/axis_rotation_native_verification.json',PROJECT/'analysis/retail_empty_stubs_verification.json']+[PROJECT/'source'/n for n in names]
for a in (0x8c605dac,0x8c605e0a,0x8c639cf4,0x8c639ff8):files.append(PROJECT/f'analysis/draw-motion-setup/{a:08x}_instructions.tsv')
report=dict(source=bridge.relative_to(PROJECT).as_posix(),compiled_source_count=len(names),newly_recovered_entries=0,reused_routine_credit=0,candidate_recovery_credit=0,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Motion setup/recursive dispatcher/base transform callbacks connected to existing native state save/restore, matrix helpers and optional original empty callback. No game-service substitutes; remaining interpolation/mesh services stop explicitly. Older builds/proofs preserved.')
(PROJECT/'analysis/draw_motion_setup_connection.json').write_text(json.dumps(report,indent=2)+'\n')
print('Motion setup native connection generated:',len(names),'sources.')
