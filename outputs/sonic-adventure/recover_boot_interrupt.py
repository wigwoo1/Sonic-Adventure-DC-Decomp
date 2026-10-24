"""Original interrupt, selection and buffer-building proposals with explicit VRAM."""
import csv,hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;SUBMIT_PARENT=PROJECT/'recover_boot_submit.py'
exec(compile(SUBMIT_PARENT.read_text().split("folder='boot-submit'")[0],str(SUBMIT_PARENT),'exec'),globals())
remainder=PROJECT/'source/sa_unsigned_remainder.c';remainder_proof=PROJECT/'analysis/unsigned_remainder_verification.json'
assert hashlib.sha256(remainder.read_bytes()).hexdigest()==json.loads(remainder_proof.read_text())['source_sha256']
div_source=remainder.read_text();div_source=div_source[div_source.index('static void div1_step'):div_source.index('SA_STAGE_EXPORT int')]
header+='\n'+div_source.replace('div1_step','div_step')
marker=' if(valid_ram(a,n))return 1;';assert header.count(marker)==1
header=header.replace(marker,marker+'\n if((a&0x1fffffffu)==0x005f6900u && n==4u)return 1;\n if((n==4u||n==8u) && !(a&3u) && (a&0x1fffffffu)>=0x05000000u && (a&0x1fffffffu)<=0x05800000u-n)return 1;')
notes=[];diagnostics=[]
for folder in ('boot-interrupt','boot-interrupt-lower','boot-device','boot-submit'):
 path=PROJECT/'analysis'/folder/'diagnostics.tsv';diagnostics.append(path)
 for note in csv.DictReader(path.open(),delimiter='\t'):
  address=int(note['address'],16)
  if folder=='boot-device' and address!=0x8c65253e:continue
  if folder=='boot-submit' and address!=0x8c641f00:continue
  assert note['completed']=='true' and not note['error'];notes.append(dict(note,folder=folder))
assert len(notes)==13
functions=[];routines=[]
for note in notes:
 start=int(note['address'],16);listing=PROJECT/'analysis'/note['folder']/f'{start:08x}_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'));end=max(int(r['address'],16)+2 for r in rows)
 chosen=BASE_CHOSEN.replace('main-dispatch-model-traversal/',note['folder']+'/');exec(compile(chosen,str(TEMPLATE),'exec'),globals())
 function,record=recover(start,end,f'sa_boot_interrupt_{start:08x}',len(rows))
 marker='sa_shared_service call,uint32_t *stop){';assert function.count(marker)==1
 function=function.replace(marker,marker[:-2]+',uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){\n int io_status=0;(void)io_status;(void)bus;(void)integer;')
 function=function.replace('(void)entry_pr;','(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;')
 for name in ('rd','sx','wr','fload','fstore'):function=function.replace(name+'(m,',name+'(m,bus,&io_status,')
 function=re.sub(r'((?:rd|sx|wr|fload|fstore)\(m,[^;]*\);)',r'\1if(io_status)return io_status;',function)
 function=re.sub(r'(r\[\d+\])=rd\(([^;]+)\);if\(io_status\)return io_status;',r'{uint32_t loaded=rd(\2);if(io_status)return io_status;\1=loaded;}',function)
 guards={0x8c650b60:{0x8c650b76:(False,2)},0x8c641f00:{0x8c642040:(True,2),0x8c64207e:(False,64),0x8c642106:(False,64)}}.get(start,{})
 if guards:
  function=function.replace(' int status;uint32_t target;',f' unsigned wait_visits[{len(guards)}]={{0}};(void)wait_visits;\n int status;uint32_t target;',1)
  for i,(pc,(active,limit)) in enumerate(guards.items()):
   marker=f'L{pc:x}: ';assert function.count(marker)==1;condition='(*sr&1u)' if active else '!(*sr&1u)'
   function=function.replace(marker,f'L{pc:x}:\n#ifdef SA_BOUND_RETAIL_STALLS\n *stop=0x{pc:x}u;if({condition} && ++wait_visits[{i}]>={limit}u)return -4;\n#endif\n ',1)
  record['bounded_research_waits']=[dict(pc=hex(pc),continue_when_t=active,max_continue_tests=limit,status=-4) for pc,(active,limit) in guards.items()]
 record['registration_requires_complete_behavior']=True;functions.append(function);routines.append(record)
division=PROJECT/'source/unsigned_division.c';division_proof=PROJECT/'analysis/unsigned_division_verification.json'
assert hashlib.sha256(division.read_bytes()).hexdigest()==json.loads(division_proof.read_text())['source_sha256']
source=PROJECT/'source/sa_boot_interrupt.c';source.write_text(header+'\n'.join(functions))
files=[Path(__file__),SUBMIT_PARENT,WAIT_PARENT,PARENT,GBR_PARENT,SAMPLING,PREVIOUS,PARENT_TEMPLATE,PROJECT/'recover_draw_polygon_lower.py',PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',TRANSLATOR,TEMPLATE,BINARY,source,PROJECT/'source/vertex_bus.h',PROJECT/'source/sh4_integer_state.h',PROJECT/'source/sh4_integer_state.c',division,division_proof,remainder,remainder_proof,PROJECT/'ghidra/export_boot_interrupt.ps1',PROJECT/'ghidra/export_boot_interrupt_lower.ps1',PROJECT/'ghidra/SeedCallbackFunctions.java',PROJECT/'ghidra/ExportFailedFunctionEvidence.java']+diagnostics+[Path(r['listing']) for r in routines]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=routines,instruction_rows=sum(r['instruction_rows'] for r in routines),newly_registered_entries=0,reused_routine_credit=0,reused_math=['0x8c10cb10','0x8c10cc80'],scope='Thirteen byte-checked interrupt/reset/selection/submission/buffer/VRAM-transfer and existing arithmetic adapters. DIV1 logic is copied exactly from the prior recovered remainder helper; math receives no duplicate credit. IRQ delivery, physical DMA/GPU behavior and behavior qualification remain separate.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(PROJECT/'analysis/boot_interrupt_translation.json').write_text(json.dumps(report,indent=2)+'\n')
print('Generated',len(routines),'interrupt proposals /',report['instruction_rows'],'original rows; existing math reused')
