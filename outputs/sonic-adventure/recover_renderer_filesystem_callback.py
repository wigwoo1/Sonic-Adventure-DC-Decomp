"""Recover the original gate; delegate its shared tail to the real native worker."""
import csv,hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;ROOT=PROJECT.parents[1]
EVENT_PARENT=PROJECT/'recover_boot_event.py'
exec(compile(EVENT_PARENT.read_text().split('notes=[];diagnostics=[]')[0],str(EVENT_PARENT),'exec'),globals())
full=PROJECT/'analysis/renderer-filesystem-callback/8c5ffcf0_instructions.tsv'
worker=PROJECT/'analysis/boot-file-pump/8c6005c2_instructions.tsv'
rows=list(csv.DictReader(full.open(),delimiter='\t'));shared=list(csv.DictReader(worker.open(),delimiter='\t'))
all_rows={r['address']:r for r in rows};old_rows={r['address']:r for r in shared}
assert len(all_rows)==334 and len(old_rows)==327 and old_rows.keys()<=all_rows.keys()
for pc,row in all_rows.items():
 a=int(pc,16);raw=bytes.fromhex(row['bytes']);assert len(raw)==int(row['length'])==2 and binary[a-0x8c010000:a-0x8c010000+2]==raw
for pc,row in old_rows.items():assert all_rows[pc]['bytes']==row['bytes'] and all_rows[pc]['instruction']==row['instruction'],pc
entry_rows=[r for r in rows if r['address'] not in old_rows]
assert {int(r['address'],16) for r in entry_rows}==set(range(0x8c5ffcf0,0x8c5ffcfe,2))
folder=PROJECT/'analysis/renderer-filesystem-callback-entry';folder.mkdir(exist_ok=True)
listing=folder/'8c5ffcf0_instructions.tsv';assert not listing.exists()
with listing.open('w',newline='') as out:
 writer=csv.DictWriter(out,fieldnames=rows[0].keys(),delimiter='\t');writer.writeheader();writer.writerows(entry_rows)
chosen=BASE_CHOSEN.replace('main-dispatch-model-traversal/','renderer-filesystem-callback-entry/')
exec(compile(chosen,str(TEMPLATE),'exec'),globals())
function,record=recover(0x8c5ffcf0,0x8c5ffcfe,'sa_renderer_filesystem_callback_8c5ffcf0',7)
marker='sa_shared_service call,uint32_t *stop){';assert function.count(marker)==1
function=function.replace(marker,marker[:-2]+',uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){\n int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;')
function=function.replace('(void)entry_pr;','(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;')
for name in ('rd','sx','wr','fload','fstore'):function=function.replace(name+'(m,',name+'(m,bus,&io_status,')
function=re.sub(r'((?:rd|sx|wr|fload|fstore)\(m,[^;]*\);)',r'\1if(io_status)return io_status;',function)
function=re.sub(r'(r\[\d+\])=rd\(([^;]+)\);if\(io_status\)return io_status;',r'{uint32_t loaded=rd(\2);if(io_status)return io_status;\1=loaded;}',function)
header=header.replace(header[header.index('/*'):header.index(' */')+3],'/* Original registered filesystem callback gate5ffcf0.\n * The shared327-row tail executes through the real existing6005c2 worker. */')
source=PROJECT/'source/sa_renderer_filesystem_callback.c';assert not source.exists();source.write_text(header+function)
record.update(registration_requires_complete_behavior=True,shared_tail_entry='0x8c6005c2',shared_tail_rows=327,shared_tail_new_credit=0,canonical_registered_reuse=False)
files=[Path(__file__),EVENT_PARENT,SUBMIT_PARENT,WAIT_PARENT,PARENT,GBR_PARENT,SAMPLING,PREVIOUS,PARENT_TEMPLATE,TRANSLATOR,TEMPLATE,BINARY,source,listing,full,worker,PROJECT/'source/sa_boot_file.c',PROJECT/'source/sa_file_registered_callback.c',PROJECT/'ghidra/export_renderer_filesystem_callback.ps1']
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=[record],instruction_rows=7,full_Ghidra_rows_byte_checked=334,canonical_shared_rows_reused=327,shared_tail_target='0x8c6005c2',newly_registered_entries=0,scope='Complete7-roworiginal gate:context+20==1 returns;otherwiseoriginal unconditionalbranch tail-dispatches theexisting real327-row6005c2worker. FullGhidrabody andoldworkerrows arebyte-checked andretained. No suppliedbackendresult,stub orduplicate shared-tail credit. Generation isnotbehavior qualification.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
target=PROJECT/'analysis/renderer_filesystem_callback_translation.json';assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n');print('Recovered7-rowgate;334originalrowschecked;327sharedrows reused withoutcredit',flush=True)
