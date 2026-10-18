"""Original motion setup/dispatcher and non-interpolated transform callbacks."""
import hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;PARENT_TEMPLATE=PROJECT/'recover_draw_matrix_registers.py'
exec(compile(PARENT_TEMPLATE.read_text().split('functions=[];routines=[]')[0],str(PARENT_TEMPLATE),'exec'),globals())
header=header.replace('Complete register-input matrix scale and translation; names inferred.','Motion setup, recursive dispatch and base callbacks; names inferred.')
chosen=chosen.replace('main-dispatch-model-traversal/','draw-motion-setup/')
exec(compile(chosen,str(TEMPLATE),'exec'),globals())
specs=((0x8c60e5e4,0x8c60e6da,123,'setup'),(0x8c60e31c,0x8c60e3d2,91,'dispatch'),(0x8c60cfc4,0x8c60cfe2,15,'base_position'),(0x8c60cfe2,0x8c60cffc,13,'base_rotation'),(0x8c60cffc,0x8c60d016,13,'base_rotation_yxz'),(0x8c60d016,0x8c60d034,15,'base_scale'),(0x8c60d034,0x8c60d054,16,'format_zero'))
functions=[];routines=[]
for start,end,count,label in specs:
 function,record=recover(start,end,f'sa_motion_{label}_{start:08x}',count)
 function=function.replace('(void)entry_pr;','(void)entry_pr;(void)sr;(void)sx;');functions.append(function);routines.append(record)
source=PROJECT/'source/sa_draw_motion_setup.c';source.write_text(header+'\n'.join(functions))
files=[Path(__file__),PARENT_TEMPLATE,PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',TRANSLATOR,TEMPLATE,BINARY,source,PROJECT/'ghidra/export_draw_motion_setup.ps1',PROJECT/'ghidra/SeedCallbackFunctions.java',PROJECT/'ghidra/ExportFailedFunctionEvidence.java']+[Path(r['listing']) for r in routines]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=routines,instruction_rows=286,newly_recovered_entries=7,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Seven complete static guarded bodies / 286 original rows. Motion setup, recursive object driver, four base transforms and format-zero callback chain. Interpolation handlers and motion mesh drawing remain distinct lower dependencies; behavior evidence separate.')
(PROJECT/'analysis/draw_motion_setup_translation.json').write_text(json.dumps(report,indent=2)+'\n')
