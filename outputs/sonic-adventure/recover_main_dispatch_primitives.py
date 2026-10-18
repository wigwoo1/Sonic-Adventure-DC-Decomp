"""Recover two particle primitive emitters from their complete retail bodies."""
import hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
VERTEX_GENERATOR=PROJECT/'recover_main_dispatch_vertex_workers.py'
prefix=VERTEX_GENERATOR.read_text().split('functions=[];records=[]')[0]
prefix=prefix.replace("'main-dispatch-vertex-workers/'","'main-dispatch-primitives/'")
exec(compile(prefix,str(VERTEX_GENERATOR),'exec'),globals())
marker="        if text.startswith(('pref ','ocbi ')):"
assert code.count(marker)==1
code=code.replace(marker,"        if text=='frchg':return 'if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;'\n"+marker)
exec(compile(code,str(TEMPLATE),'exec'),globals())
functions=[];records=[]
for args in ((0x8C61FFE4,0x8C62040A,'sa_primitive_emit_8c61ffe4',513),
             (0x8C620428,0x8C6209A2,'sa_primitive_emit_clipped_8c620428',669)):
 function,record=recover(*args)
 function=function.replace('void *ctx,sa_shared_service call){','const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){\n int io_status=0;')
 function=function.replace('(void)fpul;(void)macl;','(void)fpul;(void)macl;(void)sx;')
 for name in ('rd','sx','wr','fload','fstore'):function=function.replace(name+'(m,',name+'(m,bus,&io_status,')
 function=re.sub(r'((?:rd|sx|wr|fload|fstore)\(m,[^;]*\);)',r'\1if(io_status)return io_status;',function)
 # Original switch masks a zero-extended 16-bit word with c000, then handles
 # 0,4000,8000,c000. Its final default BRA/NOP cannot execute for any input.
 if args[0]==0x8C61FFE4:
  probes={0x8C61FFEE:0xD356,0x8C61FFF4:0x6041,0x8C61FFF8:0x600D,0x8C61FFFC:0x2039,0x8C620000:0x8800,0x8C620002:0x8D11,0x8C620006:0x9199,0x8C620008:0x3010,0x8C62000A:0x8B01,0x8C620010:0xD14E,0x8C620012:0x3010,0x8C620014:0x8B01,0x8C62001A:0xD14B,0x8C62001C:0x3010,0x8C62001E:0x8B01}
  literals=((0x8C620148,4,0xC000),(0x8C62013C,2,0x4000),(0x8C62014C,4,0x8000))
  impossible_rows=(0x8C620024,0x8C620026);impossible_edge=(0x8C62001E,0x8C620024)
 else:
  probes={0x8C620438:0x6043,0x8C62043A:0xD314,0x8C620440:0x6001,0x8C620442:0x600D,0x8C620444:0x2039,0x8C620446:0x8800,0x8C620448:0x8D11,0x8C62044C:0x911D,0x8C62044E:0x3010,0x8C620450:0x8B01,0x8C620456:0xD10E,0x8C620458:0x3010,0x8C62045A:0x8B01,0x8C620460:0xD10A,0x8C620462:0x3010,0x8C620464:0x8B01}
  literals=((0x8C62048C,4,0xC000),(0x8C62048A,2,0x4000),(0x8C620490,4,0x8000))
  impossible_rows=(0x8C62046A,0x8C62046C);impossible_edge=(0x8C620464,0x8C62046A)
 for address,word in probes.items():assert int.from_bytes(binary[address-0x8C010000:address-0x8C010000+2],'little')==word
 for address,width,value in literals:assert int.from_bytes(binary[address-0x8C010000:address-0x8C010000+width],'little')==value
 outcomes={word&0xC000 for word in range(65536)};assert outcomes=={0,0x4000,0x8000,0xC000}
 record['syntactic_reachable_addresses']=record['reachable_addresses']
 record['reachable_addresses']=[a for a in record['reachable_addresses'] if int(a,16) not in impossible_rows]
 record['syntactic_conditional_edges']=record['conditional_edges']
 record['conditional_edges']=[e for e in record['conditional_edges'] if tuple(int(a,16) for a in e)!=impossible_edge]
 record['unreachable_proof']=dict(reason='Original zero-extended 16-bit input masked by c000 yields exactly four values; prior comparisons handle 0/4000/8000, so final c000 comparison is always equal. Default BRA/NOP and final BF taken edge are impossible.',input_values_exhausted=65536,mask_values=sorted(outcomes),checked_instructions={hex(a):hex(w) for a,w in probes.items()},rows=[hex(a) for a in impossible_rows],edge=[hex(a) for a in impossible_edge])
 functions.append(function);records.append(record)
source=PROJECT/'source/sa_main_dispatch_primitives.c'
header=header.replace('Vertex workers with portable MMIO/store-queue interface; names inferred.','Particle primitive emitters with portable queue interface; names inferred.')
source.write_text(header+'\n'.join(functions))
files=[Path(__file__),VERTEX_GENERATOR,PROJECT/'recover_main_dispatch_mesh_services.py',PROJECT/'recover_main_dispatch_particle_callback.py',BINARY,TRANSLATOR,TEMPLATE,source,PROJECT/'source/vertex_bus.h',PROJECT/'source/vertex_bus.c']+[Path(r['listing']) for r in records]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=records,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='1182 byte-checked rows statically translated; 1178 behavior-reachable rows and 72 reachable outcomes, with exhaustive topology-mask proofs for four impossible rows/two edges. Three lower clipping/alternate services remain explicit; behavior proof separate.')
(PROJECT/'analysis/main_dispatch_primitives_translation.json').write_text(json.dumps(report,indent=2)+'\n')
