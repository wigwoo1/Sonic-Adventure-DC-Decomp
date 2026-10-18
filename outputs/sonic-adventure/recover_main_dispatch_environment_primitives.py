"""Recover the complete normal-derived environment UV primitive family; lower services stay explicit."""
import hashlib,json,re,csv
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
VERTEX_GENERATOR=PROJECT/'recover_main_dispatch_vertex_workers.py'
prefix=VERTEX_GENERATOR.read_text().split('functions=[];records=[]')[0]
prefix=prefix.replace("'main-dispatch-vertex-workers/'","'main-dispatch-uv-family2/'")
exec(compile(prefix,str(VERTEX_GENERATOR),'exec'),globals())
fneg_marker="        if text.startswith('fmov'):";assert code.count(fneg_marker)==1
code=code.replace(fneg_marker,"        if text.startswith('fneg '):return f'if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][{n}]^=0x80000000u;'\n"+fneg_marker)
marker="        if text.startswith(('pref ','ocbi ')):"
assert code.count(marker)==1
code=code.replace(marker,"        if text=='frchg':return 'if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;'\n"+marker)
exec(compile(code,str(TEMPLATE),'exec'),globals())
functions=[];records=[]
for args in ((0x8C619C34,0x8C61A456,'sa_environment_primitive_emit_8c619c34',1001),
             (0x8C61A488,0x8C61ADFC,'sa_environment_primitive_clipped_8c61a488',1152)):
 function,record=recover(*args)
 function=function.replace('void *ctx,sa_shared_service call){','const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){\n int io_status=0;')
 function=function.replace('(void)fpul;(void)macl;','(void)fpul;(void)macl;(void)sx;')
 for name in ('rd','sx','wr','fload','fstore'):function=function.replace(name+'(m,',name+'(m,bus,&io_status,')
 function=re.sub(r'((?:rd|sx|wr|fload|fstore)\(m,[^;]*\);)',r'\1if(io_status)return io_status;',function)
 if args[0]==0x8C619C34:
  start,stop=0x8C619C40,0x8C619C7C
  probes={0x8C619C40:0xD317,0x8C619C48:0x6041,0x8C619C4C:0x600D,0x8C619C50:0x2039,0x8C619C54:0x8800,0x8C619C56:0x8D11,0x8C619C5A:0x9119,0x8C619C5C:0x3010,0x8C619C5E:0x8B01,0x8C619C64:0xD10F,0x8C619C66:0x3010,0x8C619C68:0x8B01,0x8C619C6E:0xD10C,0x8C619C70:0x3010,0x8C619C72:0x8B01}
  literals=((0x8C619CA0,4,0xC000),(0x8C619C90,2,0x4000),(0x8C619CA4,4,0x8000))
  impossible_rows=(0x8C619C78,0x8C619C7A);impossible_edge=(0x8C619C72,0x8C619C78)
 else:
  start,stop=0x8C61A49C,0x8C61A4D4
  probes={0x8C61A49C:0xD315,0x8C61A4A6:0x6001,0x8C61A4A8:0x600D,0x8C61A4AA:0x2039,0x8C61A4AC:0x8800,0x8C61A4AE:0x8D11,0x8C61A4B2:0x911B,0x8C61A4B4:0x3010,0x8C61A4B6:0x8B01,0x8C61A4BC:0xD10E,0x8C61A4BE:0x3010,0x8C61A4C0:0x8B01,0x8C61A4C6:0xD10B,0x8C61A4C8:0x3010,0x8C61A4CA:0x8B01}
  literals=((0x8C61A4F4,4,0xC000),(0x8C61A4EC,2,0x4000),(0x8C61A4F8,4,0x8000))
  impossible_rows=(0x8C61A4D0,0x8C61A4D2);impossible_edge=(0x8C61A4CA,0x8C61A4D0)
 for address,word in probes.items():assert int.from_bytes(binary[address-0x8C010000:address-0x8C010000+2],'little')==word,hex(address)
 for address,width,value in literals:assert int.from_bytes(binary[address-0x8C010000:address-0x8C010000+width],'little')==value
 outcomes={word&0xC000 for word in range(65536)};assert outcomes=={0,0x4000,0x8000,0xC000}
 record['syntactic_reachable_addresses']=record['reachable_addresses']
 record['reachable_addresses']=[a for a in record['reachable_addresses'] if int(a,16) not in impossible_rows]
 record['syntactic_conditional_edges']=record['conditional_edges']
 record['conditional_edges']=[e for e in record['conditional_edges'] if tuple(int(a,16) for a in e)!=impossible_edge]
 record['unreachable_proof']=dict(reason='Byte-checked switch zero-extends a 16-bit word and masks c000. Exhausting all 65536 values yields only 0/4000/8000/c000, handled by the preceding comparisons; final default BRA/NOP and BF taken edge cannot execute.',input_values_exhausted=65536,mask_values=sorted(outcomes),checked_instructions={hex(a):hex(w) for a,w in probes.items()},rows=[hex(a) for a in impossible_rows],edge=[hex(a) for a in impossible_edge])
 functions.append(function);records.append(record)
source=PROJECT/'source/sa_main_dispatch_environment_primitives.c'
header=header.replace('Vertex workers with portable MMIO/store-queue interface; names inferred.','Normal-derived environment UV primitives with portable queues; names inferred.')
source.write_text(header+'\n'.join(functions))
files=[Path(__file__),VERTEX_GENERATOR,PROJECT/'recover_main_dispatch_mesh_services.py',PROJECT/'recover_main_dispatch_particle_callback.py',BINARY,TRANSLATOR,TEMPLATE,source,PROJECT/'source/vertex_bus.h',PROJECT/'source/vertex_bus_lighting.c']+[Path(r['listing']) for r in records]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=records,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='2153 original rows statically translated, four default rows/two edges excluded by exhaustive mask proofs. Three lower environment/near/alternate services remain explicit fixtures; behavior proof separate.')
(PROJECT/'analysis/main_dispatch_environment_primitives_translation.json').write_text(json.dumps(report,indent=2)+'\n')
print('Generated environment family:',sum(len(r['reachable_addresses']) for r in records),'reachable rows')
