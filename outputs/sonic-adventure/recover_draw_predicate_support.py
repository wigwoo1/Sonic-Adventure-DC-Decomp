"""Original draw predicate and float-state wrapper/copy; candidate draw body."""
import hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;PARENT=PROJECT/'recover_draw_matrix_registers.py'
exec(compile(PARENT.read_text().split('functions=[];routines=[]')[0],str(PARENT),'exec'),globals())
header=header.replace('Complete register-input matrix scale and translation; names inferred.','Draw predicate and four-float state wrapper/copy; names inferred.')
chosen=chosen.replace('main-dispatch-model-traversal/','draw-predicate-support/')
exec(compile(chosen,str(TEMPLATE),'exec'),globals())
functions=[];routines=[]
for start,end,count,label in ((0x8c038d00,0x8c038da0,66,'predicate'),(0x8c043a20,0x8c043a40,16,'float_state_wrapper'),(0x8c60edf0,0x8c60ee0a,13,'float_state_copy')):
 function,record=recover(start,end,f'sa_draw_{label}_{start:08x}',count)
 function=function.replace('(void)entry_pr;', '(void)entry_pr;(void)sr;(void)sx;')
 functions.append(function);routines.append(record)
source=PROJECT/'source/sa_draw_predicate_support.c';source.write_text(header+'\n'.join(functions))
files=[Path(__file__),PROJECT/'recover_draw_matrix_registers.py',PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',BINARY,TRANSLATOR,TEMPLATE,source,PROJECT/'ghidra/export_draw_predicate_support.ps1',PROJECT/'ghidra/ExportFailedFunctionEvidence.java']+[Path(r['listing']) for r in routines]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=routines,instruction_rows=95,newly_recovered_entries=3,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Three complete byte-checked guarded native bodies: draw predicate, four-float stack wrapper and original global copy. Behavior proof separate; inferred names.')
(PROJECT/'analysis/draw_predicate_support_translation.json').write_text(json.dumps(report,indent=2)+'\n')
