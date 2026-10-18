"""Recover the complete environment-UV constant-factor primitive family; lower services stay explicit."""
import hashlib,json,re,csv
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
VERTEX_GENERATOR=PROJECT/'recover_main_dispatch_vertex_workers.py'
prefix=VERTEX_GENERATOR.read_text().split('functions=[];records=[]')[0]
prefix=prefix.replace("'main-dispatch-vertex-workers/'","'main-dispatch-remaining-primitives/'")
exec(compile(prefix,str(VERTEX_GENERATOR),'exec'),globals())
marker="        if text.startswith(('pref ','ocbi ')):"
assert code.count(marker)==1
code=code.replace(marker,"        if text=='frchg':return 'if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;'\n"+marker)
exec(compile(code,str(TEMPLATE),'exec'),globals())
functions=[];records=[]
for args in ((0x8C61F108,0x8C61F7AC,'sa_environment_constant_primitive_emit_8c61f108',818),
             (0x8C61F7C8,0x8C61FFB4,'sa_environment_constant_primitive_clipped_8c61f7c8',971)):
 function,record=recover(*args)
 function=function.replace('void *ctx,sa_shared_service call){','const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){\n int io_status=0;')
 function=function.replace('(void)fpul;(void)macl;','(void)fpul;(void)macl;(void)sx;')
 for name in ('rd','sx','wr','fload','fstore'):function=function.replace(name+'(m,',name+'(m,bus,&io_status,')
 function=re.sub(r'((?:rd|sx|wr|fload|fstore)\(m,[^;]*\);)',r'\1if(io_status)return io_status;',function)
 if args[0]==0x8C61F108:
  probes={0x8C61F116:0xD318,0x8C61F11C:0x6041,0x8C61F120:0x600D,0x8C61F124:0x2039,0x8C61F128:0x8800,0x8C61F12C:0x8D11,0x8C61F130:0x911B,0x8C61F132:0x3010,0x8C61F134:0x8B01,0x8C61F13A:0xD110,0x8C61F13C:0x3010,0x8C61F13E:0x8B01,0x8C61F144:0xD10C,0x8C61F146:0x3010,0x8C61F148:0x8B01}
  literals=((0x8C61F178,4,0xC000),(0x8C61F16A,2,0x4000),(0x8C61F17C,4,0x8000))
  impossible_rows=(0x8C61F14E,0x8C61F150);impossible_edge=(0x8C61F148,0x8C61F14E);ret=0x8C61F7A8
 else:
  probes={0x8C61F7DE:0xD316,0x8C61F7E8:0x6001,0x8C61F7EC:0x600D,0x8C61F7EE:0x2039,0x8C61F7F0:0x8800,0x8C61F7F2:0x8D11,0x8C61F7F6:0x911B,0x8C61F7F8:0x3010,0x8C61F7FA:0x8B01,0x8C61F800:0xD10E,0x8C61F802:0x3010,0x8C61F804:0x8B01,0x8C61F80A:0xD10B,0x8C61F80C:0x3010,0x8C61F80E:0x8B01}
  literals=((0x8C61F838,4,0xC000),(0x8C61F830,2,0x4000),(0x8C61F83C,4,0x8000))
  impossible_rows=(0x8C61F814,0x8C61F816);impossible_edge=(0x8C61F80E,0x8C61F814);ret=0x8C61FFB0
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
source=PROJECT/'source/sa_main_dispatch_environment_constant_primitives.c'
header=header.replace('Vertex workers with portable MMIO/store-queue interface; names inferred.','Environment-UV/constant-factor primitive family with portable queue interface; names inferred.')
source.write_text(header+'\n'.join(functions))
files=[Path(__file__),VERTEX_GENERATOR,PROJECT/'recover_main_dispatch_mesh_services.py',PROJECT/'recover_main_dispatch_particle_callback.py',BINARY,TRANSLATOR,TEMPLATE,source,PROJECT/'source/vertex_bus.h',PROJECT/'source/vertex_bus_lighting.c']+[Path(r['listing']) for r in records]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=records,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='1789 original rows statically translated; exhaustive input-mask proofs exclude four unreachable rows/two edges. Three lower environment/constant clipping/alternate services remain explicit fixtures; behavior proof separate.')
(PROJECT/'analysis/main_dispatch_environment_constant_primitives_translation.json').write_text(json.dumps(report,indent=2)+'\n')
print('Generated environment/constant family:',sum(len(r['reachable_addresses']) for r in records),'reachable rows')
