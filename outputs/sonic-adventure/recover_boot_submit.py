"""Original submission/draining/record-builder bodies with explicit CPU/bus state."""
import csv,hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;WAIT_PARENT=PROJECT/'recover_draw_view_registers.py'
exec(compile(WAIT_PARENT.read_text().split("folder='draw-view-registers'")[0],str(WAIT_PARENT),'exec'),globals())
header=header.replace('#include "vertex_bus.h"','#include "vertex_bus.h"\n#include "sh4_integer_state.h"')
marker=' if(valid_ram(a,n))return 1;';assert header.count(marker)==1
header=header.replace(marker,marker+'\n if(n==4u && (a==0xffd8000cu || a==0xffa00020u || a==0xffa00024u || a==0xffa00028u || a==0xffa0002cu || a==0xa05f6800u || a==0xa05f6804u || a==0xa05f6808u))return 1;')
marker="        assert not text.startswith('f'),text";assert BASE_CHOSEN.count(marker)==1
BASE_CHOSEN=BASE_CHOSEN.replace(marker,"""        prior=rows.get(a-2,(0,''))[0]
        delay=int(prior in (0x000b,0x002b) or prior>>12 in (0xa,0xb) or prior>>8 in (0x8d,0x8f) or prior&0xf0ff in (0x400b,0x402b,0x0003,0x0023))
        if w&0xf0ff==0x0002:return f'status=sa_sh4_stc_sr(*sr,&r[{n}],{delay}u);if(status)return status;'
        if w&0xf0ff==0x400e:return f'status=sa_sh4_ldc_sr(r[{n}],sr,r,integer,{delay}u);if(status)return status;'
"""+marker)
folder='boot-submit';directory=PROJECT/'analysis'/folder;diagnostics=directory/'diagnostics.tsv'
notes=list(csv.DictReader(diagnostics.open(),delimiter='\t'));assert len(notes)==4 and all(n['completed']=='true' and not n['error'] for n in notes)
functions=[];routines=[]
guards={0x8c641f00:{0x8c642040:True,0x8c64207e:False,0x8c642106:False},0x8c641e00:{0x8c641e78:True,0x8c641e8e:True}}
for note in notes:
 start=int(note['address'],16);listing=directory/f'{start:08x}_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'));end=max(int(r['address'],16)+2 for r in rows)
 chosen=BASE_CHOSEN.replace('main-dispatch-model-traversal/',folder+'/');exec(compile(chosen,str(TEMPLATE),'exec'),globals())
 function,record=recover(start,end,f'sa_boot_submit_{start:08x}',len(rows))
 marker='sa_shared_service call,uint32_t *stop){';assert function.count(marker)==1
 function=function.replace(marker,marker[:-2]+',uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){\n int io_status=0;(void)io_status;(void)bus;(void)integer;')
 function=function.replace('(void)entry_pr;','(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;')
 for name in ('rd','sx','wr','fload','fstore'):function=function.replace(name+'(m,',name+'(m,bus,&io_status,')
 function=re.sub(r'((?:rd|sx|wr|fload|fstore)\(m,[^;]*\);)',r'\1if(io_status)return io_status;',function)
 function=re.sub(r'(r\[\d+\])=rd\(([^;]+)\);if\(io_status\)return io_status;',r'{uint32_t loaded=rd(\2);if(io_status)return io_status;\1=loaded;}',function)
 own=guards.get(start,{})
 if own:
  function=function.replace(' int status;uint32_t target;',f' unsigned guard_visits[{len(own)}]={{0}};(void)guard_visits;\n int status;uint32_t target;',1)
  for i,(pc,taken_true) in enumerate(own.items()):
   label=f'L{pc:x}: ';assert function.count(label)==1
   condition='(*sr&1u)' if taken_true else '!(*sr&1u)'
   function=function.replace(label,label+f'\n#ifdef SA_BOUND_RETAIL_STALLS\n *stop=0x{pc:x}u;if({condition} && ++guard_visits[{i}]>=2u)return -4;\n#endif\n ',1)
  record['bounded_research_waits']=[dict(pc=hex(pc),continue_when_t=active,max_continue_tests=2,status=-4) for pc,active in own.items()]
 functions.append(function);record['registration_requires_complete_behavior']=True;routines.append(record)
source=PROJECT/'source/sa_boot_submit.c';source.write_text(header+'\n'.join(functions))
files=[Path(__file__),WAIT_PARENT,PARENT,GBR_PARENT,SAMPLING,PREVIOUS,PARENT_TEMPLATE,PROJECT/'recover_draw_polygon_lower.py',PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',TRANSLATOR,TEMPLATE,BINARY,source,PROJECT/'source/vertex_bus.h',PROJECT/'source/sh4_integer_state.h',PROJECT/'source/sh4_integer_state.c',diagnostics,PROJECT/'ghidra/export_boot_submit.ps1',PROJECT/'ghidra/SeedCallbackFunctions.java',PROJECT/'ghidra/ExportFailedFunctionEvidence.java']+[Path(r['listing']) for r in routines]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=routines,instruction_rows=sum(r['instruction_rows'] for r in routines),newly_registered_entries=0,reused_routine_credit=0,scope='Four original submission/draining/transfer-record/register-read proposals, with original SR mask operations and explicit inactive integer-bank state. Research-only retry/wait limits return -4 without producing a game result. Behavior acceptance and registration remain separate.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(PROJECT/'analysis/boot_submit_translation.json').write_text(json.dumps(report,indent=2)+'\n')
print('Generated',len(routines),'submission proposals /',report['instruction_rows'],'instruction rows')
