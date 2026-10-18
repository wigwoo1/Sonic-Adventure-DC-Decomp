"""Recover the complete signed-UV constant-factor primitive family; lower services stay explicit."""
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
for args in ((0x8C61CF9C,0x8C61D404,'sa_uv_constant_primitive_emit_8c61cf9c',531),
             (0x8C61D408,0x8C61DB92,'sa_uv_constant_primitive_clipped_8c61d408',892)):
 function,record=recover(*args)
 function=function.replace('void *ctx,sa_shared_service call){','const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){\n int io_status=0;')
 function=function.replace('(void)fpul;(void)macl;','(void)fpul;(void)macl;(void)sx;')
 for name in ('rd','sx','wr','fload','fstore'):function=function.replace(name+'(m,',name+'(m,bus,&io_status,')
 function=re.sub(r'((?:rd|sx|wr|fload|fstore)\(m,[^;]*\);)',r'\1if(io_status)return io_status;',function)
 if args[0]==0x8C61CF9C:
  probes={0x8C61CFA8:0xD32D,0x8C61CFB0:0x6041,0x8C61CFB4:0x600D,0x8C61CFB8:0x2039,0x8C61CFBC:0x8800,0x8C61CFC0:0x8D11,0x8C61CFC4:0x9146,0x8C61CFC6:0x3010,0x8C61CFC8:0x8B01,0x8C61CFCE:0xD125,0x8C61CFD0:0x3010,0x8C61CFD2:0x8B01,0x8C61CFD8:0xD121,0x8C61CFDA:0x3010,0x8C61CFDC:0x8B01}
  literals=((0x8C61D060,4,0xC000),(0x8C61D054,2,0x4000),(0x8C61D064,4,0x8000))
  impossible_rows=(0x8C61CFE2,0x8C61CFE4);impossible_edge=(0x8C61CFDC,0x8C61CFE2);ret=0x8C61D400
 else:
  probes={0x8C61D41E:0xD339,0x8C61D426:0x6041,0x8C61D42A:0x600D,0x8C61D42C:0x2039,0x8C61D42E:0x8800,0x8C61D430:0x8D11,0x8C61D434:0x915F,0x8C61D436:0x3010,0x8C61D438:0x8B01,0x8C61D43E:0xD132,0x8C61D440:0x3010,0x8C61D442:0x8B01,0x8C61D448:0xD12E,0x8C61D44A:0x3010,0x8C61D44C:0x8B01}
  literals=((0x8C61D504,4,0xC000),(0x8C61D4F6,2,0x4000),(0x8C61D508,4,0x8000))
  impossible_rows=(0x8C61D452,0x8C61D454);impossible_edge=(0x8C61D44C,0x8C61D452);ret=0x8C61DB8E
 function=function.replace(' (void)fpul;',' uint32_t entry_pr=*pr;\n (void)fpul;',1)
 function=function.replace(f'L{ret:08x}: ',f'L{ret:08x}: if(*pr!=entry_pr)return -3;',1)
 record['native_return_guard']=dict(pc=hex(ret),status=-3,scope='Stop before RTS delay-slot effects when a saved return address differs from the incoming address; host unsupported-transfer boundary.')
 for address,word in probes.items():assert int.from_bytes(binary[address-0x8C010000:address-0x8C010000+2],'little')==word,hex(address)
 for address,width,value in literals:assert int.from_bytes(binary[address-0x8C010000:address-0x8C010000+width],'little')==value
 outcomes={word&0xC000 for word in range(65536)};assert outcomes=={0,0x4000,0x8000,0xC000}
 record['syntactic_reachable_addresses']=record['reachable_addresses']
 record['reachable_addresses']=[a for a in record['reachable_addresses'] if int(a,16) not in impossible_rows]
 record['syntactic_conditional_edges']=record['conditional_edges']
 record['conditional_edges']=[e for e in record['conditional_edges'] if tuple(int(a,16) for a in e)!=impossible_edge]
 record['unreachable_proof']=dict(reason='Byte-checked switch zero-extends its input word and masks c000. All 65536 inputs yield exactly 0/4000/8000/c000; previous comparisons handle the first three. Final BF taken edge and default BRA/NOP cannot execute.',input_values_exhausted=65536,mask_values=sorted(outcomes),checked_instructions={hex(a):hex(w) for a,w in probes.items()},rows=[hex(a) for a in impossible_rows],edge=[hex(a) for a in impossible_edge])
 functions.append(function);records.append(record)
source=PROJECT/'source/sa_main_dispatch_uv_constant_primitives.c'
header=header.replace('Vertex workers with portable MMIO/store-queue interface; names inferred.','UV/constant-factor primitive family with portable queue interface; names inferred.')
source.write_text(header+'\n'.join(functions))
files=[Path(__file__),VERTEX_GENERATOR,PROJECT/'recover_main_dispatch_mesh_services.py',PROJECT/'recover_main_dispatch_particle_callback.py',BINARY,TRANSLATOR,TEMPLATE,source,PROJECT/'source/vertex_bus.h',PROJECT/'source/vertex_bus_lighting.c']+[Path(r['listing']) for r in records]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=records,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='1423 original rows statically translated; exhaustive input-mask proofs exclude four unreachable rows/two edges. Three lower UV/constant clipping/alternate services remain explicit fixtures; behavior proof separate.')
(PROJECT/'analysis/main_dispatch_uv_constant_primitives_translation.json').write_text(json.dumps(report,indent=2)+'\n')
print('Generated UV/constant family:',sum(len(r['reachable_addresses']) for r in records),'reachable rows')
