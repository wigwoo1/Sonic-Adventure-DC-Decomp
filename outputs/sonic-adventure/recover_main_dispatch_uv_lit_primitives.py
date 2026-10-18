"""Recover the complete normal-lit primitive family; lower services stay explicit."""
import hashlib,json,re,csv
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
VERTEX_GENERATOR=PROJECT/'recover_main_dispatch_vertex_workers.py'
prefix=VERTEX_GENERATOR.read_text().split('functions=[];records=[]')[0]
prefix=prefix.replace("'main-dispatch-vertex-workers/'","'main-dispatch-next-primitives/'")
exec(compile(prefix,str(VERTEX_GENERATOR),'exec'),globals())
marker="        if text.startswith(('pref ','ocbi ')):"
assert code.count(marker)==1
code=code.replace(marker,"        if text=='frchg':return 'if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;'\n"+marker)
exec(compile(code,str(TEMPLATE),'exec'),globals())
functions=[];records=[]
for args in ((0x8C618D18,0x8C6192E0,'sa_uv_lit_primitive_emit_8c618d18',708),
             (0x8C619310,0x8C619BF6,'sa_uv_lit_primitive_clipped_8c619310',1067)):
 function,record=recover(*args)
 function=function.replace('void *ctx,sa_shared_service call){','const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){\n int io_status=0;')
 function=function.replace('(void)fpul;(void)macl;','(void)fpul;(void)macl;(void)sx;')
 for name in ('rd','sx','wr','fload','fstore'):function=function.replace(name+'(m,',name+'(m,bus,&io_status,')
 function=re.sub(r'((?:rd|sx|wr|fload|fstore)\(m,[^;]*\);)',r'\1if(io_status)return io_status;',function)
 if args[0]==0x8C618D18:
  probes={0x8C618D24:0xD32D,0x8C618D2C:0x6041,0x8C618D30:0x600D,0x8C618D34:0x2039,0x8C618D38:0x8800,0x8C618D3A:0x8D11,0x8C618D3E:0x9148,0x8C618D40:0x3010,0x8C618D42:0x8B01,0x8C618D48:0xD125,0x8C618D4A:0x3010,0x8C618D4C:0x8B01,0x8C618D52:0xD122,0x8C618D54:0x3010,0x8C618D56:0x8B01}
  literals=((0x8C618DDC,4,0xC000),(0x8C618DD2,2,0x4000),(0x8C618DE0,4,0x8000))
  impossible_rows=(0x8C618D5C,0x8C618D5E);impossible_edge=(0x8C618D56,0x8C618D5C)
 else:
  probes={0x8C619324:0xD336,0x8C61932A:0x6041,0x8C61932E:0x600D,0x8C619330:0x2039,0x8C619332:0x8800,0x8C619334:0x8D11,0x8C619338:0x915F,0x8C61933A:0x3010,0x8C61933C:0x8B01,0x8C619342:0xD130,0x8C619344:0x3010,0x8C619346:0x8B01,0x8C61934C:0xD12C,0x8C61934E:0x3010,0x8C619350:0x8B01}
  literals=((0x8C619400,4,0xC000),(0x8C6193FA,2,0x4000),(0x8C619404,4,0x8000))
  impossible_rows=(0x8C619356,0x8C619358);impossible_edge=(0x8C619350,0x8C619356)
 for address,word in probes.items():assert int.from_bytes(binary[address-0x8C010000:address-0x8C010000+2],'little')==word,hex(address)
 for address,width,value in literals:assert int.from_bytes(binary[address-0x8C010000:address-0x8C010000+width],'little')==value
 outcomes={word&0xC000 for word in range(65536)};assert outcomes=={0,0x4000,0x8000,0xC000}
 record['syntactic_reachable_addresses']=record['reachable_addresses']
 record['reachable_addresses']=[a for a in record['reachable_addresses'] if int(a,16) not in impossible_rows]
 record['syntactic_conditional_edges']=record['conditional_edges']
 record['conditional_edges']=[e for e in record['conditional_edges'] if tuple(int(a,16) for a in e)!=impossible_edge]
 record['unreachable_proof']=dict(reason='Byte-checked switch zero-extends its input word and masks c000. All 65536 inputs yield exactly 0/4000/8000/c000; previous comparisons handle the first three. Final BF taken edge and default BRA/NOP cannot execute.',input_values_exhausted=65536,mask_values=sorted(outcomes),checked_instructions={hex(a):hex(w) for a,w in probes.items()},rows=[hex(a) for a in impossible_rows],edge=[hex(a) for a in impossible_edge])
 functions.append(function);records.append(record)
source=PROJECT/'source/sa_main_dispatch_uv_lit_primitives.c'
header=header.replace('Vertex workers with portable MMIO/store-queue interface; names inferred.','UV/lit primitive family with portable queue interface; names inferred.')
source.write_text(header+'\n'.join(functions))
files=[Path(__file__),VERTEX_GENERATOR,PROJECT/'recover_main_dispatch_mesh_services.py',PROJECT/'recover_main_dispatch_particle_callback.py',BINARY,TRANSLATOR,TEMPLATE,source,PROJECT/'source/vertex_bus.h',PROJECT/'source/vertex_bus_lighting.c']+[Path(r['listing']) for r in records]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=records,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='1775 original rows statically translated; exhaustive input-mask proofs exclude four unreachable rows/two edges. Three lower UV/clipping/alternate services remain explicit fixtures; behavior proof separate.')
(PROJECT/'analysis/main_dispatch_uv_lit_primitives_translation.json').write_text(json.dumps(report,indent=2)+'\n')
print('Generated UV/lit family:',sum(len(r['reachable_addresses']) for r in records),'reachable rows')
