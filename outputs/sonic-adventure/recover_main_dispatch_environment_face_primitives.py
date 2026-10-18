"""Recover the complete environment-UV/face-factor primitive family; lower services stay explicit."""
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
fneg_marker="        if text.startswith('fmov'):"
code=code.replace(fneg_marker,"        if text.startswith('fneg '):return f'if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][{n}]^=0x80000000u;'\n"+fneg_marker)
exec(compile(code,str(TEMPLATE),'exec'),globals())
functions=[];records=[]
for args in ((0x8C61DB98,0x8C61E4BC,'sa_environment_face_primitive_emit_8c61db98',1116),
             (0x8C61E4F4,0x8C61F0D2,'sa_environment_face_primitive_clipped_8c61e4f4',1429)):
 function,record=recover(*args)
 function=function.replace('void *ctx,sa_shared_service call){','const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){\n int io_status=0;')
 function=function.replace('(void)fpul;(void)macl;','(void)fpul;(void)macl;(void)sx;')
 for name in ('rd','sx','wr','fload','fstore'):function=function.replace(name+'(m,',name+'(m,bus,&io_status,')
 function=re.sub(r'((?:rd|sx|wr|fload|fstore)\(m,[^;]*\);)',r'\1if(io_status)return io_status;',function)
 if args[0]==0x8C61DB98:
  probes={0x8C61DBAC:0xD319,0x8C61DBBA:0x6001,0x8C61DBBC:0x600D,0x8C61DBBE:0x2039,0x8C61DBC0:0x8800,0x8C61DBC2:0x8D11,0x8C61DBC6:0x911B,0x8C61DBC8:0x3010,0x8C61DBCA:0x8B01,0x8C61DBD0:0xD111,0x8C61DBD2:0x3010,0x8C61DBD4:0x8B01,0x8C61DBDA:0xD10E,0x8C61DBDC:0x3010,0x8C61DBDE:0x8B01}
  literals=((0x8C61DC14,4,0xC000),(0x8C61DC00,2,0x4000),(0x8C61DC18,4,0x8000))
  impossible_rows=(0x8C61DBE4,0x8C61DBE6);impossible_edge=(0x8C61DBDE,0x8C61DBE4);ret=0x8C61E4B8
 else:
  probes={0x8C61E50A:0xD316,0x8C61E514:0x6001,0x8C61E516:0x600D,0x8C61E518:0x2039,0x8C61E51A:0x8800,0x8C61E51C:0x8D11,0x8C61E520:0x911D,0x8C61E522:0x3010,0x8C61E524:0x8B01,0x8C61E52A:0xD10F,0x8C61E52C:0x3010,0x8C61E52E:0x8B01,0x8C61E534:0xD10B,0x8C61E536:0x3010,0x8C61E538:0x8B01}
  literals=((0x8C61E564,4,0xC000),(0x8C61E55E,2,0x4000),(0x8C61E568,4,0x8000))
  impossible_rows=(0x8C61E53E,0x8C61E540);impossible_edge=(0x8C61E538,0x8C61E53E);ret=0x8C61F0CE
  function=function.replace('L8c61ef72: ','L8c61ef72: if((r[14]&0x1f000000u)!=0x0c000000u)return -3;',1)
  record['native_vertex_pointer_guard']=dict(pc='0x8c61ef72',status=-3,scope='Before normal read through live R14 outside modeled RAM; the crossing-strip branch needs valid live vertex pointers.')
 function=function.replace(' (void)fpul;',' uint32_t entry_pr=*pr;\n (void)fpul;',1)
 function=function.replace(f'L{ret:08x}: ',f'L{ret:08x}: if(*pr!=entry_pr)return -3;',1)
 record['native_return_guard']=dict(pc=hex(ret),status=-3,scope='Stop before RTS delay-slot effects if a saved return address differs from the incoming address; host unsupported-transfer boundary.')
 for address,word in probes.items():assert int.from_bytes(binary[address-0x8C010000:address-0x8C010000+2],'little')==word,hex(address)
 for address,width,value in literals:assert int.from_bytes(binary[address-0x8C010000:address-0x8C010000+width],'little')==value
 outcomes={word&0xC000 for word in range(65536)};assert outcomes=={0,0x4000,0x8000,0xC000}
 record['syntactic_reachable_addresses']=record['reachable_addresses']
 record['reachable_addresses']=[a for a in record['reachable_addresses'] if int(a,16) not in impossible_rows]
 record['syntactic_conditional_edges']=record['conditional_edges']
 record['conditional_edges']=[e for e in record['conditional_edges'] if tuple(int(a,16) for a in e)!=impossible_edge]
 record['unreachable_proof']=dict(reason='Byte-checked switch zero-extends its input word and masks c000. All 65536 inputs yield exactly 0/4000/8000/c000; previous comparisons handle the first three. Final BF taken edge and default BRA/NOP cannot execute.',input_values_exhausted=65536,mask_values=sorted(outcomes),checked_instructions={hex(a):hex(w) for a,w in probes.items()},rows=[hex(a) for a in impossible_rows],edge=[hex(a) for a in impossible_edge])
 functions.append(function);records.append(record)
source=PROJECT/'source/sa_main_dispatch_environment_face_primitives.c'
header=header.replace('Vertex workers with portable MMIO/store-queue interface; names inferred.','Environment UV/face-factor primitive family with portable queue interface; names inferred.')
source.write_text(header+'\n'.join(functions))
files=[Path(__file__),VERTEX_GENERATOR,PROJECT/'recover_main_dispatch_mesh_services.py',PROJECT/'recover_main_dispatch_particle_callback.py',BINARY,TRANSLATOR,TEMPLATE,source,PROJECT/'source/vertex_bus.h',PROJECT/'source/vertex_bus_lighting.c']+[Path(r['listing']) for r in records]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=records,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='2545 original rows statically translated; exhaustive input-mask proofs exclude four unreachable rows/two edges. Three lower environment/face clipping/alternate services remain explicit fixtures; behavior proof separate.')
(PROJECT/'analysis/main_dispatch_environment_face_primitives_translation.json').write_text(json.dumps(report,indent=2)+'\n')
print('Generated environment/face family:',sum(len(r['reachable_addresses']) for r in records),'reachable rows')
