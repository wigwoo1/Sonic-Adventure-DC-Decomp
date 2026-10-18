"""Recover geometry preparation and the R8-preserving model wrapper."""
import hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
GENERATOR=PROJECT/'recover_main_dispatch_particle_callback.py'
# Reuse the frozen byte/CFG translator without executing its output-writing tail.
prefix=GENERATOR.read_text().split('functions=[];records=[]')[0]
assert prefix.count("'main-dispatch-particle-callback/'")==1
prefix=prefix.replace("'main-dispatch-particle-callback/'","'main-dispatch-particle-geometry/'")
marker="        if text.startswith('fcmp/gt '):"
assert prefix.count(marker)==1
prefix=prefix.replace(marker,"""        if text.startswith('fmac '):
            return f'{{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[{m}],&fr[{n}],&f->fpscr,&fr[{n}]);if(status)return status;}}'
"""+marker)
exec(compile(prefix,str(GENERATOR),'exec'),globals())
header=header.replace('#include "sh4_add_sub.h"','#include "sh4_add_sub.h"\n#include "sh4_fmac.h"')
header=header.replace('Particle callback/update/draw/state wrappers; names inferred.','Geometry preparation and R8-preserving model wrapper; names inferred.')
header=header.replace('SA_STAGE_EXPORT int sa_particle_work_update_8c0448ba(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,sa_shared_service);\n','')
functions=[];records=[]
for args in ((0x8C620F2C,0x8C62113C,'sa_particle_geometry_prepare_8c620f2c',258),
             (0x8C6215FA,0x8C621614,'sa_particle_model_wrapper_8c6215fa',13)):
 function,record=recover(*args)
 if args[0]==0x8C620F2C:record['literal_pool_gap']=['0x8c62110c','0x8c621118']
 functions.append(function);records.append(record)
source=PROJECT/'source/sa_main_dispatch_particle_geometry.c';source.write_text(header+'\n'.join(functions))
files=[Path(__file__),GENERATOR,BINARY,TRANSLATOR,TEMPLATE,source]+[Path(r['listing']) for r in records]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=records,
            sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},
            scope='271 original rows checked and statically translated; 12-byte literal gap excluded from instruction count. Native behavior verification is separate; model traversal 8c62113c remains a service.')
(PROJECT/'analysis/main_dispatch_particle_geometry_translation.json').write_text(json.dumps(report,indent=2)+'\n')
