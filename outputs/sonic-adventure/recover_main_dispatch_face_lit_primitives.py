"""Recover the complete face-lit primitive family and numerical normalizer; lower services stay explicit."""
import hashlib,json,re,csv
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
VERTEX_GENERATOR=PROJECT/'recover_main_dispatch_vertex_workers.py'
prefix=VERTEX_GENERATOR.read_text().split('functions=[];records=[]')[0]
prefix=prefix.replace("'main-dispatch-vertex-workers/'","'main-dispatch-next-family/'")
exec(compile(prefix,str(VERTEX_GENERATOR),'exec'),globals())
fneg_marker="        if text.startswith('fmov'):";assert code.count(fneg_marker)==1
code=code.replace(fneg_marker,"        if text.startswith('fneg '):return f'if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][{n}]^=0x80000000u;'\n"+fneg_marker)
marker="        if text.startswith(('pref ','ocbi ')):"
assert code.count(marker)==1
code=code.replace(marker,"        if text=='frchg':return 'if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;'\n"+marker)
exec(compile(code,str(TEMPLATE),'exec'),globals())
functions=[];records=[]
for args in ((0x8C61AE3C,0x8C61B444,'sa_face_lit_primitive_emit_8c61ae3c',742),
             (0x8C61B488,0x8C61BE02,'sa_face_lit_primitive_clipped_8c61b488',1117),
             (0x8C63A94C,0x8C63A96C,'sa_face_vector_normalize_8c63a94c',16)):
 function,record=recover(*args)
 function=function.replace('void *ctx,sa_shared_service call){','const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){\n int io_status=0;')
 function=function.replace('(void)fpul;(void)macl;','(void)fpul;(void)macl;(void)sx;(void)sr;')
 for name in ('rd','sx','wr','fload','fstore'):function=function.replace(name+'(m,',name+'(m,bus,&io_status,')
 function=re.sub(r'((?:rd|sx|wr|fload|fstore)\(m,[^;]*\);)',r'\1if(io_status)return io_status;',function)
 if args[0]==0x8C61B488:
  assert function.count('L8c61bca2: ')==1
  function=function.replace('L8c61bca2: ','L8c61bca2: if((r[14]&0x1f000000u)!=0x0c000000u)return -3;',1)
  record['native_vertex_pointer_guard']=dict(pc='0x8c61bca2',status=-3,scope='Before a normal-vector load through overwritten R14 outside modeled RAM; host unsupported transfer, not hardware fault dispatch.')
 if args[0]!=0x8C63A94C:
  ret=0x8C61B440 if args[0]==0x8C61AE3C else 0x8C61BDFE
  function=function.replace(' (void)fpul;',' uint32_t entry_pr=*pr;\n (void)fpul;',1)
  function=function.replace(f'L{ret:08x}: ',f'L{ret:08x}: if(*pr!=entry_pr)return -3;',1)
  record['native_return_guard']=dict(pc=hex(ret),status=-3,scope='Stop before RTS delay-slot effects if stack writes changed the saved incoming return address; no hardware transfer/fault model.')
 if args[0]!=0x8C63A94C:
  if args[0]==0x8C61AE3C:
   probes={0x8C61AE4C:0xD315,0x8C61AE58:0x6001,0x8C61AE5A:0x600D,0x8C61AE5C:0x2039,0x8C61AE5E:0x8800,0x8C61AE60:0x8D11,0x8C61AE64:0x911A,0x8C61AE66:0x3010,0x8C61AE68:0x8B01,0x8C61AE6E:0xD10E,0x8C61AE70:0x3010,0x8C61AE72:0x8B01,0x8C61AE78:0xD10A,0x8C61AE7A:0x3010,0x8C61AE7C:0x8B01}
   literals=((0x8C61AEA4,4,0xC000),(0x8C61AE9C,2,0x4000),(0x8C61AEA8,4,0x8000))
   impossible_rows=(0x8C61AE82,0x8C61AE84);impossible_edge=(0x8C61AE7C,0x8C61AE82)
  else:
   probes={0x8C61B49A:0xD314,0x8C61B4A0:0x6001,0x8C61B4A2:0x600D,0x8C61B4A4:0x2039,0x8C61B4A6:0x8800,0x8C61B4A8:0x8D11,0x8C61B4AC:0x911D,0x8C61B4AE:0x3010,0x8C61B4B0:0x8B01,0x8C61B4B6:0xD10E,0x8C61B4B8:0x3010,0x8C61B4BA:0x8B01,0x8C61B4C0:0xD10A,0x8C61B4C2:0x3010,0x8C61B4C4:0x8B01}
   literals=((0x8C61B4EC,4,0xC000),(0x8C61B4EA,2,0x4000),(0x8C61B4F0,4,0x8000))
   impossible_rows=(0x8C61B4CA,0x8C61B4CC);impossible_edge=(0x8C61B4C4,0x8C61B4CA)
  for address,word in probes.items():assert int.from_bytes(binary[address-0x8C010000:address-0x8C010000+2],'little')==word,hex(address)
  for address,width,value in literals:assert int.from_bytes(binary[address-0x8C010000:address-0x8C010000+width],'little')==value
  outcomes={word&0xC000 for word in range(65536)};assert outcomes=={0,0x4000,0x8000,0xC000}
  record['syntactic_reachable_addresses']=record['reachable_addresses']
  record['reachable_addresses']=[a for a in record['reachable_addresses'] if int(a,16) not in impossible_rows]
  record['syntactic_conditional_edges']=record['conditional_edges']
  record['conditional_edges']=[e for e in record['conditional_edges'] if tuple(int(a,16) for a in e)!=impossible_edge]
  record['unreachable_proof']=dict(reason='Exact original switch instructions zero-extend a word and mask c000. Exhausting all 65536 words yields only the four handled outcomes; final default BRA/NOP and BF taken edge are impossible.',input_values_exhausted=65536,mask_values=sorted(outcomes),checked_instructions={hex(a):hex(w) for a,w in probes.items()},rows=[hex(a) for a in impossible_rows],edge=[hex(a) for a in impossible_edge])
 if args[0]==0x8C63A94C:
  origin=list(csv.DictReader((PROJECT/'analysis/instructions/model_vector_normalize_dependencies.tsv').open(),delimiter='\t'))[:16]
  assert [int(r['address'],16) for r in origin]==list(range(0x8C63A94C,0x8C63A96C,2))
  assert all(int(r['opcode'],16)==int.from_bytes(binary[int(r['address'],16)-0x8C010000:int(r['address'],16)-0x8C010000+2],'little') for r in origin)
  record['listing_origin']='Format conversion of archived analysis/instructions/model_vector_normalize_dependencies.tsv; all 16 original words rechecked.'
 functions.append(function);records.append(record)
source=PROJECT/'source/sa_main_dispatch_face_lit_primitives.c'
header=header.replace('Vertex workers with portable MMIO/store-queue interface; names inferred.','Face-lit primitives and numerical normalizer with portable queues; names inferred.')
source.write_text(header+'\n'.join(functions))
files=[Path(__file__),VERTEX_GENERATOR,PROJECT/'recover_main_dispatch_mesh_services.py',PROJECT/'recover_main_dispatch_particle_callback.py',BINARY,TRANSLATOR,TEMPLATE,source,PROJECT/'analysis/instructions/model_vector_normalize_dependencies.tsv',PROJECT/'source/vertex_bus.h',PROJECT/'source/vertex_bus_lighting.c']+[Path(r['listing']) for r in records]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=records,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='1875 original rows statically translated, four default rows/two edges excluded by exhaustive mask proofs. Three lower face-lit near/alternate services remain explicit fixtures; behavior proof separate.')
(PROJECT/'analysis/main_dispatch_face_lit_primitives_translation.json').write_text(json.dumps(report,indent=2)+'\n')
print('Generated face-lit family:',sum(len(r['reachable_addresses']) for r in records),'reachable rows')
