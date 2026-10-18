"""Recover complete alternate strip emitters, reusing the original empty stub."""
import hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
VERTEX_GENERATOR=PROJECT/'recover_main_dispatch_vertex_workers.py'
prefix=VERTEX_GENERATOR.read_text().split('functions=[];records=[]')[0]
prefix=prefix.replace("'main-dispatch-vertex-workers/'","'main-dispatch-face-lit-lower/'")
exec(compile(prefix,str(VERTEX_GENERATOR),'exec'),globals())
marker="        if text.startswith('fmov'):";assert code.count(marker)==1
code=code.replace(marker,"        if text.startswith('fneg '):return f'if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][{n}]^=0x80000000u;'\n"+marker)
code=code.replace("        if text.startswith(('pref ','ocbi ')):","        if text.startswith('ocbwb '):return f'if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[{n}],2u);if(status)return status;'\n        if text.startswith(('pref ','ocbi ')):")
exec(compile(code,str(TEMPLATE),'exec'),globals())
functions=[];records=[]
for args in ((0x8C613DFA,0x8C614076,'sa_face_alternate_strip_8c613dfa',300),
             (0x8C61572C,0x8C615B84,'sa_face_alternate_strip_clipped_8c61572c',530)):
 function,record=recover(*args)
 function=function.replace('void *ctx,sa_shared_service call){','const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){\n int io_status=0;')
 function=function.replace('(void)fpul;(void)macl;','(void)fpul;(void)macl;(void)sx;')
 for name in ('rd','sx','wr','fload','fstore'):function=function.replace(name+'(m,',name+'(m,bus,&io_status,')
 function=re.sub(r'((?:rd|sx|wr|fload|fstore)\(m,[^;]*\);)',r'\1if(io_status)return io_status;',function)
 functions.append(function);records.append(record)
source=PROJECT/'source/sa_main_dispatch_face_alternate_strips.c'
header=header.replace('Vertex workers with portable MMIO/store-queue interface; names inferred.','Face-selected alternate strip emitters with portable queue interface; names inferred.')
source.write_text(header+'\n'.join(functions))
assert binary[0x8C611708-0x8C010000:0x8C61170C-0x8C010000]==bytes.fromhex('0b000900')
files=[Path(__file__),VERTEX_GENERATOR,PROJECT/'recover_main_dispatch_mesh_services.py',PROJECT/'recover_main_dispatch_particle_callback.py',BINARY,TRANSLATOR,TEMPLATE,source,PROJECT/'source/vertex_bus.h',PROJECT/'source/vertex_bus_lighting.c',PROJECT/'source/retail_empty_stubs.c']+[Path(r['listing']) for r in records]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=records,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},reused_stub=dict(address='0x8c611708',retail_bytes='0b000900',source='source/retail_empty_stubs.c',new_recovery_credit=0),scope='830 byte-checked rows statically translated. Existing empty stub and recovered near-plane clipper are reused; runtime behavior proof separate.')
(PROJECT/'analysis/main_dispatch_face_alternate_strips_translation.json').write_text(json.dumps(report,indent=2)+'\n')
