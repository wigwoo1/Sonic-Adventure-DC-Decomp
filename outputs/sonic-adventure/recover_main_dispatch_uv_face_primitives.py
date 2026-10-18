"""Recover the complete normal-lit primitive family; lower services stay explicit."""
import hashlib,json,re,csv
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
VERTEX_GENERATOR=PROJECT/'recover_main_dispatch_vertex_workers.py'
prefix=VERTEX_GENERATOR.read_text().split('functions=[];records=[]')[0]
prefix=prefix.replace("'main-dispatch-vertex-workers/'","'main-dispatch-uv-face-family/'")
exec(compile(prefix,str(VERTEX_GENERATOR),'exec'),globals())
marker="        if text.startswith(('pref ','ocbi ')):"
assert code.count(marker)==1
code=code.replace(marker,"        if text=='frchg':return 'if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;'\n"+marker)
exec(compile(code,str(TEMPLATE),'exec'),globals())
functions=[];records=[]
for args in ((0x8C61BE44,0x8C61C47E,'sa_uv_face_primitive_emit_8c61be44',765),
             (0x8C61C4B4,0x8C61CF62,'sa_uv_face_primitive_clipped_8c61c4b4',1277)):
 function,record=recover(*args)
 function=function.replace('void *ctx,sa_shared_service call){','const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){\n int io_status=0;')
 function=function.replace('(void)fpul;(void)macl;','(void)fpul;(void)macl;(void)sx;')
 for name in ('rd','sx','wr','fload','fstore'):function=function.replace(name+'(m,',name+'(m,bus,&io_status,')
 function=re.sub(r'((?:rd|sx|wr|fload|fstore)\(m,[^;]*\);)',r'\1if(io_status)return io_status;',function)
 if args[0]==0x8C61BE44:
  probes={0x8C61BE5A:0xD32D,0x8C61BE62:0x6051,0x8C61BE66:0x600D,0x8C61BE6A:0x2039,0x8C61BE6C:0x8800,0x8C61BE6E:0x8D11,0x8C61BE72:0x9148,0x8C61BE74:0x3010,0x8C61BE76:0x8B01,0x8C61BE7C:0xD125,0x8C61BE7E:0x3010,0x8C61BE80:0x8B01,0x8C61BE86:0xD122,0x8C61BE88:0x3010,0x8C61BE8A:0x8B01}
  literals=((0x8C61BF10,4,0xC000),(0x8C61BF06,2,0x4000),(0x8C61BF14,4,0x8000))
  impossible_rows=(0x8C61BE90,0x8C61BE92);impossible_edge=(0x8C61BE8A,0x8C61BE90)
 else:
  probes={0x8C61C4CA:0xD337,0x8C61C4D0:0x6041,0x8C61C4D4:0x600D,0x8C61C4D6:0x2039,0x8C61C4D8:0x8800,0x8C61C4DA:0x8D11,0x8C61C4DE:0x915F,0x8C61C4E0:0x3010,0x8C61C4E2:0x8B01,0x8C61C4E8:0xD130,0x8C61C4EA:0x3010,0x8C61C4EC:0x8B01,0x8C61C4F2:0xD12D,0x8C61C4F4:0x3010,0x8C61C4F6:0x8B01}
  literals=((0x8C61C5A8,4,0xC000),(0x8C61C5A0,2,0x4000),(0x8C61C5AC,4,0x8000))
  impossible_rows=(0x8C61C4FC,0x8C61C4FE);impossible_edge=(0x8C61C4F6,0x8C61C4FC)
 if args[0]==0x8C61C4B4:
  function=function.replace('L8c61cdc6: ','L8c61cdc6: if((r[14]&0x1f000000u)!=0x0c000000u)return -3;',1)
  record['native_vertex_pointer_guard']=dict(pc='0x8c61cdc6',status=-3,scope='Before a normal read through live R14 outside modeled RAM; the crossing-strip branch requires valid live vertex pointers.')
 ret=0x8C61C47A if args[0]==0x8C61BE44 else 0x8C61CF5E
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
source=PROJECT/'source/sa_main_dispatch_uv_face_primitives.c'
header=header.replace('Vertex workers with portable MMIO/store-queue interface; names inferred.','UV/face primitive family with portable queue interface; names inferred.')
source.write_text(header+'\n'.join(functions))
files=[Path(__file__),VERTEX_GENERATOR,PROJECT/'recover_main_dispatch_mesh_services.py',PROJECT/'recover_main_dispatch_particle_callback.py',BINARY,TRANSLATOR,TEMPLATE,source,PROJECT/'source/vertex_bus.h',PROJECT/'source/vertex_bus_lighting.c']+[Path(r['listing']) for r in records]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=records,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='2042 original rows statically translated; exhaustive input-mask proofs exclude four unreachable rows/two edges. Three lower UV/face clipping/alternate services remain explicit fixtures; behavior proof separate.')
(PROJECT/'analysis/main_dispatch_uv_face_primitives_translation.json').write_text(json.dumps(report,indent=2)+'\n')
print('Generated UV/lit family:',sum(len(r['reachable_addresses']) for r in records),'reachable rows')
