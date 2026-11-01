"""Recover the original input tick callback and its actual submission tail."""
import csv,hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;ROOT=PROJECT.parents[1]
EVENT_PARENT=PROJECT/'recover_boot_event.py'
exec(compile(EVENT_PARENT.read_text().split('notes=[];diagnostics=[]')[0],str(EVENT_PARENT),'exec'),globals())
functions=[];routines=[];evidence=[]
for address,folder in ((0x8c646024,'renderer-input-irq-mask'),(0x8c6463ac,'renderer-input-irq-tail')):
 listing=PROJECT/'analysis'/folder/f'{address:08x}_instructions.tsv'
 rows=list(csv.DictReader(listing.open(),delimiter='\t'))
 for row in rows:
  pc=int(row['address'],16);raw=bytes.fromhex(row['bytes']);assert binary[pc-0x8c010000:pc-0x8c010000+len(raw)]==raw
 chosen=BASE_CHOSEN.replace('main-dispatch-model-traversal/',folder+'/');exec(compile(chosen,str(TEMPLATE),'exec'),globals())
 function,record=recover(address,max(int(r['address'],16)+2 for r in rows),f'sa_renderer_input_tick_{address:08x}',len(rows))
 marker='sa_shared_service call,uint32_t *stop){';assert function.count(marker)==1
 function=function.replace(marker,marker[:-2]+',uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){\n int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;')
 function=function.replace('(void)entry_pr;','(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)r;')
 for name in ('rd','sx','wr','fload','fstore'):function=function.replace(name+'(m,',name+'(m,bus,&io_status,')
 function=re.sub(r'((?:rd|sx|wr|fload|fstore)\(m,[^;]*\);)',r'\1if(io_status)return io_status;',function)
 function=re.sub(r'(r\[\d+\])=rd\(([^;]+)\);if\(io_status\)return io_status;',r'{uint32_t loaded=rd(\2);if(io_status)return io_status;\1=loaded;}',function)
 function=re.sub(r'(r\[\d+\])=sx\(([^;]+)\);if\(io_status\)return io_status;',r'{uint32_t loaded=sx(\2);if(io_status)return io_status;\1=loaded;}',function)
 functions.append(function);record['registration_requires_complete_behavior']=True;routines.append(record)
 evidence += [listing,PROJECT/'analysis'/folder/f'{address:08x}_draft.c',PROJECT/'analysis'/folder/'diagnostics.tsv']
source=PROJECT/'source/sa_renderer_input_tick.c';assert not source.exists();source.write_text(header+'\n'.join(functions))
files=[Path(__file__),EVENT_PARENT,SUBMIT_PARENT,WAIT_PARENT,PARENT,GBR_PARENT,SAMPLING,PREVIOUS,PARENT_TEMPLATE,TRANSLATOR,TEMPLATE,BINARY,source,*evidence]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=routines,instruction_rows=sum(r['instruction_rows'] for r in routines),newly_registered_entries=0,new_original_routine_credit=0,scope='Complete original tick callback and input submission tail proposals. Actual existing input pump/submission and any declared real callback execute through dispatch. No scheduler result or device completion is supplied. Generation alone does not qualify or register either routine.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
target=PROJECT/'analysis/renderer_input_tick_translation.json';assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n');print('Generated',len(routines),'original proposals /',report['instruction_rows'],'byte-checked rows',flush=True)
