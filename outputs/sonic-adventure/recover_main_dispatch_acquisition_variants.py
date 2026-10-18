"""Recover three complete command-header acquisition variants as static C."""
import hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
VERTEX_GENERATOR=PROJECT/'recover_main_dispatch_vertex_workers.py'
prefix=VERTEX_GENERATOR.read_text().split('functions=[];records=[]')[0]
prefix=prefix.replace("'main-dispatch-vertex-workers/'","'main-dispatch-acquisition-variants/'")
exec(compile(prefix,str(VERTEX_GENERATOR),'exec'),globals())
functions=[];records=[]
for args in ((0x8C605F3C,0x8C605F80,'sa_material_acquire_secondary_8c605f3c',34),
             (0x8C6064B0,0x8C606518,'sa_material_acquire_extended_8c6064b0',52),
             (0x8C6065B0,0x8C606610,'sa_material_acquire_extended_secondary_8c6065b0',48)):
 function,record=recover(*args)
 function=function.replace('void *ctx,sa_shared_service call){','const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){\n int io_status=0;')
 function=function.replace('(void)fpul;(void)macl;','(void)fpul;(void)macl;(void)sr;(void)sx;(void)fload;(void)fstore;')
 for name in ('rd','sx','wr','fload','fstore'):function=function.replace(name+'(m,',name+'(m,bus,&io_status,')
 function=re.sub(r'((?:rd|sx|wr|fload|fstore)\(m,[^;]*\);)',r'\1if(io_status)return io_status;',function)
 functions.append(function);records.append(record)
source=PROJECT/'source/sa_main_dispatch_acquisition_variants.c'
header=header.replace('Vertex workers with portable MMIO/store-queue interface; names inferred.','Command-header acquisition variants with portable queues; names inferred.')
source.write_text(header+'\n'.join(functions))
files=[Path(__file__),VERTEX_GENERATOR,PROJECT/'recover_main_dispatch_mesh_services.py',PROJECT/'recover_main_dispatch_particle_callback.py',BINARY,TRANSLATOR,TEMPLATE,source,PROJECT/'source/vertex_bus.h',PROJECT/'source/vertex_bus.c']+[Path(r['listing']) for r in records]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=records,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='134 byte-checked rows statically translated. Three-/eight-word packets and independent cursor slots preserved; behavior proof separate.')
(PROJECT/'analysis/main_dispatch_acquisition_variants_translation.json').write_text(json.dumps(report,indent=2)+'\n')
