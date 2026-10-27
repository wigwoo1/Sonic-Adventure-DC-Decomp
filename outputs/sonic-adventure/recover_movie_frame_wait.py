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
folder=PROJECT/'analysis/movie-frame-dispatch';folder.mkdir(exist_ok=True)
original=PROJECT/'analysis/movie-frame-resource/8c64c34c_instructions.tsv'
raw=list(csv.DictReader(original.open(),delimiter='\t'));prefix=[r for r in raw if 0x8c64c34c<=int(r['address'],16)<0x8c64c374]
assert len(raw)==2558 and len(prefix)==20 and prefix[-2]['instruction']=='jmp @r0'
with (folder/'8c64c34c_instructions.tsv').open('w',newline='') as stream:
 writer=csv.DictWriter(stream,fieldnames=list(raw[0]),delimiter='\t');writer.writeheader();writer.writerows(prefix)
with (folder/'diagnostics.tsv').open('w',newline='') as stream:
 writer=csv.DictWriter(stream,fieldnames=['address','function','body_bytes','minimum','maximum','completed','error'],delimiter='\t');writer.writeheader();writer.writerow(dict(address='8c64c34c',function='entry_prefix_before_indirect_tail',body_bytes=40,minimum='8c64c34c',maximum='8c64c373',completed='true',error=''))
partition=dict(original_listing_sha256=hashlib.sha256(original.read_bytes()).hexdigest(),original_export_rows=2558,prefix_rows=20,prefix_end_exclusive='0x8c64c374',tail_site='0x8c64c370',delay_slot='0x8c64c372',actual_tail_targets=['0x8c64c374','0x8c64d1a6'],scope='Byte-checked entry prefix before real indirect tail; full original merged Ghidra export preserved. Downstream bodies are separate reached dependencies, not omitted behavior or completed dispatcher proof.')
(folder/'partition.json').write_text(json.dumps(partition,indent=2)+'\n')
groups={'movie-frame-wait':{0x8c051790,0x8c6433e0,0x8c055c4c},'movie-frame-lower':{0x8c6088bc,0x8c608914,0x8c608922,0x8c608c82},'movie-frame-resource':{0x8c608e24,0x8c64ddc0},'movie-frame-dispatch':{0x8c64c34c}}
for folder,included in groups.items():
 path=PROJECT/'analysis'/folder/'diagnostics.tsv';diagnostics.append(path)
 for note in csv.DictReader(path.open(),delimiter='\t'):
  assert note['completed']=='true' and not note['error']
  if int(note['address'],16) in included:notes.append(dict(note,folder=folder))
assert len(notes)==10
functions=[];routines=[]
for note in notes:
 start=int(note['address'],16);listing=PROJECT/'analysis'/note['folder']/f'{start:08x}_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'));end=max(int(r['address'],16)+2 for r in rows)
 body_min=min(int(r['address'],16) for r in rows)
 chosen=BASE_CHOSEN.replace('main-dispatch-model-traversal/',note['folder']+'/')
 marker='assert len(raw)==int(row[\'length\'])==2 and start<=a<end';assert chosen.count(marker)==1
 chosen=chosen.replace(marker,'assert len(raw)==int(row[\'length\'])==2 and body_min<=a<end')
 exec(compile(chosen,str(TEMPLATE),'exec'),globals())
 function,record=recover(start,end,f'sa_movie_frame_wait_{start:08x}',len(rows))
 marker='sa_shared_service call,uint32_t *stop){';assert function.count(marker)==1
 function=function.replace(marker,marker[:-2]+',uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){\n int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;')
 function=function.replace('(void)entry_pr;','(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;')
 for name in ('rd','sx','wr','fload','fstore'):function=function.replace(name+'(m,',name+'(m,bus,&io_status,')
 function=re.sub(r'((?:rd|sx|wr|fload|fstore)\(m,[^;]*\);)',r'\1if(io_status)return io_status;',function)
 function=re.sub(r'(r\[\d+\])=rd\(([^;]+)\);if\(io_status\)return io_status;',r'{uint32_t loaded=rd(\2);if(io_status)return io_status;\1=loaded;}',function)
 guards={0x8c051798:True} if start==0x8c051790 else {}
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
 functions.append(function);routines.append(record)
header=header.replace(header[header.index('/*'):header.index(' */')+3],'/* Original frame wait, queue records, release/submission and display sync.\n * Static C from byte-checked SH-4 instructions; complete explicit state.\n * Missing callees remain reached boundaries; frame wait is research-bounded\n * only under SA_BOUND_RETAIL_STALLS and is not a successful completion.\n */')
functions=[f.replace('(void)gbr;','(void)gbr;(void)valid_access;') for f in functions]
source=PROJECT/'source/sa_movie_frame_wait.c';source.write_text(header+'\n'.join(functions))
files=[PROJECT/'analysis/movie-frame-dispatch/partition.json',PROJECT/'analysis/movie-frame-resource/8c64c34c_instructions.tsv',Path(__file__),EVENT_PARENT,SUBMIT_PARENT,WAIT_PARENT,PARENT,GBR_PARENT,SAMPLING,PREVIOUS,PARENT_TEMPLATE,PROJECT/'recover_draw_polygon_lower.py',PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',TRANSLATOR,TEMPLATE,BINARY,source,PROJECT/'source/vertex_bus.h',PROJECT/'source/sh4_integer_state.h',PROJECT/'source/sh4_integer_state.c',PROJECT/'source/sh4_vector_base.c',PROJECT/'source/sh4_vector_base.h',remainder,remainder_proof]+diagnostics+[Path(r['listing']) for r in routines]+[PROJECT/'recover_boot_file.py']+list((PROJECT/'ghidra').glob('export_movie_frame*.ps1'))
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=routines,instruction_rows=sum(r['instruction_rows'] for r in routines),newly_registered_entries=0,reused_routine_credit=0,excluded_entries={hex(a):why for a,why in excluded.items()},scope='Ten original frame/display/queue/resource proposals with actual callees. Existing canonical entries receive zero additional recovery credit. Missing dependencies remain reached frontiers. Positive RAM-counter wait remains real; research-only stop is incomplete, not asynchronous progress or successful completion.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(PROJECT/'analysis/movie_frame_wait_translation.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated',len(routines),'file proposals /',report['instruction_rows'],'rows')
