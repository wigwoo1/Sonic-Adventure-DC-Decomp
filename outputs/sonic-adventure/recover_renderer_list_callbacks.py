"""Original TA completion and registered display callbacks; no generation credit."""
import csv,hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;EVENT_PARENT=PROJECT/'recover_boot_event.py'
exec(compile(EVENT_PARENT.read_text().split('notes=[];diagnostics=[]')[0],str(EVENT_PARENT),'exec'),globals())
registry=json.loads((PROJECT/'analysis/recovered_routines.json').read_text())['routines']
functions=[];routines=[];listings=[]
for folder,start in [('boot-submit-callbacks',0x8c651f1e)]+[('renderer-list-callbacks',a) for a in (0x8c604546,0x8c604568,0x8c604570)]:
 listing=PROJECT/'analysis'/folder/f'{start:08x}_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'));listings.append(listing)
 chosen=BASE_CHOSEN.replace('main-dispatch-model-traversal/',folder+'/');exec(compile(chosen,str(TEMPLATE),'exec'),globals())
 function,record=recover(start,max(int(r['address'],16)+2 for r in rows),f'sa_renderer_list_callbacks_{start:08x}',len(rows))
 marker='sa_shared_service call,uint32_t *stop){';assert function.count(marker)==1
 function=function.replace(marker,marker[:-2]+',uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){\n int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;')
 function=function.replace('(void)entry_pr;','(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;')
 for name in ('rd','sx','wr','fload','fstore'):function=function.replace(name+'(m,',name+'(m,bus,&io_status,')
 function=re.sub(r'((?:rd|sx|wr|fload|fstore)\(m,[^;]*\);)',r'\1if(io_status)return io_status;',function)
 function=re.sub(r'(r\[\d+\])=rd\(([^;]+)\);if\(io_status\)return io_status;',r'{uint32_t loaded=rd(\2);if(io_status)return io_status;\1=loaded;}',function)
 record.update(registration_requires_complete_behavior=True,canonical_registered_reuse=any(r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==hex(start) for r in registry))
 functions.append(function);routines.append(record)
header=header.replace(header[header.index('/*'):header.index(' */')+3],'/* Original TA completion and actual registered display callbacks. */')
# Reuse the exact existing checked VRAM domain from the interrupt translation.
vram_line=next(line for line in (PROJECT/'source/sa_boot_interrupt.c').read_text().splitlines() if '(a&0x1fffffffu)>=0x05000000u' in line)
marker=' if(valid_ram(a,n))return 1;';assert header.count(marker)==1;header=header.replace(marker,marker+'\n'+vram_line)
source=PROJECT/'source/sa_renderer_list_callbacks.c';assert not source.exists();source.write_text(header+'\n'.join(functions))
files=[Path(__file__),EVENT_PARENT,SUBMIT_PARENT,WAIT_PARENT,PARENT,GBR_PARENT,SAMPLING,PREVIOUS,PARENT_TEMPLATE,TRANSLATOR,TEMPLATE,BINARY,source,PROJECT/'source/sa_boot_interrupt.c',PROJECT/'source/vertex_bus.h',PROJECT/'source/sh4_integer_state.h',*listings]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=routines,instruction_rows=sum(r['instruction_rows'] for r in routines),newly_registered_entries=0,canonical_reused_entries=['0x8c604546'],proposed_new_original_entries=['0x8c651f1e','0x8c604568','0x8c604570'],scope='Exact original bodies andchecked existing VRAM mapping. Oldproposal/source/build bytes retained. Canonical604546 getsno duplicatecredit;all other recovery credit awaitscompletebehavior andintegration qualification.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
target=PROJECT/'analysis/renderer_list_callbacks_translation.json';assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n');print('Translated fourcallback bodies/',report['instruction_rows'],'rows;no new credit yet',flush=True)
