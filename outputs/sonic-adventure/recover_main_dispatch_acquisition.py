"""Recover the first material-header acquisition path; no opcode interpreter."""
import hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
VERTEX_GENERATOR=PROJECT/'recover_main_dispatch_vertex_workers.py'
prefix=VERTEX_GENERATOR.read_text().split('functions=[];records=[]')[0]
# The inherited translator checks every listing byte against the pinned binary.
prefix=prefix.replace("'main-dispatch-vertex-workers/'","'main-dispatch-primitives/'")
exec(compile(prefix,str(VERTEX_GENERATOR),'exec'),globals())
function,record=recover(0x8C605E68,0x8C605EB4,'sa_material_acquire_8c605e68',38)
function=function.replace('void *ctx,sa_shared_service call){','const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){\n int io_status=0;')
function=function.replace('(void)fpul;(void)macl;','(void)fpul;(void)macl;(void)sr;(void)sx;(void)fload;(void)fstore;')
for name in ('rd','sx','wr','fload','fstore'):function=function.replace(name+'(m,',name+'(m,bus,&io_status,')
function=re.sub(r'((?:rd|sx|wr|fload|fstore)\(m,[^;]*\);)',r'\1if(io_status)return io_status;',function)
source=PROJECT/'source/sa_main_dispatch_acquisition.c'
header=header.replace('Vertex workers with portable MMIO/store-queue interface; names inferred.','Material-header acquisition with portable queue interface; name inferred.')
source.write_text(header+function)
files=[Path(__file__),VERTEX_GENERATOR,PROJECT/'recover_main_dispatch_mesh_services.py',PROJECT/'recover_main_dispatch_particle_callback.py',BINARY,TRANSLATOR,TEMPLATE,source,PROJECT/'source/vertex_bus.h',PROJECT/'source/vertex_bus.c',Path(record['listing'])]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=[record],sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='38 byte-checked rows translated statically. Three header words are written and one PREF flushes a whole 32-byte queue; five previous queue words intentionally persist. Behavior proof separate.')
(PROJECT/'analysis/main_dispatch_acquisition_translation.json').write_text(json.dumps(report,indent=2)+'\n')

