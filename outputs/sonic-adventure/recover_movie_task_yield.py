"""Byte-checked earlier initialization and original memory/status dependencies."""
"""Original file lookup/open/read/close proposals; platform progress stays explicit."""
import csv,hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;EVENT_PARENT=PROJECT/'recover_boot_event.py'
exec(compile(EVENT_PARENT.read_text().split('notes=[];diagnostics=[]')[0],str(EVENT_PARENT),'exec'),globals())
remainder=PROJECT/'source/sa_unsigned_remainder.c';remainder_proof=PROJECT/'analysis/unsigned_remainder_verification.json'
assert hashlib.sha256(remainder.read_bytes()).hexdigest()==json.loads(remainder_proof.read_text())['source_sha256']
division=remainder.read_text();division=division[division.index('static void div1_step'):division.index('SA_STAGE_EXPORT int')]
notes=[];diagnostics=[];excluded={}
# Preserve the merged export; partition only at the encoded BRA/delay slot.
original=PROJECT/'analysis/movie-task-yield/8c668d56_instructions.tsv'
common=PROJECT/'analysis/movie-task-yield-common/8c668e1e_instructions.tsv'
raw_rows=list(csv.DictReader(original.open(),delimiter='\t'));common_rows=list(csv.DictReader(common.open(),delimiter='\t'))
prefix_rows=[r for r in raw_rows if 0x8c668d56<=int(r['address'],16)<0x8c668d66]
assert len(raw_rows)==39 and len(prefix_rows)==8 and len(common_rows)==31 and raw_rows==prefix_rows+common_rows
assert prefix_rows[-2]['bytes']=='5ca0' and prefix_rows[-1]['bytes']=='00e4'
folder=PROJECT/'analysis/movie-task-yield-prefix';folder.mkdir(exist_ok=True)
with (folder/'8c668d56_instructions.tsv').open('w',newline='') as f:
 writer=csv.DictWriter(f,fieldnames=list(prefix_rows[0]),delimiter='\t');writer.writeheader();writer.writerows(prefix_rows)
note=dict(next(csv.DictReader((PROJECT/'analysis/movie-task-yield/diagnostics.tsv').open(),delimiter='\t')));note.update(body_bytes='16',minimum='8c668d56',maximum='8c668d65')
with (folder/'diagnostics.tsv').open('w',newline='') as f:
 writer=csv.DictWriter(f,fieldnames=list(note),delimiter='\t');writer.writeheader();writer.writerow(note)
(folder/'partition.json').write_text(json.dumps(dict(original_rows=39,prefix_rows=8,shared_rows=31,unique_rows=39,encoded_tail_branch='0x8c668d62',delay_slot='0x8c668d64',shared_target='0x8c668e1e',shared_rows_credited_once=True,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in (original,common)}),indent=2)+'\n')
groups={'movie-task-yield-prefix':{0x8c668d56},'movie-task-yield-common':{0x8c668e1e},'movie-task-yield':{0x8c668e5c}}
for folder,included in groups.items():
 path=PROJECT/'analysis'/folder/'diagnostics.tsv';diagnostics.append(path)
 for note in csv.DictReader(path.open(),delimiter='\t'):
  assert note['completed']=='true' and not note['error']
  if int(note['address'],16) in included:notes.append(dict(note,folder=folder))
assert len(notes)==3
functions=[];routines=[]
for note in notes:
 start=int(note['address'],16);listing=PROJECT/'analysis'/note['folder']/f'{start:08x}_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'));end=max(int(r['address'],16)+2 for r in rows)
 body_min=min(int(r['address'],16) for r in rows)
 chosen=BASE_CHOSEN.replace('main-dispatch-model-traversal/',note['folder']+'/')
 marker='assert len(raw)==int(row[\'length\'])==2 and start<=a<end';assert chosen.count(marker)==1
 chosen=chosen.replace(marker,'assert len(raw)==int(row[\'length\'])==2 and body_min<=a<end')
 marker_control="        assert not text.startswith('f'),text";assert chosen.count(marker_control)==1
 controls="        if w&0xf0ff==0x400a:return f'integer->mach=r[{n}];'\n        if w&0xf0ff==0x401a:return f'*macl=r[{n}];'\n"
 chosen=chosen.replace(marker_control,controls+marker_control)
 exec(compile(chosen,str(TEMPLATE),'exec'),globals())
 function,record=recover(start,end,f'sa_movie_task_yield_{start:08x}',len(rows))
 marker='sa_shared_service call,uint32_t *stop){';assert function.count(marker)==1
 function=function.replace(marker,marker[:-2]+',uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){\n int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;')
 function=function.replace('(void)entry_pr;','(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;')
 for name in ('rd','sx','wr','fload','fstore'):function=function.replace(name+'(m,',name+'(m,bus,&io_status,')
 function=re.sub(r'((?:rd|sx|wr|fload|fstore)\(m,[^;]*\);)',r'\1if(io_status)return io_status;',function)
 function=re.sub(r'(r\[\d+\])=rd\(([^;]+)\);if\(io_status\)return io_status;',r'{uint32_t loaded=rd(\2);if(io_status)return io_status;\1=loaded;}',function)
 guards={}
 if guards:
  function=function.replace(' int status;uint32_t target;',' unsigned delay_visits=0;(void)delay_visits;\n int status;uint32_t target;',1)
  for pc,active in guards.items():
   marker=f'L{pc:x}: ';assert function.count(marker)==1
   function=function.replace(marker,f'L{pc:x}:\n#ifdef SA_BOUND_RETAIL_STALLS\n *stop=0x{pc:x}u;if((*sr&1u) && ++delay_visits>=2u)return -4;\n#endif\n ',1)
  record['bounded_research_waits']=[dict(pc=hex(pc),continue_when_t=active,max_continue_tests=2,status=-4) for pc,active in guards.items()]
 record['registration_requires_complete_behavior']=True
 record['discontiguous_before_entry_rows']=[r['address'] for r in rows if int(r['address'],16)<start]
 addresses=sorted(int(r['address'],16) for r in rows);ranges=[]
 for address in addresses:
  if ranges and ranges[-1][1]==address:ranges[-1][1]+=2
  else:ranges.append([address,address+2])
 record['retail_instruction_ranges']=[dict(start=hex(a),end_exclusive=hex(b),sha256=hashlib.sha256(binary[a-0x8c010000:b-0x8c010000]).hexdigest()) for a,b in ranges]
 if start==0x8c668e1e:
  name='sa_movie_task_yield_8c668e1e';args='m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr'
  signature=function[:function.index('{')].strip();run_signature=signature.replace('SA_STAGE_EXPORT int '+name,'static int task_yield_run').replace('const uint32_t *vbr)', 'const uint32_t *vbr,uint32_t resume_pc)')
  function=run_signature+'{'+function[function.index('{')+1:]
  function=function.replace(' goto L8c668e1e;', ' if(resume_pc==0x8c668e48u)goto L8c668e48;goto L8c668e1e;',1)
  marker='if(*pr!=entry_pr)return -3;';assert function.count(marker)==1;function=function.replace(marker,'')
  marker='r[15]+=4;return 0; /* rts */';assert function.count(marker)==1
  function=function.replace(marker,'r[15]+=4;if(*pr==entry_pr)return 0;if(!call)return -2;return call(ctx,*pr,m,f,sr,r,pr); /* rts */')
  function+='\n'+signature+'{return task_yield_run('+args+',0);}'
  function+='\n'+signature.replace(name,'sa_movie_task_yield_resume_8c668e48')+'{return task_yield_run('+args+',0x8c668e48u);}'
 functions.append(function);routines.append(record)
header=header.replace(header[header.index('/*'):header.index(' */')+3],'/* Original task termination/yield wrappers and shared save/restore path.\n * Static C from byte-checked SH-4 instructions; complete explicit state.\n * Missing callees remain reached boundaries; no callback result is substituted.\n * Context and interrupt entry remain explicit platform state.\n */')
functions=[f.replace('(void)gbr;','(void)gbr;(void)valid_access;(void)r;') for f in functions]
source=PROJECT/'source/sa_movie_task_yield.c';source.write_text(header+'\n'.join(functions))
files=[Path(__file__),EVENT_PARENT,SUBMIT_PARENT,WAIT_PARENT,PARENT,GBR_PARENT,SAMPLING,PREVIOUS,PARENT_TEMPLATE,PROJECT/'recover_draw_polygon_lower.py',PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',TRANSLATOR,TEMPLATE,BINARY,source,PROJECT/'source/vertex_bus.h',PROJECT/'source/sh4_integer_state.h',PROJECT/'source/sh4_integer_state.c',PROJECT/'source/sh4_vector_base.c',PROJECT/'source/sh4_vector_base.h',remainder,remainder_proof]+diagnostics+[Path(r['listing']) for r in routines]+[PROJECT/'recover_boot_file.py',original,common,PROJECT/'analysis/movie-task-yield-prefix/partition.json']+list((PROJECT/'ghidra').glob('export_movie_task_yield*.ps1'))
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=routines,instruction_rows=sum(r['instruction_rows'] for r in routines),newly_registered_entries=0,reused_routine_credit=0,excluded_entries={hex(a):why for a,why in excluded.items()},scope='Three byte-checked task termination/yield proposals41unique rows;shared31row implementation credited once. Real context save/restore and continuation handling require behavior qualification;no CPU/callback substitutes.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(PROJECT/'analysis/movie_task_yield_translation.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated',len(routines),'file proposals /',report['instruction_rows'],'rows')
