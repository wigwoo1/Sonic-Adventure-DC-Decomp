"""Complete guarded register-input scale/translation bodies for object drawing."""
import hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
PARENT=PROJECT/'recover_overlay_model_blend.py'
exec(compile(PARENT.read_text().split('function,blend_record=recover')[0],str(PARENT),'exec'),globals())
BINARY=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';binary=BINARY.read_bytes()
assert hashlib.sha256(binary).hexdigest()=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
header=header.replace('AL_MAIN model morph and word-copy dispatcher; names inferred.','Complete register-input matrix scale and translation; names inferred.')
header=header.replace('#include "sh4_add_sub.h"','#include "sh4_add_sub.h"\n#include "sh4_vector_numeric.h"')
chosen=code.replace('overlay-initializer-dependencies/','main-dispatch-model-traversal/')
marker="        assert not text.startswith('f'),text"
assert chosen.count(marker)==1
chosen=chosen.replace(marker,"""        if text=='fschg':return 'if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;'
        if text.startswith(('fipr ','ftrv ')):return f'status=sa_sh4_vector_numeric(0x{w:04x}u,f,*fpul);if(status)return status;'
"""+marker)
exec(compile(chosen,str(TEMPLATE),'exec'),globals())
functions=[];routines=[]
for start,end,count,label in ((0x8c63a69c,0x8c63a742,83,'scale'),(0x8c63a878,0x8c63a8da,49,'translate')):
 function,record=recover(start,end,f'sa_draw_matrix_{label}_registers_{start:08x}',count)
 function=function.replace('(void)entry_pr;', '(void)entry_pr;(void)sx;')
 functions.append(function);routines.append(record)
source=PROJECT/'source/sa_draw_matrix_registers.c';source.write_text(header+'\n'.join(functions))
files=[Path(__file__),PARENT,PROJECT/'recover_main_dispatch_particle_callback.py',TRANSLATOR,TEMPLATE,BINARY,source]+[Path(r['listing']) for r in routines]
prior=[('source/model_matrix_scale_registers.c','analysis/model_matrix_scale_registers_verification.json'),('source/model_hierarchy_position_native.c','analysis/model_hierarchy_position_native_verification.json')]
files.extend(PROJECT/p for pair in prior for p in pair)
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=routines,instruction_rows=132,newly_registered_entries=2,reused_prior_implementation_credit=0,prior_limited_implementations=[dict(source=s,verification=v) for s,v in prior],sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='132 byte-checked original rows statically translated with dynamic RAM, reached arithmetic and return guards. Both implicit-XMTRX and explicit matrix paths. Existing limited sources preserved; vector arithmetic uses pinned native numerical support.')
(PROJECT/'analysis/draw_matrix_registers_translation.json').write_text(json.dumps(report,indent=2)+'\n')
