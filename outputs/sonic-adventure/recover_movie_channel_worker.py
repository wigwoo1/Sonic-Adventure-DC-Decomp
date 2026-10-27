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
groups={'movie-channel-worker':{0x8c64790e,0x8c647912,0x8c647916,0x8c64791a,0x8c64791e,0x8c647922,0x8c647926,0x8c647980,0x8c66086a}}
for folder,included in groups.items():
 path=PROJECT/'analysis'/folder/'diagnostics.tsv';diagnostics.append(path)
 for note in csv.DictReader(path.open(),delimiter='\t'):
  assert note['completed']=='true' and not note['error']
  if int(note['address'],16) in included:notes.append(dict(note,folder=folder))
assert len(notes)==9
# Decode every signed table offset from the original executable, retaining RAM loads.
table_base=0x8c6479c8;braf=0x8c6479b4
table_offsets=[int.from_bytes(binary[table_base-0x8c010000+2*i:table_base-0x8c010000+2*i+2],"little",signed=True) for i in range(21)]
computed_targets={braf+4+offset for offset in table_offsets}
assert len(computed_targets)==19
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
 cfg="        if w==0xB or w>>12 in (0xA,0xB) or w&0xF0FF in (0x400B,0x402B):"
 assert chosen.count(cfg)==1
 chosen=chosen.replace(cfg,"        if w&0xf0ff==0x0023:\n            assert a==braf and a+2 in rows;seen.add(a+2);todo.extend(computed_targets)\n        el"+cfg.lstrip())
 emitter="        elif w>>12==0xA:"
 assert chosen.count(emitter)==1
 dynamic="""        elif w&0xf0ff==0x0023:
            assert a==braf
            code=f'target=0x{a+4:x}u+r[{w>>8&15}];'+simple(a+2)+'switch(target){'+''.join(f'case 0x{v:x}u:goto L{v:x};' for v in sorted(computed_targets))+'default:if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);}'
            slots.add(a+2);terminal=True
"""
 chosen=chosen.replace(emitter,dynamic+emitter)
 exec(compile(chosen,str(TEMPLATE),'exec'),globals())
 function,record=recover(start,end,f'sa_movie_channel_worker_{start:08x}',len(rows))
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
 if start==0x8c647980:
  signature=function[:function.index('{')].strip()
  run_signature=signature.replace('SA_STAGE_EXPORT int sa_movie_channel_worker_8c647980','static int channel_worker_run').replace('const uint32_t *vbr)','const uint32_t *vbr,uint32_t resume_pc)')
  function=run_signature+'{'+function[function.index('{')+1:]
  marker=' goto L8c647980;';assert function.count(marker)==1
  function=function.replace(marker,' if(resume_pc==0x8c647982u)goto L8c647982;if(resume_pc==0x8c647b74u)goto L8c647b74;goto L8c647980;',1)
  args='m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr'
  function+='\n'+signature+'{return channel_worker_run('+args+',0);}'
  for pc in (0x8c647982,0x8c647b74):
   function+='\n'+signature.replace('sa_movie_channel_worker_8c647980',f'sa_movie_channel_worker_resume_{pc:08x}')+'{return channel_worker_run('+args+f',0x{pc:x}u);}}'
  record.update(shared_entry='0x8c647982',yield_continuation='0x8c647b74',table_base=hex(table_base),braf_pc=hex(braf),computed_table_targets=[hex(braf+4+offset) for offset in table_offsets],computed_unique_targets=[hex(v) for v in sorted(computed_targets)])
 functions.append(function);routines.append(record)
header=header.replace(header[header.index('/*'):header.index(' */')+3],'/* Original channel selectors, command worker and yield thunk; semantics inferred.\n * Static C from byte-checked SH-4 instructions; complete explicit state.\n * Missing callees remain reached boundaries; no callback result is substituted.\n * Context and interrupt entry remain explicit platform state.\n */')
functions=[f.replace('(void)gbr;','(void)gbr;(void)valid_access;(void)r;') for f in functions]
source=PROJECT/'source/sa_movie_channel_worker.c';source.write_text(header+'\n'.join(functions))
files=[Path(__file__),EVENT_PARENT,SUBMIT_PARENT,WAIT_PARENT,PARENT,GBR_PARENT,SAMPLING,PREVIOUS,PARENT_TEMPLATE,PROJECT/'recover_draw_polygon_lower.py',PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',TRANSLATOR,TEMPLATE,BINARY,source,PROJECT/'source/vertex_bus.h',PROJECT/'source/sh4_integer_state.h',PROJECT/'source/sh4_integer_state.c',PROJECT/'source/sh4_vector_base.c',PROJECT/'source/sh4_vector_base.h',remainder,remainder_proof]+diagnostics+[Path(r['listing']) for r in routines]+[PROJECT/'recover_boot_file.py']+[PROJECT/'ghidra/export_movie_channel_worker.ps1']
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=routines,instruction_rows=sum(r['instruction_rows'] for r in routines),newly_registered_entries=0,reused_routine_credit=0,excluded_entries={hex(a):why for a,why in excluded.items()},scope='Nine original channel entry/worker/yield-thunk proposals272rows. All21 command table slots and19targets preserved. Shared init and yield continuation do not duplicate credit. Decoder callees remain real missing boundaries; complete worker coverage requires further recovery. No game-service results or task state substitutes.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(PROJECT/'analysis/movie_channel_worker_translation.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated',len(routines),'file proposals /',report['instruction_rows'],'rows')
