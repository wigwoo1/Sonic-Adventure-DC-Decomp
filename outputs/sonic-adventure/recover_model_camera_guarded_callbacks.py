"""Reuse original callback rows with reached memory guards; preserve old copies."""
import hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;PARENT=PROJECT/'recover_draw_matrix_registers.py'
exec(compile(PARENT.read_text().split('functions=[];routines=[]')[0],str(PARENT),'exec'),globals())
chosen=chosen.replace('main-dispatch-model-traversal/','main-dispatch-particle-callback/')
exec(compile(chosen,str(TEMPLATE),'exec'),globals())
header=header.replace('Complete register-input matrix scale and translation; names inferred.','Guarded original particle callbacks and rendering-state wrappers; existing rows reused.')
header+='\n#include "sa_draw_motion_stop.h"\nSA_STAGE_EXPORT int sa_particle_work_update_8c0448ba(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,sa_shared_service);\n'
functions=[];records=[]
for args in ((0x8c04481a,0x8c0448b4,'sa_particle_primary_8c04481a',27),(0x8c0448b4,0x8c0449c2,'sa_particle_secondary_8c0448b4',135),(0x8c6215d6,0x8c6215fa,'sa_particle_draw_wrapper_8c6215d6',18),(0x8c605dac,0x8c605e0a,'sa_particle_render_state_save_8c605dac',47),(0x8c605e0a,0x8c605e68,'sa_particle_render_state_restore_8c605e0a',47)):
 function,record=recover(*args)
 marker='void *ctx,sa_shared_service call,uint32_t *stop){';assert function.count(marker)==1
 function=function.replace(marker,'void *ctx,sa_shared_service call){').replace('(void)entry_pr;','(void)entry_pr;(void)sx;')
 function=re.sub(r'\*stop=(0x[0-9a-f]+u);',r'sa_motion_publish_stop(ctx,\1);',function);assert '*stop' not in function
 if args[0]==0x8c0448b4:
  function=function.replace('SA_STAGE_EXPORT int sa_particle_secondary_8c0448b4','static int secondary_engine')
  function=function.replace('void *ctx,sa_shared_service call){','void *ctx,sa_shared_service call,uint32_t entry){')
  function=function.replace(' goto L8c0448b4;',' if(entry==0x8c0448bau)goto L8c0448ba;goto L8c0448b4;',1)
  for name,entry in (('sa_particle_secondary_8c0448b4',0x8c0448b4),('sa_particle_work_update_8c0448ba',0x8c0448ba)):
   function+=f'\nSA_STAGE_EXPORT int {name}(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,sa_shared_service call){{return secondary_engine(m,f,sr,r,pr,fpul,macl,ctx,call,0x{entry:x}u);}}\n'
  record['shared_internal_entry']='0x8c0448ba'
 if args[0]==0x8c04481a:
  old='if(!call)return -2;return call(ctx,0x8c0448bau,m,f,sr,r,pr);';assert function.count(old)==1
  function=function.replace(old,'return sa_particle_work_update_8c0448ba(m,f,sr,r,pr,fpul,macl,ctx,call);')
 functions.append(function);records.append(record)
source=PROJECT/'source/sa_model_camera_guarded_callbacks.c';assert not source.exists();source.write_text(header+'\n'.join(functions))
recipe=PROJECT/'build_model_callback_roots.ps1';build=PROJECT/'build_model_camera_dependency.ps1';assert not build.exists()
build.write_text(recipe.read_text().replace('sa_motion_reuse_sa_main_dispatch_particle_callback.c','sa_model_camera_guarded_callbacks.c').replace('model_callback_roots.dll','model_camera_dependency.dll'))
files=[Path(__file__),PARENT,PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',TEMPLATE,TRANSLATOR,BINARY,source,recipe,build,PROJECT/'source/sa_draw_motion_stop.h']+[Path(r['listing']) for r in records]
report=dict(status='byte_checked_guarded_callback_copy_behavior_proof_separate',routines=records,original_rows_reused=274,new_original_routine_credit=0,prior_sources_preserved=True,scope='Reached dynamic RAM access and RTS guards publish the original PC; the camera pointer is never supplied or replaced.',sha256={str(f):hashlib.sha256(f.read_bytes()).hexdigest() for f in files})
out=PROJECT/'analysis/model_camera_guarded_callbacks_translation.json';assert not out.exists();out.write_text(json.dumps(report,indent=2)+'\n')
