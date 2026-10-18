"""Recover the complete near-plane triangle clipping helper as static C."""
import hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
VERTEX_GENERATOR=PROJECT/'recover_main_dispatch_vertex_workers.py'
prefix=VERTEX_GENERATOR.read_text().split('functions=[];records=[]')[0]
prefix=prefix.replace("'main-dispatch-vertex-workers/'","'main-dispatch-uv-lower/'")
exec(compile(prefix,str(VERTEX_GENERATOR),'exec'),globals())
function,record=recover(0x8C611A14,0x8C611F24,'sa_uv_triangle_near_clip_8c611a14',616)
function=function.replace('void *ctx,sa_shared_service call){','const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){\n int io_status=0;')
function=function.replace('(void)fpul;(void)macl;','(void)fpul;(void)macl;(void)sx;')
for name in ('rd','sx','wr','fload','fstore'):function=function.replace(name+'(m,',name+'(m,bus,&io_status,')
function=re.sub(r'((?:rd|sx|wr|fload|fstore)\(m,[^;]*\);)',r'\1if(io_status)return io_status;',function)
# Wide FMOV fixtures can overwrite the saved pointer array in the original
# stack frame. Stop before the reached unsupported RAM transfer, as the filter
# integration already does; do not invent physical hardware exception dispatch.
function=function.replace('L8c611b9a: ','L8c611b9a: if(((r[2]+12u)&0x1f000000u)!=0x0c000000u)return -3;',1)
record['native_saved_pointer_guard']=dict(pc='0x8c611b9a',status=-3,scope='Before flags load through a saved vertex pointer outside modeled RAM; unsupported host transfer, not a hardware exception model.')
source=PROJECT/'source/sa_main_dispatch_uv_near_clip.c'
header=header.replace('Vertex workers with portable MMIO/store-queue interface; names inferred.','UV/lit triangle near-plane clipper with portable queue interface; name inferred.')
source.write_text(header+function)
files=[Path(__file__),VERTEX_GENERATOR,PROJECT/'recover_main_dispatch_mesh_services.py',PROJECT/'recover_main_dispatch_particle_callback.py',BINARY,TRANSLATOR,TEMPLATE,source,PROJECT/'source/vertex_bus.h',PROJECT/'source/vertex_bus_lighting.c',Path(record['listing'])]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=[record],sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='616 byte-checked original instruction rows translated statically; no external function calls. Behavior verification separate.')
(PROJECT/'analysis/main_dispatch_uv_near_clip_translation.json').write_text(json.dumps(report,indent=2)+'\n')
