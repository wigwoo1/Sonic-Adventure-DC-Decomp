"""Recover four vertex workers with an explicit portable queue/memory interface."""
import hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;GENERATOR=PROJECT/'recover_main_dispatch_mesh_services.py'
prefix=GENERATOR.read_text().split('JUMPS=')[0]
prefix=prefix.replace("'main-dispatch-mesh-services/'","'main-dispatch-vertex-workers/'")
exec(compile(prefix,str(GENERATOR),'exec'),globals())
# Add cache instructions to the same byte-checked static translator.
old="        assert not text.startswith('f'),text";assert code.count(old)==1
code=code.replace(old,"""        if text.startswith(('pref ','ocbi ')):
            op=1 if text.startswith('pref ') else 0
            return f'if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[{n}],{op}u);if(status)return status;'
"""+old)
exec(compile(code,str(TEMPLATE),'exec'),globals())
header=header.replace('#include "fpu_cleanup_transfer.h"','#include "fpu_cleanup_transfer.h"\n#include "vertex_bus.h"\n#include "sh4_vector_numeric.h"\n#include "sh4_fmac.h"\n#include "sh4_divide.h"')
header=header.replace('Particle callback/update/draw/state wrappers; names inferred.','Vertex workers with portable MMIO/store-queue interface; names inferred.')
header=header.replace('SA_STAGE_EXPORT int sa_particle_work_update_8c0448ba(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,sa_shared_service);\n','')
# Keep guest RAM direct, route only non-RAM accesses through the supplied bus.
header=header.replace('uint8_t *m,uint32_t a,','uint8_t *m,const struct sa_vertex_bus *bus,int *io_status,uint32_t a,')
header=header.replace(' uint32_t value=0;for(unsigned i=0;i<n;i++)', ''' if((a&0x1f000000u)!=0x0c000000u){uint32_t value=0;if(!bus||!bus->read){*io_status=-2;return 0;}*io_status=bus->read(bus->context,a,n,&value);return value;}
 uint32_t value=0;for(unsigned i=0;i<n;i++)''',1)
header=header.replace(' for(unsigned i=0;i<n;i++)m[', ''' if((a&0x1f000000u)!=0x0c000000u){if(!bus||!bus->write){*io_status=-2;return;}*io_status=bus->write(bus->context,a,value,n);return;}
 for(unsigned i=0;i<n;i++)m[''',1)
header=header.replace('rd(m,','rd(m,bus,io_status,').replace('wr(m,','wr(m,bus,io_status,')
header=header.replace('uint8_t *m,struct sa_fpu_banks *f','uint8_t *m,const struct sa_vertex_bus *bus,int *io_status,struct sa_fpu_banks *f')
functions=[];records=[]
for args in ((0x8C611060,0x8C61111E,'sa_vertex_project_normals_8c611060',84),
             (0x8C611122,0x8C6111D2,'sa_vertex_project_8c611122',78),
             (0x8C6111D6,0x8C6112DE,'sa_vertex_classify_normals_8c6111d6',118),
             (0x8C6112E2,0x8C6113DE,'sa_vertex_classify_8c6112e2',112)):
 function,record=recover(*args)
 function=function.replace('void *ctx,sa_shared_service call){','const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){\n int io_status=0;')
 function=function.replace('(void)fpul;(void)macl;','(void)fpul;(void)macl;(void)sr;(void)sx;')
 for name in ('rd','sx','wr','fload','fstore'):function=function.replace(name+'(m,',name+'(m,bus,&io_status,')
 function=re.sub(r'((?:rd|sx|wr|fload|fstore)\(m,[^;]*\);)',r'\1if(io_status)return io_status;',function)
 functions.append(function);records.append(record)
source=PROJECT/'source/sa_main_dispatch_vertex_workers.c';source.write_text(header+'\n'.join(functions))
files=[Path(__file__),GENERATOR,BINARY,TRANSLATOR,TEMPLATE,source,PROJECT/'source/vertex_bus.h',PROJECT/'source/vertex_bus.c']+[Path(r['listing']) for r in records]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=records,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},
            scope='392 byte-checked rows compiled statically. Non-RAM reads/writes and cache/prefetch use explicit portable bus; behavior proof separate.',
            hardware_reference='https://www.renesas.com/en/document/mah/sh7750-sh7750s-sh7750r-group-users-manual-hardware',reference_sections='4.7.1-4.7.3, printed pages 142-144; MMU-off bounded software queue model only.')
(PROJECT/'analysis/main_dispatch_vertex_workers_translation.json').write_text(json.dumps(report,indent=2)+'\n')
