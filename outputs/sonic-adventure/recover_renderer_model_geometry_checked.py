"""Checked original geometry copy; preserve all earlier valid-RAM implementations."""
import hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;PARENT=PROJECT/'recover_draw_matrix_registers.py'
exec(compile(PARENT.read_text().split('functions=[];routines=[]')[0],str(PARENT),'exec'),globals())
chosen=chosen.replace('main-dispatch-model-traversal/','main-dispatch-particle-geometry/')
exec(compile(chosen,str(TEMPLATE),'exec'),globals())
header=header.replace('Complete register-input matrix scale and translation; names inferred.','Guarded model geometry preparation; existing rows reused without new credit.')
header+='\nvoid sa_motion_publish_stop(void *,uint32_t);\n'
functions=[];routines=[]
for args in ((0x8c620f2c,0x8c62113c,'sa_particle_geometry_prepare_8c620f2c',258),(0x8c6215fa,0x8c621614,'sa_particle_model_wrapper_8c6215fa',13)):
 function,record=recover(*args)
 old='void *ctx,sa_shared_service call,uint32_t *stop){';assert function.count(old)==1
 function=function.replace(old,'void *ctx,sa_shared_service call){').replace('(void)entry_pr;','(void)entry_pr;(void)sx;')
 function=re.sub(r'\*stop=(0x[0-9a-f]+u);',r'sa_motion_publish_stop(ctx,\1);',function);assert '*stop' not in function
 functions.append(function);routines.append(record)
source=PROJECT/'source/sa_renderer_model_geometry_checked.c';assert not source.exists();source.write_text(header+'\n'.join(functions))
files=[Path(__file__),PARENT,PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',BINARY,TRANSLATOR,TEMPLATE,source]+[Path(r['listing']) for r in routines]
report=dict(status='byte_checked_guarded_geometry_copy_behavior_proof_separate',routines=routines,original_rows_reused=271,new_original_routine_credit=0,prior_sources_preserved=True,scope='Dynamic RAM reads/writes are checked before their reached instruction; original stop PC published through the existing ABI. The original slot8c8ea3e0 is not supplied or replaced.',sha256={str(f):hashlib.sha256(f.read_bytes()).hexdigest() for f in files})
target=PROJECT/'analysis/renderer_model_geometry_checked_translation.json';assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n')
