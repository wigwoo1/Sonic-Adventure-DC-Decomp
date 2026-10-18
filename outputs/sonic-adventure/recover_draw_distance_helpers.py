"""Complete original distance and signed-square-root control/state effects."""
import hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
TEMPLATE_PARENT=PROJECT/'recover_draw_matrix_registers.py'
exec(compile(TEMPLATE_PARENT.read_text().split('functions=[];routines=[]')[0],str(TEMPLATE_PARENT),'exec'),globals())
header=header.replace('Complete register-input matrix scale and translation; names inferred.','Distance helper and signed-square-root tail; names inferred.')
header+='int sa_sh4_sqrt(uint32_t,uint32_t*,uint32_t*);\n'
chosen=chosen.replace('main-dispatch-model-traversal/','overlay-draw-distance-helpers/')
marker="        assert not text.startswith('f'),text";assert chosen.count(marker)==1
chosen=chosen.replace(marker,"""        if text.startswith('fabs '):return f'if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][{n}]&=0x7fffffffu;'
        if text.startswith('fsqrt '):return f'{{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_sqrt(fr[{n}],&f->fpscr,&fr[{n}]);if(status)return status;}}'
"""+marker)
exec(compile(chosen,str(TEMPLATE),'exec'),globals())
functions=[];routines=[]
for start,end,count,label in ((0x8c640128,0x8c640168,32,'distance'),(0x8c63a9c4,0x8c63a9d8,10,'signed_sqrt')):
 function,record=recover(start,end,f'sa_draw_{label}_{start:08x}',count)
 function=function.replace('(void)entry_pr;', '(void)entry_pr;(void)sr;(void)sx;(void)m;(void)r;');functions.append(function);routines.append(record)
source=PROJECT/'source/sa_draw_distance_helpers.c';source.write_text(header+'\n'.join(functions))
files=[Path(__file__),TEMPLATE_PARENT,PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',TRANSLATOR,TEMPLATE,BINARY,source,PROJECT/'source/sh4_sqrt.c',PROJECT/'source/scalar_sqrt_wrapper.c',PROJECT/'analysis/scalar_sqrt_wrapper_verification.json',PROJECT/'analysis/native_sh4_sqrt_verification.json',PROJECT/'verify_native_sh4_sqrt.py',PROJECT/'ghidra/export_overlay_draw_distance_helpers.ps1',PROJECT/'ghidra/ExportFailedFunctionEvidence.java']+[Path(r['listing']) for r in routines]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=routines,instruction_rows=42,newly_registered_entries=2,new_distance_instruction_rows=32,completed_existing_sqrt_rows=10,prior_limited_sqrt_credit=0,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='42 original rows statically translated with reached RAM/FPU/return guards and native FSQRT. New distance body and completed guarded representation of existing limited signed-sqrt source; prior source/proof and native arithmetic backend preserved. Behavior proof separate.')
(PROJECT/'analysis/draw_distance_helpers_translation.json').write_text(json.dumps(report,indent=2)+'\n')
