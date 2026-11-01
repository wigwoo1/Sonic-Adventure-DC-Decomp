"""Translate original651f1e; generation alone gives no recovery credit."""
import csv,hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
EVENT_PARENT=PROJECT/'recover_boot_event.py'
exec(compile(EVENT_PARENT.read_text().split('notes=[];diagnostics=[]')[0],str(EVENT_PARENT),'exec'),globals())
listing=PROJECT/'analysis/boot-submit-callbacks/8c651f1e_instructions.tsv'
rows=list(csv.DictReader(listing.open(),delimiter='\t'))
start=0x8c651f1e;end=max(int(r['address'],16)+2 for r in rows)
chosen=BASE_CHOSEN.replace('main-dispatch-model-traversal/','boot-submit-callbacks/')
exec(compile(chosen,str(TEMPLATE),'exec'),globals())
function,record=recover(start,end,'sa_renderer_list_completion_8c651f1e',len(rows))
marker='sa_shared_service call,uint32_t *stop){';assert function.count(marker)==1
function=function.replace(marker,marker[:-2]+',uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){\n int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;')
function=function.replace('(void)entry_pr;','(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;')
for name in ('rd','sx','wr','fload','fstore'):function=function.replace(name+'(m,',name+'(m,bus,&io_status,')
function=re.sub(r'((?:rd|sx|wr|fload|fstore)\(m,[^;]*\);)',r'\1if(io_status)return io_status;',function)
function=re.sub(r'(r\[\d+\])=rd\(([^;]+)\);if\(io_status\)return io_status;',r'{uint32_t loaded=rd(\2);if(io_status)return io_status;\1=loaded;}',function)
header=header.replace(header[header.index('/*'):header.index(' */')+3],'/* Original TA list-completion callback651f1e. Exact instruction order. */')
source=PROJECT/'source/sa_renderer_list_completion.c';assert not source.exists();source.write_text(header+function)
record.update(registration_requires_complete_behavior=True,canonical_registered_reuse=False)
registry=json.loads((PROJECT/'analysis/recovered_routines.json').read_text())['routines']
assert not any(r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==hex(start) for r in registry)
files=[Path(__file__),EVENT_PARENT,SUBMIT_PARENT,WAIT_PARENT,PARENT,GBR_PARENT,SAMPLING,PREVIOUS,PARENT_TEMPLATE,TRANSLATOR,TEMPLATE,BINARY,source,listing,PROJECT/'source/vertex_bus.h',PROJECT/'source/sh4_integer_state.h']
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=[record],instruction_rows=len(rows),newly_registered_entries=0,scope='Original callback proposal only. Full behavior/branch/memory checks required before original-routine credit. No new wait bound orsupplied game result.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
target=PROJECT/'analysis/renderer_list_completion_translation.json';assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n')
print('Translated651f1e',len(rows),'original instruction rows;no recovery credit yet',flush=True)
