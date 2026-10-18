"""Recover distinct motion-mesh preprocessing and state wrapper."""
import hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;PARENT_TEMPLATE=PROJECT/'recover_draw_matrix_registers.py'
exec(compile(PARENT_TEMPLATE.read_text().split('functions=[];routines=[]')[0],str(PARENT_TEMPLATE),'exec'),globals())
header=header.replace('Complete register-input matrix scale and translation; names inferred.','Motion mesh matrix/light preprocessing and wrapper; names inferred.')
chosen=chosen.replace('main-dispatch-model-traversal/','draw-motion-mesh/')
exec(compile(chosen,str(TEMPLATE),'exec'),globals())
functions=[];routines=[]
for start,end,count,label in ((0x8c617c34,0x8c617e46,259,'prepare'),(0x8c621574,0x8c62159a,19,'wrapper')):
 function,record=recover(start,end,f'sa_motion_mesh_{label}_{start:08x}',count)
 function=function.replace('(void)entry_pr;','(void)entry_pr;(void)sr;(void)sx;');functions.append(function);routines.append(record)
source=PROJECT/'source/sa_draw_motion_mesh.c';source.write_text(header+'\n'.join(functions))
files=[Path(__file__),PARENT_TEMPLATE,PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',TRANSLATOR,TEMPLATE,BINARY,source,PROJECT/'ghidra/export_draw_motion_mesh.ps1',PROJECT/'ghidra/SeedCallbackFunctions.java',PROJECT/'ghidra/ExportFailedFunctionEvidence.java']+[Path(r['listing']) for r in routines]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=routines,instruction_rows=278,newly_recovered_entries=2,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Two complete static guarded original bodies: motion mesh matrix/light preprocessing and render-state wrapper, 278 byte-checked rows. Existing lower mesh wrapper/pipeline is reused rather than substituted for preprocessing; behavior evidence separate.')
(PROJECT/'analysis/draw_motion_mesh_translation.json').write_text(json.dumps(report,indent=2)+'\n')
